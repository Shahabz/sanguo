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
#include "map.h"
#include "map_zone.h"
#include "map_activity.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern MapZoneInfo *g_zoneinfo;
extern int g_zoneinfo_maxnum;

extern MapActivityInfo *g_activityinfo;
extern int g_activityinfo_maxnum;

MapActivity *g_map_activity = NULL;
int g_map_activity_maxcount = 0;
extern int g_map_activity_maxindex;

int g_activity_lastfree_index = -1;

// 读档完毕的回调
int map_activity_loadcb( int index )
{
	if ( index < 0 || index >= g_map_activity_maxcount )
		return -1;

	// 添加到地图显示单元
	g_map_activity[index].unit_index = mapunit_add( MAPUNIT_TYPE_ACTIVITY, index );

	// 占地块信息添加到世界地图
	map_addobject( MAPUNIT_TYPE_ACTIVITY, index, g_map_activity[index].posx, g_map_activity[index].posy );
	return 0;
}

int map_activity_load()
{
	g_map_activity = (MapActivity*)malloc( sizeof( MapActivity ) * g_map_activity_maxcount );
	memset( g_map_activity, 0, sizeof( MapActivity ) * g_map_activity_maxcount );
	for ( int tmpi = 0; tmpi < g_map_activity_maxcount; tmpi++ )
	{
		g_map_activity[tmpi].unit_index = -1;
	}
	g_activity_lastfree_index = -1;

	printf_msg( "MapActivity  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_map_activity_maxcount, (sizeof( MapActivity )*g_map_activity_maxcount) / 1024.0 / 1024.0, sizeof( MapActivity ) / 1024.0 );
	map_activity_load_auto( map_activity_getptr, map_activity_loadcb, "map_activity" );
	return 0;
}

int map_activity_save( FILE *fp )
{
	map_activity_batch_save_auto( g_map_activity, g_map_activity_maxcount, "map_activity", fp );
	return 0;
}

int map_activity_maxcount_set( int maxcount )
{
	g_map_activity_maxcount = maxcount;
	return 0;
}

MapActivity *map_activity_getptr( int index )
{
	if ( index < 0 || index >= g_map_activity_maxcount )
		return NULL;
	return &g_map_activity[index];
}

MapActivityInfo *map_activity_getconfig( int kind )
{
	if ( kind < 0 || kind >= g_activityinfo_maxnum )
		return NULL;
	return &g_activityinfo[kind];
}

// 显示单元属性
void map_activity_makeunit( int index, SLK_NetS_AddMapUnit *pAttr )
{
	if ( index < 0 || index >= g_map_activity_maxcount )
		return;
	MapActivityInfo *config = map_activity_getconfig( g_map_activity[index].kind );
	if ( !config )
		return;
	pAttr->m_posx = g_map_activity[index].posx;
	pAttr->m_posy = g_map_activity[index].posy;
	pAttr->m_char_value[0] = (char)g_map_activity[index].kind;
	pAttr->m_char_value[1] = (char)config->level;
	pAttr->m_char_value[2] = (char)config->type;
	pAttr->m_char_value_count = 3;
	if ( g_map_activity[index].maxhp > 0 )
	{
		pAttr->m_int_value[0] = g_map_activity[index].hp;
		pAttr->m_int_value[1] = g_map_activity[index].maxhp;
		pAttr->m_int_value_count = 2;
	}
}

// 位置
void map_activity_getpos( int index, short *posx, short *posy )
{
	if ( index < 0 || index >= g_map_activity_maxcount )
		return;
	*posx = g_map_activity[index].posx;
	*posy = g_map_activity[index].posy;
}

// 获取一个空闲索引
int map_activity_getfreeindex()
{
	int freeindex = -1;
	if ( g_activity_lastfree_index < 0 || g_activity_lastfree_index >= g_map_activity_maxcount - 1 )
	{
		for ( int index = 0; index < g_map_activity_maxcount; index++ )
		{
			if ( g_map_activity[index].kind <= 0 )
			{
				freeindex = index;
				g_activity_lastfree_index = index + 1;
				break;
			}
		}
	}
	else
	{
		for ( int index = g_activity_lastfree_index; index < g_map_activity_maxcount; index++ )
		{ // 这里注意，用途是提升查找空闲索引效率
			if ( g_map_activity[index].kind <= 0 )
			{
				freeindex = index;
				g_activity_lastfree_index = index + 1;
				break;
			}
		}
	}

	return freeindex;
}

// 创建一个活动怪
int map_activity_create( short kind, short posx, short posy, int deltime, int actorid )
{
	if ( kind <= 0 || kind >= g_activityinfo_maxnum )
		return -1;
	int index = map_activity_getfreeindex();
	if ( index < 0 )
		return -1;
	g_map_activity[index].index = index;
	g_map_activity[index].kind = kind;
	g_map_activity[index].posx = posx;
	g_map_activity[index].posy = posy;
	g_map_activity[index].deltime = deltime;
	g_map_activity[index].actorid = actorid;
	g_map_activity[index].activityid = (char)g_activityinfo[kind].activityid;
	g_map_activity[index].hp = g_activityinfo[kind].maxhp;
	g_map_activity[index].maxhp = g_activityinfo[kind].maxhp;
	g_map_activity[index].unit_index = mapunit_add( MAPUNIT_TYPE_ACTIVITY, index );
	map_addobject( MAPUNIT_TYPE_ACTIVITY, index, posx, posy );
	return g_map_activity[index].unit_index;
}

// 删除一个活动怪
int map_activity_delete( int index )
{
	if ( index < 0 || index >= g_map_activity_maxcount )
		return -1;
	mapunit_del( MAPUNIT_TYPE_ACTIVITY, index, g_map_activity[index].unit_index );
	map_delobject( MAPUNIT_TYPE_ACTIVITY, index, g_map_activity[index].posx, g_map_activity[index].posy );
	memset( &g_map_activity[index], 0, sizeof( MapActivity ) );
	g_map_activity[index].unit_index = -1;
	return 0;
}

// 根据活动删除活动怪
int map_activity_delete_withactivityid( int activityid )
{
	for ( int tmpi = 0; tmpi < g_map_activity_maxcount; tmpi++ )
	{
		if ( g_map_activity[tmpi].activityid == activityid )
		{
			map_activity_delete( tmpi );
		}
	}
	return 0;
}

// 活动怪数量
int map_activity_num( int zoneid, int kind )
{
	int num = 0;
	for ( int tmpi = 0; tmpi < g_map_activity_maxcount; tmpi++ )
	{
		if ( g_map_activity[tmpi].kind != kind )
			continue;
		if ( map_zone_inrange( zoneid, g_map_activity[tmpi].posx, g_map_activity[tmpi].posy ) == 1 )
		{
			num += 1;
		}
	}
	return num;
}

int map_activity_num_withactivityid( int zoneid, int activityid )
{
	int num = 0;
	for ( int tmpi = 0; tmpi < g_map_activity_maxcount; tmpi++ )
	{
		if ( g_map_activity[tmpi].activityid != activityid )
			continue;
		if ( map_zone_inrange( zoneid, g_map_activity[tmpi].posx, g_map_activity[tmpi].posy ) == 1 )
		{
			num += 1;
		}
	}
	return num;
}

// 根据一个点的范围进行生成
int map_activity_range_brush( short kind, short posx, short posy, int range, int deltime, int actorid )
{
	short pPosx = -1;
	short pPosy = -1;
	map_getrandpos_withrange( posx, posy, range, &pPosx, &pPosy );
	if ( pPosx < 0 || pPosy < 0 )
	{
		for ( int range_ex = range + 1; range_ex < range + 64; range_ex++ )
		{
			map_getrandpos_withrange( posx, posy, range_ex, &pPosx, &pPosy );
			if ( pPosx >= 0 && pPosy >= 0 )
				break;
		}
	}
	if ( pPosx >= 0 && pPosy >= 0 )
	{
		map_activity_create( kind, pPosx, pPosy, deltime, actorid );
	}
	return -1;
}

// 获取活动怪奖励
int map_activity_sendaward( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( kind <= 0 || kind >= g_activityinfo_maxnum )
		return -1;
	awardgroup_sendinfo( actor_index, g_activityinfo[kind].awardgroup, 6, kind, 4 );
	return 0;
}
