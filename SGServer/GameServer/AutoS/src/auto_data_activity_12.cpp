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
#include "auto_data_activity_12.h"
extern MYSQL *myData;
extern lua_State* servL;
ActivityInfo12 *g_activity_12 = NULL;
int g_activity_12_maxnum = 0;

int activityinfo12_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from activity_12;" );
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
			g_activity_12_maxnum = atoi( row[0] ) + 1;
		else
			g_activity_12_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_activity_12 = (ActivityInfo12 *)malloc( sizeof(ActivityInfo12)*g_activity_12_maxnum );
	memset( g_activity_12, 0, sizeof(ActivityInfo12)*g_activity_12_maxnum );

	sprintf( szSQL, "select `id`,`level`,`marchtime`,`attack`,`defense`,`troops`,`awardkind0`,`awardkind1`,`awardkind2`,`awardkind3`,`awardnum0`,`awardnum1`,`awardnum2`,`awardnum3` from activity_12;" );
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
		if ( id < 0 || id >= g_activity_12_maxnum  )
			continue;
		g_activity_12[id].id = atoi(row[offset++]);
		g_activity_12[id].level = atoi(row[offset++]);
		g_activity_12[id].marchtime = atoi(row[offset++]);
		g_activity_12[id].attack = atoi(row[offset++]);
		g_activity_12[id].defense = atoi(row[offset++]);
		g_activity_12[id].troops = atoi(row[offset++]);
		g_activity_12[id].awardkind[0] = atoi(row[offset++]);
		g_activity_12[id].awardkind[1] = atoi(row[offset++]);
		g_activity_12[id].awardkind[2] = atoi(row[offset++]);
		g_activity_12[id].awardkind[3] = atoi(row[offset++]);
		g_activity_12[id].awardnum[0] = atoi(row[offset++]);
		g_activity_12[id].awardnum[1] = atoi(row[offset++]);
		g_activity_12[id].awardnum[2] = atoi(row[offset++]);
		g_activity_12[id].awardnum[3] = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int activityinfo12_reload_auto()
{
	if ( g_activity_12 )
	{
		free( g_activity_12 );
		g_activity_12 = NULL;
	}
	g_activity_12_maxnum = 0;
	activityinfo12_init_auto();
	return 0;
}

