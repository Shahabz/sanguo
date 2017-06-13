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
#include "auto_data_item.h"
extern MYSQL *myData;
extern lua_State* servL;
ItemKind *g_itemkind = NULL;
int g_itemkind_maxnum = 0;

int itemkind_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(kind) from item;" );
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
			g_itemkind_maxnum = atoi( row[0] ) + 1;
		else
			g_itemkind_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_itemkind = (ItemKind *)malloc( sizeof(ItemKind)*g_itemkind_maxnum );
	memset( g_itemkind, 0, sizeof(ItemKind)*g_itemkind_maxnum );

	sprintf( szSQL, "select `kind`,`name`,`level`,`price`,`type`,`situation`,`overlap`,`base_ability0`,`base_ability1`,`base_value0`,`base_value1`,`ability0`,`ability1`,`ability2`,`ability3`,`value_min0`,`value_min1`,`value_min2`,`value_min3`,`value_max0`,`value_max1`,`value_max2`,`value_max3`,`color_level` from item;" );
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
		if ( kind < 0 || kind >= g_itemkind_maxnum  )
			continue;
		g_itemkind[kind].m_kind = atoi(row[offset++]);
		memcpy( g_itemkind[kind].m_name, row[offset++], 32 ); g_itemkind[kind].m_name[31]=0;
		g_itemkind[kind].m_level = atoi(row[offset++]);
		g_itemkind[kind].m_price = atoi(row[offset++]);
		g_itemkind[kind].m_type = atoi(row[offset++]);
		g_itemkind[kind].m_situation = atoi(row[offset++]);
		g_itemkind[kind].m_overlap = atoi(row[offset++]);
		g_itemkind[kind].m_base_ability[0] = atoi(row[offset++]);
		g_itemkind[kind].m_base_ability[1] = atoi(row[offset++]);
		g_itemkind[kind].m_base_value[0] = atoi(row[offset++]);
		g_itemkind[kind].m_base_value[1] = atoi(row[offset++]);
		g_itemkind[kind].m_ability[0] = atoi(row[offset++]);
		g_itemkind[kind].m_ability[1] = atoi(row[offset++]);
		g_itemkind[kind].m_ability[2] = atoi(row[offset++]);
		g_itemkind[kind].m_ability[3] = atoi(row[offset++]);
		g_itemkind[kind].m_value_min[0] = atoi(row[offset++]);
		g_itemkind[kind].m_value_min[1] = atoi(row[offset++]);
		g_itemkind[kind].m_value_min[2] = atoi(row[offset++]);
		g_itemkind[kind].m_value_min[3] = atoi(row[offset++]);
		g_itemkind[kind].m_value_max[0] = atoi(row[offset++]);
		g_itemkind[kind].m_value_max[1] = atoi(row[offset++]);
		g_itemkind[kind].m_value_max[2] = atoi(row[offset++]);
		g_itemkind[kind].m_value_max[3] = atoi(row[offset++]);
		g_itemkind[kind].m_color_level = atoi(row[offset++]);
	}
	mysql_free_result( res );
	itemkind_luatable_auto();
	return 0;
}

int itemkind_reload_auto()
{
	if ( g_itemkind )
	{
		free( g_itemkind );
		g_itemkind = NULL;
	}
	g_itemkind_maxnum = 0;
	itemkind_init_auto();
	return 0;
}

int itemkind_luatable_auto()
{
	lua_newtable( servL );
	for ( int kind = 0; kind < g_itemkind_maxnum; kind++ )
	{
		lua_pushinteger( servL, kind );
		lua_newtable( servL );

		lua_pushstring( servL, "m_kind" );
		lua_pushinteger( servL, g_itemkind[kind].m_kind );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "m_name" );
		lua_pushstring( servL, g_itemkind[kind].m_name );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "m_level" );
		lua_pushinteger( servL, g_itemkind[kind].m_level );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "m_price" );
		lua_pushinteger( servL, g_itemkind[kind].m_price );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "m_type" );
		lua_pushinteger( servL, g_itemkind[kind].m_type );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "m_situation" );
		lua_pushinteger( servL, g_itemkind[kind].m_situation );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "m_overlap" );
		lua_pushinteger( servL, g_itemkind[kind].m_overlap );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "m_base_ability" );
		lua_newtable( servL );
		for ( int i = 0; i < 2; i++ )
		{
			lua_pushinteger( servL, i );
			lua_pushinteger( servL, g_itemkind[kind].m_base_ability[i] );
			lua_rawset( servL, -3 );
		}
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "m_base_value" );
		lua_newtable( servL );
		for ( int i = 0; i < 2; i++ )
		{
			lua_pushinteger( servL, i );
			lua_pushinteger( servL, g_itemkind[kind].m_base_value[i] );
			lua_rawset( servL, -3 );
		}
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "m_ability" );
		lua_newtable( servL );
		for ( int i = 0; i < 4; i++ )
		{
			lua_pushinteger( servL, i );
			lua_pushinteger( servL, g_itemkind[kind].m_ability[i] );
			lua_rawset( servL, -3 );
		}
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "m_value_min" );
		lua_newtable( servL );
		for ( int i = 0; i < 4; i++ )
		{
			lua_pushinteger( servL, i );
			lua_pushinteger( servL, g_itemkind[kind].m_value_min[i] );
			lua_rawset( servL, -3 );
		}
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "m_value_max" );
		lua_newtable( servL );
		for ( int i = 0; i < 4; i++ )
		{
			lua_pushinteger( servL, i );
			lua_pushinteger( servL, g_itemkind[kind].m_value_max[i] );
			lua_rawset( servL, -3 );
		}
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "m_color_level" );
		lua_pushinteger( servL, g_itemkind[kind].m_color_level );
		lua_rawset( servL, -3 );

		lua_rawset( servL, 1 );
	}
	lua_setglobal( servL, "g_itemkind" );

	lua_pushinteger( servL, g_itemkind_maxnum );
	lua_setglobal( servL, "g_itemkind_maxnum" );
	return 0;
}
