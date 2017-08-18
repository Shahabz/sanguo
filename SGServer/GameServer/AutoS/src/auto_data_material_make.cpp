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
#include "auto_data_material_make.h"
extern MYSQL *myData;
extern lua_State* servL;
MaterialMakeInfo *g_material_make = NULL;
int g_material_make_maxnum = 0;

int materialmakeinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(id) from material_make;" );
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
			g_material_make_maxnum = atoi( row[0] ) + 1;
		else
			g_material_make_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_material_make = (MaterialMakeInfo *)malloc( sizeof(MaterialMakeInfo)*g_material_make_maxnum );
	memset( g_material_make, 0, sizeof(MaterialMakeInfo)*g_material_make_maxnum );

	sprintf( szSQL, "select `id`,`sec`,`silver`,`wood`,`itemkind0`,`itemkind1`,`itemkind2`,`itemkind3`,`itemkind4`,`itemkind5`,`materialkind0`,`materialkind1`,`materialkind2`,`materialkind3`,`num_odds0`,`num_odds1`,`num_odds2`,`num_odds3`,`num_odds4` from material_make;" );
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
		if ( id < 0 || id >= g_material_make_maxnum  )
			continue;
		g_material_make[id].id = atoi(row[offset++]);
		g_material_make[id].sec = atoi(row[offset++]);
		g_material_make[id].silver = atoi(row[offset++]);
		g_material_make[id].wood = atoi(row[offset++]);
		g_material_make[id].itemkind[0] = atoi(row[offset++]);
		g_material_make[id].itemkind[1] = atoi(row[offset++]);
		g_material_make[id].itemkind[2] = atoi(row[offset++]);
		g_material_make[id].itemkind[3] = atoi(row[offset++]);
		g_material_make[id].itemkind[4] = atoi(row[offset++]);
		g_material_make[id].itemkind[5] = atoi(row[offset++]);
		g_material_make[id].materialkind[0] = atoi(row[offset++]);
		g_material_make[id].materialkind[1] = atoi(row[offset++]);
		g_material_make[id].materialkind[2] = atoi(row[offset++]);
		g_material_make[id].materialkind[3] = atoi(row[offset++]);
		g_material_make[id].num_odds[0] = atoi(row[offset++]);
		g_material_make[id].num_odds[1] = atoi(row[offset++]);
		g_material_make[id].num_odds[2] = atoi(row[offset++]);
		g_material_make[id].num_odds[3] = atoi(row[offset++]);
		g_material_make[id].num_odds[4] = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int materialmakeinfo_reload_auto()
{
	if ( g_material_make )
	{
		free( g_material_make );
		g_material_make = NULL;
	}
	g_material_make_maxnum = 0;
	materialmakeinfo_init_auto();
	return 0;
}

