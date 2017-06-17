#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_city_building_barracks.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int city_building_barracks_load_auto( int cityid, int city_index, LPCB_GETBUILDINGBARRACKS pCB_GetBuildingBarracks, char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	BuildingBarracks *pBuildingBarracks;

	sprintf( szSQL, "select `cityid`,`offset`,`kind`,`level`,`finish`,`soldiers`,`queue`,`trainnum0`,`trainnum1`,`trainnum2`,`trainnum3`,`trainnum4`,`trainnum5`,`trainnum6`,`trainnum7`,`trainfinish0`,`trainfinish1`,`trainfinish2`,`trainfinish3`,`trainfinish4`,`trainfinish5`,`trainfinish6`,`trainfinish7`,`trainlong` from %s where cityid='%d'", pTab, cityid );
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
		pBuildingBarracks = pCB_GetBuildingBarracks( city_index, atoi(row[1]) );
		if( pBuildingBarracks == NULL )
			continue;
		offset++;
		offset++;
		pBuildingBarracks->kind = atoi(row[offset++]);
		pBuildingBarracks->level = atoi(row[offset++]);
		pBuildingBarracks->finish = atoi(row[offset++]);
		pBuildingBarracks->soldiers = atoi(row[offset++]);
		pBuildingBarracks->queue = atoi(row[offset++]);
		pBuildingBarracks->trainnum[0] = atoi(row[offset++]);
		pBuildingBarracks->trainnum[1] = atoi(row[offset++]);
		pBuildingBarracks->trainnum[2] = atoi(row[offset++]);
		pBuildingBarracks->trainnum[3] = atoi(row[offset++]);
		pBuildingBarracks->trainnum[4] = atoi(row[offset++]);
		pBuildingBarracks->trainnum[5] = atoi(row[offset++]);
		pBuildingBarracks->trainnum[6] = atoi(row[offset++]);
		pBuildingBarracks->trainnum[7] = atoi(row[offset++]);
		pBuildingBarracks->trainfinish[0] = atoi(row[offset++]);
		pBuildingBarracks->trainfinish[1] = atoi(row[offset++]);
		pBuildingBarracks->trainfinish[2] = atoi(row[offset++]);
		pBuildingBarracks->trainfinish[3] = atoi(row[offset++]);
		pBuildingBarracks->trainfinish[4] = atoi(row[offset++]);
		pBuildingBarracks->trainfinish[5] = atoi(row[offset++]);
		pBuildingBarracks->trainfinish[6] = atoi(row[offset++]);
		pBuildingBarracks->trainfinish[7] = atoi(row[offset++]);
		pBuildingBarracks->trainlong = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}
int city_building_barracks_save_auto( int cityid, int offset, BuildingBarracks *pBuildingBarracks, char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pBuildingBarracks == NULL )
		return -1;

RE_BUILDINGBARRACKS_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`cityid`,`offset`,`kind`,`level`,`finish`,`soldiers`,`queue`,`trainnum0`,`trainnum1`,`trainnum2`,`trainnum3`,`trainnum4`,`trainnum5`,`trainnum6`,`trainnum7`,`trainfinish0`,`trainfinish1`,`trainfinish2`,`trainfinish3`,`trainfinish4`,`trainfinish5`,`trainfinish6`,`trainfinish7`,`trainlong`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,cityid,offset,pBuildingBarracks->kind,pBuildingBarracks->level,pBuildingBarracks->finish,pBuildingBarracks->soldiers,pBuildingBarracks->queue,pBuildingBarracks->trainnum[0],pBuildingBarracks->trainnum[1],pBuildingBarracks->trainnum[2],pBuildingBarracks->trainnum[3],pBuildingBarracks->trainnum[4],pBuildingBarracks->trainnum[5],pBuildingBarracks->trainnum[6],pBuildingBarracks->trainnum[7],pBuildingBarracks->trainfinish[0],pBuildingBarracks->trainfinish[1],pBuildingBarracks->trainfinish[2],pBuildingBarracks->trainfinish[3],pBuildingBarracks->trainfinish[4],pBuildingBarracks->trainfinish[5],pBuildingBarracks->trainfinish[6],pBuildingBarracks->trainfinish[7],pBuildingBarracks->trainlong);
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
			goto RE_BUILDINGBARRACKS_UPDATE;
		}
		return -1;
	}
	return 0;
}

int city_building_barracks_batch_save_auto( int cityid, BuildingBarracks *pBuildingBarracks, int maxcount, char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pBuildingBarracks == NULL )
		return -1;

	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pBuildingBarracks[index].kind <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`cityid`,`offset`,`kind`,`level`,`finish`,`soldiers`,`queue`,`trainnum0`,`trainnum1`,`trainnum2`,`trainnum3`,`trainnum4`,`trainnum5`,`trainnum6`,`trainnum7`,`trainfinish0`,`trainfinish1`,`trainfinish2`,`trainfinish3`,`trainfinish4`,`trainfinish5`,`trainfinish6`,`trainfinish7`,`trainlong`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,cityid,index,pBuildingBarracks[index].kind,pBuildingBarracks[index].level,pBuildingBarracks[index].finish,pBuildingBarracks[index].soldiers,pBuildingBarracks[index].queue,pBuildingBarracks[index].trainnum[0],pBuildingBarracks[index].trainnum[1],pBuildingBarracks[index].trainnum[2],pBuildingBarracks[index].trainnum[3],pBuildingBarracks[index].trainnum[4],pBuildingBarracks[index].trainnum[5],pBuildingBarracks[index].trainnum[6],pBuildingBarracks[index].trainnum[7],pBuildingBarracks[index].trainfinish[0],pBuildingBarracks[index].trainfinish[1],pBuildingBarracks[index].trainfinish[2],pBuildingBarracks[index].trainfinish[3],pBuildingBarracks[index].trainfinish[4],pBuildingBarracks[index].trainfinish[5],pBuildingBarracks[index].trainfinish[6],pBuildingBarracks[index].trainfinish[7],pBuildingBarracks[index].trainlong);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",cityid,index,pBuildingBarracks[index].kind,pBuildingBarracks[index].level,pBuildingBarracks[index].finish,pBuildingBarracks[index].soldiers,pBuildingBarracks[index].queue,pBuildingBarracks[index].trainnum[0],pBuildingBarracks[index].trainnum[1],pBuildingBarracks[index].trainnum[2],pBuildingBarracks[index].trainnum[3],pBuildingBarracks[index].trainnum[4],pBuildingBarracks[index].trainnum[5],pBuildingBarracks[index].trainnum[6],pBuildingBarracks[index].trainnum[7],pBuildingBarracks[index].trainfinish[0],pBuildingBarracks[index].trainfinish[1],pBuildingBarracks[index].trainfinish[2],pBuildingBarracks[index].trainfinish[3],pBuildingBarracks[index].trainfinish[4],pBuildingBarracks[index].trainfinish[5],pBuildingBarracks[index].trainfinish[6],pBuildingBarracks[index].trainfinish[7],pBuildingBarracks[index].trainlong);
			strcat( g_batchsql, szSQL );
		}
		count += 1;
		if ( count > 64 )
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
