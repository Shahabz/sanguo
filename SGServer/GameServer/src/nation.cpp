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
#include "army_group.h"
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

extern NationUpgrade *g_nation_upgrade;
extern int g_nation_upgrade_maxnum;

extern NationPlace *g_nation_place;
extern int g_nation_place_maxnum;

Nation g_nation[NATION_MAX] = { 0 };

// 读档完毕的回调
int nation_loadcb( int nation )
{
	if ( nation <= 0 || nation >= NATION_MAX )
		return -1;
	if ( g_nation[nation].level < 1 )
		g_nation[nation].level = 1;
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
	if ( nation < 0 || nation >= NATION_MAX )
		return 0;
	g_nation[nation].capital_townid = townid;
	return 0;
}
int nation_capital_townid_get( int nation )
{
	if ( nation < 0 || nation >= NATION_MAX )
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
	if ( nation < 0 || nation >= NATION_MAX )
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

// 国王名
void nation_kingname( char nation, char *out )
{
	Nation *pNation = nation_getptr( nation );
	if ( !pNation )
		return;
	if ( pNation->official_actorid[0] > 0 )
	{
		int city_index = pNation->official_city_index[0];
		if ( city_index < 0 || city_index >= g_city_maxcount )
		{
			city_index = city_getindex_withactorid( pNation->official_actorid[0] );
			if ( city_index < 0 || city_index >= g_city_maxcount )
				return;
		}
		strncpy( out, g_city[city_index].name, NAME_SIZE );
	}
	return;
}

// 国家升级
int nation_upgrade( char nation )
{
	Nation *pNation = nation_getptr( nation );
	if ( !pNation )
		return -1;
	if ( pNation->level >= g_nation_upgrade_maxnum - 1 )
		return -1;
	pNation->level += 1;
	return 0;
}

// 国家经验
int nation_exp( char nation, int exp )
{
	Nation *pNation = nation_getptr(nation );
	if ( !pNation )
		return -1;
	if ( pNation->level >= g_nation_upgrade_maxnum - 1 )
		return -1;
	pNation->exp += exp;

	// 检查升级
	char isup = 0;
	while ( pNation->exp >= g_nation_upgrade[pNation->level].config[1].maxexp )
	{
		int curlevel = pNation->level;
		// 可以升级
		if ( nation_upgrade( nation ) < 0 )
			break;
		pNation->exp -= g_nation_upgrade[pNation->level].config[1].maxexp;
		isup = 1;
	}

	if ( isup == 1 )
	{

	}
	return 0;
}

// 国家信息
int nation_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1;
	SLK_NetS_NationInfo pValue = { 0 };
	pValue.m_level = pNation->level;
	pValue.m_exp = pNation->exp;
	pValue.m_donate_num = actor_get_today_char_times( actor_index, TODAY_CHAR_NATION_DONATE );
	pValue.m_myrank = pCity->rank_nation;
	strncpy( pValue.m_notice, pNation->notice, sizeof( char )*NATION_NOTICE_MAX );
	pValue.m_notice_len = strlen( pValue.m_notice );
	nation_kingname( pCity->nation, pValue.m_kingname );
	pValue.m_kingname_len = strlen( pValue.m_kingname );
	netsend_nationinfo_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 国家信息
int nation_sendbase( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1;
	SLK_NetS_NationBase pValue = { 0 };
	pValue.m_level = pNation->level;
	pValue.m_exp = pNation->exp;
	pValue.m_donate_num = actor_get_today_char_times( actor_index, TODAY_CHAR_NATION_DONATE );
	pValue.m_myrank = pCity->rank_nation;
	netsend_nationbase_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 国家建设升级
int nation_build( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1;
	if ( pNation->level <= 0 || pNation->level >= g_nation_upgrade_maxnum - 1 )
		return -1;

	int donatenum = actor_get_today_char_times( actor_index, TODAY_CHAR_NATION_DONATE );
	if ( donatenum >= g_nation_upgrade[pNation->level].maxnum )
		return -1;
	int silver = g_nation_upgrade[pNation->level].config[donatenum].silver;
	int wood = g_nation_upgrade[pNation->level].config[donatenum].wood;
	int food = g_nation_upgrade[pNation->level].config[donatenum].food;

	if ( silver > 0 )
	{
		if ( pCity->silver < silver )
			return -1;
		city_changesilver( pCity->index, -silver, PATH_NATIONUPGRADE );
	}
	if ( wood > 0 )
	{
		if ( pCity->wood < wood )
			return -1;
		city_changesilver( pCity->index, -wood, PATH_NATIONUPGRADE );
	}
	if ( food > 0 )
	{
		if ( pCity->food < food )
			return -1;
		city_changesilver( pCity->index, -food, PATH_NATIONUPGRADE );
	}
	
	// 给与经验
	nation_exp( pCity->nation, g_nation_upgrade[pNation->level].config[donatenum].exp );
	// 给威望
	city_changeprestige( pCity->index, g_nation_upgrade[pNation->level].config[donatenum].prestige, PATH_NATIONUPGRADE );
	// 添加次数
	actor_add_today_char_times( actor_index, TODAY_CHAR_NATION_DONATE ); 
	nation_sendbase( actor_index );
	return 0;
}

// 国家爵位晋升
int nation_place_upgrade( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->place < 0 || pCity->place >= 15 || pCity->place >= g_nation_place_maxnum )
		return -1;

	NationPlace *config = &g_nation_place[pCity->place];
	if ( !config )
		return -1;
	// 检查
	if ( pCity->prestige < config->prestige )
	{
		return -1;
	}
	if ( pCity->silver < config->silver )
	{
		return -1;
	}
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		if ( config->cost_kind[tmpi] <= 0 )
			continue;
		if ( item_getitemnum( actor_index, config->cost_kind[tmpi] ) < config->cost_num[tmpi] )
			return -1;
	}
	// 扣
	city_changeprestige( pCity->index, -config->prestige, PATH_NATIONPLACE );
	city_changesilver( pCity->index, -config->silver, PATH_NATIONPLACE );
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		if ( config->cost_kind[tmpi] <= 0 )
			continue;
		item_lost( actor_index, config->cost_kind[tmpi], config->cost_num[tmpi], PATH_NATIONPLACE );
	}
	pCity->place += 1;
	hero_attr_calc_all( pCity, 0 );

	int pValue[2] = { 0 };
	pValue[0] = 3;
	pValue[1] = pCity->place;
	actor_notify_value( actor_index, NOTIFY_VALUECHANGE, 2, pValue, NULL );
	return 0;
}

// 国家城池
int nation_town_sendinfo( int actor_index, int townid, char path )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	SLK_NetS_NationTown pValue = { 0 };
	pValue.m_path = path;
	map_town_info_makestruct( &pValue.m_info, &g_map_town[townid], pCity->actorid, path );
	netsend_nationtown_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 国家城池列表
int nation_town_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_NationTownList pValue = { 0 };

	// 发送开始
	pValue.m_op = 1;
	pValue.m_count = 0;
	netsend_nationtownlist_S( actor_index, SENDTYPE_ACTOR, &pValue );

	// 发送过程
	pValue.m_op = 2;
	for ( int townid = 1; townid < g_map_town_maxcount; townid++ )
	{
		if ( g_map_town[townid].townid <= 0 )
			continue;
		if ( g_map_town[townid].nation != pCity->nation )
			continue;
		if ( g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_TYPE8 || g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_TYPE9 )
			continue;

		map_town_info_makestruct( &pValue.m_list[pValue.m_count].m_info, &g_map_town[townid], pCity->actorid, 0 );
		pValue.m_count += 1;
		if ( pValue.m_count >= 32 )
		{
			netsend_nationtownlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
			pValue.m_count = 0;
		}
	}
	if ( pValue.m_count > 0 )
	{
		netsend_nationtownlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}

	// 发送结束
	pValue.m_op = 3;
	pValue.m_count = 0;
	netsend_nationtownlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 修复
int nation_town_repair( int actor_index, int townid )
{
	map_town_soldiers_repair( actor_index, townid );
	nation_town_sendinfo( actor_index, townid, 0 );
	return 0;
}

// 重建
int nation_town_rebuild( int actor_index, int townid )
{
	map_town_ask_owner( actor_index, townid );
	nation_town_sendinfo( actor_index, townid, 0 );
	return 0;
}

// 国家战争列表
int nation_town_warlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	char nation = pCity->nation;
	SLK_NetS_NationWarList pValue = { 0 };
	// 发送开始
	pValue.m_op = 1;
	netsend_nationwarlist_S( actor_index, SENDTYPE_ACTOR, &pValue );

	// 发送过程
	pValue.m_op = 2;
	for ( int tmpi = 0; tmpi < NATION_TOWN_WAR_MAX; tmpi++ )
	{
		int group_index = g_nation[nation].town_war_index[tmpi];
		if ( group_index < 0 || group_index >= g_armygroup_maxcount )
			continue;
		if ( g_armygroup[group_index].id <= 0 )
			continue;
		if ( g_armygroup[group_index].to_type != MAPUNIT_TYPE_TOWN )
			continue;
		int townid = g_armygroup[group_index].to_id;
		if ( townid <= 0 || townid >= g_map_town_maxcount )
			continue;

		if ( nation == g_map_town[townid].nation )
		{ // 我看的据点属于我的国家

			// 那么我属于防御方
			pValue.m_list[pValue.m_count].m_attack = 2;
			pValue.m_list[pValue.m_count].m_nation = g_map_town[townid].nation;
			pValue.m_list[pValue.m_count].m_total = armygroup_to_totals( group_index );

			// 攻击方信息
			pValue.m_list[pValue.m_count].m_t_nation = g_armygroup[group_index].from_nation;
			pValue.m_list[pValue.m_count].m_t_total = armygroup_from_totals( group_index );
		}
		else if ( nation == g_armygroup[group_index].from_nation )
		{ // 不是我国家的据点, 看看我是不是攻击方的

			// 那么我属于攻击方
			pValue.m_list[pValue.m_count].m_attack = 1;
			pValue.m_list[pValue.m_count].m_nation = pCity->nation;
			pValue.m_list[pValue.m_count].m_total = armygroup_from_totals( group_index );

			pValue.m_list[pValue.m_count].m_t_nation = g_armygroup[group_index].to_nation;
			pValue.m_list[pValue.m_count].m_t_total = armygroup_to_totals( group_index );
		}
		else
		{ // 第三方
			continue;
		}

		pValue.m_list[pValue.m_count].m_group_index = group_index;
		pValue.m_list[pValue.m_count].m_group_id = g_armygroup[group_index].id;
		pValue.m_list[pValue.m_count].m_statetime = g_armygroup[group_index].statetime;
		pValue.m_list[pValue.m_count].m_stateduration = g_armygroup[group_index].stateduration;
		pValue.m_list[pValue.m_count].m_type = g_armygroup[group_index].type;
		pValue.m_list[pValue.m_count].m_townid = townid;
		pValue.m_list[pValue.m_count].m_town_nation = g_map_town[townid].nation;
		pValue.m_list[pValue.m_count].m_unit_index = g_map_town[townid].unit_index;
		pValue.m_count += 1;
		if ( pValue.m_count > 32 )
		{
			pValue.m_op = 1;
			netsend_nationwarlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
			pValue.m_count = 0;
		}
	}

	if ( pValue.m_count > 0 )
	{
		netsend_nationwarlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}

	pValue.m_op = 3;
	pValue.m_count = 0;
	netsend_nationwarlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 国家城池战争列表
int nation_city_warlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	char nation = pCity->nation;
	SLK_NetS_NationCityWarList pValue = { 0 };
	// 发送开始
	pValue.m_op = 1;
	netsend_nationcitywarlist_S( actor_index, SENDTYPE_ACTOR, &pValue );

	for ( int tmpi = 0; tmpi < NATION_CITY_WAR_MAX; tmpi++ )
	{
		int group_index = g_nation[nation].city_war_index[tmpi];
		if ( group_index < 0 || group_index >= g_armygroup_maxcount )
			continue;
		if ( g_armygroup[group_index].id <= 0 )
			continue;
		if ( g_armygroup[group_index].from_type != MAPUNIT_TYPE_CITY || g_armygroup[group_index].to_type != MAPUNIT_TYPE_CITY )
			continue;

		// 攻击方玩家
		City *pAtkCity = city_indexptr( g_armygroup[group_index].from_index );
		if ( !pAtkCity )
			continue;

		// 防御方玩家
		City *pDefCity = city_indexptr( g_armygroup[group_index].to_index );
		if ( !pDefCity )
			continue;

		// 我既不是攻击国也不是防御国
		if ( pCity->nation != pAtkCity->nation && pCity->nation != pDefCity->nation )
			continue;

		if ( pCity->nation == pAtkCity->nation )
		{ // 我属于攻击方
			pValue.m_list[pValue.m_count].m_attack = 1;

			pValue.m_list[pValue.m_count].m_nation = pAtkCity->nation;
			pValue.m_list[pValue.m_count].m_level = city_mainlevel( pAtkCity->index );
			strncpy( pValue.m_list[pValue.m_count].m_name, pAtkCity->name, NAME_SIZE );
			pValue.m_list[pValue.m_count].m_name_length = strlen( pValue.m_list[pValue.m_count].m_name );
			pValue.m_list[pValue.m_count].m_posx = pAtkCity->posx;
			pValue.m_list[pValue.m_count].m_posy = pAtkCity->posy;
			pValue.m_list[pValue.m_count].m_actorid = pAtkCity->actorid;
			pValue.m_list[pValue.m_count].m_total = armygroup_from_totals( tmpi );

			pValue.m_list[pValue.m_count].m_t_nation = pDefCity->nation;
			pValue.m_list[pValue.m_count].m_t_level = city_mainlevel( pDefCity->index );
			strncpy( pValue.m_list[pValue.m_count].m_t_name, pDefCity->name, NAME_SIZE );
			pValue.m_list[pValue.m_count].m_t_name_length = strlen( pValue.m_list[pValue.m_count].m_t_name );
			pValue.m_list[pValue.m_count].m_t_posx = pDefCity->posx;
			pValue.m_list[pValue.m_count].m_t_posy = pDefCity->posy;
			pValue.m_list[pValue.m_count].m_t_actorid = pDefCity->actorid;
			pValue.m_list[pValue.m_count].m_t_total = armygroup_to_totals( tmpi );
		}
		else
		{ // 我属于防御方
			pValue.m_list[pValue.m_count].m_attack = 2;

			pValue.m_list[pValue.m_count].m_nation = pDefCity->nation;
			pValue.m_list[pValue.m_count].m_level = city_mainlevel( pDefCity->index );
			strncpy( pValue.m_list[pValue.m_count].m_name, pDefCity->name, NAME_SIZE );
			pValue.m_list[pValue.m_count].m_name_length = strlen( pValue.m_list[pValue.m_count].m_name );
			pValue.m_list[pValue.m_count].m_posx = pDefCity->posx;
			pValue.m_list[pValue.m_count].m_posy = pDefCity->posy;
			pValue.m_list[pValue.m_count].m_actorid = pDefCity->actorid;
			pValue.m_list[pValue.m_count].m_total = armygroup_to_totals( tmpi );

			pValue.m_list[pValue.m_count].m_t_nation = pAtkCity->nation;
			pValue.m_list[pValue.m_count].m_t_level = city_mainlevel( pAtkCity->index );
			strncpy( pValue.m_list[pValue.m_count].m_t_name, pAtkCity->name, NAME_SIZE );
			pValue.m_list[pValue.m_count].m_t_name_length = strlen( pValue.m_list[pValue.m_count].m_t_name );
			pValue.m_list[pValue.m_count].m_t_posx = pAtkCity->posx;
			pValue.m_list[pValue.m_count].m_t_posy = pAtkCity->posy;
			pValue.m_list[pValue.m_count].m_t_actorid = pAtkCity->actorid;
			pValue.m_list[pValue.m_count].m_t_total = armygroup_from_totals( tmpi );
		}

		pValue.m_list[pValue.m_count].m_group_index = tmpi;
		pValue.m_list[pValue.m_count].m_group_id = g_armygroup[tmpi].id;
		pValue.m_list[pValue.m_count].m_statetime = g_armygroup[tmpi].statetime;
		pValue.m_list[pValue.m_count].m_stateduration = g_armygroup[tmpi].stateduration;
		pValue.m_list[pValue.m_count].m_type = g_armygroup[tmpi].type;
		pValue.m_list[pValue.m_count].m_unit_index = pDefCity->unit_index;

		pValue.m_count += 1;
		if ( pValue.m_count >= 12 )
		{
			pValue.m_op = 2;
			netsend_nationcitywarlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
			pValue.m_count = 0;
		}
	}

	if ( pValue.m_count > 0 )
	{
		pValue.m_op = 2;
		netsend_nationcitywarlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
		pValue.m_count = 0;
	}

	pValue.m_op = 3;
	pValue.m_count = 0;
	netsend_nationcitywarlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
