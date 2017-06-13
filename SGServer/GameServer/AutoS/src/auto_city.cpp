#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_city.h"
extern MYSQL *myGame;
extern char g_batchsql[BATCHSQL_MAXSIZE];

int g_city_maxindex = 0;

int city_load_auto( LPCB_GETCITY pCB_GetCity, LPCB_LOADCITY pCB_LoadCity, char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	City *pCity;
	int actorid = 0;

	sprintf( szSQL, "select `cityid`,`actorid`,`name`,`type`,`shape`,`headid`,`country`,`ipcountry`,`language`,`os`,`platid`,`createtime`,`lastlogin`,`lastlogout`,`state`,`posx`,`posy`,`sflag`,`level`,`vipexp`,`politics`,`politics_exp`,`farm`,`farm_exp`,`business`,`business_exp`,`military`,`military_exp`,`people`,`food`,`money`,`soldiers` from %s ", pTab );
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
		pCity = pCB_GetCity( actorid );
		if( pCity == NULL )
			continue;
		pCity->cityid = atoi(row[offset++]);
		pCity->actorid = atoi(row[offset++]);
		memcpy( pCity->name, row[offset++], 22 ); pCity->name[21]=0;
		pCity->type = atoi(row[offset++]);
		pCity->shape = atoi(row[offset++]);
		pCity->headid = atoi(row[offset++]);
		pCity->country = atoi(row[offset++]);
		memcpy( pCity->ipcountry, row[offset++], 3 ); pCity->ipcountry[2]=0;
		pCity->language = atoi(row[offset++]);
		pCity->os = atoi(row[offset++]);
		pCity->platid = atoi(row[offset++]);
		pCity->createtime = atoi(row[offset++]);
		pCity->lastlogin = atoi(row[offset++]);
		pCity->lastlogout = atoi(row[offset++]);
		pCity->state = atoi(row[offset++]);
		pCity->posx = atoi(row[offset++]);
		pCity->posy = atoi(row[offset++]);
		pCity->sflag = atoi(row[offset++]);
		pCity->level = atoi(row[offset++]);
		pCity->vipexp = atoi(row[offset++]);
		pCity->politics = atoi(row[offset++]);
		pCity->politics_exp = atoi(row[offset++]);
		pCity->farm = atoi(row[offset++]);
		pCity->farm_exp = atoi(row[offset++]);
		pCity->business = atoi(row[offset++]);
		pCity->business_exp = atoi(row[offset++]);
		pCity->military = atoi(row[offset++]);
		pCity->military_exp = atoi(row[offset++]);
		pCity->people = atoi(row[offset++]);
		pCity->food = atoi(row[offset++]);
		pCity->money = atoi(row[offset++]);
		pCity->soldiers = atoi(row[offset++]);
		if( pCB_LoadCity )
			pCB_LoadCity( pCity->actorid );
		actorid += 1;
	}
	g_city_maxindex = actorid;
	mysql_free_result( res );
	return 0;
}
int city_save_auto( City *pCity, char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pCity == NULL )
		return -1;

	char szText_name[MAX_PATH]={0};
	char szText_ipcountry[MAX_PATH]={0};
RE_CITY_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`cityid`,`actorid`,`name`,`type`,`shape`,`headid`,`country`,`ipcountry`,`language`,`os`,`platid`,`createtime`,`lastlogin`,`lastlogout`,`state`,`posx`,`posy`,`sflag`,`level`,`vipexp`,`politics`,`politics_exp`,`farm`,`farm_exp`,`business`,`business_exp`,`military`,`military_exp`,`people`,`food`,`money`,`soldiers`) Values('%d','%d','%s','%d','%d','%d','%d','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,pCity->cityid,pCity->actorid,db_escape((const char *)pCity->name,szText_name,0),pCity->type,pCity->shape,pCity->headid,pCity->country,db_escape((const char *)pCity->ipcountry,szText_ipcountry,0),pCity->language,pCity->os,pCity->platid,pCity->createtime,pCity->lastlogin,pCity->lastlogout,pCity->state,pCity->posx,pCity->posy,pCity->sflag,pCity->level,pCity->vipexp,pCity->politics,pCity->politics_exp,pCity->farm,pCity->farm_exp,pCity->business,pCity->business_exp,pCity->military,pCity->military_exp,pCity->people,pCity->food,pCity->money,pCity->soldiers);
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
			goto RE_CITY_UPDATE;
		}
		return -1;
	}
	return 0;
}

int city_batch_save_auto( City *pCity, int maxcount, char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	if ( pCity == NULL )
		return -1;

	char szText_name[MAX_PATH]={0};
	char szText_ipcountry[MAX_PATH]={0};
	int count = 0;
	memset( g_batchsql, 0, sizeof(char)*BATCHSQL_MAXSIZE );
	for ( int index = 0; index < maxcount; index++ )
	{
		if ( pCity[index].cityid <= 0 )
			continue;
		if ( count == 0 )
		{
			sprintf( g_batchsql, "REPLACE INTO %s (`cityid`,`actorid`,`name`,`type`,`shape`,`headid`,`country`,`ipcountry`,`language`,`os`,`platid`,`createtime`,`lastlogin`,`lastlogout`,`state`,`posx`,`posy`,`sflag`,`level`,`vipexp`,`politics`,`politics_exp`,`farm`,`farm_exp`,`business`,`business_exp`,`military`,`military_exp`,`people`,`food`,`money`,`soldiers`) Values('%d','%d','%s','%d','%d','%d','%d','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,pCity[index].cityid,pCity[index].actorid,db_escape((const char *)pCity[index].name,szText_name,0),pCity[index].type,pCity[index].shape,pCity[index].headid,pCity[index].country,db_escape((const char *)pCity[index].ipcountry,szText_ipcountry,0),pCity[index].language,pCity[index].os,pCity[index].platid,pCity[index].createtime,pCity[index].lastlogin,pCity[index].lastlogout,pCity[index].state,pCity[index].posx,pCity[index].posy,pCity[index].sflag,pCity[index].level,pCity[index].vipexp,pCity[index].politics,pCity[index].politics_exp,pCity[index].farm,pCity[index].farm_exp,pCity[index].business,pCity[index].business_exp,pCity[index].military,pCity[index].military_exp,pCity[index].people,pCity[index].food,pCity[index].money,pCity[index].soldiers);
		}
		else
		{
			sprintf( szSQL, ",('%d','%d','%s','%d','%d','%d','%d','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pCity[index].cityid,pCity[index].actorid,db_escape((const char *)pCity[index].name,szText_name,0),pCity[index].type,pCity[index].shape,pCity[index].headid,pCity[index].country,db_escape((const char *)pCity[index].ipcountry,szText_ipcountry,0),pCity[index].language,pCity[index].os,pCity[index].platid,pCity[index].createtime,pCity[index].lastlogin,pCity[index].lastlogout,pCity[index].state,pCity[index].posx,pCity[index].posy,pCity[index].sflag,pCity[index].level,pCity[index].vipexp,pCity[index].politics,pCity[index].politics_exp,pCity[index].farm,pCity[index].farm_exp,pCity[index].business,pCity[index].business_exp,pCity[index].military,pCity[index].military_exp,pCity[index].people,pCity[index].food,pCity[index].money,pCity[index].soldiers);
			strcat( g_batchsql, szSQL );
		}
		count += 1;
		if ( count > 5 )
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
