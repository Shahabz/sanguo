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
#include "auto_data_quest_talk.h"
extern MYSQL *myData;
extern lua_State* servL;
QuestTalk *g_quest_talk = NULL;
int g_quest_talk_maxnum = 0;

int questtalk_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from quest_talk;" );
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
			g_quest_talk_maxnum = atoi( row[0] ) + 1;
		else
			g_quest_talk_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_quest_talk = (QuestTalk *)malloc( sizeof(QuestTalk)*g_quest_talk_maxnum );
	memset( g_quest_talk, 0, sizeof(QuestTalk)*g_quest_talk_maxnum );

	sprintf( szSQL, "select `id`,`nextid`,`herokind`,`talk_textid`,`btn_textid`,`trigger_kind0`,`trigger_kind1`,`trigger_num0`,`trigger_num1`,`limit_questid`,`complete_questid` from quest_talk;" );
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
		if ( id < 0 || id >= g_quest_talk_maxnum  )
			continue;
		g_quest_talk[id].id = atoi(row[offset++]);
		g_quest_talk[id].nextid = atoi(row[offset++]);
		g_quest_talk[id].herokind = atoi(row[offset++]);
		g_quest_talk[id].talk_textid = atoi(row[offset++]);
		g_quest_talk[id].btn_textid = atoi(row[offset++]);
		g_quest_talk[id].trigger_kind[0] = atoi(row[offset++]);
		g_quest_talk[id].trigger_kind[1] = atoi(row[offset++]);
		g_quest_talk[id].trigger_num[0] = atoi(row[offset++]);
		g_quest_talk[id].trigger_num[1] = atoi(row[offset++]);
		g_quest_talk[id].limit_questid = atoi(row[offset++]);
		g_quest_talk[id].complete_questid = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int questtalk_reload_auto()
{
	if ( g_quest_talk )
	{
		free( g_quest_talk );
		g_quest_talk = NULL;
	}
	g_quest_talk_maxnum = 0;
	questtalk_init_auto();
	return 0;
}

