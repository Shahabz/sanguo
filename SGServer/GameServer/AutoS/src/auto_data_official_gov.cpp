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
#include "auto_data_official_gov.h"
extern MYSQL *myData;
extern lua_State* servL;
OfficialGov *g_official_gov = NULL;
int g_official_gov_maxnum = 0;

int officialgov_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(kind) from official_gov;" );
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
			g_official_gov_maxnum = atoi( row[0] ) + 1;
		else
			g_official_gov_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_official_gov = (OfficialGov *)malloc( sizeof(OfficialGov)*g_official_gov_maxnum );
	memset( g_official_gov, 0, sizeof(OfficialGov)*g_official_gov_maxnum );

	sprintf( szSQL, "select `kind`,`level`,`color`,`buildinglevel`,`produce`,`haveiron`,`duration`,`silver`,`token`,`free`,`shape` from official_gov;" );
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
		if ( kind < 0 || kind >= g_official_gov_maxnum  )
			continue;
		g_official_gov[kind].kind = atoi(row[offset++]);
		g_official_gov[kind].level = atoi(row[offset++]);
		g_official_gov[kind].color = atoi(row[offset++]);
		g_official_gov[kind].buildinglevel = atoi(row[offset++]);
		g_official_gov[kind].produce = atoi(row[offset++]);
		g_official_gov[kind].haveiron = atoi(row[offset++]);
		g_official_gov[kind].duration = atoi(row[offset++]);
		g_official_gov[kind].silver = atoi(row[offset++]);
		g_official_gov[kind].token = atoi(row[offset++]);
		g_official_gov[kind].free = atoi(row[offset++]);
		g_official_gov[kind].shape = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int officialgov_reload_auto()
{
	if ( g_official_gov )
	{
		free( g_official_gov );
		g_official_gov = NULL;
	}
	g_official_gov_maxnum = 0;
	officialgov_init_auto();
	return 0;
}

