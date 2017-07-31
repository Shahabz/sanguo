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
#include "auto_data_equip_wash.h"
extern MYSQL *myData;
extern lua_State* servL;
EquipWashInfo *g_equipwash = NULL;
int g_equipwash_maxnum = 0;

int equipwashinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(washid) from equip_wash;" );
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
			g_equipwash_maxnum = atoi( row[0] ) + 1;
		else
			g_equipwash_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_equipwash = (EquipWashInfo *)malloc( sizeof(EquipWashInfo)*g_equipwash_maxnum );
	memset( g_equipwash, 0, sizeof(EquipWashInfo)*g_equipwash_maxnum );

	sprintf( szSQL, "select `washid`,`level`,`ability`,`value` from equip_wash;" );
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
		int washid = atoi( row[0] );
		if ( washid < 0 || washid >= g_equipwash_maxnum  )
			continue;
		g_equipwash[washid].washid = atoi(row[offset++]);
		g_equipwash[washid].level = atoi(row[offset++]);
		g_equipwash[washid].ability = atoi(row[offset++]);
		g_equipwash[washid].value = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int equipwashinfo_reload_auto()
{
	if ( g_equipwash )
	{
		free( g_equipwash );
		g_equipwash = NULL;
	}
	g_equipwash_maxnum = 0;
	equipwashinfo_init_auto();
	return 0;
}

