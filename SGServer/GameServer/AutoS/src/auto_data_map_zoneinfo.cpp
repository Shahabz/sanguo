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
#include "auto_data_map_zoneinfo.h"
extern MYSQL *myData;
extern lua_State* servL;
MapZoneInfo *g_zoneinfo = NULL;
int g_zoneinfo_maxnum = 0;

int mapzoneinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(id) from map_zoneinfo;" );
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
			g_zoneinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_zoneinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_zoneinfo = (MapZoneInfo *)malloc( sizeof(MapZoneInfo)*g_zoneinfo_maxnum );
	memset( g_zoneinfo, 0, sizeof(MapZoneInfo)*g_zoneinfo_maxnum );

	sprintf( szSQL, "select `id`,`open` from map_zoneinfo;" );
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
		if ( id < 0 || id >= g_zoneinfo_maxnum  )
			continue;
		g_zoneinfo[id].id = atoi(row[offset++]);
		g_zoneinfo[id].open = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int mapzoneinfo_reload_auto()
{
	if ( g_zoneinfo )
	{
		free( g_zoneinfo );
		g_zoneinfo = NULL;
	}
	g_zoneinfo_maxnum = 0;
	mapzoneinfo_init_auto();
	return 0;
}

