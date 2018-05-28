#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_map_pickup.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int g_map_pickup_maxindex = 0;

int map_pickup_load_auto( LPCB_GETMAPPICKUP pCB_GetMapPickup, LPCB_LOADMAPPICKUP pCB_LoadMapPickup, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	MapPickup *pMapPickup;
	int index = 0;

	sprintf( szSQL, "select `index`,`posx`,`posy`,`kind`,`value` from %s ", pTab );
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
		pMapPickup = pCB_GetMapPickup( atoi(row[0]) );
		if( pMapPickup == NULL )
			continue;
		pMapPickup->index = atoi(row[offset++]);
		pMapPickup->posx = atoi(row[offset++]);
		pMapPickup->posy = atoi(row[offset++]);
		pMapPickup->kind = atoi(row[offset++]);
		pMapPickup->value = atoi(row[offset++]);
		if( pCB_LoadMapPickup )
			pCB_LoadMapPickup( pMapPickup->index );
		index = pMapPickup->index;
		if ( index >= g_map_pickup_maxindex )
		{
			g_map_pickup_maxindex = index + 1;
		}
	}
	mysql_free_result( res );
	return 0;
}
int map_pickup_save_auto( MapPickup *pMapPickup, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pMapPickup == NULL )
		return -1;

RE_MAPPICKUP_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`index`,`posx`,`posy`,`kind`,`value`) Values('%d','%d','%d','%d','%d')",pTab,pMapPickup->index,pMapPickup->posx,pMapPickup->posy,pMapPickup->kind,pMapPickup->value);
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
			goto RE_MAPPICKUP_UPDATE;
		}
		return -1;
	}
	return 0;
}

int map_pickup_batch_save_auto( MapPickup *pMapPickup, int maxcount, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pMapPickup == NULL )
		return -1;

	char reconnect_flag = 0;
RE_MAPPICKUP_TRUNCATE:
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
				goto RE_MAPPICKUP_TRUNCATE;
			}
		}
	}

	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pMapPickup[index].kind <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`index`,`posx`,`posy`,`kind`,`value`) Values('%d','%d','%d','%d','%d')",pTab,pMapPickup[index].index,pMapPickup[index].posx,pMapPickup[index].posy,pMapPickup[index].kind,pMapPickup[index].value);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d','%d','%d')",pMapPickup[index].index,pMapPickup[index].posx,pMapPickup[index].posy,pMapPickup[index].kind,pMapPickup[index].value);
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
