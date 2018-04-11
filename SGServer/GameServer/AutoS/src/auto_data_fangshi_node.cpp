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
#include "auto_data_fangshi_node.h"
extern MYSQL *myData;
extern lua_State* servL;
FangshiNode *g_fangshi_node = NULL;
int g_fangshi_node_maxnum = 0;

int fangshinode_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from fangshi_node;" );
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
			g_fangshi_node_maxnum = atoi( row[0] ) + 1;
		else
			g_fangshi_node_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_fangshi_node = (FangshiNode *)malloc( sizeof(FangshiNode)*g_fangshi_node_maxnum );
	memset( g_fangshi_node, 0, sizeof(FangshiNode)*g_fangshi_node_maxnum );

	sprintf( szSQL, "select `id`,`awardgroup`,`nextid0`,`nextid1`,`nextid2`,`nextodds0`,`nextodds1`,`nextodds2`,`preid` from fangshi_node;" );
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
		if ( id < 0 || id >= g_fangshi_node_maxnum  )
			continue;
		g_fangshi_node[id].id = atoi(row[offset++]);
		g_fangshi_node[id].awardgroup = atoi(row[offset++]);
		g_fangshi_node[id].nextid[0] = atoi(row[offset++]);
		g_fangshi_node[id].nextid[1] = atoi(row[offset++]);
		g_fangshi_node[id].nextid[2] = atoi(row[offset++]);
		g_fangshi_node[id].nextodds[0] = atoi(row[offset++]);
		g_fangshi_node[id].nextodds[1] = atoi(row[offset++]);
		g_fangshi_node[id].nextodds[2] = atoi(row[offset++]);
		g_fangshi_node[id].preid = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int fangshinode_reload_auto()
{
	if ( g_fangshi_node )
	{
		free( g_fangshi_node );
		g_fangshi_node = NULL;
	}
	g_fangshi_node_maxnum = 0;
	fangshinode_init_auto();
	return 0;
}

