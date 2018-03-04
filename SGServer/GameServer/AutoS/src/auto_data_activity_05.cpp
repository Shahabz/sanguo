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
#include "auto_data_activity_05.h"
extern MYSQL *myData;
extern lua_State* servL;
ActivityInfo05 *g_activity_05 = NULL;
int g_activity_05_maxnum = 0;

int activityinfo05_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from activity_05;" );
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
			g_activity_05_maxnum = atoi( row[0] ) + 1;
		else
			g_activity_05_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_activity_05 = (ActivityInfo05 *)malloc( sizeof(ActivityInfo05)*g_activity_05_maxnum );
	memset( g_activity_05, 0, sizeof(ActivityInfo05)*g_activity_05_maxnum );

	sprintf( szSQL, "select `id`,`awardkind`,`awardnum`,`token`,`weight` from activity_05;" );
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
		int id = atoi( row[0] );
		if ( id < 0 || id >= g_activity_05_maxnum  )
			continue;
		g_activity_05[id].id = atoi(row[offset++]);
		g_activity_05[id].awardkind = atoi(row[offset++]);
		g_activity_05[id].awardnum = atoi(row[offset++]);
		g_activity_05[id].token = atoi(row[offset++]);
		g_activity_05[id].weight = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int activityinfo05_reload_auto()
{
	if ( g_activity_05 )
	{
		free( g_activity_05 );
		g_activity_05 = NULL;
	}
	g_activity_05_maxnum = 0;
	activityinfo05_init_auto();
	return 0;
}

