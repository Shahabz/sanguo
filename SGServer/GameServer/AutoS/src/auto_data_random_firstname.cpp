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
#include "auto_data_random_firstname.h"
extern MYSQL *myData;
extern lua_State* servL;
FirstName *g_firstname = NULL;
int g_firstname_maxnum = 0;

int firstname_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from random_firstname;" );
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
			g_firstname_maxnum = atoi( row[0] ) + 1;
		else
			g_firstname_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_firstname = (FirstName *)malloc( sizeof(FirstName)*g_firstname_maxnum );
	memset( g_firstname, 0, sizeof(FirstName)*g_firstname_maxnum );

	sprintf( szSQL, "select `id`,`str` from random_firstname;" );
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
		if ( id < 0 || id >= g_firstname_maxnum  )
			continue;
		g_firstname[id].id = atoi(row[offset++]);
		memcpy( g_firstname[id].str, row[offset++], 16 ); g_firstname[id].str[15]=0;
	}
	mysql_free_result( res );
	return 0;
}

int firstname_reload_auto()
{
	if ( g_firstname )
	{
		free( g_firstname );
		g_firstname = NULL;
	}
	g_firstname_maxnum = 0;
	firstname_init_auto();
	return 0;
}

