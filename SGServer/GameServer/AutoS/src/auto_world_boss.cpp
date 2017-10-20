#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_world_boss.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int g_world_boss_maxindex = 0;

int world_boss_load_auto( LPCB_GETWORLDBOSS pCB_GetWorldBoss, LPCB_LOADWORLDBOSS pCB_LoadWorldBoss, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	WorldBoss *pWorldBoss;
	int bossid = 0;

	sprintf( szSQL, "select `bossid`,`hp`,`maxhp` from %s ", pTab );
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
		pWorldBoss = pCB_GetWorldBoss( atoi(row[0]) );
		if( pWorldBoss == NULL )
			continue;
		pWorldBoss->bossid = atoi(row[offset++]);
		pWorldBoss->hp = atoi(row[offset++]);
		pWorldBoss->maxhp = atoi(row[offset++]);
		if( pCB_LoadWorldBoss )
			pCB_LoadWorldBoss( pWorldBoss->bossid );
		bossid = pWorldBoss->bossid;
		if ( bossid >= g_world_boss_maxindex )
		{
			g_world_boss_maxindex = bossid + 1;
		}
	}
	mysql_free_result( res );
	return 0;
}
int world_boss_save_auto( WorldBoss *pWorldBoss, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pWorldBoss == NULL )
		return -1;

RE_WORLDBOSS_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`bossid`,`hp`,`maxhp`) Values('%d','%d','%d')",pTab,pWorldBoss->bossid,pWorldBoss->hp,pWorldBoss->maxhp);
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
			goto RE_WORLDBOSS_UPDATE;
		}
		return -1;
	}
	return 0;
}

int world_boss_batch_save_auto( WorldBoss *pWorldBoss, int maxcount, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pWorldBoss == NULL )
		return -1;

	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pWorldBoss[index].bossid <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`bossid`,`hp`,`maxhp`) Values('%d','%d','%d')",pTab,pWorldBoss[index].bossid,pWorldBoss[index].hp,pWorldBoss[index].maxhp);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d')",pWorldBoss[index].bossid,pWorldBoss[index].hp,pWorldBoss[index].maxhp);
			strcat( g_batchsql, szSQL );
		}
		count += 1;
		if ( count > 8 )
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
