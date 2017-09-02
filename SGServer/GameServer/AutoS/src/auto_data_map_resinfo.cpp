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
#include "auto_data_map_resinfo.h"
extern MYSQL *myData;
extern lua_State* servL;
MapResInfo *g_resinfo = NULL;
int g_resinfo_maxnum = 0;

int mapresinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(kind) from map_resinfo;" );
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
			g_resinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_resinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_resinfo = (MapResInfo *)malloc( sizeof(MapResInfo)*g_resinfo_maxnum );
	memset( g_resinfo, 0, sizeof(MapResInfo)*g_resinfo_maxnum );

	sprintf( szSQL, "select `kind`,`level`,`type`,`herolevel`,`num`,`sec` from map_resinfo;" );
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
		if ( kind < 0 || kind >= g_resinfo_maxnum  )
			continue;
		g_resinfo[kind].kind = atoi(row[offset++]);
		g_resinfo[kind].level = atoi(row[offset++]);
		g_resinfo[kind].type = atoi(row[offset++]);
		g_resinfo[kind].herolevel = atoi(row[offset++]);
		g_resinfo[kind].num = atoi(row[offset++]);
		g_resinfo[kind].sec = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int mapresinfo_reload_auto()
{
	if ( g_resinfo )
	{
		free( g_resinfo );
		g_resinfo = NULL;
	}
	g_resinfo_maxnum = 0;
	mapresinfo_init_auto();
	return 0;
}

