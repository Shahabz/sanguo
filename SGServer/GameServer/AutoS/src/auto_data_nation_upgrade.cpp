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
#include "auto_data_nation_upgrade.h"
extern MYSQL *myData;
extern lua_State* servL;
NationUpgrade *g_nation_upgrade = NULL;
int g_nation_upgrade_maxnum = 0;

int nationupgrade_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`level`) from nation_upgrade;" );
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
			g_nation_upgrade_maxnum = atoi( row[0] ) + 1;
		else
			g_nation_upgrade_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_nation_upgrade = (NationUpgrade *)malloc( sizeof(NationUpgrade)*g_nation_upgrade_maxnum );
	memset( g_nation_upgrade, 0, sizeof(NationUpgrade)*g_nation_upgrade_maxnum );

	sprintf( szSQL, "select `level`, max( `num` ) from nation_upgrade group by `level`;" );
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
		if ( level < 0 || level >= g_nation_upgrade_maxnum  )
			continue;
		g_nation_upgrade[level].maxnum = atoi( row[1] ) + 1;
		g_nation_upgrade[level].config = (NationUpgradeConfig *)malloc( sizeof(NationUpgradeConfig)*g_nation_upgrade[level].maxnum );
		memset( g_nation_upgrade[level].config, 0, sizeof(NationUpgradeConfig)*g_nation_upgrade[level].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `level`,`num`,`silver`,`wood`,`food`,`exp`,`maxexp`,`prestige` from nation_upgrade;" );
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
		if ( level < 0 || level >= g_nation_upgrade_maxnum  )
			continue;
		int num = atoi( row[1] );
		if ( num < 0 || num >= g_nation_upgrade[level].maxnum )
			continue;
		g_nation_upgrade[level].config[num].level = atoi(row[offset++]);
		g_nation_upgrade[level].config[num].num = atoi(row[offset++]);
		g_nation_upgrade[level].config[num].silver = atoi(row[offset++]);
		g_nation_upgrade[level].config[num].wood = atoi(row[offset++]);
		g_nation_upgrade[level].config[num].food = atoi(row[offset++]);
		g_nation_upgrade[level].config[num].exp = atoi(row[offset++]);
		g_nation_upgrade[level].config[num].maxexp = atoi(row[offset++]);
		g_nation_upgrade[level].config[num].prestige = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int nationupgrade_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_nation_upgrade_maxnum; tmpi++ )
	{
		if ( g_nation_upgrade[tmpi].config )
		{
			free( g_nation_upgrade[tmpi].config );
			g_nation_upgrade[tmpi].config = NULL;
		}
	}
	if ( g_nation_upgrade )
	{
		free( g_nation_upgrade );
		g_nation_upgrade = NULL;
	}
	g_nation_upgrade_maxnum = 0;
	nationupgrade_init_auto();
	return 0;
}

