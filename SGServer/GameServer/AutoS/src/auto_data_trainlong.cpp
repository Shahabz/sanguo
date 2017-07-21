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
#include "db.h"
#include "auto_data_trainlong.h"
extern MYSQL *myData;
extern lua_State* servL;
TrainlongInfo *g_trainlong = NULL;
int g_trainlong_maxnum = 0;

int trainlonginfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(level) from trainlong;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	if( ( row = mysql_fetch_row( res ) ) )
	{
		if ( row[0] )
			g_trainlong_maxnum = atoi( row[0] ) + 1;
		else
			g_trainlong_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_trainlong = (TrainlongInfo *)malloc( sizeof(TrainlongInfo)*g_trainlong_maxnum );
	memset( g_trainlong, 0, sizeof(TrainlongInfo)*g_trainlong_maxnum );

	sprintf( szSQL, "select `level`,`timelong`,`sliver` from trainlong;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		offset = 0;
		int level = atoi( row[0] );
		if ( level < 0 || level >= g_trainlong_maxnum  )
			continue;
		g_trainlong[level].level = atoi(row[offset++]);
		g_trainlong[level].timelong = atoi(row[offset++]);
		g_trainlong[level].sliver = atoi(row[offset++]);
	}
	mysql_free_result( res );
	trainlonginfo_luatable_auto();
	return 0;
}

int trainlonginfo_reload_auto()
{
	if ( g_trainlong )
	{
		free( g_trainlong );
		g_trainlong = NULL;
	}
	g_trainlong_maxnum = 0;
	trainlonginfo_init_auto();
	return 0;
}

int trainlonginfo_luatable_auto()
{
	lua_newtable( servL );
	for ( int level = 0; level < g_trainlong_maxnum; level++ )
	{
		lua_pushinteger( servL, level );
		lua_newtable( servL );

		lua_pushstring( servL, "level" );
		lua_pushinteger( servL, g_trainlong[level].level );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "timelong" );
		lua_pushinteger( servL, g_trainlong[level].timelong );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "sliver" );
		lua_pushinteger( servL, g_trainlong[level].sliver );
		lua_rawset( servL, -3 );

		lua_rawset( servL, 1 );
	}
	lua_setglobal( servL, "g_trainlong" );

	lua_pushinteger( servL, g_trainlong_maxnum );
	lua_setglobal( servL, "g_trainlong_maxnum" );
	return 0;
}
