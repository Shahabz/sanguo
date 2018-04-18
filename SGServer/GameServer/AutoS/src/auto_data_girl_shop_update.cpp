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
#include "auto_data_girl_shop_update.h"
extern MYSQL *myData;
extern lua_State* servL;
GirlShopUpdate *g_girlshop_update = NULL;
int g_girlshop_update_maxnum = 0;

int girlshopupdate_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`count`) from girl_shop_update;" );
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
			g_girlshop_update_maxnum = atoi( row[0] ) + 1;
		else
			g_girlshop_update_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_girlshop_update = (GirlShopUpdate *)malloc( sizeof(GirlShopUpdate)*g_girlshop_update_maxnum );
	memset( g_girlshop_update, 0, sizeof(GirlShopUpdate)*g_girlshop_update_maxnum );

	sprintf( szSQL, "select `count`,`token`,`viplevel` from girl_shop_update;" );
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
		int count = atoi( row[0] );
		if ( count < 0 || count >= g_girlshop_update_maxnum  )
			continue;
		g_girlshop_update[count].count = atoi(row[offset++]);
		g_girlshop_update[count].token = atoi(row[offset++]);
		g_girlshop_update[count].viplevel = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int girlshopupdate_reload_auto()
{
	if ( g_girlshop_update )
	{
		free( g_girlshop_update );
		g_girlshop_update = NULL;
	}
	g_girlshop_update_maxnum = 0;
	girlshopupdate_init_auto();
	return 0;
}

