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
#include "auto_data_vipbag.h"
extern MYSQL *myData;
extern lua_State* servL;
VipBag *g_vipbag = NULL;
int g_vipbag_maxnum = 0;

int vipbag_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`level`) from vipbag;" );
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
			g_vipbag_maxnum = atoi( row[0] ) + 1;
		else
			g_vipbag_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_vipbag = (VipBag *)malloc( sizeof(VipBag)*g_vipbag_maxnum );
	memset( g_vipbag, 0, sizeof(VipBag)*g_vipbag_maxnum );

	sprintf( szSQL, "select `level`,`token`,`original_token`,`awardkind0`,`awardkind1`,`awardkind2`,`awardkind3`,`awardkind4`,`awardkind5`,`awardnum0`,`awardnum1`,`awardnum2`,`awardnum3`,`awardnum4`,`awardnum5` from vipbag;" );
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
		if ( level < 0 || level >= g_vipbag_maxnum  )
			continue;
		g_vipbag[level].level = atoi(row[offset++]);
		g_vipbag[level].token = atoi(row[offset++]);
		g_vipbag[level].original_token = atoi(row[offset++]);
		g_vipbag[level].awardkind[0] = atoi(row[offset++]);
		g_vipbag[level].awardkind[1] = atoi(row[offset++]);
		g_vipbag[level].awardkind[2] = atoi(row[offset++]);
		g_vipbag[level].awardkind[3] = atoi(row[offset++]);
		g_vipbag[level].awardkind[4] = atoi(row[offset++]);
		g_vipbag[level].awardkind[5] = atoi(row[offset++]);
		g_vipbag[level].awardnum[0] = atoi(row[offset++]);
		g_vipbag[level].awardnum[1] = atoi(row[offset++]);
		g_vipbag[level].awardnum[2] = atoi(row[offset++]);
		g_vipbag[level].awardnum[3] = atoi(row[offset++]);
		g_vipbag[level].awardnum[4] = atoi(row[offset++]);
		g_vipbag[level].awardnum[5] = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int vipbag_reload_auto()
{
	if ( g_vipbag )
	{
		free( g_vipbag );
		g_vipbag = NULL;
	}
	g_vipbag_maxnum = 0;
	vipbag_init_auto();
	return 0;
}

