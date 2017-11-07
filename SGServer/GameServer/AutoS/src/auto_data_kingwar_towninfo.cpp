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
#include "auto_data_kingwar_towninfo.h"
extern MYSQL *myData;
extern lua_State* servL;
KingwarTownInfo *g_kingwar_towninfo = NULL;
int g_kingwar_towninfo_maxnum = 0;

int kingwartowninfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(id) from kingwar_towninfo;" );
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
			g_kingwar_towninfo_maxnum = atoi( row[0] ) + 1;
		else
			g_kingwar_towninfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_kingwar_towninfo = (KingwarTownInfo *)malloc( sizeof(KingwarTownInfo)*g_kingwar_towninfo_maxnum );
	memset( g_kingwar_towninfo, 0, sizeof(KingwarTownInfo)*g_kingwar_towninfo_maxnum );

	sprintf( szSQL, "select `id`,`type`,`base_nation`,`doorid` from kingwar_towninfo;" );
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
		if ( id < 0 || id >= g_kingwar_towninfo_maxnum  )
			continue;
		g_kingwar_towninfo[id].id = atoi(row[offset++]);
		g_kingwar_towninfo[id].type = atoi(row[offset++]);
		g_kingwar_towninfo[id].base_nation = atoi(row[offset++]);
		g_kingwar_towninfo[id].doorid = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int kingwartowninfo_reload_auto()
{
	if ( g_kingwar_towninfo )
	{
		free( g_kingwar_towninfo );
		g_kingwar_towninfo = NULL;
	}
	g_kingwar_towninfo_maxnum = 0;
	kingwartowninfo_init_auto();
	return 0;
}

