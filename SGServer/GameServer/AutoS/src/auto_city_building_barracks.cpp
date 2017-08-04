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

int city_building_barracks_load_auto( int actorid, int city_index, LPCB_GETBUILDINGBARRACKS pCB_GetBuildingBarracks, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	BuildingBarracks *pBuildingBarracks;

	sprintf( szSQL, "select `actorid`,`offset`,`kind`,`level`,`soldiers`,`trainnum`,`trainsec`,`trainsec_need`,`queuenum0`,`queuenum1`,`queuenum2`,`queuenum3`,`queuenum4`,`queuenum5`,`queuenum6`,`queuenum7`,`queuenum8`,`queuenum9`,`queuenum10`,`queuesec0`,`queuesec1`,`queuesec2`,`queuesec3`,`queuesec4`,`queuesec5`,`queuesec6`,`queuesec7`,`queuesec8`,`queuesec9`,`queuesec10`,`queue`,`trainlong`,`overnum`,`quicksec` from %s where actorid='%d'", pTab, actorid );
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
		pBuildingBarracks->soldiers = atoi(row[offset++]);
		pBuildingBarracks->trainnum = atoi(row[offset++]);
		pBuildingBarracks->trainsec = atoi(row[offset++]);
		pBuildingBarracks->trainsec_need = atoi(row[offset++]);
		pBuildingBarracks->queuenum[0] = atoi(row[offset++]);
		pBuildingBarracks->queuenum[1] = atoi(row[offset++]);
		pBuildingBarracks->queuenum[2] = atoi(row[offset++]);
		pBuildingBarracks->queuenum[3] = atoi(row[offset++]);
		pBuildingBarracks->queuenum[4] = atoi(row[offset++]);
		pBuildingBarracks->queuenum[5] = atoi(row[offset++]);
		pBuildingBarracks->queuenum[6] = atoi(row[offset++]);
		pBuildingBarracks->queuenum[7] = atoi(row[offset++]);
		pBuildingBarracks->queuenum[8] = atoi(row[offset++]);
		pBuildingBarracks->queuenum[9] = atoi(row[offset++]);
		pBuildingBarracks->queuenum[10] = atoi(row[offset++]);
		pBuildingBarracks->queuesec[0] = atoi(row[offset++]);
		pBuildingBarracks->queuesec[1] = atoi(row[offset++]);
		pBuildingBarracks->queuesec[2] = atoi(row[offset++]);
		pBuildingBarracks->queuesec[3] = atoi(row[offset++]);
		pBuildingBarracks->queuesec[4] = atoi(row[offset++]);
		pBuildingBarracks->queuesec[5] = atoi(row[offset++]);
		pBuildingBarracks->queuesec[6] = atoi(row[offset++]);
		pBuildingBarracks->queuesec[7] = atoi(row[offset++]);
		pBuildingBarracks->queuesec[8] = atoi(row[offset++]);
		pBuildingBarracks->queuesec[9] = atoi(row[offset++]);
		pBuildingBarracks->queuesec[10] = atoi(row[offset++]);
		pBuildingBarracks->queue = atoi(row[offset++]);
		pBuildingBarracks->trainlong = atoi(row[offset++]);
		pBuildingBarracks->overnum = atoi(row[offset++]);
		pBuildingBarracks->quicksec = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}
int city_building_barracks_save_auto( int actorid, int offset, BuildingBarracks *pBuildingBarracks, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pBuildingBarracks == NULL )
		return -1;

RE_BUILDINGBARRACKS_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`actorid`,`offset`,`kind`,`level`,`soldiers`,`trainnum`,`trainsec`,`trainsec_need`,`queuenum0`,`queuenum1`,`queuenum2`,`queuenum3`,`queuenum4`,`queuenum5`,`queuenum6`,`queuenum7`,`queuenum8`,`queuenum9`,`queuenum10`,`queuesec0`,`queuesec1`,`queuesec2`,`queuesec3`,`queuesec4`,`queuesec5`,`queuesec6`,`queuesec7`,`queuesec8`,`queuesec9`,`queuesec10`,`queue`,`trainlong`,`overnum`,`quicksec`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,actorid,offset,pBuildingBarracks->kind,pBuildingBarracks->level,pBuildingBarracks->soldiers,pBuildingBarracks->trainnum,pBuildingBarracks->trainsec,pBuildingBarracks->trainsec_need,pBuildingBarracks->queuenum[0],pBuildingBarracks->queuenum[1],pBuildingBarracks->queuenum[2],pBuildingBarracks->queuenum[3],pBuildingBarracks->queuenum[4],pBuildingBarracks->queuenum[5],pBuildingBarracks->queuenum[6],pBuildingBarracks->queuenum[7],pBuildingBarracks->queuenum[8],pBuildingBarracks->queuenum[9],pBuildingBarracks->queuenum[10],pBuildingBarracks->queuesec[0],pBuildingBarracks->queuesec[1],pBuildingBarracks->queuesec[2],pBuildingBarracks->queuesec[3],pBuildingBarracks->queuesec[4],pBuildingBarracks->queuesec[5],pBuildingBarracks->queuesec[6],pBuildingBarracks->queuesec[7],pBuildingBarracks->queuesec[8],pBuildingBarracks->queuesec[9],pBuildingBarracks->queuesec[10],pBuildingBarracks->queue,pBuildingBarracks->trainlong,pBuildingBarracks->overnum,pBuildingBarracks->quicksec);
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

int city_building_barracks_batch_save_auto( int actorid, BuildingBarracks *pBuildingBarracks, int maxcount, const char *pTab, FILE *fp )
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
			sprintf( g_batchsql, "REPLACE INTO %s (`actorid`,`offset`,`kind`,`level`,`soldiers`,`trainnum`,`trainsec`,`trainsec_need`,`queuenum0`,`queuenum1`,`queuenum2`,`queuenum3`,`queuenum4`,`queuenum5`,`queuenum6`,`queuenum7`,`queuenum8`,`queuenum9`,`queuenum10`,`queuesec0`,`queuesec1`,`queuesec2`,`queuesec3`,`queuesec4`,`queuesec5`,`queuesec6`,`queuesec7`,`queuesec8`,`queuesec9`,`queuesec10`,`queue`,`trainlong`,`overnum`,`quicksec`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,actorid,index,pBuildingBarracks[index].kind,pBuildingBarracks[index].level,pBuildingBarracks[index].soldiers,pBuildingBarracks[index].trainnum,pBuildingBarracks[index].trainsec,pBuildingBarracks[index].trainsec_need,pBuildingBarracks[index].queuenum[0],pBuildingBarracks[index].queuenum[1],pBuildingBarracks[index].queuenum[2],pBuildingBarracks[index].queuenum[3],pBuildingBarracks[index].queuenum[4],pBuildingBarracks[index].queuenum[5],pBuildingBarracks[index].queuenum[6],pBuildingBarracks[index].queuenum[7],pBuildingBarracks[index].queuenum[8],pBuildingBarracks[index].queuenum[9],pBuildingBarracks[index].queuenum[10],pBuildingBarracks[index].queuesec[0],pBuildingBarracks[index].queuesec[1],pBuildingBarracks[index].queuesec[2],pBuildingBarracks[index].queuesec[3],pBuildingBarracks[index].queuesec[4],pBuildingBarracks[index].queuesec[5],pBuildingBarracks[index].queuesec[6],pBuildingBarracks[index].queuesec[7],pBuildingBarracks[index].queuesec[8],pBuildingBarracks[index].queuesec[9],pBuildingBarracks[index].queuesec[10],pBuildingBarracks[index].queue,pBuildingBarracks[index].trainlong,pBuildingBarracks[index].overnum,pBuildingBarracks[index].quicksec);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",actorid,index,pBuildingBarracks[index].kind,pBuildingBarracks[index].level,pBuildingBarracks[index].soldiers,pBuildingBarracks[index].trainnum,pBuildingBarracks[index].trainsec,pBuildingBarracks[index].trainsec_need,pBuildingBarracks[index].queuenum[0],pBuildingBarracks[index].queuenum[1],pBuildingBarracks[index].queuenum[2],pBuildingBarracks[index].queuenum[3],pBuildingBarracks[index].queuenum[4],pBuildingBarracks[index].queuenum[5],pBuildingBarracks[index].queuenum[6],pBuildingBarracks[index].queuenum[7],pBuildingBarracks[index].queuenum[8],pBuildingBarracks[index].queuenum[9],pBuildingBarracks[index].queuenum[10],pBuildingBarracks[index].queuesec[0],pBuildingBarracks[index].queuesec[1],pBuildingBarracks[index].queuesec[2],pBuildingBarracks[index].queuesec[3],pBuildingBarracks[index].queuesec[4],pBuildingBarracks[index].queuesec[5],pBuildingBarracks[index].queuesec[6],pBuildingBarracks[index].queuesec[7],pBuildingBarracks[index].queuesec[8],pBuildingBarracks[index].queuesec[9],pBuildingBarracks[index].queuesec[10],pBuildingBarracks[index].queue,pBuildingBarracks[index].trainlong,pBuildingBarracks[index].overnum,pBuildingBarracks[index].quicksec);
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
