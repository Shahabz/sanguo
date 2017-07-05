#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_actor_hero.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int actor_hero_load_auto( int actorid, int actor_index, LPCB_GETHERO pCB_GetHero, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	Hero *pHero;

	sprintf( szSQL, "select `id`,`actorid`,`offset`,`kind`,`color`,`level`,`exp`,`soldiers`,`attack_wash`,`defense_wash`,`troops_wash` from %s where id='%d'", pTab, actorid );
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
		pHero = pCB_GetHero( actor_index, atoi(row[2]) );
		if( pHero == NULL )
			continue;
		pHero->id = atoll(row[offset++]);
		pHero->actorid = atoi(row[offset++]);
		pHero->offset = atoi(row[offset++]);
		pHero->kind = atoi(row[offset++]);
		pHero->color = atoi(row[offset++]);
		pHero->level = atoi(row[offset++]);
		pHero->exp = atoi(row[offset++]);
		pHero->soldiers = atoi(row[offset++]);
		pHero->attack_wash = atoi(row[offset++]);
		pHero->defense_wash = atoi(row[offset++]);
		pHero->troops_wash = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}
int actor_hero_save_auto( Hero *pHero, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pHero == NULL )
		return -1;

	char sz64_id[21]={0};
RE_HERO_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`id`,`actorid`,`offset`,`kind`,`color`,`level`,`exp`,`soldiers`,`attack_wash`,`defense_wash`,`troops_wash`) Values('%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,lltoa(pHero->id,sz64_id,10 ),pHero->actorid,pHero->offset,pHero->kind,pHero->color,pHero->level,pHero->exp,pHero->soldiers,pHero->attack_wash,pHero->defense_wash,pHero->troops_wash);
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
			goto RE_HERO_UPDATE;
		}
		return -1;
	}
	return 0;
}

int actor_hero_batch_save_auto( Hero *pHero, int maxcount, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pHero == NULL )
		return -1;

	char sz64_id[21]={0};
	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pHero[index].id <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`id`,`actorid`,`offset`,`kind`,`color`,`level`,`exp`,`soldiers`,`attack_wash`,`defense_wash`,`troops_wash`) Values('%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,lltoa(pHero[index].id,sz64_id,10 ),pHero[index].actorid,pHero[index].offset,pHero[index].kind,pHero[index].color,pHero[index].level,pHero[index].exp,pHero[index].soldiers,pHero[index].attack_wash,pHero[index].defense_wash,pHero[index].troops_wash);
		}
		else
		{
			sprintf( szSQL, ",('%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",lltoa(pHero[index].id,sz64_id,10 ),pHero[index].actorid,pHero[index].offset,pHero[index].kind,pHero[index].color,pHero[index].level,pHero[index].exp,pHero[index].soldiers,pHero[index].attack_wash,pHero[index].defense_wash,pHero[index].troops_wash);
			strcat( g_batchsql, szSQL );
		}
		count += 1;
		if ( count > 128 )
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
