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

	sprintf( szSQL, "select `kind`, max( `level` ) from tiance_quest group by `kind`;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		int kind = atoi( row[0] );
		if ( kind < 0 || kind >= g_tiance_quest_maxnum  )
			continue;
		g_tiance_quest[kind].maxnum = atoi( row[1] ) + 1;
		g_tiance_quest[kind].config = (TianceQuestConfig *)malloc( sizeof(TianceQuestConfig)*g_tiance_quest[kind].maxnum );
		memset( g_tiance_quest[kind].config, 0, sizeof(TianceQuestConfig)*g_tiance_quest[kind].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `kind`,`level`,`actor_level`,`ability`,`value`,`showvalue`,`silver`,`wood`,`food`,`iron`,`progress` from tiance_quest;" );
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
		int level = atoi( row[1] );
		if ( level < 0 || level >= g_tiance_quest[kind].maxnum )
			continue;
		g_tiance_quest[kind].config[level].kind = atoi(row[offset++]);
		g_tiance_quest[kind].config[level].level = atoi(row[offset++]);
		g_tiance_quest[kind].config[level].actor_level = atoi(row[offset++]);
		g_tiance_quest[kind].config[level].ability = atoi(row[offset++]);
		g_tiance_quest[kind].config[level].value = atoi(row[offset++]);
		g_tiance_quest[kind].config[level].showvalue = atoi(row[offset++]);
		g_tiance_quest[kind].config[level].silver = atoi(row[offset++]);
		g_tiance_quest[kind].config[level].wood = atoi(row[offset++]);
		g_tiance_quest[kind].config[level].food = atoi(row[offset++]);
		g_tiance_quest[kind].config[level].iron = atoi(row[offset++]);
		g_tiance_quest[kind].config[level].progress = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int tiancequest_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_tiance_quest_maxnum; tmpi++ )
	{
		if ( g_tiance_quest[tmpi].config )
		{
			free( g_tiance_quest[tmpi].config );
			g_tiance_quest[tmpi].config = NULL;
		}
	}
	if ( g_tiance_quest )
	{
		free( g_tiance_quest );
		g_tiance_quest = NULL;
	}
	g_tiance_quest_maxnum = 0;
	tiancequest_init_auto();
	return 0;
}

