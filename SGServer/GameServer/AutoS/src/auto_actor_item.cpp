#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_actor_item.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int actor_item_load_auto( int actorid, int actor_index, LPCB_GETITEM pCB_GetItem, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	Item *pItem;

	sprintf( szSQL, "select `id`,`actorid`,`offset`,`kind`,`num`,`ability0`,`ability1`,`ability2`,`ability3`,`value0`,`value1`,`value2`,`value3`,`color_level` from %s where id='%d'", pTab, actorid );
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
		pItem = pCB_GetItem( actor_index, atoi(row[2]) );
		if( pItem == NULL )
			continue;
		pItem->id = atoll(row[offset++]);
		pItem->actorid = atoi(row[offset++]);
		pItem->offset = atoi(row[offset++]);
		pItem->m_kind = atoi(row[offset++]);
		pItem->m_num = atoi(row[offset++]);
		pItem->m_ability[0] = atoi(row[offset++]);
		pItem->m_ability[1] = atoi(row[offset++]);
		pItem->m_ability[2] = atoi(row[offset++]);
		pItem->m_ability[3] = atoi(row[offset++]);
		pItem->m_value[0] = atoi(row[offset++]);
		pItem->m_value[1] = atoi(row[offset++]);
		pItem->m_value[2] = atoi(row[offset++]);
		pItem->m_value[3] = atoi(row[offset++]);
		pItem->m_color_level = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}
int actor_item_save_auto( Item *pItem, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pItem == NULL )
		return -1;

	char sz64_id[21]={0};
RE_ITEM_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`id`,`actorid`,`offset`,`kind`,`num`,`ability0`,`ability1`,`ability2`,`ability3`,`value0`,`value1`,`value2`,`value3`,`color_level`) Values('%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,lltoa(pItem->id,sz64_id,10 ),pItem->actorid,pItem->offset,pItem->m_kind,pItem->m_num,pItem->m_ability[0],pItem->m_ability[1],pItem->m_ability[2],pItem->m_ability[3],pItem->m_value[0],pItem->m_value[1],pItem->m_value[2],pItem->m_value[3],pItem->m_color_level);
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
			goto RE_ITEM_UPDATE;
		}
		return -1;
	}
	return 0;
}

int actor_item_batch_save_auto( Item *pItem, int maxcount, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pItem == NULL )
		return -1;

	char sz64_id[21]={0};
	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pItem[index].id <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`id`,`actorid`,`offset`,`kind`,`num`,`ability0`,`ability1`,`ability2`,`ability3`,`value0`,`value1`,`value2`,`value3`,`color_level`) Values('%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,lltoa(pItem[index].id,sz64_id,10 ),pItem[index].actorid,pItem[index].offset,pItem[index].m_kind,pItem[index].m_num,pItem[index].m_ability[0],pItem[index].m_ability[1],pItem[index].m_ability[2],pItem[index].m_ability[3],pItem[index].m_value[0],pItem[index].m_value[1],pItem[index].m_value[2],pItem[index].m_value[3],pItem[index].m_color_level);
		}
		else
		{
			sprintf( szSQL, ",('%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",lltoa(pItem[index].id,sz64_id,10 ),pItem[index].actorid,pItem[index].offset,pItem[index].m_kind,pItem[index].m_num,pItem[index].m_ability[0],pItem[index].m_ability[1],pItem[index].m_ability[2],pItem[index].m_ability[3],pItem[index].m_value[0],pItem[index].m_value[1],pItem[index].m_value[2],pItem[index].m_value[3],pItem[index].m_color_level);
			strcat( g_batchsql, szSQL );
		}
		count += 1;
		if ( count > 256 )
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
