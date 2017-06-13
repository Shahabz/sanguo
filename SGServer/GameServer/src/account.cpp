#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <winsock2.h>
#include <mswsock.h>
#endif
#include <mysql.h>
#include "actor.h"
#include "client.h"
#include "account.h"
#include "db.h"
#include "login.h"
#include "define.h"
#include "client.h"

extern Actor *g_actors;
extern int g_maxactornum;
extern MYSQL *myLog;
int g_accountid = 0;

int account_init()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];
	// 表中的最大ID值
	sprintf( szSQL, "select max(id) from account" );
	if( mysql_query( myLog, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error(myLog) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myLog );
	if( ( row = mysql_fetch_row( res ) ) )
	{
		if( row[0] )
			g_accountid = atoi( row[0] )+1;
		else
			g_accountid = 1;
	}
	else
	{
		g_accountid = 1;
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );
	return 0;
}

// 进入
int account_in( int actor_index, char type )
{
	char	szSQL[1024];
	int accountid;
	char szUserID[21];

	int platid;
	int invcode;
	i64 userid;
	int actorid;
	short actorlevel;

	SClientInfo ClientInfo;
	int reconnect_flag = 0;

	if( client_getuserinfo( actor_index, &ClientInfo ) < 0 )
		return -1;

	userid = client_getuserid( actor_index );
	platid = client_getplatid( actor_index );
	invcode = client_getinvcode( actor_index );
	actorid = g_actors[actor_index].actorid;
	actorlevel = g_actors[actor_index].level;

	lltoa( userid, szUserID, 10);

	if( userid < 0 || actorid <= 0 )
	{
		write_gamelog( "account in [userid:%s actorid:%d]", szUserID, actorid );
		return -1;
	}
	accountid = g_accountid++;
RE_LOG_ACCIN:
	sprintf( szSQL, "insert into account (id,platid,userid,username,actorid,ip,in_time,in_level,deviceid,invcode) values('%d','%d','%s','%s','%d','%s',now(),'%d','%s','%d')", accountid, platid, szUserID,ClientInfo.username,actorid,client_getip(actor_index),actorlevel,client_getdeviceid(actor_index),invcode );
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
			goto RE_LOG_ACCIN;
		}
		return -1;
	}
	return accountid;
}

// 退出
int account_out( int actor_index, char type )
{
	int accountid;
	char	szSQL[1024];

	SClientInfo ClientInfo;
	int reconnect_flag = 0;
	
	short actorlevel;

	if( client_getuserinfo( actor_index, &ClientInfo ) < 0 )
		return -1;

	accountid = g_actors[actor_index].accountid;
	if( accountid <= 0 )
	{
		write_gamelog( "account out [%d]", accountid );
		return -1;
	}
	actorlevel = g_actors[actor_index].level;
RE_LOG_ACCOUT:
	if( type == 1 )
		sprintf( szSQL, "update account set op_type='2',out_time=now(),out_level='%d' where id='%d'", actorlevel, accountid );
	else
		sprintf( szSQL, "update account set op_type='1',out_time=now(),out_level='%d' where id='%d'", actorlevel, accountid );
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
			goto RE_LOG_ACCOUT;
		}
		return -1;
	}
	return 0;
}

