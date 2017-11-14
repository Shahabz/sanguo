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
#include "auto_data_kingwar.h"
extern MYSQL *myData;
extern lua_State* servL;
KingWarConfig *g_kingwar_config = NULL;
int g_kingwar_config_maxnum = 0;

int kingwarconfig_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(id) from kingwar;" );
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
			g_kingwar_config_maxnum = atoi( row[0] ) + 1;
		else
			g_kingwar_config_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_kingwar_config = (KingWarConfig *)malloc( sizeof(KingWarConfig)*g_kingwar_config_maxnum );
	memset( g_kingwar_config, 0, sizeof(KingWarConfig)*g_kingwar_config_maxnum );

	sprintf( szSQL, "select `id`,`exp`,`drawing_num`,`rank_award_point`,`totalkill`,`totalkill_award_point`,`treasure_donum`,`treasure_costpoint`,`treasure_costtoken`,`treasure_award`,`treasure_equipodds`,`exchange_item`,`exchange_point` from kingwar;" );
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
		if ( id < 0 || id >= g_kingwar_config_maxnum  )
			continue;
		g_kingwar_config[id].id = atoi(row[offset++]);
		g_kingwar_config[id].exp = atoi(row[offset++]);
		g_kingwar_config[id].drawing_num = atoi(row[offset++]);
		g_kingwar_config[id].rank_award_point = atoi(row[offset++]);
		g_kingwar_config[id].totalkill = atoi(row[offset++]);
		g_kingwar_config[id].totalkill_award_point = atoi(row[offset++]);
		g_kingwar_config[id].treasure_donum = atoi(row[offset++]);
		g_kingwar_config[id].treasure_costpoint = atoi(row[offset++]);
		g_kingwar_config[id].treasure_costtoken = atoi(row[offset++]);
		g_kingwar_config[id].treasure_award = atoi(row[offset++]);
		g_kingwar_config[id].treasure_equipodds = atoi(row[offset++]);
		g_kingwar_config[id].exchange_item = atoi(row[offset++]);
		g_kingwar_config[id].exchange_point = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int kingwarconfig_reload_auto()
{
	if ( g_kingwar_config )
	{
		free( g_kingwar_config );
		g_kingwar_config = NULL;
	}
	g_kingwar_config_maxnum = 0;
	kingwarconfig_init_auto();
	return 0;
}

