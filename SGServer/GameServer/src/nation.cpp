#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "db.h"
#include "define.h"
#include "dserver.h"
#include "building.h"
#include "actor.h"
#include "hero.h"
#include "area.h"
#include "actor_send.h"
#include "actor_times.h"
#include "server_netsend_auto.h"
#include "mapunit.h"
#include "fight.h"
#include "actor_notify.h"
#include "item.h"
#include "award.h"
#include "global.h"
#include "quest.h"
#include "activity.h"
#include "system.h"
#include "army.h"
#include "map.h"
#include "map_town.h"
#include "mail.h"
#include "city.h"
#include "nation.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern City * g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern MonsterInfo *g_monster;
extern int g_monster_maxnum;

extern ArmyGroup *g_armygroup;
extern int g_armygroup_maxcount;

extern MapTownInfo *g_towninfo;
extern int g_towninfo_maxnum;

extern MapTown *g_map_town;
extern int g_map_town_maxcount;
extern int g_map_town_maxindex;

Nation g_nation[NATION_MAX] = { 0 };

// 读档完毕的回调
int nation_loadcb( int nation )
{
	if ( nation <= 0 || nation >= NATION_MAX )
		return -1;
	return 0;
}

int nation_load()
{
	memset( g_nation, 0, sizeof( Nation ) * NATION_MAX );
	printf_msg( "Nation  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", NATION_MAX, (sizeof( Nation )*NATION_MAX) / 1024.0 / 1024.0, sizeof( Nation ) / 1024.0 );
	for ( int nation = 0; nation < NATION_MAX; nation++ )
	{
		g_nation[nation].nation = nation;
		for ( int tmpi = 0; tmpi < NATION_OFFICIAL_MAX; tmpi++ )
		{
			g_nation[nation].official_city_index[tmpi] = -1;
		}

		for ( int tmpi = 0; tmpi < NATION_TOWN_WAR_MAX; tmpi++ )
		{
			g_nation[nation].town_war_index[tmpi] = -1;
		}

		for ( int tmpi = 0; tmpi < NATION_CITY_WAR_MAX; tmpi++ )
		{
			g_nation[nation].city_war_index[tmpi] = -1;
		}
		// 计算名城人口
		nation_people_famous_calc( nation );
		// 计算都城人口
		nation_people_capital_calc( nation );
	}
	nation_load_auto( nation_getptr, nation_loadcb, "nation" );
	return 0;
}

int nation_save( FILE *fp )
{
	for ( int nation = 0; nation < NATION_MAX; nation++ )
	{
		nation_save_auto( &g_nation[nation], "nation", fp );
	}
	return 0;
}

Nation *nation_getptr( int nation )
{
	if ( nation < 0 || nation >= NATION_MAX )
		return NULL;
	return &g_nation[nation];
}

// 设置国家都城
int nation_capital_townid( int nation, int townid )
{
	if ( nation <= 0 || nation >= NATION_MAX )
		return 0;
	g_nation[nation].capital_townid = townid;
	return 0;
}
int nation_capital_townid_get( int nation )
{
	if ( nation <= 0 || nation >= NATION_MAX )
		return 0;
	return g_nation[nation].capital_townid;
}

// 国家人口
int nation_people_calc( int nation )
{
	if ( nation <= 0 || nation >= NATION_MAX )
		return 0;
	g_nation[nation].people = g_nation[nation].people_famous + g_nation[nation].people_capital;
	return 0;
}
int nation_people_get( int nation )
{
	if ( nation <= 0 || nation >= NATION_MAX )
		return 0;
	return g_nation[nation].people;
}

// 名称人口计算
int nation_people_famous_calc( int nation )
{
	if ( nation <= 0 || nation >= NATION_MAX )
		return 0;
	int people = 0;
	for ( int townid = 1; townid < g_map_town_maxcount; townid++ )
	{
		if ( g_towninfo[townid].id <= 0 )
			continue;
		if ( nation == g_map_town[townid].nation )
		{
			people += g_towninfo[townid].people;
		}
	}
	g_nation[nation].people_famous = people;
	nation_people_calc( nation );
	return people;
}

// 都城人口计算
int nation_people_capital_calc( int nation )
{
	if ( nation < 0 || nation >= NATION_MAX )
		return 0;
	int people = 0;
	for ( int townid = 1; townid < g_map_town_maxcount; townid++ )
	{
		if ( g_towninfo[townid].id <= 0 )
			continue;
		if ( g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_TYPE8 )
		{
			if ( nation == g_map_town[townid].nation )
			{
				people = g_map_town[townid].dev_exp;
				g_nation[nation].capital_townid = townid;
				break;
			}
		}
	}
	g_nation[nation].people_capital = people;
	nation_people_calc( nation );
	return people;
}

// 都城人口设置
int nation_people_capital_set( int nation, int people )
{
	if ( nation < 0 || nation >= NATION_MAX )
		return 0;
	g_nation[nation].people_capital = people;
	nation_people_calc( nation );
	return 0;
}

// 该国家指定类型城池数量
int nation_town_num( int nation, int towntype )
{
	if ( nation <= 0 || nation >= NATION_MAX )
		return 0;
	int num = 0;
	for ( int townid = 1; townid < g_map_town_maxcount; townid++ )
	{
		if ( g_towninfo[townid].id <= 0 )
			continue;
		if ( nation != g_map_town[townid].nation )
			continue;
		if ( g_towninfo[townid].type == towntype )
		{
			num += 1;
		}
	}
	return num;
}

// 城战攻击信息
int nation_city_war_add( int nation, int group_index )
{
	if ( nation <= 0 || nation >= NATION_MAX )
		return -1;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	int index = -1;
	for ( int tmpi = 0; tmpi < NATION_CITY_WAR_MAX; tmpi++ )
	{
		if ( g_nation[nation].city_war_index[tmpi] < 0 )
		{
			index = tmpi;
			break;
		}
	}
	if ( index < 0 )
		return -1;
	g_nation[nation].city_war_index[index] = group_index;
	return 0;
}
int nation_city_war_del( int nation, int group_index )
{
	if ( nation <= 0 || nation >= NATION_MAX )
		return -1;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	int tmpi = 0;
	for ( tmpi = 0; tmpi < NATION_CITY_WAR_MAX; tmpi++ )
	{
		if ( g_nation[nation].city_war_index[tmpi] == group_index )
		{
			g_nation[nation].city_war_index[tmpi] = -1;
			break;
		}
	}
	return 0;
}

// 国战攻击信息
int nation_town_war_add( int nation, int group_index )
{
	if ( nation <= 0 || nation >= NATION_MAX )
		return -1;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	int index = -1;
	for ( int tmpi = 0; tmpi < NATION_TOWN_WAR_MAX; tmpi++ )
	{
		if ( g_nation[nation].town_war_index[tmpi] < 0 )
		{
			index = tmpi;
			break;
		}
	}
	if ( index < 0 )
		return -1;
	g_nation[nation].town_war_index[index] = group_index;
	return 0;
}
int nation_town_war_del( int nation, int group_index )
{
	if ( nation <= 0 || nation >= NATION_MAX )
		return -1;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	int tmpi = 0;
	for ( tmpi = 0; tmpi < NATION_TOWN_WAR_MAX; tmpi++ )
	{
		if ( g_nation[nation].town_war_index[tmpi] == group_index )
		{
			g_nation[nation].town_war_index[tmpi] = -1;
			break;
		}
	}
	return 0;
}
