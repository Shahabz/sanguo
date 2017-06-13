#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "netserver.h"

#ifndef WIN32
#include <sys/time.h>
#endif

#include "actor.h"
#include "global_net.h"
#include "global_netprocess.h"
#include "global_process_auto.h"
#include "define.h"

extern Actor *g_actors;
extern int g_actornum;
extern int g_maxactornum;

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

InterService *g_pInterService = NULL;
int g_pInterServiceCount = 0;

/*
使用方法：
1.初始化，调用global_process_init
2.发送函数使用global_sendtoqueue( 目标服务器编号(-1是全部), 指令(自定义), value1(自定义), value2(自定义), data(自定义), datasize );
3.修改global_process_recv函数，处理接收到的其他服务器传送来的信息。
*/

int global_process_recv( short server_from, short cmd, int value1, int value2, char *buf, int size )
{
	printf_msg( "server_from:%d cmd:%d value1:%d value2:%d string:%s size:%d\n", server_from, cmd, value1, value2, buf, size );
	proc_command_G( server_from, cmd, value1, value2, buf, size );
	return 0;
}

void global_process_init()
{
	global_net_init( g_Config.global_ipaddress, g_Config.global_port, g_Config.server_code, global_process_recv );
}

int inter_service_init()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];

	// 获得表格中最大的建筑类型编号
	sprintf( szSQL, "select count(serverid) from inter_service" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	if ( (row = mysql_fetch_row( res )) )
	{
		if ( row[0] )
			g_pInterServiceCount = atoi( row[0] ) + 1;
		else
			g_pInterServiceCount = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_pInterService = (InterService *)malloc( sizeof(InterService)*g_pInterServiceCount );
	memset( g_pInterService, 0, sizeof(InterService)*g_pInterServiceCount );

	// 获得表格中最大的等级
	sprintf( szSQL, "SELECT serverid,type,target_sevid1,target_sevid2,target_sevid3,target_sevid4,target_sevid5,target_sevid6,target_sevid7,target_sevid8 FROM inter_service;" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	int offset = 0;
	res = mysql_store_result( myData );
	while ( (row = mysql_fetch_row( res )) )
	{
		int index = 0;
		g_pInterService[offset].serverid = atoi( row[index++] );
		g_pInterService[offset].type = atoi( row[index++] );
		g_pInterService[offset].target_sevid[0] = atoi( row[index++] );
		g_pInterService[offset].target_sevid[1] = atoi( row[index++] );
		g_pInterService[offset].target_sevid[2] = atoi( row[index++] );
		g_pInterService[offset].target_sevid[3] = atoi( row[index++] );
		g_pInterService[offset].target_sevid[4] = atoi( row[index++] );
		g_pInterService[offset].target_sevid[5] = atoi( row[index++] );
		g_pInterService[offset].target_sevid[6] = atoi( row[index++] );
		g_pInterService[offset].target_sevid[7] = atoi( row[index++] );
		for ( int tmpi = 0; tmpi < INTERSERVICE; tmpi++ )
		{
			if ( g_pInterService[offset].target_sevid[tmpi] > 0 )
				g_pInterService[offset].target_sevcount += 1;
		}
		offset += 1;
	}
	mysql_free_result( res );
	return 0;
}

int inter_service_reload()
{
	if ( g_pInterService )
	{
		free( g_pInterService );
		g_pInterService = NULL;
	}
	g_pInterServiceCount = 0;
	inter_service_init();
	return 0;
}

short inter_service_getindex()
{
	for ( int tmpi = 0; tmpi < g_pInterServiceCount; tmpi++ )
	{
		if ( g_pInterService[tmpi].serverid == g_Config.server_code )
		{
			return tmpi;
		}
	}
	return -1;
}

short inter_service_getserverid()
{
	short serverid = 0;
	short index = inter_service_getindex();
	if ( index < 0 )
		return 0;
	InterService *pInter = &g_pInterService[index];
	if ( pInter == NULL )
		return 0;
	if ( pInter->type == 0 )
	{
		if ( pInter->target_sevcount <= 0 )
			return 0;
		short offset = rand() % pInter->target_sevcount;
		serverid = pInter->target_sevid[offset];
	}
	else
	{

	}
	return serverid;
}
