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
#include "auto_data_city_guard.h"
extern MYSQL *myData;
extern lua_State* servL;
CityGuardInfo *g_cityguardinfo = NULL;
int g_cityguardinfo_maxnum = 0;

int cityguardinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`monsterid`) from city_guard;" );
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
			g_cityguardinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_cityguardinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_cityguardinfo = (CityGuardInfo *)malloc( sizeof(CityGuardInfo)*g_cityguardinfo_maxnum );
	memset( g_cityguardinfo, 0, sizeof(CityGuardInfo)*g_cityguardinfo_maxnum );

	sprintf( szSQL, "select `monsterid`, max( `color` ) from city_guard group by `monsterid`;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		int monsterid = atoi( row[0] );
		if ( monsterid < 0 || monsterid >= g_cityguardinfo_maxnum  )
			continue;
		g_cityguardinfo[monsterid].maxnum = atoi( row[1] ) + 1;
		g_cityguardinfo[monsterid].config = (CityGuardInfoConfig *)malloc( sizeof(CityGuardInfoConfig)*g_cityguardinfo[monsterid].maxnum );
		memset( g_cityguardinfo[monsterid].config, 0, sizeof(CityGuardInfoConfig)*g_cityguardinfo[monsterid].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `monsterid`,`color`,`attack`,`defense`,`troops`,`attack_growth`,`defense_growth`,`troops_growth`,`attack_increase`,`defense_increase`,`assault`,`defend`,`line`,`skill` from city_guard;" );
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
		int monsterid = atoi( row[0] );
		if ( monsterid < 0 || monsterid >= g_cityguardinfo_maxnum  )
			continue;
		int color = atoi( row[1] );
		if ( color < 0 || color >= g_cityguardinfo[monsterid].maxnum )
			continue;
		g_cityguardinfo[monsterid].config[color].monsterid = atoi(row[offset++]);
		g_cityguardinfo[monsterid].config[color].color = atoi(row[offset++]);
		g_cityguardinfo[monsterid].config[color].attack = atoi(row[offset++]);
		g_cityguardinfo[monsterid].config[color].defense = atoi(row[offset++]);
		g_cityguardinfo[monsterid].config[color].troops = atoi(row[offset++]);
		g_cityguardinfo[monsterid].config[color].attack_growth = atoi(row[offset++]);
		g_cityguardinfo[monsterid].config[color].defense_growth = atoi(row[offset++]);
		g_cityguardinfo[monsterid].config[color].troops_growth = atoi(row[offset++]);
		g_cityguardinfo[monsterid].config[color].attack_increase = atoi(row[offset++]);
		g_cityguardinfo[monsterid].config[color].defense_increase = atoi(row[offset++]);
		g_cityguardinfo[monsterid].config[color].assault = atoi(row[offset++]);
		g_cityguardinfo[monsterid].config[color].defend = atoi(row[offset++]);
		g_cityguardinfo[monsterid].config[color].line = atoi(row[offset++]);
		g_cityguardinfo[monsterid].config[color].skill = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int cityguardinfo_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_cityguardinfo_maxnum; tmpi++ )
	{
		if ( g_cityguardinfo[tmpi].config )
		{
			free( g_cityguardinfo[tmpi].config );
			g_cityguardinfo[tmpi].config = NULL;
		}
	}
	if ( g_cityguardinfo )
	{
		free( g_cityguardinfo );
		g_cityguardinfo = NULL;
	}
	g_cityguardinfo_maxnum = 0;
	cityguardinfo_init_auto();
	return 0;
}

