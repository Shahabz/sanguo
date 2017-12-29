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
#include "auto_data_upgrade.h"
extern MYSQL *myData;
extern lua_State* servL;
UpgradeInfo *g_upgradeinfo = NULL;
int g_upgradeinfo_maxnum = 0;

int upgradeinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`level`) from upgrade;" );
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
			g_upgradeinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_upgradeinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_upgradeinfo = (UpgradeInfo *)malloc( sizeof(UpgradeInfo)*g_upgradeinfo_maxnum );
	memset( g_upgradeinfo, 0, sizeof(UpgradeInfo)*g_upgradeinfo_maxnum );

	sprintf( szSQL, "select `level`,`exp`,`heroexp0`,`heroexp1`,`heroexp2`,`heroexp3`,`heroexp4`,`heroexp5`,`battlepower`,`awardgroup`,`login_award` from upgrade;" );
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
		if ( level < 0 || level >= g_upgradeinfo_maxnum  )
			continue;
		g_upgradeinfo[level].level = atoi(row[offset++]);
		g_upgradeinfo[level].exp = atoi(row[offset++]);
		g_upgradeinfo[level].heroexp[0] = atoi(row[offset++]);
		g_upgradeinfo[level].heroexp[1] = atoi(row[offset++]);
		g_upgradeinfo[level].heroexp[2] = atoi(row[offset++]);
		g_upgradeinfo[level].heroexp[3] = atoi(row[offset++]);
		g_upgradeinfo[level].heroexp[4] = atoi(row[offset++]);
		g_upgradeinfo[level].heroexp[5] = atoi(row[offset++]);
		g_upgradeinfo[level].battlepower = atoi(row[offset++]);
		g_upgradeinfo[level].awardgroup = atoi(row[offset++]);
		g_upgradeinfo[level].login_award = atoi(row[offset++]);
	}
	mysql_free_result( res );
	upgradeinfo_luatable_auto();
	return 0;
}

int upgradeinfo_reload_auto()
{
	if ( g_upgradeinfo )
	{
		free( g_upgradeinfo );
		g_upgradeinfo = NULL;
	}
	g_upgradeinfo_maxnum = 0;
	upgradeinfo_init_auto();
	return 0;
}

int upgradeinfo_luatable_auto()
{
	lua_newtable( servL );
	for ( int level = 0; level < g_upgradeinfo_maxnum; level++ )
	{
		lua_pushinteger( servL, level );
		lua_newtable( servL );

		lua_pushstring( servL, "level" );
		lua_pushinteger( servL, g_upgradeinfo[level].level );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "exp" );
		lua_pushinteger( servL, g_upgradeinfo[level].exp );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "heroexp" );
		lua_newtable( servL );
		for ( int i = 0; i < 6; i++ )
		{
			lua_pushinteger( servL, i );
			lua_pushinteger( servL, g_upgradeinfo[level].heroexp[i] );
			lua_rawset( servL, -3 );
		}
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "battlepower" );
		lua_pushinteger( servL, g_upgradeinfo[level].battlepower );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "awardgroup" );
		lua_pushinteger( servL, g_upgradeinfo[level].awardgroup );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "login_award" );
		lua_pushinteger( servL, g_upgradeinfo[level].login_award );
		lua_rawset( servL, -3 );

		lua_rawset( servL, 1 );
	}
	lua_setglobal( servL, "g_upgradeinfo" );

	lua_pushinteger( servL, g_upgradeinfo_maxnum );
	lua_setglobal( servL, "g_upgradeinfo_maxnum" );
	return 0;
}
