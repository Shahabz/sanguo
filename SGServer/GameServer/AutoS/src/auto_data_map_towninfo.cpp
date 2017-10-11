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
#include "auto_data_map_towninfo.h"
extern MYSQL *myData;
extern lua_State* servL;
MapTownInfo *g_towninfo = NULL;
int g_towninfo_maxnum = 0;

int maptowninfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(id) from map_towninfo;" );
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
			g_towninfo_maxnum = atoi( row[0] ) + 1;
		else
			g_towninfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_towninfo = (MapTownInfo *)malloc( sizeof(MapTownInfo)*g_towninfo_maxnum );
	memset( g_towninfo, 0, sizeof(MapTownInfo)*g_towninfo_maxnum );

	sprintf( szSQL, "select `id`,`type`,`zoneid`,`posx`,`posy`,`grid`,`range`,`level`,`preid`,`base_award`,`other_award`,`produce_maxnum`,`produce_maxsec`,`levy_prestige`,`protect_maxsec`,`fight_maxsec`,`own_maxsec`,`ask_silver`,`ask_wood`,`ask_food`,`ask_iron`,`monster`,`monster_guard`,`fight_silver`,`fight_wood`,`fight_food` from map_towninfo;" );
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
		if ( id < 0 || id >= g_towninfo_maxnum  )
			continue;
		g_towninfo[id].id = atoi(row[offset++]);
		g_towninfo[id].type = atoi(row[offset++]);
		g_towninfo[id].zoneid = atoi(row[offset++]);
		g_towninfo[id].posx = atoi(row[offset++]);
		g_towninfo[id].posy = atoi(row[offset++]);
		g_towninfo[id].grid = atoi(row[offset++]);
		g_towninfo[id].range = atoi(row[offset++]);
		g_towninfo[id].level = atoi(row[offset++]);
		memcpy( g_towninfo[id].preid, row[offset++], 64 ); g_towninfo[id].preid[63]=0;
		g_towninfo[id].base_award = atoi(row[offset++]);
		g_towninfo[id].other_award = atoi(row[offset++]);
		g_towninfo[id].produce_maxnum = atoi(row[offset++]);
		g_towninfo[id].produce_maxsec = atoi(row[offset++]);
		g_towninfo[id].levy_prestige = atoi(row[offset++]);
		g_towninfo[id].protect_maxsec = atoi(row[offset++]);
		g_towninfo[id].fight_maxsec = atoi(row[offset++]);
		g_towninfo[id].own_maxsec = atoi(row[offset++]);
		g_towninfo[id].ask_silver = atoi(row[offset++]);
		g_towninfo[id].ask_wood = atoi(row[offset++]);
		g_towninfo[id].ask_food = atoi(row[offset++]);
		g_towninfo[id].ask_iron = atoi(row[offset++]);
		memcpy( g_towninfo[id].monster, row[offset++], 128 ); g_towninfo[id].monster[127]=0;
		memcpy( g_towninfo[id].monster_guard, row[offset++], 128 ); g_towninfo[id].monster_guard[127]=0;
		g_towninfo[id].fight_silver = atoi(row[offset++]);
		g_towninfo[id].fight_wood = atoi(row[offset++]);
		g_towninfo[id].fight_food = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int maptowninfo_reload_auto()
{
	if ( g_towninfo )
	{
		free( g_towninfo );
		g_towninfo = NULL;
	}
	g_towninfo_maxnum = 0;
	maptowninfo_init_auto();
	return 0;
}

