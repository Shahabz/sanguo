#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "db.h"
#include "define.h"
#include "city.h"
#include "building.h"
#include "actor.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "actor_times.h"
#include "award.h"
#include "server_netsend_auto.h"
#include "fight.h"
#include "quest.h"
#include "nation.h"
#include "equip.h"
#include "map_enemy.h"
#include "city_attr.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

extern UpgradeInfo *g_upgradeinfo;
extern int g_upgradeinfo_maxnum;

extern City *g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;
extern Fight g_fight;
extern QuestInfo *g_questinfo;
extern int g_questinfo_maxnum;

extern QuestTalk *g_quest_talk;
extern int g_quest_talk_maxnum;

extern TianceQuest *g_tiance_quest;
extern int g_tiance_quest_maxnum;

extern OfficialForging *g_official_forging;
extern int g_official_forging_maxnum;

inline QuestInfo *quest_config( int questid )
{
	if ( questid <= 0 || questid >= g_questinfo_maxnum )
		return NULL;
	return &g_questinfo[questid];
}

int quest_newplayer( int actor_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->questid[0] > 1 || g_actors[actor_index].quest_talkid > 0 )
		return -1;
	quest_give_main( actor_index, 1 );
	g_actors[actor_index].quest_talkid = 101;
	//quest_sendawardinfo( actor_index, 1, QUEST_TYPE_MAIN );
	//quest_getaward( actor_index, 1 );
	return 0;
}

// 给与一个主线任务
int quest_give_main( int actor_index, int questid )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return 0;
	if ( pCity->questid[0] == questid )
		return 0;
	QuestInfo *questinfo = quest_config( questid );
	if ( !questinfo )
		return 0;
	if ( questinfo->brushlevel > 0 )
	{
		map_enemy_range_brush( questinfo->brushlevel, pCity->posx, pCity->posy, questinfo->brushrange, (int)time(NULL)*600, 0 );
	}
	if ( questinfo->guide > 0 )
	{
		int value[1] = { 0 };
		value[0] = questinfo->guide;
		actor_notify_value( actor_index, NOTIFY_GUIDE, 1, value, NULL );
	}
	pCity->questid[0] = questid;
	pCity->questvalue[0] = 0;
	wlog( 0, LOGOP_QUEST, 0, 0, questid, 0, pCity->actorid, pCity->building[0].level );
	return 0;
}

// 给与一个支线任务
int quest_give_branch( int actor_index, int questid )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return 0;
	// 先检查之前是否有这个任务
	for ( int tmpi = 0; tmpi < CITY_QUEST_MAX; tmpi++ )
	{
		if ( pCity->questid[tmpi] == questid )
		{
			return 0;
		}
	}
	for ( int tmpi = 0; tmpi < CITY_QUEST_MAX; tmpi++ )
	{
		if ( pCity->questid[tmpi] <= 0 || pCity->questid[tmpi] >= g_questinfo_maxnum )
		{
			pCity->questid[tmpi] = questid;
			pCity->questvalue[tmpi] = 0;
			wlog( 0, LOGOP_QUEST, 0, 1, questid, 0, pCity->actorid, pCity->building[0].level );
			break;
		}
	}
	return 0;
}

// 添加任务数值，并主动检查任务是否完成
int quest_addvalue( City *pCity, int datatype, int datakind, int dataoffset, int value )
{
	if ( !pCity )
		return -1;
	for ( int tmpi = 0; tmpi < CITY_QUEST_MAX; tmpi++ )
	{
		int questid = pCity->questid[tmpi];
		QuestInfo *questinfo = quest_config( questid );
		if ( !questinfo )
			continue;
		if ( questinfo->datatype != datatype )
			continue;
		if ( questinfo->datatype == QUEST_DATATYPE_EQUIP_UP )
		{ // 给穿装备的任务特殊处理
			if ( questinfo->datakind > 0 && questinfo->datakind == datakind || questinfo->datakind == 0 )
			{
				if ( questinfo->dataoffset == dataoffset )
				{
					pCity->questvalue[tmpi] += value;
					if ( pCity->questvalue[tmpi] >= questinfo->needvalue )
					{
						if ( pCity->actor_index >= 0 && pCity->actor_index < g_maxactornum && tmpi == 0 )
						{ // 任务完成，通知领奖
							quest_sendawardinfo( pCity->actor_index, questid, QUEST_TYPE_MAIN );
						}
					}
				}
			}
		}
		else
		{
			if ( questinfo->datakind > 0 && questinfo->datakind == datakind || questinfo->datakind == 0 )
			{
				pCity->questvalue[tmpi] += value;
				if ( pCity->questvalue[tmpi] >= questinfo->needvalue )
				{
					if ( pCity->actor_index >= 0 && pCity->actor_index < g_maxactornum && tmpi == 0 )
					{ // 任务完成，通知领奖
						quest_sendawardinfo( pCity->actor_index, questid, QUEST_TYPE_MAIN );
					}
				}
				else
				{
					quest_sendlist( pCity->actor_index );
				}
			}
		}
	}
	return 0;
}

// 设置任务数值，并主动检查任务是否完成
int quest_setvalue( City *pCity, int datatype, int datakind, int dataoffset, int value )
{
	if ( !pCity )
		return -1;
	for ( int tmpi = 0; tmpi < CITY_QUEST_MAX; tmpi++ )
	{
		int questid = pCity->questid[tmpi];
		QuestInfo *questinfo = quest_config( questid );
		if ( !questinfo )
			continue;
		if ( questinfo->datatype != datatype )
			continue;
		
		if ( questinfo->datakind > 0 && questinfo->datakind == datakind || questinfo->datakind == 0 )
		{
			pCity->questvalue[tmpi] = value;
			if ( pCity->questvalue[tmpi] >= questinfo->needvalue )
			{
				if ( pCity->actor_index >= 0 && pCity->actor_index < g_maxactornum && tmpi == 0 )
				{ // 任务完成，通知领奖
					quest_sendawardinfo( pCity->actor_index, questid, QUEST_TYPE_MAIN );
				}
			}
		}
	}
	return 0;
}

// 添加任务数值，并主动检查任务是否完成(主线任务使用的)
int quest_main_addvalue( City *pCity, int datatype, int datakind, int dataoffset, int value )
{
	if ( !pCity )
		return 0;
	QuestInfo *questinfo = quest_config( pCity->questid[0] );
	if ( !questinfo )
		return 0;
	if ( questinfo->datatype != datatype )
		return 0;
	if ( questinfo->datakind > 0 && questinfo->datakind == datakind || questinfo->datakind == 0 )
	{
		pCity->questvalue[0] += value;
		if ( pCity->questvalue[0] >= questinfo->needvalue )
		{
			if ( pCity->actor_index >= 0 && pCity->actor_index < g_maxactornum )
			{ // 任务完成，通知领奖
				quest_sendawardinfo( pCity->actor_index, pCity->questid[0], QUEST_TYPE_MAIN );
			}
		}
	}
	return 0;
}

// 客户端发过来的操作任务完成
int quest_client_op( int actor_index, int questid, int datakind )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return 0;
	if ( datakind == -20 )
	{ // 给自动建造
		if ( actor_get_sflag( actor_index, ACTOR_SFLAG_QUEST_AUTOBUILD ) == 0 )
		{

			actor_set_sflag( actor_index, ACTOR_SFLAG_QUEST_AUTOBUILD, 1 );
		}
		return 0;
	}
	QuestInfo *questinfo = quest_config( questid );
	if ( !questinfo )
		return -1;
	if ( questinfo->datatype > 10 )
		return -1;
	quest_addvalue( pCity, questinfo->datatype, datakind, 0, 1 );
	return 0;
}

// 被动检查身上任务是否完成
int quest_checkcomplete( int actor_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return 0;
	for ( int tmpi = 0; tmpi < CITY_QUEST_MAX; tmpi++ )
	{
		int questid = pCity->questid[tmpi];
		if ( questid <= 1 )
			continue;
		if ( quest_check( pCity->actor_index, questid, NULL ) == QUEST_COMPLETEFLAG_SUCCESS )
		{
			if ( tmpi == 0 )
			{ // 主线任务
				quest_sendawardinfo( pCity->actor_index, questid, QUEST_TYPE_MAIN );
			}
			else
			{
				quest_sendlist( actor_index );
			}
			break;
		}
	}
	return 0;
}

// 任务检查
int quest_check( int actor_index, int questid, int *value )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return 0;
	QuestInfo *questinfo = quest_config( questid );
	if ( !questinfo )
		return 0;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return 0;
	// 正数索引自定义，负数索引为data_record索引
	if ( questinfo->datatype > 0 )
	{
		if ( questinfo->datatype == QUEST_DATATYPE_ACTOR_LEVEL )
		{ // 领主等级
			if ( value )
				*value = g_actors[actor_index].level;
			if ( g_actors[actor_index].level >= questinfo->needvalue )
			{
				return 1;
			}
		}
		else if ( questinfo->datatype == QUEST_DATATYPE_BUILDING_LEVEL || questinfo->datatype == QUEST_DATATYPE_BUILDING_UPGRADE )
		{ // 建筑等级
			if ( questinfo->dataoffset > 100 )
			{ // 升级资源1和2，3和4，这类的特殊处理
				int buildinglevel1 = building_getlevel( g_actors[actor_index].city_index, questinfo->datakind, (questinfo->dataoffset / 100)-1 );
				int buildinglevel2 = building_getlevel( g_actors[actor_index].city_index, questinfo->datakind, (questinfo->dataoffset % 100) - 1 );
				int minlevel = buildinglevel1;
				if ( minlevel > buildinglevel2 )
				{
					minlevel = buildinglevel2;
				}
				if ( value )
					*value = minlevel;
				if ( buildinglevel1 >= questinfo->needvalue && buildinglevel2 >= questinfo->needvalue )
				{
					return 1;
				}
			}
			else
			{
				int buildinglevel = building_getlevel( g_actors[actor_index].city_index, questinfo->datakind, questinfo->dataoffset - 1 );
				if ( value )
					*value = buildinglevel;
				if ( buildinglevel >= questinfo->needvalue )
				{
					return 1;
				}
				else if ( questinfo->datatype == QUEST_DATATYPE_BUILDING_UPGRADE )
				{
					if ( pCity->questvalue[0] >= questinfo->needvalue )
					{
						return 1;
					}
				}
			}
		}
		else if ( questinfo->datatype == QUEST_DATATYPE_BUILDING_SILVER )
		{ // 民居达N级N数量
			int buildingnum = 0;
			for ( int tmpi = 0; tmpi < BUILDING_RES_MAXNUM; tmpi++ )
			{
				if ( pCity->building_res[tmpi].kind == BUILDING_Silver && pCity->building_res[tmpi].level >= questinfo->datakind )
					buildingnum++;
			}
			if ( value )
				*value = buildingnum;
			if ( buildingnum >= questinfo->needvalue )
			{
				return 1;
			}
			if ( !value )
			{// 更新一下
				quest_sendlist( actor_index );
			}
		}
		else if ( questinfo->datatype == QUEST_DATATYPE_BUILDING_WOOD )
		{ // 伐木场达N级N数量
			int buildingnum = 0;
			for ( int tmpi = 0; tmpi < BUILDING_RES_MAXNUM; tmpi++ )
			{
				if ( pCity->building_res[tmpi].kind == BUILDING_Wood && pCity->building_res[tmpi].level >= questinfo->datakind )
					buildingnum++;
			}
			if ( value )
				*value = buildingnum;
			if ( buildingnum >= questinfo->needvalue )
			{
				return 1;
			}
			if ( !value )
			{// 更新一下
				quest_sendlist( actor_index );
			}
		}
		else if ( questinfo->datatype == QUEST_DATATYPE_BUILDING_FOOD )
		{ // 农田达N级N数量
			int buildingnum = 0;
			for ( int tmpi = 0; tmpi < BUILDING_RES_MAXNUM; tmpi++ )
			{
				if ( pCity->building_res[tmpi].kind == BUILDING_Food && pCity->building_res[tmpi].level >= questinfo->datakind )
					buildingnum++;
			}
			if ( value )
				*value = buildingnum;
			if ( buildingnum >= questinfo->needvalue )
			{
				return 1;
			}
			if ( !value )
			{// 更新一下
				quest_sendlist( actor_index );
			}
		}
		else if ( questinfo->datatype == QUEST_DATATYPE_BUILDING_IRON )
		{ // 矿场达N级N数量
			int buildingnum = 0;
			for ( int tmpi = 0; tmpi < BUILDING_RES_MAXNUM; tmpi++ )
			{
				if ( pCity->building_res[tmpi].kind == BUILDING_Iron && pCity->building_res[tmpi].level >= questinfo->datakind )
					buildingnum++;
			}
			if ( value )
				*value = buildingnum;
			if ( buildingnum >= questinfo->needvalue )
			{
				return 1;
			}
			if ( !value )
			{// 更新一下
				quest_sendlist( actor_index );
			}
		}
		else if ( questinfo->datatype == QUEST_DATATYPE_CITY_TECH || questinfo->datatype == QUEST_DATATYPE_CITY_TECHONE )
		{ // 研究N科技N级
			if ( questinfo->datakind < 0 || questinfo->datakind >= CITY_TECH_MAX )
				return 0;
			if ( value )
				*value = pCity->techlevel[questinfo->datakind];
			if ( pCity->techlevel[questinfo->datakind] >= questinfo->needvalue )
			{
				return 1;
			}
		}
		else if ( questinfo->datatype == QUEST_DATATYPE_STORY || questinfo->datatype == QUEST_DATATYPE_HERO_CALL_STORY )
		{ // 副本
			if ( value )
				*value = g_actors[actor_index].storyid;
			if ( g_actors[actor_index].storyid > questinfo->datakind )
			{
				return 1;
			}
		}
		else if ( questinfo->datatype == QUEST_DATATYPE_HERO_CALL )
		{ // 招募武将
			if ( value )
				*value = 0;
			for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
			{
				if ( pCity->hero[tmpi].kind == questinfo->datakind )
				{
					if ( value )
						*value = 1;
					return 1;
				}
			}

			for ( int tmpi = 0; tmpi < HERO_ACTOR_MAX; tmpi++ )
			{
				if ( g_actors[actor_index].hero[tmpi].kind == questinfo->datakind )
				{
					if ( value )
						*value = 1;
					return 1;
				}
			}
		}
		else if ( questinfo->datatype == QUEST_DATATYPE_EQUIP_UP )
		{ // 给N武将穿N装备
			if ( value )
				*value = 0;
			int equiptype = equip_gettype( questinfo->dataoffset ) - 1;
			if ( questinfo->datakind > 0 )
			{ // 指定武将
				for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
				{
					if ( pCity->hero[tmpi].kind == questinfo->datakind && pCity->hero[tmpi].equip[equiptype].kind == questinfo->dataoffset )
					{
						if ( value )
							*value = 1;
						return 1;
					}
				}
				for ( int tmpi = 0; tmpi < HERO_ACTOR_MAX; tmpi++ )
				{
					if ( g_actors[actor_index].hero[tmpi].kind == questinfo->datakind && g_actors[actor_index].hero[tmpi].equip[equiptype].kind == questinfo->dataoffset )
					{
						if ( value )
							*value = 1;
						return 1;
					}
				}
			}
			else
			{ // 任意武将
				for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
				{
					if ( pCity->hero[tmpi].equip[equiptype].kind == questinfo->dataoffset )
					{
						if ( value )
							*value = 1;
						return 1;
					}
				}
				for ( int tmpi = 0; tmpi < HERO_ACTOR_MAX; tmpi++ )
				{
					if ( g_actors[actor_index].hero[tmpi].equip[equiptype].kind == questinfo->dataoffset )
					{
						if ( value )
							*value = 1;
						return 1;
					}
				}
			}
		}
		else if ( questinfo->datatype == QUEST_DATATYPE_SMITHYOFFHIRE )
		{ // 招募铁匠，当前有正在工作的铁匠
			if ( value )
				*value = 0;
			int kind = pCity->ofkind[0];
			if ( kind > 0 && kind < g_official_forging_maxnum )
			{
				if ( g_official_forging[kind].level >= questinfo->needvalue )
				{
					if ( value )
						*value = 1;
					return 1;
				}
			}
		}
		else if ( questinfo->datatype == QUEST_DATATYPE_NEQUIP_FORGING )
		{ // 打造国器
			if ( value )
				*value = 0;
			int index = questinfo->datakind - 1;
			if ( index >= 0 && index < 6 )
			{
				if ( pCity->neq_sec[index] > 0 || pCity->neq_lv[index] > 0 )
				{
					if ( value )
						*value = 1;
					return 1;
				}
			}
		}
		else if ( questinfo->datatype == QUEST_DATATYPE_NEQUIP_LEVELNUM )
		{ // 升级国器
			if ( value )
				*value = 0;
			for ( int tmpi = 0; tmpi < 6; tmpi++ )
			{
				if ( pCity->neq_sec[tmpi] > 0 || pCity->neq_lv[tmpi] > 0 )
				{
					if ( value )
						*value = 1;
					return 1;
				}
			}
		}
		else if ( questinfo->datatype == QUEST_DATATYPE_EQUIP_FORGING_OP )
		{ // 打造装备的操作
			if ( value )
				*value = 0;
			if ( equip_has( actor_index, questinfo->datakind ) > 0 )
			{
				if ( value )
					*value = 1;
				return 1;
			}
			if ( equip_herohas( actor_index, questinfo->datakind ) > 0 )
			{
				if ( value )
					*value = 1;
				return 1;
			}
			if ( pCity->forgingkind == questinfo->datakind )
			{
				if ( value )
					*value = 1;
				return 1;
			}
		}
		else
		{
			for ( int tmpi = 0; tmpi < CITY_QUEST_MAX; tmpi++ )
			{
				if ( pCity->questid[tmpi] == questid )
				{
					if ( value )
						*value = pCity->questvalue[tmpi];
					if ( pCity->questvalue[tmpi] >= questinfo->needvalue )
					{
						return 1;
					}
					break;
				}
			}
		}
	}
	else if ( questinfo->datatype == 0  )
	{
		if ( value )
			*value = 1;
		return 1;
	}
	// 负数索引为data_record索引
	else if ( questinfo->datatype < 0 )
	{
		int data_record_index = -questinfo->datatype;
		int data_record_value = data_record_getvalue( pCity, data_record_index );
		if ( value )
			*value = data_record_value;
		if ( data_record_value >= questinfo->needvalue )
		{
			return 1;
		}
	}
	return 0;
}

// 领取任务奖励
int quest_getaward( int actor_index, int questid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	QuestInfo *questinfo = quest_config( questid );
	if ( !questinfo )
		return 0;
	if ( quest_check( actor_index, questid, NULL ) != QUEST_COMPLETEFLAG_SUCCESS )
		return -1;

	// 给与基础奖励
	for ( int tmpi = 0; tmpi < 5; tmpi++ )
	{
		if ( g_questinfo[questid].awardkind[tmpi] == 0 )
			continue;
		award_getaward( actor_index, g_questinfo[questid].awardkind[tmpi], g_questinfo[questid].awardnum[tmpi], -1, PATH_QUEST, NULL );
	}

	// 解锁支线任务
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		if ( questinfo->unlock_branch[tmpi] > 0 )
		{
			quest_give_branch( actor_index, questinfo->unlock_branch[tmpi] );
		}
	}

	// 触发给与建筑，英雄，功能等事件
	for ( int i = 0; i < 2; i++ )
	{
		if ( questinfo->trigger_kind[i] > 0 )
		{
			award_getaward( actor_index, questinfo->trigger_kind[i], questinfo->trigger_num[i], 0, PATH_QUEST, NULL );
		}
	}

	// 触发一个对话
	if ( questinfo->trigger_talk > 0 )
	{
		quest_talk( actor_index, questinfo->trigger_talk );
	}

	if ( questinfo->type == QUEST_TYPE_MAIN )
	{ // 给下一个主线任务
		quest_give_main( actor_index, questinfo->nextid );
	}
	else if ( questinfo->type == QUEST_TYPE_BRANCH )
	{ // 给下一个支线任务
		for ( int tmpi = 1; tmpi < CITY_QUEST_MAX; tmpi++ )
		{
			if ( pCity->questid[tmpi] == questid )
			{
				pCity->questid[tmpi] = 0;
				break;
			}
		}
		quest_give_branch( actor_index, questinfo->nextid );
	}

	// 任务
	if ( questinfo->type == QUEST_TYPE_BRANCH )
	{
		quest_addvalue( city_getptr( actor_index ), QUEST_DATATYPE_BRANCH_QUEST, 0, 0, 1 );
	}

	quest_sendlist( actor_index );
	quest_checkcomplete( actor_index );
	return 0;
}

// 任务列表
int quest_sendlist( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_QuestList pValue = { 0 };
	for ( int tmpi = 0; tmpi < CITY_QUEST_MAX; tmpi++ )
	{
		int questid = pCity->questid[tmpi];
		if ( questid <= 0 || questid >= g_questinfo_maxnum )
			continue;
		int nowvalue = 0;
		int flag = quest_check( actor_index, questid, &nowvalue );
		pValue.m_list[pValue.m_count].m_questid = questid;
		pValue.m_list[pValue.m_count].m_flag = flag;
		pValue.m_list[pValue.m_count].m_datatype = g_questinfo[questid].datatype;
		pValue.m_list[pValue.m_count].m_datakind = g_questinfo[questid].datakind;
		pValue.m_list[pValue.m_count].m_dataoffset = g_questinfo[questid].dataoffset;
		pValue.m_list[pValue.m_count].m_nameid = g_questinfo[questid].nameid;
		pValue.m_list[pValue.m_count].m_descid = g_questinfo[questid].descid;
		if ( flag == QUEST_COMPLETEFLAG_SUCCESS )
		{
			if ( g_questinfo[questid].datatype == QUEST_DATATYPE_STORY )
			{
				pValue.m_list[pValue.m_count].m_value = nowvalue;
			}
			else
			{
				pValue.m_list[pValue.m_count].m_value = g_questinfo[questid].needvalue;
			}
		}
		else
		{
			pValue.m_list[pValue.m_count].m_value = nowvalue;
		}
		pValue.m_list[pValue.m_count].m_needvalue = g_questinfo[questid].needvalue;

		for ( int i = 0; i < 5; i++ )
		{
			pValue.m_list[pValue.m_count].m_awardkind[i] = g_questinfo[questid].awardkind[i];
			pValue.m_list[pValue.m_count].m_awardnum[i] = g_questinfo[questid].awardnum[i];
		}
		pValue.m_count += 1;
	}
	netsend_questlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 发送任务奖励
int quest_sendawardinfo( int actor_index, int questid, char type )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( questid <= 0 || questid >= g_questinfo_maxnum )
		return -1;
	SLK_NetS_QuestAward pValue = { 0 };
	pValue.m_type = type;
	pValue.m_questid = questid;
	pValue.m_datatype = g_questinfo[questid].datatype;
	pValue.m_datakind = g_questinfo[questid].datakind;
	pValue.m_dataoffset = g_questinfo[questid].dataoffset;
	pValue.m_nameid = g_questinfo[questid].nameid;
	pValue.m_value = g_questinfo[questid].needvalue;
	pValue.m_needvalue = g_questinfo[questid].needvalue;
	pValue.m_count = 0;
	for ( int tmpi = 0; tmpi < 5; tmpi++ )
	{
		if ( g_questinfo[questid].awardkind[tmpi] == 0 )
			continue;
		pValue.m_list[pValue.m_count].m_kind = g_questinfo[questid].awardkind[tmpi];
		pValue.m_list[pValue.m_count].m_num = g_questinfo[questid].awardnum[tmpi];
		pValue.m_count++;
	}
	netsend_questaward_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 任务说话
int quest_talk( int actor_index, int talkid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( talkid <= 0 || talkid >= g_quest_talk_maxnum )
		return -1;
	g_actors[actor_index].quest_talkid = talkid;
	SLK_NetS_QuestTalk pValue = { 0 };
	pValue.m_talkid = talkid;
	pValue.m_op = (char)g_quest_talk[talkid].op;
	pValue.m_shape = g_quest_talk[talkid].shape;
	pValue.m_talk_textid = g_quest_talk[talkid].talk_textid;
	pValue.m_btn_textid = g_quest_talk[talkid].btn_textid;
	pValue.m_format = (char)g_quest_talk[talkid].format;
	netsend_questtalk_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 客户端点击后，获取下一条对话
int quest_talk_next( int actor_index, int talkid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( talkid <= 0 || talkid >= g_quest_talk_maxnum )
		return -1;
	if ( g_actors[actor_index].quest_talkid != talkid )
		return -1;

	// 触发给与建筑，英雄，功能等事件
	for ( int i = 0; i < 2; i++ )
	{
		if ( g_quest_talk[talkid].trigger_kind[i] > 0 )
		{
			award_getaward( actor_index, g_quest_talk[talkid].trigger_kind[i], g_quest_talk[talkid].trigger_num[i], 0, PATH_QUEST, NULL );
		}
	}

	// 点击后直接完成哪个主线任务
	if ( g_quest_talk[talkid].complete_questid > 0 )
	{
		QuestInfo *questinfo = quest_config( g_quest_talk[talkid].complete_questid );
		if ( questinfo )
		{
			quest_main_addvalue( city_getptr( actor_index ), questinfo->datatype, questinfo->datakind, questinfo->dataoffset, 1 );
		}
	}

	// 跳到下一个任务
	if ( g_quest_talk[talkid].next_questid > 0 )
	{
		quest_give_main( actor_index, g_quest_talk[talkid].next_questid );
	}

	if ( g_quest_talk[talkid].guide > 0 )
	{
		int value[1] = { 0 };
		value[0] = g_quest_talk[talkid].guide;
		actor_notify_value( actor_index, NOTIFY_GUIDE, 1, value, NULL );
	}

	g_actors[actor_index].quest_talkid = 0;
	quest_talk( actor_index, g_quest_talk[talkid].nextid );
	return 0;
}

// 任务说话检查
int quest_talk_check( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( g_actors[actor_index].quest_talkid > 0 )
	{
		quest_talk( actor_index, g_actors[actor_index].quest_talkid );
	}
	return 0;
}

// 客户端请求一个对话
int quest_talk_client_ask( int actor_index, int talkid )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( talkid <= 0 || talkid >= g_quest_talk_maxnum )
		return -1;
	if ( pCity->questid[0] != g_quest_talk[talkid].limit_questid )
		return -1;
	quest_talk( actor_index, talkid );
	return 0;
}

// 战斗
int quest_fight( int actor_index, int storyid )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	SLK_NetC_StoryBattle pValue = { 0 };
	pValue.m_storyid = storyid;
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		pValue.m_herokind[tmpi] = pCity->hero[tmpi].kind;
	}
	// 触发战斗
	if ( fight_start_bystory( actor_index, &pValue ) < 0 )
		return -1;

	// 内容
	char content[1024] = { 0 };
	
	// 播放战斗
	fight_play( actor_index, g_fight.unit_json, content );

	// 任务
	quest_main_addvalue( pCity, QUEST_DATATYPE_NEWFIGHT, 0, 0, 1 );
	return 0;
}

// 重置所有任务
int quest_gm_reset( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	memset( &pCity->questid, 0, sizeof( short )*CITY_QUEST_MAX );
	memset( &pCity->questvalue, 0, sizeof( int )*CITY_QUEST_MAX );
	quest_sendlist( actor_index );
	return 0;
}

// GM设置任务
int quest_gm_set( int actor_index, int questid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( questid <= 0 )
	{
		questid = 1;
	}
	if ( questid >= g_questinfo_maxnum )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	quest_give_main( actor_index, questid );
	quest_sendlist( actor_index );
	return 0;
}

int data_record_addvalue( struct _city *pCity, int offset, int value )
{
	if ( pCity == NULL )
		return -1;
	if ( offset < 0 || offset >= DATA_RECORD_MAXNUM )
		return -1;
	if ( pCity->data_record[offset] > INT_MAX - value )
		pCity->data_record[offset] = INT_MAX;
	else
		pCity->data_record[offset] += value;

	return 0;
}

int data_record_setvalue( struct _city *pCity, int offset, int value )
{
	if ( pCity == NULL )
		return -1;
	if ( offset < 0 || offset >= DATA_RECORD_MAXNUM )
		return -1;
	if ( value >= INT_MAX )
	{
		value = INT_MAX;
	}
	pCity->data_record[offset] = value;
	return 0;
}

int data_record_getvalue( struct _city *pCity, int offset )
{
	if ( pCity == NULL )
		return 0;
	if ( offset < 0 || offset >= DATA_RECORD_MAXNUM )
		return 0;
	return pCity->data_record[offset];
}

// 天策府任务
int tiance_quest_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_TianceQuest pValue = { 0 };
	pValue.m_tc_level[0] = pCity->tc_level[0];
	pValue.m_tc_level[1] = pCity->tc_level[1];
	pValue.m_tc_progress[0] = pCity->tc_progress[0];
	pValue.m_tc_progress[1] = pCity->tc_progress[1];
	netsend_tiancequest_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

//// 领取天策府任务
//int tiance_quest_get( int actor_index, short kind )
//{
//	ACTOR_CHECK_INDEX( actor_index );
//	City *pCity = city_getptr( actor_index );
//	if ( !pCity )
//		return -1;
//	Nation *pNation = nation_getptr( pCity->nation );
//	if ( !pNation )
//		return -1;
//	if ( kind <= 0 || kind >= g_tiance_quest_maxnum - 1 )
//		return -1;
//	// 任务已经完成
//	if ( pCity->tc_state & (1 << kind) )
//		return -1;
//
//	// 检查前一个任务是否完成
//	short prekind = g_tiance_quest[kind].prekind;
//	if ( prekind > 0 && prekind < g_tiance_quest_maxnum )
//	{
//		if ( (pCity->tc_state & (1 << prekind)) == 0 )
//			return -1;
//	}
//
//	char has = 0;
//	if ( pCity->level >= g_tiance_quest[kind].actor_level )
//	{ // 玩家等级达到条件时，可领取
//		has = 1;
//	}
//	else if ( pNation->tiance_level >= g_tiance_quest[kind].tiance_level )
//	{ // 等级不足时，根据全服的点兵统领等级领取
//		has = 1;
//	}
//	if ( has == 0 )
//	{
//		return -1;
//	}
//
//	pCity->tc_kind = (char)kind;
//	pCity->tc_num = 0;
//	tiance_quest_sendinfo( actor_index );
//	for ( int tmpi = 0; tmpi < g_tiance_quest[pCity->tc_kind].brush_enemynum; tmpi++ )
//	{
//		map_enemy_range_brush( g_tiance_quest[pCity->tc_kind].brush_enemykind, pCity->posx, pCity->posy, 4, 0, pCity->actorid );
//	}
//	
//	return 0;
//}
//
//// 添加天策府任务数值
//int tiance_quest_addnum( City *pCity )
//{
//	if ( !pCity )
//		return -1;
//	pCity->tc_num += 1;
//	return 0;
//}
//
//// 天策府任务完成
//int tiance_quest_complete( int actor_index, short kind )
//{
//	ACTOR_CHECK_INDEX( actor_index );
//	City *pCity = city_getptr( actor_index );
//	if ( !pCity )
//		return -1;
//	Nation *pNation = nation_getptr( pCity->nation );
//	if ( !pNation )
//		return -1;
//	if ( kind <= 0 || kind >= g_tiance_quest_maxnum - 1 )
//		return -1;
//	// 任务如果已经完成状态
//	if ( pCity->tc_state & (1 << kind) )
//		return -1;
//
//	// 数值没有达到条件
//	if ( pCity->tc_num < g_tiance_quest[kind].brush_enemynum )
//		return -1;
//
//	// 设为已经领取状态
//	pCity->tc_state |= (1 << kind);
//
//	// 给与点兵点数
//	nation_tiance_point( pCity->nation, g_tiance_quest[kind].brush_enemynum );
//	pCity->tc_num = 0;
//	tiance_quest_sendinfo( actor_index );
//	return 0;
//}

// 点兵科技激活
int tiance_quest_tech_activate( int actor_index, short kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind >= g_tiance_quest_maxnum )
		return -1;
	char tc_level = pCity->tc_level[kind - 1] + 1;
	if ( tc_level <= 0 || tc_level >= g_tiance_quest[kind].maxnum )
	{
		return -1;
	}
	
	TianceQuestConfig *config = &g_tiance_quest[kind].config[tc_level];
	if ( !config )
		return -1;

	// 角色等级是否满足
	if ( pCity->level < config->actor_level )
		return -1;

	// 资源是否满足
	if ( pCity->silver < config->silver )
		return -1;
	if ( pCity->wood < config->wood )
		return -1;
	if ( pCity->food < config->food )
		return -1;
	if ( pCity->iron < config->iron )
		return -1;

	// 扣减资源
	city_changesilver( pCity->index, -config->silver, PATH_TCQUEST );
	city_changewood( pCity->index, -config->wood, PATH_TCQUEST );
	city_changefood( pCity->index, -config->food, PATH_TCQUEST );
	city_changeiron( pCity->index, -config->iron, PATH_TCQUEST );

	pCity->tc_progress[kind - 1] += 1;
	if ( pCity->tc_progress[kind - 1] >= config->progress )
	{
		pCity->tc_progress[kind - 1] = 0;
		pCity->tc_level[kind - 1] += 1;
		if ( pCity->tc_level[kind - 1] >= g_tiance_quest[kind].maxnum )
		{
			pCity->tc_level[kind - 1] = g_tiance_quest[kind].maxnum - 1;
		}
	}

	tiance_quest_sendinfo( actor_index );
	city_attr_reset( pCity );
	return 0;
}
