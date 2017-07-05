#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_city_guard.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int city_guard_load_auto( int actorid, int city_index, LPCB_GETCITYGUARD pCB_GetCityGuard, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	CityGuard *pCityGuard;

	sprintf( szSQL, "select `actorid`,`offset`,`shape`,`level`,`color`,`corps`,`soldiers`,`soldiers_max` from %s where actorid='%d'", pTab, actorid );
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
		pCityGuard = pCB_GetCityGuard( city_index, atoi(row[1]) );
		if( pCityGuard == NULL )
			continue;
		offset++;
		offset++;
		pCityGuard->shape = atoi(row[offset++]);
		pCityGuard->level = atoi(row[offset++]);
		pCityGuard->color = atoi(row[offset++]);
		pCityGuard->corps = atoi(row[offset++]);
		pCityGuard->soldiers = atoi(row[offset++]);
		pCityGuard->soldiers_max = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}
int city_guard_save_auto( int actorid, int offset, CityGuard *pCityGuard, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pCityGuard == NULL )
		return -1;

RE_CITYGUARD_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`actorid`,`offset`,`shape`,`level`,`color`,`corps`,`soldiers`,`soldiers_max`) Values('%d','%d','%d','%d','%d','%d','%d','%d')",pTab,actorid,offset,pCityGuard->shape,pCityGuard->level,pCityGuard->color,pCityGuard->corps,pCityGuard->soldiers,pCityGuard->soldiers_max);
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
			goto RE_CITYGUARD_UPDATE;
		}
		return -1;
	}
	return 0;
}

int city_guard_batch_save_auto( int actorid, CityGuard *pCityGuard, int maxcount, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pCityGuard == NULL )
		return -1;

	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`actorid`,`offset`,`shape`,`level`,`color`,`corps`,`soldiers`,`soldiers_max`) Values('%d','%d','%d','%d','%d','%d','%d','%d')",pTab,actorid,index,pCityGuard[index].shape,pCityGuard[index].level,pCityGuard[index].color,pCityGuard[index].corps,pCityGuard[index].soldiers,pCityGuard[index].soldiers_max);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d','%d','%d','%d','%d','%d')",actorid,index,pCityGuard[index].shape,pCityGuard[index].level,pCityGuard[index].color,pCityGuard[index].corps,pCityGuard[index].soldiers,pCityGuard[index].soldiers_max);
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
