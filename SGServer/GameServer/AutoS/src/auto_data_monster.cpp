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
#include "auto_data_monster.h"
extern MYSQL *myData;
extern lua_State* servL;
MonsterInfo *g_monster = NULL;
int g_monster_maxnum = 0;

int monsterinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`monsterid`) from monster;" );
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
			g_monster_maxnum = atoi( row[0] ) + 1;
		else
			g_monster_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_monster = (MonsterInfo *)malloc( sizeof(MonsterInfo)*g_monster_maxnum );
	memset( g_monster, 0, sizeof(MonsterInfo)*g_monster_maxnum );

	sprintf( szSQL, "select `monsterid`,`shape`,`level`,`color`,`corps`,`attack`,`defense`,`troops`,`attack_increase`,`defense_increase`,`assault`,`defend`,`line`,`skill` from monster;" );
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
		int monsterid = atoi( row[0] );
		if ( monsterid < 0 || monsterid >= g_monster_maxnum  )
			continue;
		g_monster[monsterid].monsterid = atoi(row[offset++]);
		g_monster[monsterid].shape = atoi(row[offset++]);
		g_monster[monsterid].level = atoi(row[offset++]);
		g_monster[monsterid].color = atoi(row[offset++]);
		g_monster[monsterid].corps = atoi(row[offset++]);
		g_monster[monsterid].attack = atoi(row[offset++]);
		g_monster[monsterid].defense = atoi(row[offset++]);
		g_monster[monsterid].troops = atoi(row[offset++]);
		g_monster[monsterid].attack_increase = atoi(row[offset++]);
		g_monster[monsterid].defense_increase = atoi(row[offset++]);
		g_monster[monsterid].assault = atoi(row[offset++]);
		g_monster[monsterid].defend = atoi(row[offset++]);
		g_monster[monsterid].line = atoi(row[offset++]);
		g_monster[monsterid].skill = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int monsterinfo_reload_auto()
{
	if ( g_monster )
	{
		free( g_monster );
		g_monster = NULL;
	}
	g_monster_maxnum = 0;
	monsterinfo_init_auto();
	return 0;
}

