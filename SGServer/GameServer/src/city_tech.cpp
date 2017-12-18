#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <math.h>
#include "db.h"
#include "global.h"
#include "actor.h"
#include "city.h"
#include "city_attr.h"
#include "city_tech.h"
#include "building.h"
#include "mapunit.h"
#include "map.h"
#include "equip.h"
#include "quest.h"
#include "hero.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;
extern Map g_map;

extern Actor *g_actors;
extern int g_maxactornum;

extern City *g_city;
extern int g_city_maxcount;

extern BuildingUpgrade *g_building_upgrade;
extern int g_building_upgrade_maxnum;

extern OfficialTech *g_official_tech;
extern int g_official_tech_maxnum;

extern TechInfo *g_techinfo;
extern int g_techinfo_maxnum;

// 城池科技研究
int city_tech_upgrade( int actor_index, short kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( kind <= 0 || kind >= g_techinfo_maxnum )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Building *pBuilding = building_getptr_kind( g_actors[actor_index].city_index, BUILDING_Tech );
	if ( !pBuilding )
		return -1;
	if ( pBuilding->sec > 0 )
		return -1;
	if ( pBuilding->overvalue > 0 )
		return -1;
	int level = pCity->techlevel[kind];
	int uplevel = pCity->techlevel[kind] + 1;
	int progress = pCity->techprogress[kind];
	if ( uplevel <= 0 || uplevel >= g_techinfo[kind].maxnum )
		return -1;

	TechInfoConfig *config = &g_techinfo[kind].config[uplevel];
	if ( !config )
		return -1;

	// 太学院等级检查
	if ( pBuilding->level < config->buildinglevel )
		return -1;

	// 前置检查
	int prekind = config->prekind;
	int prelevel = config->prelevel;
	if ( prekind > 0 && prekind < g_techinfo_maxnum )
	{
		if ( pCity->techlevel[prekind] < prelevel )
		{
			return -1;
		}
	}

	// 资源检查
	if ( config->silver > 0 )
	{
		if ( pCity->silver < config->silver )
			return -1;
	}
	if ( config->wood > 0 )
	{
		if ( pCity->wood < config->wood )
			return -1;
	}
	if ( config->food > 0 )
	{
		if ( pCity->food < config->food )
			return -1;
	}
	if ( config->iron > 0 )
	{
		if ( pCity->iron < config->iron )
			return -1;
	}

	if ( config->silver > 0 )
		city_changesilver( pCity->index, -config->silver, PATH_TECH_UPGRADE );
	if ( config->wood > 0 )
		city_changewood( pCity->index, -config->wood, PATH_TECH_UPGRADE );
	if ( config->food > 0 )
		city_changefood( pCity->index, -config->food, PATH_TECH_UPGRADE );
	if ( config->iron > 0 )
		city_changeiron( pCity->index, -config->iron, PATH_TECH_UPGRADE );

	// 更新研究员信息
	pCity->ofquick[2] = 0;
	city_officialhire_sendinfo( pCity, 2 );

	pBuilding->sec = config->sec;
	pBuilding->needsec = config->sec;
	pBuilding->value = kind;
	pBuilding->overvalue = 0;
	building_sendinfo( actor_index, BUILDING_Tech );
	wlog( 0, LOGOP_TECH, PATH_TECH_UPGRADE, kind, pCity->techlevel[kind], pCity->techprogress[kind], pCity->actorid, city_mainlevel( pCity->index ) );

	// 任务
	quest_addvalue( pCity, QUEST_DATATYPE_CITY_TECHONE, kind, 0, 1 );
	return 0;
}

// 加速
int city_tech_quick( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Building *pBuilding = building_getptr_kind( g_actors[actor_index].city_index, BUILDING_Tech );
	if ( !pBuilding )
		return -1;
	int kind = pBuilding->value;
	if ( kind <= 0 || kind >= g_techinfo_maxnum )
		return -1;
	int token = (int)ceil((pBuilding->sec/60+1) * global.techquick_token);
	if ( actor_change_token( actor_index, -token, PATH_TECH_QUICK, 0 ) < 0 )
		return -1;

	wlog( 0, LOGOP_TECH, PATH_TECH_QUICK, kind, pCity->techlevel[kind], pCity->techprogress[kind], pCity->actorid, city_mainlevel( pCity->index ) );
	city_tech_finish( pCity, pBuilding );
	return 0;
}

// 免费加速
int city_tech_freequick( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Building *pBuilding = building_getptr_kind( g_actors[actor_index].city_index, BUILDING_Tech );
	if ( !pBuilding )
		return -1;
	int kind = pBuilding->value;
	if ( kind <= 0 || kind >= g_techinfo_maxnum )
		return -1;

	int ofkind = pCity->ofkind[2];
	if ( ofkind <= 0 || ofkind >= g_official_tech_maxnum )
		return -1;

	// -1 这个科技已经使用过免费加速
	if ( pCity->ofquick[2] == -1 )
		return -1;
	if ( pCity->ofquick[2] > 0 )
	{ // 之前使用过，后来更换了研究员，时间=更换后-更换前的差值
		pBuilding->sec -= pCity->ofquick[2];
	}
	else if ( pCity->ofquick[2] == 0 )
	{ // 正常加速时间
		pBuilding->sec -= g_official_tech[ofkind].quick;
	}

	// 完成
	if ( pBuilding->sec <= 0 )
	{
		city_tech_finish( pCity, pBuilding );
	}
	else
	{
		// 更新研究员信息
		pCity->ofquick[2] = -1;
		city_officialhire_sendinfo( pCity, 2 );
		building_sendinfo( pCity->actor_index, pBuilding->kind );
	}

	wlog( 0, LOGOP_TECH, PATH_TECH_FREEQUICK, kind, pCity->techlevel[kind], pCity->techprogress[kind], pCity->actorid, city_mainlevel( pCity->index ) );
	return 0;
}

// 完成
int city_tech_finish( City *pCity, Building *pBuilding )
{
	if ( !pCity || !pBuilding )
		return -1;
	// 更新研究员信息
	pCity->ofquick[2] = 0;
	city_officialhire_sendinfo( pCity, 2 );

	int kind = pBuilding->value;
	if ( kind <= 0 || kind >= g_techinfo_maxnum )
	{
		pBuilding->sec = 0;
		pBuilding->needsec = 0;
		pBuilding->value = 0;
		pBuilding->overvalue = 0;
		wlog( 0, LOGOP_TECH, PATH_TECH_FINISH, kind, -1, -1, pCity->actorid, city_mainlevel( pCity->index ) );
		return -1;
	}
	pBuilding->sec = 0;
	pBuilding->needsec = 0;
	pBuilding->overvalue = kind;
	pBuilding->value = 0;
	building_sendinfo( pCity->actor_index, pBuilding->kind );
	wlog( 0, LOGOP_TECH, PATH_TECH_FINISH, pBuilding->overvalue, pCity->techlevel[kind], pCity->techprogress[kind], pCity->actorid, city_mainlevel( pCity->index ) );
	return 0;
}

// 领取
int city_tech_get( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Building *pBuilding = building_getptr_kind( g_actors[actor_index].city_index, BUILDING_Tech );
	if ( !pBuilding )
		return -1;
	int kind = pBuilding->overvalue;
	pBuilding->overvalue = 0;
	if ( kind <= 0 || kind >= g_techinfo_maxnum )
		return -1;

	int level = pCity->techlevel[kind];
	int uplevel = pCity->techlevel[kind] + 1;
	if ( uplevel <= 0 || uplevel >= g_techinfo[kind].maxnum )
		return -1;

	TechInfoConfig *config = &g_techinfo[kind].config[uplevel];
	if ( !config )
		return -1;
	
	pCity->techprogress[kind] += 1;
	if ( pCity->techprogress[kind] >= config->progress )
	{
		pCity->techlevel[kind] += 1;
		pCity->techprogress[kind] = 0;
	}
	
	if ( pCity->actor_index >= 0 )
	{
		SLK_NetS_TechChange pValue = { 0 };
		pValue.m_kind = kind;
		pValue.m_level = pCity->techlevel[kind];
		pValue.m_progress = pCity->techprogress[kind];
		netsend_techchange_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
		building_sendinfo( pCity->actor_index, pBuilding->kind );
	}
	// 计算临时属性
	city_attr_reset( pCity );

	// 重算武将属性
	if ( kind > 20 )
	{
		hero_attr_calc_all( pCity, 1 );
	}

	// 任务
	quest_addvalue( pCity, QUEST_DATATYPE_CITY_TECH, kind, 0, pCity->techlevel[kind] );

	city_event_add( pCity->index, CITY_EVENT_TECH, kind, pCity->techlevel[kind] * 100 + pCity->techprogress[kind] );
	wlog( 0, LOGOP_TECH, PATH_TECH_GET, kind, pCity->techlevel[kind], pCity->techprogress[kind], g_actors[actor_index].actorid, city_mainlevel( pCity->index ) );
	return 0;
}

//GM
int city_tech_get_gm( City *pCity, int kind )
{
	Building *pBuilding = building_getptr_kind( pCity->index, BUILDING_Tech );
	if ( !pBuilding )
		return -1;
	if ( kind <= 0 || kind >= g_techinfo_maxnum )
		return -1;

	int level = pCity->techlevel[kind];
	int uplevel = pCity->techlevel[kind] + 1;
	if ( uplevel <= 0 || uplevel >= g_techinfo[kind].maxnum )
		return -1;

	TechInfoConfig *config = &g_techinfo[kind].config[uplevel];
	if ( !config )
		return -1;

	pCity->techprogress[kind] += 1;
	if ( pCity->techprogress[kind] >= config->progress )
	{
		pCity->techlevel[kind] += 1;
		pCity->techprogress[kind] = 0;
	}

	if ( pCity->actor_index >= 0 )
	{
		SLK_NetS_TechChange pValue = { 0 };
		pValue.m_kind = kind;
		pValue.m_level = pCity->techlevel[kind];
		pValue.m_progress = pCity->techprogress[kind];
		netsend_techchange_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
		building_sendinfo( pCity->actor_index, pBuilding->kind );
	}
	// 计算临时属性
	city_attr_reset( pCity );

	return 0;
}