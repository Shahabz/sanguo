#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <signal.h>
#include <syslog.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/ipc.h>   
#include <sys/msg.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include "netserver.h"

#include "mytimer.h"
#include "netfunc.h"
#include "gameproc.h"
#include "utils.h"
#include "define.h"
#include "server_netsend_auto.h"
#include "script.h"
#include "mem_dump.h"

const char *GetRootPath();

extern SConfig g_Config;
extern char g_NotPrintMsg;
int fps_limit = 30;

char g_szWorkPath[MAX_PATH];

// 设置当前服务器状态，用于鉴定当前服务器状态
// 0正常运行 1~N 启动中 -1启动失败 -10开始关闭 -11关闭失败 没有文件是关闭状态
int serv_setstat( int stat )
{
	char szStatFile[MAX_PATH];
	sprintf( szStatFile, "%s/.stat", GetRootPath() );

	FILE *fp;
	fp = fopen( szStatFile, "wb" );
	if( fp == NULL )
		return -1;

	fprintf( fp, "%d\r\n", stat );
	fclose( fp );
	return 0;
}
void serv_rmstat()
{
	char szStatFile[MAX_PATH];
	sprintf( szStatFile, "%s/.stat", GetRootPath( ) );
#ifdef WIN32
	( void )_unlink( szStatFile );
#else
	(void)unlink( szStatFile );
#endif
}

#ifndef WIN32
#define LOCKFILE "nj.pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

int lockfile( int ff )
{
	struct flock fl;
	fl.l_type = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;
	return ( fcntl(ff,F_SETLK,&fl) );
}

void already_running()
{
	int fd;
	char buf[16];
	char szLockFile[MAX_PATH];

	sprintf(szLockFile, "%s/%s", GetRootPath(), LOCKFILE );

	fd = open( szLockFile,O_RDWR|O_CREAT,LOCKMODE );
	if( fd < 0 )
	{
		printf_msg( "already running.[1]\n" );
		exit(0);
	}

	if( lockfile(fd) < 0 )
	{
		if( errno == EACCES || errno == EAGAIN )
		{
			close( fd );
			printf_msg( "already running.[2]\n" );
			exit(0);
		}
		printf_msg( "already running.[3]\n" );
		exit(0);
	}
	ftruncate( fd, 0 );

	sprintf( buf,"%d", getpid() );
	write( fd, buf, strlen(buf) );
}

int getmypid()
{
	int fd;
	int size;
	char buf[16];
	char szLockFile[MAX_PATH];

	sprintf(szLockFile, "%s/%s", GetRootPath(), LOCKFILE );

	fd = open( szLockFile,O_RDWR|O_CREAT,LOCKMODE );
	if( fd < 0 )
		return -1;

	if( lockfile(fd) < 0 )
	{
		if( errno == EACCES || errno == EAGAIN )
		{
			size = read( fd, buf, 16 );
			buf[size] = 0;
			close(fd);
			return atoi(buf);
		}
		size = read( fd, buf, 16 );
		buf[size] = 0;
		close(fd);
		return atoi(buf);
	}
	return -1;
}

// linux 消息队列
struct linuxmsg_info
{
	long int msg_type;
};
#define LINUXMSG_KEY 198700
int linuxmsg_send( int type )
{
	// 检查消息队列是否存在,不存在就创建
	int linuxmsgid = msgget( LINUXMSG_KEY, IPC_EXCL ); 
	if( linuxmsgid < 0 )
	{
		linuxmsgid = msgget( LINUXMSG_KEY, 0666 | IPC_CREAT );  
		if( linuxmsgid < 0 )  
		{  
			printf_msg( "msgget failed with error: %d\n", errno );  
			return -1;
		}  
	}

	struct linuxmsg_info msginfo;
	msginfo.msg_type = type;
	if ( msgsnd( linuxmsgid, &msginfo,sizeof(struct linuxmsg_info), IPC_NOWAIT ) < 0 ) 
	{
		printf_msg( "msgsnd() write msg failed,errno=%d[%s]\n", errno, strerror( errno ) );
		return -1;
	}

	printf_msg( "linuxmsg_send linuxmsgid:%d\n", linuxmsgid );
	return 0;
}
int linuxmsg_recv()
{
	int linuxmsgid = msgget( LINUXMSG_KEY, IPC_EXCL ); 
	if( linuxmsgid < 0 ) 
	{
		//printf_msg( "linuxmsg_recv linuxmsgid:%d\n", linuxmsgid );
		return -1;
	}
	
	struct linuxmsg_info msginfo;
	if ( msgrcv( linuxmsgid, &msginfo, sizeof(struct linuxmsg_info), 0, IPC_NOWAIT ) == -1 )
	{
		return -1;
	}

	printf_msg( "linuxmsg_recv msginfo.msg_type:%d\n", msginfo.msg_type );
	if ( msginfo.msg_type == 1 )
	{
		int tmpi = lua_reload( -1 );
		printf_msg( "lua_reload ThisTimer:%d\n", tmpi );
		write_gamelog( "lua_reload ThisTimer:%d", tmpi );
	}
	else if ( msginfo.msg_type == 2 )
	{
		process_dbreload();
		printf_msg( "process_dbreload\n" );
		write_gamelog( "process_dbreload" );
	}

	// 用完就删除消息队列
	if( msgctl( linuxmsgid, IPC_RMID, 0 ) == -1 )
	{  
		printf_msg( "msgctl(IPC_RMID) failed,errno=%d[%s]\n", errno, strerror( errno ) );

		//char cmdMsg[64]={0};
		//sprintf( cmdMsg, "ipcrm -Q %d", LINUXMSG_KEY );
		//system( cmdMsg );
	}  
	return 0;
}
#endif

int configprocess( const char *pSign, const char *pValue )
{
	if( strcmp( pSign, "SERVER_HOST" ) == 0 )
	{
		g_Config.ipaddress = inet_addr( pValue );
	}
	else if( strcmp( pSign, "SERVER_PORT" ) == 0 )
	{
		g_Config.port = atoi(pValue);
	}
	else if( strcmp( pSign, "USERSRV_HOST" ) == 0 )
	{
		strcpy( g_Config.user_host, pValue );
		g_Config.user_ipaddress = inet_addr( pValue );
	}
	else if( strcmp( pSign, "USERSRV_PORT" ) == 0 )
	{
		g_Config.user_port = atoi(pValue);
	}
	else if( strcmp( pSign, "GLOBALSRV_HOST" ) == 0 )
	{
		g_Config.global_ipaddress = inet_addr( pValue );
	}
	else if( strcmp( pSign, "GLOBALSRV_PORT" ) == 0 )
	{
		g_Config.global_port = atoi( pValue );
	}
	else if( strcmp( pSign, "DSRV_HOST" ) == 0 )
	{
		g_Config.ds_ipaddress = inet_addr( pValue );
	}
	else if( strcmp( pSign, "DSRV_PORT" ) == 0 )
	{
		g_Config.ds_port = atoi(pValue);
	}
	else if( strcmp( pSign, "DATADB_HOST" ) == 0 )
	{
		strcpy( g_Config.datadb_host, pValue );
	}
	else if( strcmp( pSign, "DATADB_USER" ) == 0 )
	{
		strcpy( g_Config.datadb_user, pValue );
	}
	else if( strcmp( pSign, "DATADB_PASS" ) == 0 )
	{
		strcpy( g_Config.datadb_pass, pValue );
	}
	else if( strcmp( pSign, "DATADB_DATABASE" ) == 0 )
	{
		strcpy( g_Config.datadb_database, pValue );
	}
	else if( strcmp( pSign, "GAMEDB_HOST" ) == 0 )
	{
		strcpy( g_Config.gamedb_host, pValue );
	}
	else if( strcmp( pSign, "GAMEDB_USER" ) == 0 )
	{
		strcpy( g_Config.gamedb_user, pValue );
	}
	else if( strcmp( pSign, "GAMEDB_PASS" ) == 0 )
	{
		strcpy( g_Config.gamedb_pass, pValue );
	}
	else if( strcmp( pSign, "GAMEDB_DATABASE" ) == 0 )
	{
		strcpy( g_Config.gamedb_database, pValue );
	}
	else if( strcmp( pSign, "LOGDB_HOST" ) == 0 )
	{
		strcpy( g_Config.logdb_host, pValue );
	}
	else if( strcmp( pSign, "LOGDB_USER" ) == 0 )
	{
		strcpy( g_Config.logdb_user, pValue );
	}
	else if( strcmp( pSign, "LOGDB_PASS" ) == 0 )
	{
		strcpy( g_Config.logdb_pass, pValue );
	}
	else if( strcmp( pSign, "LOGDB_DATABASE" ) == 0 )
	{
		strcpy( g_Config.logdb_database, pValue );
	}
	else if ( strcmp( pSign, "REDIS_HOST" ) == 0 )
	{
		strcpy( g_Config.redis_host, pValue );
	}
	else if ( strcmp( pSign, "REDIS_PORT" ) == 0 )
	{
		g_Config.redis_port = atoi( pValue );
	}
	else if( strcmp( pSign, "MAX_CONNECTION" ) == 0 )
	{
#ifdef WIN32
		g_Config.max_connection = atoi(pValue);
#else
		g_Config.max_connection = atoi(pValue);
#endif
	}
	else if ( strcmp( pSign, "MAX_CITYCOUNT" ) == 0 )
	{
		g_Config.max_citycount = atoi( pValue );
	}
	else if ( strcmp( pSign, "MAX_MAPUNITCOUNT" ) == 0 )
	{
		g_Config.max_mapunitcount = atoi( pValue );
	}
	else if ( strcmp( pSign, "MAX_CLUBCOUNT" ) == 0 )
	{
		g_Config.max_clubcount = atoi( pValue );
	}
	else if( strcmp( pSign, "MIN_VER1" ) == 0 )
	{
		g_Config.minver.ss.m1 = atoi(pValue);
	}
	else if( strcmp( pSign, "MIN_VER2" ) == 0 )
	{
		g_Config.minver.ss.m2 = atoi(pValue);
	}
	else if( strcmp( pSign, "MIN_VER3" ) == 0 )
	{
		g_Config.minver.ss.m3 = atoi(pValue);
	}
	else if( strcmp( pSign, "AREA_CODE" ) == 0 )
	{
		g_Config.area_code = atoi(pValue);
	}
	else if( strcmp( pSign, "SERVER_CODE" ) == 0 )
	{
		g_Config.server_code = atoi(pValue);
	}
	else if( strcmp( pSign, "SERVER_NAME" ) == 0 )
	{
		strcpy( g_Config.server_name, pValue );
	}
	else if ( strcmp( pSign, "SERVER_START_DATE" ) == 0 )
	{
		g_Config.server_start_date = atoi( pValue );
	}
	else if ( strcmp( pSign, "SERVER_START_HOUR" ) == 0 )
	{
		g_Config.server_start_hour = atoi( pValue );
	}
	else if ( strcmp( pSign, "SERVER_START_MIN" ) == 0 )
	{
		g_Config.server_start_min = atoi( pValue );
	}
	else if ( strcmp( pSign, "SERVER_EXIT_WEEK" ) == 0 )
	{
		g_Config.server_exit_week = atoi( pValue );
	}
	else if ( strcmp( pSign, "SERVER_EXIT_HOUR" ) == 0 )
	{
		g_Config.server_exit_hour = atoi( pValue );
	}
	else if ( strcmp( pSign, "SERVER_EXIT_MIN" ) == 0 )
	{
		g_Config.server_exit_min = atoi( pValue );
	}
	else if ( strcmp( pSign, "SHOWFLAG" ) == 0 )
	{
		g_Config.showflag = atoi( pValue );
	}
	else if ( strcmp( pSign, "SHOWCDKEY" ) == 0 )
	{
		g_Config.showcdkey = atoi( pValue );
	}
	else if ( strcmp( pSign, "SHOWVIP" ) == 0 )
	{
		g_Config.showvip = atoi( pValue );
	}
	else if ( strcmp( pSign, "SHOWMCARD" ) == 0 )
	{
		g_Config.showmcard = atoi( pValue );
	}
	else if ( strcmp( pSign, "NOPRINT" ) == 0 )
	{
		g_Config.noprint = atoi( pValue );
		g_NotPrintMsg = atoi( pValue );;
	}
	else if ( strcmp( pSign, "USE_ZH" ) == 0 )
	{
		g_Config.use_zh = atoi( pValue );
	}
	else if( strcmp( pSign, "USEGATE" ) == 0 )
	{
		g_Config.usegate = atoi( pValue );
	}
	else if( strcmp( pSign, "WEBLOG_HOST" ) == 0 )
	{
		strcpy( g_Config.log_host, pValue );
	}
	else if( strcmp( pSign, "WEBLOG_PORT" ) == 0 )
	{
		g_Config.log_port = atoi( pValue );
	}
	else if( strcmp( pSign, "WEBLOG_PLATFORM" ) == 0 )
	{
		strcpy( g_Config.log_platform, pValue );
	}
	else if ( strcmp( pSign, "USENETLOG" ) == 0 )
	{
		g_Config.usenetlog = atoi( pValue );
	}

	return 0;
}

// 计时器发送指令
void target_incrementor()
{
	char ch = 0;
	netsend_timer_S( -2, 0, &ch );
}

// 这里截获崩溃吧
void actors_on_core(); // 定义在actor.h
void sig_process( int signo )
{
#ifndef WIN32
	// 先恢复信号为缺省，免得死循环进入了
    signal( SIGSEGV, SIG_DFL );
    signal( SIGBUS, SIG_DFL );
    signal( SIGFPE, SIG_DFL );
	signal( SIGABRT, SIG_DFL );
    printf_msg("System core signal:%d\n",signo);
#endif
	actors_on_core();
}

#ifdef TESTMEM
#include "crtdbg.h"
inline void EnableMemLeakCheck( )
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}
#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif

int main( int argc, char *argv[] )
{
	char isoutput = 0;
	const char *rootpath;
#ifdef TESTMEM
	EnableMemLeakCheck( );
	_CrtSetBreakAlloc( 52 );
#endif
#ifndef WIN32
	int childpid;//, fd, fdtablesize;
//	int fp;
	int waittimes;
	chdir( GetRootPath() );
#else
	SetCurrentDirectory( GetRootPath() );
#endif
	rootpath = GetRootPath();
	if( rootpath )
		strcpy( g_szWorkPath, rootpath );
	else
		g_szWorkPath[0] = 0;
	if( argc > 1 )
	{
		if( strncmp( argv[1], "-o", 2 ) == 0 )
			isoutput = 1;
#ifndef WIN32
		else if( strncmp( argv[1], "start", 5 ) == 0 )
		{
			// 开始启动
			serv_setstat( 1 );

			signal(SIGTTOU,SIG_IGN);
			signal(SIGTTIN,SIG_IGN);
			signal(SIGTSTP,SIG_IGN);
			signal(SIGHUP,SIG_IGN);

			if( fork() != 0 )
			{
				sleep(8);
				exit(0);
			}

			already_running();
		}
		else if( strncmp( argv[1], "stop", 4 ) == 0 )
		{
			// 开始关闭
			serv_setstat( -10 );

			childpid = getmypid();
			if( childpid > 0 )
			{
				waittimes = 10;
				kill( childpid, SIGHUP );
				while( waittimes > 0 )
				{
					sleep(1);
					childpid = getmypid();
					if( childpid <= 0 )
					{
						printf_msg( "Server Stop.\n" );
						// 已经关闭
						// serv_setstat( -20 );
						return 0;
					}
				}
				printf_msg( "Server Cannot Stop.\n" );
				// 关闭失败
				serv_setstat( -11 );
			}
			else
			{
				// 已经关闭
				// serv_setstat( -20 );
				printf_msg( "no run pid:%d\n", -1 );
				return 0;
			}
			return -1;
		}
		else if ( strncmp( argv[1], "lua", 3 ) == 0 )
		{
			childpid = getmypid();
			if( childpid > 0 )
			{
				linuxmsg_send( 1 );
			}
			else
			{
				// 已经关闭
				// serv_setstat( -20 );
				printf_msg( "no run pid:%d\n", -1 );
				return 0;
			}
			return -1;
		}
		else if ( strncmp( argv[1], "sql", 3 ) == 0 )
		{
			childpid = getmypid();
			if( childpid > 0 )
			{
				linuxmsg_send( 2 );
			}
			else
			{
				// 已经关闭
				// serv_setstat( -20 );
				printf_msg( "no run pid:%d\n", -1 );
				return 0;
			}
			return -1;
		}
#endif

	}
#ifdef WIN32
	g_Config.max_connection = 512;
	// 开始启动
	serv_setstat( 1 );
#else
	g_Config.max_connection = 512;
	// 设置退出方式
	signal( SIGSEGV, sig_process );
	signal( SIGBUS, sig_process );
	signal( SIGFPE, sig_process );
	signal( SIGABRT, sig_process );
#endif

	g_Config.minver.value = 0;
	memset( g_Config.server_name, 0, sizeof(g_Config.server_name) );
	g_Config.server_exit_week = -1;
	g_Config.server_exit_hour = -1;
	g_Config.server_exit_min = -1;
	g_Config.showflag = 1;
	g_Config.showcdkey = 1;
	g_Config.showvip = 1;
	g_Config.showmcard = 1;
	g_Config.max_citycount = 20000;
	g_Config.max_clubcount = 256;
	g_Config.global_ipaddress = 0;
	g_Config.global_port = 0;
	g_Config.usegate = 0;
	g_Config.usenetlog = 1;
	u_getcfg( "server.cfg", configprocess );
	u_loginit( LOG_PATH );

	if ( g_Config.usenetlog == 1 )
		open_netlog();
	open_gamelog();

	#ifndef WIN32
	if ( strncmp( argv[1], "actor", 5 ) == 0 )
	{
		mem2struct_actor( argv[2], atoi( argv[3] ) );
		return -1;
	}
	else if ( strncmp( argv[1], "city", 4 ) == 0 )
	{
		mem2struct_city( argv[2], atoi( argv[3] ) );
		return -1;
	}
#endif

	// 随机种子
	srand( (unsigned int)time(NULL) );
#ifdef _USEGATE
	netserver_setgatemode( g_Config.usegate );
#endif
	// 服务进程初始化
	if( process_init( g_Config.max_connection ) < 0 )
	{
		// 启动失败
		serv_setstat( -1 );
#ifdef WIN32
		Sleep( 5000 );
#else
		sleep( 5 );
#endif
		return -1;
	}

	//output_start( isoutput );

	if( server_init( g_Config.ipaddress, g_Config.port, g_Config.max_connection, g_Config.minver.value ) < 0 )
	{
#ifdef WIN32
		Sleep( 3000 );
#else
		sleep( 3 );
#endif
		// 启动失败
		serv_setstat( -1 );
		return -1;
	}
	init_timer();
	append_timer( (void *)target_incrementor, 0, 1193181/fps_limit );

	process_set_exit( g_Config.server_exit_week, g_Config.server_exit_hour, g_Config.server_exit_min, (g_Config.server_exit_week<0?0:1) );

	// 基本启动成功了
	serv_setstat( 0 );

	server_start( process_client, process_onclose, process_error, process_ondestory );

	//output_setmsg( "Now Exitting..." );
	exit_timer();
	printf_msg( "Now Exitting...\n" );
	server_destroy();
	process_close();
	u_logcloseall();
	//output_setmsg( "System Exited." );
#ifdef WIN32
		Sleep( 1000 );
#else
		sleep( 1 );
#endif
	printf_msg( "System Exited.\n" );
	// 已经关闭
	serv_rmstat();
	return 0;
}
