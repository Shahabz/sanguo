#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_actor_friend.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int actor_friend_load_auto( int actorid, int actor_index, LPCB_GETACTORFRIEND pCB_GetActorFriend, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	ActorFriend *pActorFriend;

	sprintf( szSQL, "select `actorid`,`offset`,`friend_actorid`,`jointime` from %s where actorid='%d'", pTab, actorid );
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
		pActorFriend = pCB_GetActorFriend( actor_index, atoi(row[0]) );
		if( pActorFriend == NULL )
			continue;
		pActorFriend->actorid = atoi(row[offset++]);
		pActorFriend->offset = atoi(row[offset++]);
		pActorFriend->friend_actorid = atoi(row[offset++]);
		pActorFriend->jointime = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}
int actor_friend_save_auto( ActorFriend *pActorFriend, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pActorFriend == NULL )
		return -1;

RE_ACTORFRIEND_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`actorid`,`offset`,`friend_actorid`,`jointime`) Values('%d','%d','%d','%d')",pTab,pActorFriend->actorid,pActorFriend->offset,pActorFriend->friend_actorid,pActorFriend->jointime);
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
			goto RE_ACTORFRIEND_UPDATE;
		}
		return -1;
	}
	return 0;
}

int actor_friend_batch_save_auto( ActorFriend *pActorFriend, int maxcount, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pActorFriend == NULL )
		return -1;

	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pActorFriend[index].actorid <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`actorid`,`offset`,`friend_actorid`,`jointime`) Values('%d','%d','%d','%d')",pTab,pActorFriend[index].actorid,pActorFriend[index].offset,pActorFriend[index].friend_actorid,pActorFriend[index].jointime);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d','%d')",pActorFriend[index].actorid,pActorFriend[index].offset,pActorFriend[index].friend_actorid,pActorFriend[index].jointime);
			strcat( g_batchsql, szSQL );
		}
		count += 1;
		if ( count > 100 )
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
