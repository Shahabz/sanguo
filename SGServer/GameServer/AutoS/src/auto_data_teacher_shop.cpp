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
#include "auto_data_teacher_shop.h"
extern MYSQL *myData;
extern lua_State* servL;
TeacherShop *g_teacher_shop = NULL;
int g_teacher_shop_maxnum = 0;

int teachershop_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from teacher_shop;" );
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
			g_teacher_shop_maxnum = atoi( row[0] ) + 1;
		else
			g_teacher_shop_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_teacher_shop = (TeacherShop *)malloc( sizeof(TeacherShop)*g_teacher_shop_maxnum );
	memset( g_teacher_shop, 0, sizeof(TeacherShop)*g_teacher_shop_maxnum );

	sprintf( szSQL, "select `id`,`itemkind`,`itemnum`,`showlevel`,`buylevel`,`point` from teacher_shop;" );
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
		if ( id < 0 || id >= g_teacher_shop_maxnum  )
			continue;
		g_teacher_shop[id].id = atoi(row[offset++]);
		g_teacher_shop[id].itemkind = atoi(row[offset++]);
		g_teacher_shop[id].itemnum = atoi(row[offset++]);
		g_teacher_shop[id].showlevel = atoi(row[offset++]);
		g_teacher_shop[id].buylevel = atoi(row[offset++]);
		g_teacher_shop[id].point = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int teachershop_reload_auto()
{
	if ( g_teacher_shop )
	{
		free( g_teacher_shop );
		g_teacher_shop = NULL;
	}
	g_teacher_shop_maxnum = 0;
	teachershop_init_auto();
	return 0;
}

