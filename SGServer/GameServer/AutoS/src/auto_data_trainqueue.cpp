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
#include "auto_data_trainqueue.h"
extern MYSQL *myData;
extern lua_State* servL;
TrainqueueInfo *g_trainqueue = NULL;
int g_trainqueue_maxnum = 0;

int trainqueueinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`queue`) from trainqueue;" );
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
			g_trainqueue_maxnum = atoi( row[0] ) + 1;
		else
			g_trainqueue_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_trainqueue = (TrainqueueInfo *)malloc( sizeof(TrainqueueInfo)*g_trainqueue_maxnum );
	memset( g_trainqueue, 0, sizeof(TrainqueueInfo)*g_trainqueue_maxnum );

	sprintf( szSQL, "select `queue`,`token` from trainqueue;" );
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
		int queue = atoi( row[0] );
		if ( queue < 0 || queue >= g_trainqueue_maxnum  )
			continue;
		g_trainqueue[queue].queue = atoi(row[offset++]);
		g_trainqueue[queue].token = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int trainqueueinfo_reload_auto()
{
	if ( g_trainqueue )
	{
		free( g_trainqueue );
		g_trainqueue = NULL;
	}
	g_trainqueue_maxnum = 0;
	trainqueueinfo_init_auto();
	return 0;
}

