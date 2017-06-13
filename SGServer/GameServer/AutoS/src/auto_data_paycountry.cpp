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
#include "auto_data_paycountry.h"
extern MYSQL *myData;
extern lua_State* servL;
PayCountry *g_paycountry = NULL;
int g_paycountry_maxnum = 0;

int paycountry_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(country_code) from paycountry;" );
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
			g_paycountry_maxnum = atoi( row[0] ) + 1;
		else
			g_paycountry_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_paycountry = (PayCountry *)malloc( sizeof(PayCountry)*g_paycountry_maxnum );
	memset( g_paycountry, 0, sizeof(PayCountry)*g_paycountry_maxnum );

	sprintf( szSQL, "select `country_code`,`country_str`,`offset` from paycountry;" );
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
		int country_code = atoi( row[0] );
		if ( country_code < 0 || country_code >= g_paycountry_maxnum  )
			continue;
		g_paycountry[country_code].country_code = atoi(row[offset++]);
		memcpy( g_paycountry[country_code].country_str, row[offset++], 3 ); g_paycountry[country_code].country_str[2]=0;
		g_paycountry[country_code].offset = atoi(row[offset++]);
	}
	mysql_free_result( res );
	paycountry_luatable_auto();
	return 0;
}

int paycountry_reload_auto()
{
	if ( g_paycountry )
	{
		free( g_paycountry );
		g_paycountry = NULL;
	}
	g_paycountry_maxnum = 0;
	paycountry_init_auto();
	return 0;
}

int paycountry_luatable_auto()
{
	lua_newtable( servL );
	for ( int country_code = 0; country_code < g_paycountry_maxnum; country_code++ )
	{
		lua_pushinteger( servL, country_code );
		lua_newtable( servL );

		lua_pushstring( servL, "country_code" );
		lua_pushinteger( servL, g_paycountry[country_code].country_code );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "country_str" );
		lua_pushstring( servL, g_paycountry[country_code].country_str );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "offset" );
		lua_pushinteger( servL, g_paycountry[country_code].offset );
		lua_rawset( servL, -3 );

		lua_rawset( servL, 1 );
	}
	lua_setglobal( servL, "g_paycountry" );

	lua_pushinteger( servL, g_paycountry_maxnum );
	lua_setglobal( servL, "g_paycountry_maxnum" );
	return 0;
}
