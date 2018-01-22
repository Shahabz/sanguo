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
#include "auto_data_nation_official.h"
extern MYSQL *myData;
extern lua_State* servL;
NationOfficial *g_nation_official = NULL;
int g_nation_official_maxnum = 0;

int nationofficial_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`official`) from nation_official;" );
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
			g_nation_official_maxnum = atoi( row[0] ) + 1;
		else
			g_nation_official_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_nation_official = (NationOfficial *)malloc( sizeof(NationOfficial)*g_nation_official_maxnum );
	memset( g_nation_official, 0, sizeof(NationOfficial)*g_nation_official_maxnum );

	sprintf( szSQL, "select `official`,`dismissal`,`fight`,`online_notify`,`notify`,`cityfight_body`,`callnum`,`callactornum`,`yield`,`buildquick` from nation_official;" );
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
		int official = atoi( row[0] );
		if ( official < 0 || official >= g_nation_official_maxnum  )
			continue;
		g_nation_official[official].official = atoi(row[offset++]);
		g_nation_official[official].dismissal = atoi(row[offset++]);
		g_nation_official[official].fight = atoi(row[offset++]);
		g_nation_official[official].online_notify = atoi(row[offset++]);
		g_nation_official[official].notify = atoi(row[offset++]);
		g_nation_official[official].cityfight_body = atoi(row[offset++]);
		g_nation_official[official].callnum = atoi(row[offset++]);
		g_nation_official[official].callactornum = atoi(row[offset++]);
		g_nation_official[official].yield = atoi(row[offset++]);
		g_nation_official[official].buildquick = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int nationofficial_reload_auto()
{
	if ( g_nation_official )
	{
		free( g_nation_official );
		g_nation_official = NULL;
	}
	g_nation_official_maxnum = 0;
	nationofficial_init_auto();
	return 0;
}

