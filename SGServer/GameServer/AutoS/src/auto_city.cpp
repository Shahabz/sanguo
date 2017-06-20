#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_city.h"
extern MYSQL *myGame;
int g_city_maxindex = 0;

int city_load_auto( LPCB_GETCITY pCB_GetCity, LPCB_LOADCITY pCB_LoadCity, char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	City *pCity;
	int actorid = 0;

	sprintf( szSQL, "select `cityid`,`actorid`,`name`,`type`,`shape`,`headid`,`country`,`ipcountry`,`language`,`os`,`platid`,`createtime`,`lastlogin`,`lastlogout`,`state`,`posx`,`posy`,`sflag`,`level`,`vipexp`,`nation`,`official`,`place`,`zone`,`battlepower`,`mokilllv`,`body`,`bodysec`,`silver`,`wood`,`food`,`iron`,`levynum`,`levysec`,`people`,`worker_type`,`worker_offset`,`worker_sec`,`worker_type_ex`,`worker_offset_ex`,`worker_sec_ex`,`worker_expire_ex` from %s ", pTab );
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
		pCity->nation = atoi(row[offset++]);
		pCity->official = atoi(row[offset++]);
		pCity->place = atoi(row[offset++]);
		pCity->zone = atoi(row[offset++]);
		pCity->battlepower = atoi(row[offset++]);
		pCity->mokilllv = atoi(row[offset++]);
		pCity->body = atoi(row[offset++]);
		pCity->bodysec = atoi(row[offset++]);
		pCity->silver = atoi(row[offset++]);
		pCity->wood = atoi(row[offset++]);
		pCity->food = atoi(row[offset++]);
		pCity->iron = atoi(row[offset++]);
		pCity->levynum = atoi(row[offset++]);
		pCity->levysec = atoi(row[offset++]);
		pCity->people = atoi(row[offset++]);
		pCity->worker_type = atoi(row[offset++]);
		pCity->worker_offset = atoi(row[offset++]);
		pCity->worker_sec = atoi(row[offset++]);
		pCity->worker_type_ex = atoi(row[offset++]);
		pCity->worker_offset_ex = atoi(row[offset++]);
		pCity->worker_sec_ex = atoi(row[offset++]);
		pCity->worker_expire_ex = atoi(row[offset++]);
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
	sprintf( szSQL, "REPLACE INTO %s (`cityid`,`actorid`,`name`,`type`,`shape`,`headid`,`country`,`ipcountry`,`language`,`os`,`platid`,`createtime`,`lastlogin`,`lastlogout`,`state`,`posx`,`posy`,`sflag`,`level`,`vipexp`,`nation`,`official`,`place`,`zone`,`battlepower`,`mokilllv`,`body`,`bodysec`,`silver`,`wood`,`food`,`iron`,`levynum`,`levysec`,`people`,`worker_type`,`worker_offset`,`worker_sec`,`worker_type_ex`,`worker_offset_ex`,`worker_sec_ex`,`worker_expire_ex`) Values('%d','%d','%s','%d','%d','%d','%d','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,pCity->cityid,pCity->actorid,db_escape((const char *)pCity->name,szText_name,0),pCity->type,pCity->shape,pCity->headid,pCity->country,db_escape((const char *)pCity->ipcountry,szText_ipcountry,0),pCity->language,pCity->os,pCity->platid,pCity->createtime,pCity->lastlogin,pCity->lastlogout,pCity->state,pCity->posx,pCity->posy,pCity->sflag,pCity->level,pCity->vipexp,pCity->nation,pCity->official,pCity->place,pCity->zone,pCity->battlepower,pCity->mokilllv,pCity->body,pCity->bodysec,pCity->silver,pCity->wood,pCity->food,pCity->iron,pCity->levynum,pCity->levysec,pCity->people,pCity->worker_type,pCity->worker_offset,pCity->worker_sec,pCity->worker_type_ex,pCity->worker_offset_ex,pCity->worker_sec_ex,pCity->worker_expire_ex);
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

