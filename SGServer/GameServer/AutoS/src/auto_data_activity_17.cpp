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
#include "auto_data_activity_17.h"
extern MYSQL *myData;
extern lua_State* servL;
ActivityInfo17 *g_activity_17 = NULL;
int g_activity_17_maxnum = 0;

int activityinfo17_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from activity_17;" );
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
			g_activity_17_maxnum = atoi( row[0] ) + 1;
		else
			g_activity_17_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_activity_17 = (ActivityInfo17 *)malloc( sizeof(ActivityInfo17)*g_activity_17_maxnum );
	memset( g_activity_17, 0, sizeof(ActivityInfo17)*g_activity_17_maxnum );

	sprintf( szSQL, "select `id`,`pay`,`dollar`,`awardkind00`,`awardkind01`,`awardkind02`,`awardkind03`,`awardkind04`,`awardkind05`,`awardkind06`,`awardkind07`,`awardkind08`,`awardkind09`,`awardkind10`,`awardkind11`,`awardkind12`,`awardkind13`,`awardkind14`,`awardkind15`,`awardnum00`,`awardnum01`,`awardnum02`,`awardnum03`,`awardnum04`,`awardnum05`,`awardnum06`,`awardnum07`,`awardnum08`,`awardnum09`,`awardnum10`,`awardnum11`,`awardnum12`,`awardnum13`,`awardnum14`,`awardnum15` from activity_17;" );
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
		if ( id < 0 || id >= g_activity_17_maxnum  )
			continue;
		g_activity_17[id].id = atoi(row[offset++]);
		g_activity_17[id].pay = atoi(row[offset++]);
		g_activity_17[id].dollar = atoi(row[offset++]);
		g_activity_17[id].awardkind[0] = atoi(row[offset++]);
		g_activity_17[id].awardkind[1] = atoi(row[offset++]);
		g_activity_17[id].awardkind[2] = atoi(row[offset++]);
		g_activity_17[id].awardkind[3] = atoi(row[offset++]);
		g_activity_17[id].awardkind[4] = atoi(row[offset++]);
		g_activity_17[id].awardkind[5] = atoi(row[offset++]);
		g_activity_17[id].awardkind[6] = atoi(row[offset++]);
		g_activity_17[id].awardkind[7] = atoi(row[offset++]);
		g_activity_17[id].awardkind[8] = atoi(row[offset++]);
		g_activity_17[id].awardkind[9] = atoi(row[offset++]);
		g_activity_17[id].awardkind[10] = atoi(row[offset++]);
		g_activity_17[id].awardkind[11] = atoi(row[offset++]);
		g_activity_17[id].awardkind[12] = atoi(row[offset++]);
		g_activity_17[id].awardkind[13] = atoi(row[offset++]);
		g_activity_17[id].awardkind[14] = atoi(row[offset++]);
		g_activity_17[id].awardkind[15] = atoi(row[offset++]);
		g_activity_17[id].awardnum[0] = atoi(row[offset++]);
		g_activity_17[id].awardnum[1] = atoi(row[offset++]);
		g_activity_17[id].awardnum[2] = atoi(row[offset++]);
		g_activity_17[id].awardnum[3] = atoi(row[offset++]);
		g_activity_17[id].awardnum[4] = atoi(row[offset++]);
		g_activity_17[id].awardnum[5] = atoi(row[offset++]);
		g_activity_17[id].awardnum[6] = atoi(row[offset++]);
		g_activity_17[id].awardnum[7] = atoi(row[offset++]);
		g_activity_17[id].awardnum[8] = atoi(row[offset++]);
		g_activity_17[id].awardnum[9] = atoi(row[offset++]);
		g_activity_17[id].awardnum[10] = atoi(row[offset++]);
		g_activity_17[id].awardnum[11] = atoi(row[offset++]);
		g_activity_17[id].awardnum[12] = atoi(row[offset++]);
		g_activity_17[id].awardnum[13] = atoi(row[offset++]);
		g_activity_17[id].awardnum[14] = atoi(row[offset++]);
		g_activity_17[id].awardnum[15] = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int activityinfo17_reload_auto()
{
	if ( g_activity_17 )
	{
		free( g_activity_17 );
		g_activity_17 = NULL;
	}
	g_activity_17_maxnum = 0;
	activityinfo17_init_auto();
	return 0;
}

