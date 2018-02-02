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
#include "auto_data_wishing_pack.h"
extern MYSQL *myData;
extern lua_State* servL;
WishingPack *g_wishing_pack = NULL;
int g_wishing_pack_maxnum = 0;

int wishingpack_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from wishing_pack;" );
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
			g_wishing_pack_maxnum = atoi( row[0] ) + 1;
		else
			g_wishing_pack_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_wishing_pack = (WishingPack *)malloc( sizeof(WishingPack)*g_wishing_pack_maxnum );
	memset( g_wishing_pack, 0, sizeof(WishingPack)*g_wishing_pack_maxnum );

	sprintf( szSQL, "select `id`,`awardkind`,`awardnum`,`costkind`,`costnum`,`token` from wishing_pack;" );
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
		if ( id < 0 || id >= g_wishing_pack_maxnum  )
			continue;
		g_wishing_pack[id].id = atoi(row[offset++]);
		g_wishing_pack[id].awardkind = atoi(row[offset++]);
		g_wishing_pack[id].awardnum = atoi(row[offset++]);
		g_wishing_pack[id].costkind = atoi(row[offset++]);
		g_wishing_pack[id].costnum = atoi(row[offset++]);
		g_wishing_pack[id].token = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int wishingpack_reload_auto()
{
	if ( g_wishing_pack )
	{
		free( g_wishing_pack );
		g_wishing_pack = NULL;
	}
	g_wishing_pack_maxnum = 0;
	wishingpack_init_auto();
	return 0;
}

