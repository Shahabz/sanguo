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
#include "auto_data_activity_03.h"
extern MYSQL *myData;
extern lua_State* servL;
ActivityInfo03 *g_activity_03 = NULL;
int g_activity_03_maxnum = 0;

int activityinfo03_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from activity_03;" );
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
			g_activity_03_maxnum = atoi( row[0] ) + 1;
		else
			g_activity_03_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_activity_03 = (ActivityInfo03 *)malloc( sizeof(ActivityInfo03)*g_activity_03_maxnum );
	memset( g_activity_03, 0, sizeof(ActivityInfo03)*g_activity_03_maxnum );

	sprintf( szSQL, "select `id`,`nameid`,`record_offset`,`needvalue`,`awardkind0`,`awardkind1`,`awardkind2`,`awardkind3`,`awardkind4`,`awardnum0`,`awardnum1`,`awardnum2`,`awardnum3`,`awardnum4` from activity_03;" );
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
		if ( id < 0 || id >= g_activity_03_maxnum  )
			continue;
		g_activity_03[id].id = atoi(row[offset++]);
		g_activity_03[id].nameid = atoi(row[offset++]);
		g_activity_03[id].record_offset = atoi(row[offset++]);
		g_activity_03[id].needvalue = atoi(row[offset++]);
		g_activity_03[id].awardkind[0] = atoi(row[offset++]);
		g_activity_03[id].awardkind[1] = atoi(row[offset++]);
		g_activity_03[id].awardkind[2] = atoi(row[offset++]);
		g_activity_03[id].awardkind[3] = atoi(row[offset++]);
		g_activity_03[id].awardkind[4] = atoi(row[offset++]);
		g_activity_03[id].awardnum[0] = atoi(row[offset++]);
		g_activity_03[id].awardnum[1] = atoi(row[offset++]);
		g_activity_03[id].awardnum[2] = atoi(row[offset++]);
		g_activity_03[id].awardnum[3] = atoi(row[offset++]);
		g_activity_03[id].awardnum[4] = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int activityinfo03_reload_auto()
{
	if ( g_activity_03 )
	{
		free( g_activity_03 );
		g_activity_03 = NULL;
	}
	g_activity_03_maxnum = 0;
	activityinfo03_init_auto();
	return 0;
}

