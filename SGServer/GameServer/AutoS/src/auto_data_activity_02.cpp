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

	sprintf( szSQL, "select max(`id`) from activity_02;" );
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

	sprintf( szSQL, "select `id`,`level`,`unlock_nameid`,`unlock_descid`,`cond_kind`,`cond_level`,`actor_level`,`awardkind0`,`awardkind1`,`awardkind2`,`awardnum0`,`awardnum1`,`awardnum2`,`sprite_buildingkind`,`sprite_techkind`,`sprite_color` from activity_02;" );
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
		if ( id < 0 || id >= g_activity_02_maxnum  )
			continue;
		g_activity_02[id].id = atoi(row[offset++]);
		g_activity_02[id].level = atoi(row[offset++]);
		g_activity_02[id].unlock_nameid = atoi(row[offset++]);
		g_activity_02[id].unlock_descid = atoi(row[offset++]);
		g_activity_02[id].cond_kind = atoi(row[offset++]);
		g_activity_02[id].cond_level = atoi(row[offset++]);
		g_activity_02[id].actor_level = atoi(row[offset++]);
		g_activity_02[id].awardkind[0] = atoi(row[offset++]);
		g_activity_02[id].awardkind[1] = atoi(row[offset++]);
		g_activity_02[id].awardkind[2] = atoi(row[offset++]);
		g_activity_02[id].awardnum[0] = atoi(row[offset++]);
		g_activity_02[id].awardnum[1] = atoi(row[offset++]);
		g_activity_02[id].awardnum[2] = atoi(row[offset++]);
		g_activity_02[id].sprite_buildingkind = atoi(row[offset++]);
		g_activity_02[id].sprite_techkind = atoi(row[offset++]);
		g_activity_02[id].sprite_color = atoi(row[offset++]);
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

