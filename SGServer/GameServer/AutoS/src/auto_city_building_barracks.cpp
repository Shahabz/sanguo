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

int city_building_barracks_load_auto( int actorid, int city_index, LPCB_GETBUILDINGBARRACKS pCB_GetBuildingBarracks, char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	BuildingBarracks *pBuildingBarracks;

	sprintf( szSQL, "select `actorid`,`offset`,`kind`,`level`,`soldiers`,`queue`,`trainnum0`,`trainnum1`,`trainnum2`,`trainnum3`,`trainnum4`,`trainnum5`,`trainnum6`,`trainnum7`,`trainsec0`,`trainsec1`,`trainsec2`,`trainsec3`,`trainsec4`,`trainsec5`,`trainsec6`,`trainsec7`,`trainlong`,`quicksec`,`value` from %s where actorid='%d'", pTab, actorid );
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
		pBuildingBarracks->queue = atoi(row[offset++]);
		pBuildingBarracks->trainnum[0] = atoi(row[offset++]);
		pBuildingBarracks->trainnum[1] = atoi(row[offset++]);
		pBuildingBarracks->trainnum[2] = atoi(row[offset++]);
		pBuildingBarracks->trainnum[3] = atoi(row[offset++]);
		pBuildingBarracks->trainnum[4] = atoi(row[offset++]);
		pBuildingBarracks->trainnum[5] = atoi(row[offset++]);
		pBuildingBarracks->trainnum[6] = atoi(row[offset++]);
		pBuildingBarracks->trainnum[7] = atoi(row[offset++]);
		pBuildingBarracks->trainsec[0] = atoi(row[offset++]);
		pBuildingBarracks->trainsec[1] = atoi(row[offset++]);
		pBuildingBarracks->trainsec[2] = atoi(row[offset++]);
		pBuildingBarracks->trainsec[3] = atoi(row[offset++]);
		pBuildingBarracks->trainsec[4] = atoi(row[offset++]);
		pBuildingBarracks->trainsec[5] = atoi(row[offset++]);
		pBuildingBarracks->trainsec[6] = atoi(row[offset++]);
		pBuildingBarracks->trainsec[7] = atoi(row[offset++]);
		pBuildingBarracks->trainlong = atoi(row[offset++]);
		pBuildingBarracks->quicksec = atoi(row[offset++]);
		pBuildingBarracks->value = atoi(row[offset++]);
	}
	mysql_free_result( res );
	return 0;
}
int city_building_barracks_save_auto( int actorid, int offset, BuildingBarracks *pBuildingBarracks, char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pBuildingBarracks == NULL )
		return -1;

RE_BUILDINGBARRACKS_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`actorid`,`offset`,`kind`,`level`,`soldiers`,`queue`,`trainnum0`,`trainnum1`,`trainnum2`,`trainnum3`,`trainnum4`,`trainnum5`,`trainnum6`,`trainnum7`,`trainsec0`,`trainsec1`,`trainsec2`,`trainsec3`,`trainsec4`,`trainsec5`,`trainsec6`,`trainsec7`,`trainlong`,`quicksec`,`value`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,actorid,offset,pBuildingBarracks->kind,pBuildingBarracks->level,pBuildingBarracks->soldiers,pBuildingBarracks->queue,pBuildingBarracks->trainnum[0],pBuildingBarracks->trainnum[1],pBuildingBarracks->trainnum[2],pBuildingBarracks->trainnum[3],pBuildingBarracks->trainnum[4],pBuildingBarracks->trainnum[5],pBuildingBarracks->trainnum[6],pBuildingBarracks->trainnum[7],pBuildingBarracks->trainsec[0],pBuildingBarracks->trainsec[1],pBuildingBarracks->trainsec[2],pBuildingBarracks->trainsec[3],pBuildingBarracks->trainsec[4],pBuildingBarracks->trainsec[5],pBuildingBarracks->trainsec[6],pBuildingBarracks->trainsec[7],pBuildingBarracks->trainlong,pBuildingBarracks->quicksec,pBuildingBarracks->value);
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

int city_building_barracks_batch_save_auto( int actorid, BuildingBarracks *pBuildingBarracks, int maxcount, char *pTab, FILE *fp )
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
			sprintf( g_batchsql, "REPLACE INTO %s (`actorid`,`offset`,`kind`,`level`,`soldiers`,`queue`,`trainnum0`,`trainnum1`,`trainnum2`,`trainnum3`,`trainnum4`,`trainnum5`,`trainnum6`,`trainnum7`,`trainsec0`,`trainsec1`,`trainsec2`,`trainsec3`,`trainsec4`,`trainsec5`,`trainsec6`,`trainsec7`,`trainlong`,`quicksec`,`value`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,actorid,index,pBuildingBarracks[index].kind,pBuildingBarracks[index].level,pBuildingBarracks[index].soldiers,pBuildingBarracks[index].queue,pBuildingBarracks[index].trainnum[0],pBuildingBarracks[index].trainnum[1],pBuildingBarracks[index].trainnum[2],pBuildingBarracks[index].trainnum[3],pBuildingBarracks[index].trainnum[4],pBuildingBarracks[index].trainnum[5],pBuildingBarracks[index].trainnum[6],pBuildingBarracks[index].trainnum[7],pBuildingBarracks[index].trainsec[0],pBuildingBarracks[index].trainsec[1],pBuildingBarracks[index].trainsec[2],pBuildingBarracks[index].trainsec[3],pBuildingBarracks[index].trainsec[4],pBuildingBarracks[index].trainsec[5],pBuildingBarracks[index].trainsec[6],pBuildingBarracks[index].trainsec[7],pBuildingBarracks[index].trainlong,pBuildingBarracks[index].quicksec,pBuildingBarracks[index].value);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",actorid,index,pBuildingBarracks[index].kind,pBuildingBarracks[index].level,pBuildingBarracks[index].soldiers,pBuildingBarracks[index].queue,pBuildingBarracks[index].trainnum[0],pBuildingBarracks[index].trainnum[1],pBuildingBarracks[index].trainnum[2],pBuildingBarracks[index].trainnum[3],pBuildingBarracks[index].trainnum[4],pBuildingBarracks[index].trainnum[5],pBuildingBarracks[index].trainnum[6],pBuildingBarracks[index].trainnum[7],pBuildingBarracks[index].trainsec[0],pBuildingBarracks[index].trainsec[1],pBuildingBarracks[index].trainsec[2],pBuildingBarracks[index].trainsec[3],pBuildingBarracks[index].trainsec[4],pBuildingBarracks[index].trainsec[5],pBuildingBarracks[index].trainsec[6],pBuildingBarracks[index].trainsec[7],pBuildingBarracks[index].trainlong,pBuildingBarracks[index].quicksec,pBuildingBarracks[index].value);
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
