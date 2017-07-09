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

	sprintf( szSQL, "select max(monsterid) from monster;" );
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

	sprintf( szSQL, "select monsterid, max( type ) from monster group by monsterid;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		int monsterid = atoi( row[0] );
		if ( monsterid < 0 || monsterid >= g_monster_maxnum  )
			continue;
		g_monster[monsterid].maxnum = atoi( row[1] ) + 1;
		g_monster[monsterid].config = (MonsterInfoConfig *)malloc( sizeof(MonsterInfoConfig)*g_monster[monsterid].maxnum );
		memset( g_monster[monsterid].config, 0, sizeof(MonsterInfoConfig)*g_monster[monsterid].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `monsterid`,`type`,`color`,`corps`,`attack`,`defense`,`troops`,`attack_growth`,`defense_growth`,`troops_growth`,`attack_increase`,`defense_increase`,`assault`,`defend`,`line`,`skill` from monster;" );
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
		int type = atoi( row[1] );
		if ( type < 0 || type >= g_monster[monsterid].maxnum )
			continue;
		g_monster[monsterid].config[type].monsterid = atoi(row[offset++]);
		g_monster[monsterid].config[type].type = atoi(row[offset++]);
		g_monster[monsterid].config[type].color = atoi(row[offset++]);
		g_monster[monsterid].config[type].corps = atoi(row[offset++]);
		g_monster[monsterid].config[type].attack = atoi(row[offset++]);
		g_monster[monsterid].config[type].defense = atoi(row[offset++]);
		g_monster[monsterid].config[type].troops = atoi(row[offset++]);
		g_monster[monsterid].config[type].attack_growth = atoi(row[offset++]);
		g_monster[monsterid].config[type].defense_growth = atoi(row[offset++]);
		g_monster[monsterid].config[type].troops_growth = atoi(row[offset++]);
		g_monster[monsterid].config[type].attack_increase = atoi(row[offset++]);
		g_monster[monsterid].config[type].defense_increase = atoi(row[offset++]);
		g_monster[monsterid].config[type].assault = atoi(row[offset++]);
		g_monster[monsterid].config[type].defend = atoi(row[offset++]);
		g_monster[monsterid].config[type].line = atoi(row[offset++]);
		g_monster[monsterid].config[type].skill = atoi(row[offset++]);
	}
	mysql_free_result( res );
	monsterinfo_luatable_auto();
	return 0;
}

int monsterinfo_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_monster_maxnum; tmpi++ )
	{
		if ( g_monster[tmpi].config )
		{
			free( g_monster[tmpi].config );
			g_monster[tmpi].config = NULL;
		}
	}
	if ( g_monster )
	{
		free( g_monster );
		g_monster = NULL;
	}
	g_monster_maxnum = 0;
	monsterinfo_init_auto();
	return 0;
}

int monsterinfo_luatable_auto()
{
	lua_newtable( servL );
	lua_setglobal( servL, "g_monster" );

	lua_pushinteger( servL, g_monster_maxnum );
	lua_setglobal( servL, "g_monster_maxnum" );
	return 0;
}
