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
#include "auto_data_equip.h"
extern MYSQL *myData;
extern lua_State* servL;
EquipInfo *g_equipinfo = NULL;
int g_equipinfo_maxnum = 0;

int equipinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`kind`) from equip;" );
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
			g_equipinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_equipinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_equipinfo = (EquipInfo *)malloc( sizeof(EquipInfo)*g_equipinfo_maxnum );
	memset( g_equipinfo, 0, sizeof(EquipInfo)*g_equipinfo_maxnum );

	sprintf( szSQL, "select `kind`,`type`,`actorlevel`,`color`,`ability`,`value`,`sec`,`silver`,`material_kind0`,`material_kind1`,`material_kind2`,`material_kind3`,`material_kind4`,`material_kind5`,`material_num0`,`material_num1`,`material_num2`,`material_num3`,`material_num4`,`material_num5`,`prestige`,`battlepower`,`resolve_kind0`,`resolve_kind1`,`resolve_kind2`,`resolve_kind3`,`resolve_min0`,`resolve_min1`,`resolve_min2`,`resolve_min3`,`resolve_max0`,`resolve_max1`,`resolve_max2`,`resolve_max3` from equip;" );
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
		if ( kind < 0 || kind >= g_equipinfo_maxnum  )
			continue;
		g_equipinfo[kind].kind = atoi(row[offset++]);
		g_equipinfo[kind].type = atoi(row[offset++]);
		g_equipinfo[kind].actorlevel = atoi(row[offset++]);
		g_equipinfo[kind].color = atoi(row[offset++]);
		g_equipinfo[kind].ability = atoi(row[offset++]);
		g_equipinfo[kind].value = atoi(row[offset++]);
		g_equipinfo[kind].sec = atoi(row[offset++]);
		g_equipinfo[kind].silver = atoi(row[offset++]);
		g_equipinfo[kind].material_kind[0] = atoi(row[offset++]);
		g_equipinfo[kind].material_kind[1] = atoi(row[offset++]);
		g_equipinfo[kind].material_kind[2] = atoi(row[offset++]);
		g_equipinfo[kind].material_kind[3] = atoi(row[offset++]);
		g_equipinfo[kind].material_kind[4] = atoi(row[offset++]);
		g_equipinfo[kind].material_kind[5] = atoi(row[offset++]);
		g_equipinfo[kind].material_num[0] = atoi(row[offset++]);
		g_equipinfo[kind].material_num[1] = atoi(row[offset++]);
		g_equipinfo[kind].material_num[2] = atoi(row[offset++]);
		g_equipinfo[kind].material_num[3] = atoi(row[offset++]);
		g_equipinfo[kind].material_num[4] = atoi(row[offset++]);
		g_equipinfo[kind].material_num[5] = atoi(row[offset++]);
		g_equipinfo[kind].prestige = atoi(row[offset++]);
		g_equipinfo[kind].battlepower = atoi(row[offset++]);
		g_equipinfo[kind].resolve_kind[0] = atoi(row[offset++]);
		g_equipinfo[kind].resolve_kind[1] = atoi(row[offset++]);
		g_equipinfo[kind].resolve_kind[2] = atoi(row[offset++]);
		g_equipinfo[kind].resolve_kind[3] = atoi(row[offset++]);
		g_equipinfo[kind].resolve_min[0] = atoi(row[offset++]);
		g_equipinfo[kind].resolve_min[1] = atoi(row[offset++]);
		g_equipinfo[kind].resolve_min[2] = atoi(row[offset++]);
		g_equipinfo[kind].resolve_min[3] = atoi(row[offset++]);
		g_equipinfo[kind].resolve_max[0] = atoi(row[offset++]);
		g_equipinfo[kind].resolve_max[1] = atoi(row[offset++]);
		g_equipinfo[kind].resolve_max[2] = atoi(row[offset++]);
		g_equipinfo[kind].resolve_max[3] = atoi(row[offset++]);
	}
	mysql_free_result( res );
	equipinfo_luatable_auto();
	return 0;
}

int equipinfo_reload_auto()
{
	if ( g_equipinfo )
	{
		free( g_equipinfo );
		g_equipinfo = NULL;
	}
	g_equipinfo_maxnum = 0;
	equipinfo_init_auto();
	return 0;
}

int equipinfo_luatable_auto()
{
	lua_newtable( servL );
	for ( int kind = 0; kind < g_equipinfo_maxnum; kind++ )
	{
		lua_pushinteger( servL, kind );
		lua_newtable( servL );

		lua_pushstring( servL, "kind" );
		lua_pushinteger( servL, g_equipinfo[kind].kind );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "type" );
		lua_pushinteger( servL, g_equipinfo[kind].type );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "actorlevel" );
		lua_pushinteger( servL, g_equipinfo[kind].actorlevel );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "color" );
		lua_pushinteger( servL, g_equipinfo[kind].color );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "ability" );
		lua_pushinteger( servL, g_equipinfo[kind].ability );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "value" );
		lua_pushinteger( servL, g_equipinfo[kind].value );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "sec" );
		lua_pushinteger( servL, g_equipinfo[kind].sec );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "silver" );
		lua_pushinteger( servL, g_equipinfo[kind].silver );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "material_kind" );
		lua_newtable( servL );
		for ( int i = 0; i < 6; i++ )
		{
			lua_pushinteger( servL, i );
			lua_pushinteger( servL, g_equipinfo[kind].material_kind[i] );
			lua_rawset( servL, -3 );
		}
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "material_num" );
		lua_newtable( servL );
		for ( int i = 0; i < 6; i++ )
		{
			lua_pushinteger( servL, i );
			lua_pushinteger( servL, g_equipinfo[kind].material_num[i] );
			lua_rawset( servL, -3 );
		}
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "prestige" );
		lua_pushinteger( servL, g_equipinfo[kind].prestige );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "battlepower" );
		lua_pushinteger( servL, g_equipinfo[kind].battlepower );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "resolve_kind" );
		lua_newtable( servL );
		for ( int i = 0; i < 4; i++ )
		{
			lua_pushinteger( servL, i );
			lua_pushinteger( servL, g_equipinfo[kind].resolve_kind[i] );
			lua_rawset( servL, -3 );
		}
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "resolve_min" );
		lua_newtable( servL );
		for ( int i = 0; i < 4; i++ )
		{
			lua_pushinteger( servL, i );
			lua_pushinteger( servL, g_equipinfo[kind].resolve_min[i] );
			lua_rawset( servL, -3 );
		}
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "resolve_max" );
		lua_newtable( servL );
		for ( int i = 0; i < 4; i++ )
		{
			lua_pushinteger( servL, i );
			lua_pushinteger( servL, g_equipinfo[kind].resolve_max[i] );
			lua_rawset( servL, -3 );
		}
		lua_rawset( servL, -3 );

		lua_rawset( servL, 1 );
	}
	lua_setglobal( servL, "g_equipinfo" );

	lua_pushinteger( servL, g_equipinfo_maxnum );
	lua_setglobal( servL, "g_equipinfo_maxnum" );
	return 0;
}
