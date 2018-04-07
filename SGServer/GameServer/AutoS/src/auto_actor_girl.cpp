#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_actor_girl.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int actor_girl_load_auto( int actorid, int city_index, LPCB_GETGIRL pCB_GetGirl, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	Girl *pGirl;

	sprintf( szSQL, "select `actorid`,`kind`,`love_level`,`love_num`,`color`,`soul`,`love_exp`,`love_fday` from %s where actorid='%d'", pTab, actorid );
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
		pGirl = pCB_GetGirl( city_index, atoi(row[1]) );
		if( pGirl == NULL )
			continue;
		pGirl->actorid = atoi(row[offset++]);
		pGirl->kind = atoi(row[offset++]);
		pGirl->love_level = atoi(row[offset++]);
		pGirl->love_num = atoi(row[offset++]);
		pGirl->color = atoi(row[offset++]);
		pGirl->soul = atoi(row[offset++]);
		pGirl->love_exp = atoi(row[offset++]);
		pGirl->love_fday = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}
int actor_girl_save_auto( int actorid, int offset, Girl *pGirl, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pGirl == NULL )
		return -1;

RE_GIRL_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`actorid`,`kind`,`love_level`,`love_num`,`color`,`soul`,`love_exp`,`love_fday`) Values('%d','%d','%d','%d','%d','%d','%d','%d')",pTab,pGirl->actorid,pGirl->kind,pGirl->love_level,pGirl->love_num,pGirl->color,pGirl->soul,pGirl->love_exp,pGirl->love_fday);
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
			goto RE_GIRL_UPDATE;
		}
		return -1;
	}
	return 0;
}

int actor_girl_batch_save_auto( int actorid, Girl *pGirl, int maxcount, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pGirl == NULL )
		return -1;

	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pGirl[index].kind <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`actorid`,`kind`,`love_level`,`love_num`,`color`,`soul`,`love_exp`,`love_fday`) Values('%d','%d','%d','%d','%d','%d','%d','%d')",pTab,pGirl[index].actorid,pGirl[index].kind,pGirl[index].love_level,pGirl[index].love_num,pGirl[index].color,pGirl[index].soul,pGirl[index].love_exp,pGirl[index].love_fday);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d','%d','%d','%d','%d','%d')",pGirl[index].actorid,pGirl[index].kind,pGirl[index].love_level,pGirl[index].love_num,pGirl[index].color,pGirl[index].soul,pGirl[index].love_exp,pGirl[index].love_fday);
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
