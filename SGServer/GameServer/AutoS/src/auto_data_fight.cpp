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
#include "auto_data_fight.h"
extern MYSQL *myData;
extern lua_State* servL;
FightInfo *g_fightinfo = NULL;
int g_fightinfo_maxnum = 0;

int fightinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(id) from fight;" );
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
			g_fightinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_fightinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_fightinfo = (FightInfo *)malloc( sizeof(FightInfo)*g_fightinfo_maxnum );
	memset( g_fightinfo, 0, sizeof(FightInfo)*g_fightinfo_maxnum );

	sprintf( szSQL, "select `id`,`scene`,`monsterid0`,`monsterid1`,`monsterid2`,`monsterid3`,`monsterid4`,`monsterid5`,`monsterid6`,`monsterid7` from fight;" );
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
		if ( id < 0 || id >= g_fightinfo_maxnum  )
			continue;
		g_fightinfo[id].id = atoi(row[offset++]);
		g_fightinfo[id].scene = atoi(row[offset++]);
		g_fightinfo[id].monsterid[0] = atoi(row[offset++]);
		g_fightinfo[id].monsterid[1] = atoi(row[offset++]);
		g_fightinfo[id].monsterid[2] = atoi(row[offset++]);
		g_fightinfo[id].monsterid[3] = atoi(row[offset++]);
		g_fightinfo[id].monsterid[4] = atoi(row[offset++]);
		g_fightinfo[id].monsterid[5] = atoi(row[offset++]);
		g_fightinfo[id].monsterid[6] = atoi(row[offset++]);
		g_fightinfo[id].monsterid[7] = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int fightinfo_reload_auto()
{
	if ( g_fightinfo )
	{
		free( g_fightinfo );
		g_fightinfo = NULL;
	}
	g_fightinfo_maxnum = 0;
	fightinfo_init_auto();
	return 0;
}

