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
#include "auto_data_wishing_change.h"
extern MYSQL *myData;
extern lua_State* servL;
WishingChange *g_wishing_change = NULL;
int g_wishing_change_maxnum = 0;

int wishingchange_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from wishing_change;" );
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
			g_wishing_change_maxnum = atoi( row[0] ) + 1;
		else
			g_wishing_change_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_wishing_change = (WishingChange *)malloc( sizeof(WishingChange)*g_wishing_change_maxnum );
	memset( g_wishing_change, 0, sizeof(WishingChange)*g_wishing_change_maxnum );

	sprintf( szSQL, "select `id`,`awardkind0`,`awardkind1`,`awardnum0`,`awardnum1`,`costkind`,`costnum` from wishing_change;" );
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
		if ( id < 0 || id >= g_wishing_change_maxnum  )
			continue;
		g_wishing_change[id].id = atoi(row[offset++]);
		g_wishing_change[id].awardkind[0] = atoi(row[offset++]);
		g_wishing_change[id].awardkind[1] = atoi(row[offset++]);
		g_wishing_change[id].awardnum[0] = atoi(row[offset++]);
		g_wishing_change[id].awardnum[1] = atoi(row[offset++]);
		g_wishing_change[id].costkind = atoi(row[offset++]);
		g_wishing_change[id].costnum = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int wishingchange_reload_auto()
{
	if ( g_wishing_change )
	{
		free( g_wishing_change );
		g_wishing_change = NULL;
	}
	g_wishing_change_maxnum = 0;
	wishingchange_init_auto();
	return 0;
}

