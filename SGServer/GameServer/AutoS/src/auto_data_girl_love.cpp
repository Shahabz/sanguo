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
#include "auto_data_girl_love.h"
extern MYSQL *myData;
extern lua_State* servL;
GirlLove *g_girllove = NULL;
int g_girllove_maxnum = 0;

int girllove_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`level`) from girl_love;" );
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
			g_girllove_maxnum = atoi( row[0] ) + 1;
		else
			g_girllove_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_girllove = (GirlLove *)malloc( sizeof(GirlLove)*g_girllove_maxnum );
	memset( g_girllove, 0, sizeof(GirlLove)*g_girllove_maxnum );

	sprintf( szSQL, "select `level`,`exp`,`soncount`,`makelove_exp`,`killenemy_exp`,`citywar_exp`,`nationwar_exp`,`kingwar_exp`,`today_maxexp` from girl_love;" );
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
		int level = atoi( row[0] );
		if ( level < 0 || level >= g_girllove_maxnum  )
			continue;
		g_girllove[level].level = atoi(row[offset++]);
		g_girllove[level].exp = atoi(row[offset++]);
		g_girllove[level].soncount = atoi(row[offset++]);
		g_girllove[level].makelove_exp = atoi(row[offset++]);
		g_girllove[level].killenemy_exp = atoi(row[offset++]);
		g_girllove[level].citywar_exp = atoi(row[offset++]);
		g_girllove[level].nationwar_exp = atoi(row[offset++]);
		g_girllove[level].kingwar_exp = atoi(row[offset++]);
		g_girllove[level].today_maxexp = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int girllove_reload_auto()
{
	if ( g_girllove )
	{
		free( g_girllove );
		g_girllove = NULL;
	}
	g_girllove_maxnum = 0;
	girllove_init_auto();
	return 0;
}

