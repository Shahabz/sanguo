#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "global.h"
#include "actor.h"
#include "city.h"
#include "building.h"
#include "mapunit.h"
#include "map.h"
#include "server_netsend_auto.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;
extern Map g_map;

extern Actor *g_actors;
extern int g_maxactornum;

extern int g_city_maxindex;
extern City *g_city;
extern int g_city_maxcount;

extern BuildingUpgrade *g_building_upgrade;
extern int g_building_upgrade_maxnum;

// 获取建筑指针
Building* building_getptr( int city_index, int offset )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	if ( offset < 0 || offset >= BUILDING_MAXNUM )
		return NULL;
	return &g_city[city_index].building[offset];
}

BuildingBarracks* buildingbarracks_getptr( int city_index, int offset )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	if ( offset < 0 || offset >= BUILDING_BARRACKS_MAXNUM )
		return NULL;
	return &g_city[city_index].building_barracks[offset];
}

BuildingRes* buildingres_getptr( int city_index, int offset )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	if ( offset < 0 || offset >= BUILDING_RES_MAXNUM )
		return NULL;
	return &g_city[city_index].building_res[offset];
}

// 获取建筑的配置信息
BuildingUpgradeConfig* building_getconfig( struct _city *pCity, short buildingkind )
{
	if ( pCity == NULL )
		return NULL;
	short buildingindex = building_getindex( pCity, buildingkind );
	if ( buildingindex < 0 || buildingindex >= CITY_BUILDING_MAX )
		return NULL;
	int buildinglevel = pCity->building[buildingindex].level;
	if ( buildinglevel < 0 || buildinglevel > g_BuildingUpgrade[buildingkind].maxlevel )
		return NULL;
	return &g_BuildingUpgrade[buildingkind].info[buildinglevel];
}

// 创建建筑
int building_create( int city_index, int kind, int offset )
{
	CITY_CHECK_INDEX( city_index );
	if ( kind < BUILDING_Infantry )
	{ // 普通建筑

	}
	else if ( kind == BUILDING_Infantry || kind == BUILDING_Cavalry || kind == BUILDING_Archer || kind == BUILDING_Militiaman )
	{ // 兵营建筑

	}
	else if ( kind == BUILDING_Silver || kind == BUILDING_Wood || kind == BUILDING_Food || kind == BUILDING_Iron )
	{ // 资源建筑
	}
	else
	{

	}
	return 0;
}

// 建筑升级
int building_upgrade( int city_index, int kind, int offset )
{
	CITY_CHECK_INDEX( city_index );
	if ( kind < BUILDING_Infantry )
	{ // 普通建筑
		int buildingindex = 
		g_city[city_index].building[]
	}
	else if ( kind == BUILDING_Infantry || kind == BUILDING_Cavalry || kind == BUILDING_Archer || kind == BUILDING_Militiaman )
	{ // 兵营建筑

	}
	else if ( kind == BUILDING_Silver || kind == BUILDING_Wood || kind == BUILDING_Food || kind == BUILDING_Iron )
	{ // 资源建筑
	}
	else
	{

	}
	return 0;
}

// 获取士兵数量
int building_soldiers_total( int city_index, char kind )
{
	CITY_CHECK_INDEX( city_index );
	int total = 0;
	for ( int tmpi = 0; tmpi < BUILDING_BARRACKS_MAXNUM; tmpi++ )
	{
		if ( g_city[city_index].building_barracks[tmpi].kind == kind ||
			 g_city[city_index].building_barracks[tmpi].value == kind )
		{
			total += g_city[city_index].building_barracks[tmpi].soldiers;
		}
	}
	return total;
}

int building_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_BuildingList pValue = { 0 };

	for ( int tmpi = 0; tmpi < BUILDING_MAXNUM; tmpi++ )
	{
		pValue.m_building[tmpi].m_kind = pCity->building[tmpi].kind;
	}

	netsend_buildinglist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
