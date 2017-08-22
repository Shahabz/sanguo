#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_map_zone.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int g_map_zone_maxindex = 0;

int map_zone_load_auto( LPCB_GETMAPZONE pCB_GetMapZone, LPCB_LOADMAPZONE pCB_LoadMapZone, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	MapZone *pMapZone;
	int zoneid = 0;

	sprintf( szSQL, "select `zoneid`,`allow` from %s ", pTab );
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
		pMapZone = pCB_GetMapZone( zoneid );
		if( pMapZone == NULL )
			continue;
		pMapZone->zoneid = atoi(row[offset++]);
		pMapZone->allow = atoi(row[offset++]);
		if( pCB_LoadMapZone )
			pCB_LoadMapZone( pMapZone->zoneid );
		zoneid += 1;
	}
	g_map_zone_maxindex = zoneid;
	mysql_free_result( res );
	return 0;
}
int map_zone_save_auto( MapZone *pMapZone, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pMapZone == NULL )
		return -1;

RE_MAPZONE_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`zoneid`,`allow`) Values('%d','%d')",pTab,pMapZone->zoneid,pMapZone->allow);
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
			goto RE_MAPZONE_UPDATE;
		}
		return -1;
	}
	return 0;
}

int map_zone_batch_save_auto( MapZone *pMapZone, int maxcount, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pMapZone == NULL )
		return -1;

	char reconnect_flag = 0;
RE_MAPZONE_TRUNCATE:
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
				goto RE_MAPZONE_TRUNCATE;
			}
		}
	}

	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pMapZone[index].zoneid <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`zoneid`,`allow`) Values('%d','%d')",pTab,pMapZone[index].zoneid,pMapZone[index].allow);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d')",pMapZone[index].zoneid,pMapZone[index].allow);
			strcat( g_batchsql, szSQL );
		}
		count += 1;
		if ( count > 32 )
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
