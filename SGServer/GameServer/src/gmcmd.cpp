#include "gmcmd.h"
#include "actor.h"
#include "gameproc.h"
#include "dserver.h"
#include "netserver.h"
#include "server_netsend_auto.h"
#include <mysql.h>

extern MYSQL *myData;
extern Actor *g_actors;
extern int g_actornum;
extern SConfig g_Config;
extern char g_bServerIsInit;
SCmdInfo *g_cmdinfo;
int g_cmdcount;
int g_gm_result[64];

int readtoqueue( int client_index, char *pBuf, int readsize, char stat );

int gm_init()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];
	int cmdid;
	int tmpi;

	sprintf( szSQL, "select max(cmd) from gmcmd" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myData ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	if ( (row = mysql_fetch_row( res )) )
	{
		if ( row[0] )
			g_cmdcount = atoi( row[0] ) + 1;
		else
			g_cmdcount = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	// 分配空间
	g_cmdinfo = (SCmdInfo *)malloc( sizeof(SCmdInfo)*g_cmdcount );
	memset( g_cmdinfo, 0, sizeof(SCmdInfo)*g_cmdcount );

	sprintf( szSQL, "select cmd,allowgmtools from gmcmd" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myData ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );

	while ( (row = mysql_fetch_row( res )) )
	{
		cmdid = atoi( row[0] );
		if ( cmdid < 0 || cmdid >= g_cmdcount )
		{
			mysql_free_result( res );
			return -1;
		}
		g_cmdinfo[cmdid].m_allowgmtools = atoi( row[1] );
	}
	mysql_free_result( res );

	for ( tmpi = 0; tmpi < 64; tmpi++ )
	{
		g_gm_result[tmpi] = 0;
	}

	return d_start( gm_proc, g_Config.ds_ipaddress, g_Config.ds_port );
}

char gm_isallow_gmtools( short cmdid )
{
	if ( cmdid < 0 || cmdid >= g_cmdcount )
		return 0;
	return g_cmdinfo[cmdid].m_allowgmtools;
}

int gm_getindex()
{
	int tmpi;
	for ( tmpi = 0; tmpi < 64; tmpi++ )
	{
		if ( g_gm_result[tmpi] == 0 )
			return tmpi;
	}
	return -1;
}

int gm_set_result( int client_index, int value )
{
	int gmindex;
	gmindex = -(client_index + 10000);
	if ( gmindex < 0 || gmindex >= 64 )
		return -1;
	g_gm_result[gmindex] = value;
	return 0;
}

int gm_get_result( int client_index )
{
	int gmindex;
	gmindex = -(client_index + 10000);
	if ( gmindex < 0 || gmindex >= 64 )
		return -1;
	return g_gm_result[gmindex];
}

int gm_sendtolocal( short cmd, int arg1, int arg2, int arg3, int arg4, char *str )
{
	int gmindex;
	int tmpi;
	int result = 0;

	if ( str == NULL )
		return -1;
	
	if ( g_bServerIsInit == 0 )
	{
		return -1;
	}

	SLK_NetU_Gmlocalcmd Value = {};
	Value.m_cmd = cmd;
	Value.m_value[0] = arg1;
	Value.m_value[1] = arg2;
	Value.m_value[2] = arg3;
	Value.m_value[3] = arg4;
	Value.m_msglen = (short)strlen( str );
	if ( Value.m_msglen > 0 )
	{
		if ( Value.m_msglen > 1023 )
			Value.m_msglen = 1023;
		memcpy( Value.m_msg, str, Value.m_msglen );
		Value.m_msg[1023] = 0;
	}

	gmindex = gm_getindex();
	if ( gmindex < 0 )
	{
		write_netlog( "GMcmd(%d):%d value:%d,%d,%d,%d,%s", gmindex, cmd, arg1, arg2, arg3, arg4, Value.m_msg );
		netsend_gmlocalcmd_S( -2, 0, &Value );
	}
	else
	{
		write_netlog( "GMcmd(%d):%d value:%d,%d,%d,%d,%s", gmindex, cmd, arg1, arg2, arg3, arg4, Value.m_msg );
		gm_set_result( -gmindex - 10000, -1 );
		netsend_gmlocalcmd_S( -gmindex - 10000, 0, &Value );
		// result初值为0＝未使用 -1＝已占用 
		for ( tmpi = 0; tmpi < 5; tmpi++ )
		{
			result = gm_get_result( -gmindex - 10000 );
			if ( result != -1 )
			{
				gm_set_result( -gmindex - 10000, 0 );
				break;
			}
			Sleep( 200 );
		}
	}

	return result;
}

// 发送包：short(0) 表示短连接并且返回人数
// 发送包：short(9999) 表示需要长连接
// 发送包：short(cmd) | arg1 | arg2 | arg3 | arg4 | short(str len) | string
// 返回包：int(result)
// 要非常注意此处调用，该调用为另外的线程，并非属于主逻辑线程
// 如果有非线程安全的调用，需要将指令发送到总接收队列中，以保证安全
int gm_proc( short cmd, int arg1, int arg2, int arg3, int arg4, char *str )
{
	switch ( cmd )
	{
	case 0:
		return g_actornum;
		break;
	default:
		if ( gm_isallow_gmtools( cmd ) )
			return gm_sendtolocal( cmd, arg1, arg2, arg3, arg4, str );
		break;
	}
	return 0;
}

