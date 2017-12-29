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
#include "auto_data_hero_visit.h"
extern MYSQL *myData;
extern lua_State* servL;
HeroVisit *g_hero_visit = NULL;
int g_hero_visit_maxnum = 0;

int herovisit_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from hero_visit;" );
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
			g_hero_visit_maxnum = atoi( row[0] ) + 1;
		else
			g_hero_visit_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_hero_visit = (HeroVisit *)malloc( sizeof(HeroVisit)*g_hero_visit_maxnum );
	memset( g_hero_visit, 0, sizeof(HeroVisit)*g_hero_visit_maxnum );

	sprintf( szSQL, "select `id`,`awardgroup`,`kind`,`num`,`color`,`value` from hero_visit;" );
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
		if ( id < 0 || id >= g_hero_visit_maxnum  )
			continue;
		g_hero_visit[id].id = atoi(row[offset++]);
		g_hero_visit[id].awardgroup = atoi(row[offset++]);
		g_hero_visit[id].kind = atoi(row[offset++]);
		g_hero_visit[id].num = atoi(row[offset++]);
		g_hero_visit[id].color = atoi(row[offset++]);
		g_hero_visit[id].value = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int herovisit_reload_auto()
{
	if ( g_hero_visit )
	{
		free( g_hero_visit );
		g_hero_visit = NULL;
	}
	g_hero_visit_maxnum = 0;
	herovisit_init_auto();
	return 0;
}

