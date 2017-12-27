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
#include "auto_data_paystore.h"
extern MYSQL *myData;
extern lua_State* servL;
PayStore *g_paystore = NULL;
int g_paystore_maxnum = 0;

int paystore_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(id) from paystore;" );
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
			g_paystore_maxnum = atoi( row[0] ) + 1;
		else
			g_paystore_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_paystore = (PayStore *)malloc( sizeof(PayStore)*g_paystore_maxnum );
	memset( g_paystore, 0, sizeof(PayStore)*g_paystore_maxnum );

	sprintf( szSQL, "select `id`,`goodsid`,`limitbuy_gifttoken`,`limitbuy_saveindex`,`limitbuy_count`,`everyday_gifttoken`,`everyday_saveindex`,`everyday_count` from paystore;" );
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
		if ( id < 0 || id >= g_paystore_maxnum  )
			continue;
		g_paystore[id].id = atoi(row[offset++]);
		g_paystore[id].goodsid = atoi(row[offset++]);
		g_paystore[id].limitbuy_gifttoken = atoi(row[offset++]);
		g_paystore[id].limitbuy_saveindex = atoi(row[offset++]);
		g_paystore[id].limitbuy_count = atoi(row[offset++]);
		g_paystore[id].everyday_gifttoken = atoi(row[offset++]);
		g_paystore[id].everyday_saveindex = atoi(row[offset++]);
		g_paystore[id].everyday_count = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int paystore_reload_auto()
{
	if ( g_paystore )
	{
		free( g_paystore );
		g_paystore = NULL;
	}
	g_paystore_maxnum = 0;
	paystore_init_auto();
	return 0;
}

