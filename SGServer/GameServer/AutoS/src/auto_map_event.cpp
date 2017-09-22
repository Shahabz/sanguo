#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_map_event.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int g_map_event_maxindex = 0;

int map_event_load_auto( LPCB_GETMAPEVENT pCB_GetMapEvent, LPCB_LOADMAPEVENT pCB_LoadMapEvent, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	MapEvent *pMapEvent;
	int index = 0;

	sprintf( szSQL, "select `index`,`posx`,`posy`,`kind`,`waitsec`,`actorid`,`state` from %s ", pTab );
	if( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myGame) );
		write_gamelog( "%s", szSQL );
		if( mysql_ping(myGame) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	while( ( row = mysql_fetch_row( res ) ) )
	{
		offset = 0;
		pMapEvent = pCB_GetMapEvent( atoi(row[0]) );
		if( pMapEvent == NULL )
			continue;
		pMapEvent->index = atoi(row[offset++]);
		pMapEvent->posx = atoi(row[offset++]);
		pMapEvent->posy = atoi(row[offset++]);
		pMapEvent->kind = atoi(row[offset++]);
		pMapEvent->waitsec = atoi(row[offset++]);
		pMapEvent->actorid = atoi(row[offset++]);
		pMapEvent->state = atoi(row[offset++]);
		if( pCB_LoadMapEvent )
			pCB_LoadMapEvent( pMapEvent->index );
		index = pMapEvent->index;
		if ( index >= g_map_event_maxindex )
		{
			g_map_event_maxindex = index + 1;
		}
	}
	mysql_free_result( res );
	return 0;
}
int map_event_save_auto( MapEvent *pMapEvent, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pMapEvent == NULL )
		return -1;

RE_MAPEVENT_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`index`,`posx`,`posy`,`kind`,`waitsec`,`actorid`,`state`) Values('%d','%d','%d','%d','%d','%d','%d')",pTab,pMapEvent->index,pMapEvent->posx,pMapEvent->posy,pMapEvent->kind,pMapEvent->waitsec,pMapEvent->actorid,pMapEvent->state);
	if( fp )
	{
		fprintf( fp, "%s;\n", szSQL );
	}
	else if( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myGame) );
		write_gamelog( "%s", szSQL );
		if( reconnect_flag )
			return -1;
		if( mysql_ping(myGame) != 0 )
		{
			db_reconnect_game();
			reconnect_flag = 1;
			goto RE_MAPEVENT_UPDATE;
		}
		return -1;
	}
	return 0;
}

int map_event_batch_save_auto( MapEvent *pMapEvent, int maxcount, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pMapEvent == NULL )
		return -1;

	char reconnect_flag = 0;
RE_MAPEVENT_TRUNCATE:
	sprintf( szSQL, "TRUNCATE `%s`", pTab );
	if( fp )
	{
		fprintf( fp, "%s;\n", szSQL );
	}
	else if( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myGame) );
		write_gamelog( "%s", szSQL );
		if( reconnect_flag == 0 )
		{
			if( mysql_ping(myGame) != 0 )
			{
				db_reconnect_game();
				reconnect_flag = 1;
				goto RE_MAPEVENT_TRUNCATE;
			}
		}
	}

	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pMapEvent[index].kind <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`index`,`posx`,`posy`,`kind`,`waitsec`,`actorid`,`state`) Values('%d','%d','%d','%d','%d','%d','%d')",pTab,pMapEvent[index].index,pMapEvent[index].posx,pMapEvent[index].posy,pMapEvent[index].kind,pMapEvent[index].waitsec,pMapEvent[index].actorid,pMapEvent[index].state);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d','%d','%d','%d','%d')",pMapEvent[index].index,pMapEvent[index].posx,pMapEvent[index].posy,pMapEvent[index].kind,pMapEvent[index].waitsec,pMapEvent[index].actorid,pMapEvent[index].state);
			strcat( g_batchsql, szSQL );
		}
		count += 1;
		if ( count > 2048 )
		{
			count = 0;
			db_query( fp, g_batchsql );
			memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
		}
	}
	if ( count > 0 )
	{
		count = 0;
		db_query( fp, g_batchsql );
		memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	}
	return 0;
}
