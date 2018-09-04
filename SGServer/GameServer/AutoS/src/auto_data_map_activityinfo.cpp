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
#include "auto_data_map_activityinfo.h"
extern MYSQL *myData;
extern lua_State* servL;
MapActivityInfo *g_activityinfo = NULL;
int g_activityinfo_maxnum = 0;

int mapactivityinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`kind`) from map_activityinfo;" );
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
			g_activityinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_activityinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_activityinfo = (MapActivityInfo *)malloc( sizeof(MapActivityInfo)*g_activityinfo_maxnum );
	memset( g_activityinfo, 0, sizeof(MapActivityInfo)*g_activityinfo_maxnum );

	sprintf( szSQL, "select `kind`,`type`,`level`,`shape`,`maxhp`,`activityid`,`awardgroup`,`monsterid0`,`monsterid1`,`monsterid2`,`monsterid3` from map_activityinfo;" );
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
		if ( kind < 0 || kind >= g_activityinfo_maxnum  )
			continue;
		g_activityinfo[kind].kind = atoi(row[offset++]);
		g_activityinfo[kind].type = atoi(row[offset++]);
		g_activityinfo[kind].level = atoi(row[offset++]);
		g_activityinfo[kind].shape = atoi(row[offset++]);
		g_activityinfo[kind].maxhp = atoi(row[offset++]);
		g_activityinfo[kind].activityid = atoi(row[offset++]);
		g_activityinfo[kind].awardgroup = atoi(row[offset++]);
		g_activityinfo[kind].monsterid[0] = atoi(row[offset++]);
		g_activityinfo[kind].monsterid[1] = atoi(row[offset++]);
		g_activityinfo[kind].monsterid[2] = atoi(row[offset++]);
		g_activityinfo[kind].monsterid[3] = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int mapactivityinfo_reload_auto()
{
	if ( g_activityinfo )
	{
		free( g_activityinfo );
		g_activityinfo = NULL;
	}
	g_activityinfo_maxnum = 0;
	mapactivityinfo_init_auto();
	return 0;
}

