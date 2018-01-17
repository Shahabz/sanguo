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
#include "auto_data_nation_quest.h"
extern MYSQL *myData;
extern lua_State* servL;
NationQuest *g_nation_quest = NULL;
int g_nation_quest_maxnum = 0;

int nationquest_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`level`) from nation_quest;" );
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
			g_nation_quest_maxnum = atoi( row[0] ) + 1;
		else
			g_nation_quest_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_nation_quest = (NationQuest *)malloc( sizeof(NationQuest)*g_nation_quest_maxnum );
	memset( g_nation_quest, 0, sizeof(NationQuest)*g_nation_quest_maxnum );

	sprintf( szSQL, "select `level`, max( `kind` ) from nation_quest group by `level`;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		int level = atoi( row[0] );
		if ( level < 0 || level >= g_nation_quest_maxnum  )
			continue;
		g_nation_quest[level].maxnum = atoi( row[1] ) + 1;
		g_nation_quest[level].config = (NationQuestConfig *)malloc( sizeof(NationQuestConfig)*g_nation_quest[level].maxnum );
		memset( g_nation_quest[level].config, 0, sizeof(NationQuestConfig)*g_nation_quest[level].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `level`,`kind`,`needvalue`,`awardkind0`,`awardkind1`,`awardkind2`,`awardkind3`,`awardkind4`,`awardnum0`,`awardnum1`,`awardnum2`,`awardnum3`,`awardnum4`,`other_awardkind`,`other_awardnum`,`other_awardodds` from nation_quest;" );
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
		if ( level < 0 || level >= g_nation_quest_maxnum  )
			continue;
		int kind = atoi( row[1] );
		if ( kind < 0 || kind >= g_nation_quest[level].maxnum )
			continue;
		g_nation_quest[level].config[kind].level = atoi(row[offset++]);
		g_nation_quest[level].config[kind].kind = atoi(row[offset++]);
		g_nation_quest[level].config[kind].needvalue = atoi(row[offset++]);
		g_nation_quest[level].config[kind].awardkind[0] = atoi(row[offset++]);
		g_nation_quest[level].config[kind].awardkind[1] = atoi(row[offset++]);
		g_nation_quest[level].config[kind].awardkind[2] = atoi(row[offset++]);
		g_nation_quest[level].config[kind].awardkind[3] = atoi(row[offset++]);
		g_nation_quest[level].config[kind].awardkind[4] = atoi(row[offset++]);
		g_nation_quest[level].config[kind].awardnum[0] = atoi(row[offset++]);
		g_nation_quest[level].config[kind].awardnum[1] = atoi(row[offset++]);
		g_nation_quest[level].config[kind].awardnum[2] = atoi(row[offset++]);
		g_nation_quest[level].config[kind].awardnum[3] = atoi(row[offset++]);
		g_nation_quest[level].config[kind].awardnum[4] = atoi(row[offset++]);
		g_nation_quest[level].config[kind].other_awardkind = atoi(row[offset++]);
		g_nation_quest[level].config[kind].other_awardnum = atoi(row[offset++]);
		g_nation_quest[level].config[kind].other_awardodds = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int nationquest_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_nation_quest_maxnum; tmpi++ )
	{
		if ( g_nation_quest[tmpi].config )
		{
			free( g_nation_quest[tmpi].config );
			g_nation_quest[tmpi].config = NULL;
		}
	}
	if ( g_nation_quest )
	{
		free( g_nation_quest );
		g_nation_quest = NULL;
	}
	g_nation_quest_maxnum = 0;
	nationquest_init_auto();
	return 0;
}

