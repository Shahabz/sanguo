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
#include "chat.h"
#include "army_group.h"
#include "rank.h"
#include "world_quest.h"
#include "nation.h"
#include "nation_hero.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern City * g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern UpgradeInfo *g_upgradeinfo;
extern int g_upgradeinfo_maxnum;

extern MonsterInfo *g_monster;
extern int g_monster_maxnum;

extern ArmyGroup *g_armygroup;
extern int g_armygroup_maxcount;

extern MapTownInfo *g_towninfo;
extern int g_towninfo_maxnum;

extern MapTown *g_map_town;
extern int g_map_town_maxcount;
extern int g_map_town_maxindex;

extern char g_open_town3;
extern char g_open_town6;
extern char g_open_townking;

extern NationUpgrade *g_nation_upgrade;
extern int g_nation_upgrade_maxnum;

extern NationPlace *g_nation_place;
extern int g_nation_place_maxnum;

extern NationQuest *g_nation_quest;
extern int g_nation_quest_maxnum;

extern NationMission *g_nation_mission;
extern int g_nation_mission_maxnum;

extern NationOfficial *g_nation_official;
extern int g_nation_official_maxnum;

// 国家排名
extern ActorRank *g_rank_nation[3];
extern int g_rank_nation_count[3];

// 国家信息
Nation g_nation[NATION_MAX] = { 0 };

// 荣誉排行榜
NationRank g_nation_rank[NATION_MAX] = { 0 };

// 官员系统状态时间
char g_nation_official_state = 0;
int g_nation_official_statetime = 0;

// 国家日志
SLK_NetS_NationLog g_nation_log[3][NATION_LOG_CACHE_QUEUE_COUNT] = { 0 };

// 读档完毕的回调
int nation_loadcb( int nation )
{
	if ( nation <= 0 || nation >= NATION_MAX )
		return -1;
	if ( g_nation[nation].level < 1 )
	{
		g_nation[nation].level = 1;
	}
	// 国家任务检查
	for ( int tmpi = 0; tmpi < NATION_QUEST_MAX; tmpi++ )
	{
		if ( g_nation[nation].questlevel[tmpi] == 0 )
		{
			g_nation[nation].questlevel[tmpi] = g_nation[nation].level;
		}
		g_nation[nation].questkind[tmpi] = tmpi + 1;
	}
	// 国家荣誉任务检查
	if ( g_nation[nation].missionlevel == 0 )
	{
		g_nation[nation].missionlevel = 1;
		for ( int tmpi = 0; tmpi < NATION_MISSION_MAX; tmpi++ )
		{
			g_nation[nation].missionvalue[tmpi] = 0;
		}
	}
	// 国家荣誉排行榜
	for ( int tmpi = 0; tmpi < NATION_RANK_MAX; tmpi++ )
	{
		for ( int i = 0; i < NATION_RANK_MEMBERNUM; i++ )
		{
			g_nation_rank[nation].member[tmpi][i].actorid = 0;
			g_nation_rank[nation].member[tmpi][i].city_index = -1;
		}
	}
	// 官员索引
	for ( int tmpi = 0; tmpi < NATION_OFFICIAL_MAX; tmpi++ )
	{
		if ( g_nation[nation].official_actorid[tmpi] > 0 )
		{
			g_nation[nation].official_city_index[tmpi] = city_getindex_withactorid( g_nation[nation].official_actorid[tmpi] );
		}
	}
	// 候选人
	for ( int tmpi = 0; tmpi < NATION_CANDIDATE_MAX; tmpi++ )
	{
		if ( g_nation[nation].candidate_actorid[tmpi] > 0 )
		{
			g_nation[nation].candidate_city_index[tmpi] = city_getindex_withactorid( g_nation[nation].candidate_actorid[tmpi] );
		}
	}
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

		for ( int tmpi = 0; tmpi < NATION_CANDIDATE_MAX; tmpi++ )
		{
			g_nation[nation].candidate_city_index[tmpi] = -1;
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

// 修改国家公告
int nation_notice( int actor_index, char *pMsg )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1;
	if ( !pMsg )
		return -1;
	if ( nation_official_right( pCity->official, NATION_OFFICIAL_RIGHT_NOTIFY ) == 0 )
		return -1;
	if ( (int)time( NULL ) - pNation->notice_change_stamp < global.nation_change_notice_sec )
	{
		actor_notify_alert( actor_index, 1844 );
		return -1;
	}
	strncpy( pNation->notice, pMsg, sizeof( char )*NATION_NOTICE_MAX );
	nation_sendinfo( actor_index );

	// 滚动公告
	char v1[32] = { 0 };
	strncpy( v1, pCity->name, NAME_SIZE );
	system_talkjson( 0, pCity->nation, 6013, v1, NULL, NULL, NULL, NULL, NULL, 1 );
	return 0;
}

// 上限提醒
int nation_online_notify( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->official == 0 || pCity->official == NATION_OFFICIAL_R6 )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1;

	char v1[32] = { 0 };
	char v2[32] = { 0 };
	sprintf( v1, "%s%d", TAG_NATION, pCity->nation );
	strncpy( v2, pCity->name, NAME_SIZE );

	if ( pCity->official >= NATION_OFFICIAL_R1 && pCity->official <= NATION_OFFICIAL_R3 )
	{ // 国王上线提醒
		if ( (int)time( NULL ) - pNation->online_notify_king_stamp < global.nation_online_notify_sec )
			return -1;
		system_talkjson_world( 6014, v1, v2, NULL, NULL, NULL, NULL, 1 );
		pNation->online_notify_king_stamp = (int)time( NULL );
	}
	else if ( pCity->official == NATION_OFFICIAL_R4 )
	{ // 丞相上线提醒
		if ( (int)time( NULL ) - pNation->online_notify_r4_stamp < global.nation_online_notify_sec )
			return -1;
		system_talkjson( 0, pCity->nation, 6015, v1, v2, NULL, NULL, NULL, NULL, 1 );
		pNation->online_notify_r4_stamp = (int)time( NULL );
	}
	else if ( pCity->official >= NATION_OFFICIAL_R5 )
	{ // 军师上线提醒
		if ( (int)time( NULL ) - pNation->online_notify_r5_stamp < global.nation_online_notify_sec )
			return -1;
		system_talkjson( 0, pCity->nation, 6016, v1, v2, NULL, NULL, NULL, NULL, 1 );
		pNation->online_notify_r5_stamp = (int)time( NULL );
	}
	return 0;
}

// 国家升级
int nation_upgrade( char nation )
{
	Nation *pNation = nation_getptr( nation );
	if ( !pNation )
		return -1;
	if ( pNation->level >= g_nation_upgrade_maxnum - 1 )
		return -1;
	if ( pNation->level == 3 )
	{
		if ( g_open_townking == 0 )
		{// 需要击杀董卓后升级
			return -1;
		}
	}
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
	{ // 检查是否出现国家名将
		nation_hero_check( nation );
	}
	return 0;
}

// 天策府统领点数
int nation_tiance_point( char nation, int value )
{
	Nation *pNation = nation_getptr( nation );
	if ( !pNation )
		return -1;
	pNation->tiance_point += value;
	if ( pNation->level >= 8 )
		return -1;
	if ( pNation->tiance_point > g_upgradeinfo[pNation->level+1].tiance_point )
	{
		pNation->tiance_point -= g_upgradeinfo[pNation->level + 1].tiance_point;
		pNation->level += 1;
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

	for ( int tmpi = 0; tmpi < NATION_QUEST_MAX; tmpi++ )
	{
		char questlevel = pNation->questlevel[tmpi];
		if ( questlevel <= 0 || questlevel >= g_nation_quest_maxnum )
			continue;
		char questkind = pNation->questkind[tmpi];
		if ( questkind <= 0 || questkind >= g_nation_quest[questlevel].maxnum )
			continue;
		pValue.m_questvalue[tmpi] = pCity->nation_qv[tmpi];
		pValue.m_questvalue_max[tmpi] = g_nation_quest[questlevel].config[questkind].needvalue;
	}

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
	if ( pNation->level == 3 && pNation->exp >= g_nation_upgrade[pNation->level].config[1].maxexp )
	{
		if ( g_open_townking == 0 )
		{// 需要击杀董卓后升级
			actor_notify_alert( actor_index, 1891 );
			return -1;
		}
	}

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

	// 国家荣誉任务
	nation_mission_addvalue( pCity->nation, NATION_MISSIONKIND_BUILD, 1 );

	// 国家荣誉排行
	nation_rank_addvalue( pCity, NATION_RANK_BUILD, 1 );
	return 0;
}

// 国家爵位晋升
int nation_place_upgrade( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->place < 0 || pCity->place >= global.nation_place_max || pCity->place >= g_nation_place_maxnum )
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
		if ( g_map_town[townid].zoneid != pCity->zone )
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
		if ( g_map_town[townid].zoneid != pCity->zone )
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
		
		// 防守方不在我的地图
		if ( pCity->zone != pDefCity->zone )
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

// 国家任务
int nation_quest_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1;
	SLK_NetS_NationQuestList pValue = { 0 };
	for ( int tmpi = 0; tmpi < NATION_QUEST_MAX; tmpi++ )
	{
		char questlevel = pNation->questlevel[tmpi];
		if ( questlevel <= 0 || questlevel >= g_nation_quest_maxnum )
			continue;
		char questkind = pNation->questkind[tmpi];
		if ( questkind <= 0 || questkind >= g_nation_quest[questlevel].maxnum )
			continue;
		pValue.m_list[pValue.m_count].m_level = questlevel;
		pValue.m_list[pValue.m_count].m_kind = questkind;
		pValue.m_list[pValue.m_count].m_value = pCity->nation_qv[tmpi];
		pValue.m_list[pValue.m_count].m_needvalue = g_nation_quest[questlevel].config[questkind].needvalue;

		for ( int i = 0; i < 5; i++ )
		{
			pValue.m_list[pValue.m_count].m_awardkind[i] = g_nation_quest[questlevel].config[questkind].awardkind[i];
			pValue.m_list[pValue.m_count].m_awardnum[i] = g_nation_quest[questlevel].config[questkind].awardnum[i];
		}
		pValue.m_count += 1;
	}
	netsend_nationquestlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 刷新国家任务
int nation_quest_update()
{
	for ( int nation = 1; nation < NATION_MAX; nation++ )
	{
		for ( int tmpi = 0; tmpi < NATION_QUEST_MAX; tmpi++ )
		{
			g_nation[nation].questlevel[tmpi] = g_nation[nation].level;
			g_nation[nation].questkind[tmpi] = tmpi + 1;
		}
	}

	for ( int city_index = 0; city_index < g_city_maxindex/*注意：使用索引位置，为了效率*/; city_index++ )
	{
		if ( g_city[city_index].actorid <= 0 )
			continue;
		g_city[city_index].nation_qv[0] = 0;
		g_city[city_index].nation_qv[1] = 0;
		g_city[city_index].nation_qv[2] = 0;
	}
	return 0;
}

// 任务数值
int nation_quest_addvalue( City *pCity, char kind, int value )
{
	if ( !pCity )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1;

	char index = kind - 1;
	if ( index < 0 || index >= NATION_QUEST_MAX )
		return -1;

	char questlevel = pNation->questlevel[index];
	if ( questlevel <= 0 || questlevel >= g_nation_quest_maxnum )
		return -1;
	char questkind = pNation->questkind[index];
	if ( questkind <= 0 || questkind >= g_nation_quest[questlevel].maxnum )
		return -1;
	if ( pCity->nation_qv[index] == -1 ) // 代表已经领取奖励
		return -1;
	if ( pCity->nation_qv[index] >= g_nation_quest[questlevel].config[questkind].needvalue )
		return -1;

	pCity->nation_qv[index] += value;
	if ( pCity->nation_qv[index] >= g_nation_quest[questlevel].config[questkind].needvalue )
	{ // 完成

	}

	return 0;
}

// 领取奖励
int nation_quest_getaward( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1; 
	char index = kind - 1;
	if ( index < 0 || index >= NATION_QUEST_MAX )
		return -1;

	char questlevel = pNation->questlevel[index];
	if ( questlevel <= 0 || questlevel >= g_nation_quest_maxnum )
		return -1;
	char questkind = pNation->questkind[index];
	if ( questkind <= 0 || questkind >= g_nation_quest[questlevel].maxnum )
		return -1;
	if ( pCity->nation_qv[index] == -1 ) // 代表已经领取奖励
		return -1;
	if ( pCity->nation_qv[index] < g_nation_quest[questlevel].config[questkind].needvalue )
		return -1;

	// 给与基础奖励
	for ( int tmpi = 0; tmpi < 5; tmpi++ )
	{
		if ( g_nation_quest[questlevel].config[questkind].awardkind[tmpi] == 0 )
			continue;
		award_getaward( actor_index, g_nation_quest[questlevel].config[questkind].awardkind[tmpi], g_nation_quest[questlevel].config[questkind].awardnum[tmpi], -1, PATH_NATIONQUEST, NULL );
	}

	// 给与额外奖励
	int allvalue = 0;
	for ( int tmpi = 0; tmpi < 5; tmpi++ )
	{
		allvalue += g_nation_quest[questlevel].config[questkind].other_awardodds[tmpi];
	}
	if ( allvalue == 0 )
		return -1;

	int curvalue = 0;
	int odds = allvalue > 0 ? rand() % allvalue : 0;
	for ( int tmpi = 0; tmpi < 6; tmpi++ )
	{
		// 按照评价值方式随机
		curvalue = g_nation_quest[questlevel].config[questkind].other_awardodds[tmpi];
		if ( curvalue > 0 && curvalue > odds )
		{
			award_getaward( actor_index, g_nation_quest[questlevel].config[questkind].other_awardkind[tmpi], g_nation_quest[questlevel].config[questkind].other_awardnum[tmpi], -1, PATH_NATIONQUEST, NULL );
			// 公告
			char v1[32] = { 0 };
			strncpy( v1, pCity->name, NAME_SIZE );
			system_talkjson( 0, pCity->nation, 6021, v1, NULL, NULL, NULL, NULL, NULL, 1 );
			break;
		}
		odds -= curvalue;
	}

	pCity->nation_qv[index] = -1;
	nation_quest_sendlist( actor_index );
	nation_sendinfo( actor_index );
	return 0;
}

// 国家荣誉任务列表
int nation_mission_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1;
	char misssionlevel = pNation->missionlevel;
	if ( misssionlevel <= 0 || misssionlevel >= g_nation_mission_maxnum )
	{
		pNation->missionlevel = 1;
		misssionlevel = 1;
	}
	SLK_NetS_NationMissionList pValue = { 0 };
	for ( int tmpi = 0; tmpi < 3; tmpi++ )
	{
		char baglevel = tmpi;
		if ( baglevel < 0 || baglevel >= g_nation_mission[misssionlevel].maxnum )
			continue;

		pValue.m_list[pValue.m_count].m_value[0] = pNation->missionvalue[0];
		pValue.m_list[pValue.m_count].m_needvalue[0] = g_nation_mission[misssionlevel].config[baglevel].needvalue[0];

		pValue.m_list[pValue.m_count].m_value[1] = pNation->missionvalue[1];
		pValue.m_list[pValue.m_count].m_needvalue[1] = g_nation_mission[misssionlevel].config[baglevel].needvalue[1];

		pValue.m_list[pValue.m_count].m_value[2] = pNation->missionvalue[2];
		pValue.m_list[pValue.m_count].m_needvalue[2] = g_nation_mission[misssionlevel].config[baglevel].needvalue[2];

		for ( int i = 0; i < 2; i++ )
		{
			pValue.m_list[pValue.m_count].m_awardkind[i] = g_nation_mission[misssionlevel].config[baglevel].awardkind[i];
			pValue.m_list[pValue.m_count].m_awardnum[i] = g_nation_mission[misssionlevel].config[baglevel].awardnum[i];
		}

		pValue.m_list[pValue.m_count].m_isget = actor_get_today_char_times( actor_index, tmpi + TODAY_CHAR_NATION_MISSION_AWARD1 );
		pValue.m_count += 1;
	}
	netsend_nationmissionlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 国家荣誉任务
int nation_mission_addvalue( char nation, char kind, int value )
{
	Nation *pNation = nation_getptr( nation );
	if ( !pNation )
		return -1;
	char index = kind - 1;
	if ( index < 0 || index >= NATION_MISSION_MAX )
		return -1;	
	pNation->missionvalue[index] += value;
	return 0;
}

// 刷新国家荣誉任务
int nation_mission_update()
{
	for ( int nation = 1; nation < NATION_MAX; nation++ )
	{
		g_nation[nation].missionlevel = g_nation[nation].level;
		for ( int tmpi = 0; tmpi < NATION_MISSION_MAX; tmpi++ )
		{
			g_nation[nation].missionvalue[tmpi] = 0;
		}
	}
	return 0;
}

// 国家荣誉任务奖励
int nation_mission_getaward( int actor_index, int baglevel )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1;
	baglevel -= 1;
	if ( baglevel < 0 || baglevel >= 3 )
		return -1;
	char misssionlevel = pNation->missionlevel;
	if ( misssionlevel <= 0 || misssionlevel >= g_nation_mission_maxnum )
		return -1;

	for ( int tmpi = 0; tmpi < NATION_MISSION_MAX; tmpi++ )
	{
		if ( pNation->missionvalue[tmpi] < g_nation_mission[misssionlevel].config[baglevel].needvalue[tmpi] )
		{
			return -1;
		}
	}

	if ( actor_get_today_char_times( actor_index, baglevel + TODAY_CHAR_NATION_MISSION_AWARD1 ) > 0 )
		return -1;

	// 给与基础奖励
	for ( int tmpi = 0; tmpi < 2; tmpi++ )
	{
		if ( g_nation_mission[misssionlevel].config[baglevel].awardkind[tmpi] == 0 )
			continue;
		award_getaward( actor_index, g_nation_mission[misssionlevel].config[baglevel].awardkind[tmpi], g_nation_mission[misssionlevel].config[baglevel].awardnum[tmpi], -1, PATH_NATIONMISSION, NULL );
	}

	actor_add_today_char_times( actor_index, baglevel + TODAY_CHAR_NATION_MISSION_AWARD1 );
	nation_mission_sendlist( actor_index );
	return 0;
}

// 国家荣誉排行榜
int nation_rank_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1;
	NationRank *pRank = &g_nation_rank[pCity->nation];
	SLK_NetS_NationRankList pValue = { 0 };
	pValue.m_vote = pCity->vote;
	for ( int tmpi = 0; tmpi < NATION_RANK_MEMBERNUM; tmpi++ )
	{
		int city_index = pRank->member[NATION_RANK_BUILD - 1][tmpi].city_index;
		if ( city_index < 0 || city_index >= g_city_maxcount )
			continue;
		strncpy( pValue.m_buildrank[tmpi].m_name, g_city[city_index].name, sizeof( char )*NAME_SIZE );
		pValue.m_buildrank[tmpi].m_name_len = strlen( pValue.m_buildrank[tmpi].m_name );
		pValue.m_buildrank[tmpi].m_value = g_city[city_index].nation_rankv[0];
	}

	for ( int tmpi = 0; tmpi < NATION_RANK_MEMBERNUM; tmpi++ )
	{
		int city_index = pRank->member[NATION_RANK_CITY - 1][tmpi].city_index;
		if ( city_index < 0 || city_index >= g_city_maxcount )
			continue;
		strncpy( pValue.m_cityrank[tmpi].m_name, g_city[city_index].name, sizeof( char )*NAME_SIZE );
		pValue.m_cityrank[tmpi].m_name_len = strlen( pValue.m_cityrank[tmpi].m_name );
		pValue.m_cityrank[tmpi].m_value = g_city[city_index].nation_rankv[1];
	}

	for ( int tmpi = 0; tmpi < NATION_RANK_MEMBERNUM; tmpi++ )
	{
		int city_index = pRank->member[NATION_RANK_TOWN - 1][tmpi].city_index;
		if ( city_index < 0 || city_index >= g_city_maxcount )
			continue;
		strncpy( pValue.m_townrank[tmpi].m_name, g_city[city_index].name, sizeof( char )*NAME_SIZE );
		pValue.m_townrank[tmpi].m_name_len = strlen( pValue.m_townrank[tmpi].m_name );
		pValue.m_townrank[tmpi].m_value = g_city[city_index].nation_rankv[2];
	}
	netsend_nationranklist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 添加数值并排序
void nation_rank_addvalue( City *pCity, char kind, int value )
{
	if ( !pCity )
		return;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return;
	NationRank *pRank = &g_nation_rank[pCity->nation];
	int index = kind - 1;
	if ( index < 0 || index >= NATION_RANK_MAX )
		return;
	pCity->nation_rankv[index] += value;
	if ( pCity->nation_rankv[index] == 0 )
		return;
	int replace_tmpi = -1;
	for ( int tmpi = 0; tmpi < NATION_RANK_MEMBERNUM; tmpi++ )
	{
		int value = 0;
		int city_index = pRank->member[index][tmpi].city_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
		{
			value = g_city[city_index].nation_rankv[index];
		}

		if ( pCity->nation_rankv[index] > value )
		{
			replace_tmpi = tmpi;
			break;
		}
	}

	if ( replace_tmpi < 0 || replace_tmpi >= NATION_RANK_MEMBERNUM )
	{
		return;
	}

	// 检查我是不是已经在里面了
	int my_tmpi = -1;
	for ( int tmpi = 0; tmpi < NATION_RANK_MEMBERNUM; tmpi++ )
	{
		int actorid = pRank->member[index][tmpi].actorid;
		if ( actorid == pCity->actorid )
		{
			my_tmpi = tmpi;
			break;
		}
	}

	if ( my_tmpi >= 0 && my_tmpi < NATION_RANK_MEMBERNUM )
	{ // 在里面
		int space = my_tmpi - replace_tmpi;
		if ( space == 1 )
		{ // 直接交换
			NationRankMember tmpMember = { 0 };
			memcpy( &tmpMember, &pRank->member[index][replace_tmpi], sizeof( NationRankMember ) );
			memcpy( &pRank->member[index][replace_tmpi], &pRank->member[index][my_tmpi], sizeof( NationRankMember ) );
			memcpy( &pRank->member[index][my_tmpi], &tmpMember, sizeof( NationRankMember ) );
		}
		else if ( space > 1 )
		{ // 重新排序, 这种情况除了不太可能出现
			for ( int i = 0; i < NATION_RANK_MEMBERNUM; i++ )
			{
				for ( int j = 0; j < NATION_RANK_MEMBERNUM-i; j++ )
				{
					int city_index = pRank->member[index][j].city_index;
					int next_city_index = pRank->member[index][j+1].city_index;
					int value = 0;
					int next_value = 0;
					if ( city_index >= 0 && city_index < g_city_maxcount )
					{
						value = g_city[city_index].nation_rankv[index];
					}
					if ( next_city_index >= 0 && next_city_index < g_city_maxcount )
					{
						next_value = g_city[next_city_index].nation_rankv[index];
					}
					if ( value > next_value )
					{
						NationRankMember tmpMember = { 0 };
						memcpy( &tmpMember, &pRank->member[index][j], sizeof( NationRankMember ) );
						memcpy( &pRank->member[index][j], &pRank->member[index][j+1], sizeof( NationRankMember ) );
						memcpy( &pRank->member[index][j+1], &tmpMember, sizeof( NationRankMember ) );
					}
				}
			
			}
		}
	}
	else
	{// 不在里面
		if ( replace_tmpi < NATION_RANK_MEMBERNUM - 1 )
		{
			memmove( &pRank->member[index][replace_tmpi + 1], &pRank->member[index][replace_tmpi], sizeof( NationRankMember ) * (NATION_RANK_MEMBERNUM - 1 - replace_tmpi) );
		}
		pRank->member[index][replace_tmpi].city_index = pCity->index;
		pRank->member[index][replace_tmpi].actorid = pCity->actorid;
	}
}

// 重新计算排名
int nation_rank_calc()
{
	for ( int city_index = 0; city_index < g_city_maxindex/*注意：使用索引位置，为了效率*/; city_index++ )
	{
		if ( g_city[city_index].actorid <= 0 )
			continue;
		nation_rank_addvalue( &g_city[city_index], NATION_RANK_BUILD, 0 );
		nation_rank_addvalue( &g_city[city_index], NATION_RANK_CITY, 0 );
		nation_rank_addvalue( &g_city[city_index], NATION_RANK_TOWN, 0 );
	}
	return 0;
}

// 发奖励并且更新数据
void nation_rank_update()
{
	char v1[64] = { 0 };
	char v2[64] = { 0 };
	char v3[64] = { 0 };

	for ( int nation = 1; nation < NATION_MAX; nation++ )
	{
		NationRank *pRank = &g_nation_rank[nation];
		for ( int tmpi = 0; tmpi < NATION_RANK_MEMBERNUM; tmpi++ )
		{
			int city_index = pRank->member[NATION_RANK_BUILD - 1][tmpi].city_index;
			if ( city_index < 0 || city_index >= g_city_maxcount )
				continue;
			city_changevote( city_index, global.nation_rank_vote[tmpi], PATH_NATIONRANK );

			sprintf( v1, "%d", tmpi+1 );
			sprintf( v2, "%d", global.nation_rank_vote[tmpi] );
			sprintf( v3, "%d", g_city[city_index].vote );
			mail_system( g_city[city_index].actor_index, g_city[city_index].actorid, 5038, 5535, v1, v2, v3, "", 0 );

			pRank->member[NATION_RANK_BUILD - 1][tmpi].actorid = 0;
			pRank->member[NATION_RANK_BUILD - 1][tmpi].city_index = -1;
		}

		for ( int tmpi = 0; tmpi < NATION_RANK_MEMBERNUM; tmpi++ )
		{
			int city_index = pRank->member[NATION_RANK_CITY - 1][tmpi].city_index;
			if ( city_index < 0 || city_index >= g_city_maxcount )
				continue;
			city_changevote( city_index, global.nation_rank_vote[tmpi], PATH_NATIONRANK );

			sprintf( v1, "%d", tmpi + 1 );
			sprintf( v2, "%d", global.nation_rank_vote[tmpi] );
			sprintf( v3, "%d", g_city[city_index].vote );
			mail_system( g_city[city_index].actor_index, g_city[city_index].actorid, 5039, 5536, v1, v2, v3, "", 0 );

			pRank->member[NATION_RANK_CITY - 1][tmpi].actorid = 0;
			pRank->member[NATION_RANK_CITY - 1][tmpi].city_index = -1;
		}

		for ( int tmpi = 0; tmpi < NATION_RANK_MEMBERNUM; tmpi++ )
		{
			int city_index = pRank->member[NATION_RANK_TOWN - 1][tmpi].city_index;
			if ( city_index < 0 || city_index >= g_city_maxcount )
				continue;
			city_changevote( city_index, global.nation_rank_vote[tmpi], PATH_NATIONRANK );

			sprintf( v1, "%d", tmpi + 1 );
			sprintf( v2, "%d", global.nation_rank_vote[tmpi] );
			sprintf( v3, "%d", g_city[city_index].vote );
			mail_system( g_city[city_index].actor_index, g_city[city_index].actorid, 5040, 5537, v1, v2, v3, "", 0 );

			pRank->member[NATION_RANK_TOWN - 1][tmpi].actorid = 0;
			pRank->member[NATION_RANK_TOWN - 1][tmpi].city_index = -1;
		}
	}

	for ( int city_index = 0; city_index < g_city_maxindex/*注意：使用索引位置，为了效率*/; city_index++ )
	{
		if ( g_city[city_index].actorid <= 0 )
			continue;
		g_city[city_index].nation_rankv[0] = 0;
		g_city[city_index].nation_rankv[1] = 0;
		g_city[city_index].nation_rankv[2] = 0;
	}	
}


// 官员功能
int nation_official_open()
{
	if ( g_nation_official_statetime > 0 )
		return -1;
	// 获取时间
	time_t t;
	time( &t );
	struct tm *newtime;
	newtime = localtime( &t );
	// 生成当天24点的时间戳
	struct tm BeginTm = { 0 };
	BeginTm.tm_year = newtime->tm_year;
	BeginTm.tm_mon = newtime->tm_mon;
	BeginTm.tm_mday = newtime->tm_mday;
	BeginTm.tm_hour = 23;
	BeginTm.tm_min = 59;
	BeginTm.tm_sec = 59;
	// 重新生成时间戳
	g_nation_official_statetime = (int)mktime( &BeginTm ) + 1;
	world_data_set( WORLD_DATA_NATION_OFFICIAL_STATETIME, g_nation_official_statetime, NULL, NULL );
	return 0;
}

// 官员选举每秒逻辑
void nation_official_logic()
{
	if ( g_nation_official_statetime <= 0 )
		return;
	if ( (int)time(NULL) >= g_nation_official_statetime )
	{
		if ( g_nation_official_state == 0 )
		{ // 待首次开启->选举期
			g_nation_official_state = 1;
			g_nation_official_statetime = g_nation_official_statetime + global.nation_official_select * 3600 - 1;
			world_data_set( WORLD_DATA_NATION_OFFICIAL_STATE, g_nation_official_state, NULL, NULL );
			world_data_set( WORLD_DATA_NATION_OFFICIAL_STATETIME, g_nation_official_statetime, NULL, NULL );
			nation_official_candidate_create();
			mail_sendall( 5041, 5538, NULL, NULL, NULL, "" );
			ui_update( 0, SENDTYPE_WORLD, UI_UPDATE_NATIONOFFICIAL );
		}
		else if ( g_nation_official_state == 1 )
		{ // 选举期->任期
			g_nation_official_state = 2;
			g_nation_official_statetime = g_nation_official_statetime + global.nation_official_term * 86400 - 1;
			world_data_set( WORLD_DATA_NATION_OFFICIAL_STATE, g_nation_official_state, NULL, NULL );
			world_data_set( WORLD_DATA_NATION_OFFICIAL_STATETIME, g_nation_official_statetime, NULL, NULL );
			nation_official_create();
			mail_sendall( 5043, 5540, NULL, NULL, NULL, "" );
			ui_update( 0, SENDTYPE_WORLD, UI_UPDATE_NATIONOFFICIAL );
		}
		else if ( g_nation_official_state == 2 )
		{ // 任期->选举期
			g_nation_official_state = 1;
			g_nation_official_statetime = g_nation_official_statetime + global.nation_official_select * 3600 - 1;
			world_data_set( WORLD_DATA_NATION_OFFICIAL_STATE, g_nation_official_state, NULL, NULL );
			world_data_set( WORLD_DATA_NATION_OFFICIAL_STATETIME, g_nation_official_statetime, NULL, NULL );
			nation_official_candidate_create();
			mail_sendall( 5042, 5539, NULL, NULL, NULL, "" );
			ui_update( 0, SENDTYPE_WORLD, UI_UPDATE_NATIONOFFICIAL );
		}
	}
}

// 创建候选人列表
int nation_official_candidate_create()
{
	// 创建候选人
	for ( int nation = 0; nation < 3; nation++ )
	{
		for ( int tmpi = 0; tmpi < NATION_CANDIDATE_MAX; tmpi++ )
		{
			int city_index = g_rank_nation[nation][tmpi].city_index;
			if ( city_index < 0 || city_index >= g_city_maxcount )
				continue;
			g_nation[nation + 1].candidate_city_index[tmpi] = city_index;
			g_nation[nation + 1].candidate_actorid[tmpi] = g_city[city_index].actorid;
			g_nation[nation + 1].candidate_ballot[tmpi] = 0;
		}
	}

	// 清空官员
	for ( int nation = 0; nation < NATION_MAX; nation++ )
	{
		for ( int tmpi = 0; tmpi < NATION_OFFICIAL_MAX; tmpi++ )
		{
			g_nation[nation].official_actorid[tmpi] = 0;
			g_nation[nation].official_city_index[tmpi] = -1;
		}
	}

	// 给玩家票数，清空之前官职
	for ( int city_index = 0; city_index < g_city_maxindex/*注意：使用索引位置，为了效率*/; city_index++ )
	{
		if ( g_city[city_index].actorid <= 0 )
			continue;
		g_city[city_index].ballot = 0;
		g_city[city_index].tokenballot = 0;
		if ( g_city[city_index].level >= global.nation_vote_actorlevel )
		{
			g_city[city_index].vote = global.nation_vote_basenum;
		}

		if ( g_city[city_index].official > 0 )
		{
			g_city[city_index].official = 0;
			// 如果玩家在线，通知更新
			if ( g_city[city_index].actor_index >= 0 )
			{
				int pValue[2] = { 0 };
				pValue[0] = 4;
				pValue[1] = g_city[city_index].official;
				actor_notify_value( g_city[city_index].actor_index, NOTIFY_VALUECHANGE, 2, pValue, NULL );
			}
		}
	}
	return 0;
}

// 排序函数
int nation_official_sort( char nation )
{
	for ( int i = 0; i < NATION_CANDIDATE_MAX; i++ )
	{
		for ( int j = i; j < NATION_CANDIDATE_MAX - i; j++ )
		{
			if ( g_nation[nation].candidate_ballot[j] < g_nation[nation].candidate_ballot[j + 1] )
			{
				int temp_actorid;
				int temp_city_index;
				int temp_ballot;

				temp_actorid = g_nation[nation].candidate_actorid[j];
				temp_city_index = g_nation[nation].candidate_city_index[j];
				temp_ballot = g_nation[nation].candidate_ballot[j];

				g_nation[nation].candidate_actorid[j] = g_nation[nation].candidate_actorid[j+1];
				g_nation[nation].candidate_city_index[j] = g_nation[nation].candidate_city_index[j+1];
				g_nation[nation].candidate_ballot[j] = g_nation[nation].candidate_ballot[j+1];

				g_nation[nation].candidate_actorid[j + 1] = temp_actorid;
				g_nation[nation].candidate_city_index[j + 1] = temp_city_index;
				g_nation[nation].candidate_ballot[j + 1] = temp_ballot;
			}
		}
		
	}
	return 0;
}

// 官员创建
int nation_official_create()
{
	char v1[32] = { 0 };
	char v2[32] = { 0 };
	// 创建官员
	for ( int nation = 1; nation < NATION_MAX; nation++ )
	{
		nation_official_sort( nation );
		for ( int tmpi = 0; tmpi < NATION_CANDIDATE_MAX; tmpi++ )
		{
			int city_index = g_nation[nation].candidate_city_index[tmpi];
			if ( city_index < 0 || city_index >= g_city_maxcount )
				continue;
			g_nation[nation].official_actorid[tmpi] = g_city[city_index].actorid;
			g_nation[nation].official_city_index[tmpi] = city_index;

			if ( tmpi == 0 )
			{
				if ( nation == 1 )
					g_city[city_index].official = NATION_OFFICIAL_R1;
				else if ( nation == 2 )
					g_city[city_index].official = NATION_OFFICIAL_R2;
				else if ( nation == 3 )
					g_city[city_index].official = NATION_OFFICIAL_R3;
			}
			else if ( tmpi == 1 )
			{
				g_city[city_index].official = NATION_OFFICIAL_R4;
			}
			else if ( tmpi == 2 )
			{
				g_city[city_index].official = NATION_OFFICIAL_R5;
			}
			else
			{
				g_city[city_index].official = NATION_OFFICIAL_R6;
			}

			// 如果玩家在线，通知更新
			if ( g_city[city_index].actor_index >= 0 )
			{
				int pValue[2] = { 0 };
				pValue[0] = 4;
				pValue[1] = g_city[city_index].official;
				actor_notify_value( g_city[city_index].actor_index, NOTIFY_VALUECHANGE, 2, pValue, NULL );
			}
		}
	}

	// 发公告
	for ( int nation = 1; nation < NATION_MAX; nation++ )
	{// 优先国王
		int city_index = g_nation[nation].official_city_index[0];
		if ( city_index < 0 || city_index >= g_city_maxcount )
			continue;
		strncpy( v1, g_city[city_index].name, NAME_SIZE );
		sprintf( v2, "%s%d", TAG_NATION, nation );
		system_talkjson_world( 6017, v1, v2, NULL, NULL, NULL, NULL, 1 );
	}
	for ( int nation = 1; nation < NATION_MAX; nation++ )
	{ // 其次其他职位
		for ( int tmpi = 1; tmpi < NATION_CANDIDATE_MAX; tmpi++ )
		{
			int city_index = g_nation[nation].official_city_index[tmpi];
			if ( city_index < 0 || city_index >= g_city_maxcount )
				continue;
			if ( tmpi == 1 )
			{
				// 公告
				strncpy( v1, g_city[city_index].name, NAME_SIZE );
				system_talkjson( 0, nation, 6018, v1, NULL, NULL, NULL, NULL, NULL, 1 );
			}
			else if ( tmpi == 2 )
			{
				// 公告
				strncpy( v1, g_city[city_index].name, NAME_SIZE );
				system_talkjson( 0, nation, 6019, v1, NULL, NULL, NULL, NULL, NULL, 1 );
			}
			else
			{
				// 公告
				strncpy( v1, g_city[city_index].name, NAME_SIZE );
				system_talkjson( 0, nation, 6020, v1, NULL, NULL, NULL, NULL, NULL, 1 );
			}
		}
	}


	// 清空候选人
	for ( int nation = 0; nation < NATION_MAX; nation++ )
	{
		for ( int tmpi = 0; tmpi < NATION_CANDIDATE_MAX; tmpi++ )
		{
			g_nation[nation].candidate_city_index[tmpi] = -1;
			g_nation[nation].candidate_actorid[tmpi] = 0;
			g_nation[nation].candidate_ballot[tmpi] = 0;
		}
	}

	// 清空选票数
	for ( int city_index = 0; city_index < g_city_maxindex/*注意：使用索引位置，为了效率*/; city_index++ )
	{
		if ( g_city[city_index].actorid <= 0 )
			continue;
		g_city[city_index].vote = 0;
		g_city[city_index].ballot = 0;
		g_city[city_index].tokenballot = 0;
	}
	return 0;
}

// 投票
int nation_official_ballot( int actor_index, int target_actorid, int istoken )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1;

	if ( istoken == 0 )
	{// 非钻石拉票
		if ( pCity->vote <= 0 )
		{
			return -1;
		}
	}

	int tmpi = -1;
	for ( tmpi = 0; tmpi < NATION_CANDIDATE_MAX; tmpi++ )
	{
		if ( pNation->candidate_actorid[tmpi] == target_actorid )
		{
			break;
		}
	}
	if ( tmpi < 0 )
		return -1;

	int ballot = 0;
	if ( istoken == 0 )
	{
		ballot = pCity->vote;
		pNation->candidate_ballot[tmpi] += pCity->vote;
		city_changevote( pCity->index, -pCity->vote, PATH_NATIONBALLOT );
		pCity->ballot = 1;
	}
	else
	{
		int costtoken = int( pow( (pCity->tokenballot + 1), 1.3 ) * 5 );
		if ( actor_change_token( actor_index, -costtoken, PATH_NATIONBALLOT, 0 ) < 0 )
			return -1;
		ballot = 1;
		pNation->candidate_ballot[tmpi] += 1;
		pCity->tokenballot += 1;
		if ( pCity->tokenballot >= 30000 )
			pCity->tokenballot = 30000;
	}
	nation_official_sendlist( actor_index );

	int city_index = pNation->candidate_city_index[tmpi];
	if ( city_index >= 0 && city_index < g_city_maxcount )
	{
		char v1[32] = { 0 };
		char v2[32] = { 0 };
		sprintf( v1, "%s", g_city[city_index].name );
		sprintf( v2, "%d", ballot );
		actor_notify_pop_v( actor_index, 1823, v1, v2 );
	}
	return 0;
}

// 官员列表
int nation_official_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1;
	if ( g_nation_official_state == 0 )
	{
		SLK_NetS_NationCandidateList pValue = { 0 };
		pValue.m_count = 0;
		pValue.m_endtime = g_nation_official_statetime;
		netsend_nationcandidatelist_S( actor_index, SENDTYPE_ACTOR, &pValue );
		return -1;
	}
	
	if ( g_nation_official_state == 1 )
	{ // 选举期，发送候选人列表
		SLK_NetS_NationCandidateList pValue = { 0 };
		for ( int tmpi = 0; tmpi < NATION_CANDIDATE_MAX; tmpi++ )
		{
			int city_index = pNation->candidate_city_index[tmpi];
			if ( city_index < 0 || city_index >= g_city_maxcount )
				continue;
			pValue.m_list[pValue.m_count].m_actorid = g_city[city_index].actorid;
			pValue.m_list[pValue.m_count].m_level = g_city[city_index].level;
			pValue.m_list[pValue.m_count].m_battlepower = g_city[city_index].battlepower;
			strncpy( pValue.m_list[pValue.m_count].m_name, g_city[city_index].name, sizeof( char )*NAME_SIZE );
			pValue.m_list[pValue.m_count].m_namelen = strlen( pValue.m_list[pValue.m_count].m_name );
			pValue.m_list[pValue.m_count].m_ballot = pNation->candidate_ballot[tmpi];
			pValue.m_count += 1;
		}
		pValue.m_endtime = g_nation_official_statetime;
		pValue.m_myvote = pCity->vote;
		pValue.m_isballot = pCity->ballot;
		pValue.m_tokenballot = pCity->tokenballot;
		netsend_nationcandidatelist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}
	else if ( g_nation_official_state == 2 )
	{ // 任期，发送官员列表
		SLK_NetS_NationOfficialList pValue = { 0 };
		for ( int tmpi = 0; tmpi < NATION_OFFICIAL_MAX; tmpi++ )
		{
			int city_index = pNation->official_city_index[tmpi];
			if ( city_index < 0 || city_index >= g_city_maxcount )
				continue;
			pValue.m_list[pValue.m_count].m_official = g_city[city_index].official;
			pValue.m_list[pValue.m_count].m_level = g_city[city_index].level;
			pValue.m_list[pValue.m_count].m_battlepower = g_city[city_index].battlepower;
			strncpy( pValue.m_list[pValue.m_count].m_name, g_city[city_index].name, sizeof( char )*NAME_SIZE );
			pValue.m_list[pValue.m_count].m_namelen = strlen( pValue.m_list[pValue.m_count].m_name );
			pValue.m_list[pValue.m_count].m_zone = g_city[city_index].zone;
			pValue.m_count += 1;
		}
		pValue.m_endtime = g_nation_official_statetime;
		netsend_nationofficiallist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}
	return 0;
}

// 国家官员替换列表
int nation_official_replace_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1;
	if ( pCity->official < NATION_OFFICIAL_R1 || pCity->official > NATION_OFFICIAL_R3 )
		return -1;

	SLK_NetS_NationReplaceList pValue = { 0 };
	// 发送开始
	pValue.m_op = 1;
	netsend_nationreplacelist_S( actor_index, SENDTYPE_ACTOR, &pValue );

	// 发送过程
	pValue.m_op = 2;
	// 先发送10个官员
	for ( int tmpi = 3; tmpi < NATION_OFFICIAL_MAX; tmpi++ )
	{
		int city_index = pNation->official_city_index[tmpi];
		if ( city_index < 0 || city_index >= g_city_maxcount )
			continue;
		pValue.m_list[pValue.m_count].m_actorid = g_city[city_index].actorid;
		strncpy( pValue.m_list[pValue.m_count].m_name, g_city[city_index].name, sizeof(char)*NAME_SIZE );
		pValue.m_list[pValue.m_count].m_namelen = strlen( pValue.m_list[pValue.m_count].m_name );
		pValue.m_list[pValue.m_count].m_level = g_city[city_index].level;
		pValue.m_list[pValue.m_count].m_battlepower = g_city[city_index].battlepower;
		pValue.m_list[pValue.m_count].m_place = g_city[city_index].place;
		pValue.m_list[pValue.m_count].m_official = g_city[city_index].official;
		pValue.m_count += 1;
	}
	// 在发送30个排行榜内容
	char nation = pCity->nation - 1;
	int count = 30 > g_rank_nation_count[nation] ? g_rank_nation_count[nation] : 30;
	for ( int tmpi = 0; tmpi < count; tmpi++ )
	{
		int city_index = g_rank_nation[nation][tmpi].city_index;
		if ( city_index < 0 || city_index >= g_city_maxcount )
			continue;
		pValue.m_list[pValue.m_count].m_actorid = g_city[city_index].actorid;
		strncpy( pValue.m_list[pValue.m_count].m_name, g_city[city_index].name, sizeof( char )*NAME_SIZE );
		pValue.m_list[pValue.m_count].m_namelen = strlen( pValue.m_list[pValue.m_count].m_name );
		pValue.m_list[pValue.m_count].m_level = g_city[city_index].level;
		pValue.m_list[pValue.m_count].m_battlepower = g_city[city_index].battlepower;
		pValue.m_list[pValue.m_count].m_place = g_city[city_index].place;
		pValue.m_list[pValue.m_count].m_official = g_city[city_index].official;
		pValue.m_count += 1;
		if ( pValue.m_count >= 30 )
		{
			netsend_nationreplacelist_S( actor_index, SENDTYPE_ACTOR, &pValue );
			pValue.m_count = 0;
		}
	}
	
	if ( pValue.m_count > 0 )
	{
		netsend_nationreplacelist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}

	pValue.m_op = 3;
	pValue.m_count = 0;
	netsend_nationreplacelist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 国家官员任免
int nation_official_replace_up( int actor_index, int target_actorid )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1;
	if ( nation_official_right( pCity->official, NATION_OFFICIAL_RIGHT_DISMISSAL ) == 0 )
		return -1;

	int offset = -1;
	for ( int tmpi = 0; tmpi < NATION_OFFICIAL_MAX; tmpi++ )
	{
		if ( pNation->official_actorid[tmpi] <= 0 )
		{
			offset = tmpi;
			break;
		}
	}
	if ( offset < 0 )
	{
		actor_notify_alert( actor_index, 1843 );
		return -1;
	}

	int city_index = city_getindex_withactorid( target_actorid );
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return -1;
	
	if ( g_city[city_index].official > 0 )
		return -1;
	pNation->official_actorid[offset] = target_actorid;
	pNation->official_city_index[offset] = city_index;
	g_city[city_index].official = NATION_OFFICIAL_R6;
	// 如果玩家在线，通知更新
	if ( g_city[city_index].actor_index >= 0 )
	{
		int pValue[2] = { 0 };
		pValue[0] = 4;
		pValue[1] = g_city[city_index].official;
		actor_notify_value( g_city[city_index].actor_index, NOTIFY_VALUECHANGE, 2, pValue, NULL );
	}
	nation_official_replace_sendlist( actor_index );
	return 0;
}

// 国家官员罢免
int nation_official_replace_down( int actor_index, int target_actorid )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1;
	if ( nation_official_right( pCity->official, NATION_OFFICIAL_RIGHT_DISMISSAL ) == 0 )
		return -1;

	int offset = -1;
	for ( int tmpi = 0; tmpi < NATION_OFFICIAL_MAX; tmpi++ )
	{
		if ( pNation->official_actorid[tmpi] == target_actorid )
		{
			offset = tmpi;
			break;
		}
	}
	if ( offset < 0 )
	{
		nation_official_replace_sendlist( actor_index );
		return -1;
	}

	int city_index = city_getindex_withactorid( target_actorid );
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return -1;

	if ( g_city[city_index].official != NATION_OFFICIAL_R6 )
		return -1;
	pNation->official_actorid[offset] = 0;
	pNation->official_city_index[offset] = -1;
	g_city[city_index].official = 0;
	// 如果玩家在线，通知更新
	if ( g_city[city_index].actor_index >= 0 )
	{
		int pValue[2] = { 0 };
		pValue[0] = 4;
		pValue[1] = g_city[city_index].official;
		actor_notify_value( g_city[city_index].actor_index, NOTIFY_VALUECHANGE, 2, pValue, NULL );
	}
	nation_official_replace_sendlist( actor_index );
	return 0;
}

// 获取权限
int nation_official_right( char official, char right )
{
	if ( official <= 0 || official >=  g_nation_official_maxnum )
		return 0;
	int v = 0;
	switch ( right )
	{
	case NATION_OFFICIAL_RIGHT_DISMISSAL:
		v = g_nation_official[official].dismissal;
		break;
	case NATION_OFFICIAL_RIGHT_FIGHT:
		v = g_nation_official[official].fight;
		break;
	case NATION_OFFICIAL_RIGHT_ONLINE_NOTIFY:
		v = g_nation_official[official].online_notify;
		break;
	case NATION_OFFICIAL_RIGHT_NOTIFY:
		v = g_nation_official[official].notify;
		break;
	case NATION_OFFICIAL_RIGHT_CITYFIGHT_BODY:
		v = g_nation_official[official].cityfight_body;
		break;
	case NATION_OFFICIAL_RIGHT_CALLNUM:
		v = g_nation_official[official].callnum;
		break;
	case NATION_OFFICIAL_RIGHT_YIELD:
		v = g_nation_official[official].yield;
		break;
	case NATION_OFFICIAL_RIGHT_BUILDQUICK:
		v = g_nation_official[official].buildquick;
		break;
	case NATION_OFFICIAL_RIGHT_CALLACTORNUM:
		v = g_nation_official[official].callactornum;
		break;
	default:
		break;
	}
	return v;
}


// 添加国家日志缓存队列
int nationlog_cache_queue_add( SLK_NetS_NationLog *pCache, SLK_NetS_NationLog *pValue )
{
	int tmpi = 0;
	for ( tmpi = 0; tmpi < NATION_LOG_CACHE_QUEUE_COUNT; tmpi++ )
	{
		if ( pCache[tmpi].m_type <= 0 )
		{
			memcpy( &pCache[tmpi], pValue, sizeof( SLK_NetS_NationLog ) );
			break;
		}
	}
	if ( tmpi >= NATION_LOG_CACHE_QUEUE_COUNT )
	{
		memmove( &pCache[0], &pCache[1], sizeof( SLK_NetS_NationLog )*(NATION_LOG_CACHE_QUEUE_COUNT - 1) );
		memcpy( &pCache[NATION_LOG_CACHE_QUEUE_COUNT - 1], pValue, sizeof( SLK_NetS_NationLog ) );
	}
	return 0;
}

// 添加国家日志到数据库
int nationlog_cache_queue_add_db( char nation, SLK_NetS_NationLog *pValue )
{
	char szSQL[2048] = { 0 };
	char szText_name[NAME_SIZE * 2 + 1] = { 0 };
	db_escape( pValue->m_name, szText_name, 0 );
	sprintf( szSQL, "INSERT INTO `nation_log` ( `nation`,`type`,`townid`,`name`,`target_nation`,`optime` ) VALUES ('%d','%d','%d','%s','%d','%d')",
		nation, pValue->m_type, pValue->m_townid, szText_name, pValue->m_target_nation, pValue->m_optime );
	dbwork_addsql( szSQL, DBWORK_CMD_NORMAL, 0 );
	return 0;
}

// 读取国家日志
int nationlog_cache_loaddb( char nation, SLK_NetS_NationLog *pCache )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[1024];
	sprintf( szSQL, "SELECT `nation`,`type`,`townid`,`name`,`target_nation`,`optime` FROM nation_log WHERE nation='%d' ORDER BY optime DESC LIMIT %d;", nation, NATION_LOG_CACHE_QUEUE_COUNT );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( (row = mysql_fetch_row( res )) )
	{
		int index = 0;
		SLK_NetS_NationLog info = { 0 };
		nation = atoi( row[index++] );
		info.m_type = atoi( row[index++] );
		info.m_townid = atoi( row[index++] );
		strncpy( info.m_name, row[index++], NAME_SIZE );
		info.m_namelen = strlen( info.m_name );
		info.m_target_nation = atoi( row[index++] );
		info.m_optime = atoi( row[index++] );
		nationlog_cache_queue_add( pCache, &info );
	}
	mysql_free_result( res );
	return 0;
}
int nationlog_cache_load()
{
	nationlog_cache_loaddb( 1, g_nation_log[0] );
	nationlog_cache_loaddb( 2, g_nation_log[1] );
	nationlog_cache_loaddb( 3, g_nation_log[2] );
	return 0;
}

// 记录国家日志
void nationlog_add( char nation, int type, int townid, char *name, char target_nation )
{
	nation = nation - 1;
	if ( nation < 0 || nation >= 3 )
		return;
	SLK_NetS_NationLog info = { 0 };
	info.m_type = type;
	info.m_townid = townid;
	strncpy( info.m_name, name, NAME_SIZE );
	info.m_namelen = strlen( info.m_name );
	info.m_target_nation = target_nation;
	info.m_optime = (int)time(NULL);
	nationlog_cache_queue_add( g_nation_log[nation], &info );
	nationlog_cache_queue_add_db( nation, &info );
}

int nationlog_sendlist( int actor_index, int page )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int beginindex = page * 9;
	int endindex = beginindex + 9;
	if ( endindex > NATION_LOG_CACHE_QUEUE_COUNT )
		endindex = NATION_LOG_CACHE_QUEUE_COUNT;

	char nation = pCity->nation - 1;
	SLK_NetS_NationLogList pValue = { 0 };
	for ( int tmpi = beginindex; tmpi < endindex; tmpi++ )
	{
		if ( g_nation_log[nation][tmpi].m_type == 0 )
			continue;
		pValue.m_list[pValue.m_count].m_type = g_nation_log[nation][tmpi].m_type;
		pValue.m_list[pValue.m_count].m_townid = g_nation_log[nation][tmpi].m_townid;
		strncpy( pValue.m_list[pValue.m_count].m_name, g_nation_log[nation][tmpi].m_name, NAME_SIZE );
		pValue.m_list[pValue.m_count].m_namelen = g_nation_log[nation][tmpi].m_namelen;
		pValue.m_list[pValue.m_count].m_target_nation = g_nation_log[nation][tmpi].m_target_nation;
		pValue.m_list[pValue.m_count].m_optime = g_nation_log[nation][tmpi].m_optime;
		pValue.m_count += 1;
		if ( pValue.m_count >= 9 )
		{
			break;
		}
	}
	netsend_nationloglist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
