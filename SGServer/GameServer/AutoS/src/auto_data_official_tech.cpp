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
#include "auto_data_official_tech.h"
extern MYSQL *myData;
extern lua_State* servL;
OfficialTech *g_official_tech = NULL;
int g_official_tech_maxnum = 0;

int officialtech_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`kind`) from official_tech;" );
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
			g_official_tech_maxnum = atoi( row[0] ) + 1;
		else
			g_official_tech_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_official_tech = (OfficialTech *)malloc( sizeof(OfficialTech)*g_official_tech_maxnum );
	memset( g_official_tech, 0, sizeof(OfficialTech)*g_official_tech_maxnum );

	sprintf( szSQL, "select `kind`,`level`,`color`,`buildinglevel`,`quick`,`duration`,`silver`,`token`,`vip`,`free`,`shape` from official_tech;" );
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
		if ( kind < 0 || kind >= g_official_tech_maxnum  )
			continue;
		g_official_tech[kind].kind = atoi(row[offset++]);
		g_official_tech[kind].level = atoi(row[offset++]);
		g_official_tech[kind].color = atoi(row[offset++]);
		g_official_tech[kind].buildinglevel = atoi(row[offset++]);
		g_official_tech[kind].quick = atoi(row[offset++]);
		g_official_tech[kind].duration = atoi(row[offset++]);
		g_official_tech[kind].silver = atoi(row[offset++]);
		g_official_tech[kind].token = atoi(row[offset++]);
		g_official_tech[kind].vip = atoi(row[offset++]);
		g_official_tech[kind].free = atoi(row[offset++]);
		g_official_tech[kind].shape = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int officialtech_reload_auto()
{
	if ( g_official_tech )
	{
		free( g_official_tech );
		g_official_tech = NULL;
	}
	g_official_tech_maxnum = 0;
	officialtech_init_auto();
	return 0;
}

