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
#include "auto_data_nation_equip.h"
extern MYSQL *myData;
extern lua_State* servL;
NationEquipInfo *g_nationequip = NULL;
int g_nationequip_maxnum = 0;

int nationequipinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`kind`) from nation_equip;" );
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
			g_nationequip_maxnum = atoi( row[0] ) + 1;
		else
			g_nationequip_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_nationequip = (NationEquipInfo *)malloc( sizeof(NationEquipInfo)*g_nationequip_maxnum );
	memset( g_nationequip, 0, sizeof(NationEquipInfo)*g_nationequip_maxnum );

	sprintf( szSQL, "select `kind`, max( `level` ) from nation_equip group by `kind`;" );
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
		if ( kind < 0 || kind >= g_nationequip_maxnum  )
			continue;
		g_nationequip[kind].maxnum = atoi( row[1] ) + 1;
		g_nationequip[kind].config = (NationEquipInfoConfig *)malloc( sizeof(NationEquipInfoConfig)*g_nationequip[kind].maxnum );
		memset( g_nationequip[kind].config, 0, sizeof(NationEquipInfoConfig)*g_nationequip[kind].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `kind`,`level`,`actorlevel`,`itemkind`,`itemnum`,`exp`,`progress`,`iron`,`sec`,`ability`,`value` from nation_equip;" );
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
		if ( kind < 0 || kind >= g_nationequip_maxnum  )
			continue;
		int level = atoi( row[1] );
		if ( level < 0 || level >= g_nationequip[kind].maxnum )
			continue;
		g_nationequip[kind].config[level].kind = atoi(row[offset++]);
		g_nationequip[kind].config[level].level = atoi(row[offset++]);
		g_nationequip[kind].config[level].actorlevel = atoi(row[offset++]);
		g_nationequip[kind].config[level].itemkind = atoi(row[offset++]);
		g_nationequip[kind].config[level].itemnum = atoi(row[offset++]);
		g_nationequip[kind].config[level].exp = atoi(row[offset++]);
		g_nationequip[kind].config[level].progress = atoi(row[offset++]);
		g_nationequip[kind].config[level].iron = atoi(row[offset++]);
		g_nationequip[kind].config[level].sec = atoi(row[offset++]);
		g_nationequip[kind].config[level].ability = atoi(row[offset++]);
		g_nationequip[kind].config[level].value = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int nationequipinfo_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_nationequip_maxnum; tmpi++ )
	{
		if ( g_nationequip[tmpi].config )
		{
			free( g_nationequip[tmpi].config );
			g_nationequip[tmpi].config = NULL;
		}
	}
	if ( g_nationequip )
	{
		free( g_nationequip );
		g_nationequip = NULL;
	}
	g_nationequip_maxnum = 0;
	nationequipinfo_init_auto();
	return 0;
}

