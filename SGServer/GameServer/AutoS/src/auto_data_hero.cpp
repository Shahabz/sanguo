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
#include "auto_data_hero.h"
extern MYSQL *myData;
extern lua_State* servL;
HeroInfo *g_heroinfo = NULL;
int g_heroinfo_maxnum = 0;

int heroinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(kind) from hero;" );
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
			g_heroinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_heroinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_heroinfo = (HeroInfo *)malloc( sizeof(HeroInfo)*g_heroinfo_maxnum );
	memset( g_heroinfo, 0, sizeof(HeroInfo)*g_heroinfo_maxnum );

	sprintf( szSQL, "select `kind`,`color`,`corps`,`attack_base`,`attack_wash`,`attack_wash_limit`,`defense_base`,`defense_wash`,`defense_wash_limit`,`troops_base`,`troops_wash`,`troops_wash_limit`,`total_wash` from hero;" );
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
		int kind = atoi( row[0] );
		if ( kind < 0 || kind >= g_heroinfo_maxnum  )
			continue;
		g_heroinfo[kind].kind = atoi(row[offset++]);
		g_heroinfo[kind].color = atoi(row[offset++]);
		g_heroinfo[kind].corps = atoi(row[offset++]);
		g_heroinfo[kind].attack_base = atoi(row[offset++]);
		g_heroinfo[kind].attack_wash = atoi(row[offset++]);
		g_heroinfo[kind].attack_wash_limit = atoi(row[offset++]);
		g_heroinfo[kind].defense_base = atoi(row[offset++]);
		g_heroinfo[kind].defense_wash = atoi(row[offset++]);
		g_heroinfo[kind].defense_wash_limit = atoi(row[offset++]);
		g_heroinfo[kind].troops_base = atoi(row[offset++]);
		g_heroinfo[kind].troops_wash = atoi(row[offset++]);
		g_heroinfo[kind].troops_wash_limit = atoi(row[offset++]);
		g_heroinfo[kind].total_wash = atoi(row[offset++]);
	}
	mysql_free_result( res );
	heroinfo_luatable_auto();
	return 0;
}

int heroinfo_reload_auto()
{
	if ( g_heroinfo )
	{
		free( g_heroinfo );
		g_heroinfo = NULL;
	}
	g_heroinfo_maxnum = 0;
	heroinfo_init_auto();
	return 0;
}

int heroinfo_luatable_auto()
{
	lua_newtable( servL );
	for ( int kind = 0; kind < g_heroinfo_maxnum; kind++ )
	{
		lua_pushinteger( servL, kind );
		lua_newtable( servL );

		lua_pushstring( servL, "kind" );
		lua_pushinteger( servL, g_heroinfo[kind].kind );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "color" );
		lua_pushinteger( servL, g_heroinfo[kind].color );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "corps" );
		lua_pushinteger( servL, g_heroinfo[kind].corps );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "attack_base" );
		lua_pushinteger( servL, g_heroinfo[kind].attack_base );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "attack_wash" );
		lua_pushinteger( servL, g_heroinfo[kind].attack_wash );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "attack_wash_limit" );
		lua_pushinteger( servL, g_heroinfo[kind].attack_wash_limit );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "defense_base" );
		lua_pushinteger( servL, g_heroinfo[kind].defense_base );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "defense_wash" );
		lua_pushinteger( servL, g_heroinfo[kind].defense_wash );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "defense_wash_limit" );
		lua_pushinteger( servL, g_heroinfo[kind].defense_wash_limit );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "troops_base" );
		lua_pushinteger( servL, g_heroinfo[kind].troops_base );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "troops_wash" );
		lua_pushinteger( servL, g_heroinfo[kind].troops_wash );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "troops_wash_limit" );
		lua_pushinteger( servL, g_heroinfo[kind].troops_wash_limit );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "total_wash" );
		lua_pushinteger( servL, g_heroinfo[kind].total_wash );
		lua_rawset( servL, -3 );

		lua_rawset( servL, 1 );
	}
	lua_setglobal( servL, "g_heroinfo" );

	lua_pushinteger( servL, g_heroinfo_maxnum );
	lua_setglobal( servL, "g_heroinfo_maxnum" );
	return 0;
}
