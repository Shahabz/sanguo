#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_actor_equip.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int actor_equip_load_auto( int actorid, int actor_index, LPCB_GETEQUIP pCB_GetEquip, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	Equip *pEquip;

	sprintf( szSQL, "select `id`,`actorid`,`offset`,`kind`,`washid` from %s where id='%d'", pTab, actorid );
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
		pEquip = pCB_GetEquip( actor_index, atoi(row[2]) );
		if( pEquip == NULL )
			continue;
		pEquip->id = atoll(row[offset++]);
		pEquip->actorid = atoi(row[offset++]);
		pEquip->offset = atoi(row[offset++]);
		pEquip->kind = atoi(row[offset++]);
		memcpy( pEquip->washid, row[offset++], 6 ); pEquip->washid[5]=0;
	}
	mysql_free_result( res );
	return 0;
}
int actor_equip_save_auto( Equip *pEquip, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pEquip == NULL )
		return -1;

	char sz64_id[21]={0};
	char szText_washid[MAX_PATH]={0};
RE_EQUIP_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`id`,`actorid`,`offset`,`kind`,`washid`) Values('%s','%d','%d','%d','%s')",pTab,lltoa(pEquip->id,sz64_id,10 ),pEquip->actorid,pEquip->offset,pEquip->kind,db_escape((const char *)pEquip->washid,szText_washid,0));
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
			goto RE_EQUIP_UPDATE;
		}
		return -1;
	}
	return 0;
}

int actor_equip_batch_save_auto( Equip *pEquip, int maxcount, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pEquip == NULL )
		return -1;

	char sz64_id[21]={0};
	char szText_washid[MAX_PATH]={0};
	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pEquip[index].id <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`id`,`actorid`,`offset`,`kind`,`washid`) Values('%s','%d','%d','%d','%s')",pTab,lltoa(pEquip[index].id,sz64_id,10 ),pEquip[index].actorid,pEquip[index].offset,pEquip[index].kind,db_escape((const char *)pEquip[index].washid,szText_washid,0));
		}
		else
		{
			sprintf( szSQL, ",('%s','%d','%d','%d','%s')",lltoa(pEquip[index].id,sz64_id,10 ),pEquip[index].actorid,pEquip[index].offset,pEquip[index].kind,db_escape((const char *)pEquip[index].washid,szText_washid,0));
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
