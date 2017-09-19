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

//extern Army *g_army;
//extern int g_army_maxcount;
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

	//// 计算临时变量
	//if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	//{ // 出发是城池
	//	int city_index = city_getindex_withactorid( g_army[army_index].from_id );
	//	g_army[army_index].from_index = city_index;
	//}
	//else if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
	//{ // 出发是城镇
	//	g_army[army_index].from_index = g_army[army_index].from_id;
	//}

	//if ( g_army[army_index].to_type == MAPUNIT_TYPE_CITY )
	//{ // 目的是城池
	//	g_army[army_index].to_index = city_getindex_withactorid( g_army[army_index].to_id );
	//}
	//else if ( g_army[army_index].to_type == MAPUNIT_TYPE_TOWN )
	//{ // 目的是城镇
	//	g_army[army_index].to_index = g_army[army_index].to_id;
	//}


	//// 目标是玩家
	//City *pTargetCity = army_getcityptr_target( army_index );
	//if ( pTargetCity )
	//{
	//	// 城战状态

	//}
	//else
	//{

	//}
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
int armygroup_create( char from_type, int from_id, char to_type, int to_id, int stateduration )
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

		armygroup_delete( group_index );
	}
}
