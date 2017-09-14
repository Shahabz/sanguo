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
#include "auto_data_map_enemyinfo.h"
extern MYSQL *myData;
extern lua_State* servL;
MapEnemyInfo *g_enemyinfo = NULL;
int g_enemyinfo_maxnum = 0;

int mapenemyinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(kind) from map_enemyinfo;" );
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
			g_enemyinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_enemyinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_enemyinfo = (MapEnemyInfo *)malloc( sizeof(MapEnemyInfo)*g_enemyinfo_maxnum );
	memset( g_enemyinfo, 0, sizeof(MapEnemyInfo)*g_enemyinfo_maxnum );

	sprintf( szSQL, "select `kind`,`level`,`first_awardgroup`,`awardgroup`,`monsterid0`,`monsterid1`,`monsterid2`,`monsterid3` from map_enemyinfo;" );
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
		if ( kind < 0 || kind >= g_enemyinfo_maxnum  )
			continue;
		g_enemyinfo[kind].kind = atoi(row[offset++]);
		g_enemyinfo[kind].level = atoi(row[offset++]);
		g_enemyinfo[kind].first_awardgroup = atoi(row[offset++]);
		g_enemyinfo[kind].awardgroup = atoi(row[offset++]);
		g_enemyinfo[kind].monsterid[0] = atoi(row[offset++]);
		g_enemyinfo[kind].monsterid[1] = atoi(row[offset++]);
		g_enemyinfo[kind].monsterid[2] = atoi(row[offset++]);
		g_enemyinfo[kind].monsterid[3] = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int mapenemyinfo_reload_auto()
{
	if ( g_enemyinfo )
	{
		free( g_enemyinfo );
		g_enemyinfo = NULL;
	}
	g_enemyinfo_maxnum = 0;
	mapenemyinfo_init_auto();
	return 0;
}

