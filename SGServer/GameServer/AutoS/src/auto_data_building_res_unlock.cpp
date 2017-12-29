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
#include "auto_data_building_res_unlock.h"
extern MYSQL *myData;
extern lua_State* servL;
BuildingResUnlock *g_building_res_unlock = NULL;
int g_building_res_unlock_maxnum = 0;

int buildingresunlock_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`kind`) from building_res_unlock;" );
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
			g_building_res_unlock_maxnum = atoi( row[0] ) + 1;
		else
			g_building_res_unlock_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_building_res_unlock = (BuildingResUnlock *)malloc( sizeof(BuildingResUnlock)*g_building_res_unlock_maxnum );
	memset( g_building_res_unlock, 0, sizeof(BuildingResUnlock)*g_building_res_unlock_maxnum );

	sprintf( szSQL, "select `kind`, max( `number` ) from building_res_unlock group by `kind`;" );
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
		if ( kind < 0 || kind >= g_building_res_unlock_maxnum  )
			continue;
		g_building_res_unlock[kind].maxnum = atoi( row[1] ) + 1;
		g_building_res_unlock[kind].config = (BuildingResUnlockConfig *)malloc( sizeof(BuildingResUnlockConfig)*g_building_res_unlock[kind].maxnum );
		memset( g_building_res_unlock[kind].config, 0, sizeof(BuildingResUnlockConfig)*g_building_res_unlock[kind].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `kind`,`number`,`itemkind`,`itemnum` from building_res_unlock;" );
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
		if ( kind < 0 || kind >= g_building_res_unlock_maxnum  )
			continue;
		int number = atoi( row[1] );
		if ( number < 0 || number >= g_building_res_unlock[kind].maxnum )
			continue;
		g_building_res_unlock[kind].config[number].kind = atoi(row[offset++]);
		g_building_res_unlock[kind].config[number].number = atoi(row[offset++]);
		g_building_res_unlock[kind].config[number].itemkind = atoi(row[offset++]);
		g_building_res_unlock[kind].config[number].itemnum = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int buildingresunlock_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_building_res_unlock_maxnum; tmpi++ )
	{
		if ( g_building_res_unlock[tmpi].config )
		{
			free( g_building_res_unlock[tmpi].config );
			g_building_res_unlock[tmpi].config = NULL;
		}
	}
	if ( g_building_res_unlock )
	{
		free( g_building_res_unlock );
		g_building_res_unlock = NULL;
	}
	g_building_res_unlock_maxnum = 0;
	buildingresunlock_init_auto();
	return 0;
}

