#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_map_town.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int g_map_town_maxindex = 0;

int map_town_load_auto( LPCB_GETMAPTOWN pCB_GetMapTown, LPCB_LOADMAPTOWN pCB_LoadMapTown, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	MapTown *pMapTown;
	int townid = 0;

	sprintf( szSQL, "select `townid`,`nation`,`name`,`protect_sec`,`produce_num`,`produce_sec`,`own_actorid`,`own_sec`,`join_actorid`,`ask_actorid`,`soldier`,`soldier_update`,`dev_level`,`dev_exp`,`attackcd` from %s ", pTab );
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
		pMapTown = pCB_GetMapTown( atoi(row[0]) );
		if( pMapTown == NULL )
			continue;
		pMapTown->townid = atoi(row[offset++]);
		pMapTown->nation = atoi(row[offset++]);
		memcpy( pMapTown->name, row[offset++], 22 ); pMapTown->name[21]=0;
		pMapTown->protect_sec = atoi(row[offset++]);
		pMapTown->produce_num = atoi(row[offset++]);
		pMapTown->produce_sec = atoi(row[offset++]);
		pMapTown->own_actorid = atoi(row[offset++]);
		pMapTown->own_sec = atoi(row[offset++]);
		memcpy( pMapTown->join_actorid, row[offset++], sizeof(int)*128 );
		memcpy( pMapTown->ask_actorid, row[offset++], sizeof(int)*128 );
		memcpy( pMapTown->soldier, row[offset++], sizeof(int)*16 );
		pMapTown->soldier_update = atoi(row[offset++]);
		pMapTown->dev_level = atoi(row[offset++]);
		pMapTown->dev_exp = atoi(row[offset++]);
		pMapTown->attackcd = atoi(row[offset++]);
		if( pCB_LoadMapTown )
			pCB_LoadMapTown( pMapTown->townid );
		townid = pMapTown->townid;
		if ( townid >= g_map_town_maxindex )
		{
			g_map_town_maxindex = townid + 1;
		}
	}
	mysql_free_result( res );
	return 0;
}
int map_town_save_auto( MapTown *pMapTown, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pMapTown == NULL )
		return -1;

	char szText_name[MAX_PATH]={0};
	char szText_join_actorid[sizeof(int)*128*2+1]={0};
	char szText_ask_actorid[sizeof(int)*128*2+1]={0};
	char szText_soldier[sizeof(int)*16*2+1]={0};
RE_MAPTOWN_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`townid`,`nation`,`name`,`protect_sec`,`produce_num`,`produce_sec`,`own_actorid`,`own_sec`,`join_actorid`,`ask_actorid`,`soldier`,`soldier_update`,`dev_level`,`dev_exp`,`attackcd`) Values('%d','%d','%s','%d','%d','%d','%d','%d','%s','%s','%s','%d','%d','%d','%d')",pTab,pMapTown->townid,pMapTown->nation,db_escape((const char *)pMapTown->name,szText_name,0),pMapTown->protect_sec,pMapTown->produce_num,pMapTown->produce_sec,pMapTown->own_actorid,pMapTown->own_sec,db_escape((const char *)pMapTown->join_actorid,szText_join_actorid,sizeof(int)*128),db_escape((const char *)pMapTown->ask_actorid,szText_ask_actorid,sizeof(int)*128),db_escape((const char *)pMapTown->soldier,szText_soldier,sizeof(int)*16),pMapTown->soldier_update,pMapTown->dev_level,pMapTown->dev_exp,pMapTown->attackcd);
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
			goto RE_MAPTOWN_UPDATE;
		}
		return -1;
	}
	return 0;
}

int map_town_batch_save_auto( MapTown *pMapTown, int maxcount, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pMapTown == NULL )
		return -1;

	char reconnect_flag = 0;
RE_MAPTOWN_TRUNCATE:
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
				goto RE_MAPTOWN_TRUNCATE;
			}
		}
	}

	char szText_name[MAX_PATH]={0};
	char szText_join_actorid[sizeof(int)*128*2+1]={0};
	char szText_ask_actorid[sizeof(int)*128*2+1]={0};
	char szText_soldier[sizeof(int)*16*2+1]={0};
	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pMapTown[index].townid <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`townid`,`nation`,`name`,`protect_sec`,`produce_num`,`produce_sec`,`own_actorid`,`own_sec`,`join_actorid`,`ask_actorid`,`soldier`,`soldier_update`,`dev_level`,`dev_exp`,`attackcd`) Values('%d','%d','%s','%d','%d','%d','%d','%d','%s','%s','%s','%d','%d','%d','%d')",pTab,pMapTown[index].townid,pMapTown[index].nation,db_escape((const char *)pMapTown[index].name,szText_name,0),pMapTown[index].protect_sec,pMapTown[index].produce_num,pMapTown[index].produce_sec,pMapTown[index].own_actorid,pMapTown[index].own_sec,db_escape((const char *)pMapTown[index].join_actorid,szText_join_actorid,sizeof(int)*128),db_escape((const char *)pMapTown[index].ask_actorid,szText_ask_actorid,sizeof(int)*128),db_escape((const char *)pMapTown[index].soldier,szText_soldier,sizeof(int)*16),pMapTown[index].soldier_update,pMapTown[index].dev_level,pMapTown[index].dev_exp,pMapTown[index].attackcd);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%s','%d','%d','%d','%d','%d','%s','%s','%s','%d','%d','%d','%d')",pMapTown[index].townid,pMapTown[index].nation,db_escape((const char *)pMapTown[index].name,szText_name,0),pMapTown[index].protect_sec,pMapTown[index].produce_num,pMapTown[index].produce_sec,pMapTown[index].own_actorid,pMapTown[index].own_sec,db_escape((const char *)pMapTown[index].join_actorid,szText_join_actorid,sizeof(int)*128),db_escape((const char *)pMapTown[index].ask_actorid,szText_ask_actorid,sizeof(int)*128),db_escape((const char *)pMapTown[index].soldier,szText_soldier,sizeof(int)*16),pMapTown[index].soldier_update,pMapTown[index].dev_level,pMapTown[index].dev_exp,pMapTown[index].attackcd);
			strcat( g_batchsql, szSQL );
		}
		count += 1;
		if ( count > 8 )
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
