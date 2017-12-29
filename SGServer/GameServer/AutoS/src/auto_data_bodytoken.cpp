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
#include "auto_data_bodytoken.h"
extern MYSQL *myData;
extern lua_State* servL;
BodyTokenInfo *g_bodytoken = NULL;
int g_bodytoken_maxnum = 0;

int bodytokeninfo_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`count`) from bodytoken;" );
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
			g_bodytoken_maxnum = atoi( row[0] ) + 1;
		else
			g_bodytoken_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_bodytoken = (BodyTokenInfo *)malloc( sizeof(BodyTokenInfo)*g_bodytoken_maxnum );
	memset( g_bodytoken, 0, sizeof(BodyTokenInfo)*g_bodytoken_maxnum );

	sprintf( szSQL, "select `count`,`token` from bodytoken;" );
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
		int count = atoi( row[0] );
		if ( count < 0 || count >= g_bodytoken_maxnum  )
			continue;
		g_bodytoken[count].count = atoi(row[offset++]);
		g_bodytoken[count].token = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int bodytokeninfo_reload_auto()
{
	if ( g_bodytoken )
	{
		free( g_bodytoken );
		g_bodytoken = NULL;
	}
	g_bodytoken_maxnum = 0;
	bodytokeninfo_init_auto();
	return 0;
}

