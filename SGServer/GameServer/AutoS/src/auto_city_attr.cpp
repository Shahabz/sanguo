#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_city_attr.h"
extern MYSQL *myGame;
int g_city_attr_maxindex = 0;

int city_attr_load_auto( LPCB_GETCITYATTR pCB_GetCityAttr, LPCB_LOADCITYATTR pCB_LoadCityAttr, const char *pTab )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[8192] = {0};
	int offset = 0;
	CityAttr *pCityAttr;

	sprintf( szSQL, "select `actorid`,`silver_per`,`wood_per`,`food_per`,`iron_per`,`protectres_per`,`buildingsec_per`,`infantry_attack`,`cavalry_attack`,`archer_attack` from %s ", pTab );
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
		pCityAttr->actorid = atoi(row[offset++]);
		pCityAttr->silver_per = atoi(row[offset++]);
		pCityAttr->wood_per = atoi(row[offset++]);
		pCityAttr->food_per = atoi(row[offset++]);
		pCityAttr->iron_per = atoi(row[offset++]);
		pCityAttr->protectres_per = atoi(row[offset++]);
		pCityAttr->buildingsec_per = atoi(row[offset++]);
		pCityAttr->infantry_attack = atoi(row[offset++]);
		pCityAttr->cavalry_attack = atoi(row[offset++]);
		pCityAttr->archer_attack = atoi(row[offset++]);
	}
	g_city_attr_maxindex = ;
	mysql_free_result( res );
	return 0;
}
int city_attr_save_auto( CityAttr *pCityAttr, const char *pTab, FILE *fp )
{
	char	szSQL[8192] = {0};
	char reconnect_flag = 0;
	if ( pCityAttr == NULL )
		return -1;

RE_CITYATTR_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`actorid`,`silver_per`,`wood_per`,`food_per`,`iron_per`,`protectres_per`,`buildingsec_per`,`infantry_attack`,`cavalry_attack`,`archer_attack`) Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,pCityAttr->actorid,pCityAttr->silver_per,pCityAttr->wood_per,pCityAttr->food_per,pCityAttr->iron_per,pCityAttr->protectres_per,pCityAttr->buildingsec_per,pCityAttr->infantry_attack,pCityAttr->cavalry_attack,pCityAttr->archer_attack);
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
			goto RE_CITYATTR_UPDATE;
		}
		return -1;
	}
	return 0;
}

