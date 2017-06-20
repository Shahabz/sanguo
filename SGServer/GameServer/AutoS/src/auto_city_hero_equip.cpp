#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_city_hero_equip.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int city_hero_equip_load_auto( int cityid, int city_index, LPCB_GETHEROEQUIP pCB_GetHeroEquip, char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	HeroEquip *pHeroEquip;

	sprintf( szSQL, "select `cityid`,`herokind`,`offset`,`kind`,`ability0`,`ability1`,`ability2`,`ability3`,`value0`,`value1`,`value2`,`value3` from %s where cityid='%d'", pTab, cityid );
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
		pHeroEquip = pCB_GetHeroEquip( city_index, atoi(row[1]), atoi(row[2]) );
		if( pHeroEquip == NULL )
			continue;
		offset++;
		offset++;
		offset++;
		pHeroEquip->kind = atoi(row[offset++]);
		pHeroEquip->ability[0] = atoi(row[offset++]);
		pHeroEquip->ability[1] = atoi(row[offset++]);
		pHeroEquip->ability[2] = atoi(row[offset++]);
		pHeroEquip->ability[3] = atoi(row[offset++]);
		pHeroEquip->value[0] = atoi(row[offset++]);
		pHeroEquip->value[1] = atoi(row[offset++]);
		pHeroEquip->value[2] = atoi(row[offset++]);
		pHeroEquip->value[3] = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}
int city_hero_equip_save_auto( int cityid, int herokind, int offset, HeroEquip *pHeroEquip, char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pHeroEquip == NULL )
		return -1;

RE_HEROEQUIP_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`cityid`,`herokind`,`offset`,`kind`,`ability0`,`ability1`,`ability2`,`ability3`,`value0`,`value1`,`value2`,`value3`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,cityid,herokind,offset,pHeroEquip->kind,pHeroEquip->ability[0],pHeroEquip->ability[1],pHeroEquip->ability[2],pHeroEquip->ability[3],pHeroEquip->value[0],pHeroEquip->value[1],pHeroEquip->value[2],pHeroEquip->value[3]);
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
			goto RE_HEROEQUIP_UPDATE;
		}
		return -1;
	}
	return 0;
}

int city_hero_equip_batch_save_auto( int cityid, int herokind, HeroEquip *pHeroEquip, int maxcount,  char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pHeroEquip == NULL )
		return -1;

	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pHeroEquip[index].kind <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`cityid`,`herokind`,`offset`,`kind`,`ability0`,`ability1`,`ability2`,`ability3`,`value0`,`value1`,`value2`,`value3`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,cityid,herokind,index,pHeroEquip[index].kind,pHeroEquip[index].ability[0],pHeroEquip[index].ability[1],pHeroEquip[index].ability[2],pHeroEquip[index].ability[3],pHeroEquip[index].value[0],pHeroEquip[index].value[1],pHeroEquip[index].value[2],pHeroEquip[index].value[3]);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",cityid,herokind,index,pHeroEquip[index].kind,pHeroEquip[index].ability[0],pHeroEquip[index].ability[1],pHeroEquip[index].ability[2],pHeroEquip[index].ability[3],pHeroEquip[index].value[0],pHeroEquip[index].value[1],pHeroEquip[index].value[2],pHeroEquip[index].value[3]);
			strcat( g_batchsql, szSQL );
		}
		count += 1;
		if ( count > 6 )
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
