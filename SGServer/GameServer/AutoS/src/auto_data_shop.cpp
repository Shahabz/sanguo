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
#include "auto_data_shop.h"
extern MYSQL *myData;
extern lua_State* servL;
Shop *g_shop = NULL;
int g_shop_maxnum = 0;

int shop_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`type`) from shop;" );
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
			g_shop_maxnum = atoi( row[0] ) + 1;
		else
			g_shop_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_shop = (Shop *)malloc( sizeof(Shop)*g_shop_maxnum );
	memset( g_shop, 0, sizeof(Shop)*g_shop_maxnum );

	sprintf( szSQL, "select `type`, max( `index` ) from shop group by `type`;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		int type = atoi( row[0] );
		if ( type < 0 || type >= g_shop_maxnum  )
			continue;
		g_shop[type].maxnum = atoi( row[1] ) + 1;
		g_shop[type].config = (ShopConfig *)malloc( sizeof(ShopConfig)*g_shop[type].maxnum );
		memset( g_shop[type].config, 0, sizeof(ShopConfig)*g_shop[type].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `type`,`index`,`awardkind`,`awardnum`,`token`,`original_token`,`actorlevel_min`,`actorlevel_max`,`sale`,`buyuse` from shop;" );
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
		int type = atoi( row[0] );
		if ( type < 0 || type >= g_shop_maxnum  )
			continue;
		int index = atoi( row[1] );
		if ( index < 0 || index >= g_shop[type].maxnum )
			continue;
		g_shop[type].config[index].type = atoi(row[offset++]);
		g_shop[type].config[index].index = atoi(row[offset++]);
		g_shop[type].config[index].awardkind = atoi(row[offset++]);
		g_shop[type].config[index].awardnum = atoi(row[offset++]);
		g_shop[type].config[index].token = atoi(row[offset++]);
		g_shop[type].config[index].original_token = atoi(row[offset++]);
		g_shop[type].config[index].actorlevel_min = atoi(row[offset++]);
		g_shop[type].config[index].actorlevel_max = atoi(row[offset++]);
		g_shop[type].config[index].sale = atoi(row[offset++]);
		g_shop[type].config[index].buyuse = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int shop_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_shop_maxnum; tmpi++ )
	{
		if ( g_shop[tmpi].config )
		{
			free( g_shop[tmpi].config );
			g_shop[tmpi].config = NULL;
		}
	}
	if ( g_shop )
	{
		free( g_shop );
		g_shop = NULL;
	}
	g_shop_maxnum = 0;
	shop_init_auto();
	return 0;
}

