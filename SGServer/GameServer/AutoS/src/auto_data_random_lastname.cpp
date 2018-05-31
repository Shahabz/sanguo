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
#include "auto_data_random_lastname.h"
extern MYSQL *myData;
extern lua_State* servL;
LastName *g_lastname = NULL;
int g_lastname_maxnum = 0;

int lastname_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from random_lastname;" );
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
			g_lastname_maxnum = atoi( row[0] ) + 1;
		else
			g_lastname_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_lastname = (LastName *)malloc( sizeof(LastName)*g_lastname_maxnum );
	memset( g_lastname, 0, sizeof(LastName)*g_lastname_maxnum );

	sprintf( szSQL, "select `id`,`str` from random_lastname;" );
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
		if ( id < 0 || id >= g_lastname_maxnum  )
			continue;
		g_lastname[id].id = atoi(row[offset++]);
		memcpy( g_lastname[id].str, row[offset++], 16 ); g_lastname[id].str[15]=0;
	}
	mysql_free_result( res );
	return 0;
}

int lastname_reload_auto()
{
	if ( g_lastname )
	{
		free( g_lastname );
		g_lastname = NULL;
	}
	g_lastname_maxnum = 0;
	lastname_init_auto();
	return 0;
}

