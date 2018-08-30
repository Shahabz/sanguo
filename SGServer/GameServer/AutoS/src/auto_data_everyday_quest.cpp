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
#include "auto_data_everyday_quest.h"
extern MYSQL *myData;
extern lua_State* servL;
EverydayQuest *g_everyday_quest = NULL;
int g_everyday_quest_maxnum = 0;

int everydayquest_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from everyday_quest;" );
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
			g_everyday_quest_maxnum = atoi( row[0] ) + 1;
		else
			g_everyday_quest_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_everyday_quest = (EverydayQuest *)malloc( sizeof(EverydayQuest)*g_everyday_quest_maxnum );
	memset( g_everyday_quest, 0, sizeof(EverydayQuest)*g_everyday_quest_maxnum );

	sprintf( szSQL, "select `id`,`textid`,`needvalue`,`saveindex`,`levelmin`,`levelmax`,`awardkind0`,`awardkind1`,`awardnum0`,`awardnum1`,`sort` from everyday_quest;" );
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
		if ( id < 0 || id >= g_everyday_quest_maxnum  )
			continue;
		g_everyday_quest[id].id = atoi(row[offset++]);
		g_everyday_quest[id].textid = atoi(row[offset++]);
		g_everyday_quest[id].needvalue = atoi(row[offset++]);
		g_everyday_quest[id].saveindex = atoi(row[offset++]);
		g_everyday_quest[id].levelmin = atoi(row[offset++]);
		g_everyday_quest[id].levelmax = atoi(row[offset++]);
		g_everyday_quest[id].awardkind[0] = atoi(row[offset++]);
		g_everyday_quest[id].awardkind[1] = atoi(row[offset++]);
		g_everyday_quest[id].awardnum[0] = atoi(row[offset++]);
		g_everyday_quest[id].awardnum[1] = atoi(row[offset++]);
		g_everyday_quest[id].sort = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int everydayquest_reload_auto()
{
	if ( g_everyday_quest )
	{
		free( g_everyday_quest );
		g_everyday_quest = NULL;
	}
	g_everyday_quest_maxnum = 0;
	everydayquest_init_auto();
	return 0;
}

