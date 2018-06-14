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
#include "auto_data_robot_ai.h"
extern MYSQL *myData;
extern lua_State* servL;
RobotAi *g_robot_ai = NULL;
int g_robot_ai_maxnum = 0;

int robotai_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from robot_ai;" );
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
			g_robot_ai_maxnum = atoi( row[0] ) + 1;
		else
			g_robot_ai_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_robot_ai = (RobotAi *)malloc( sizeof(RobotAi)*g_robot_ai_maxnum );
	memset( g_robot_ai, 0, sizeof(RobotAi)*g_robot_ai_maxnum );

	sprintf( szSQL, "select `id`,`actorexp_add`,`silver_init`,`silver_add`,`silver_max`,`wood_init`,`wood_add`,`wood_max`,`food_init`,`food_add`,`food_max`,`iron_init`,`iron_add`,`iron_max`,`herokind0`,`herokind1`,`herokind2`,`herokind3` from robot_ai;" );
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
		if ( id < 0 || id >= g_robot_ai_maxnum  )
			continue;
		g_robot_ai[id].id = atoi(row[offset++]);
		g_robot_ai[id].actorexp_add = (float)atof(row[offset++]);
		g_robot_ai[id].silver_init = atoi(row[offset++]);
		g_robot_ai[id].silver_add = (float)atof(row[offset++]);
		g_robot_ai[id].silver_max = (float)atof(row[offset++]);
		g_robot_ai[id].wood_init = atoi(row[offset++]);
		g_robot_ai[id].wood_add = (float)atof(row[offset++]);
		g_robot_ai[id].wood_max = (float)atof(row[offset++]);
		g_robot_ai[id].food_init = atoi(row[offset++]);
		g_robot_ai[id].food_add = (float)atof(row[offset++]);
		g_robot_ai[id].food_max = (float)atof(row[offset++]);
		g_robot_ai[id].iron_init = atoi(row[offset++]);
		g_robot_ai[id].iron_add = (float)atof(row[offset++]);
		g_robot_ai[id].iron_max = (float)atof(row[offset++]);
		g_robot_ai[id].herokind[0] = atoi(row[offset++]);
		g_robot_ai[id].herokind[1] = atoi(row[offset++]);
		g_robot_ai[id].herokind[2] = atoi(row[offset++]);
		g_robot_ai[id].herokind[3] = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int robotai_reload_auto()
{
	if ( g_robot_ai )
	{
		free( g_robot_ai );
		g_robot_ai = NULL;
	}
	g_robot_ai_maxnum = 0;
	robotai_init_auto();
	return 0;
}

