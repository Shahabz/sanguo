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
#include "auto_data_world_bossinfo.h"
extern MYSQL *myData;
extern lua_State* servL;
WorldBossInfo *g_worldbossinfo = NULL;
int g_worldbossinfo_maxnum = 0;

int worldbossinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`bossid`) from world_bossinfo;" );
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
			g_worldbossinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_worldbossinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_worldbossinfo = (WorldBossInfo *)malloc( sizeof(WorldBossInfo)*g_worldbossinfo_maxnum );
	memset( g_worldbossinfo, 0, sizeof(WorldBossInfo)*g_worldbossinfo_maxnum );

	sprintf( szSQL, "select `bossid`,`monster`,`hp`,`exp`,`actorlevel`,`awardgroup`,`kill_awardgroup` from world_bossinfo;" );
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
		int bossid = atoi( row[0] );
		if ( bossid < 0 || bossid >= g_worldbossinfo_maxnum  )
			continue;
		g_worldbossinfo[bossid].bossid = atoi(row[offset++]);
		memcpy( g_worldbossinfo[bossid].monster, row[offset++], 128 ); g_worldbossinfo[bossid].monster[127]=0;
		g_worldbossinfo[bossid].hp = atoi(row[offset++]);
		g_worldbossinfo[bossid].exp = atoi(row[offset++]);
		g_worldbossinfo[bossid].actorlevel = atoi(row[offset++]);
		g_worldbossinfo[bossid].awardgroup = atoi(row[offset++]);
		g_worldbossinfo[bossid].kill_awardgroup = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int worldbossinfo_reload_auto()
{
	if ( g_worldbossinfo )
	{
		free( g_worldbossinfo );
		g_worldbossinfo = NULL;
	}
	g_worldbossinfo_maxnum = 0;
	worldbossinfo_init_auto();
	return 0;
}

