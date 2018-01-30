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
#include "auto_data_nation_heroinfo.h"
extern MYSQL *myData;
extern lua_State* servL;
NationHeroInfo *g_nation_heroinfo = NULL;
int g_nation_heroinfo_maxnum = 0;

int nationheroinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`herokind`) from nation_heroinfo;" );
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
			g_nation_heroinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_nation_heroinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_nation_heroinfo = (NationHeroInfo *)malloc( sizeof(NationHeroInfo)*g_nation_heroinfo_maxnum );
	memset( g_nation_heroinfo, 0, sizeof(NationHeroInfo)*g_nation_heroinfo_maxnum );

	sprintf( szSQL, "select `herokind`,`nation`,`nationlevel`,`offset`,`visit_token`,`other_visit_token`,`catch_odds`,`other_catch_odds`,`rob_odds`,`other_rob_odds`,`awardgroup`,`monsterid0`,`monsterid1`,`monsterid2`,`monsterid3`,`call_silver`,`call_itemkind0`,`call_itemkind1`,`call_itemkind2`,`call_itemkind3`,`call_itemnum`,`loyal_itemnum` from nation_heroinfo;" );
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
		int herokind = atoi( row[0] );
		if ( herokind < 0 || herokind >= g_nation_heroinfo_maxnum  )
			continue;
		g_nation_heroinfo[herokind].herokind = atoi(row[offset++]);
		g_nation_heroinfo[herokind].nation = atoi(row[offset++]);
		g_nation_heroinfo[herokind].nationlevel = atoi(row[offset++]);
		g_nation_heroinfo[herokind].offset = atoi(row[offset++]);
		g_nation_heroinfo[herokind].visit_token = atoi(row[offset++]);
		g_nation_heroinfo[herokind].other_visit_token = atoi(row[offset++]);
		g_nation_heroinfo[herokind].catch_odds = (float)atof(row[offset++]);
		g_nation_heroinfo[herokind].other_catch_odds = (float)atof(row[offset++]);
		g_nation_heroinfo[herokind].rob_odds = (float)atof(row[offset++]);
		g_nation_heroinfo[herokind].other_rob_odds = (float)atof(row[offset++]);
		g_nation_heroinfo[herokind].awardgroup = atoi(row[offset++]);
		g_nation_heroinfo[herokind].monsterid[0] = atoi(row[offset++]);
		g_nation_heroinfo[herokind].monsterid[1] = atoi(row[offset++]);
		g_nation_heroinfo[herokind].monsterid[2] = atoi(row[offset++]);
		g_nation_heroinfo[herokind].monsterid[3] = atoi(row[offset++]);
		g_nation_heroinfo[herokind].call_silver = atoi(row[offset++]);
		g_nation_heroinfo[herokind].call_itemkind[0] = atoi(row[offset++]);
		g_nation_heroinfo[herokind].call_itemkind[1] = atoi(row[offset++]);
		g_nation_heroinfo[herokind].call_itemkind[2] = atoi(row[offset++]);
		g_nation_heroinfo[herokind].call_itemkind[3] = atoi(row[offset++]);
		g_nation_heroinfo[herokind].call_itemnum = atoi(row[offset++]);
		g_nation_heroinfo[herokind].loyal_itemnum = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int nationheroinfo_reload_auto()
{
	if ( g_nation_heroinfo )
	{
		free( g_nation_heroinfo );
		g_nation_heroinfo = NULL;
	}
	g_nation_heroinfo_maxnum = 0;
	nationheroinfo_init_auto();
	return 0;
}

