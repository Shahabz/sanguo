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
#include "auto_data_girl.h"
extern MYSQL *myData;
extern lua_State* servL;
GirlInfo *g_girlinfo = NULL;
int g_girlinfo_maxnum = 0;

int girlinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`kind`) from girl;" );
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
			g_girlinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_girlinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_girlinfo = (GirlInfo *)malloc( sizeof(GirlInfo)*g_girlinfo_maxnum );
	memset( g_girlinfo, 0, sizeof(GirlInfo)*g_girlinfo_maxnum );

	sprintf( szSQL, "select `kind`, max( `color` ) from girl group by `kind`;" );
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
		if ( kind < 0 || kind >= g_girlinfo_maxnum  )
			continue;
		g_girlinfo[kind].maxnum = atoi( row[1] ) + 1;
		g_girlinfo[kind].config = (GirlInfoConfig *)malloc( sizeof(GirlInfoConfig)*g_girlinfo[kind].maxnum );
		memset( g_girlinfo[kind].config, 0, sizeof(GirlInfoConfig)*g_girlinfo[kind].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `kind`,`color`,`init_color`,`type`,`attack_increase`,`defense_increase`,`attack_growth`,`defense_growth`,`troops_growth`,`private_herokind`,`private_attack_increase`,`private_defense_increase`,`private_attack_growth`,`private_defense_growth`,`private_troops_growth`,`soul`,`soulchange`,`love_star`,`private_love_star` from girl;" );
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
		if ( kind < 0 || kind >= g_girlinfo_maxnum  )
			continue;
		int color = atoi( row[1] );
		if ( color < 0 || color >= g_girlinfo[kind].maxnum )
			continue;
		g_girlinfo[kind].config[color].kind = atoi(row[offset++]);
		g_girlinfo[kind].config[color].color = atoi(row[offset++]);
		g_girlinfo[kind].config[color].init_color = atoi(row[offset++]);
		g_girlinfo[kind].config[color].type = atoi(row[offset++]);
		g_girlinfo[kind].config[color].attack_increase = atoi(row[offset++]);
		g_girlinfo[kind].config[color].defense_increase = atoi(row[offset++]);
		g_girlinfo[kind].config[color].attack_growth = atoi(row[offset++]);
		g_girlinfo[kind].config[color].defense_growth = atoi(row[offset++]);
		g_girlinfo[kind].config[color].troops_growth = atoi(row[offset++]);
		g_girlinfo[kind].config[color].private_herokind = atoi(row[offset++]);
		g_girlinfo[kind].config[color].private_attack_increase = atoi(row[offset++]);
		g_girlinfo[kind].config[color].private_defense_increase = atoi(row[offset++]);
		g_girlinfo[kind].config[color].private_attack_growth = atoi(row[offset++]);
		g_girlinfo[kind].config[color].private_defense_growth = atoi(row[offset++]);
		g_girlinfo[kind].config[color].private_troops_growth = atoi(row[offset++]);
		g_girlinfo[kind].config[color].soul = atoi(row[offset++]);
		g_girlinfo[kind].config[color].soulchange = atoi(row[offset++]);
		g_girlinfo[kind].config[color].love_star = atoi(row[offset++]);
		g_girlinfo[kind].config[color].private_love_star = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int girlinfo_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_girlinfo_maxnum; tmpi++ )
	{
		if ( g_girlinfo[tmpi].config )
		{
			free( g_girlinfo[tmpi].config );
			g_girlinfo[tmpi].config = NULL;
		}
	}
	if ( g_girlinfo )
	{
		free( g_girlinfo );
		g_girlinfo = NULL;
	}
	g_girlinfo_maxnum = 0;
	girlinfo_init_auto();
	return 0;
}

