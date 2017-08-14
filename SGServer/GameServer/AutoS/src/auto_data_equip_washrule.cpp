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
#include "auto_data_equip_washrule.h"
extern MYSQL *myData;
extern lua_State* servL;
EquipWashRule *g_equip_washrule = NULL;
int g_equip_washrule_maxnum = 0;

int equipwashrule_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(color) from equip_washrule;" );
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
			g_equip_washrule_maxnum = atoi( row[0] ) + 1;
		else
			g_equip_washrule_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_equip_washrule = (EquipWashRule *)malloc( sizeof(EquipWashRule)*g_equip_washrule_maxnum );
	memset( g_equip_washrule, 0, sizeof(EquipWashRule)*g_equip_washrule_maxnum );

	sprintf( szSQL, "select `color`,`valuenum`,`levellimit`,`superskill`,`token` from equip_washrule;" );
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
		int color = atoi( row[0] );
		if ( color < 0 || color >= g_equip_washrule_maxnum  )
			continue;
		g_equip_washrule[color].color = atoi(row[offset++]);
		g_equip_washrule[color].valuenum = atoi(row[offset++]);
		g_equip_washrule[color].levellimit = atoi(row[offset++]);
		g_equip_washrule[color].superskill = atoi(row[offset++]);
		g_equip_washrule[color].token = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int equipwashrule_reload_auto()
{
	if ( g_equip_washrule )
	{
		free( g_equip_washrule );
		g_equip_washrule = NULL;
	}
	g_equip_washrule_maxnum = 0;
	equipwashrule_init_auto();
	return 0;
}

