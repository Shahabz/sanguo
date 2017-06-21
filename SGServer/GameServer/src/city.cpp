#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "global.h"
#include "actor.h"
#include "city.h"
#include "building.h"
#include "hero.h"
#include "equip.h"
#include "mapunit.h"
#include "map.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;
extern Map g_map;

extern Actor *g_actors;
extern int g_maxactornum;

extern int g_city_maxindex;
City *g_city = NULL;
int g_city_maxcount = 0;
char g_city_allinited = 0;

// 城池数据重置
int city_reset()
{
	memset( g_city, 0, sizeof(City)*g_city_maxcount );
	for ( int tmpi = 0; tmpi < g_city_maxcount; tmpi++ )
	{
		g_city[tmpi].index = -1;
		g_city[tmpi].unit_index = -1;
		g_city[tmpi].actor_index = -1;
	}
	return 0;
}

// 城池读档完毕的回调
int city_loadcb( int city_index )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return -1;
	// 读取普通建筑
	city_building_load_auto( g_city[city_index].actorid, city_index, building_getptr, "city_building" );

	// 读取兵营建筑
	city_building_barracks_load_auto( g_city[city_index].actorid, city_index, buildingbarracks_getptr, "city_building_barracks" );

	// 读取资源建筑
	city_building_res_load_auto( g_city[city_index].actorid, city_index, buildingres_getptr, "city_building_res" );

	// 读取上阵英雄
	actor_hero_load_auto( g_city[city_index].actorid, city_index, city_hero_getptr, "actor_hero" );

	// 读取上阵英雄的装备
	actor_equip_load_auto( g_city[city_index].actorid, city_index, city_equip_getptr, "actor_equip" );

	// 添加到地图显示单元
	g_city[city_index].unit_index = mapunit_add( MAPUNIT_TYPE_CITY, city_index );

	// 占地块信息添加到世界地图
	map_addobject( MAPUNIT_TYPE_CITY, g_city[city_index].posx, g_city[city_index].posy, MAPUNIT_TYPE_CITY );
	return 0;
}

// 服务器启动读取所有城池信息到内存
int city_load()
{
	g_city_maxcount = g_Config.max_citycount;
	g_city = (City*)malloc( sizeof(City)* g_city_maxcount );
	printf_msg( "City  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_city_maxcount, (sizeof(City)*g_city_maxcount) / 1024.0 / 1024.0, sizeof(City) / 1024.0 );
	city_reset();
	city_load_auto( city_indexptr, city_loadcb, "city" );
	g_city_allinited = 1;
	return 0;
}

// 服务器关闭，所有城池信息存到数据库
int city_save( FILE *fp )
{
	for ( int city_index = 0; city_index < g_city_maxcount; city_index++ )
	{
		printf_msg( "city_save %d/%d\r", city_index + 1, g_city_maxcount );
		if ( g_city[city_index].actorid <= 0 )
			continue;
		city_single_save( &g_city[city_index], fp );
	}
	return 0;
}

// 单城池存储
int city_single_save( City *pCity, FILE *fp )
{
	// 城池基本信息
	city_save_auto( pCity, "city", fp );

	// 普通建筑
	city_building_batch_save_auto( pCity->actorid, pCity->building, BUILDING_MAXNUM, "city_building", fp );

	// 兵营建筑
	city_building_barracks_batch_save_auto( pCity->actorid, pCity->building_barracks, BUILDING_BARRACKS_MAXNUM, "city_building_barracks", fp );

	// 资源建筑
	city_building_res_batch_save_auto( pCity->actorid, pCity->building_res, BUILDING_RES_MAXNUM, "city_building_res", fp );

	// 英雄
	actor_hero_batch_save_auto( pCity->hero, HERO_CITY_MAX, "actor_hero", fp );

	// 英雄装备
	for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind <= 0 )
			continue;
		actor_equip_batch_save_auto( pCity->hero[tmpi].equip, HEROEQUIP_MAX, "actor_equip", fp );
	}
	return 0;
}

// 获取这个玩家的城池属性
City *city_indexptr( int city_index )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	return &g_city[city_index];
}

// 获取这个玩家的城池属性
City *city_getptr( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return NULL;
	int city_index = g_actors[actor_index].city_index;
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	return &g_city[city_index];
}

// 获取这个城池属性
City *city_getptr_withactorid( int actorid )
{
	int city_index = city_getindex_withactorid( actorid );
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	return &g_city[city_index];
}

// 根据玩家id找到城池索引
int city_getindex_withactorid( int actorid )
{
	int city_index = -1;
	for ( int tmpi = 0; tmpi < g_city_maxindex/*注意：使用索引位置，为了效率*/; tmpi++ )
	{
		if ( g_city[tmpi].actorid < MINACTORID )
			continue;
		if ( g_city[tmpi].actorid == actorid )
		{
			city_index = tmpi;
			break;
		}
	}
	return city_index;
}

// 创建一个新城市
int city_new( City *pCity )
{
	if ( pCity == NULL )
		return -1;
	int city_index = -1;
	for ( city_index = 1; city_index < g_city_maxcount; city_index++ )
	{
		if ( g_city[city_index].actorid <= 0 )
		{
			memcpy( &g_city[city_index], pCity, sizeof(City) );
			g_city[city_index].index = city_index;
			g_city[city_index].actor_index = -1;
			g_city[city_index].unit_index = -1;
			break;
		}
	}

	// 没创建成功
	if ( city_index >= g_city_maxcount )
		return -1;

	// 为了遍历的效率，计算最大的索引
	if ( city_index >= g_city_maxindex )
	{
		g_city_maxindex = city_index + 1;
	}

	// 存档
	city_save_auto( &g_city[city_index], "city", NULL );
	return city_index;
}
