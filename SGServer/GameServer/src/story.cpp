#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <math.h>
#include <limits.h>
#include "db.h"
#include "global.h"
#include "actor.h"
#include "city.h"
#include "city_attr.h"
#include "city_tech.h"
#include "building.h"
#include "hero.h"
#include "equip.h"
#include "mapunit.h"
#include "map.h"
#include "server_netsend_auto.h"
#include "system.h"
#include "item.h"
#include "vip.h"
#include "actor_notify.h"
#include "story.h"
#include "fight.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern UpgradeInfo *g_upgradeinfo;
extern int g_upgradeinfo_maxnum;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;
extern Map g_map;

extern Actor *g_actors;
extern int g_maxactornum;

extern StoryInfo *g_storyinfo;
extern int g_storyinfo_maxnum;

extern MonsterInfo *g_monster;
extern int g_monster_maxnum;

extern Fight g_fight;

StoryInfo *story_getconfig( int id )
{
	if ( id <= 0 || id >= g_storyinfo_maxnum )
		return NULL;
	return &g_storyinfo[id];
}

// 发送所有副本信息
int story_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( g_actors[actor_index].storyid == 0 )
		g_actors[actor_index].storyid = 11;

	SLK_NetS_StoryList pValue = { 0 };
	pValue.m_storyid = g_actors[actor_index].storyid;
	for ( int tmpi = 0; tmpi < STORY_STAR_OFFSETMAX; tmpi++ )
	{// 精英副本星级
		pValue.m_story_star[tmpi] = g_actors[actor_index].story_star[tmpi];
	}
	for ( int tmpi = 0; tmpi < STORY_HERO_OFFSETMAX; tmpi++ )
	{// 招募副本
		pValue.m_story_hero[tmpi] = g_actors[actor_index].story_hero[tmpi];
	}
	for ( int tmpi = 0; tmpi < STORY_RESTIME_OFFSETMAX; tmpi++ )
	{// 资源副本时间
		pValue.m_story_restime[tmpi] = g_actors[actor_index].story_restime[tmpi];
	}
	for ( int tmpi = 0; tmpi < STORY_RESNUM_OFFSETMAX; tmpi++ )
	{// 资源副本次数
		pValue.m_story_resnum[tmpi] = g_actors[actor_index].story_resnum[tmpi];
	}
	for ( int tmpi = 0; tmpi < STORY_RESRESET_OFFSETMAX; tmpi++ )
	{// 资源副本重置次数
		pValue.m_story_resreset[tmpi] = g_actors[actor_index].story_resreset[tmpi];
	}
	for ( int tmpi = 0; tmpi < STORY_ITEMNUM_OFFSETMAX; tmpi++ )
	{// 资源副本重置次数
		pValue.m_story_itemnum[tmpi] = g_actors[actor_index].story_itemnum[tmpi];
	}
	for ( int tmpi = 0; tmpi < STORY_DRAWING_OFFSETMAX; tmpi++ )
	{// 装备图纸副本是否购买
		pValue.m_story_drawing[tmpi] = g_actors[actor_index].story_drawing[tmpi];
	}
	netsend_storylist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 发送关卡的配置信息
int story_sendrank( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	StoryInfo *config = story_getconfig( id );
	if ( !config )
		return -1;

	SLK_NetS_StoryRank pValue = { 0 };
	pValue.m_exp = config->exp;
	pValue.m_body = (char)config->body;
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		int monsterid = config->monsterid[tmpi];
		if ( monsterid <= 0 || monsterid >= g_monster_maxnum )
			continue;
		pValue.m_list[pValue.m_count].m_monsterid = monsterid;
		pValue.m_list[pValue.m_count].m_level = g_monster[monsterid].level;
		pValue.m_list[pValue.m_count].m_shape = g_monster[monsterid].shape;
		pValue.m_list[pValue.m_count].m_color = (char)g_monster[monsterid].color;
		pValue.m_list[pValue.m_count].m_corps = (char)g_monster[monsterid].corps;
		pValue.m_count += 1;
	}
	netsend_storyrank_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

//// 更新副本状态
//int story_sendrankstate( int actor_index, int id )
//{
//	ACTOR_CHECK_INDEX( actor_index );
//	StoryInfo *config = story_getconfig( id );
//	if ( !config )
//		return -1;
//	if ( config->saveoffset < 0 || config->saveoffset >= 512 )
//		return -1;
//	SLK_NetS_StoryState pValue = { 0 };
//	pValue.m_storyid = id;
//	pValue.m_saveoffset = config->saveoffset;
//	pValue.m_state = g_actors[actor_index].story_state[config->saveoffset];
//	netsend_storystate_S( actor_index, SENDTYPE_ACTOR, &pValue );
//	return 0;
//}
//
//// 更新副本关卡次数
//int story_sendranknum( int actor_index, int id )
//{
//	ACTOR_CHECK_INDEX( actor_index );
//	StoryInfo *config = story_getconfig( id );
//	if ( !config )
//		return -1;
//	if ( config->ranknum_saveoffset < 0 || config->ranknum_saveoffset >= 32 )
//		return -1;
//	SLK_NetS_StoryRanknum pValue = { 0 };
//	pValue.m_storyid = id;
//	pValue.m_saveoffset = config->ranknum_saveoffset;
//	pValue.m_num = g_actors[actor_index].story_ranknum[config->ranknum_saveoffset];
//	netsend_storyranknum_S( actor_index, SENDTYPE_ACTOR, &pValue );
//	return 0;
//}
//
//// 更新副本关卡时间
//int story_sendranktime( int actor_index, int id )
//{
//	ACTOR_CHECK_INDEX( actor_index );
//	StoryInfo *config = story_getconfig( id );
//	if ( !config )
//		return -1;
//	if ( config->ranksec_saveoffset < 0 || config->ranksec_saveoffset >= 32 )
//		return -1;
//	SLK_NetS_StoryRanktime pValue = { 0 };
//	pValue.m_storyid = id;
//	pValue.m_saveoffset = config->ranksec_saveoffset;
//	pValue.m_time = g_actors[actor_index].story_ranktime[config->ranksec_saveoffset];
//	netsend_storyranktime_S( actor_index, SENDTYPE_ACTOR, &pValue );
//	return 0;
//}

// 战斗
int story_battle( int actor_index, SLK_NetC_StoryBattle *pValue )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	StoryInfo *config = story_getconfig( pValue->m_storyid );
	if ( !config )
		return -1;
	if ( config->type == STORY_TYPE_NORMAL )
	{ // 普通副本打完一次就不能再打
		if ( g_actors[actor_index].storyid > pValue->m_storyid )
		{
			return -1;
		}
	}
	else if( config->type == STORY_TYPE_BOSS )
	{ // 精英副本，可以反复打

	}
	else if ( config->type == STORY_TYPE_ITEM )
	{ // 道具副本，道具次数没了，就不能打了
		int saveoffset = config->itemnum_saveoffset;
		if ( saveoffset < 0 || saveoffset >= STORY_ITEMNUM_OFFSETMAX )
			return -1;
		if ( g_actors[actor_index].story_itemnum[saveoffset] >= config->itemnum )
			return -1;
	}
	else
		return -1;

	// 触发战斗
	if ( fight_start_bystory( actor_index, pValue ) < 0 )
		return -1;

	// 播放战斗
	fight_play( actor_index, g_fight.unit_json );

	if ( g_fight.result == FIGHT_WIN )
	{ // 胜利
		// 给奖励
		AwardGetInfo getinfo = { 0 };
		city_actorexp( pCity->index, config->exp, PATH_STORY );
		city_changesilver( pCity->index, config->silver, PATH_STORY );
		awardgroup_withindex( actor_index, config->awardgroup, 0, PATH_STORY, &getinfo );

		// 当前进度的下一个副本id
		//g_actors[actor_index].storyid = config->nextid;

		//// 精英副本
		//if ( config->type == STORY_TYPE_BOSS )
		//{ // 计算战斗评分
		//	char star = 1;
		//	int lostper = (int)ceil(g_fight.attack_total_hp / (float)g_fight.attack_total_maxhp * 100);
		//	if ( lostper <= 60 )
		//		star = 3;
		//	else if ( lostper <= 80 )
		//		star = 2;

		//	g_actors[actor_index].story_state[config->saveoffset] = star;
		//}
		//else
		//{
		//	g_actors[actor_index].story_state[config->saveoffset] = 1;
		//}

		// 更新
		story_sendinfo( actor_index );
	}
	else
	{ // 失败

	}
	return 0;
}
