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
#include "auto_data_hero_colorup.h"
extern MYSQL *myData;
extern lua_State* servL;
HeroColorupInfo *g_hero_colorup = NULL;
int g_hero_colorup_maxnum = 0;

int herocolorupinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(color) from hero_colorup;" );
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
			g_hero_colorup_maxnum = atoi( row[0] ) + 1;
		else
			g_hero_colorup_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_hero_colorup = (HeroColorupInfo *)malloc( sizeof(HeroColorupInfo)*g_hero_colorup_maxnum );
	memset( g_hero_colorup, 0, sizeof(HeroColorupInfo)*g_hero_colorup_maxnum );

	sprintf( szSQL, "select `color`,`itemkind`,`itemnum`,`value`,`needvalue`,`odds`,`nextcolor` from hero_colorup;" );
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
		int color = atoi( row[0] );
		if ( color < 0 || color >= g_hero_colorup_maxnum  )
			continue;
		g_hero_colorup[color].color = atoi(row[offset++]);
		g_hero_colorup[color].itemkind = atoi(row[offset++]);
		g_hero_colorup[color].itemnum = atoi(row[offset++]);
		g_hero_colorup[color].value = atoi(row[offset++]);
		g_hero_colorup[color].needvalue = atoi(row[offset++]);
		g_hero_colorup[color].odds = atoi(row[offset++]);
		g_hero_colorup[color].nextcolor = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int herocolorupinfo_reload_auto()
{
	if ( g_hero_colorup )
	{
		free( g_hero_colorup );
		g_hero_colorup = NULL;
	}
	g_hero_colorup_maxnum = 0;
	herocolorupinfo_init_auto();
	return 0;
}

