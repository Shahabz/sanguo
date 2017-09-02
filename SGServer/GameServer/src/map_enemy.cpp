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
#include "map_enemy.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern MapZoneInfo *g_zoneinfo;
extern int g_zoneinfo_maxnum;

extern MapEnemyInfo *g_enemyinfo;
extern int g_enemyinfo_maxnum;

MapEnemy *g_map_enemy = NULL;
int g_map_enemy_maxcount = 0;
extern int g_map_enemy_maxindex;

int g_enemy_lastfree_index = -1;

// 读档完毕的回调
int map_enemy_loadcb( int index )
{
	if ( index < 0 || index >= g_map_enemy_maxcount )
		return -1;

	// 添加到地图显示单元
	g_map_enemy[index].unit_index = mapunit_add( MAPUNIT_TYPE_ENEMY, index );

	// 占地块信息添加到世界地图
	map_addobject( MAPUNIT_TYPE_ENEMY, index, g_map_enemy[index].posx, g_map_enemy[index].posy );
	return 0;
}

int map_enemy_load()
{
	g_map_enemy = (MapEnemy*)malloc( sizeof( MapEnemy ) * g_map_enemy_maxcount );
	memset( g_map_enemy, 0, sizeof( MapEnemy ) * g_map_enemy_maxcount );
	for ( int tmpi = 0; tmpi < g_map_enemy_maxcount; tmpi++ )
	{
		g_map_enemy[tmpi].unit_index = -1;
	}
	g_enemy_lastfree_index = -1;

	printf_msg( "MapEnemy  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_map_enemy_maxcount, (sizeof( MapEnemy )*g_map_enemy_maxcount) / 1024.0 / 1024.0, sizeof( MapEnemy ) / 1024.0 );
	map_enemy_load_auto( map_enemy_getptr, map_enemy_loadcb, "map_enemy" );
	return 0;
}

int map_enemy_save( FILE *fp )
{
	map_enemy_batch_save_auto( g_map_enemy, g_map_enemy_maxcount, "map_enemy", fp );
	return 0;
}

int map_enemy_maxcount_set( int maxcount )
{
	g_map_enemy_maxcount = maxcount;
	return 0;
}

MapEnemy *map_enemy_getptr( int index )
{
	if ( index < 0 || index >= g_map_enemy_maxcount )
		return NULL;
	return &g_map_enemy[index];
}

MapEnemyInfo *map_enemy_getconfig( int kind )
{
	if ( kind < 0 || kind >= g_enemyinfo_maxnum )
		return NULL;
	return &g_enemyinfo[kind];
}

// 显示单元属性
void map_enemy_makeunit( int index, SLK_NetS_AddMapUnit *pAttr )
{
	if ( index < 0 || index >= g_map_enemy_maxcount )
		return;
	MapEnemyInfo *config = map_enemy_getconfig( g_map_enemy[index].kind );
	if ( !config )
		return;
	pAttr->m_posx = g_map_enemy[index].posx;
	pAttr->m_posy = g_map_enemy[index].posy;
	pAttr->m_char_value[0] = (char)config->level;
	pAttr->m_char_value_count = 1;
	pAttr->m_short_value[0] = g_map_enemy[index].kind;
	pAttr->m_short_value_count = 1;
}

// 位置
void map_enemy_getpos( int index, short *posx, short *posy )
{
	if ( index < 0 || index >= g_map_enemy_maxcount )
		return;
	*posx = g_map_enemy[index].posx;
	*posy = g_map_enemy[index].posy;
}

// 获取一个空闲索引
int map_enemy_getfreeindex()
{
	int freeindex = -1;
	if ( g_enemy_lastfree_index < 0 || g_enemy_lastfree_index >= g_map_enemy_maxcount - 1 )
	{
		for ( int index = 0; index < g_map_enemy_maxcount; index++ )
		{
			if ( g_map_enemy[index].kind <= 0 )
			{
				freeindex = index;
				g_enemy_lastfree_index = index + 1;
				break;
			}
		}
	}
	else
	{
		for ( int index = g_enemy_lastfree_index; index < g_map_enemy_maxcount; index++ )
		{ // 这里注意，用途是提升查找空闲索引效率
			if ( g_map_enemy[index].kind <= 0 )
			{
				freeindex = index;
				g_enemy_lastfree_index = index + 1;
				break;
			}
		}
	}

	return freeindex;
}

// 创建一个流寇
int map_enemy_create( short kind, short posx, short posy, int deltime )
{
	if ( kind <= 0 || kind >= g_enemyinfo_maxnum )
		return -1;
	int index = map_enemy_getfreeindex();
	if ( index < 0 )
		return -1;
	g_map_enemy[index].index = index;
	g_map_enemy[index].kind = kind;
	g_map_enemy[index].posx = posx;
	g_map_enemy[index].posy = posy;
	g_map_enemy[index].deltime = deltime;
	g_map_enemy[index].unit_index = mapunit_add( MAPUNIT_TYPE_ENEMY, index );
	map_addobject( MAPUNIT_TYPE_ENEMY, index, posx, posy );
	return g_map_enemy[index].unit_index;
}

// 删除一个流寇
int map_enemy_delete( int index )
{
	if ( index < 0 || index >= g_map_enemy_maxcount )
		return -1;
	mapunit_del( MAPUNIT_TYPE_ENEMY, index, g_map_enemy[index].unit_index );
	map_delobject( MAPUNIT_TYPE_ENEMY, index, g_map_enemy[index].posx, g_map_enemy[index].posy );
	memset( &g_map_enemy[index], 0, sizeof( MapEnemy ) );
	g_map_enemy[index].unit_index = -1;
	return 0;
}

// 野怪数量
int map_enemy_num( int zoneid, int kind )
{
	int num = 0;
	for ( int tmpi = 0; tmpi < g_map_enemy_maxcount; tmpi++ )
	{
		if ( g_map_enemy[tmpi].kind != kind )
			continue;
		if ( map_zone_inrange( zoneid, g_map_enemy[tmpi].posx, g_map_enemy[tmpi].posy ) == 1 )
		{
			num += 1;
		}
	}
	return num;
}

// 根据一个点的范围进行生成
int map_enemy_rang_brush( short kind, short posx, short posy, int range, int deltime )
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
		map_enemy_create( kind, pPosx, pPosy, deltime );
	}
	return -1;
}

// 添加选定次数
void map_enemy_addselected( int index )
{
	if ( index < 0 || index >= g_map_enemy_maxcount )
		return;
	if ( g_map_enemy[index].selected_count < 127 )
	{
		g_map_enemy[index].selected_count += 1;
	}
	return;
}

// 减少选定次数
void map_enemy_subselected( int index )
{
	if ( index < 0 || index >= g_map_enemy_maxcount )
		return;
	if ( g_map_enemy[index].selected_count > 0 )
	{
		g_map_enemy[index].selected_count -= 1;
	}
	return;
}

// 获取野怪奖励
int map_enemy_sendaward( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( kind <= 0 || kind >= g_enemyinfo_maxnum )
		return -1;
	awardgroup_sendinfo( actor_index, g_enemyinfo[kind].awardgroup, 1, kind, 4 );
	return 0;
}
