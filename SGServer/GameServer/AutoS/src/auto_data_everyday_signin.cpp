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
#include "auto_data_everyday_signin.h"
extern MYSQL *myData;
extern lua_State* servL;
everydaySignin *g_everyday_signin = NULL;
int g_everyday_signin_maxnum = 0;

int everydaysignin_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from everyday_signin;" );
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
			g_everyday_signin_maxnum = atoi( row[0] ) + 1;
		else
			g_everyday_signin_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_everyday_signin = (everydaySignin *)malloc( sizeof(everydaySignin)*g_everyday_signin_maxnum );
	memset( g_everyday_signin, 0, sizeof(everydaySignin)*g_everyday_signin_maxnum );

	sprintf( szSQL, "select `id`,`awardkind0`,`awardkind1`,`awardnum0`,`awardnum1`,`token` from everyday_signin;" );
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
		if ( id < 0 || id >= g_everyday_signin_maxnum  )
			continue;
		g_everyday_signin[id].id = atoi(row[offset++]);
		g_everyday_signin[id].awardkind[0] = atoi(row[offset++]);
		g_everyday_signin[id].awardkind[1] = atoi(row[offset++]);
		g_everyday_signin[id].awardnum[0] = atoi(row[offset++]);
		g_everyday_signin[id].awardnum[1] = atoi(row[offset++]);
		g_everyday_signin[id].token = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int everydaysignin_reload_auto()
{
	if ( g_everyday_signin )
	{
		free( g_everyday_signin );
		g_everyday_signin = NULL;
	}
	g_everyday_signin_maxnum = 0;
	everydaysignin_init_auto();
	return 0;
}

