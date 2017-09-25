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
#include "city.h"
#include "map_town.h"
#include "map_enemy.h"
#include "map_res.h"
#include "map.h"
#include "mail.h"
#include "chat.h"

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
extern Fight g_fight;

extern Army *g_army;
extern int g_army_maxcount;
//extern int g_army_count;
//extern int g_army_maxindex;

ArmyGroup *g_armygroup = NULL;
int g_armygroup_maxcount = 0;
int g_armygroup_count = 0;
extern int g_army_group_maxindex;

// 读档完毕的回调
int armygroup_loadcb( int group_index )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;

	if ( g_armygroup[group_index].from_type == MAPUNIT_TYPE_CITY )
	{ // 出发是城池
		int city_index = city_getindex_withactorid( g_armygroup[group_index].from_id );
		g_armygroup[group_index].from_index = city_index;
	}
	else if ( g_armygroup[group_index].from_type == MAPUNIT_TYPE_TOWN )
	{ // 出发是城镇
		int index = g_armygroup[group_index].from_id;
		g_armygroup[group_index].from_index = index;
	}

	// 目的数据
	if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_CITY )
	{ // 目的是城池
		int city_index = city_getindex_withactorid( g_armygroup[group_index].to_id );
		g_armygroup[group_index].to_index = city_index;
		City *pTargetCity = city_indexptr( city_index );
		if ( pTargetCity )
		{
			// 添加被攻击列表
			city_underfire_groupadd( pTargetCity, group_index );
	
			// 城战状态
			city_setstate( pTargetCity, CITY_STATE_ARMYGROUP );
		}
	}
	else if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_TOWN )
	{ // 目的是城镇
		int index = g_armygroup[group_index].to_id;
		g_armygroup[group_index].to_index = index;
	}

	g_armygroup_count += 1;
	return 0;
}

int armygroup_load()
{
	g_armygroup_maxcount = g_Config.max_citycount * 1;
	g_armygroup = (ArmyGroup*)malloc( sizeof( ArmyGroup ) * g_armygroup_maxcount );
	memset( g_armygroup, 0, sizeof( ArmyGroup ) * g_armygroup_maxcount );
	printf_msg( "ArmyGroup  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_armygroup_maxcount, (sizeof( ArmyGroup )*g_armygroup_maxcount) / 1024.0 / 1024.0, sizeof( ArmyGroup ) / 1024.0 );
	army_group_load_auto( armygroup_getptr, armygroup_loadcb, "map_army_group" );
	return 0;
}

int armygroup_save( FILE *fp )
{
	for ( int tmpi = 0; tmpi < g_armygroup_maxcount; tmpi++ )
	{
		if ( g_armygroup[tmpi].id <= 0 )
			continue;
		army_group_save_auto( &g_armygroup[tmpi], "map_army_group", fp );
	}
	return 0;
}

// 数据库删除一只部队
void armygroup_del_db( int group_index )
{
	char szSQL[256] = { 0 };
	sprintf( szSQL, "DELETE FROM `map_army_group` WHERE `index`='%d'", group_index );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
	}
	return;
}

ArmyGroup *armygroup_getptr( int group_index )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return NULL;
	return &g_armygroup[group_index];
}

// 获取空闲army_index
int armygroup_getfreeindex()
{
	for ( int tmpi = 0; tmpi < g_armygroup_maxcount; tmpi++ )
	{
		if ( g_armygroup[tmpi].id <= 0 )
		{
			return tmpi;
		}
	}
	return -1;
}

// 创建一个集结
int armygroup_create( char type, char from_type, int from_id, char to_type, int to_id, int stateduration )
{
	int group_index = armygroup_getfreeindex();
	if ( group_index < 0 )
		return -1;
	// 为了遍历的效率，计算最大的索引
	if ( group_index >= g_army_group_maxindex )
	{
		g_army_group_maxindex = group_index + 1;
	}

	memset( &g_armygroup[group_index], 0, sizeof( ArmyGroup ) );
	g_armygroup[group_index].index = group_index;
	g_armygroup[group_index].from_index = -1;
	g_armygroup[group_index].to_index = -1;
	for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
	{
		g_armygroup[group_index].attack_armyindex[tmpi] = -1;
		g_armygroup[group_index].defense_armyindex[tmpi] = -1;
	}

	g_armygroup[group_index].id = (int)time( NULL );
	g_armygroup[group_index].state = ARMYGROUP_STATE_ING;
	g_armygroup[group_index].statetime = 0;
	g_armygroup[group_index].stateduration = stateduration;
	g_armygroup[group_index].type = type;

	// 出发数据
	g_armygroup[group_index].from_type = from_type;
	g_armygroup[group_index].from_id = from_id;
	if ( g_armygroup[group_index].from_type == MAPUNIT_TYPE_CITY )
	{ // 出发是城池
		int city_index = city_getindex_withactorid( from_id );
		g_armygroup[group_index].from_index = city_index;
		city_getpos( city_index, &g_armygroup[group_index].from_posx, &g_armygroup[group_index].from_posy );
	}
	else if ( g_armygroup[group_index].from_type == MAPUNIT_TYPE_TOWN )
	{ // 出发是城镇
		int index = g_armygroup[group_index].from_id;
		g_armygroup[group_index].from_index = index;
		map_town_getpos( index, &g_armygroup[group_index].from_posx, &g_armygroup[group_index].from_posy );
	}

	// 目的数据
	g_armygroup[group_index].to_type = to_type;
	g_armygroup[group_index].to_id = to_id;
	if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_CITY )
	{ // 目的是城池
		int city_index = city_getindex_withactorid( g_armygroup[group_index].to_id );
		g_armygroup[group_index].to_index = city_index;
		city_getpos( city_index, &g_armygroup[group_index].to_posx, &g_armygroup[group_index].to_posy );

		City *pTargetCity = city_indexptr( city_index );
		if ( pTargetCity )
		{
			// 添加被攻击列表
			city_underfire_groupadd( pTargetCity, group_index );

			// 城战状态
			city_setstate( pTargetCity, CITY_STATE_ARMYGROUP );
		}
	}
	else if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_TOWN )
	{ // 目的是城镇
		int index = g_armygroup[group_index].to_id;
		g_armygroup[group_index].to_index = index;
		map_town_getpos( index, &g_armygroup[group_index].to_posx, &g_armygroup[group_index].to_posy );
	}

	g_armygroup_count += 1;
	return group_index;
}

// 删除一个集结
void armygroup_delete( int group_index )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return;
	if ( g_armygroup[group_index].id <= 0 )
		return;

	int city_index = -1;
	// 目标是玩家
	if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_CITY )
	{
		city_index = g_armygroup[group_index].to_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
		{ // 删除被攻击列表
			city_underfire_groupdel( &g_city[city_index], group_index );
		}
	}
	for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
	{
		if ( g_armygroup[group_index].attack_armyindex[tmpi] >= 0 )
		{
			army_setstate( g_armygroup[group_index].attack_armyindex[tmpi], ARMY_STATE_REBACK );
		}

		if ( g_armygroup[group_index].defense_armyindex[tmpi] >= 0 )
		{
			army_setstate( g_armygroup[group_index].defense_armyindex[tmpi], ARMY_STATE_REBACK );
		}
	}
	armygroup_del_db( group_index );
	memset( &g_armygroup[group_index], 0, sizeof( ArmyGroup ) );
	g_armygroup[group_index].index = -1;
	g_armygroup[group_index].from_index = -1;
	g_armygroup[group_index].to_index = -1;
	for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
	{
		g_armygroup[group_index].attack_armyindex[tmpi] = -1;
		g_armygroup[group_index].defense_armyindex[tmpi] = -1;
	}
	g_armygroup_count -= 1;
}

// 所有集结逻辑
void armygroup_alllogic()
{
	for ( int tmpi = 0; tmpi < g_armygroup_maxcount; tmpi++ )
	{
		if ( g_armygroup[tmpi].id <= 0 )
			continue;
		armygroup_logic( tmpi );
	}
}

// 单只集结逻辑
void armygroup_logic( int group_index )
{
	g_armygroup[group_index].statetime += 1;
	if ( g_armygroup[group_index].statetime >= g_armygroup[group_index].stateduration )
	{ // 战斗开始
		if ( fight_start_armygroup( group_index ) >= 0 )
		{
			// 战损
			fight_lost_calc();

			if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_CITY )
			{
				armygroup_vs_city( group_index, &g_fight );
			}
			else if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_TOWN )
			{
				armygroup_vs_town( group_index, &g_fight );
			}
		}
		armygroup_delete( group_index );
	}
}

// 添加部队
int armygroup_addarmy( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	int group_index = g_army[army_index].group_index;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	if ( g_army[army_index].group_id != g_armygroup[group_index].id )
		return -1;

	if ( g_army[army_index].action == ARMY_ACTION_GROUP_CREATE || g_army[army_index].action == ARMY_ACTION_GROUP_ATTACK )
	{
		for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
		{
			if ( g_armygroup[group_index].attack_armyindex[tmpi] < 0 )
			{
				g_armygroup[group_index].attack_armyindex[tmpi] = army_index;
				g_army[army_index].group_pos = tmpi;
				return tmpi;
			}
		}
	}
	else
	{
		for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
		{
			if ( g_armygroup[group_index].defense_armyindex[tmpi] < 0 )
			{
				g_armygroup[group_index].defense_armyindex[tmpi] = army_index;
				g_army[army_index].group_pos = tmpi;
				return tmpi;
			}
		}
	}
	
	return -1;
}

// 删除部队
void armygroup_delarmy( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	int group_index = g_army[army_index].group_index;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return;
	if ( g_army[army_index].group_id != g_armygroup[group_index].id )
		return;

	if ( g_army[army_index].action == ARMY_ACTION_GROUP_CREATE || g_army[army_index].action == ARMY_ACTION_GROUP_ATTACK )
	{
		for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
		{
			if ( g_armygroup[group_index].attack_armyindex[tmpi] == army_index )
			{
				g_armygroup[group_index].attack_armyindex[tmpi] = -1;
				g_army[army_index].group_pos = -1;

				// 数据前移
				if ( tmpi < ARMYGROUP_MAXCOUNT - 1 )
				{
					memmove( &g_armygroup[group_index].attack_armyindex[tmpi], &g_armygroup[group_index].attack_armyindex[tmpi + 1], sizeof( int )*(ARMYGROUP_MAXCOUNT - 1) );
				}
				break;
			}
		}
	}
	else
	{
		for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
		{
			if ( g_armygroup[group_index].defense_armyindex[tmpi] == army_index )
			{
				g_armygroup[group_index].defense_armyindex[tmpi] = -1;
				g_army[army_index].group_pos = -1;

				// 数据前移
				if ( tmpi < ARMYGROUP_MAXCOUNT - 1 )
				{
					memmove( &g_armygroup[group_index].defense_armyindex[tmpi], &g_armygroup[group_index].defense_armyindex[tmpi + 1], sizeof( int )*(ARMYGROUP_MAXCOUNT - 1) );
				}
				break;
			}
		}
	}
}

// 设置队长
void armygroup_setleader( int group_index, int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return;
	g_army[army_index].group_index = group_index;
	g_army[army_index].group_id = g_armygroup[group_index].id;
	g_armygroup[group_index].leader_index = army_index;
}

// 检查是否具备自动解散条件
int armygroup_dismiss( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	int group_index = g_army[army_index].group_index;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	if ( g_army[army_index].group_id != g_armygroup[group_index].id )
		return -1;
	if ( g_armygroup[group_index].from_type != MAPUNIT_TYPE_CITY )
		return -1;

	City *pArmyCity = army_getcityptr( army_index );
	if ( !pArmyCity )
		return -1;
	
	City *pCity = city_indexptr( g_armygroup[group_index].from_index );
	if ( !pCity )
		return -1;

	// 不是发起者的部队，就不检查了
	if ( pArmyCity->actorid != pCity->actorid )
		return -1;
	
	// 检查发起者的部队，在这个集结里还存在不了，不存在，解散集结
	char has = 0;
	for ( int tmpi = 0; tmpi < CITY_BATTLEQUEUE_MAX; tmpi++ )
	{
		int index = pCity->battle_armyindex[tmpi];
		if ( index < 0 || index >= g_army_maxcount )
			continue;
		if ( g_army[index].state == ARMY_STATE_MARCH || g_army[index].state == ARMY_STATE_GROUP_END )
		{
			if ( g_army[index].to_type == MAPUNIT_TYPE_CITY && g_army[index].to_id == g_armygroup[group_index].to_id )
			{
				has = 1;
				break;
			}
		}
	}

	if ( has == 0 )
	{
		// 给双方发送集结撤退的邮件

		armygroup_delete( group_index );
	}
	return 0;
}

int armygroup_statetime( int group_index )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return 0;
	return g_armygroup[group_index].statetime;
}             

int armygroup_stateduration( int group_index )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return 0;
	return g_armygroup[group_index].stateduration;
}

// 攻击方兵力
int armygroup_from_totals( int group_index )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return 0;
	int total = 0;

	for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
	{
		int army_index = g_armygroup[group_index].attack_armyindex[tmpi];
		if ( army_index < 0 )
			continue;
		total += g_army[army_index].totals;
	}

	return total;
}

// 防御方兵力
int armygroup_to_totals( int group_index )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return 0;
	int total = 0;

	if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_CITY )
	{
		City *pCity = city_indexptr( g_armygroup[group_index].to_index );
		if ( pCity )
		{
			// 城墙部队
			for ( int tmpi = 0; tmpi < CITY_GUARD_MAX; tmpi++ )
			{
				int monsterid = pCity->guard[tmpi].monsterid;
				if ( monsterid <= 0 )
					continue;
				total += pCity->guard[tmpi].soldiers;
			}

			// 玩家主力部队
			for ( int tmpi = 0; tmpi < 4; tmpi++ )
			{
				total += pCity->hero[tmpi].soldiers;
			}

			// 羽林卫部队
			for ( int tmpi = 8; tmpi < 12; tmpi++ )
			{
				total += pCity->hero[tmpi].soldiers;
			}

			// 驻防部队
			for ( int index = 0; index < CITY_HELPDEFENSE_MAX; index++ )
			{
				int army_index = pCity->help_armyindex[index];
				if ( army_index < 0 )
					continue;
				if ( g_army[army_index].state != ARMY_STATE_HELP )
					continue;
				total += g_army[army_index].totals;
			}
		}
	}
	// 协防部队
	for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
	{
		int army_index = g_armygroup[group_index].attack_armyindex[tmpi];
		if ( army_index < 0 )
			continue;
		total += g_army[army_index].totals;
	}
	return total;
}

// 城战结果
int armygroup_vs_city( int group_index, Fight *pFight )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;

	City *pCity = city_indexptr( g_armygroup[group_index].from_index );
	if ( !pCity )
		return -1;

	City *pTargetCity = city_indexptr( g_armygroup[group_index].to_index );
	if ( !pTargetCity )
		return -1;

	i64 mailid = 0;
	char title[MAIL_TITLE_MAXSIZE] = { 0 };
	char content[MAIL_CONTENT_MAXSIZE] = { 0 };

	if ( pFight->result == FIGHT_WIN )
	{
		// 掠夺
		int atk_protect = 0;
		int def_protect = 0;
		int lost_silver = 0;
		int lost_wood = 0;
		int lost_food = 0;
		int lost_people = 0;
		int rob_silver = 0;
		int rob_wood = 0;
		int rob_food = 0;
		int rob_people = 0;

		// 银币
		atk_protect = building_store_protect( pCity, 1 );
		def_protect = building_store_protect( pCity, 1 );
		lost_silver = (int)ceil( abs( pTargetCity->silver - def_protect ) * global.cityfight_rob_v1 );
		if ( pCity->silver > atk_protect )
		{
			rob_silver = (int)ceil( lost_silver * global.cityfight_rob_v2 );
		}
		else
		{
			rob_silver = lost_silver;
		}

		// 木材
		atk_protect = building_store_protect( pCity, 2 );
		def_protect = building_store_protect( pCity, 2 );
		lost_wood = (int)ceil( abs( pTargetCity->wood - def_protect ) * global.cityfight_rob_v1 );
		if ( pCity->wood > atk_protect )
		{
			rob_wood = (int)ceil( lost_wood * global.cityfight_rob_v2 );
		}
		else
		{
			rob_wood = lost_wood;
		}

		// 粮食
		atk_protect = building_store_protect( pCity, 3 );
		def_protect = building_store_protect( pCity, 3 );
		lost_food = (int)ceil( abs( pTargetCity->food - def_protect ) * global.cityfight_rob_v1 );
		if ( pCity->food > atk_protect )
		{
			rob_food = (int)ceil( lost_food * global.cityfight_rob_v2 );
		}
		else
		{
			rob_food = lost_food;
		}

		// 人口
		lost_people = min( ceil(pTargetCity->people * global.cityfight_rob_v3), building_people_max( pTargetCity ) );
		rob_people = ceil( lost_people * global.cityfight_rob_v4 );

		// 损失
		city_changesilver( pTargetCity->index, -lost_silver, PATH_FIGHT );
		city_changewood( pTargetCity->index, -lost_wood, PATH_FIGHT );
		city_changefood( pTargetCity->index, -lost_food, PATH_FIGHT );
		city_changepeople( pTargetCity->index, -lost_people, PATH_FIGHT );

		// 掠夺
		for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
		{ // 找到这个城池的部队
			int army_index = g_armygroup[group_index].attack_armyindex[tmpi];
			if ( army_index < 0 )
				continue;
			City *pArmyCity = army_getcityptr( army_index );
			if ( !pArmyCity )
				continue;
			if ( pArmyCity->actorid == pCity->actorid )
			{
				army_carry_additem( army_index, AWARDKIND_SILVER, rob_silver );
				army_carry_additem( army_index, AWARDKIND_WOOD, rob_wood );
				army_carry_additem( army_index, AWARDKIND_FOOD, rob_food );
				army_carry_additem( army_index, AWARDKIND_PEOPLE, rob_people );
				break;
			}
		}

		// 进攻成功邮件
		sprintf( title, "%s%d", TAG_TEXTID, 5019 );// 城战进攻胜利
		sprintf( content, "{\"my\":1,\"win\":1,\"na\":\"%s\",\"n\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tna\":\"%s\",\"tn\":%d,\"tlv\":%d,\"tpos\":\"%d,%d\",\"silver\":%d,\"wood\":%d,\"food\":%d,\"people\":%d}",
			pCity->name, pCity->nation, pCity->level, pCity->posx, pCity->posy, pTargetCity->name, pTargetCity->nation, pTargetCity->level, g_armygroup[group_index].to_posx, g_armygroup[group_index].to_posy, rob_silver, rob_wood, rob_food, rob_people );

		// 集结所有人发送邮件
		armygroup_mail( group_index, 1, NULL, MAIL_TYPE_FIGHT_CITY, title, content, "", pFight );


		// 防守失败邮件
		sprintf( title, "%s%d", TAG_TEXTID, 5022 );// 城战防守失败
		sprintf( content, "{\"my\":2,\"win\":0,\"na\":\"%s\",\"n\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tna\":\"%s\",\"tn\":%d,\"tlv\":%d,\"tpos\":\"%d,%d\",\"silver\":%d,\"wood\":%d,\"food\":%d,\"people\":%d}",
			pTargetCity->name, pTargetCity->nation, pTargetCity->level, g_armygroup[group_index].to_posx, g_armygroup[group_index].to_posy, pCity->name, pCity->nation, pCity->level, pCity->posx, pCity->posy, lost_silver, lost_wood, lost_food, lost_people );

		// 集结所有人发送邮件
		armygroup_mail( group_index, 2, pTargetCity, MAIL_TYPE_FIGHT_CITY, title, content, "", pFight );

		// 随机迁移城主地图
		char zoneid = map_zone_getid( pTargetCity->posx, pTargetCity->posy );
		if ( zoneid > 0 && zoneid < g_zoneinfo_maxnum )
		{
			short move_posx, move_posy;
			map_zone_randpos( zoneid, &move_posx, &move_posy );
			if ( map_canmove( move_posx, move_posy ) == 1 )
			{
				city_move( pTargetCity, move_posx, move_posy );

				// 给城主发送击飞邮件
				mail_system( pTargetCity->actor_index, pTargetCity->actorid, 5023, 5521, pCity->name, NULL, "" );
			}
		}
	}
	else
	{
		// 进攻失败邮件
		sprintf( title, "%s%d", TAG_TEXTID, 5020 ); // 城战进攻失败
		sprintf( content, "{\"my\":1,\"win\":0,\"na\":\"%s\",\"n\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tna\":\"%s\",\"tn\":%d,\"tlv\":%d,\"tpos\":\"%d,%d\"}",
			pCity->name, pCity->nation, pCity->level, pCity->posx, pCity->posy, pTargetCity->name, pTargetCity->nation, pTargetCity->level, g_armygroup[group_index].to_posx, g_armygroup[group_index].to_posy );

		// 集结所有人发送邮件
		armygroup_mail( group_index, 1, NULL, MAIL_TYPE_FIGHT_CITY, title, content, "", pFight );

		// 防守成功邮件
		sprintf( title, "%s%d", TAG_TEXTID, 5021 );// 城战防守胜利
		sprintf( content, "{\"my\":2,\"win\":1,\"na\":\"%s\",\"n\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tna\":\"%s\",\"tn\":%d,\"tlv\":%d,\"tpos\":\"%d,%d\"}",
			pTargetCity->name, pTargetCity->nation, pTargetCity->level, g_armygroup[group_index].to_posx, g_armygroup[group_index].to_posy, pCity->name, pCity->nation, pCity->level, pCity->posx, pCity->posy );

		// 集结所有人发送邮件
		armygroup_mail( group_index, 2, pTargetCity, MAIL_TYPE_FIGHT_CITY, title, content, "", pFight );
	}

	return 0;
}

// 国战战结果
int armygroup_vs_town( int group_index, Fight *pFight )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;

	return 0;
}

// 城战列表
int armygroup_city_sendlist( int actor_index, int unit_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	MapUnit *unit = &g_mapunit[unit_index];
	if ( !unit )
		return -1;
	if ( unit->type != MAPUNIT_TYPE_CITY )
		return -1;
	if ( unit->index < 0 || unit->index >= g_city_maxcount )
		return -1;
	City *pTargetCity = &g_city[unit->index];
	if ( !pTargetCity )
		return -1;

	SLK_NetS_CityArmyGroupList pValue = { 0 };
	pValue.m_unit_index = unit_index;
	pValue.m_nation = pTargetCity->nation;
	
	// 发送开始
	pValue.m_flag = 0;
	netsend_cityarmygrouplist_S( actor_index, SENDTYPE_ACTOR, &pValue );

	for ( int tmpi = 0; tmpi < g_armygroup_maxcount; tmpi++ )
	{
		if ( g_armygroup[tmpi].id <= 0 )
			continue;
		if ( g_armygroup[tmpi].from_type != MAPUNIT_TYPE_CITY || g_armygroup[tmpi].to_type != MAPUNIT_TYPE_CITY )
			continue;
		// 攻击方玩家
		City *pAtkCity = city_indexptr( g_armygroup[tmpi].from_index );
		if ( !pAtkCity )
			continue;

		// 防御方玩家
		City *pDefCity = city_indexptr( g_armygroup[tmpi].to_index ); 
		if ( !pDefCity )
			continue;

		// 我既不是攻击国也不是防御国
		if ( pCity->nation != pAtkCity->nation && pCity->nation != pDefCity->nation )
			continue;

		if ( g_armygroup[tmpi].from_id == pTargetCity->actorid )
		{ // 我看的人属于攻击方

			City *pDefCity = city_indexptr( g_armygroup[tmpi].to_index );
			if ( !pDefCity )
				continue;
			pValue.m_list[pValue.m_count].m_attack = 1;

			pValue.m_list[pValue.m_count].m_nation = pTargetCity->nation;
			pValue.m_list[pValue.m_count].m_level = city_mainlevel( pTargetCity->index );
			strncpy( pValue.m_list[pValue.m_count].m_name, pTargetCity->name, NAME_SIZE );
			pValue.m_list[pValue.m_count].m_name_length = strlen( pValue.m_list[pValue.m_count].m_name );
			pValue.m_list[pValue.m_count].m_posx = pTargetCity->posx;
			pValue.m_list[pValue.m_count].m_posy = pTargetCity->posy;
			pValue.m_list[pValue.m_count].m_actorid = pTargetCity->actorid;
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
		else if ( g_armygroup[tmpi].to_id == pTargetCity->actorid )
		{ // 我看的人属于防御方
			City *pAtkCity = city_indexptr( g_armygroup[tmpi].from_index );
			if ( !pAtkCity )
				continue;
			pValue.m_list[pValue.m_count].m_attack = 2;

			pValue.m_list[pValue.m_count].m_nation = pTargetCity->nation;
			pValue.m_list[pValue.m_count].m_level = city_mainlevel( pTargetCity->index );
			strncpy( pValue.m_list[pValue.m_count].m_name, pTargetCity->name, NAME_SIZE );
			pValue.m_list[pValue.m_count].m_name_length = strlen( pValue.m_list[pValue.m_count].m_name );
			pValue.m_list[pValue.m_count].m_posx = pTargetCity->posx;
			pValue.m_list[pValue.m_count].m_posy = pTargetCity->posy;
			pValue.m_list[pValue.m_count].m_actorid = pTargetCity->actorid;
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
		else
			continue;

		pValue.m_list[pValue.m_count].m_group_index = tmpi;
		pValue.m_list[pValue.m_count].m_group_id = g_armygroup[tmpi].id;
		pValue.m_list[pValue.m_count].m_statetime = g_armygroup[tmpi].statetime;
		pValue.m_list[pValue.m_count].m_stateduration = g_armygroup[tmpi].stateduration;
		pValue.m_list[pValue.m_count].m_type = g_armygroup[tmpi].type;
		pValue.m_totalcount += 1;
		pValue.m_count += 1;
		if ( pValue.m_count > 10 )
		{
			pValue.m_flag = 1;
			netsend_cityarmygrouplist_S( actor_index, SENDTYPE_ACTOR, &pValue );
			pValue.m_count = 0;
		}
	}

	if ( pValue.m_count > 0 )
	{
		pValue.m_flag = 1;
		netsend_cityarmygrouplist_S( actor_index, SENDTYPE_ACTOR, &pValue );
		pValue.m_count = 0;
	}

	pValue.m_flag = 2;
	netsend_cityarmygrouplist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 国战列表
int armygroup_town_sendlist( int actor_index, int unit_index )
{
	ACTOR_CHECK_INDEX( actor_index );

	return 0;
}

// 请求帮助
int armygroup_askhelp( int actor_index, int group_index, int group_id )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	if ( g_armygroup[group_index].id != group_id )
		return -1;

	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	char msg[127] = { 0 };
	if ( pCity->actorid == g_armygroup[group_index].from_id )
	{// 请求者是攻击方
		if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_CITY )
		{
			// 对方的城池
			City *pTargetCity = city_indexptr( g_armygroup[group_index].to_index );
			if ( !pTargetCity )
				return -1;
			// 我对{ 0 }的{ 1 }<color = d95df4ff><url = { 2 }>[{2}]< / url>< / color>发起了城战，请求支援！
			snprintf( msg, 127, "{\"n\":%d,\"na\":\"%s\",\"pos\":\"%d,%d\"}", pTargetCity->nation, pTargetCity->name, pTargetCity->posx, pTargetCity->posy );
			chat_actortalk( actor_index, CHAT_CHANNEL_NATION, CHAT_MSGTYPE_ATTACK_ASKHELP, msg );
		}

	}
	else if ( pCity->actorid == g_armygroup[group_index].to_id )
	{// 请求者是防御方
		if ( g_armygroup[group_index].from_type == MAPUNIT_TYPE_CITY )
		{
			// 对方的城池
			City *pTargetCity = city_indexptr( g_armygroup[group_index].from_index );
			if ( !pTargetCity )
				return -1;

			// 我的城池<color = d95df4ff><url = { 0 }>[{0}]< / url>< / color>被{ 1 }的{ 2 }发起了城战，请求支援！
			snprintf( msg, 127, "{\"n\":%d,\"na\":\"%s\",\"pos\":\"%d,%d\"}", pTargetCity->nation, pTargetCity->name, pCity->posx, pCity->posy );
			chat_actortalk( actor_index, CHAT_CHANNEL_NATION, CHAT_MSGTYPE_DEFENSE_ASKHELP, msg );
		}
	}
	return 0;
}

// 集结所有人发送邮件
int armygroup_mail( int group_index, char attack, City *defenseCity, char type, char *title, char *content, char *attach, Fight *fight )
{
	if ( attack == 1 )
	{
		for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
		{
			int army_index = g_armygroup[group_index].attack_armyindex[tmpi];
			if ( army_index < 0 )
				continue;
			City *pArmyCity = army_getcityptr( army_index );
			if ( !pArmyCity )
				continue;
			i64 mailid = mail( pArmyCity->actor_index, pArmyCity->actorid, type, title, content, "", 0 );
			if ( fight )
			{
				mail_fight( mailid, pArmyCity->actorid, fight->unit_json );
			}
		}
	}
	else if( attack == 2 )
	{
		for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
		{
			int army_index = g_armygroup[group_index].defense_armyindex[tmpi];
			if ( army_index < 0 )
				continue;
			City *pArmyCity = army_getcityptr( army_index );
			if ( !pArmyCity )
				continue;
			i64 mailid = mail( pArmyCity->actor_index, pArmyCity->actorid, type, title, content, "", 0 );
			if ( fight )
			{
				mail_fight( mailid, pArmyCity->actorid, fight->unit_json );
			}
		}

		if ( defenseCity )
		{
			i64 mailid = mail( defenseCity->actor_index, defenseCity->actorid, type, title, content, "", 0 );
			if ( fight )
			{
				mail_fight( mailid, defenseCity->actorid, fight->unit_json );
			}
		}
	}
	return 0;
}

