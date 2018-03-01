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
#include "auto_data_activity_06.h"
extern MYSQL *myData;
extern lua_State* servL;
ActivityInfo06 *g_activity_06 = NULL;
int g_activity_06_maxnum = 0;

int activityinfo06_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`Id`) from activity_06;" );
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
			g_activity_06_maxnum = atoi( row[0] ) + 1;
		else
			g_activity_06_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_activity_06 = (ActivityInfo06 *)malloc( sizeof(ActivityInfo06)*g_activity_06_maxnum );
	memset( g_activity_06, 0, sizeof(ActivityInfo06)*g_activity_06_maxnum );

	sprintf( szSQL, "select `Id` from activity_06;" );
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
		int Id = atoi( row[0] );
		if ( Id < 0 || Id >= g_activity_06_maxnum  )
			continue;
		g_activity_06[Id].Id = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int activityinfo06_reload_auto()
{
	if ( g_activity_06 )
	{
		free( g_activity_06 );
		g_activity_06 = NULL;
	}
	g_activity_06_maxnum = 0;
	activityinfo06_init_auto();
	return 0;
}

