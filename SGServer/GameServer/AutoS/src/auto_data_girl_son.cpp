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
#include "auto_data_girl_son.h"
extern MYSQL *myData;
extern lua_State* servL;
GirlSon *g_girlson = NULL;
int g_girlson_maxnum = 0;

int girlson_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`herokind`) from girl_son;" );
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
			g_girlson_maxnum = atoi( row[0] ) + 1;
		else
			g_girlson_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_girlson = (GirlSon *)malloc( sizeof(GirlSon)*g_girlson_maxnum );
	memset( g_girlson, 0, sizeof(GirlSon)*g_girlson_maxnum );

	sprintf( szSQL, "select `herokind`, max( `index` ) from girl_son group by `herokind`;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		int herokind = atoi( row[0] );
		if ( herokind < 0 || herokind >= g_girlson_maxnum  )
			continue;
		g_girlson[herokind].maxnum = atoi( row[1] ) + 1;
		g_girlson[herokind].config = (GirlSonConfig *)malloc( sizeof(GirlSonConfig)*g_girlson[herokind].maxnum );
		memset( g_girlson[herokind].config, 0, sizeof(GirlSonConfig)*g_girlson[herokind].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `herokind`,`index`,`born_odds`,`grow_sec`,`girlkind`,`attack`,`defense`,`attack_increase`,`defense_increase` from girl_son;" );
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
		int herokind = atoi( row[0] );
		if ( herokind < 0 || herokind >= g_girlson_maxnum  )
			continue;
		int index = atoi( row[1] );
		if ( index < 0 || index >= g_girlson[herokind].maxnum )
			continue;
		g_girlson[herokind].config[index].herokind = atoi(row[offset++]);
		g_girlson[herokind].config[index].index = atoi(row[offset++]);
		g_girlson[herokind].config[index].born_odds = atoi(row[offset++]);
		g_girlson[herokind].config[index].grow_sec = atoi(row[offset++]);
		g_girlson[herokind].config[index].girlkind = atoi(row[offset++]);
		g_girlson[herokind].config[index].attack = atoi(row[offset++]);
		g_girlson[herokind].config[index].defense = atoi(row[offset++]);
		g_girlson[herokind].config[index].attack_increase = atoi(row[offset++]);
		g_girlson[herokind].config[index].defense_increase = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int girlson_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_girlson_maxnum; tmpi++ )
	{
		if ( g_girlson[tmpi].config )
		{
			free( g_girlson[tmpi].config );
			g_girlson[tmpi].config = NULL;
		}
	}
	if ( g_girlson )
	{
		free( g_girlson );
		g_girlson = NULL;
	}
	g_girlson_maxnum = 0;
	girlson_init_auto();
	return 0;
}

