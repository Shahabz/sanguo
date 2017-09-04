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
#include "auto_data_map_eventinfo.h"
extern MYSQL *myData;
extern lua_State* servL;
MapEventInfo *g_eventinfo = NULL;
int g_eventinfo_maxnum = 0;

int mapeventinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(kind) from map_eventinfo;" );
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
			g_eventinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_eventinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_eventinfo = (MapEventInfo *)malloc( sizeof(MapEventInfo)*g_eventinfo_maxnum );
	memset( g_eventinfo, 0, sizeof(MapEventInfo)*g_eventinfo_maxnum );

	sprintf( szSQL, "select `kind`,`type`,`waitsec`,`costtype`,`costnum`,`awardkind`,`awardnum` from map_eventinfo;" );
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
		if ( kind < 0 || kind >= g_eventinfo_maxnum  )
			continue;
		g_eventinfo[kind].kind = atoi(row[offset++]);
		g_eventinfo[kind].type = atoi(row[offset++]);
		g_eventinfo[kind].waitsec = atoi(row[offset++]);
		g_eventinfo[kind].costtype = atoi(row[offset++]);
		g_eventinfo[kind].costnum = atoi(row[offset++]);
		g_eventinfo[kind].awardkind = atoi(row[offset++]);
		g_eventinfo[kind].awardnum = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int mapeventinfo_reload_auto()
{
	if ( g_eventinfo )
	{
		free( g_eventinfo );
		g_eventinfo = NULL;
	}
	g_eventinfo_maxnum = 0;
	mapeventinfo_init_auto();
	return 0;
}

