#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_nation_hero.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int g_nation_hero_maxindex = 0;

int nation_hero_load_auto( LPCB_GETNATIONHERO pCB_GetNationHero, LPCB_LOADNATIONHERO pCB_LoadNationHero, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	NationHero *pNationHero;
	int kind = 0;

	sprintf( szSQL, "select `kind`,`actorid`,`posx`,`posy`,`runsec` from %s ", pTab );
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
		pNationHero = pCB_GetNationHero( atoi(row[0]) );
		if( pNationHero == NULL )
			continue;
		pNationHero->kind = atoi(row[offset++]);
		pNationHero->actorid = atoi(row[offset++]);
		pNationHero->posx = atoi(row[offset++]);
		pNationHero->posy = atoi(row[offset++]);
		pNationHero->runsec = atoi(row[offset++]);
		if( pCB_LoadNationHero )
			pCB_LoadNationHero( pNationHero->kind );
		kind = pNationHero->kind;
		if ( kind >= g_nation_hero_maxindex )
		{
			g_nation_hero_maxindex = kind + 1;
		}
	}
	mysql_free_result( res );
	return 0;
}
int nation_hero_save_auto( NationHero *pNationHero, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pNationHero == NULL )
		return -1;

RE_NATIONHERO_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`kind`,`actorid`,`posx`,`posy`,`runsec`) Values('%d','%d','%d','%d','%d')",pTab,pNationHero->kind,pNationHero->actorid,pNationHero->posx,pNationHero->posy,pNationHero->runsec);
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
			goto RE_NATIONHERO_UPDATE;
		}
		return -1;
	}
	return 0;
}

int nation_hero_batch_save_auto( NationHero *pNationHero, int maxcount, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pNationHero == NULL )
		return -1;

	char reconnect_flag = 0;
RE_NATIONHERO_TRUNCATE:
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
				goto RE_NATIONHERO_TRUNCATE;
			}
		}
	}

	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pNationHero[index].kind <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`kind`,`actorid`,`posx`,`posy`,`runsec`) Values('%d','%d','%d','%d','%d')",pTab,pNationHero[index].kind,pNationHero[index].actorid,pNationHero[index].posx,pNationHero[index].posy,pNationHero[index].runsec);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d','%d','%d')",pNationHero[index].kind,pNationHero[index].actorid,pNationHero[index].posx,pNationHero[index].posy,pNationHero[index].runsec);
			strcat( g_batchsql, szSQL );
		}
		count += 1;
		if ( count > 15 )
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
