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
#include "auto_data_paygoods.h"
extern MYSQL *myData;
extern lua_State* servL;
PayGoods *g_paygoods = NULL;
int g_paygoods_maxnum = 0;

int paygoods_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(goodsid) from paygoods;" );
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
			g_paygoods_maxnum = atoi( row[0] ) + 1;
		else
			g_paygoods_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_paygoods = (PayGoods *)malloc( sizeof(PayGoods)*g_paygoods_maxnum );
	memset( g_paygoods, 0, sizeof(PayGoods)*g_paygoods_maxnum );

	sprintf( szSQL, "select `goodsid`,`productid`,`tier`,`token`,`awardgroup0`,`awardgroup1`,`awardgroup2`,`awardgroup3`,`awardgroup4`,`type`,`nameid`,`icon`,`sale`,`be_worth`,`stage`,`descid`,`point`,`nextid`,`nextcd`,`nextcount`,`clubaward` from paygoods;" );
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
		int goodsid = atoi( row[0] );
		if ( goodsid < 0 || goodsid >= g_paygoods_maxnum  )
			continue;
		g_paygoods[goodsid].goodsid = atoi(row[offset++]);
		g_paygoods[goodsid].productid = atoi(row[offset++]);
		g_paygoods[goodsid].tier = atoi(row[offset++]);
		g_paygoods[goodsid].token = atoi(row[offset++]);
		g_paygoods[goodsid].awardgroup[0] = atoi(row[offset++]);
		g_paygoods[goodsid].awardgroup[1] = atoi(row[offset++]);
		g_paygoods[goodsid].awardgroup[2] = atoi(row[offset++]);
		g_paygoods[goodsid].awardgroup[3] = atoi(row[offset++]);
		g_paygoods[goodsid].awardgroup[4] = atoi(row[offset++]);
		g_paygoods[goodsid].type = atoi(row[offset++]);
		g_paygoods[goodsid].nameid = atoi(row[offset++]);
		g_paygoods[goodsid].icon = atoi(row[offset++]);
		g_paygoods[goodsid].sale = (float)atof(row[offset++]);
		g_paygoods[goodsid].be_worth = atoi(row[offset++]);
		g_paygoods[goodsid].stage = atoi(row[offset++]);
		g_paygoods[goodsid].descid = atoi(row[offset++]);
		g_paygoods[goodsid].point = (float)atof(row[offset++]);
		g_paygoods[goodsid].nextid = atoi(row[offset++]);
		g_paygoods[goodsid].nextcd = atoi(row[offset++]);
		g_paygoods[goodsid].nextcount = atoi(row[offset++]);
		g_paygoods[goodsid].clubaward = atoi(row[offset++]);
	}
	mysql_free_result( res );
	paygoods_luatable_auto();
	return 0;
}

int paygoods_reload_auto()
{
	if ( g_paygoods )
	{
		free( g_paygoods );
		g_paygoods = NULL;
	}
	g_paygoods_maxnum = 0;
	paygoods_init_auto();
	return 0;
}

int paygoods_luatable_auto()
{
	lua_newtable( servL );
	for ( int goodsid = 0; goodsid < g_paygoods_maxnum; goodsid++ )
	{
		lua_pushinteger( servL, goodsid );
		lua_newtable( servL );

		lua_pushstring( servL, "goodsid" );
		lua_pushinteger( servL, g_paygoods[goodsid].goodsid );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "productid" );
		lua_pushinteger( servL, g_paygoods[goodsid].productid );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "tier" );
		lua_pushinteger( servL, g_paygoods[goodsid].tier );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "token" );
		lua_pushinteger( servL, g_paygoods[goodsid].token );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "awardgroup" );
		lua_newtable( servL );
		for ( int i = 0; i < 5; i++ )
		{
			lua_pushinteger( servL, i );
			lua_pushinteger( servL, g_paygoods[goodsid].awardgroup[i] );
			lua_rawset( servL, -3 );
		}
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "type" );
		lua_pushinteger( servL, g_paygoods[goodsid].type );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "nameid" );
		lua_pushinteger( servL, g_paygoods[goodsid].nameid );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "icon" );
		lua_pushinteger( servL, g_paygoods[goodsid].icon );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "sale" );
		lua_pushnumber( servL, g_paygoods[goodsid].sale );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "be_worth" );
		lua_pushinteger( servL, g_paygoods[goodsid].be_worth );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "stage" );
		lua_pushinteger( servL, g_paygoods[goodsid].stage );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "descid" );
		lua_pushinteger( servL, g_paygoods[goodsid].descid );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "point" );
		lua_pushnumber( servL, g_paygoods[goodsid].point );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "nextid" );
		lua_pushinteger( servL, g_paygoods[goodsid].nextid );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "nextcd" );
		lua_pushinteger( servL, g_paygoods[goodsid].nextcd );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "nextcount" );
		lua_pushinteger( servL, g_paygoods[goodsid].nextcount );
		lua_rawset( servL, -3 );

		lua_pushstring( servL, "clubaward" );
		lua_pushinteger( servL, g_paygoods[goodsid].clubaward );
		lua_rawset( servL, -3 );

		lua_rawset( servL, 1 );
	}
	lua_setglobal( servL, "g_paygoods" );

	lua_pushinteger( servL, g_paygoods_maxnum );
	lua_setglobal( servL, "g_paygoods_maxnum" );
	return 0;
}
