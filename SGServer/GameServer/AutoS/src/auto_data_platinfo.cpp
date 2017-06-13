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
#include "auto_data_platinfo.h"
extern MYSQL *myData;
extern lua_State* servL;
PlatInfo *g_platinfo = NULL;
int g_platinfo_maxnum = 0;

int platinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(platid) from platinfo;" );
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
			g_platinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_platinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_platinfo = (PlatInfo *)malloc( sizeof(PlatInfo)*g_platinfo_maxnum );
	memset( g_platinfo, 0, sizeof(PlatInfo)*g_platinfo_maxnum );

	sprintf( szSQL, "select `platid`,`userhost`,`userport`,`allow` from platinfo;" );
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
		int platid = atoi( row[0] );
		if ( platid < 0 || platid >= g_platinfo_maxnum  )
			continue;
		g_platinfo[platid].platid = atoi(row[offset++]);
		memcpy( g_platinfo[platid].userhost, row[offset++], 64 ); g_platinfo[platid].userhost[63]=0;
		g_platinfo[platid].userport = atoi(row[offset++]);
		g_platinfo[platid].allow = atoi(row[offset++]);
	}
	mysql_free_result( res );
	platinfo_luatable_auto();
	return 0;
}

int platinfo_reload_auto()
{
	if ( g_platinfo )
	{
		free( g_platinfo );
		g_platinfo = NULL;
	}
	g_platinfo_maxnum = 0;
	platinfo_init_auto();
	return 0;
}

int platinfo_luatable_auto()
{
	lua_newtable( servL );
	for ( int platid = 0; platid < g_platinfo_maxnum; platid++ )
	{
		lua_pushinteger( servL, platid );
		lua_newtable( servL );

		lua_pushstring( servL, "platid" );
		lua_pushinteger( servL, g_platinfo[platid].platid );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "userhost" );
		lua_pushstring( servL, g_platinfo[platid].userhost );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "userport" );
		lua_pushinteger( servL, g_platinfo[platid].userport );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "allow" );
		lua_pushinteger( servL, g_platinfo[platid].allow );
		lua_rawset( servL, -3 );

		lua_rawset( servL, 1 );
	}
	lua_setglobal( servL, "g_platinfo" );

	lua_pushinteger( servL, g_platinfo_maxnum );
	lua_setglobal( servL, "g_platinfo_maxnum" );
	return 0;
}
