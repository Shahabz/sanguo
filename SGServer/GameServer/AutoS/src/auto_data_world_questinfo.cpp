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
#include "auto_data_world_questinfo.h"
extern MYSQL *myData;
extern lua_State* servL;
WorldQuestInfo *g_worldquestinfo = NULL;
int g_worldquestinfo_maxnum = 0;

int worldquestinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(questid) from world_questinfo;" );
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
			g_worldquestinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_worldquestinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_worldquestinfo = (WorldQuestInfo *)malloc( sizeof(WorldQuestInfo)*g_worldquestinfo_maxnum );
	memset( g_worldquestinfo, 0, sizeof(WorldQuestInfo)*g_worldquestinfo_maxnum );

	sprintf( szSQL, "select `questid`,`type`,`saveindex`,`sort`,`preid`,`value`,`awardgroup` from world_questinfo;" );
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
		int questid = atoi( row[0] );
		if ( questid < 0 || questid >= g_worldquestinfo_maxnum  )
			continue;
		g_worldquestinfo[questid].questid = atoi(row[offset++]);
		g_worldquestinfo[questid].type = atoi(row[offset++]);
		g_worldquestinfo[questid].saveindex = atoi(row[offset++]);
		g_worldquestinfo[questid].sort = atoi(row[offset++]);
		g_worldquestinfo[questid].preid = atoi(row[offset++]);
		g_worldquestinfo[questid].value = atoi(row[offset++]);
		g_worldquestinfo[questid].awardgroup = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int worldquestinfo_reload_auto()
{
	if ( g_worldquestinfo )
	{
		free( g_worldquestinfo );
		g_worldquestinfo = NULL;
	}
	g_worldquestinfo_maxnum = 0;
	worldquestinfo_init_auto();
	return 0;
}

