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
#include "wqueue.h"
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
#include "city.h"
#include "army.h"
#include "map.h"
#include "map_event.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern City * g_city;
extern int g_city_maxcount;

extern MapZoneInfo *g_zoneinfo;
extern int g_zoneinfo_maxnum;

extern MapEventInfo *g_eventinfo;
extern int g_eventinfo_maxnum;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;

MapEvent *g_map_event = NULL;
int g_map_event_maxcount = 0;
extern int g_map_event_maxindex;

int g_event_lastfree_index = -1;

// 读档完毕的回调
int map_event_loadcb( int index )
{
	if ( index < 0 || index >= g_map_event_maxcount )
		return -1;

	// 添加到地图显示单元
	g_map_event[index].unit_index = mapunit_add( MAPUNIT_TYPE_EVENT, index, g_map_event[index].actorid );

	// 占地块信息添加到世界地图
	map_addobject( MAPUNIT_TYPE_EVENT, index, g_map_event[index].posx, g_map_event[index].posy );

	// 添加失败
	if ( map_event_addcity( city_getindex_withactorid( g_map_event[index].actorid ), index ) <= 0 )
	{
		map_event_delete( index, -1 );
		return -1;
	}
	return 0;
}

int map_event_load()
{
	g_map_event = (MapEvent*)malloc( sizeof( MapEvent ) * g_map_event_maxcount );
	memset( g_map_event, 0, sizeof( MapEvent ) * g_map_event_maxcount );
	for ( int tmpi = 0; tmpi < g_map_event_maxcount; tmpi++ )
	{
		g_map_event[tmpi].unit_index = -1;
	}
	g_event_lastfree_index = -1;

	printf_msg( "MapEvent  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_map_event_maxcount, (sizeof( MapEvent )*g_map_event_maxcount) / 1024.0 / 1024.0, sizeof( MapEvent ) / 1024.0 );
	map_event_load_auto( map_event_getptr, map_event_loadcb, "map_event" );
	return 0;
}

int map_event_save( FILE *fp )
{
	map_event_batch_save_auto( g_map_event, g_map_event_maxcount, "map_event", fp );
	return 0;
}

int map_event_maxcount_set( int maxcount )
{
	g_map_event_maxcount = maxcount;
	return 0;
}

MapEvent *map_event_getptr( int index )
{
	if ( index < 0 || index >= g_map_event_maxcount )
		return NULL;
	return &g_map_event[index];
}

MapEventInfo *map_event_getconfig( int kind )
{
	if ( kind < 0 || kind >= g_eventinfo_maxnum )
		return NULL;
	return &g_eventinfo[kind];
}

// 显示单元属性
void map_event_makeunit( int index, SLK_NetS_AddMapUnit *pAttr, int actorid )
{
	if ( index < 0 || index >= g_map_event_maxcount )
		return;
	MapEventInfo *config = map_event_getconfig( g_map_event[index].kind );
	if ( !config )
		return;
	pAttr->m_state = g_map_event[index].state;
	pAttr->m_posx = g_map_event[index].posx;
	pAttr->m_posy = g_map_event[index].posy;
	pAttr->m_char_value[0] = (char)config->type;
	pAttr->m_char_value_count = 1;

	pAttr->m_short_value[0] = g_map_event[index].kind;
	pAttr->m_short_value_count = 1;

	pAttr->m_int_value[0] = actorid;
	pAttr->m_int_value[1] = g_map_event[index].waitsec;
	pAttr->m_int_value_count = 2;
}

// 位置
void map_event_getpos( int index, short *posx, short *posy )
{
	if ( index < 0 || index >= g_map_event_maxcount )
		return;
	*posx = g_map_event[index].posx;
	*posy = g_map_event[index].posy;
}

// 获取一个空闲索引
int map_event_getfreeindex()
{
	int freeindex = -1;
	if ( g_event_lastfree_index < 0 || g_event_lastfree_index >= g_map_event_maxcount - 1 )
	{
		for ( int index = 0; index < g_map_event_maxcount; index++ )
		{
			if ( g_map_event[index].kind <= 0 )
			{
				freeindex = index;
				g_event_lastfree_index = index + 1;
				break;
			}
		}
	}
	else
	{
		for ( int index = g_event_lastfree_index; index < g_map_event_maxcount; index++ )
		{ // 这里注意，用途是提升查找空闲索引效率
			if ( g_map_event[index].kind <= 0 )
			{
				freeindex = index;
				g_event_lastfree_index = index + 1;
				break;
			}
		}
	}

	return freeindex;
}

// 创建一个事件
int map_event_create( short kind, short posx, short posy, int city_index )
{
	if ( kind <= 0 || kind >= g_eventinfo_maxnum )
		return -1;
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return -1;
	int index = map_event_getfreeindex();
	if ( index < 0 )
		return -1;
	if ( map_event_addcity( city_index, index ) <= 0 )
		return -1;
	g_map_event[index].index = index;
	g_map_event[index].kind = kind;
	g_map_event[index].posx = posx;
	g_map_event[index].posy = posy;
	g_map_event[index].actorid = g_city[city_index].actorid;
	g_map_event[index].unit_index = mapunit_add( MAPUNIT_TYPE_EVENT, index, g_city[city_index].actorid );
	map_addobject( MAPUNIT_TYPE_EVENT, index, posx, posy );
	return g_map_event[index].unit_index;
}

// 删除一个事件
int map_event_delete( int index, int city_index )
{
	if ( index < 0 || index >= g_map_event_maxcount )
		return -1;
	mapunit_del( MAPUNIT_TYPE_EVENT, index, g_map_event[index].unit_index );
	map_delobject( MAPUNIT_TYPE_EVENT, index, g_map_event[index].posx, g_map_event[index].posy );
	if ( city_index >= 0 && city_index < g_city_maxcount )
	{
		map_event_deletecity( city_index, index );
	}
	else
	{
		map_event_deletecity( city_getindex_withactorid( g_map_event[index].actorid ), index );
	}
	memset( &g_map_event[index], 0, sizeof( MapEvent ) );
	g_map_event[index].unit_index = -1;
	return 0;
}

// 改变事件坐标
int map_event_changepos( int index, short posx, short posy )
{
	if ( index < 0 || index >= g_map_event_maxcount )
		return -1;
	if ( map_canmove( posx, posy ) == 0 )
		return -1;
	map_delobject( MAPUNIT_TYPE_EVENT, index, g_map_event[index].posx, g_map_event[index].posy );
	g_map_event[index].posx = posx;
	g_map_event[index].posy = posy;
	map_addobject( MAPUNIT_TYPE_EVENT, index, g_map_event[index].posx, g_map_event[index].posy );
	mapunit_area_change( g_map_event[index].unit_index, g_map_event[index].posx, g_map_event[index].posy, 1 );
	return 0;
}

// 重新随机一个点
int map_event_changepos_rand( int index )
{
	if ( index < 0 || index >= g_map_event_maxcount )
		return -1;
	City *pCity = city_getptr_withactorid( g_map_event[index].actorid );
	if ( !pCity )
		return -1;
	short pPosx = -1;
	short pPosy = -1;
	int range = 2;
	while ( range < 10 )
	{
		map_getrandpos_withrange( pCity->posx, pCity->posy, range, &pPosx, &pPosy );
		if ( pPosx >= 0 && pPosy >= 0 )
		{
			map_event_changepos( index, pPosx, pPosy );
			break;
		}
		range += 1;
	}
	return 0;
}

// 重新随机一个点-有中心点
int map_event_changepos_randhaspos( int index, short posx, short posy )
{
	if ( index < 0 || index >= g_map_event_maxcount )
		return -1;
	short pPosx = -1;
	short pPosy = -1;
	int range = 2;
	while ( range < 10 )
	{
		map_getrandpos_withrange( posx, posy, range, &pPosx, &pPosy );
		if ( pPosx >= 0 && pPosy >= 0 )
		{
			map_event_changepos( index, pPosx, pPosy );
			break;
		}
		range += 1;
	}
	return 0;
}

// 事件数量
int map_event_num( int actorid )
{
	int num = 0;
	for ( int tmpi = 0; tmpi < g_map_event_maxcount; tmpi++ )
	{
		if ( g_map_event[tmpi].actorid != actorid )
			continue;
		num += 1;
	}
	return num;
}

// 删除属于这玩家的事件
int map_event_delete_actor( int actorid, int city_index )
{
	for ( int tmpi = 0; tmpi < g_map_event_maxcount; tmpi++ )
	{
		if ( g_map_event[tmpi].actorid != actorid )
			continue;
		if ( g_map_event[tmpi].state > 0 )
			continue;
		if ( g_map_event[tmpi].waitsec > 0 )
			continue;
		map_event_delete( tmpi, city_index );
	}
	return 0;
}

// 根据一个点的范围进行生成
int map_event_range_brush( short kind, short posx, short posy, int range, int city_index )
{
	short pPosx = -1;
	short pPosy = -1;
	map_getrandpos_withrange( posx, posy, range, &pPosx, &pPosy );
	if ( pPosx >= 0 && pPosy >= 0 )
	{
		map_event_create( kind, pPosx, pPosy, city_index );
	}
	return -1;
}

int map_event_addcity( int city_index, int index )
{
	CITY_CHECK_INDEX( city_index );
	for ( int tmpi = 0; tmpi < CITY_MAPEVENT_MAX; tmpi++ )
	{
		if ( g_city[city_index].mapevent_index[tmpi] < 0 )
		{
			g_city[city_index].mapevent_index[tmpi] = index;
			return 1;
		}
	}
	
	return 0;
}

int map_event_deletecity( int city_index, int index )
{
	CITY_CHECK_INDEX( city_index );
	for ( int tmpi = 0; tmpi < CITY_MAPEVENT_MAX; tmpi++ )
	{
		if ( g_city[city_index].mapevent_index[tmpi] == index )
		{
			g_city[city_index].mapevent_index[tmpi] = -1;
			break;
		}
	}
	return 0;
}

// 每个玩家的事件逻辑
int map_event_citylogic( int city_index )
{
	CITY_CHECK_INDEX( city_index );
	for ( int tmpi = 0; tmpi < CITY_MAPEVENT_MAX; tmpi++ )
	{
		int mapevent_index = g_city[city_index].mapevent_index[tmpi];
		if ( mapevent_index < 0 || mapevent_index >= g_map_event_maxcount)
			continue;
		if ( g_map_event[mapevent_index].waitsec > 0 )
		{
			g_map_event[mapevent_index].waitsec -= 1;
			if ( g_map_event[mapevent_index].waitsec <= 0 )
			{
				// 完成
				g_map_event[mapevent_index].state = 1;
				mapunit_update( MAPUNIT_TYPE_EVENT, mapevent_index, g_map_event[mapevent_index].unit_index );
			}
		}
	}
	return 0;
}

// 采集事件
int map_event_gather( int actor_index, int unit_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;
	MapUnit *unit = &g_mapunit[unit_index];
	if ( !unit )
		return -1;
	if ( unit->type != MAPUNIT_TYPE_EVENT )
		return -1;
	if ( unit->index < 0 || unit->index >= g_map_event_maxcount )
		return -1;
	if ( unit->actorid != g_actors[actor_index].actorid )
		return -1;
	if ( g_map_event[unit->index].state > 0 )
	{
		map_event_getaward( actor_index, unit_index );
		return -1;
	}
	MapEventInfo *config = map_event_getconfig( g_map_event[unit->index].kind );
	if ( !config )
		return -1;


	g_map_event[unit->index].waitsec = config->waitsec;
	if ( g_map_event[unit->index].waitsec > 0 )
	{
		g_map_event[unit->index].state = 0;
	}
	else
	{
		g_map_event[unit->index].state = 1;
	}
	mapunit_update( MAPUNIT_TYPE_EVENT, unit->index, unit_index );
	return 0;
}

// 领取事件奖励
int map_event_getaward( int actor_index, int unit_index )
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
	if ( unit->type != MAPUNIT_TYPE_EVENT )
		return -1;
	if ( unit->index < 0 || unit->index >= g_map_event_maxcount )
		return -1;
	if ( unit->actorid != g_actors[actor_index].actorid )
		return -1;
	if ( g_map_event[unit->index].state == 0 )
	{
		map_event_gather( actor_index, unit_index );
		return -1;
	}
	MapEventInfo *config = map_event_getconfig( g_map_event[unit->index].kind );
	if ( !config )
		return -1;

	actor_system_pop( actor_index, 1020 + config->type );

	// 获取奖励
	AwardGetInfo info = { 0 };
	award_getaward( actor_index, config->awardkind, config->awardnum, -1, PATH_MAPEVENT, &info );

	map_event_delete( unit->index, pCity->index );
	return 0;
}
