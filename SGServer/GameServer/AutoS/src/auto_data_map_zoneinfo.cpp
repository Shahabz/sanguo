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
#include "auto_data_map_zoneinfo.h"
extern MYSQL *myData;
extern lua_State* servL;
MapZoneInfo *g_zoneinfo = NULL;
int g_zoneinfo_maxnum = 0;

int mapzoneinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(id) from map_zoneinfo;" );
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
			g_zoneinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_zoneinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_zoneinfo = (MapZoneInfo *)malloc( sizeof(MapZoneInfo)*g_zoneinfo_maxnum );
	memset( g_zoneinfo, 0, sizeof(MapZoneInfo)*g_zoneinfo_maxnum );

	sprintf( szSQL, "select `id`,`open`,`center_posx`,`center_posy`,`top_left_posx`,`top_left_posy`,`top_right_posx`,`top_right_posy`,`bottom_left_posx`,`bottom_left_posy`,`bottom_right_posx`,`bottom_right_posy`,`enemykind`,`enemynum`,`reskind`,`resnum`,`eventkind`,`eventnum`,`actorlevel`,`killenemy`,`questid` from map_zoneinfo;" );
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
		int id = atoi( row[0] );
		if ( id < 0 || id >= g_zoneinfo_maxnum  )
			continue;
		g_zoneinfo[id].id = atoi(row[offset++]);
		g_zoneinfo[id].open = atoi(row[offset++]);
		g_zoneinfo[id].center_posx = atoi(row[offset++]);
		g_zoneinfo[id].center_posy = atoi(row[offset++]);
		g_zoneinfo[id].top_left_posx = atoi(row[offset++]);
		g_zoneinfo[id].top_left_posy = atoi(row[offset++]);
		g_zoneinfo[id].top_right_posx = atoi(row[offset++]);
		g_zoneinfo[id].top_right_posy = atoi(row[offset++]);
		g_zoneinfo[id].bottom_left_posx = atoi(row[offset++]);
		g_zoneinfo[id].bottom_left_posy = atoi(row[offset++]);
		g_zoneinfo[id].bottom_right_posx = atoi(row[offset++]);
		g_zoneinfo[id].bottom_right_posy = atoi(row[offset++]);
		memcpy( g_zoneinfo[id].enemykind, row[offset++], 256 ); g_zoneinfo[id].enemykind[255]=0;
		memcpy( g_zoneinfo[id].enemynum, row[offset++], 512 ); g_zoneinfo[id].enemynum[511]=0;
		memcpy( g_zoneinfo[id].reskind, row[offset++], 256 ); g_zoneinfo[id].reskind[255]=0;
		memcpy( g_zoneinfo[id].resnum, row[offset++], 512 ); g_zoneinfo[id].resnum[511]=0;
		memcpy( g_zoneinfo[id].eventkind, row[offset++], 256 ); g_zoneinfo[id].eventkind[255]=0;
		g_zoneinfo[id].eventnum = atoi(row[offset++]);
		g_zoneinfo[id].actorlevel = atoi(row[offset++]);
		g_zoneinfo[id].killenemy = atoi(row[offset++]);
		g_zoneinfo[id].questid = atoi(row[offset++]);
	}
	mysql_free_result( res );
	mapzoneinfo_luatable_auto();
	return 0;
}

int mapzoneinfo_reload_auto()
{
	if ( g_zoneinfo )
	{
		free( g_zoneinfo );
		g_zoneinfo = NULL;
	}
	g_zoneinfo_maxnum = 0;
	mapzoneinfo_init_auto();
	return 0;
}

int mapzoneinfo_luatable_auto()
{
	lua_newtable( servL );
	for ( int id = 0; id < g_zoneinfo_maxnum; id++ )
	{
		lua_pushinteger( servL, id );
		lua_newtable( servL );

		lua_pushstring( servL, "id" );
		lua_pushinteger( servL, g_zoneinfo[id].id );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "open" );
		lua_pushinteger( servL, g_zoneinfo[id].open );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "center_posx" );
		lua_pushinteger( servL, g_zoneinfo[id].center_posx );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "center_posy" );
		lua_pushinteger( servL, g_zoneinfo[id].center_posy );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "top_left_posx" );
		lua_pushinteger( servL, g_zoneinfo[id].top_left_posx );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "top_left_posy" );
		lua_pushinteger( servL, g_zoneinfo[id].top_left_posy );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "top_right_posx" );
		lua_pushinteger( servL, g_zoneinfo[id].top_right_posx );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "top_right_posy" );
		lua_pushinteger( servL, g_zoneinfo[id].top_right_posy );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "bottom_left_posx" );
		lua_pushinteger( servL, g_zoneinfo[id].bottom_left_posx );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "bottom_left_posy" );
		lua_pushinteger( servL, g_zoneinfo[id].bottom_left_posy );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "bottom_right_posx" );
		lua_pushinteger( servL, g_zoneinfo[id].bottom_right_posx );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "bottom_right_posy" );
		lua_pushinteger( servL, g_zoneinfo[id].bottom_right_posy );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "enemykind" );
		lua_pushstring( servL, g_zoneinfo[id].enemykind );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "enemynum" );
		lua_pushstring( servL, g_zoneinfo[id].enemynum );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "reskind" );
		lua_pushstring( servL, g_zoneinfo[id].reskind );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "resnum" );
		lua_pushstring( servL, g_zoneinfo[id].resnum );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "eventkind" );
		lua_pushstring( servL, g_zoneinfo[id].eventkind );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "eventnum" );
		lua_pushinteger( servL, g_zoneinfo[id].eventnum );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "actorlevel" );
		lua_pushinteger( servL, g_zoneinfo[id].actorlevel );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "killenemy" );
		lua_pushinteger( servL, g_zoneinfo[id].killenemy );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "questid" );
		lua_pushinteger( servL, g_zoneinfo[id].questid );
		lua_rawset( servL, -3 );

		lua_rawset( servL, 1 );
	}
	lua_setglobal( servL, "g_zoneinfo" );

	lua_pushinteger( servL, g_zoneinfo_maxnum );
	lua_setglobal( servL, "g_zoneinfo_maxnum" );
	return 0;
}
