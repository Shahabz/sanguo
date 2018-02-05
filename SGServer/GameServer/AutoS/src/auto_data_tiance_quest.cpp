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
#include "auto_data_tiance_quest.h"
extern MYSQL *myData;
extern lua_State* servL;
TianceQuest *g_tiance_quest = NULL;
int g_tiance_quest_maxnum = 0;

int tiancequest_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`kind`) from tiance_quest;" );
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
			g_tiance_quest_maxnum = atoi( row[0] ) + 1;
		else
			g_tiance_quest_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_tiance_quest = (TianceQuest *)malloc( sizeof(TianceQuest)*g_tiance_quest_maxnum );
	memset( g_tiance_quest, 0, sizeof(TianceQuest)*g_tiance_quest_maxnum );

	sprintf( szSQL, "select `kind`,`prekind`,`nextkind`,`level`,`tiance_level`,`actor_level`,`brush_enemykind`,`brush_enemynum`,`ability`,`value`,`showvalue` from tiance_quest;" );
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
		if ( kind < 0 || kind >= g_tiance_quest_maxnum  )
			continue;
		g_tiance_quest[kind].kind = atoi(row[offset++]);
		g_tiance_quest[kind].prekind = atoi(row[offset++]);
		g_tiance_quest[kind].nextkind = atoi(row[offset++]);
		g_tiance_quest[kind].level = atoi(row[offset++]);
		g_tiance_quest[kind].tiance_level = atoi(row[offset++]);
		g_tiance_quest[kind].actor_level = atoi(row[offset++]);
		g_tiance_quest[kind].brush_enemykind = atoi(row[offset++]);
		g_tiance_quest[kind].brush_enemynum = atoi(row[offset++]);
		g_tiance_quest[kind].ability = atoi(row[offset++]);
		g_tiance_quest[kind].value = atoi(row[offset++]);
		g_tiance_quest[kind].showvalue = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int tiancequest_reload_auto()
{
	if ( g_tiance_quest )
	{
		free( g_tiance_quest );
		g_tiance_quest = NULL;
	}
	g_tiance_quest_maxnum = 0;
	tiancequest_init_auto();
	return 0;
}

