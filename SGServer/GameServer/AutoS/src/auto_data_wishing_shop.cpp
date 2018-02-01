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
#include "auto_data_wishing_shop.h"
extern MYSQL *myData;
extern lua_State* servL;
WishingShop *g_wishing_shop = NULL;
int g_wishing_shop_maxnum = 0;

int wishingshop_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from wishing_shop;" );
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
			g_wishing_shop_maxnum = atoi( row[0] ) + 1;
		else
			g_wishing_shop_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_wishing_shop = (WishingShop *)malloc( sizeof(WishingShop)*g_wishing_shop_maxnum );
	memset( g_wishing_shop, 0, sizeof(WishingShop)*g_wishing_shop_maxnum );

	sprintf( szSQL, "select `id`,`color`,`level`,`awardkind`,`awardnum`,`costkind`,`costnum`,`value` from wishing_shop;" );
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
		if ( id < 0 || id >= g_wishing_shop_maxnum  )
			continue;
		g_wishing_shop[id].id = atoi(row[offset++]);
		g_wishing_shop[id].color = atoi(row[offset++]);
		g_wishing_shop[id].level = atoi(row[offset++]);
		g_wishing_shop[id].awardkind = atoi(row[offset++]);
		g_wishing_shop[id].awardnum = atoi(row[offset++]);
		g_wishing_shop[id].costkind = atoi(row[offset++]);
		g_wishing_shop[id].costnum = atoi(row[offset++]);
		g_wishing_shop[id].value = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int wishingshop_reload_auto()
{
	if ( g_wishing_shop )
	{
		free( g_wishing_shop );
		g_wishing_shop = NULL;
	}
	g_wishing_shop_maxnum = 0;
	wishingshop_init_auto();
	return 0;
}

