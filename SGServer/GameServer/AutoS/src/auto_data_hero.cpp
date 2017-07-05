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

	sprintf( szSQL, "select kind, max( color ) from hero group by kind;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		int kind = atoi( row[0] );
		if ( kind < 0 || kind >= g_heroinfo_maxnum  )
			continue;
		g_heroinfo[kind].maxnum = atoi( row[1] ) + 1;
		g_heroinfo[kind].config = (HeroInfoConfig *)malloc( sizeof(HeroInfoConfig)*g_heroinfo[kind].maxnum );
		memset( g_heroinfo[kind].config, 0, sizeof(HeroInfoConfig)*g_heroinfo[kind].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `kind`,`color`,`corps`,`attack_bas0`,`attack_bas1`,`attack_wash`,`attack_wash_limit`,`defense_bas0`,`defense_bas1`,`defense_wash`,`defense_wash_limit`,`troops_bas0`,`troops_bas1`,`troops_wash`,`troops_wash_limit`,`total_wash` from hero;" );
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
		int color = atoi( row[1] );
		if ( color < 0 || color >= g_heroinfo[kind].maxnum )
			continue;
		g_heroinfo[kind].config[color].kind = atoi(row[offset++]);
		g_heroinfo[kind].config[color].color = atoi(row[offset++]);
		g_heroinfo[kind].config[color].corps = atoi(row[offset++]);
		g_heroinfo[kind].config[color].attack_bas[0] = atoi(row[offset++]);
		g_heroinfo[kind].config[color].attack_bas[1] = atoi(row[offset++]);
		g_heroinfo[kind].config[color].attack_wash = atoi(row[offset++]);
		g_heroinfo[kind].config[color].attack_wash_limit = atoi(row[offset++]);
		g_heroinfo[kind].config[color].defense_bas[0] = atoi(row[offset++]);
		g_heroinfo[kind].config[color].defense_bas[1] = atoi(row[offset++]);
		g_heroinfo[kind].config[color].defense_wash = atoi(row[offset++]);
		g_heroinfo[kind].config[color].defense_wash_limit = atoi(row[offset++]);
		g_heroinfo[kind].config[color].troops_bas[0] = atoi(row[offset++]);
		g_heroinfo[kind].config[color].troops_bas[1] = atoi(row[offset++]);
		g_heroinfo[kind].config[color].troops_wash = atoi(row[offset++]);
		g_heroinfo[kind].config[color].troops_wash_limit = atoi(row[offset++]);
		g_heroinfo[kind].config[color].total_wash = atoi(row[offset++]);
	}
	mysql_free_result( res );
	heroinfo_luatable_auto();
	return 0;
}

int heroinfo_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_heroinfo_maxnum; tmpi++ )
	{
		if ( g_heroinfo[tmpi].config )
		{
			free( g_heroinfo[tmpi].config );
			g_heroinfo[tmpi].config = NULL;
		}
	}
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
	lua_setglobal( servL, "g_heroinfo" );

	lua_pushinteger( servL, g_heroinfo_maxnum );
	lua_setglobal( servL, "g_heroinfo_maxnum" );
	return 0;
}
