#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <strings.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#endif
#include <mysql.h>
#include <time.h>
#include "db.h"
#include "actor.h"
#include "gameproc.h"
#include "define.h"

extern int g_tick;

MYSQL *myData = NULL;
MYSQL *myGame = NULL;
MYSQL *myLog = NULL;

MYSQL **g_pmyGame = NULL;

extern SConfig g_Config;
extern int g_city_maxcount;

char g_batchsql[BATCHSQL_MAXSIZE] = { 0 };

int db_init()
{
	if( (myData = mysql_init((MYSQL*) 0)) == NULL )
	{
		printf_msg("Failed to init mysql\n");
		return -1;
	}
	if( !mysql_real_connect( myData,g_Config.datadb_host,g_Config.datadb_user,g_Config.datadb_pass,g_Config.datadb_database,0,NULL,0 ) )
	{
		printf_msg("Failed to connect to data database: Error: %s\n", mysql_error(myData));
		return -1;
	}
	mysql_set_character_set( myData, "utf8" );

	if( (myGame = mysql_init((MYSQL*) 0)) == NULL )
	{
		printf_msg("Failed to init mysql\n");
		return -1;
	}
	if( !mysql_real_connect( myGame,g_Config.gamedb_host,g_Config.gamedb_user,g_Config.gamedb_pass,g_Config.gamedb_database,0,NULL,0 ) )
	{
		printf_msg("Failed to connect to game database: Error: %s\n", mysql_error(myGame));
		return -1;
	}
	mysql_set_character_set( myGame, "utf8" );

	if( (myLog = mysql_init((MYSQL*) 0)) == NULL )
	{
		printf_msg("Failed to init mysql\n");
		return -1;
	}
	if( !mysql_real_connect( myLog,g_Config.logdb_host,g_Config.logdb_user,g_Config.logdb_pass,g_Config.logdb_database,0,NULL,0 ) )
	{
		printf_msg("Failed to connect to log database: Error: %s\n", mysql_error(myLog));
		return -1;
	}
	mysql_set_character_set( myLog, "utf8" );
	return 0;
}

int db_reconnect_game()
{
	MYSQL myMem;
	memcpy( &myMem, myGame, sizeof(MYSQL) );
	myGame = mysql_init((MYSQL*) 0);
	if( !mysql_real_connect( myGame,g_Config.gamedb_host,g_Config.gamedb_user,g_Config.gamedb_pass,g_Config.gamedb_database,0,NULL,0 ) )
	{
		printf_msg("Failed to connect to game database: Error: %s\n", mysql_error(myGame));
		memcpy( myGame, &myMem, sizeof(MYSQL) );
		return -1;
	}
	mysql_set_character_set( myGame, "utf8" );
	return 0;
}

int db_reconnect_log()
{
	MYSQL myMem;
	memcpy( &myMem, myLog, sizeof(MYSQL) );
	myLog = mysql_init((MYSQL*) 0);
	if( !mysql_real_connect( myLog,g_Config.logdb_host,g_Config.logdb_user,g_Config.logdb_pass,g_Config.logdb_database,0,NULL,0 ) )
	{
		printf_msg("Failed to connect to log database: Error: %s\n", mysql_error(myLog));
		memcpy( myLog, &myMem, sizeof(MYSQL) );
		return -1;
	}
	mysql_set_character_set( myLog, "utf8" );
	return 0;
}
int db_opendata()
{
	if ( myData )
		return -1;
	if( (myData = mysql_init((MYSQL*) 0)) == NULL )
	{
		printf_msg("Failed to init mysql\n");
		return -1;
	}
	if( !mysql_real_connect( myData,g_Config.datadb_host,g_Config.datadb_user,g_Config.datadb_pass,g_Config.datadb_database,0,NULL,0 ) )
	{
		printf_msg("Failed to connect to data database: Error: %s\n", mysql_error(myData));
		return -1;
	}
	mysql_set_character_set( myData, "utf8" );
	return 0;
}

int db_closedata()
{
	mysql_close( myData ) ;
	myData = NULL;
	return 0;
}

int db_closegame()
{
	mysql_close( myGame );
	mysql_close( myLog );
	return 0;
}

char *db_escape( const char *pStr, char *pBuf, int size )
{
//	mysql_real_escape_string( myData, pBuf, pStr, (unsigned long)(size<=0?strlen(pStr)+1:size) );
	mysql_real_escape_string( myGame, pBuf, pStr, (unsigned long)(size<=0?strlen(pStr):size) );
	return pBuf;
}

int db_query( FILE *fp, const char *pSql )
{
	if ( fp )
	{
		fprintf( fp, "%s;\n", pSql );
	}
	else if ( mysql_query( myGame, pSql ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", pSql );
		return -1;
	}
	return 0;
}



Mthr *g_pthr_dbwork[DBWORK_THREAD_NUM];
BOOL g_bDBEnd;
BOOL g_bDBInit = FALSE;

// 执行队列
SDBQueue *g_dbqueue;
int g_nDBQueueMaxNum = 0;
int g_nDBQueueHead;
int g_nDBQueueTail;
ThrMux	*g_db_mmux;

// 完成队列
SDBQueueComplete *g_dbqueue_complete;
int g_nDBQueueCompleteMaxNum = 0;
int g_nDBQueueCompleteHead;
int g_nDBQueueCompleteTail;
ThrMux	*g_dbcomplete_mmux;

void dbwork_process( int ptr )
{
	int index = ptr;
	char szSql[MAX_DBQUEUE_BUFF] = {0};
	char cmd = 0;
	int value = 0;
	if( (g_pmyGame[index] = mysql_init( (MYSQL*)0 )) == NULL )
	{
		printf_msg( "Failed to init mysql\n" );
		return;
	}
	if( !mysql_real_connect( g_pmyGame[index], g_Config.gamedb_host, g_Config.gamedb_user, g_Config.gamedb_pass, g_Config.gamedb_database, 0, NULL, 0 ) )
	{
		printf_msg( "Failed to connect to game database: Error: %s\n", mysql_error( g_pmyGame[index] ) );
		return;
	}
	mysql_set_character_set( g_pmyGame[index], "utf8" );

	char reconnect_flag = 0;
	BOOL bDBEnd = FALSE;
	while( !bDBEnd )
	{
		// 锁住登录的队列缓冲
		mmux_lock( g_db_mmux );
		while( g_nDBQueueTail == g_nDBQueueHead )
		{
			mcond_wait( g_pthr_dbwork[index], g_db_mmux );
			if( g_bDBEnd )
			{
				mmux_unlock( g_db_mmux );
				return;
			}
		}

		// 从队列中取出一项
		strncpy( szSql, g_dbqueue[g_nDBQueueHead].sql, MAX_DBQUEUE_BUFF-1 );
		cmd = g_dbqueue[g_nDBQueueHead].cmd;
		value = g_dbqueue[g_nDBQueueHead].value;

		// 头部步进
		g_nDBQueueHead++;
		if( g_nDBQueueHead >= g_nDBQueueMaxNum )
		{
			g_nDBQueueHead = 0;
		}
		mmux_unlock( g_db_mmux );

		reconnect_flag = 0;
RE_MYSQLQUERY:
		//
		if ( cmd == DBWORK_CMD_MAIL_INSERT || cmd == DBWORK_CMD_TALK_CACHE || cmd == DBWORK_CMD_NORMAL )
		{
			if ( mysql_query( g_pmyGame[index], szSql ) )
			{
				printf_msg( "Query failed (%s)\n", mysql_error( g_pmyGame[index] ) );
				//write_gamelog( "%s", szSql );
				if ( reconnect_flag )
				{
#ifdef WIN32
					Sleep( 5000 );
#else
					sleep( 5 );
#endif
				}
				if ( mysql_ping( g_pmyGame[index] ) != 0 )
				{
					g_pmyGame[index] = mysql_init( (MYSQL*)0 );
					if ( !mysql_real_connect( g_pmyGame[index], g_Config.gamedb_host, g_Config.gamedb_user, g_Config.gamedb_pass, g_Config.gamedb_database, 0, NULL, 0 ) )
						printf_msg( "Failed to connect to game database: Error: %s\n", mysql_error( g_pmyGame[index] ) );
					else
						mysql_set_character_set( g_pmyGame[index], "utf8" );

					reconnect_flag = 1;
					if ( g_bDBEnd )
					{
						bDBEnd = TRUE;
						return;
					}
					goto RE_MYSQLQUERY;
				}
			}
		}

		// 执行完毕函数
		if ( cmd == DBWORK_CMD_MAIL_INSERTCOMPLETE )
		{
			dbwork_addcomplete( cmd, value );
		}
	}
	// end
}

void dbwork_initqueue()
{
	g_nDBQueueMaxNum = g_city_maxcount / 3;
	g_db_mmux = mmux_open();
	g_nDBQueueHead = 0;
	g_nDBQueueTail = 0;
	g_dbqueue = (SDBQueue *)malloc( sizeof(SDBQueue)*g_nDBQueueMaxNum );
	memset( g_dbqueue, 0, sizeof(SDBQueue)*g_nDBQueueMaxNum );
	printf_msg( "DBWork  maxcount=%d  memory=%0.2fMB\n", g_nDBQueueMaxNum, (sizeof(SDBQueue)* g_nDBQueueMaxNum) / 1024.0 / 1024.0 );

	g_nDBQueueCompleteMaxNum = g_city_maxcount / 3;
	g_dbcomplete_mmux = mmux_open();
	g_nDBQueueCompleteHead = 0;
	g_nDBQueueCompleteTail = 0;
	g_dbqueue_complete = (SDBQueueComplete *)malloc( sizeof(SDBQueueComplete)*g_nDBQueueCompleteMaxNum );
	memset( g_dbqueue_complete, 0, sizeof(SDBQueueComplete)*g_nDBQueueCompleteMaxNum );
}

int dbwork_start()
{
	g_bDBEnd = FALSE;
	dbwork_initqueue();

	if ( g_pmyGame == NULL )
		g_pmyGame = (MYSQL **)malloc( sizeof(MYSQL *)*DBWORK_THREAD_NUM );
	for ( int tmpi = 0; tmpi < DBWORK_THREAD_NUM; tmpi++ )
	{
		g_pthr_dbwork[tmpi] = mthr_open();
		mcond_init( g_pthr_dbwork[tmpi] );
		if ( mthr_create( g_pthr_dbwork[tmpi], (void*)dbwork_process, (void *)tmpi ) < 0 )
		{
			return -1;
		}
	}
	g_bDBInit = TRUE;
	return 0;
}

int dbwork_stop()
{
	g_bDBEnd = TRUE;
	for( int tmpi = 0; tmpi < DBWORK_THREAD_NUM; tmpi++ )
	{
		mthr_join( g_pthr_dbwork[tmpi] );
		mthr_close( g_pthr_dbwork[tmpi] );
		mysql_close( g_pmyGame[tmpi] );
	}
	return 0;
}


// 添加数据到队列
// 返回-1表示队列满了，无法加入了
int dbwork_addsql( const char *pSql, char cmd, int value )
{
	if ( g_bDBInit == FALSE )
		return -1;
	int queue_tail = 0;
	while ( 1 )
	{
		// 锁住队列
		mmux_lock( g_db_mmux );
		// 获取尾部
		queue_tail = g_nDBQueueTail + 1;
		if ( queue_tail >= g_nDBQueueMaxNum )
		{
			queue_tail = 0;
		}

		// 当队列已经满了
		if ( g_nDBQueueHead == queue_tail )
		{
			mmux_unlock( g_db_mmux );
#ifdef WIN32
			Sleep( 50 );
#else
			usleep( 50000 );
#endif
		}
		else
		{
			break;
		}
	}

	// 将数据复制进来
	strncpy( g_dbqueue[g_nDBQueueTail].sql, pSql, MAX_DBQUEUE_BUFF-1 );
	g_dbqueue[g_nDBQueueTail].cmd = cmd;
	g_dbqueue[g_nDBQueueTail].value = value;

	// 尾部步进
	g_nDBQueueTail = queue_tail;

	// 解锁队列
	mmux_unlock( g_db_mmux );
	// 通知线程开始工作
	for( int tmpi = 0; tmpi < DBWORK_THREAD_NUM; tmpi++ )
		mcond_broadcast( g_pthr_dbwork[tmpi] );
	return 0;
}

void db_test()
{
	dbwork_start();
	for( int tmpi = 0; tmpi < 100; tmpi++ )
	{
		if( dbwork_addsql( "insert into ...", 0, 0 ) == -1 )
		{
#ifdef WIN32
			Sleep( 50 );
#else
			usleep( 50000 );
#endif
		}
	}
	dbwork_stop( );
}

int dbwork_addcomplete( char cmd, int value )
{
	if ( g_bDBInit == FALSE )
		return -1;

	int queue_tail = 0;

	// 锁住队列
	mmux_lock( g_dbcomplete_mmux );
	// 获取尾部
	queue_tail = g_nDBQueueCompleteTail + 1;
	if ( queue_tail >= g_nDBQueueCompleteMaxNum )
	{
		queue_tail = 0;
	}

	// 当队列已经满了
	if ( g_nDBQueueCompleteHead == queue_tail )
	{
		mmux_unlock( g_dbcomplete_mmux );
		return -1;
	}

	// 将数据复制进来
	g_dbqueue_complete[g_nDBQueueCompleteTail].cmd = cmd;
	g_dbqueue_complete[g_nDBQueueCompleteTail].value = value;

	// 尾部步进
	g_nDBQueueCompleteTail = queue_tail;

	// 解锁队列
	mmux_unlock( g_dbcomplete_mmux );
	return 0;
}

void dbwork_fetchcomplete()
{
	char cmd = 0;
	int value = 0;
	while ( 1 )
	{
		// 锁住队列缓冲
		mmux_lock( g_dbcomplete_mmux );
		if ( g_nDBQueueCompleteTail == g_nDBQueueCompleteHead )
		{
			mmux_unlock( g_dbcomplete_mmux );
			return;
		}

		// 从队列中取出一项
		cmd = g_dbqueue_complete[g_nDBQueueCompleteHead].cmd;
		value = g_dbqueue_complete[g_nDBQueueCompleteHead].value;

		// 头部步进
		g_nDBQueueCompleteHead++;
		if ( g_nDBQueueCompleteHead >= g_nDBQueueCompleteMaxNum )
		{
			g_nDBQueueCompleteHead = 0;
		}
		mmux_unlock( g_dbcomplete_mmux );

		if ( cmd == DBWORK_CMD_MAIL_INSERTCOMPLETE )
		{
			//mailqueue_insert_complete();
		}	
	}
}
