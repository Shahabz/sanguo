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
#include "auto_data_nation_equip_remake.h"
extern MYSQL *myData;
extern lua_State* servL;
NationEquipRemake *g_nequip_remake = NULL;
int g_nequip_remake_maxnum = 0;

int nationequipremake_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`kind`) from nation_equip_remake;" );
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
			g_nequip_remake_maxnum = atoi( row[0] ) + 1;
		else
			g_nequip_remake_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_nequip_remake = (NationEquipRemake *)malloc( sizeof(NationEquipRemake)*g_nequip_remake_maxnum );
	memset( g_nequip_remake, 0, sizeof(NationEquipRemake)*g_nequip_remake_maxnum );

	sprintf( szSQL, "select `kind`, max( `progress` ) from nation_equip_remake group by `kind`;" );
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
		if ( kind < 0 || kind >= g_nequip_remake_maxnum  )
			continue;
		g_nequip_remake[kind].maxnum = atoi( row[1] ) + 1;
		g_nequip_remake[kind].config = (NationEquipRemakeConfig *)malloc( sizeof(NationEquipRemakeConfig)*g_nequip_remake[kind].maxnum );
		memset( g_nequip_remake[kind].config, 0, sizeof(NationEquipRemakeConfig)*g_nequip_remake[kind].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `kind`,`progress`,`silver`,`sec`,`maxlevel` from nation_equip_remake;" );
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
		if ( kind < 0 || kind >= g_nequip_remake_maxnum  )
			continue;
		int progress = atoi( row[1] );
		if ( progress < 0 || progress >= g_nequip_remake[kind].maxnum )
			continue;
		g_nequip_remake[kind].config[progress].kind = atoi(row[offset++]);
		g_nequip_remake[kind].config[progress].progress = atoi(row[offset++]);
		g_nequip_remake[kind].config[progress].silver = atoi(row[offset++]);
		g_nequip_remake[kind].config[progress].sec = atoi(row[offset++]);
		g_nequip_remake[kind].config[progress].maxlevel = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int nationequipremake_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_nequip_remake_maxnum; tmpi++ )
	{
		if ( g_nequip_remake[tmpi].config )
		{
			free( g_nequip_remake[tmpi].config );
			g_nequip_remake[tmpi].config = NULL;
		}
	}
	if ( g_nequip_remake )
	{
		free( g_nequip_remake );
		g_nequip_remake = NULL;
	}
	g_nequip_remake_maxnum = 0;
	nationequipremake_init_auto();
	return 0;
}

