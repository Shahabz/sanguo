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
#include "auto_data_girl_shop.h"
extern MYSQL *myData;
extern lua_State* servL;
GirlShop *g_girlshop = NULL;
int g_girlshop_maxnum = 0;

int girlshop_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from girl_shop;" );
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
			g_girlshop_maxnum = atoi( row[0] ) + 1;
		else
			g_girlshop_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_girlshop = (GirlShop *)malloc( sizeof(GirlShop)*g_girlshop_maxnum );
	memset( g_girlshop, 0, sizeof(GirlShop)*g_girlshop_maxnum );

	sprintf( szSQL, "select `id`,`awardkind`,`awardnum`,`cost_awardkind`,`cost_awardnum`,`actorlevel`,`citylevel`,`viplevel`,`girlkind`,`weight`,`row` from girl_shop;" );
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
		if ( id < 0 || id >= g_girlshop_maxnum  )
			continue;
		g_girlshop[id].id = atoi(row[offset++]);
		g_girlshop[id].awardkind = atoi(row[offset++]);
		g_girlshop[id].awardnum = atoi(row[offset++]);
		g_girlshop[id].cost_awardkind = atoi(row[offset++]);
		g_girlshop[id].cost_awardnum = atoi(row[offset++]);
		g_girlshop[id].actorlevel = atoi(row[offset++]);
		g_girlshop[id].citylevel = atoi(row[offset++]);
		g_girlshop[id].viplevel = atoi(row[offset++]);
		g_girlshop[id].girlkind = atoi(row[offset++]);
		g_girlshop[id].weight = atoi(row[offset++]);
		g_girlshop[id].row = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int girlshop_reload_auto()
{
	if ( g_girlshop )
	{
		free( g_girlshop );
		g_girlshop = NULL;
	}
	g_girlshop_maxnum = 0;
	girlshop_init_auto();
	return 0;
}

