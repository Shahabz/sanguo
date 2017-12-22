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
#include "auto_data_nation_place.h"
extern MYSQL *myData;
extern lua_State* servL;
NationPlace *g_nation_place = NULL;
int g_nation_place_maxnum = 0;

int nationplace_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(kind) from nation_place;" );
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
			g_nation_place_maxnum = atoi( row[0] ) + 1;
		else
			g_nation_place_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_nation_place = (NationPlace *)malloc( sizeof(NationPlace)*g_nation_place_maxnum );
	memset( g_nation_place, 0, sizeof(NationPlace)*g_nation_place_maxnum );

	sprintf( szSQL, "select `kind`,`prestige`,`silver`,`cost_kind0`,`cost_kind1`,`cost_kind2`,`cost_kind3`,`cost_num0`,`cost_num1`,`cost_num2`,`cost_num3`,`ability`,`value` from nation_place;" );
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
		if ( kind < 0 || kind >= g_nation_place_maxnum  )
			continue;
		g_nation_place[kind].kind = atoi(row[offset++]);
		g_nation_place[kind].prestige = atoi(row[offset++]);
		g_nation_place[kind].silver = atoi(row[offset++]);
		g_nation_place[kind].cost_kind[0] = atoi(row[offset++]);
		g_nation_place[kind].cost_kind[1] = atoi(row[offset++]);
		g_nation_place[kind].cost_kind[2] = atoi(row[offset++]);
		g_nation_place[kind].cost_kind[3] = atoi(row[offset++]);
		g_nation_place[kind].cost_num[0] = atoi(row[offset++]);
		g_nation_place[kind].cost_num[1] = atoi(row[offset++]);
		g_nation_place[kind].cost_num[2] = atoi(row[offset++]);
		g_nation_place[kind].cost_num[3] = atoi(row[offset++]);
		g_nation_place[kind].ability = atoi(row[offset++]);
		g_nation_place[kind].value = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int nationplace_reload_auto()
{
	if ( g_nation_place )
	{
		free( g_nation_place );
		g_nation_place = NULL;
	}
	g_nation_place_maxnum = 0;
	nationplace_init_auto();
	return 0;
}

