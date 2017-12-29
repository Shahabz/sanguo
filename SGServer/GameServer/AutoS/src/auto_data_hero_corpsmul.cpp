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
#include "auto_data_hero_corpsmul.h"
extern MYSQL *myData;
extern lua_State* servL;
HeroCorpsmulInfo *g_hero_corpsmul = NULL;
int g_hero_corpsmul_maxnum = 0;

int herocorpsmulinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`corps`) from hero_corpsmul;" );
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
			g_hero_corpsmul_maxnum = atoi( row[0] ) + 1;
		else
			g_hero_corpsmul_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_hero_corpsmul = (HeroCorpsmulInfo *)malloc( sizeof(HeroCorpsmulInfo)*g_hero_corpsmul_maxnum );
	memset( g_hero_corpsmul, 0, sizeof(HeroCorpsmulInfo)*g_hero_corpsmul_maxnum );

	sprintf( szSQL, "select `corps`,`target_corps0`,`target_corps1`,`target_corps2` from hero_corpsmul;" );
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
		int corps = atoi( row[0] );
		if ( corps < 0 || corps >= g_hero_corpsmul_maxnum  )
			continue;
		g_hero_corpsmul[corps].corps = atoi(row[offset++]);
		g_hero_corpsmul[corps].target_corps[0] = atoi(row[offset++]);
		g_hero_corpsmul[corps].target_corps[1] = atoi(row[offset++]);
		g_hero_corpsmul[corps].target_corps[2] = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int herocorpsmulinfo_reload_auto()
{
	if ( g_hero_corpsmul )
	{
		free( g_hero_corpsmul );
		g_hero_corpsmul = NULL;
	}
	g_hero_corpsmul_maxnum = 0;
	herocorpsmulinfo_init_auto();
	return 0;
}

