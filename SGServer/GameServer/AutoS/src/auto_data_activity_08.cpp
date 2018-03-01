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
#include "auto_data_activity_08.h"
extern MYSQL *myData;
extern lua_State* servL;
ActivityInfo08 *g_activity_08 = NULL;
int g_activity_08_maxnum = 0;

int activityinfo08_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`Id`) from activity_08;" );
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
			g_activity_08_maxnum = atoi( row[0] ) + 1;
		else
			g_activity_08_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_activity_08 = (ActivityInfo08 *)malloc( sizeof(ActivityInfo08)*g_activity_08_maxnum );
	memset( g_activity_08, 0, sizeof(ActivityInfo08)*g_activity_08_maxnum );

	sprintf( szSQL, "select `Id` from activity_08;" );
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
		if ( Id < 0 || Id >= g_activity_08_maxnum  )
			continue;
		g_activity_08[Id].Id = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int activityinfo08_reload_auto()
{
	if ( g_activity_08 )
	{
		free( g_activity_08 );
		g_activity_08 = NULL;
	}
	g_activity_08_maxnum = 0;
	activityinfo08_init_auto();
	return 0;
}

