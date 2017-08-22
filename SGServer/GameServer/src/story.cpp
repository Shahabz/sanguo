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

// 一个副本的下一个副本id 1, 7, 下一个就是2，1
int story_nextid( int chapter, int rank )
{
	int next_chapter = 0;
	int next_rank = 0;
	int nextid = 0;
	if ( chapter < 0 || chapter >= g_storyinfo_maxnum )
		return -1;
	if ( rank < 0 || rank >= g_storyinfo[chapter].maxnum )
		return -1;
	if ( chapter == 0 && rank == 0 )
	{
		nextid = 11;
		return nextid;
	}
	if ( rank >= g_storyinfo[chapter].maxnum-1 )
	{ // 最后一个关卡
		if ( chapter < g_storyinfo_maxnum - 1 )
		{
			next_chapter = chapter + 1;
			next_rank = 1;
		}
		else
		{
			next_chapter = g_storyinfo_maxnum - 1;
			next_rank = g_storyinfo[next_chapter].maxnum - 1;
		}
	}
	else
	{
		next_chapter = chapter;
		next_rank = rank + 1;
	}
	nextid = next_chapter * 10 + next_rank;
	return nextid;
}

StoryInfoConfig *story_getconfig( int id )
{
	int chapter = id / 10;
	int rank = id % 10;
	if ( chapter <= 0 || chapter >= g_storyinfo_maxnum )
		return NULL;
	if ( rank <= 0 || rank >= g_storyinfo[chapter].maxnum )
		return NULL;
	return &g_storyinfo[chapter].config[rank];
}

// 发送所有副本信息
int story_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	SLK_NetS_StoryList pValue = { 0 };
	pValue.m_story_chapter = g_actors[actor_index].story_chapter;
	pValue.m_story_rank = g_actors[actor_index].story_rank;
	for ( int tmpi = 0; tmpi < 512; tmpi++ )
	{
		pValue.m_rankstate[tmpi] = g_actors[actor_index].story_state[tmpi];
	}

	for ( int tmpi = 0; tmpi < 32; tmpi++ )
	{
		pValue.m_ranknum[tmpi] = g_actors[actor_index].story_ranknum[tmpi];
	}

	for ( int tmpi = 0; tmpi < 32; tmpi++ )
	{
		pValue.m_ranktime[tmpi] = g_actors[actor_index].story_ranktime[tmpi];
	}
	netsend_storylist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 发送关卡的配置信息
int story_sendrank( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	StoryInfoConfig *config = story_getconfig( id );
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

// 更新副本状态
int story_sendrankstate( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	StoryInfoConfig *config = story_getconfig( id );
	if ( !config )
		return -1;
	if ( config->saveoffset < 0 || config->saveoffset >= 512 )
		return -1;
	SLK_NetS_StoryState pValue = { 0 };
	pValue.m_storyid = id;
	pValue.m_saveoffset = config->saveoffset;
	pValue.m_state = g_actors[actor_index].story_state[config->saveoffset];
	netsend_storystate_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 更新副本关卡次数
int story_sendranknum( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	StoryInfoConfig *config = story_getconfig( id );
	if ( !config )
		return -1;
	if ( config->ranknum_saveoffset < 0 || config->ranknum_saveoffset >= 32 )
		return -1;
	SLK_NetS_StoryRanknum pValue = { 0 };
	pValue.m_storyid = id;
	pValue.m_saveoffset = config->ranknum_saveoffset;
	pValue.m_num = g_actors[actor_index].story_ranknum[config->ranknum_saveoffset];
	netsend_storyranknum_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 更新副本关卡时间
int story_sendranktime( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	StoryInfoConfig *config = story_getconfig( id );
	if ( !config )
		return -1;
	if ( config->ranksec_saveoffset < 0 || config->ranksec_saveoffset >= 32 )
		return -1;
	SLK_NetS_StoryRanktime pValue = { 0 };
	pValue.m_storyid = id;
	pValue.m_saveoffset = config->ranksec_saveoffset;
	pValue.m_time = g_actors[actor_index].story_ranktime[config->ranksec_saveoffset];
	netsend_storyranktime_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 战斗
int story_battle( int actor_index, SLK_NetC_StoryBattle *pValue )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int chapter = pValue->m_storyid / 10;
	int rank = pValue->m_storyid % 10;
	StoryInfoConfig *config = story_getconfig( pValue->m_storyid );
	if ( !config )
		return -1;
	if ( config->saveoffset < 0 || config->saveoffset >= 512 )
		return -1;

	// 当前进度的下一个副本id
	int nextid = story_nextid( g_actors[actor_index].story_chapter, g_actors[actor_index].story_rank );

	if ( config->type == STORY_TYPE_NORMAL )
	{ // 缺省副本打完一次就不能再打
		if ( g_actors[actor_index].story_state[config->saveoffset] > 0 )
		{
			return -1;
		}
		int nextid = story_nextid( g_actors[actor_index].story_chapter, g_actors[actor_index].story_rank );
		if ( nextid != pValue->m_storyid )
			return -1;
	}

	if ( fight_start_bystory( actor_index, pValue, chapter, rank ) < 0 )
		return -1;
	if ( g_fight.result == FIGHT_WIN )
	{ // 胜利
		// 给奖励
		AwardGetInfo getinfo = { 0 };
		city_actorexp( pCity->index, config->exp, PATH_STORY );
		city_changesilver( pCity->index, config->silver, PATH_STORY );
		awardgroup_withindex( actor_index, config->awardgroup, 0, PATH_STORY, &getinfo );

		if ( chapter > g_actors[actor_index].story_chapter )
		{ // 打的是新章节
			g_actors[actor_index].story_chapter = chapter;
			g_actors[actor_index].story_rank = 1;
		}
		else if ( chapter == g_actors[actor_index].story_chapter )
		{ // 打的是新关卡
			if ( rank > g_actors[actor_index].story_rank )
				g_actors[actor_index].story_rank = rank;
		}

		// 精英副本
		if ( config->type == STORY_TYPE_BOSS )
		{ // 计算战斗评分
			char star = 1;
			int lostper = (int)ceil(g_fight.attack_total_hp / (float)g_fight.attack_total_maxhp * 100);
			if ( lostper <= 60 )
				star = 3;
			else if ( lostper <= 80 )
				star = 2;

			g_actors[actor_index].story_state[config->saveoffset] = star;
		}
		else
		{
			g_actors[actor_index].story_state[config->saveoffset] = 1;
		}

		// 更新关卡状态
		story_sendrankstate( actor_index, pValue->m_storyid );
	}
	else
	{ // 失败

	}
	return 0;
}
