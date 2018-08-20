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
#include "auto_data_everyday_event.h"
extern MYSQL *myData;
extern lua_State* servL;
EverydayEvent *g_everyday_event = NULL;
int g_everyday_event_maxnum = 0;

int everydayevent_init_auto()
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[2048] = {0};
	int offset = 0;

	sprintf( szSQL, "select max(`id`) from everyday_event;" );
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
			g_everyday_event_maxnum = atoi( row[0] ) + 1;
		else
			g_everyday_event_maxnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_everyday_event = (EverydayEvent *)malloc( sizeof(EverydayEvent)*g_everyday_event_maxnum );
	memset( g_everyday_event, 0, sizeof(EverydayEvent)*g_everyday_event_maxnum );

	sprintf( szSQL, "select `id`,`textid`,`talkid0`,`talkid1`,`awardkind0`,`awardkind1`,`awardnum0`,`awardnum1` from everyday_event;" );
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
		if ( id < 0 || id >= g_everyday_event_maxnum  )
			continue;
		g_everyday_event[id].id = atoi(row[offset++]);
		g_everyday_event[id].textid = atoi(row[offset++]);
		g_everyday_event[id].talkid[0] = atoi(row[offset++]);
		g_everyday_event[id].talkid[1] = atoi(row[offset++]);
		g_everyday_event[id].awardkind[0] = atoi(row[offset++]);
		g_everyday_event[id].awardkind[1] = atoi(row[offset++]);
		g_everyday_event[id].awardnum[0] = atoi(row[offset++]);
		g_everyday_event[id].awardnum[1] = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}

int everydayevent_reload_auto()
{
	if ( g_everyday_event )
	{
		free( g_everyday_event );
		g_everyday_event = NULL;
	}
	g_everyday_event_maxnum = 0;
	everydayevent_init_auto();
	return 0;
}

