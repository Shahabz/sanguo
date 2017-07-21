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
#include "auto_data_item.h"
extern MYSQL *myData;
extern lua_State* servL;
ItemKind *g_itemkind = NULL;
int g_itemkind_maxnum = 0;

int itemkind_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(kind) from item;" );
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
			g_itemkind_maxnum = atoi( row[0] ) + 1;
		else
			g_itemkind_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_itemkind = (ItemKind *)malloc( sizeof(ItemKind)*g_itemkind_maxnum );
	memset( g_itemkind, 0, sizeof(ItemKind)*g_itemkind_maxnum );

	sprintf( szSQL, "select `kind`,`name`,`level`,`price`,`token`,`type`,`situation`,`overlap`,`base_ability0`,`base_ability1`,`base_value0`,`base_value1`,`ability0`,`ability1`,`ability2`,`ability3`,`value_min0`,`value_min1`,`value_min2`,`value_min3`,`value_max0`,`value_max1`,`value_max2`,`value_max3`,`color_level`,`sort` from item;" );
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
		if ( kind < 0 || kind >= g_itemkind_maxnum  )
			continue;
		g_itemkind[kind].m_kind = atoi(row[offset++]);
		memcpy( g_itemkind[kind].m_name, row[offset++], 32 ); g_itemkind[kind].m_name[31]=0;
		g_itemkind[kind].m_level = atoi(row[offset++]);
		g_itemkind[kind].m_price = atoi(row[offset++]);
		g_itemkind[kind].m_token = atoi(row[offset++]);
		g_itemkind[kind].m_type = atoi(row[offset++]);
		g_itemkind[kind].m_situation = atoi(row[offset++]);
		g_itemkind[kind].m_overlap = atoi(row[offset++]);
		g_itemkind[kind].m_base_ability[0] = atoi(row[offset++]);
		g_itemkind[kind].m_base_ability[1] = atoi(row[offset++]);
		g_itemkind[kind].m_base_value[0] = atoi(row[offset++]);
		g_itemkind[kind].m_base_value[1] = atoi(row[offset++]);
		g_itemkind[kind].m_ability[0] = atoi(row[offset++]);
		g_itemkind[kind].m_ability[1] = atoi(row[offset++]);
		g_itemkind[kind].m_ability[2] = atoi(row[offset++]);
		g_itemkind[kind].m_ability[3] = atoi(row[offset++]);
		g_itemkind[kind].m_value_min[0] = atoi(row[offset++]);
		g_itemkind[kind].m_value_min[1] = atoi(row[offset++]);
		g_itemkind[kind].m_value_min[2] = atoi(row[offset++]);
		g_itemkind[kind].m_value_min[3] = atoi(row[offset++]);
		g_itemkind[kind].m_value_max[0] = atoi(row[offset++]);
		g_itemkind[kind].m_value_max[1] = atoi(row[offset++]);
		g_itemkind[kind].m_value_max[2] = atoi(row[offset++]);
		g_itemkind[kind].m_value_max[3] = atoi(row[offset++]);
		g_itemkind[kind].m_color_level = atoi(row[offset++]);
		g_itemkind[kind].m_sort = atoi(row[offset++]);
	}
	mysql_free_result( res );
	itemkind_luatable_auto();
	return 0;
}

int itemkind_reload_auto()
{
	if ( g_itemkind )
	{
		free( g_itemkind );
		g_itemkind = NULL;
	}
	g_itemkind_maxnum = 0;
	itemkind_init_auto();
	return 0;
}

