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
#include "auto_data_nation_equip_open.h"
extern MYSQL *myData;
extern lua_State* servL;
NationEquipOpen *g_nequip_open = NULL;
int g_nequip_open_maxnum = 0;

int nationequipopen_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`kind`) from nation_equip_open;" );
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
			g_nequip_open_maxnum = atoi( row[0] ) + 1;
		else
			g_nequip_open_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_nequip_open = (NationEquipOpen *)malloc( sizeof(NationEquipOpen)*g_nequip_open_maxnum );
	memset( g_nequip_open, 0, sizeof(NationEquipOpen)*g_nequip_open_maxnum );

	sprintf( szSQL, "select `kind`,`actorlevel`,`itemkind`,`itemnum`,`silver`,`sec` from nation_equip_open;" );
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
		if ( kind < 0 || kind >= g_nequip_open_maxnum  )
			continue;
		g_nequip_open[kind].kind = atoi(row[offset++]);
		g_nequip_open[kind].actorlevel = atoi(row[offset++]);
		g_nequip_open[kind].itemkind = atoi(row[offset++]);
		g_nequip_open[kind].itemnum = atoi(row[offset++]);
		g_nequip_open[kind].silver = atoi(row[offset++]);
		g_nequip_open[kind].sec = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int nationequipopen_reload_auto()
{
	if ( g_nequip_open )
	{
		free( g_nequip_open );
		g_nequip_open = NULL;
	}
	g_nequip_open_maxnum = 0;
	nationequipopen_init_auto();
	return 0;
}

