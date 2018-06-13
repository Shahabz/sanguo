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
#include "auto_data_fight_helper.h"
extern MYSQL *myData;
extern lua_State* servL;
FightHelper *g_fight_helper = NULL;
int g_fight_helper_maxnum = 0;

int fighthelper_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`kind`) from fight_helper;" );
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
			g_fight_helper_maxnum = atoi( row[0] ) + 1;
		else
			g_fight_helper_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_fight_helper = (FightHelper *)malloc( sizeof(FightHelper)*g_fight_helper_maxnum );
	memset( g_fight_helper, 0, sizeof(FightHelper)*g_fight_helper_maxnum );

	sprintf( szSQL, "select `kind`,`monsterid`,`token`,`sort` from fight_helper;" );
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
		int kind = atoi( row[0] );
		if ( kind < 0 || kind >= g_fight_helper_maxnum  )
			continue;
		g_fight_helper[kind].kind = atoi(row[offset++]);
		g_fight_helper[kind].monsterid = atoi(row[offset++]);
		g_fight_helper[kind].token = atoi(row[offset++]);
		g_fight_helper[kind].sort = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int fighthelper_reload_auto()
{
	if ( g_fight_helper )
	{
		free( g_fight_helper );
		g_fight_helper = NULL;
	}
	g_fight_helper_maxnum = 0;
	fighthelper_init_auto();
	return 0;
}

