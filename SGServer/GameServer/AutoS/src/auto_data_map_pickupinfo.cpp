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
#include "auto_data_map_pickupinfo.h"
extern MYSQL *myData;
extern lua_State* servL;
MapPickupInfo *g_pickupinfo = NULL;
int g_pickupinfo_maxnum = 0;

int mappickupinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`kind`) from map_pickupinfo;" );
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
			g_pickupinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_pickupinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_pickupinfo = (MapPickupInfo *)malloc( sizeof(MapPickupInfo)*g_pickupinfo_maxnum );
	memset( g_pickupinfo, 0, sizeof(MapPickupInfo)*g_pickupinfo_maxnum );

	sprintf( szSQL, "select `kind`,`awardkind`,`numtype`,`minnum`,`maxnum` from map_pickupinfo;" );
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
		if ( kind < 0 || kind >= g_pickupinfo_maxnum  )
			continue;
		g_pickupinfo[kind].kind = atoi(row[offset++]);
		g_pickupinfo[kind].awardkind = atoi(row[offset++]);
		g_pickupinfo[kind].numtype = atoi(row[offset++]);
		g_pickupinfo[kind].minnum = atoi(row[offset++]);
		g_pickupinfo[kind].maxnum = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int mappickupinfo_reload_auto()
{
	if ( g_pickupinfo )
	{
		free( g_pickupinfo );
		g_pickupinfo = NULL;
	}
	g_pickupinfo_maxnum = 0;
	mappickupinfo_init_auto();
	return 0;
}

