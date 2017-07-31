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
#include "auto_data_city_tech.h"
extern MYSQL *myData;
extern lua_State* servL;
TechInfo *g_techinfo = NULL;
int g_techinfo_maxnum = 0;

int techinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(kind) from city_tech;" );
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
			g_techinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_techinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_techinfo = (TechInfo *)malloc( sizeof(TechInfo)*g_techinfo_maxnum );
	memset( g_techinfo, 0, sizeof(TechInfo)*g_techinfo_maxnum );

	sprintf( szSQL, "select kind, max( level ) from city_tech group by kind;" );
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
		if ( kind < 0 || kind >= g_techinfo_maxnum  )
			continue;
		g_techinfo[kind].maxnum = atoi( row[1] ) + 1;
		g_techinfo[kind].config = (TechInfoConfig *)malloc( sizeof(TechInfoConfig)*g_techinfo[kind].maxnum );
		memset( g_techinfo[kind].config, 0, sizeof(TechInfoConfig)*g_techinfo[kind].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `kind`,`level`,`buildinglevel`,`prekind`,`prelevel`,`progress`,`sec`,`silver`,`wood`,`food`,`iron`,`ability`,`value` from city_tech;" );
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
		if ( kind < 0 || kind >= g_techinfo_maxnum  )
			continue;
		int level = atoi( row[1] );
		if ( level < 0 || level >= g_techinfo[kind].maxnum )
			continue;
		g_techinfo[kind].config[level].kind = atoi(row[offset++]);
		g_techinfo[kind].config[level].level = atoi(row[offset++]);
		g_techinfo[kind].config[level].buildinglevel = atoi(row[offset++]);
		g_techinfo[kind].config[level].prekind = atoi(row[offset++]);
		g_techinfo[kind].config[level].prelevel = atoi(row[offset++]);
		g_techinfo[kind].config[level].progress = atoi(row[offset++]);
		g_techinfo[kind].config[level].sec = atoi(row[offset++]);
		g_techinfo[kind].config[level].silver = atoi(row[offset++]);
		g_techinfo[kind].config[level].wood = atoi(row[offset++]);
		g_techinfo[kind].config[level].food = atoi(row[offset++]);
		g_techinfo[kind].config[level].iron = atoi(row[offset++]);
		g_techinfo[kind].config[level].ability = atoi(row[offset++]);
		g_techinfo[kind].config[level].value = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int techinfo_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_techinfo_maxnum; tmpi++ )
	{
		if ( g_techinfo[tmpi].config )
		{
			free( g_techinfo[tmpi].config );
			g_techinfo[tmpi].config = NULL;
		}
	}
	if ( g_techinfo )
	{
		free( g_techinfo );
		g_techinfo = NULL;
	}
	g_techinfo_maxnum = 0;
	techinfo_init_auto();
	return 0;
}

