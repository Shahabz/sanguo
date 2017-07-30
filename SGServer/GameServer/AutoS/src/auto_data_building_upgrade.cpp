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
#include "auto_data_building_upgrade.h"
extern MYSQL *myData;
extern lua_State* servL;
BuildingUpgrade *g_building_upgrade = NULL;
int g_building_upgrade_maxnum = 0;

int buildingupgrade_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(kind) from building_upgrade;" );
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
			g_building_upgrade_maxnum = atoi( row[0] ) + 1;
		else
			g_building_upgrade_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_building_upgrade = (BuildingUpgrade *)malloc( sizeof(BuildingUpgrade)*g_building_upgrade_maxnum );
	memset( g_building_upgrade, 0, sizeof(BuildingUpgrade)*g_building_upgrade_maxnum );

	sprintf( szSQL, "select kind, max( level ) from building_upgrade group by kind;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		int kind = atoi( row[0] );
		if ( kind < 0 || kind >= g_building_upgrade_maxnum  )
			continue;
		g_building_upgrade[kind].maxnum = atoi( row[1] ) + 1;
		g_building_upgrade[kind].config = (BuildingUpgradeConfig *)malloc( sizeof(BuildingUpgradeConfig)*g_building_upgrade[kind].maxnum );
		memset( g_building_upgrade[kind].config, 0, sizeof(BuildingUpgradeConfig)*g_building_upgrade[kind].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `kind`,`level`,`citylevel`,`actorlevel`,`silver`,`wood`,`food`,`iron`,`sec`,`value0`,`value1`,`value2`,`value3`,`value4`,`value5`,`value6`,`value7`,`exp`,`battlepower` from building_upgrade;" );
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
		if ( kind < 0 || kind >= g_building_upgrade_maxnum  )
			continue;
		int level = atoi( row[1] );
		if ( level < 0 || level >= g_building_upgrade[kind].maxnum )
			continue;
		g_building_upgrade[kind].config[level].kind = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].level = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].citylevel = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].actorlevel = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].silver = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].wood = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].food = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].iron = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].sec = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].value[0] = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].value[1] = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].value[2] = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].value[3] = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].value[4] = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].value[5] = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].value[6] = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].value[7] = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].exp = atoi(row[offset++]);
		g_building_upgrade[kind].config[level].battlepower = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int buildingupgrade_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_building_upgrade_maxnum; tmpi++ )
	{
		if ( g_building_upgrade[tmpi].config )
		{
			free( g_building_upgrade[tmpi].config );
			g_building_upgrade[tmpi].config = NULL;
		}
	}
	if ( g_building_upgrade )
	{
		free( g_building_upgrade );
		g_building_upgrade = NULL;
	}
	g_building_upgrade_maxnum = 0;
	buildingupgrade_init_auto();
	return 0;
}

