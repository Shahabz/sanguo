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
#include "auto_data_official_forging.h"
extern MYSQL *myData;
extern lua_State* servL;
OfficialForging *g_official_forging = NULL;
int g_official_forging_maxnum = 0;

int officialforging_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(kind) from official_forging;" );
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
			g_official_forging_maxnum = atoi( row[0] ) + 1;
		else
			g_official_forging_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_official_forging = (OfficialForging *)malloc( sizeof(OfficialForging)*g_official_forging_maxnum );
	memset( g_official_forging, 0, sizeof(OfficialForging)*g_official_forging_maxnum );

	sprintf( szSQL, "select `kind`,`level`,`color`,`buildinglevel`,`quick`,`duration`,`silver`,`token` from official_forging;" );
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
		if ( kind < 0 || kind >= g_official_forging_maxnum  )
			continue;
		g_official_forging[kind].kind = atoi(row[offset++]);
		g_official_forging[kind].level = atoi(row[offset++]);
		g_official_forging[kind].color = atoi(row[offset++]);
		g_official_forging[kind].buildinglevel = atoi(row[offset++]);
		g_official_forging[kind].quick = atoi(row[offset++]);
		g_official_forging[kind].duration = atoi(row[offset++]);
		g_official_forging[kind].silver = atoi(row[offset++]);
		g_official_forging[kind].token = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int officialforging_reload_auto()
{
	if ( g_official_forging )
	{
		free( g_official_forging );
		g_official_forging = NULL;
	}
	g_official_forging_maxnum = 0;
	officialforging_init_auto();
	return 0;
}

