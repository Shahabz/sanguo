#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_map_enemy.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int g_map_enemy_maxindex = 0;

int map_enemy_load_auto( LPCB_GETMAPENEMY pCB_GetMapEnemy, LPCB_LOADMAPENEMY pCB_LoadMapEnemy, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	MapEnemy *pMapEnemy;
	int index = 0;

	sprintf( szSQL, "select `index`,`posx`,`posy`,`kind` from %s ", pTab );
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
		pMapEnemy = pCB_GetMapEnemy( index );
		if( pMapEnemy == NULL )
			continue;
		pMapEnemy->index = atoi(row[offset++]);
		pMapEnemy->posx = atoi(row[offset++]);
		pMapEnemy->posy = atoi(row[offset++]);
		pMapEnemy->kind = atoi(row[offset++]);
		if( pCB_LoadMapEnemy )
			pCB_LoadMapEnemy( pMapEnemy->index );
		index += 1;
	}
	g_map_enemy_maxindex = index;
	mysql_free_result( res );
	return 0;
}
int map_enemy_save_auto( MapEnemy *pMapEnemy, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pMapEnemy == NULL )
		return -1;

RE_MAPENEMY_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`index`,`posx`,`posy`,`kind`) Values('%d','%d','%d','%d')",pTab,pMapEnemy->index,pMapEnemy->posx,pMapEnemy->posy,pMapEnemy->kind);
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
			goto RE_MAPENEMY_UPDATE;
		}
		return -1;
	}
	return 0;
}

int map_enemy_batch_save_auto( MapEnemy *pMapEnemy, int maxcount, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pMapEnemy == NULL )
		return -1;

	char reconnect_flag = 0;
RE_MAPENEMY_TRUNCATE:
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
				goto RE_MAPENEMY_TRUNCATE;
			}
		}
	}

	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pMapEnemy[index].index <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`index`,`posx`,`posy`,`kind`) Values('%d','%d','%d','%d')",pTab,pMapEnemy[index].index,pMapEnemy[index].posx,pMapEnemy[index].posy,pMapEnemy[index].kind);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d','%d')",pMapEnemy[index].index,pMapEnemy[index].posx,pMapEnemy[index].posy,pMapEnemy[index].kind);
			strcat( g_batchsql, szSQL );
		}
		count += 1;
		if ( count > 1024 )
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
