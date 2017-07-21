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
#include "auto_data_vip.h"
extern MYSQL *myData;
extern lua_State* servL;
VipInfo *g_vipinfo = NULL;
int g_vipinfo_maxnum = 0;

int vipinfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(level) from vip;" );
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
			g_vipinfo_maxnum = atoi( row[0] ) + 1;
		else
			g_vipinfo_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_vipinfo = (VipInfo *)malloc( sizeof(VipInfo)*g_vipinfo_maxnum );
	memset( g_vipinfo, 0, sizeof(VipInfo)*g_vipinfo_maxnum );

	sprintf( szSQL, "select `level`,`exp`,`ability0`,`ability1`,`ability2`,`ability3`,`ability4`,`ability5`,`value0`,`value1`,`value2`,`value3`,`value4`,`value5`,`goodsid` from vip;" );
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
		int level = atoi( row[0] );
		if ( level < 0 || level >= g_vipinfo_maxnum  )
			continue;
		g_vipinfo[level].level = atoi(row[offset++]);
		g_vipinfo[level].exp = atoi(row[offset++]);
		g_vipinfo[level].ability[0] = atoi(row[offset++]);
		g_vipinfo[level].ability[1] = atoi(row[offset++]);
		g_vipinfo[level].ability[2] = atoi(row[offset++]);
		g_vipinfo[level].ability[3] = atoi(row[offset++]);
		g_vipinfo[level].ability[4] = atoi(row[offset++]);
		g_vipinfo[level].ability[5] = atoi(row[offset++]);
		g_vipinfo[level].value[0] = atoi(row[offset++]);
		g_vipinfo[level].value[1] = atoi(row[offset++]);
		g_vipinfo[level].value[2] = atoi(row[offset++]);
		g_vipinfo[level].value[3] = atoi(row[offset++]);
		g_vipinfo[level].value[4] = atoi(row[offset++]);
		g_vipinfo[level].value[5] = atoi(row[offset++]);
		g_vipinfo[level].goodsid = atoi(row[offset++]);
	}
	mysql_free_result( res );
	vipinfo_luatable_auto();
	return 0;
}

int vipinfo_reload_auto()
{
	if ( g_vipinfo )
	{
		free( g_vipinfo );
		g_vipinfo = NULL;
	}
	g_vipinfo_maxnum = 0;
	vipinfo_init_auto();
	return 0;
}

