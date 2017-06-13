extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include "db.h"
#include "define.h"
#include "global.h"
extern lua_State* servL;
extern MYSQL *myGame;
extern MYSQL *myData;
extern int g_serverpoint;
extern int g_server_citylevel5_count;

Global global;
int g_world_data[WORLD_DATA_MAX] = {0};

int global_init()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];
	sprintf( szSQL, "select id, value from global" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}

	res = mysql_store_result( myData );

	lua_newtable( servL );
	while ( (row = mysql_fetch_row( res )) )
	{	
		int id = atoi( row[0] );
		float value = (float)atof( row[1] );
		if ( id == 1 )
		{
			global.actorlevel_max = atoi( row[1] );
			lua_pushstring( servL, "actorlevel_max" );
			lua_pushinteger( servL, global.actorlevel_max );
			lua_rawset( servL, -3 );
		}	
	}
	lua_setglobal( servL, "global" );
	mysql_free_result( res );
	return 0;
}

void global_reload()
{
	global_init();
	world_data_init();
}

// 全局数据初始化
int world_data_init()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[128];
	int		index = 0;

	memset( &g_world_data, 0, sizeof( int ) * WORLD_DATA_MAX );

	sprintf( szSQL, "select id, value from world_data" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( (row = mysql_fetch_row( res )) )
	{
		index = atoi( row[0] );
		if ( index >= 0 && index < WORLD_DATA_MAX )
		{
			g_world_data[index] = atoi( row[1] );
		}
	}
	mysql_free_result( res );

	return 0;
}

// 从数据库获取全局数据
int world_data_get( int index, char *strvalue )
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[128];
	int nValue = -1;

	sprintf( szSQL, "select value,strvalue from world_data where id='%d'", index );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( (row = mysql_fetch_row( res )) )
	{
		if ( row[0] )
		{
			nValue = atoi( row[0] );
			if ( strvalue && row[1] )
			{
				strcpy( strvalue, row[1] );
			}
		}
	}
	mysql_free_result( res );

	return nValue;
}

int world_data_set( int index, int value, char *strvalue, FILE *fp )
{
	char szSQL[1024];

	if ( index >= 0 && index < WORLD_DATA_MAX )
	{
		g_world_data[index] = value;
	}

	if ( strvalue )
		sprintf( szSQL, "replace into world_data ( id, value, strvalue) values('%d','%d','%s')", index, value, strvalue );
	else
		sprintf( szSQL, "replace into world_data ( id, value, strvalue) values('%d','%d','')", index, value );

	if ( fp )
	{
		fprintf( fp, "%s;\n", szSQL );
	}
	else if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}

	return 0;
}
