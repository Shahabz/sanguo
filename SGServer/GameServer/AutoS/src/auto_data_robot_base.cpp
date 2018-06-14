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
#include "auto_data_robot_base.h"
extern MYSQL *myData;
extern lua_State* servL;
RobotBase *g_robot_base = NULL;
int g_robot_base_maxnum = 0;

int robotbase_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`level`) from robot_base;" );
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
			g_robot_base_maxnum = atoi( row[0] ) + 1;
		else
			g_robot_base_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_robot_base = (RobotBase *)malloc( sizeof(RobotBase)*g_robot_base_maxnum );
	memset( g_robot_base, 0, sizeof(RobotBase)*g_robot_base_maxnum );

	sprintf( szSQL, "select `level`,`actorlevel_min`,`actorlevel_max`,`silver_add`,`silver_max`,`wood_add`,`wood_max`,`food_add`,`food_max`,`iron_add`,`iron_max`,`herokind0`,`herokind1`,`herokind2`,`herokind3`,`herokind4`,`herokind5`,`herokind6`,`herokind7`,`heroup_max`,`equipkind0`,`equipkind1`,`equipkind2`,`equipkind3`,`equipkind4`,`equipkind5` from robot_base;" );
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
		int level = atoi( row[0] );
		if ( level < 0 || level >= g_robot_base_maxnum  )
			continue;
		g_robot_base[level].level = atoi(row[offset++]);
		g_robot_base[level].actorlevel_min = atoi(row[offset++]);
		g_robot_base[level].actorlevel_max = atoi(row[offset++]);
		g_robot_base[level].silver_add = atoi(row[offset++]);
		g_robot_base[level].silver_max = atoi(row[offset++]);
		g_robot_base[level].wood_add = atoi(row[offset++]);
		g_robot_base[level].wood_max = atoi(row[offset++]);
		g_robot_base[level].food_add = atoi(row[offset++]);
		g_robot_base[level].food_max = atoi(row[offset++]);
		g_robot_base[level].iron_add = atoi(row[offset++]);
		g_robot_base[level].iron_max = atoi(row[offset++]);
		g_robot_base[level].herokind[0] = atoi(row[offset++]);
		g_robot_base[level].herokind[1] = atoi(row[offset++]);
		g_robot_base[level].herokind[2] = atoi(row[offset++]);
		g_robot_base[level].herokind[3] = atoi(row[offset++]);
		g_robot_base[level].herokind[4] = atoi(row[offset++]);
		g_robot_base[level].herokind[5] = atoi(row[offset++]);
		g_robot_base[level].herokind[6] = atoi(row[offset++]);
		g_robot_base[level].herokind[7] = atoi(row[offset++]);
		g_robot_base[level].heroup_max = atoi(row[offset++]);
		g_robot_base[level].equipkind[0] = atoi(row[offset++]);
		g_robot_base[level].equipkind[1] = atoi(row[offset++]);
		g_robot_base[level].equipkind[2] = atoi(row[offset++]);
		g_robot_base[level].equipkind[3] = atoi(row[offset++]);
		g_robot_base[level].equipkind[4] = atoi(row[offset++]);
		g_robot_base[level].equipkind[5] = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int robotbase_reload_auto()
{
	if ( g_robot_base )
	{
		free( g_robot_base );
		g_robot_base = NULL;
	}
	g_robot_base_maxnum = 0;
	robotbase_init_auto();
	return 0;
}

