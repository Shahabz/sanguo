#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include "db.h"
#include "gamelog.h"
#include "actor.h"

extern MYSQL *myLog;
extern int g_actornum;
extern Actor *g_actors;
extern int g_maxactornum;
int g_nLogGroup;
int g_connectioncount = 0;
int g_lastwlog_datesign = 0;

int log_init()
{
	g_nLogGroup = 1;
	return 0;
}

int wlogcreate()
{
	time_t t;
	struct tm *newtime;
	int datesign_now;
	
	// 打印时间
	time(&t);
	newtime = localtime( &t );
	// 判断文件名是否是今天的
	datesign_now = (1900+newtime->tm_year)*10000 + (newtime->tm_mon+1)*100 + newtime->tm_mday;
	if( datesign_now > g_lastwlog_datesign )
	{
		char szSQL[1024] = {0};
		int reconnect_flag = 0;
RE_LOG_CREATE:
		szSQL[0] = 0;
		reconnect_flag = 0;
		sprintf( szSQL, "CREATE TABLE IF NOT EXISTS `glog_%d` ( `id` int(11) NOT NULL auto_increment, `group` int(11) NOT NULL default '0', `op` int(11) NOT NULL default '0', `path` tinyint(4) NOT NULL default '0', `object` int(11) NOT NULL default '0', `objectnum` int(11) NOT NULL default '0', `objectid` bigint(20) NOT NULL default '0', `actor1` int(11) NOT NULL default '0', `actor2` int(11) NOT NULL default '0', `optime` datetime NOT NULL default '0000-00-00 00:00:00', PRIMARY KEY  (`id`) ) ENGINE=MyISAM DEFAULT CHARSET=gbk ROW_FORMAT=DYNAMIC;", datesign_now );
		if( mysql_query( myLog, szSQL ) )
		{
			printf_msg( "Query failed (%s)\n", mysql_error(myLog) );
			write_gamelog( "%s", szSQL );
			if( reconnect_flag )
				return -1;
			if( mysql_ping(myLog) != 0 )
			{
				db_reconnect_log();
				reconnect_flag = 1;
				goto RE_LOG_CREATE;
			}
			return -1;
		}
		g_lastwlog_datesign = datesign_now;
	}
	return 0;
}

int wlog( char type, int op, char path, int object, int objectnum, i64 objectid, int source, int target )
{
	char	szSQL[1024];
	char bigint[21];

	wlogcreate();
	int reconnect_flag = 0;
	lltoa( objectid, bigint, 10 );
RE_LOG_UPDATE:
	sprintf( szSQL, "insert into glog_%d (`group`,op,path,object,objectnum,objectid,actor1,actor2,optime) \
					values('%d','%d','%d','%d', '%d','%s','%d','%d',NOW())", \
					g_lastwlog_datesign, g_nLogGroup,op,path,object,objectnum,bigint,source,target );
	if( mysql_query( myLog, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error(myLog) );
		write_gamelog( "%s", szSQL );
		if( reconnect_flag )
			return -1;
		if( mysql_ping(myLog) != 0 )
		{
			db_reconnect_log();
			reconnect_flag = 1;
			goto RE_LOG_UPDATE;
		}
		return -1;
	}
	if( type == 0 )
		g_nLogGroup++;

	return 0;
}

int wlog_token( char type, int op, char path, int object, int objectnum, i64 objectid, int source, int target, i64 userid )
{
	char	szSQL[1024];
	char bigint[21];
	char biguserid[21];

	int reconnect_flag = 0;
	lltoa( objectid, bigint, 10 );
	lltoa( userid, biguserid, 10 );
RE_LOG_UPDATE:
	sprintf( szSQL, "insert into tokenlog (`group`,`op`,`path`,`object`,`objectnum`,`objectid`,`actor1`,`actor2`,`optime`,`userid`) \
										values('%d','%d','%d','%d', '%d','%s','%d','%d',NOW(),'%s')", \
										g_nLogGroup, op, path, object, objectnum, bigint, source, target, biguserid );
	if ( mysql_query( myLog, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myLog ) );
		write_gamelog( "%s", szSQL );
		if ( reconnect_flag )
			return -1;
		if ( mysql_ping( myLog ) != 0 )
		{
			db_reconnect_log();
			reconnect_flag = 1;
			goto RE_LOG_UPDATE;
		}
		return -1;
	}
	if ( type == 0 )
		g_nLogGroup++;

	return 0;
}

int wcount()
{
	char	szSQL[1024];
	int reconnect_flag = 0;

RE_LOG_CONUT:
	sprintf( szSQL, "insert into gamecount (con,game,optime) values('%d','%d',NOW())", g_connectioncount, g_actornum );
	if( mysql_query( myLog, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error(myLog) );
		write_gamelog( "%s", szSQL );
		if( reconnect_flag )
			return -1;
		if( mysql_ping(myLog) != 0 )
		{
			db_reconnect_log();
			reconnect_flag = 1;
			goto RE_LOG_CONUT;
		}
		return -1;
	}
	return 0;
}
