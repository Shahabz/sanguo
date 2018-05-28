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
#include "city.h"
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
#include "map_zone.h"
#include "map_pickup.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern MapZoneInfo *g_zoneinfo;
extern int g_zoneinfo_maxnum;

extern MapPickupInfo *g_pickupinfo;
extern int g_pickupinfo_maxnum;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;

MapPickup *g_map_pickup = NULL;
int g_map_pickup_maxcount = 0;
extern int g_map_pickup_maxindex;

int g_pickup_lastfree_index = -1;

// 读档完毕的回调
int map_pickup_loadcb( int index )
{
	if ( index < 0 || index >= g_map_pickup_maxcount )
		return -1;

	// 添加到地图显示单元
	g_map_pickup[index].unit_index = mapunit_add( MAPUNIT_TYPE_PICKUP, index );

	// 占地块信息添加到世界地图
	map_addobject( MAPUNIT_TYPE_PICKUP, index, g_map_pickup[index].posx, g_map_pickup[index].posy );
	return 0;
}

int map_pickup_load()
{
	g_map_pickup = (MapPickup*)malloc( sizeof( MapPickup ) * g_map_pickup_maxcount );
	memset( g_map_pickup, 0, sizeof( MapPickup ) * g_map_pickup_maxcount );
	for ( int tmpi = 0; tmpi < g_map_pickup_maxcount; tmpi++ )
	{
		g_map_pickup[tmpi].unit_index = -1;
	}
	g_pickup_lastfree_index = -1;

	printf_msg( "MapPickup  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_map_pickup_maxcount, (sizeof( MapPickup )*g_map_pickup_maxcount) / 1024.0 / 1024.0, sizeof( MapPickup ) / 1024.0 );
	map_pickup_load_auto( map_pickup_getptr, map_pickup_loadcb, "map_pickup" );
	return 0;
}

int map_pickup_save( FILE *fp )
{
	map_pickup_batch_save_auto( g_map_pickup, g_map_pickup_maxcount, "map_pickup", fp );
	return 0;
}

int map_pickup_maxcount_set( int maxcount )
{
	g_map_pickup_maxcount = maxcount;
	return 0;
}

MapPickup *map_pickup_getptr( int index )
{
	if ( index < 0 || index >= g_map_pickup_maxcount )
		return NULL;
	return &g_map_pickup[index];
}

MapPickupInfo *map_pickup_getconfig( int kind )
{
	if ( kind < 0 || kind >= g_pickupinfo_maxnum )
		return NULL;
	return &g_pickupinfo[kind];
}

// 显示单元属性
void map_pickup_makeunit( int index, SLK_NetS_AddMapUnit *pAttr )
{
	if ( index < 0 || index >= g_map_pickup_maxcount )
		return;
	MapPickupInfo *config = map_pickup_getconfig( g_map_pickup[index].kind );
	if ( !config )
		return;
	pAttr->m_posx = g_map_pickup[index].posx;
	pAttr->m_posy = g_map_pickup[index].posy;
	pAttr->m_short_value[0] = g_map_pickup[index].kind;
	pAttr->m_short_value_count = 1;
}

// 位置
void map_pickup_getpos( int index, short *posx, short *posy )
{
	if ( index < 0 || index >= g_map_pickup_maxcount )
		return;
	*posx = g_map_pickup[index].posx;
	*posy = g_map_pickup[index].posy;
}

// 获取一个空闲索引
int map_pickup_getfreeindex()
{
	int freeindex = -1;
	if ( g_pickup_lastfree_index < 0 || g_pickup_lastfree_index >= g_map_pickup_maxcount - 1 )
	{
		for ( int index = 0; index < g_map_pickup_maxcount; index++ )
		{
			if ( g_map_pickup[index].kind <= 0 )
			{
				freeindex = index;
				g_pickup_lastfree_index = index + 1;
				break;
			}
		}
	}
	else
	{
		for ( int index = g_pickup_lastfree_index; index < g_map_pickup_maxcount; index++ )
		{ // 这里注意，用途是提升查找空闲索引效率
			if ( g_map_pickup[index].kind <= 0 )
			{
				freeindex = index;
				g_pickup_lastfree_index = index + 1;
				break;
			}
		}
	}

	return freeindex;
}

// 创建一个拾取物品
int map_pickup_create( short kind, short posx, short posy )
{
	if ( kind <= 0 || kind >= g_pickupinfo_maxnum )
		return -1;
	int index = map_pickup_getfreeindex();
	if ( index < 0 )
		return -1;
	g_map_pickup[index].index = index;
	g_map_pickup[index].kind = kind;
	g_map_pickup[index].posx = posx;
	g_map_pickup[index].posy = posy;
	g_map_pickup[index].unit_index = mapunit_add( MAPUNIT_TYPE_PICKUP, index );
	map_addobject( MAPUNIT_TYPE_PICKUP, index, posx, posy );
	return g_map_pickup[index].unit_index;
}

// 删除一个拾取物品
int map_pickup_delete( int index )
{
	if ( index < 0 || index >= g_map_pickup_maxcount )
		return -1;
	mapunit_del( MAPUNIT_TYPE_PICKUP, index, g_map_pickup[index].unit_index );
	map_delobject( MAPUNIT_TYPE_PICKUP, index, g_map_pickup[index].posx, g_map_pickup[index].posy );
	memset( &g_map_pickup[index], 0, sizeof( MapPickup ) );
	g_map_pickup[index].unit_index = -1;
	return 0;
}

// 拾取物品数量
int map_pickup_num( int zoneid, int kind )
{
	int num = 0;
	for ( int tmpi = 0; tmpi < g_map_pickup_maxcount; tmpi++ )
	{
		if ( g_map_pickup[tmpi].kind != kind )
			continue;
		if ( map_zone_inrange( zoneid, g_map_pickup[tmpi].posx, g_map_pickup[tmpi].posy ) == 1 )
		{
			num += 1;
		}
	}
	return num;
}

// 拾取
int map_pickup_get( int actor_index, int unit_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;
	MapUnit *unit = &g_mapunit[unit_index];
	if ( !unit )
		return -1;
	if ( unit->type != MAPUNIT_TYPE_PICKUP )
		return -1;
	if ( unit->index < 0 || unit->index >= g_map_pickup_maxcount )
		return -1;
	if ( g_map_pickup[unit->index].kind == 0 )
	{// 手慢了，已经被其它主公捷足先登了！
		actor_notify_alert( actor_index, 3052 );
		return -1;
	}
	MapPickupInfo *config = map_pickup_getconfig( g_map_pickup[unit->index].kind );
	if ( !config )
		return -1;
	if ( config->awardkind < 0 )
	{
		awardgroup_withindex( actor_index, -config->awardkind, -1, PATH_MAPPICKUP, NULL );
	}
	else
	{
		int num = 0;
		if ( config->numtype == 0 )
		{
			num = random( config->minnum, config->maxnum );
		}
		else if( config->numtype == 1 )
		{
			num = g_actors[actor_index].level * config->minnum + config->maxnum;
		}
		else if ( config->numtype == 2 )
		{
			num = pCity->building[0].level * config->minnum + config->maxnum;
		}
		award_getaward( actor_index, config->awardkind, num, -1, PATH_MAPPICKUP, NULL );
	}
	map_pickup_delete( unit->index );
	return 0;
}