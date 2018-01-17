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
#include "auto_data_nation_mission.h"
extern MYSQL *myData;
extern lua_State* servL;
NationMission *g_nation_mission = NULL;
int g_nation_mission_maxnum = 0;

int nationmission_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`level`) from nation_mission;" );
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
			g_nation_mission_maxnum = atoi( row[0] ) + 1;
		else
			g_nation_mission_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_nation_mission = (NationMission *)malloc( sizeof(NationMission)*g_nation_mission_maxnum );
	memset( g_nation_mission, 0, sizeof(NationMission)*g_nation_mission_maxnum );

	sprintf( szSQL, "select `level`, max( `kind` ) from nation_mission group by `level`;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		int level = atoi( row[0] );
		if ( level < 0 || level >= g_nation_mission_maxnum  )
			continue;
		g_nation_mission[level].maxnum = atoi( row[1] ) + 1;
		g_nation_mission[level].config = (NationMissionConfig *)malloc( sizeof(NationMissionConfig)*g_nation_mission[level].maxnum );
		memset( g_nation_mission[level].config, 0, sizeof(NationMissionConfig)*g_nation_mission[level].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `level`,`kind`,`needvalue0`,`needvalue1`,`needvalue2`,`awardkind0`,`awardkind1`,`awardnum0`,`awardnum1` from nation_mission;" );
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
		int level = atoi( row[0] );
		if ( level < 0 || level >= g_nation_mission_maxnum  )
			continue;
		int kind = atoi( row[1] );
		if ( kind < 0 || kind >= g_nation_mission[level].maxnum )
			continue;
		g_nation_mission[level].config[kind].level = atoi(row[offset++]);
		g_nation_mission[level].config[kind].kind = atoi(row[offset++]);
		g_nation_mission[level].config[kind].needvalue[0] = atoi(row[offset++]);
		g_nation_mission[level].config[kind].needvalue[1] = atoi(row[offset++]);
		g_nation_mission[level].config[kind].needvalue[2] = atoi(row[offset++]);
		g_nation_mission[level].config[kind].awardkind[0] = atoi(row[offset++]);
		g_nation_mission[level].config[kind].awardkind[1] = atoi(row[offset++]);
		g_nation_mission[level].config[kind].awardnum[0] = atoi(row[offset++]);
		g_nation_mission[level].config[kind].awardnum[1] = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int nationmission_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_nation_mission_maxnum; tmpi++ )
	{
		if ( g_nation_mission[tmpi].config )
		{
			free( g_nation_mission[tmpi].config );
			g_nation_mission[tmpi].config = NULL;
		}
	}
	if ( g_nation_mission )
	{
		free( g_nation_mission );
		g_nation_mission = NULL;
	}
	g_nation_mission_maxnum = 0;
	nationmission_init_auto();
	return 0;
}

