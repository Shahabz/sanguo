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
#include "auto_data_activity_02.h"
extern MYSQL *myData;
extern lua_State* servL;
ActivityInfo02 *g_activity_02 = NULL;
int g_activity_02_maxnum = 0;

int activityinfo02_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`Id`) from activity_02;" );
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
			g_activity_02_maxnum = atoi( row[0] ) + 1;
		else
			g_activity_02_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_activity_02 = (ActivityInfo02 *)malloc( sizeof(ActivityInfo02)*g_activity_02_maxnum );
	memset( g_activity_02, 0, sizeof(ActivityInfo02)*g_activity_02_maxnum );

	sprintf( szSQL, "select `Id` from activity_02;" );
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
		if ( Id < 0 || Id >= g_activity_02_maxnum  )
			continue;
		g_activity_02[Id].Id = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int activityinfo02_reload_auto()
{
	if ( g_activity_02 )
	{
		free( g_activity_02 );
		g_activity_02 = NULL;
	}
	g_activity_02_maxnum = 0;
	activityinfo02_init_auto();
	return 0;
}

