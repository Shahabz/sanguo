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
	int actorid = 0;

	sprintf( szSQL, "select `actorid`,`silver_per`,`wood_per`,`food_per`,`iron_per`,`train_per0`,`train_per1`,`train_per2`,`movespeed_per0`,`movespeed_per1`,`movespeed_per2`,`protectres_per`,`buildingsec_per`,`materialsec_per0`,`materialsec_per1`,`gather_per0`,`gather_per1`,`heroluck_per`,`prestige_per`,`damageguard_per`,`trainspeed_per`,`hero_attack0`,`hero_attack1`,`hero_attack2`,`hero_defense0`,`hero_defense1`,`hero_defense2`,`hero_troops0`,`hero_troops1`,`hero_troops2`,`hero_up_num`,`hero_row_num`,`nequip_crit`,`spy_city`,`free_sec`,`everyday_body_buymax`,`everyday_autobuild_buynum`,`everyday_army_recall`,`ability_open_201`,`ability_open_202`,`ability_open_203`,`ability_open_204`,`ability_open_205`,`ability_open_206`,`ability_open_207` from %s ", pTab );
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
		pCityAttr = pCB_GetCityAttr( actorid );
		if( pCityAttr == NULL )
			continue;
		pCityAttr->actorid = atoi(row[offset++]);
		pCityAttr->silver_per = (float)atof(row[offset++]);
		pCityAttr->wood_per = (float)atof(row[offset++]);
		pCityAttr->food_per = (float)atof(row[offset++]);
		pCityAttr->iron_per = (float)atof(row[offset++]);
		pCityAttr->train_per[0] = atoi(row[offset++]);
		pCityAttr->train_per[1] = atoi(row[offset++]);
		pCityAttr->train_per[2] = atoi(row[offset++]);
		pCityAttr->movespeed_per[0] = atoi(row[offset++]);
		pCityAttr->movespeed_per[1] = atoi(row[offset++]);
		pCityAttr->movespeed_per[2] = atoi(row[offset++]);
		pCityAttr->protectres_per = (float)atof(row[offset++]);
		pCityAttr->buildingsec_per = (float)atof(row[offset++]);
		pCityAttr->materialsec_per[0] = atoi(row[offset++]);
		pCityAttr->materialsec_per[1] = atoi(row[offset++]);
		pCityAttr->gather_per[0] = atoi(row[offset++]);
		pCityAttr->gather_per[1] = atoi(row[offset++]);
		pCityAttr->heroluck_per = (float)atof(row[offset++]);
		pCityAttr->prestige_per = (float)atof(row[offset++]);
		pCityAttr->damageguard_per = (float)atof(row[offset++]);
		pCityAttr->trainspeed_per = (float)atof(row[offset++]);
		pCityAttr->hero_attack[0] = atoi(row[offset++]);
		pCityAttr->hero_attack[1] = atoi(row[offset++]);
		pCityAttr->hero_attack[2] = atoi(row[offset++]);
		pCityAttr->hero_defense[0] = atoi(row[offset++]);
		pCityAttr->hero_defense[1] = atoi(row[offset++]);
		pCityAttr->hero_defense[2] = atoi(row[offset++]);
		pCityAttr->hero_troops[0] = atoi(row[offset++]);
		pCityAttr->hero_troops[1] = atoi(row[offset++]);
		pCityAttr->hero_troops[2] = atoi(row[offset++]);
		pCityAttr->hero_up_num = atoi(row[offset++]);
		pCityAttr->hero_row_num = atoi(row[offset++]);
		pCityAttr->nequip_crit = atoi(row[offset++]);
		pCityAttr->spy_city = atoi(row[offset++]);
		pCityAttr->free_sec = atoi(row[offset++]);
		pCityAttr->everyday_body_buymax = atoi(row[offset++]);
		pCityAttr->everyday_autobuild_buynum = atoi(row[offset++]);
		pCityAttr->everyday_army_recall = atoi(row[offset++]);
		pCityAttr->ability_open_201 = atoi(row[offset++]);
		pCityAttr->ability_open_202 = atoi(row[offset++]);
		pCityAttr->ability_open_203 = atoi(row[offset++]);
		pCityAttr->ability_open_204 = atoi(row[offset++]);
		pCityAttr->ability_open_205 = atoi(row[offset++]);
		pCityAttr->ability_open_206 = atoi(row[offset++]);
		pCityAttr->ability_open_207 = atoi(row[offset++]);
		if( pCB_LoadCityAttr )
			pCB_LoadCityAttr( pCityAttr->actorid );
		actorid += 1;
	}
	g_city_attr_maxindex = actorid;
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
	sprintf( szSQL, "REPLACE INTO %s (`actorid`,`silver_per`,`wood_per`,`food_per`,`iron_per`,`train_per0`,`train_per1`,`train_per2`,`movespeed_per0`,`movespeed_per1`,`movespeed_per2`,`protectres_per`,`buildingsec_per`,`materialsec_per0`,`materialsec_per1`,`gather_per0`,`gather_per1`,`heroluck_per`,`prestige_per`,`damageguard_per`,`trainspeed_per`,`hero_attack0`,`hero_attack1`,`hero_attack2`,`hero_defense0`,`hero_defense1`,`hero_defense2`,`hero_troops0`,`hero_troops1`,`hero_troops2`,`hero_up_num`,`hero_row_num`,`nequip_crit`,`spy_city`,`free_sec`,`everyday_body_buymax`,`everyday_autobuild_buynum`,`everyday_army_recall`,`ability_open_201`,`ability_open_202`,`ability_open_203`,`ability_open_204`,`ability_open_205`,`ability_open_206`,`ability_open_207`) Values('%d','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,pCityAttr->actorid,pCityAttr->silver_per,pCityAttr->wood_per,pCityAttr->food_per,pCityAttr->iron_per,pCityAttr->train_per[0],pCityAttr->train_per[1],pCityAttr->train_per[2],pCityAttr->movespeed_per[0],pCityAttr->movespeed_per[1],pCityAttr->movespeed_per[2],pCityAttr->protectres_per,pCityAttr->buildingsec_per,pCityAttr->materialsec_per[0],pCityAttr->materialsec_per[1],pCityAttr->gather_per[0],pCityAttr->gather_per[1],pCityAttr->heroluck_per,pCityAttr->prestige_per,pCityAttr->damageguard_per,pCityAttr->trainspeed_per,pCityAttr->hero_attack[0],pCityAttr->hero_attack[1],pCityAttr->hero_attack[2],pCityAttr->hero_defense[0],pCityAttr->hero_defense[1],pCityAttr->hero_defense[2],pCityAttr->hero_troops[0],pCityAttr->hero_troops[1],pCityAttr->hero_troops[2],pCityAttr->hero_up_num,pCityAttr->hero_row_num,pCityAttr->nequip_crit,pCityAttr->spy_city,pCityAttr->free_sec,pCityAttr->everyday_body_buymax,pCityAttr->everyday_autobuild_buynum,pCityAttr->everyday_army_recall,pCityAttr->ability_open_201,pCityAttr->ability_open_202,pCityAttr->ability_open_203,pCityAttr->ability_open_204,pCityAttr->ability_open_205,pCityAttr->ability_open_206,pCityAttr->ability_open_207);
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

