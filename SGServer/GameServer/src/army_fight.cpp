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
#include "army_march.h"
#include "army_fight.h"
#include "city.h"
#include "map_town.h"
#include "map_enemy.h"
#include "map_res.h"
#include "map.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern HeroInfo *g_HeroInfo;
extern short g_HeroCount;

extern City * g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;

extern ItemKind *g_itemkind;
extern int g_itemkindnum;

extern MapZoneInfo *g_zoneinfo;
extern int g_zoneinfo_maxnum;

extern MapEnemyInfo *g_enemyinfo;
extern int g_enemyinfo_maxnum;

extern MapEnemy *g_map_enemy;
extern int g_map_enemy_maxcount;

extern MapRes *g_map_res;
extern int g_map_res_maxcount;

extern MapTownInfo *g_towninfo;
extern int g_towninfo_maxnum;

extern Map g_map;

extern Army *g_army;
extern int g_army_maxcount;
extern int g_army_count;
extern int g_army_maxindex;

// 与流寇战斗结果
int army_vs_enemy( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	MapMonsterConfig *config = map_monster_getconfig( g_army[army_index].target_index );
	if ( !config )
		return -1;
	if ( g_fight.result == FIGHT_WIN )
	{
		// 播放死亡动作
		map_monster_action( g_army[army_index].target_index, MONSTER_ACTION_DEAD );

		// 部队所属城池
		City *pCity = army_getcityptr( army_index );
		if ( pCity )
		{
			// 获得的奖励
			AwardGetInfo awardinfo = { 0 };
			AwardGetInfo fristawardinfo = { 0 };

			// 首杀
			char first = 0;
			if ( pCity->monster_killlevel < config->level )
			{
				pCity->monster_killlevel = config->level;
				first = 1;

				// 获取正常奖励
				awardgroup_withid( pCity->laird_actorid, config->awardgroup, PATH_FIGHT, &awardinfo );

				// 获取首杀奖励
				awardgroup_withid( pCity->laird_actorid, config->first_awardgroup, PATH_FIGHT, &fristawardinfo );

				// 通知客户端更新击杀等级
				int value[2] = { 0 };
				value[0] = 2;
				value[1] = pCity->monster_killlevel;
				actor_notify_value( pCity->actor_index, NOTIFY_WORLDMAP, 2, value, NULL );
			}
			else
			{
				// 获取正常奖励
				awardgroup_withid( pCity->laird_actorid, config->awardgroup, PATH_FIGHT, &awardinfo );
			}

			// 在夏日狂欢活动中
			if ( activity_intime( ACTIVITY_SUMMER ) == 1 )
			{
				awardgroup_withid( pCity->laird_actorid, 1791, PATH_FIGHT, &fristawardinfo );
			}

			// 发送胜利邮件
			army_mail_fightmonster( army_index, FIGHT_WIN, first, &g_fight, &awardinfo, &fristawardinfo );

			// 任务数值
			quest_addvalue( pCity, QUEST_DATAINDEX_KILLMONSTER, 0, 1 );
			quest_addvalue( pCity, QUEST_DATAINDEX_KILLMONSTER_LEVEL, config->level, 1 );

			// 联盟任务
			club_quest_addvalue( pCity->club_index, pCity, CLUB_QUESTTYPE_KILLMONSTER, config->monsterid, config->level, 1 );

			// 活跃度数值
			actor_liveness_setdata( pCity, 11, 1 );

			// 限时活动
			amatch_actor_killmonster( pCity, config->am_point );

			// 统计
			data_record_addvalue( pCity, DATA_RECORD_KILLMONSTER, 1 );
		}
	}
	else
	{
		// 被选择次数减少
		map_monster_subselected( g_army[army_index].target_index );

		// 播放待机动作
		map_monster_action( g_army[army_index].target_index, MONSTER_ACTION_IDLE );

		// 发送失败邮件
		army_mail_fightmonster( army_index, FIGHT_LOSE, 0, &g_fight, NULL, NULL );

		// 体力返还
		city_change_body( army_getcityptr( army_index ), global.fight_monster_body, PATH_FIGHT );
	}
	return 0;
}

// 与采集资源的部队战斗结果
int army_vs_res( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	int gather_army_index = map_res_getarmy( g_army[army_index].target_index );
	if ( gather_army_index < 0 || gather_army_index >= g_army_maxcount )
		return -1;

	// 防御方战场部队还原到防御方army里
	army_fightresult_toarmy( gather_army_index, 0 );
	army_mail_fightarmy( army_index, army_getcityptr( army_index ), army_getcityptr( gather_army_index ), &g_fight );

	// 联盟事件
	club_setevent_vsres( army_index, gather_army_index, g_fight.result );

	if ( g_fight.result == FIGHT_WIN )
	{
		City *pCity = army_getcityptr( gather_army_index );
		if ( pCity )
		{
			Hero *pHero = hero_getptr( pCity, g_fight.defense_hero );
			if ( pHero )
				pHero->state = HERO_STATE_DIE;
		}
		// 这里同时删除它自己的被攻击信息，之后删除就找不到索引了
		city_underfire_del_equal( army_getcityptr( gather_army_index ), gather_army_index );
		club_war_del_equal( army_getclubindex( gather_army_index ), gather_army_index );

		// 直接换成胜利者采集了
		map_res_setarmy( g_army[army_index].target_index, army_index );
		army_setstate( army_index, ARMY_STATE_GATHER );
		g_army[army_index].action = ARMY_ACTION_GATHER;

		// 检测是否全军覆没
		if ( army_fightresult_checkalldead( gather_army_index ) < 0 )
		{
			army_delete( gather_army_index );
		}
		else
		{
			army_setstate( gather_army_index, ARMY_STATE_REBACK );
		}
	}
	else
	{
		City *pCity = army_getcityptr( army_index );
		if ( pCity )
		{
			Hero *pHero = hero_getptr( pCity, g_fight.attack_hero );
			if ( pHero )
				pHero->state = HERO_STATE_DIE;
		}

		// 因为士兵会有变化，所以要重新计算资源携带量
		army_setstate( gather_army_index, ARMY_STATE_GATHER );
	}
	return 0;
}

