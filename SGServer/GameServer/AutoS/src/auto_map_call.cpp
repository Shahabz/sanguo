#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_map_call.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int g_map_call_maxindex = 0;

int map_call_load_auto( LPCB_GETMAPCALL pCB_GetMapCall, LPCB_LOADMAPCALL pCB_LoadMapCall, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	MapCall *pMapCall;
	int index = 0;

	sprintf( szSQL, "select `index`,`actorid`,`limitlevel`,`endtimestamp`,`num`,`maxnum` from %s ", pTab );
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
		pMapCall = pCB_GetMapCall( atoi(row[0]) );
		if( pMapCall == NULL )
			continue;
		pMapCall->index = atoi(row[offset++]);
		pMapCall->actorid = atoi(row[offset++]);
		pMapCall->limitlevel = atoi(row[offset++]);
		pMapCall->endtimestamp = atoi(row[offset++]);
		pMapCall->num = atoi(row[offset++]);
		pMapCall->maxnum = atoi(row[offset++]);
		if( pCB_LoadMapCall )
			pCB_LoadMapCall( pMapCall->index );
		index = pMapCall->index;
		if ( index >= g_map_call_maxindex )
		{
			g_map_call_maxindex = index + 1;
		}
	}
	mysql_free_result( res );
	return 0;
}
int map_call_save_auto( MapCall *pMapCall, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pMapCall == NULL )
		return -1;

RE_MAPCALL_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`index`,`actorid`,`limitlevel`,`endtimestamp`,`num`,`maxnum`) Values('%d','%d','%d','%d','%d','%d')",pTab,pMapCall->index,pMapCall->actorid,pMapCall->limitlevel,pMapCall->endtimestamp,pMapCall->num,pMapCall->maxnum);
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
			goto RE_MAPCALL_UPDATE;
		}
		return -1;
	}
	return 0;
}

int map_call_batch_save_auto( MapCall *pMapCall, int maxcount, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pMapCall == NULL )
		return -1;

	char reconnect_flag = 0;
RE_MAPCALL_TRUNCATE:
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
				goto RE_MAPCALL_TRUNCATE;
			}
		}
	}

	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pMapCall[index].actorid <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`index`,`actorid`,`limitlevel`,`endtimestamp`,`num`,`maxnum`) Values('%d','%d','%d','%d','%d','%d')",pTab,pMapCall[index].index,pMapCall[index].actorid,pMapCall[index].limitlevel,pMapCall[index].endtimestamp,pMapCall[index].num,pMapCall[index].maxnum);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d','%d','%d','%d')",pMapCall[index].index,pMapCall[index].actorid,pMapCall[index].limitlevel,pMapCall[index].endtimestamp,pMapCall[index].num,pMapCall[index].maxnum);
			strcat( g_batchsql, szSQL );
		}
		count += 1;
		if ( count > 100 )
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
