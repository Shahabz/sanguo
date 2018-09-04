#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_map_activity.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int g_map_activity_maxindex = 0;

int map_activity_load_auto( LPCB_GETMAPACTIVITY pCB_GetMapActivity, LPCB_LOADMAPACTIVITY pCB_LoadMapActivity, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	MapActivity *pMapActivity;
	int index = 0;

	sprintf( szSQL, "select `index`,`posx`,`posy`,`kind`,`deltime`,`actorid`,`hp`,`maxhp`,`activityid` from %s ", pTab );
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
		pMapActivity = pCB_GetMapActivity( atoi(row[0]) );
		if( pMapActivity == NULL )
			continue;
		pMapActivity->index = atoi(row[offset++]);
		pMapActivity->posx = atoi(row[offset++]);
		pMapActivity->posy = atoi(row[offset++]);
		pMapActivity->kind = atoi(row[offset++]);
		pMapActivity->deltime = atoi(row[offset++]);
		pMapActivity->actorid = atoi(row[offset++]);
		pMapActivity->hp = atoi(row[offset++]);
		pMapActivity->maxhp = atoi(row[offset++]);
		pMapActivity->activityid = atoi(row[offset++]);
		if( pCB_LoadMapActivity )
			pCB_LoadMapActivity( pMapActivity->index );
		index = pMapActivity->index;
		if ( index >= g_map_activity_maxindex )
		{
			g_map_activity_maxindex = index + 1;
		}
	}
	mysql_free_result( res );
	return 0;
}
int map_activity_save_auto( MapActivity *pMapActivity, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pMapActivity == NULL )
		return -1;

RE_MAPACTIVITY_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`index`,`posx`,`posy`,`kind`,`deltime`,`actorid`,`hp`,`maxhp`,`activityid`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,pMapActivity->index,pMapActivity->posx,pMapActivity->posy,pMapActivity->kind,pMapActivity->deltime,pMapActivity->actorid,pMapActivity->hp,pMapActivity->maxhp,pMapActivity->activityid);
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
			goto RE_MAPACTIVITY_UPDATE;
		}
		return -1;
	}
	return 0;
}

int map_activity_batch_save_auto( MapActivity *pMapActivity, int maxcount, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pMapActivity == NULL )
		return -1;

	char reconnect_flag = 0;
RE_MAPACTIVITY_TRUNCATE:
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
				goto RE_MAPACTIVITY_TRUNCATE;
			}
		}
	}

	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pMapActivity[index].kind <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`index`,`posx`,`posy`,`kind`,`deltime`,`actorid`,`hp`,`maxhp`,`activityid`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,pMapActivity[index].index,pMapActivity[index].posx,pMapActivity[index].posy,pMapActivity[index].kind,pMapActivity[index].deltime,pMapActivity[index].actorid,pMapActivity[index].hp,pMapActivity[index].maxhp,pMapActivity[index].activityid);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d','%d','%d','%d','%d','%d','%d')",pMapActivity[index].index,pMapActivity[index].posx,pMapActivity[index].posy,pMapActivity[index].kind,pMapActivity[index].deltime,pMapActivity[index].actorid,pMapActivity[index].hp,pMapActivity[index].maxhp,pMapActivity[index].activityid);
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
