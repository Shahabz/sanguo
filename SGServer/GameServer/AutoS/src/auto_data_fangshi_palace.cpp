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
#include "auto_data_fangshi_palace.h"
extern MYSQL *myData;
extern lua_State* servL;
FangshiPalace *g_fangshi_palace = NULL;
int g_fangshi_palace_maxnum = 0;

int fangshipalace_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`type`) from fangshi_palace;" );
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
			g_fangshi_palace_maxnum = atoi( row[0] ) + 1;
		else
			g_fangshi_palace_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_fangshi_palace = (FangshiPalace *)malloc( sizeof(FangshiPalace)*g_fangshi_palace_maxnum );
	memset( g_fangshi_palace, 0, sizeof(FangshiPalace)*g_fangshi_palace_maxnum );

	sprintf( szSQL, "select `type`, max( `index` ) from fangshi_palace group by `type`;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		int type = atoi( row[0] );
		if ( type < 0 || type >= g_fangshi_palace_maxnum  )
			continue;
		g_fangshi_palace[type].maxnum = atoi( row[1] ) + 1;
		g_fangshi_palace[type].config = (FangshiPalaceConfig *)malloc( sizeof(FangshiPalaceConfig)*g_fangshi_palace[type].maxnum );
		memset( g_fangshi_palace[type].config, 0, sizeof(FangshiPalaceConfig)*g_fangshi_palace[type].maxnum );
	}
	mysql_free_result( res );

	sprintf( szSQL, "select `type`,`index`,`awardkind`,`awardnum`,`weight`,`visit_weight`,`cond_herokind`,`cond_girlkind`,`cond_vip`,`cond_actorlevel`,`cond_citylevel` from fangshi_palace;" );
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
		int type = atoi( row[0] );
		if ( type < 0 || type >= g_fangshi_palace_maxnum  )
			continue;
		int index = atoi( row[1] );
		if ( index < 0 || index >= g_fangshi_palace[type].maxnum )
			continue;
		g_fangshi_palace[type].config[index].type = atoi(row[offset++]);
		g_fangshi_palace[type].config[index].index = atoi(row[offset++]);
		g_fangshi_palace[type].config[index].awardkind = atoi(row[offset++]);
		g_fangshi_palace[type].config[index].awardnum = atoi(row[offset++]);
		g_fangshi_palace[type].config[index].weight = atoi(row[offset++]);
		g_fangshi_palace[type].config[index].visit_weight = atoi(row[offset++]);
		g_fangshi_palace[type].config[index].cond_herokind = atoi(row[offset++]);
		g_fangshi_palace[type].config[index].cond_girlkind = atoi(row[offset++]);
		g_fangshi_palace[type].config[index].cond_vip = atoi(row[offset++]);
		g_fangshi_palace[type].config[index].cond_actorlevel = atoi(row[offset++]);
		g_fangshi_palace[type].config[index].cond_citylevel = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int fangshipalace_reload_auto()
{
	for ( int tmpi = 0; tmpi < g_fangshi_palace_maxnum; tmpi++ )
	{
		if ( g_fangshi_palace[tmpi].config )
		{
			free( g_fangshi_palace[tmpi].config );
			g_fangshi_palace[tmpi].config = NULL;
		}
	}
	if ( g_fangshi_palace )
	{
		free( g_fangshi_palace );
		g_fangshi_palace = NULL;
	}
	g_fangshi_palace_maxnum = 0;
	fangshipalace_init_auto();
	return 0;
}

