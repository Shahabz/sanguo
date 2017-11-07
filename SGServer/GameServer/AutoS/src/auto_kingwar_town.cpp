#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_kingwar_town.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int g_kingwar_town_maxindex = 0;

int kingwar_town_load_auto( LPCB_GETKINGWARTOWN pCB_GetKingwarTown, LPCB_LOADKINGWARTOWN pCB_LoadKingwarTown, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	KingwarTown *pKingwarTown;
	int id = 0;

	sprintf( szSQL, "select `id`,`nation`,`attack_total`,`defense_total`,`sec` from %s ", pTab );
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
		pKingwarTown = pCB_GetKingwarTown( atoi(row[0]) );
		if( pKingwarTown == NULL )
			continue;
		pKingwarTown->id = atoi(row[offset++]);
		pKingwarTown->nation = atoi(row[offset++]);
		pKingwarTown->attack_total = atoi(row[offset++]);
		pKingwarTown->defense_total = atoi(row[offset++]);
		pKingwarTown->sec = atoi(row[offset++]);
		if( pCB_LoadKingwarTown )
			pCB_LoadKingwarTown( pKingwarTown->id );
		id = pKingwarTown->id;
		if ( id >= g_kingwar_town_maxindex )
		{
			g_kingwar_town_maxindex = id + 1;
		}
	}
	mysql_free_result( res );
	return 0;
}
int kingwar_town_save_auto( KingwarTown *pKingwarTown, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pKingwarTown == NULL )
		return -1;

RE_KINGWARTOWN_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`id`,`nation`,`attack_total`,`defense_total`,`sec`) Values('%d','%d','%d','%d','%d')",pTab,pKingwarTown->id,pKingwarTown->nation,pKingwarTown->attack_total,pKingwarTown->defense_total,pKingwarTown->sec);
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
			goto RE_KINGWARTOWN_UPDATE;
		}
		return -1;
	}
	return 0;
}

int kingwar_town_batch_save_auto( KingwarTown *pKingwarTown, int maxcount, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pKingwarTown == NULL )
		return -1;

	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pKingwarTown[index].id <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`id`,`nation`,`attack_total`,`defense_total`,`sec`) Values('%d','%d','%d','%d','%d')",pTab,pKingwarTown[index].id,pKingwarTown[index].nation,pKingwarTown[index].attack_total,pKingwarTown[index].defense_total,pKingwarTown[index].sec);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d','%d','%d')",pKingwarTown[index].id,pKingwarTown[index].nation,pKingwarTown[index].attack_total,pKingwarTown[index].defense_total,pKingwarTown[index].sec);
			strcat( g_batchsql, szSQL );
		}
		count += 1;
		if ( count > 16 )
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
