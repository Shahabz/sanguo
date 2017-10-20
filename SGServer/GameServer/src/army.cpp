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
#include "army_group.h"
#include "army_march.h"
#include "army_fight.h"
#include "army_mail.h"
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

extern MapTownInfo *g_towninfo;
extern int g_towninfo_maxnum;

extern MapTown *g_map_town;
extern int g_map_town_maxcount;

extern MapEnemyInfo *g_enemyinfo;
extern int g_enemyinfo_maxnum;

extern MapEnemy *g_map_enemy;
extern int g_map_enemy_maxcount;

extern MapRes *g_map_res;
extern int g_map_res_maxcount;

extern Fight g_fight;

extern ArmyGroup *g_armygroup;
extern int g_armygroup_maxcount;

Army *g_army = NULL;
int g_army_maxcount = 0;
int g_army_count = 0;
extern int g_army_maxindex;

// 读档完毕的回调
int army_loadcb( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;

	// 计算临时变量
	if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	{ // 出发是城池
		int city_index = city_getindex_withactorid( g_army[army_index].from_id );
		g_army[army_index].from_index = city_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
		{
			city_battlequeue_add( &g_city[city_index], army_index );
		}
		// 武将状态
		// 英雄状态
		for ( int tmpi = 0; tmpi < ARMY_HERO_MAX; tmpi++ )
		{
			int hero_index = city_hero_getindex( city_index, g_army[army_index].herokind[tmpi] );
			if ( hero_index >= 0 && hero_index < HERO_CITY_MAX )
			{
				if ( g_city[city_index].hero[hero_index].state == HERO_STATE_FIGHT )
					return -1;
				hero_changestate( city_index, g_army[army_index].herokind[tmpi], HERO_STATE_FIGHT );
			}
		}
	}
	else if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
	{ // 出发是城镇
		g_army[army_index].from_index = g_army[army_index].from_id;
	}

	if ( g_army[army_index].to_type == MAPUNIT_TYPE_CITY )
	{ // 目的是城池
		g_army[army_index].to_index = city_getindex_withactorid( g_army[army_index].to_id );
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ARMY )
	{ // 目的是部队
		g_army[army_index].to_index = g_army[army_index].to_id;
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_TOWN )
	{ // 目的是城镇
		g_army[army_index].to_index = g_army[army_index].to_id;
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ENEMY )
	{ // 目的是野怪
		g_army[army_index].to_index = g_army[army_index].to_id;
		map_enemy_addselected( g_army[army_index].to_index );
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_RES )
	{ // 目的是资源点
		g_army[army_index].to_index = g_army[army_index].to_id;
		if ( g_army[army_index].state == ARMY_STATE_GATHER )
		{
			map_res_setarmy( g_army[army_index].to_index, army_index );
		}
	}
	
	// 目标是玩家
	City *pTargetCity = army_getcityptr_target( army_index );
	if ( pTargetCity && g_army[army_index].state == ARMY_STATE_HELP )
	{ // 驻防列表
		city_helparmy_add( pTargetCity, army_index );
	}

	// 总距离(不是计算行军时间的距离，这是两点之间的直线距离)
	g_army[army_index].move_total_distance = (short)sqrt( pow( (float)(g_army[army_index].from_posx - g_army[army_index].to_posx), 2 ) + pow( (float)(g_army[army_index].from_posy - g_army[army_index].to_posy), 2 ) );

	// 添加到显示单元
	if ( g_army[army_index].state == ARMY_STATE_MARCH ||
		g_army[army_index].state == ARMY_STATE_OCCUPY ||
		g_army[army_index].state == ARMY_STATE_REBACK )
	{
		g_army[army_index].unit_index = mapunit_add( MAPUNIT_TYPE_ARMY, army_index );
	}

	// 添加到占用格子
	if ( g_army[army_index].state == ARMY_STATE_OCCUPY )
	{
		map_addobject( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].posx, g_army[army_index].posy );
	}

	// 添加行军路线
	if ( g_army[army_index].state == ARMY_STATE_MARCH ||
		g_army[army_index].state == ARMY_STATE_REBACK )
	{
		army_marchroute_add( army_index );
	}
	g_army_count += 1;
	return 0;
}

int army_load()
{
	g_army_maxcount = g_Config.max_citycount * 4;
	g_army = (Army*)malloc( sizeof( Army ) * g_army_maxcount );
	memset( g_army, 0, sizeof( Army ) * g_army_maxcount );
	printf_msg( "Army  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_army_maxcount, (sizeof( Army )*g_army_maxcount) / 1024.0 / 1024.0, sizeof( Army ) / 1024.0 );
	army_load_auto( army_getptr, army_loadcb, "map_army" );
	return 0;
}

int army_save( FILE *fp )
{
	army_batch_save_auto( g_army, g_army_maxcount, "map_army", fp );
	return 0;
}

// 数据库删除一只部队
void army_del_db( int army_index )
{
	char szSQL[256] = { 0 };
	sprintf( szSQL, "DELETE FROM `map_army` WHERE `index`='%d'", army_index );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
	}
	return;
}

Army *army_getptr( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return NULL;
	return &g_army[army_index];
}

Army *army_getptr_cityhero( City *pCity, int herokind )
{
	if ( !pCity )
		return NULL;
	for ( int tmpi = 0; tmpi < CITY_BATTLEQUEUE_MAX; tmpi++ )
	{
		int army_index = pCity->battle_armyindex[tmpi];
		if ( army_index >= 0 && army_index < g_army_maxcount )
		{
			for ( int i = 0; i < ARMY_HERO_MAX; i++ )
			{
				if ( g_army[army_index].herokind[i] == herokind )
				{
					return &g_army[army_index];
				}
			}
		}
	}
	return NULL;
}

// 显示单元属性
void army_makeunit( int army_index, SLK_NetS_AddMapUnit *pAttr )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	Army *pArmy = &g_army[army_index];
	if ( pArmy->state == ARMY_STATE_IDLE ||
		pArmy->state == ARMY_STATE_GROUP_END ||
		pArmy->state == ARMY_STATE_READY ||
		pArmy->state == ARMY_STATE_GATHER ||
		pArmy->state == ARMY_STATE_HELP ||
		pArmy->state == ARMY_STATE_FIGHT )
	{
		return;
	}
	pAttr->m_state = pArmy->state;
	pAttr->m_posx = pArmy->posx;
	pAttr->m_posy = pArmy->posy;

	// 有出发位置和目的位置
	if ( pArmy->state == ARMY_STATE_MARCH || pArmy->state == ARMY_STATE_REBACK )
	{
		if ( pArmy->state == ARMY_STATE_MARCH )
		{
			// 出发数据
			pAttr->m_char_value[0] = pArmy->from_type;
			pAttr->m_short_value[0] = pArmy->from_posx;
			pAttr->m_short_value[1] = pArmy->from_posy;
		

			// 目的数据
			pAttr->m_char_value[2] = pArmy->to_type;
			pAttr->m_short_value[2] = pArmy->to_posx;
			pAttr->m_short_value[3] = pArmy->to_posy;

			// 出发格子
			if ( pArmy->from_type == MAPUNIT_TYPE_CITY || pArmy->from_type == MAPUNIT_TYPE_ARMY || pArmy->from_type == MAPUNIT_TYPE_ENEMY || pArmy->from_type == MAPUNIT_TYPE_RES )
				pAttr->m_char_value[1] = 1;
			else if ( pArmy->from_type == MAPUNIT_TYPE_TOWN )
				pAttr->m_char_value[1] = map_getobject_grid( MAPUNIT_TYPE_TOWN, pArmy->from_index);

			// 目的格子
			if ( pArmy->to_type == MAPUNIT_TYPE_CITY || pArmy->to_type == MAPUNIT_TYPE_ARMY || pArmy->to_type == MAPUNIT_TYPE_ENEMY || pArmy->to_type == MAPUNIT_TYPE_RES )
				pAttr->m_char_value[3] = 1;
			else if ( pArmy->to_type == MAPUNIT_TYPE_TOWN )
				pAttr->m_char_value[3] = map_getobject_grid( MAPUNIT_TYPE_TOWN, pArmy->to_index );


		}
		// 返程中就相反
		else
		{
			// 出发数据
			pAttr->m_char_value[0] = pArmy->to_type;
			pAttr->m_short_value[0] = pArmy->to_posx;
			pAttr->m_short_value[1] = pArmy->to_posy;

			// 目的数据
			pAttr->m_char_value[2] = pArmy->from_type;
			pAttr->m_short_value[2] = pArmy->from_posx;
			pAttr->m_short_value[3] = pArmy->from_posy;

			// 出发格子
			if ( pArmy->to_type == MAPUNIT_TYPE_CITY || pArmy->to_type == MAPUNIT_TYPE_ARMY || pArmy->to_type == MAPUNIT_TYPE_ENEMY || pArmy->to_type == MAPUNIT_TYPE_RES )
				pAttr->m_char_value[1] = 1;
			else if ( pArmy->to_type == MAPUNIT_TYPE_TOWN )
				pAttr->m_char_value[1] = map_getobject_grid( MAPUNIT_TYPE_TOWN, pArmy->to_index );

			// 目的格子
			if ( pArmy->from_type == MAPUNIT_TYPE_CITY || pArmy->from_type == MAPUNIT_TYPE_ARMY || pArmy->from_type == MAPUNIT_TYPE_ENEMY || pArmy->from_type == MAPUNIT_TYPE_RES )
				pAttr->m_char_value[3] = 1;
			else if ( pArmy->from_type == MAPUNIT_TYPE_TOWN )
				pAttr->m_char_value[3] = map_getobject_grid( MAPUNIT_TYPE_TOWN, pArmy->from_index );

		}
		// 国家
		pAttr->m_char_value[4] = army_getnation( army_index );
		pAttr->m_char_value_count = 5;

		// 行为
		pAttr->m_short_value[4] = pArmy->action;
		pAttr->m_short_value_count = 5;

		// 部队索引
		pAttr->m_int_value[0] = army_index;
		// 移动多少秒了
		pAttr->m_int_value[1] = pArmy->statetime;
		// 需要多少秒
		pAttr->m_int_value[2] = pArmy->stateduration;
		pAttr->m_int_value_count = 3;
		// 武将
		for ( int i = 0; i < ARMY_HERO_MAX; i++ )
		{
			if ( pArmy->herokind[i] > 0 )
			{
				pAttr->m_int_value[pAttr->m_int_value_count] = pArmy->herokind[i];
				pAttr->m_int_value_count += 1;
			}
		}

	}
}


// 部队位置
void army_getpos( int army_index, short *posx, short *posy )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	*posx = g_army[army_index].posx;
	*posy = g_army[army_index].posy;
}

// 获取空闲army_index
int army_getfreeindex()
{
	for ( int tmpi = 0; tmpi < g_army_maxcount; tmpi++ )
	{
		if ( g_army[tmpi].from_type <= 0 )
		{
			return tmpi;
		}
	}
	return -1;
}

// 出征
int army_battle( City *pCity, SLK_NetC_MapBattle *info )
{
	if ( !pCity )
		return -1;
	int group_index = -1;
	// 总兵力
	int totalsoldiers = 0;
	// 英雄状态检查
	for ( int tmpi = 0; tmpi < ARMY_HERO_MAX; tmpi++ )
	{
		int hero_index = city_hero_getindex( pCity->index, info->m_herokind[tmpi] );
		if ( hero_index >= 0 && hero_index < HERO_CITY_MAX )
		{
			if ( pCity->hero[hero_index].state == HERO_STATE_FIGHT )
				return -1;
			totalsoldiers += pCity->hero[hero_index].soldiers;
		}
	}

	// 行军距离
	int distance = abs( pCity->posx - info->m_to_posx ) + abs( pCity->posy - info->m_to_posy );
	// 行军时间
	int marchtime = army_marchtime_calc( pCity, pCity->posx, pCity->posy, info->m_to_posx, info->m_to_posy );
	// 消耗粮食
	int cost_food = (int)ceil( global.army_march_food_v1*totalsoldiers + global.army_march_food_v2*(marchtime*marchtime) + global.army_march_food_v3*marchtime );

	char to_type = 0;
	int to_id = 0;
	if ( info->m_to_unit_index >= 0 && info->m_to_unit_index < g_mapunit_maxcount )
	{ // 攻击的是实体
		if ( g_mapunit[info->m_to_unit_index].type == MAPUNIT_TYPE_CITY )
		{ // 城池
			int city_index = g_mapunit[info->m_to_unit_index].index;
			if ( city_index < 0 || city_index >= g_city_maxcount )
				return -1;
			City *pTargetCity = &g_city[city_index];
			if ( !pTargetCity )
				return -1;

			if ( info->m_action == ARMY_ACTION_HELP_TROOP )
			{
				int num = city_helparmy_getnum( pTargetCity );
				if ( num >= CITY_HELPDEFENSE_MAX || num >= city_helparmy_maxnum( pTargetCity ) )
				{
					actor_system_message( pCity->actor_index, 1241 ); // 可被驻防的队列已满
					return -1;
				}
			}

			if ( info->m_action == ARMY_ACTION_GROUP_CREATE )
			{// 创建集结
				// 被攻击信息数量
				if ( city_underfire_getnum( pTargetCity ) >= CITY_UNDERFIRE_MAX )
				{
					write_gamelog( "[BATTLE_FAILED_UNDERFIRE_FULL]_cityid:%d_action:%d_tocityid:%d", pCity->actorid, info->m_action, pTargetCity->actorid );
					//return -1;
				}

				// 战争守护状态检查
				if ( pTargetCity->ptsec > 0 )
				{
					actor_system_message( pCity->actor_index, 1248 );
					return -1;
				}

				// 盟友检查
				if ( pCity->nation == pTargetCity->nation )
				{
					return -1;
				}

				// 集结类型
				int grouptype = info->m_appdata;
				int stateduration = 0;
				int cost_body = 0;
				int cost_item = 0;
				if ( grouptype == 1 )
				{// 闪电
					if ( marchtime > global.cityfight_sec_limit[0] )
						return -1;
					stateduration = marchtime;
					cost_body = global.cityfight_body_cost[0];
					cost_item = global.cityfight_item_cost[0];
				}
				else if ( grouptype == 2 )
				{ // 奔袭
					if ( marchtime > global.cityfight_sec_limit[1] )
						return -1;
					if ( marchtime < global.cityfight_sec_group[1] )
					{
						stateduration = global.cityfight_sec_group[1] + marchtime;
					}
					else
					{
						stateduration = marchtime;
					}
					cost_body = global.cityfight_body_cost[1];
					cost_item = global.cityfight_item_cost[1];
				}
				else if ( grouptype == 3 )
				{ // 远征
					if ( marchtime > global.cityfight_sec_limit[2] )
						return -1;
					if ( marchtime < global.cityfight_sec_group[2] )
					{
						stateduration = global.cityfight_sec_group[2] + marchtime;
					}
					else
					{
						stateduration = marchtime;
					}
					cost_body = global.cityfight_body_cost[2];
					cost_item = global.cityfight_item_cost[2];
				}
				else
					return -1;

				// 消耗
				int itemkind = 486;
				if ( item_lost( pCity->actor_index, itemkind, cost_item, PATH_FIGHT ) < 0 )
				{
					if ( pCity->body < cost_body )
						return -1;
					city_changebody( pCity->index, -cost_body, PATH_FIGHT );
				}

				// 创建集结战场
				group_index = armygroup_create( grouptype, MAPUNIT_TYPE_CITY, pCity->actorid, MAPUNIT_TYPE_CITY, pTargetCity->actorid, stateduration );
				if ( group_index < 0 )
					return -1;

				// 破罩子
				city_changeprotect( pCity->index, -pCity->ptsec, PATH_FIGHT );
			}
			else if ( info->m_action == ARMY_ACTION_GROUP_ATTACK )
			{// 参与集结进攻
				// 盟友检查
				if ( pCity->nation == pTargetCity->nation )
				{
					return -1;
				}

				if ( info->m_group_index < 0 || info->m_group_index >= g_armygroup_maxcount )
				{
					return -1;
				}

				if ( g_armygroup[info->m_group_index].id != info->m_id )
				{
					return -1;
				}

				if ( g_armygroup[info->m_group_index].type == 1 )
				{ 
					// 闪电战无法参与进攻
					actor_notify_alert( pCity->actor_index, 1267 );
					return -1;
				}
				if ( marchtime >= (g_armygroup[info->m_group_index].stateduration - g_armygroup[info->m_group_index].statetime) )
				{
					// 此番前去将会错过会战时间，还是静观其变吧
					actor_notify_alert( pCity->actor_index, 1268 );
					return -1;
				}
				group_index = info->m_group_index;

				// 破罩子
				city_changeprotect( pCity->index, -pCity->ptsec, PATH_FIGHT );

				// 已经加入城战攻击部队
				actor_notify_alert( pCity->actor_index, 1269 );
			}
			else if ( info->m_action == ARMY_ACTION_GROUP_DEFENSE )
			{// 参与集结防守
				// 盟友检查
				if ( pCity->nation != pTargetCity->nation )
				{
					return -1;
				}
				if ( info->m_group_index < 0 || info->m_group_index >= g_armygroup_maxcount )
				{
					return -1;
				}

				if ( g_armygroup[info->m_group_index].id != info->m_id )
				{
					return -1;
				}

				if ( marchtime >= (g_armygroup[info->m_group_index].stateduration - g_armygroup[info->m_group_index].statetime) )
				{
					// 此番前去将会错过会战时间，还是静观其变吧
					actor_notify_alert( pCity->actor_index, 1268 );
					return -1;
				}
				group_index = info->m_group_index;

				// 已经加入城战防守部队
				actor_notify_alert( pCity->actor_index, 1270 );
			}

			to_type = MAPUNIT_TYPE_CITY;
			to_id = pTargetCity->actorid;
		}
		else if ( g_mapunit[info->m_to_unit_index].type == MAPUNIT_TYPE_ARMY )
		{ // 部队
			int army_index = g_mapunit[info->m_to_unit_index].index;
			if ( army_index < 0 || army_index >= g_army_maxcount )
				return -1;
			if ( g_army[army_index].state != ARMY_STATE_OCCUPY )
				return -1;

			// 获取这只部队所属城市还是NPC
			if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
			{
				int city_index = g_army[army_index].from_index;
				if ( city_index < 0 || city_index >= g_city_maxcount )
					return -1;
				City *pTargetCity = &g_city[city_index];
				if ( !pTargetCity )
					return -1;

				// 被攻击信息数量
				if ( city_underfire_getnum( pTargetCity ) >= CITY_UNDERFIRE_MAX )
				{
					write_gamelog( "[BATTLE_FAILED_UNDERFIRE_FULL]_cityid:%d_action:%d_tocityid:%d", pCity->actorid, info->m_action, pTargetCity->actorid );
					return -1;
				}

				// 盟友检查
				if ( pCity->nation == pTargetCity->nation )
				{
					return -1;
				}
			}
			else if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
			{
				int townid = g_army[army_index].from_id;
			}
			city_changeprotect( pCity->index, -pCity->ptsec, PATH_FIGHT );
			to_type = MAPUNIT_TYPE_ARMY;
			to_id = army_index;

		}
		else if ( g_mapunit[info->m_to_unit_index].type == MAPUNIT_TYPE_TOWN )
		{ // 城镇
			int townid = g_mapunit[info->m_to_unit_index].index;
			if ( townid <= 0 || townid >= g_map_town_maxcount )
				return -1;
			MapTown *pTown = map_town_getptr( townid );
			if ( !pTown )
				return -1;
			if ( g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_TYPE8 )
			{
				if ( pTown->nation > 0 )
				{// 一国之都，不可轻犯
					actor_notify_alert( pCity->actor_index, 1333 );
					return -1;
				}
			}

			if ( info->m_action == ARMY_ACTION_NATION_ATTACK )
			{ // 参与国战进攻
				if ( pCity->level < global.nationfight_actorlevel )
				{
					char v1[32] = { 0 };
					sprintf( v1, "%d", global.nationfight_actorlevel );
					actor_notify_alert_v( pCity->actor_index, 1344, v1, NULL );
					return -1;
				}
				if ( pCity->nation == pTown->nation )
				{
					return -1;
				}

				if ( info->m_group_index < 0 || info->m_group_index >= g_armygroup_maxcount )
				{
					return -1;
				}

				if ( g_armygroup[info->m_group_index].id != info->m_id )
				{
					return -1;
				}

				if ( marchtime >= (g_armygroup[info->m_group_index].stateduration - g_armygroup[info->m_group_index].statetime) )
				{
					// 此番前去将会错过会战时间，还是静观其变吧
					actor_notify_alert( pCity->actor_index, 1268 );
					return -1;
				}
				group_index = info->m_group_index;

				// 破罩子
				city_changeprotect( pCity->index, -pCity->ptsec, PATH_FIGHT );

				// 已经加入国战攻击部队
				actor_notify_alert( pCity->actor_index, 1298 );
			}
			else if ( info->m_action == ARMY_ACTION_NATION_DEFENSE )
			{ // 参与国战防守
				if ( pCity->level < global.nationfight_actorlevel )
				{
					char v1[32] = { 0 };
					sprintf( v1, "%d", global.nationfight_actorlevel );
					actor_notify_alert_v( pCity->actor_index, 1344, v1, NULL );
					return -1;
				}
				// 盟友检查
				if ( pCity->nation != pTown->nation )
				{
					return -1;
				}
				if ( info->m_group_index < 0 || info->m_group_index >= g_armygroup_maxcount )
				{
					return -1;
				}

				if ( g_armygroup[info->m_group_index].id != info->m_id )
				{
					return -1;
				}

				if ( marchtime >= (g_armygroup[info->m_group_index].stateduration - g_armygroup[info->m_group_index].statetime) )
				{
					// 此番前去将会错过会战时间，还是静观其变吧
					actor_notify_alert( pCity->actor_index, 1268 );
					return -1;
				}
				group_index = info->m_group_index;

				// 已经加入国战防守部队
				actor_notify_alert( pCity->actor_index, 1299 );
			}
			to_type = MAPUNIT_TYPE_TOWN;
			to_id = g_mapunit[info->m_to_unit_index].index;
		}
		else if ( g_mapunit[info->m_to_unit_index].type == MAPUNIT_TYPE_ENEMY )
		{ // 野怪
			MapEnemy *enemy = map_enemy_getptr( g_mapunit[info->m_to_unit_index].index );
			if ( !enemy )
				return -1;
			MapEnemyInfo *config = map_enemy_getconfig( enemy->kind );
			if ( !config )
				return -1;
			if ( config->level > pCity->mokilllv + 1 )
				return -1;
			// 消耗粮食
			if ( cost_food > pCity->food )
				return -1;
			city_changefood( pCity->index, -cost_food, PATH_MARCH );
			
			// 被选定次数
			map_enemy_addselected( g_mapunit[info->m_to_unit_index].index );

			to_type = MAPUNIT_TYPE_ENEMY;
			to_id = g_mapunit[info->m_to_unit_index].index;
		}
		else if ( g_mapunit[info->m_to_unit_index].type == MAPUNIT_TYPE_RES )
		{ // 资源点
			MapRes *res = map_res_getptr( g_mapunit[info->m_to_unit_index].index );
			if ( !res )
				return -1;
			MapResInfo *config = map_res_getconfig( res->kind );
			if ( !config )
				return -1;
			short tile_townid = map_tile_gettownid( res->posx, res->posy );
			if ( tile_townid > 0 && tile_townid < g_map_town_maxcount )
			{
				if ( g_towninfo[tile_townid].range_gather == 1 )
				{
					char tile_nation = map_tile_getnation( res->posx, res->posy );
					if ( tile_nation >= 0 && tile_nation != pCity->nation )
					{ // 非本国占领区域，不可贸然前往
						actor_notify_alert( pCity->actor_index, 1323 );
						return -1;
					}
				}
			}
			
			// 武将等级检查
			int hero_index = city_hero_getindex( pCity->index, info->m_herokind[0] );
			if ( hero_index < 0 || hero_index >= HERO_CITY_MAX )
				return -1;
			if ( pCity->hero[hero_index].level < config->herolevel )
				return -1;

			// 消耗粮食
			if ( cost_food > pCity->food )
				return -1;
			city_changefood( pCity->index, -cost_food, PATH_MARCH );

			// 如果已经有人占领
			int army_index = map_res_getarmy( g_mapunit[info->m_to_unit_index].index );
			if ( army_index >= 0 )
			{
				if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY && g_army[army_index].from_index >= 0 && g_army[army_index].from_index < g_city_maxcount )
				{
					// 盟友检查
					int city_index = g_army[army_index].from_index;
					if ( pCity->nation == g_city[city_index].nation )
					{
						//actor_system_message( pCity->actor_index, 71 );
						return -1;
					}
					city_changeprotect( pCity->index, -pCity->ptsec, PATH_FIGHT );
				}
			}
			to_type = MAPUNIT_TYPE_RES;
			to_id = g_mapunit[info->m_to_unit_index].index;
		}
	}
	else
	{	// 驻扎空地
		if ( info->m_action != ARMY_ACTION_OCCUPY )
		{
			write_gamelog( "[BATTLE_INVALID]_actorid:%d_action:%d_info->unit_index:%d", pCity->actorid, info->m_action, info->m_to_unit_index );
			//actor_system_message( pCity->actor_index, 34 );  // 目标不合法，请重新选取!
			return -1;
		}
		to_type = 0;
		to_id = 0;
	}

	// 创建部队
	int army_index = army_create( MAPUNIT_TYPE_CITY, pCity->actorid, to_type, to_id, ARMY_STATE_MARCH, info );
	if ( army_index < 0 )
		return -1;

	// 总兵力
	g_army[army_index].totals = totalsoldiers;

	// 英雄状态设置
	for ( int tmpi = 0; tmpi < ARMY_HERO_MAX; tmpi++ )
	{
		int hero_index = city_hero_getindex( pCity->index, info->m_herokind[tmpi] );
		if ( hero_index >= 0 && hero_index < HERO_CITY_MAX )
		{
			if ( pCity->hero[hero_index].state == HERO_STATE_FIGHT )
				continue;
			hero_changestate( pCity->index, info->m_herokind[tmpi], HERO_STATE_FIGHT );
		}
	}

	// 发起集结,设置队长
	if ( group_index >= 0 && group_index < g_armygroup_maxcount )
	{
		if ( info->m_action == ARMY_ACTION_GROUP_CREATE )
		{
			armygroup_setleader( group_index, army_index );
		}
		else
		{
			g_army[army_index].group_index = group_index;
			g_army[army_index].group_id = g_armygroup[group_index].id;
		}
		// 加入集结列表
		armygroup_addarmy( army_index );
	}

	// 添加到出征队列
	if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	{
		city_battlequeue_add( army_getcityptr( army_index ), army_index );
	}

	// 目标是玩家
	City *pTargetCity = army_getcityptr_target( army_index );
	if ( pTargetCity )
	{
		if ( info->m_action == ARMY_ACTION_FIGHT )
		{// 被攻击列表
			city_underfire_add( pTargetCity, army_index );
		}
	}
	return army_index;
}

// 创建一只部队
int army_create( char from_type, int from_id, char to_type, int to_id, char stat, SLK_NetC_MapBattle *info )
{
	int army_index = army_getfreeindex();
	if ( army_index < 0 )
		return -1;
	// 为了遍历的效率，计算最大的索引
	if ( army_index >= g_army_maxindex )
	{
		g_army_maxindex = army_index + 1;
	}

	memset( &g_army[army_index], 0, sizeof( Army ) );
	g_army[army_index].index = army_index;
	g_army[army_index].from_index = -1;
	g_army[army_index].to_index = -1;
	g_army[army_index].unit_index = -1;
	g_army[army_index].group_index = -1;
	g_army[army_index].group_pos = -1;
	g_army_count += 1;

	// 出发数据
	g_army[army_index].from_type = from_type;
	g_army[army_index].from_id = from_id;
	for ( int tmpi = 0; tmpi < ARMY_HERO_MAX; tmpi++ )
	{
		g_army[army_index].herokind[tmpi] = info->m_herokind[tmpi];
	}
	if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	{ // 出发是城池
		int city_index = city_getindex_withactorid( g_army[army_index].from_id );
		g_army[army_index].from_index = city_index;
		city_getpos( city_index, &g_army[army_index].from_posx, &g_army[army_index].from_posy );
	}
	else if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
	{ // 出发是城镇
		int index = g_army[army_index].from_id;
		g_army[army_index].from_index = index;
		map_town_getpos( index, &g_army[army_index].from_posx, &g_army[army_index].from_posy );
	}

	// 目的数据
	g_army[army_index].to_type = to_type;
	g_army[army_index].to_id = to_id;
	if ( g_army[army_index].to_type == MAPUNIT_TYPE_CITY )
	{ // 目的是城池
		int city_index = city_getindex_withactorid( g_army[army_index].to_id );
		g_army[army_index].to_index = city_index;
		city_getpos( city_index, &g_army[army_index].to_posx, &g_army[army_index].to_posy );
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ARMY )
	{ // 目的是部队
		int target_armyindex = g_army[army_index].to_id;
		g_army[army_index].to_index = target_armyindex;
		if ( target_armyindex < 0 || target_armyindex >= g_army_maxcount )
			return -1;
		g_army[army_index].to_posx = g_army[target_armyindex].posx;
		g_army[army_index].to_posy = g_army[target_armyindex].posy;
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_TOWN )
	{ // 目的是城镇
		int index = g_army[army_index].to_id;
		g_army[army_index].to_index = index;
		map_town_getpos( index, &g_army[army_index].to_posx, &g_army[army_index].to_posy );
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ENEMY )
	{ // 目的是流寇
		int index = g_army[army_index].to_id;
		g_army[army_index].to_index = index;
		//MapMonsterConfig *config = map_monster_getconfig( g_army[army_index].to_index );
		//if ( config )
		//	g_army[army_index].target_mark = config->monsterid;
		map_enemy_getpos( index, &g_army[army_index].to_posx, &g_army[army_index].to_posy );
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_RES )
	{ // 目的是资源点
		int index = g_army[army_index].to_id;
		g_army[army_index].to_index = index;
		map_res_getpos( index, &g_army[army_index].to_posx, &g_army[army_index].to_posy );
	}
	else
	{ // 目的是空地
		g_army[army_index].to_posx = info->m_to_posx;
		g_army[army_index].to_posy = info->m_to_posy;
	}

	g_army[army_index].id = (int)time( NULL );
	g_army[army_index].state = stat;
	g_army[army_index].statetime = 0;
	g_army[army_index].action = info->m_action;
	g_army[army_index].appdata = info->m_appdata;
	g_army[army_index].posx = g_army[army_index].from_posx;
	g_army[army_index].posy = g_army[army_index].from_posy;

	// 距离(不是计算行军时间的距离，这是两点之间的直线距离)
	g_army[army_index].move_total_distance = (short)sqrt( pow( (float)(g_army[army_index].from_posx - g_army[army_index].to_posx), 2 ) + pow( (float)(g_army[army_index].from_posy - g_army[army_index].to_posy), 2 ) );

	// 计算行军时间
	army_march_time( army_index );

	// 行军路线
	army_marchroute_add( army_index );

	// 添加到地图显示单元
	g_army[army_index].unit_index = mapunit_add( MAPUNIT_TYPE_ARMY, army_index );

	return army_index;
}

// 删除一支部队
void army_delete( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	if ( g_army[army_index].from_type <= 0 )
		return;
	mapunit_del( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].unit_index );
	if ( g_army[army_index].state == ARMY_STATE_OCCUPY )
	{
		map_delobject( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].posx, g_army[army_index].posy );
	}
	else if ( g_army[army_index].state == ARMY_STATE_FIGHT && g_army[army_index].to_type == MAPUNIT_TYPE_ENEMY )
	{
		// 被选择次数减少
		map_enemy_subselected( g_army[army_index].to_index );
	}

	army_marchroute_del( army_index );
	city_underfire_del_equal( army_getcityptr( army_index ), army_index );
	city_underfire_del( army_getcityptr_target( army_index ), army_index );
	city_helparmy_del( army_getcityptr_target( army_index ), army_index );
	armygroup_delarmy( army_index );

	if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	{
		// 删除之前需要将部队和携带的资源还原到城池里
		army_tocity( army_index );

		// 删除出征队列
		city_battlequeue_del( army_getcityptr( army_index ), army_index );

		// 邮件通知
		if ( g_army[army_index].action == ARMY_ACTION_GATHER )
		{
			// 这是为了立即召回的情况
			if ( g_army[army_index].to_type == MAPUNIT_TYPE_RES && g_army[army_index].state == ARMY_STATE_GATHER )
			{
				int index = g_army[army_index].to_index;
				if ( index >= 0 && index < g_map_res_maxcount && g_map_res[index].army_index == army_index )
				{
					map_res_setarmy( index, -1 );
				}
			}

			army_mail_gather( army_index );
		}
		else if ( g_army[army_index].action == ARMY_ACTION_GROUP_CREATE || g_army[army_index].action == ARMY_ACTION_GROUP_ATTACK )
		{ // 检查集结是否达到解散条件
			armygroup_dismiss( army_index );
		}
	}
	else if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
	{
		// 删除城镇的出征关联索引
		//map_town_battle_armyindex_del( g_army[army_index].from_index );
	}
	army_del_db( army_index );
	memset( &g_army[army_index], 0, sizeof( Army ) );
	g_army[army_index].index = -1;
	g_army[army_index].from_id = -1;
	g_army[army_index].from_index = -1;
	g_army[army_index].to_id = -1;
	g_army[army_index].to_index = -1;
	g_army[army_index].unit_index = -1;
	g_army[army_index].group_index = -1;
	g_army[army_index].group_pos = -1;
	g_army_count -= 1;
}

// 部队信息还原到城池
int army_tocity( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	City *pCity = army_getcityptr( army_index );
	if ( !pCity )
		return -1;

	// 英雄状态
	for ( int tmpi = 0; tmpi < ARMY_HERO_MAX; tmpi++ )
	{
		hero_changestate( pCity->index, g_army[army_index].herokind[tmpi], HERO_STATE_NORMAL );
	}
	actor_system_message( pCity->actor_index, 983 );
	
	// 途径
	char path = PATH_SYSTEM;
	if ( g_army[army_index].action == ARMY_ACTION_GATHER )
	{
		path = PATH_GATHER;
	}
	else if ( g_army[army_index].action == ARMY_ACTION_FIGHT )
	{
		path = PATH_FIGHT;
	}
	// 携带资源还原到城市里
	city_changesilver( pCity->index, g_army[army_index].silver, path );
	city_changewood( pCity->index, g_army[army_index].wood, path );
	city_changefood( pCity->index, g_army[army_index].food, path );
	city_changeiron( pCity->index, g_army[army_index].iron, path );

	// 如果携带钻石
	int token = g_army[army_index].token;
	if ( token > 0 )
	{
		if ( pCity->actor_index < 0 || pCity->actor_index >= g_maxactornum )
		{ // 不在线
			gift( pCity->actorid, AWARDKIND_TOKEN, token, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, path );
		}
		else
		{ // 在线
			actor_change_token( pCity->actor_index, token, path, 0 );
		}
	}

	// 如果携带奖励
	for ( int tmpi = 0; tmpi < ARMY_CARRY_ITEMMAX; tmpi++ )
	{
		int awardkind = g_army[army_index].awardkind[tmpi];
		int awardnum = g_army[army_index].awardnum[tmpi];
		if ( awardkind <= 0 )
			continue;
		if ( pCity->actor_index >= 0 )
		{
			award_getaward( pCity->actor_index, awardkind, awardnum, -1, path, NULL );
		}
		else
		{
			gift( pCity->actorid, awardkind, awardnum, awardnum, 0, 0, -1, 0, 0, -1, 0, 0, -1, path );
		}
	}
	

	if ( g_army[army_index].action == ARMY_ACTION_GATHER )
	{
		if ( g_army[army_index].wood > 0 )
		{
			// 任务数值
			//quest_addvalue( pCity, QUEST_DATAINDEX_GATHER, CityRes_Wood, (int)g_army[army_index].carry_wood );	
			// 统计
			//data_record_addvalue( pCity, DATA_RECORD_MAP_GATHER_WOOD, (int)g_army[army_index].carry_wood );

		}
		else if ( g_army[army_index].food > 0 )
		{
			//quest_addvalue( pCity, QUEST_DATAINDEX_GATHER, CityRes_Food, (int)g_army[army_index].carry_food );	
			//data_record_addvalue( pCity, DATA_RECORD_MAP_GATHER_FOOD, (int)g_army[army_index].carry_food );
		}
		else if ( g_army[army_index].iron > 0 )
		{
			//quest_addvalue( pCity, QUEST_DATAINDEX_GATHER, CityRes_Iron, (int)g_army[army_index].carry_iron );
			//data_record_addvalue( pCity, DATA_RECORD_MAP_GATHER_IRON, (int)g_army[army_index].carry_iron );
		
		}
		else if ( g_army[army_index].iron > 0 )
		{
			//quest_addvalue( pCity, QUEST_DATAINDEX_GATHER, CityRes_Mithril, (int)g_army[army_index].carry_mithril );
			//data_record_addvalue( pCity, DATA_RECORD_MAP_GATHER_MITHRIL, (int)g_army[army_index].carry_mithril );
		}
		else if ( g_army[army_index].token > 0 )
		{
			
		}
	}
	return 0;
}


// 设置部队状态
void army_setstate( int army_index, char state )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	if ( state == ARMY_STATE_REBACK && (/*g_army[army_index].state == ARMY_STATE_GROUP_ING ||*/ g_army[army_index].state == ARMY_STATE_MARCH) )
	{
		// 半路返回
		g_army[army_index].statetime = g_army[army_index].stateduration - g_army[army_index].statetime;
	}
	else
	{
		g_army[army_index].statetime = 0;
		g_army[army_index].stateduration = 0;
	}
	g_army[army_index].state = state;

	// 新状态
	switch ( g_army[army_index].state )
	{
	case ARMY_STATE_FIGHT:				// 战斗中
		mapunit_update( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].unit_index );
		//army_marchroute_del( army_index );
		break;

	case ARMY_STATE_OCCUPY:				// 驻扎中
		mapunit_update( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].unit_index );
		army_marchroute_del( army_index );
		break;

	case ARMY_STATE_GROUP_END:			// 到达指定集结地点
		mapunit_del( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].unit_index );
		g_army[army_index].unit_index = -1;
		army_marchroute_del( army_index );
		break;

	case ARMY_STATE_GATHER:				// 采集中
		army_gather_time( army_index );
		mapunit_del( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].unit_index );
		g_army[army_index].unit_index = -1;
		army_marchroute_del( army_index );
		break;

	case ARMY_STATE_HELP:				// 援助中
		mapunit_del( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].unit_index );
		g_army[army_index].stateduration = global.cityhelp_sec;
		g_army[army_index].unit_index = -1;
		army_marchroute_del( army_index );
		break;
	case ARMY_STATE_MARCH:				// 行军中
	case ARMY_STATE_REBACK:				// 返程中
		army_march_time( army_index );
		if ( g_army[army_index].unit_index < 0 )
		{
			g_army[army_index].unit_index = mapunit_add( MAPUNIT_TYPE_ARMY, army_index );
		}
		else
		{
			mapunit_update( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].unit_index );
		}
		army_marchroute_add( army_index );
		break;
	default:
		return;
	}

	// 通知部队所属城池更新出征队列
	if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	{
		city_battlequeue_sendupdate( army_index );
	}

}

// 部队逻辑
void army_alllogic()
{
	for ( int army_index = 0; army_index < g_army_maxindex/*注意：使用索引位置，为了效率*/; army_index++ )
	{
		if ( g_army[army_index].from_type <= 0 )
			continue;
		army_logic( army_index );
	}
}

// 单只部队逻辑
void army_logic( int army_index )
{
	switch ( g_army[army_index].state )
	{
	case ARMY_STATE_MARCH:				// 行军中
		army_march( army_index );
		break;
	case ARMY_STATE_FIGHT:				// 战斗中
		army_fight( army_index );
		break;
	case ARMY_STATE_GATHER:				// 采集中
		if ( army_gather( army_index ) < 0 )
		{
			army_setstate( army_index, ARMY_STATE_REBACK );
		}
		break;
	case ARMY_STATE_HELP:				// 援助中
		army_help( army_index );
		break;
	case ARMY_STATE_REBACK:				// 返程中
		army_reback( army_index );
		break;
	}
}

// 获取部队所属城池
City *army_getcityptr( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return NULL;
	if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	{
		int city_index = g_army[army_index].from_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
		{
			return &g_city[city_index];
		}
	}
	return NULL;
}

// 获取部队目标所属城池
City *army_getcityptr_target( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return NULL;
	City *pCity = NULL;
	if ( g_army[army_index].to_type == MAPUNIT_TYPE_CITY )
	{ // 目的是城池
		int city_index = g_army[army_index].to_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
			pCity = &g_city[city_index];
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ARMY )
	{ // 目的是部队
		int target_armyindex = g_army[army_index].to_id;
		pCity = army_getcityptr( target_armyindex );
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_RES )
	{ // 目的是资源点
		int index = g_army[army_index].to_id;
		pCity = army_getcityptr( map_res_getarmy( index ) );
	}
	return pCity;
}

// 获取部队名称
char *army_getname( int army_index )
{
	if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	{ // 出发是城池
		int city_index = g_army[army_index].from_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
			return g_city[city_index].name;
		return "";
	}
	else if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
	{ // 出发是城镇

	}
	return "";
}


// 获取部队目标名称
char *army_getname_target( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return NULL;
	if ( g_army[army_index].to_type == MAPUNIT_TYPE_CITY )
	{ // 目的是城池
		int city_index = g_army[army_index].to_index;
		return g_city[city_index].name;
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ARMY )
	{ // 目的是部队
		int target_armyindex = g_army[army_index].to_id;
		City *pCity = army_getcityptr( target_armyindex );
		if ( pCity )
		{
			return pCity->name;
		}
		else
		{
			return "";
		}
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_RES )
	{ // 目的是资源点
		int index = g_army[army_index].to_id;
		City *pCity = army_getcityptr( map_res_getarmy( index ) );
		if ( pCity )
		{
			return pCity->name;
		}
		else
		{
			return "";
		}
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ARMY )
	{ // 目的是部队
		City *pCity = army_getcityptr( g_army[army_index].to_index );
		if ( pCity )
		{
			return pCity->name;
		}
		else
		{
			return "";
		}
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ENEMY )
	{ // 目的是怪物
	
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_TOWN )
	{ // 目的是城镇
		
	}
	
	return "";
}

// 获取部队所属城池
int army_getcityid( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return 0;
	City *pCity = army_getcityptr( army_index );
	if ( pCity )
	{
		return pCity->actorid;
	}
	return 0;
}

// 获取部队所属国家
int army_getnation( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return 0;
	if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	{
		City *pCity = army_getcityptr( army_index );
		if ( pCity )
		{
			return pCity->nation;
		}
	}
	else if( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
	{
		MapTown *pTown = map_town_getptr( g_army[army_index].from_id );
		if ( pTown )
		{
			return pTown->nation;
		}

	}
	return 0;
}

// 获取部队目标城池
int army_getcityid_target( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return 0;
	City *pCity = army_getcityptr_target( army_index );
	if ( pCity )
	{
		return pCity->actorid;
	}
	return 0;
}

// 获取部队目标国家
int army_getnation_target( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return 0;
	City *pCity = army_getcityptr_target( army_index );
	if ( pCity )
	{
		return pCity->nation;
	}
	else
	{
		if ( g_army[army_index].to_type == MAPUNIT_TYPE_TOWN )
		{
			MapTown *pTown = map_town_getptr( g_army[army_index].to_id );
			if ( pTown )
			{
				return pTown->nation;
			}
		}
	}
	return 0;
}

// 部队添加携带道具
int army_carry_additem( int army_index, int itemkind, int itemnum )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	if ( itemkind <= 0 )
		return -1;
	if ( itemnum <= 0 )
		return -1;
	char has = 0;
	for ( int tmpi = 0; tmpi < ARMY_CARRY_ITEMMAX; tmpi++ )
	{ // 找一个相同的道具
		if ( g_army[army_index].awardkind[tmpi] == itemkind )
		{
			g_army[army_index].awardnum[tmpi] += itemnum;
			has = 1;
			break;
		}
	}

	if ( has == 0 )
	{
		for ( int tmpi = 0; tmpi < ARMY_CARRY_ITEMMAX; tmpi++ )
		{
			if ( g_army[army_index].awardkind[tmpi] <= 0 )
			{
				g_army[army_index].awardkind[tmpi] = itemkind;
				g_army[army_index].awardnum[tmpi] = itemnum;
				break;
			}
		}
	}
	return 0;
}

// 采集时间
int army_gather_time( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return 0;

	if ( g_army[army_index].to_type != MAPUNIT_TYPE_RES )
		return 0;

	int index = g_army[army_index].to_index;
	if ( index < 0 || index >= g_map_res_maxcount )
		return 0;

	MapResInfo *config = map_res_getconfig( g_map_res[index].kind );
	if ( !config )
		return 0;

	City *pCity = army_getcityptr( army_index );
	if ( !pCity )
		return 0;

	int hero_index = city_hero_getindex( pCity->index, g_army[army_index].herokind[0] );
	if ( hero_index < 0 && hero_index >= HERO_CITY_MAX )
		return 0;

	char color = pCity->hero[hero_index].color;
	if ( color < 0 || color >= 6 )
		return 0;

	// 资源里面的剩余量
	int res_total = g_map_res[index].num;

	// 每秒采集量
	float sec_gather = (config->num / (float)config->sec);

	// 采集时长
	g_army[army_index].stateduration = (int)ceil( res_total / (sec_gather* (1.0f + pCity->attr.gather_per[0]) * (1.0f + pCity->attr.gather_per[1])) );
	if ( g_army[army_index].stateduration > global.hero_gather_duration[color] )
	{
		g_army[army_index].stateduration = global.hero_gather_duration[color];
	}
	return 0;
}

// 部队采集量计算
int army_gather_calc( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;

	int index = g_army[army_index].to_index;
	if ( index < 0 || index >= g_map_res_maxcount )
	{ // 索引非法
		return -1;
	}

	MapResInfo *config = map_res_getconfig( g_map_res[index].kind );
	if ( !config )
		return -1;

	City *pCity = army_getcityptr( army_index );
	if ( !pCity )
		return -1;

	// 当前已经采集的资源数量=每秒采集量*武将采集时间*(1 + 科技加成%)*(1＋活动加成%)
	int gathernum_buff = (int)round( (config->num / (float)config->sec) * g_army[army_index].gatherbuff * (1.0f + pCity->attr.gather_per[0]) * (1.0f + pCity->attr.gather_per[1]) );
	int gathernum = gathernum_buff + ( int )round( (config->num / (float)config->sec) * (g_army[army_index].statetime - g_army[army_index].gatherbuff) * (1.0f + pCity->attr.gather_per[0]) );

	// 部队采集量
	switch ( config->type )
	{
	case 1:
		g_army[army_index].silver = gathernum;
		break;
	case 2:
		g_army[army_index].wood = gathernum;
		break;
	case 3:
		g_army[army_index].food = gathernum;
		break;
	case 4:
		g_army[army_index].iron = gathernum;
		break;
	case 5:
		g_army[army_index].token = gathernum;
	default:
		break;
	}

	// 资源采集量减少
	g_map_res[index].num -= gathernum;
	if ( g_map_res[index].num <= 0 )
	{ // 没了，删除资源点

		// 如果是都城范围,添加开发经验
		short range_townid = map_tile_gettownid( g_map_res[index].posx, g_map_res[index].posy );
		if ( range_townid > 0 && range_townid < g_towninfo_maxnum )
		{
			if ( g_towninfo[range_townid].type == MAPUNIT_TYPE_TOWN_TYPE8 )
			{
				map_town_dev_addexp( range_townid, global.town_dev_gather );
			}
		}

		map_res_delete( index );
	}
	else
	{ // 结算完毕，资源点关联部队取消
		map_res_setarmy( index, -1 );
	}

	return 0;
}

// 部队采集
int army_gather( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;

	if ( g_army[army_index].to_type != MAPUNIT_TYPE_RES )
	{ // 类型非法
		return -1;
	}

	int index = g_army[army_index].to_index;
	if ( index < 0 || index >= g_map_res_maxcount )
	{ // 索引非法
		return -1;
	}

	MapResInfo *config = map_res_getconfig( g_map_res[index].kind );
	if ( !config )
		return -1;

	// 已经采集时长
	g_army[army_index].appdata = g_map_res[index].kind;
	g_army[army_index].statetime += 1;
	g_army[army_index].gatherbuff += 1;
	if ( g_army[army_index].statetime >= g_army[army_index].stateduration )
	{// 采集完毕
		army_gather_calc( army_index );
		return -2;
	}
	return 0;
}

// 部队援助中
void army_help( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;

	// 已经援助时长
	g_army[army_index].statetime += 1;
	if ( g_army[army_index].statetime >= g_army[army_index].stateduration )
	{
		// 如果是士兵援助
		if ( g_army[army_index].action == ARMY_ACTION_HELP_TROOP )
		{
			army_setstate( army_index, ARMY_STATE_REBACK );
		}
	}
}

// 驻扎中
void army_occupy( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	if ( g_army[army_index].action == ARMY_ACTION_OCCUPY )
		return;
}

// 部队战斗
void army_fight( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;

	// 已经战斗时长
	g_army[army_index].statetime += 1;

	// 暂定播放N秒动画
	if ( g_army[army_index].statetime > ARMY_STATE_FIGHT_TIME )
	{
		// 删除军情警告
		city_underfire_del( army_getcityptr_target( army_index ), army_index );

		// 开始战斗
		if ( fight_start( army_index, g_army[army_index].to_type, g_army[army_index].to_index ) < 0 )
		{
			if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
			{
				army_delete( army_index );
			}
			else
			{
				army_setstate( army_index, ARMY_STATE_REBACK );
			}
			write_gamelog( "[FIGHT_START_ERROR]_to_type:%d", g_army[army_index].to_type );
			return;
		}

		// 战斗结算
		fight_lost_calc();

		// 攻击方是城池的部队
		if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
		{
			// 防御方是城池
			if ( g_army[army_index].to_type == MAPUNIT_TYPE_CITY )
			{
				//army_vs_city( army_index, army_getcityptr_target( army_index ) );
			}
			// 防御方是驻扎的部队
			else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ARMY )
			{
				//army_vs_army( army_index, g_army[army_index].target_index );
			}
			// 防御方是城镇
			else if ( g_army[army_index].to_type == MAPUNIT_TYPE_TOWN )
			{
				//army_vs_town( army_index, g_army[army_index].target_index );
			}
			// 防御方是怪物
			else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ENEMY )
			{
				army_vs_enemy( army_index, &g_fight );
			}
			// 防御方是资源点
			else if ( g_army[army_index].to_type == MAPUNIT_TYPE_RES )
			{
				army_vs_res( army_index, &g_fight );
			}

		}

		//	// 通知：战斗结果
		//	City *pActorCity = army_getcityptr( army_index );
		//	if ( pActorCity )
		//	{
		//		if ( g_fight.result == FIGHT_WIN )
		//			actor_system_message( pActorCity->actor_index, 65 );
		//		else
		//			actor_system_message( pActorCity->actor_index, 66 );
		//	}
		//}
		//// 攻击方是野外城镇
		//else if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
		//{
		//	// 防御方是玩家城池
		//	City *pTargetCity = army_getcityptr_target( army_index );
		//	if ( pTargetCity && g_army[army_index].appdata <= 0 )
		//	{ // 属于大地图争抢要塞玩法 
		//		army_vs_city( army_index, pTargetCity );

		//		// 保护罩时间
		//		int cuttime = 0;
		//		if ( g_fight.result == FIGHT_LOSE )
		//		{ // 攻击失败，玩家守护胜利，保护罩缩短4个小时分钟
		//			cuttime = -60 * 4;
		//		}

		//		// 添加城镇事件要在之前，位置不要动
		//		map_town_addevent( g_army[army_index].from_index, 1, pTargetCity, g_fight.result, 4, 0 );

		//		// 设置保护时间
		//		map_town_protecttime( g_army[army_index].from_index, cuttime );
		//	}
		//	else if ( pTargetCity && g_army[army_index].appdata > 0 )
		//	{ // 属于NPC攻城事件
		//		attack_event_vs_city( army_index, pTargetCity );
		//	}
		//}
		//// 攻击方是庇护所
		//else if ( g_army[army_index].from_type == MAPUNIT_TYPE_SHELTER )
		//{
		//	// 巨龙之怒活动
		//	activity_dragon_fightover( army_index, &g_fight );
		//}

		//// 城镇的部队都直接删除
		//if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN || g_army[army_index].from_type == MAPUNIT_TYPE_SHELTER )
		//{
		//	army_delete( army_index );
		//	return;
		//}

		if ( g_army[army_index].state == ARMY_STATE_GATHER )
		{ // 占领采集点胜利了
			return;
		}
		army_setstate( army_index, ARMY_STATE_REBACK );
	}
}

// 检查部队是不是自己的
int army_myself( int actor_index, int army_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;

	City *pMyCity = city_getptr( actor_index );
	if ( !pMyCity )
		return -1;

	City *pCity = army_getcityptr( army_index );
	if ( !pCity )
		return -1;

	if ( pMyCity->actorid != pCity->actorid )
		return -1;
	return 0;
}

