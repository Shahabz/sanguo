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
#include "chat.h"
#include "server_netsend_auto.h"
#include "mapunit.h"
#include "fight.h"
#include "actor_notify.h"
#include "item.h"
#include "award.h"
#include "global.h"
#include "quest.h"
#include "world_quest.h"
#include "activity.h"
#include "system.h"
#include "army.h"
#include "army_march.h"
#include "army_fight.h"
#include "city.h"
#include "map_town.h"
#include "map_enemy.h"
#include "map_res.h"
#include "map_activity.h"
#include "map.h"
#include "mail.h"
#include "nation.h"
#include "nation_hero.h"
#include "activity_04.h"
#include "auto_data_hero.h"
#include "auto_data_item.h"
#include "auto_data_map_enemyinfo.h"
#include "auto_data_map_towninfo.h"
#include "auto_data_map_zoneinfo.h"
#include "auto_data_activity_12.h"

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

extern NationHeroInfo *g_nation_heroinfo;
extern int g_nation_heroinfo_maxnum;

extern NationHero *g_nation_hero;
extern int g_nation_hero_maxcount;

extern Map g_map;

extern Army *g_army;
extern int g_army_maxcount;
extern int g_army_count;
extern int g_army_maxindex;

extern ActivityInfo12 *g_activity_12;
extern int g_activity_12_maxnum;

extern MapActivity *g_map_activity;
extern int g_map_activity_maxcount;

// 与流寇战斗结果
int army_vs_enemy( int army_index, Fight *pFight )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	MapEnemy *enemy = map_enemy_getptr( g_army[army_index].to_index );
	if ( !enemy )
		return -1;
	MapEnemyInfo *config = map_enemy_getconfig( enemy->kind );
	if ( !config )
		return -1;
	City *pCity = army_getcityptr( army_index );
	if ( !pCity )
		return -1;
	i64 mailid = 0;

	// 玩家胜利
	if ( pFight->result == FIGHT_WIN )
	{
		// 获得的奖励
		AwardGetInfo awardinfo = { 0 };

		// 首杀
		char first = 0;
		if ( pCity->mokilllv < config->level )
		{
			pCity->mokilllv = (char)config->level;
			first = 1;

			// 获取首杀奖励
			awardgroup_withid( pCity->actorid, config->first_awardgroup, PATH_ENEMY, &awardinfo );

			// 通知客户端更新击杀等级
			int value[2] = { 0 };
			value[0] = 2;
			value[1] = pCity->mokilllv;
			actor_notify_value( pCity->actor_index, NOTIFY_WORLDMAP, 2, value, NULL );
		}
		else
		{
			// 获取正常奖励
			awardgroup_withid( pCity->actorid, config->awardgroup, PATH_ENEMY, &awardinfo );
		}

		if ( config->type == 1 )
		{ // 点兵
			if ( enemy->actorid > 0 )
			{
				int city_index = city_getindex_withactorid( enemy->actorid );
				if ( city_index >= 0 && city_index < g_city_maxcount )
				{
					if ( g_city[city_index].nation == pCity->nation )
					{ // 本国的
						//tiance_quest_addnum( &g_city[city_index] );
						awardinfo.kind[awardinfo.count] = AWARDKIND_TIANCE_POINT;
						awardinfo.num[awardinfo.count] = 1;
						awardinfo.count += 1;
					}
					else
					{ // 他国的
						//tiance_quest_addnum( &g_city[city_index] );
						if ( g_city[city_index].nation == 1 )
							awardinfo.kind[awardinfo.count] = AWARDKIND_TIANCE_POINT_1;
						else if ( g_city[city_index].nation == 2 )
							awardinfo.kind[awardinfo.count] = AWARDKIND_TIANCE_POINT_2;
						else if ( g_city[city_index].nation == 3 )
							awardinfo.kind[awardinfo.count] = AWARDKIND_TIANCE_POINT_3;
						awardinfo.num[awardinfo.count] = 1;
						awardinfo.count += 1;
					}
				}
			}
		}

		// 发送胜利邮件
		char title[MAIL_TITLE_MAXSIZE] = { 0 };
		sprintf( title, "%s%d", TAG_TEXTID, 5002 );

		// 奖励展示不是附件的
		char attach[MAIL_ATTACH_MAXSIZE] = { 0 };
		if ( awardinfo.count > 0 )
		{
			for ( int tmpi = 0; tmpi < awardinfo.count; tmpi++ )
			{
				if ( awardinfo.kind[tmpi] <= 0 )
					continue;
				char tempitem[32] = { 0 };
				sprintf( tempitem, "%d,%d@", awardinfo.kind[tmpi], awardinfo.num[tmpi] );
				strcat( attach, tempitem );
			}
		}

		// 内容
		char content[MAIL_CONTENT_MAXSIZE] = { 0 };
		sprintf( content, "{\"text\":\"%s%d\",\"win\":1,\"name\":\"%s\",\"kind\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tpos\":\"%d,%d\",\"ws0\":%d,\"ws1\":%d,\"ws2\":%d,\"award\":\"%s\"}",
			TAG_TEXTID, 5502, pCity->name, config->kind, config->level, pCity->posx, pCity->posy, enemy->posx, enemy->posy, pCity->temp_wounded_soldiers[0], pCity->temp_wounded_soldiers[1], pCity->temp_wounded_soldiers[2], attach );

		mailid = mail( pCity->actor_index, pCity->actorid, MAIL_TYPE_FIGHT_ENEMY, title, content, "", 0, 0 );

		// 任务数值
		quest_addvalue( pCity, QUEST_DATATYPE_KILLMONSTER, 0, 0, 1 );
		quest_addvalue( pCity, QUEST_DATATYPE_KILLMONSTER_LEVEL, config->level, 0, 1 );

		// 数据统计
		data_record_addvalue( pCity, DATA_RECORD_KILLENEMY, 1 );
		worldquest_checkcomplete( pCity->actor_index, 0 );

		// 国家任务
		nation_quest_addvalue( pCity, NATION_QUESTKIND_ENEMY, 1 );

		// 如果是国家封地范围,添加开发经验
		short range_townid = map_tile_gettownid( enemy->posx, enemy->posy );
		if ( range_townid > 0 && range_townid < g_towninfo_maxnum )
		{
			if ( g_towninfo[range_townid].type == MAPUNIT_TYPE_TOWN_GJFD )
			{
				map_town_dev_addexp( range_townid, global.town_dev_killenemy );
			}
		}

		// 七日狂欢
		if ( config->level >= 6 && config->level <= 10 )
		{
			activity_04_addvalue( pCity->index, ACTIVITY_SEVENDAY_TYPE13, 1 );
		}

		// 每日
		everyday_quest_addvalue( pCity, 20, 1 );

		// 删除流寇
		map_enemy_delete( g_army[army_index].to_index );
	}
	else
	{
		// 被选择次数减少
		map_enemy_subselected( g_army[army_index].to_index );

		// 发送失败邮件
		char title[MAIL_TITLE_MAXSIZE] = { 0 };
		sprintf( title, "%s%d", TAG_TEXTID, 5003 );

		// 内容
		char content[MAIL_CONTENT_MAXSIZE] = { 0 };
		sprintf( content, "{\"text\":\"%s%d\",\"win\":0,\"name\":\"%s\",\"kind\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tpos\":\"%d,%d\",\"ws0\":%d,\"ws1\":%d,\"ws2\":%d}",
			TAG_TEXTID, 5503, pCity->name, config->kind, config->level, pCity->posx, pCity->posy, enemy->posx, enemy->posy, pCity->temp_wounded_soldiers[0], pCity->temp_wounded_soldiers[1], pCity->temp_wounded_soldiers[2] );

		mailid = mail( pCity->actor_index, pCity->actorid, MAIL_TYPE_FIGHT_ENEMY, title, content, "", 0, 0 );
	}

	// 插入战斗详情邮件
	if ( mailid > 0 )
	{
		mail_fight( mailid, pCity->actorid, pFight->unit_json );
	}
	return 0;
}

// 与采集资源的部队战斗结果
int army_vs_res( int army_index, Fight *pFight )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	int gather_army_index = map_res_getarmy( g_army[army_index].to_index );
	if ( gather_army_index < 0 || gather_army_index >= g_army_maxcount )
		return -1;

	City *pCity = army_getcityptr( army_index );
	if ( !pCity )
		return -1;

	City *pTargetCity = army_getcityptr( gather_army_index );
	if ( !pTargetCity )
		return -1;

	i64 mailid = 0;
	char title[MAIL_TITLE_MAXSIZE] = { 0 };
	char content[MAIL_CONTENT_MAXSIZE] = { 0 };

	if ( pFight->result == FIGHT_WIN )
	{
		// 结算完毕，位置不能换
		army_gather_calc( gather_army_index );

		// 直接换成胜利者采集了
		map_res_setarmy( g_army[army_index].to_index, army_index );
		army_setstate( army_index, ARMY_STATE_GATHER );
		g_army[army_index].action = ARMY_ACTION_GATHER;

		// 失败者返回
		g_army[gather_army_index].reback = ARMY_REBACK_FIGHTLOSE;
		army_setstate( gather_army_index, ARMY_STATE_REBACK );
		
		// 进攻成功邮件
		sprintf( title, "%s%d", TAG_TEXTID, 5015 );// 攻打采集点胜利
		sprintf( content, "{\"my\":1,\"win\":1,\"na\":\"%s\",\"n\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tna\":\"%s\",\"tn\":%d,\"tlv\":%d,\"tpos\":\"%d,%d\",\"ws0\":%d,\"ws1\":%d,\"ws2\":%d}",
			pCity->name, pCity->nation, pCity->level, pCity->posx, pCity->posy, pTargetCity->name, pTargetCity->nation, pTargetCity->level, g_army[army_index].to_posx, g_army[army_index].to_posy, pCity->temp_wounded_soldiers[0], pCity->temp_wounded_soldiers[1], pCity->temp_wounded_soldiers[2] );

		mailid = mail( pCity->actor_index, pCity->actorid, MAIL_TYPE_GATHER_FIGHT, title, content, "", 0, 0 );
		mail_fight( mailid, pCity->actorid, pFight->unit_json );

		// 防守失败邮件
		sprintf( title, "%s%d", TAG_TEXTID, 5018 );// 防守采集点失败
		sprintf( content, "{\"my\":2,\"win\":0,\"na\":\"%s\",\"n\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tna\":\"%s\",\"tn\":%d,\"tlv\":%d,\"tpos\":\"%d,%d\",\"ws0\":%d,\"ws1\":%d,\"ws2\":%d}",
			pTargetCity->name, pTargetCity->nation, pTargetCity->level, g_army[army_index].to_posx, g_army[army_index].to_posy, pCity->name, pCity->nation, pCity->level, pCity->posx, pCity->posy, pTargetCity->temp_wounded_soldiers[0], pTargetCity->temp_wounded_soldiers[1], pTargetCity->temp_wounded_soldiers[2] );

		mailid = mail( pTargetCity->actor_index, pTargetCity->actorid, MAIL_TYPE_GATHER_FIGHT, title, content, "", 0, 0 );
		mail_fight( mailid, pTargetCity->actorid, pFight->unit_json );
	}
	else
	{
		// 进攻失败邮件
		sprintf( title, "%s%d", TAG_TEXTID, 5016 ); // 攻打采集点失败
		sprintf( content, "{\"my\":1,\"win\":0,\"na\":\"%s\",\"n\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tna\":\"%s\",\"tn\":%d,\"tlv\":%d,\"tpos\":\"%d,%d\",\"ws0\":%d,\"ws1\":%d,\"ws2\":%d}",
			pCity->name, pCity->nation, pCity->level, pCity->posx, pCity->posy, pTargetCity->name, pTargetCity->nation, pTargetCity->level, g_army[army_index].to_posx, g_army[army_index].to_posy, pCity->temp_wounded_soldiers[0], pCity->temp_wounded_soldiers[1], pCity->temp_wounded_soldiers[2] );

		mailid = mail( pCity->actor_index, pCity->actorid, MAIL_TYPE_GATHER_FIGHT, title, content, "", 0, 0 );
		mail_fight( mailid, pCity->actorid, pFight->unit_json );

		// 防守成功邮件
		sprintf( title, "%s%d", TAG_TEXTID, 5017 );// 防守采集点胜利
		sprintf( content, "{\"my\":2,\"win\":1,\"na\":\"%s\",\"n\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tna\":\"%s\",\"tn\":%d,\"tlv\":%d,\"tpos\":\"%d,%d\",\"ws0\":%d,\"ws1\":%d,\"ws2\":%d}",
			pTargetCity->name, pTargetCity->nation, pTargetCity->level, g_army[army_index].to_posx, g_army[army_index].to_posy, pCity->name, pCity->nation, pCity->level, pCity->posx, pCity->posy, pTargetCity->temp_wounded_soldiers[0], pTargetCity->temp_wounded_soldiers[1], pTargetCity->temp_wounded_soldiers[2] );

		mailid = mail( pTargetCity->actor_index, pTargetCity->actorid, MAIL_TYPE_GATHER_FIGHT, title, content, "", 0, 0 );
		mail_fight( mailid, pTargetCity->actorid, pFight->unit_json );
	}
	return 0;
}

// 与活动怪的部队战斗结果
int army_vs_activity( int army_index, Fight *pFight )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	MapActivity *activity = map_activity_getptr( g_army[army_index].to_index );
	if ( !activity )
		return -1;
	MapActivityInfo *config = map_activity_getconfig( activity->kind );
	if ( !config )
		return -1;
	City *pCity = army_getcityptr( army_index );
	if ( !pCity )
		return -1;
	i64 mailid = 0;

	// 玩家胜利
	if ( pFight->result == FIGHT_WIN )
	{
		// 获得的奖励
		AwardGetInfo awardinfo = { 0 };
		awardgroup_withid( pCity->actorid, config->awardgroup, PATH_ACTIVITY_MONSTER, &awardinfo );

		// 发送胜利邮件
		char title[MAIL_TITLE_MAXSIZE] = { 0 };
		sprintf( title, "%s%d", TAG_TEXTID, 5052 );

		// 奖励展示不是附件的
		char attach[MAIL_ATTACH_MAXSIZE] = { 0 };
		if ( awardinfo.count > 0 )
		{
			for ( int tmpi = 0; tmpi < awardinfo.count; tmpi++ )
			{
				if ( awardinfo.kind[tmpi] <= 0 )
					continue;
				char tempitem[32] = { 0 };
				sprintf( tempitem, "%d,%d@", awardinfo.kind[tmpi], awardinfo.num[tmpi] );
				strcat( attach, tempitem );
			}
		}

		// 内容
		char content[MAIL_CONTENT_MAXSIZE] = { 0 };
		sprintf( content, "{\"text\":\"%s%d\",\"win\":1,\"name\":\"%s\",\"kind\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tpos\":\"%d,%d\",\"ws0\":%d,\"ws1\":%d,\"ws2\":%d,\"award\":\"%s\"}",
			TAG_TEXTID, 5502, pCity->name, config->kind, config->level, pCity->posx, pCity->posy, activity->posx, activity->posy, pCity->temp_wounded_soldiers[0], pCity->temp_wounded_soldiers[1], pCity->temp_wounded_soldiers[2], attach );

		mailid = mail( pCity->actor_index, pCity->actorid, MAIL_TYPE_FIGHT_ACTIVITY, title, content, "", 0, 0 );

		// 删除活动怪
		map_activity_delete( g_army[army_index].to_index );
	}
	else
	{
		// 发送失败邮件
		char title[MAIL_TITLE_MAXSIZE] = { 0 };
		sprintf( title, "%s%d", TAG_TEXTID, 5053 );

		// 内容
		char content[MAIL_CONTENT_MAXSIZE] = { 0 };
		sprintf( content, "{\"text\":\"%s%d\",\"win\":0,\"name\":\"%s\",\"kind\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tpos\":\"%d,%d\",\"ws0\":%d,\"ws1\":%d,\"ws2\":%d}",
			TAG_TEXTID, 5503, pCity->name, config->kind, config->level, pCity->posx, pCity->posy, activity->posx, activity->posy, pCity->temp_wounded_soldiers[0], pCity->temp_wounded_soldiers[1], pCity->temp_wounded_soldiers[2] );

		mailid = mail( pCity->actor_index, pCity->actorid, MAIL_TYPE_FIGHT_ACTIVITY, title, content, "", 0, 0 );
	}

	// 插入战斗详情邮件
	if ( mailid > 0 )
	{
		mail_fight( mailid, pCity->actorid, pFight->unit_json );
	}
	return 0;
}

// 与国家名将战斗结果
int army_vs_nationhero( int army_index, Fight *pFight )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	NationHero *nhero = nation_hero_getptr( g_army[army_index].to_index );
	if ( !nhero )
		return -1;
	NationHeroInfo *config = nation_hero_getconfig( nhero->kind );
	if ( !config )
		return -1;
	City *pCity = army_getcityptr( army_index );
	if ( !pCity )
		return -1;
	i64 mailid = 0;

	// 玩家胜利
	if ( pFight->result == FIGHT_WIN )
	{
		// 获得的奖励
		AwardGetInfo awardinfo = { 0 };
		awardgroup_withid( pCity->actorid, config->awardgroup, PATH_NATIONHERO, &awardinfo );

		// 发送胜利邮件
		char title[MAIL_TITLE_MAXSIZE] = { 0 };
		sprintf( title, "%s%d", TAG_TEXTID, 5044 );

		// 奖励展示不是附件的
		char attach[MAIL_ATTACH_MAXSIZE] = { 0 };
		if ( awardinfo.count > 0 )
		{
			for ( int tmpi = 0; tmpi < awardinfo.count; tmpi++ )
			{
				if ( awardinfo.kind[tmpi] <= 0 )
					continue;
				char tempitem[32] = { 0 };
				sprintf( tempitem, "%d,%d@", awardinfo.kind[tmpi], awardinfo.num[tmpi] );
				strcat( attach, tempitem );
			}
		}

		// 内容
		char content[MAIL_CONTENT_MAXSIZE] = { 0 };
		sprintf( content, "{\"text\":\"%s%d\",\"win\":1,\"name\":\"%s\",\"kind\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tpos\":\"%d,%d\",\"ws0\":%d,\"ws1\":%d,\"ws2\":%d,\"award\":\"%s\"}",
			TAG_TEXTID, 5502, pCity->name, nhero->kind, nhero->level, pCity->posx, pCity->posy, nhero->posx, nhero->posy, pCity->temp_wounded_soldiers[0], pCity->temp_wounded_soldiers[1], pCity->temp_wounded_soldiers[2], attach );

		mailid = mail( pCity->actor_index, pCity->actorid, MAIL_TYPE_FIGHT_NATIONHERO, title, content, "", 0, 0 );

		char v1[32] = { 0 };
		char v2[32] = { 0 };
		sprintf( v1, "%s%d", TAG_HERO, config->herokind );
		strncpy( v2, pCity->name, NAME_SIZE );

		// 抓捕几率=（1-等级/100）* 系数
		float v = 0.0f;
		if ( config->nation == pCity->nation )
		{
			v = config->catch_odds;
		}
		else
		{
			v = config->other_catch_odds;
		}

		char success = 0;
		int odds = (int)(((1.0f - (float)nhero->level / 100.0f) * v) * 100);
		int rm = rand() % 100;
		if ( rm <= odds )
		{ // 抓捕成功
			success = 1;
			if ( nation_hero_catch( pCity, config->herokind ) < 0 ) // 此步骤有可能失败
				success = 0;
		}
		else
		{ // 抓捕失败
			success = 0;
		}

		if ( success == 1 )
		{
			system_talkjson_world( 6025, v1, v2, NULL, NULL, NULL, NULL, 1 );
		}
		else
		{
			nhero->level -= 5;
			if ( nhero->level < 1 )
				nhero->level = 1;
			nation_hero_attrcalc( config->herokind );
			nation_hero_changepos( config->herokind );
			system_talkjson_world( 6024, v1, v2, NULL, NULL, NULL, NULL, 1 );
		}
	}
	else
	{
		// 发送失败邮件
		char title[MAIL_TITLE_MAXSIZE] = { 0 };
		sprintf( title, "%s%d", TAG_TEXTID, 5045 );

		// 内容
		char content[MAIL_CONTENT_MAXSIZE] = { 0 };
		sprintf( content, "{\"text\":\"%s%d\",\"win\":0,\"name\":\"%s\",\"kind\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tpos\":\"%d,%d\",\"ws0\":%d,\"ws1\":%d,\"ws2\":%d}",
			TAG_TEXTID, 5503, pCity->name, nhero->kind, nhero->level, pCity->posx, pCity->posy, nhero->posx, nhero->posy, pCity->temp_wounded_soldiers[0], pCity->temp_wounded_soldiers[1], pCity->temp_wounded_soldiers[2] );

		mailid = mail( pCity->actor_index, pCity->actorid, MAIL_TYPE_FIGHT_NATIONHERO, title, content, "", 0, 0 );
	}

	// 插入战斗详情邮件
	if ( mailid > 0 )
	{
		mail_fight( mailid, pCity->actorid, pFight->unit_json );
	}
	return 0;
}

// 活动怪和玩家城池
int army_activity_vs_city( int army_index, City *pTargetCity, Fight *pFight )
{
	i64 mailid = 0;
	char title[MAIL_TITLE_MAXSIZE] = { 0 };
	char content[MAIL_CONTENT_MAXSIZE] = { 0 };
	MapActivity *activity = map_activity_getptr( g_army[army_index].from_index );
	if ( !activity )
	{
		pTargetCity->act12_state = 2;
		map_activity_delete( pTargetCity->act12_idx );
		pTargetCity->act12_idx = -1;
		return -1;
	}
	MapActivityInfo *config = map_activity_getconfig( activity->kind );
	if ( !config )
	{
		pTargetCity->act12_state = 2;
		map_activity_delete( pTargetCity->act12_idx );
		pTargetCity->act12_idx = -1;
		return -1;
	}
	army_delete( army_index );

	int turn = pTargetCity->act12_turn;
	if ( turn <= 0 || turn >= g_activity_12_maxnum )
	{
		return -1;
	}

	if ( pFight->result == FIGHT_WIN )
	{ // 玩家防守失败
		pTargetCity->act12_state = 2;
		map_activity_delete( pTargetCity->act12_idx );
		pTargetCity->act12_idx = -1;

		// 发送失败邮件
		char title[MAIL_TITLE_MAXSIZE] = { 0 };
		sprintf( title, "%s%d", TAG_TEXTID, 5055 );

		char attach[MAIL_ATTACH_MAXSIZE] = { 0 };

		// 内容
		char content[MAIL_CONTENT_MAXSIZE] = { 0 };
		sprintf( content, "{\"text\":\"%s%d\",\"win\":0,\"my\":2,\"name\":\"%s\",\"kind\":%d,\"ws0\":%d,\"ws1\":%d,\"ws2\":%d,\"award\":\"%s\",\"turn\":%d}",
			TAG_TEXTID, 5548, pTargetCity->name, config->kind, pTargetCity->temp_wounded_soldiers[0], pTargetCity->temp_wounded_soldiers[1], pTargetCity->temp_wounded_soldiers[2], attach, pTargetCity->act12_turn );
		mailid = mail( pTargetCity->actor_index, pTargetCity->actorid, MAIL_TYPE_FIGHT_ACTIVITY12, title, content, "", 0, 0 );
	}
	else
	{ // 玩家防守成功
		// 获得的奖励
		//AwardGetInfo awardinfo = { 0 };
		//for ( int i = 0; i < 4; i++ )
		//{
		//	if ( g_activity_12[turn].awardkind[i] > 0 )
		//	{
		//		awardgroup_withid( pTargetCity->actor_index, g_activity_12[turn].awardkind[i], g_activity_12[turn].awardnum[i], 0, PATH_ACTIVITY_MONSTER, &awardinfo );
		//	}
		//}

		// 发送胜利邮件
		char title[MAIL_TITLE_MAXSIZE] = { 0 };
		sprintf( title, "%s%d", TAG_TEXTID, 5054 );

		// 奖励附件
		char attach[MAIL_ATTACH_MAXSIZE] = { 0 };
		for ( int tmpi = 0; tmpi < 4; tmpi++ )
		{
			if ( g_activity_12[turn].awardkind[tmpi] <= 0 )
				continue;
			char tempitem[32] = { 0 };
			sprintf( tempitem, "%d,%d@", g_activity_12[turn].awardkind[tmpi], g_activity_12[turn].awardnum[tmpi] );
			strcat( attach, tempitem );
		}

		// 内容
		char content[MAIL_CONTENT_MAXSIZE] = { 0 };
		sprintf( content, "{\"text\":\"%s%d\",\"win\":1,\"my\":2,\"name\":\"%s\",\"kind\":%d,\"ws0\":%d,\"ws1\":%d,\"ws2\":%d,\"award\":\"%s\",\"turn\":%d}",
			TAG_TEXTID, 5547, pTargetCity->name, config->kind, pTargetCity->temp_wounded_soldiers[0], pTargetCity->temp_wounded_soldiers[1], pTargetCity->temp_wounded_soldiers[2], "", pTargetCity->act12_turn );
		mailid = mail( pTargetCity->actor_index, pTargetCity->actorid, MAIL_TYPE_FIGHT_ACTIVITY12, title, content, attach, 0, 0 );

		if ( pTargetCity->act12_turn < g_activity_12_maxnum-1 )
		{
			pTargetCity->act12_turn += 1;
			activity_12_createarmy( pTargetCity );
		}
		else
		{ // 最后一波了
			pTargetCity->act12_turn += 1;
			pTargetCity->act12_state = 2;
			map_activity_delete( pTargetCity->act12_idx );
			pTargetCity->act12_idx = -1;
		}
	}

	// 插入战斗详情邮件
	if ( mailid > 0 )
	{
		mail_fight( mailid, pTargetCity->actorid, pFight->unit_json );
	}

	// 刷新界面
	activity_12_sendinfo( pTargetCity->actor_index );

	if ( pTargetCity )
	{
		// 城墙守卫
		city_guard_sendnum( pTargetCity->actor_index );

		// 自动补兵
		hero_addsoldiers_audo( pTargetCity );
	}
	return 0;
}
