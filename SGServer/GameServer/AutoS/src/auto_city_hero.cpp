#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_city_hero.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int city_hero_load_auto( int cityid, int city_index, LPCB_GETCITYHERO pCB_GetCityHero, char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	CityHero *pCityHero;

	sprintf( szSQL, "select `cityid`,`offset`,`kind`,`level`,`exp`,`soldiers`,`attack`,`defense`,`troops`,`growth` from %s where cityid='%d'", pTab, cityid );
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
		pCityHero = pCB_GetCityHero( city_index, atoi(row[1]) );
		if( pCityHero == NULL )
			continue;
		offset++;
		offset++;
		pCityHero->kind = atoi(row[offset++]);
		pCityHero->level = atoi(row[offset++]);
		pCityHero->exp = atoi(row[offset++]);
		pCityHero->soldiers = atoi(row[offset++]);
		pCityHero->attack = atoi(row[offset++]);
		pCityHero->defense = atoi(row[offset++]);
		pCityHero->troops = atoi(row[offset++]);
		pCityHero->growth = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}
int city_hero_save_auto( int cityid, int offset, CityHero *pCityHero, char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pCityHero == NULL )
		return -1;

RE_CITYHERO_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`cityid`,`offset`,`kind`,`level`,`exp`,`soldiers`,`attack`,`defense`,`troops`,`growth`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,cityid,offset,pCityHero->kind,pCityHero->level,pCityHero->exp,pCityHero->soldiers,pCityHero->attack,pCityHero->defense,pCityHero->troops,pCityHero->growth);
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
			goto RE_CITYHERO_UPDATE;
		}
		return -1;
	}
	return 0;
}

int city_hero_batch_save_auto( int cityid, CityHero *pCityHero, int maxcount, char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pCityHero == NULL )
		return -1;

	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pCityHero[index].kind <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`cityid`,`offset`,`kind`,`level`,`exp`,`soldiers`,`attack`,`defense`,`troops`,`growth`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,cityid,index,pCityHero[index].kind,pCityHero[index].level,pCityHero[index].exp,pCityHero[index].soldiers,pCityHero[index].attack,pCityHero[index].defense,pCityHero[index].troops,pCityHero[index].growth);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",cityid,index,pCityHero[index].kind,pCityHero[index].level,pCityHero[index].exp,pCityHero[index].soldiers,pCityHero[index].attack,pCityHero[index].defense,pCityHero[index].troops,pCityHero[index].growth);
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
