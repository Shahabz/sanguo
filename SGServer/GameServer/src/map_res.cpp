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
#include "map_res.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;

extern Army *g_army;
extern int g_army_maxcount;

extern MapResInfo *g_resinfo;
extern int g_resinfo_maxnum;

MapRes *g_map_res = NULL;
int g_map_res_maxcount = 0;
extern int g_map_res_maxindex;

int g_map_res_lastfreeindex = -1;

// 读档完毕的回调
int map_res_loadcb( int index )
{
	if ( index < 0 || index >= g_map_res_maxcount )
		return -1;

	// 添加到地图显示单元
	g_map_res[index].unit_index = mapunit_add( MAPUNIT_TYPE_RES, index );

	// 占地块信息添加到世界地图
	map_addobject( MAPUNIT_TYPE_RES, index, g_map_res[index].posx, g_map_res[index].posy );
	return 0;
}

int map_res_load()
{
	g_map_res = (MapRes*)malloc( sizeof( MapRes ) * g_map_res_maxcount );
	memset( g_map_res, 0, sizeof( MapRes ) * g_map_res_maxcount );
	for ( int tmpi = 0; tmpi < g_map_res_maxcount; tmpi++ )
	{
		g_map_res[tmpi].army_index = -1;
		g_map_res[tmpi].unit_index = -1;
	}
	g_map_res_lastfreeindex = -1;
	printf_msg( "MapRes  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_map_res_maxcount, (sizeof( MapRes )*g_map_res_maxcount) / 1024.0 / 1024.0, sizeof( MapRes ) / 1024.0 );
	map_res_load_auto( map_res_getptr, map_res_loadcb, "map_res" );
	return 0;
}

int map_res_save( FILE *fp )
{
	map_res_batch_save_auto( g_map_res, g_map_res_maxcount, "map_res", fp );
	return 0;
}

int map_res_maxcount_set( int maxcount )
{
	g_map_res_maxcount = maxcount;
	return 0;
}

MapRes *map_res_getptr( int index )
{
	if ( index < 0 || index >= g_map_res_maxcount )
		return NULL;
	return &g_map_res[index];
}

MapResInfo *map_res_getconfig( int kind )
{
	if ( kind < 0 || kind >= g_resinfo_maxnum )
		return NULL;
	return &g_resinfo[kind];
}

// 显示单元属性
void map_res_makeunit( int index, SLK_NetS_AddMapUnit *pAttr )
{
	if ( index < 0 || index >= g_map_res_maxcount )
		return;
	MapResInfo *config = map_res_getconfig( g_map_res[index].kind );
	if ( !config )
		return;
	pAttr->m_posx = g_map_res[index].posx;
	pAttr->m_posy = g_map_res[index].posy;
	pAttr->m_char_value[0] = (char)config->type;
	pAttr->m_char_value[1] = (char)config->level;
	pAttr->m_char_value_count = 2;
	if ( g_map_res[index].army_index >= 0 )
	{
		City *pCity = army_getcityptr( g_map_res[index].army_index );
		if ( pCity )
		{
			pAttr->m_state = ARMY_STATE_GATHER;
			strncpy( pAttr->m_name, pCity->name, NAME_SIZE );
			pAttr->m_namelen = strlen( pAttr->m_name );
			pAttr->m_char_value[2] = pCity->nation;
			pAttr->m_char_value_count = 3;

			pAttr->m_int_value[0] = pCity->actorid;
			pAttr->m_int_value_count = 1;
		}
	}
}

// 位置
void map_res_getpos( int index, short *posx, short *posy )
{
	if ( index < 0 || index >= g_map_res_maxcount )
		return;
	*posx = g_map_res[index].posx;
	*posy = g_map_res[index].posy;
}

// 获取一个空闲索引
int map_res_getfreeindex()
{
	int freeindex = -1;
	if ( g_map_res_lastfreeindex < 0 || g_map_res_lastfreeindex >= g_map_res_maxcount - 1 )
	{
		for ( int index = 0; index < g_map_res_maxcount; index++ )
		{
			if ( g_map_res[index].kind <= 0 )
			{
				freeindex = index;
				g_map_res_lastfreeindex = index + 1;
				break;
			}
		}
	}
	else
	{
		for ( int index = g_map_res_lastfreeindex; index < g_map_res_maxcount; index++ )
		{
			if ( g_map_res[index].kind <= 0 )
			{
				freeindex = index;
				g_map_res_lastfreeindex = index + 1;
				break;
			}
		}
	}

	return freeindex;
}

// 创建一个资源点
int map_res_create( short kind, short posx, short posy )
{
	if ( kind <= 0 || kind >= g_resinfo_maxnum )
		return -1;
	MapResInfo *config = map_res_getconfig( kind );
	if ( !config )
		return -1;
	int index = map_res_getfreeindex();
	if ( index < 0 )
		return -1;
	g_map_res[index].index = index;
	g_map_res[index].kind = kind;
	g_map_res[index].posx = posx;
	g_map_res[index].posy = posy;
	g_map_res[index].num = config->num;
	g_map_res[index].idlesec = random( 160, 240 ); // 控制在160分钟到240分钟
	g_map_res[index].army_index = -1;
	g_map_res[index].unit_index = mapunit_add( MAPUNIT_TYPE_RES, index );
	map_addobject( MAPUNIT_TYPE_RES, index, posx, posy );
	return g_map_res[index].unit_index;
}

// 删除一个资源点
int map_res_delete( int index, char checkarmy )
{
	if ( index < 0 || index >= g_map_res_maxcount )
		return -1;
	if ( checkarmy == 1 )
	{
		if ( g_map_res[index].army_index >= 0 )
			return -1;
	}
	mapunit_del( MAPUNIT_TYPE_RES, index, g_map_res[index].unit_index );
	map_delobject( MAPUNIT_TYPE_RES, index, g_map_res[index].posx, g_map_res[index].posy );
	memset( &g_map_res[index], 0, sizeof( MapRes ) );
	g_map_res[index].army_index = -1;
	g_map_res[index].unit_index = -1;
	return 0;
}

// 资源点数量
int map_res_num( int zoneid, int kind )
{
	int num = 0;
	for ( int tmpi = 0; tmpi < g_map_res_maxcount; tmpi++ )
	{
		if ( g_map_res[tmpi].kind != kind )
			continue;
		if ( map_zone_inrange( zoneid, g_map_res[tmpi].posx, g_map_res[tmpi].posy ) == 1 )
		{
			num += 1;
		}
	}
	return num;
}

// 每分钟
int map_res_logic()
{
	for ( int tmpi = 0; tmpi < g_map_res_maxcount; tmpi++ )
	{
		if ( g_map_res[tmpi].kind <= 0 )
			continue;
		if ( g_map_res[tmpi].army_index < 0 )
		{
			g_map_res[tmpi].idlesec -= 1;
			if ( g_map_res[tmpi].idlesec == 0 )
			{// 闲置超过规定时间，删除，重刷
				short kind = g_map_res[tmpi].kind;
				short oldposx = g_map_res[tmpi].posx;
				short oldposy = g_map_res[tmpi].posy;
				map_res_delete( tmpi, 1 );

				short posx = -1;
				short posy = -1;
				map_getrandpos_withrange( oldposx, oldposy, 20, &posx, &posy );
				map_res_create( kind, posx, posy );
			}
		}
	}
	return 0;
}

// 设置正在采集的部队
int map_res_setarmy( int index, int army_index )
{
	if ( index < 0 || index >= g_map_res_maxcount )
		return -1;
	g_map_res[index].army_index = army_index;
	mapunit_update( MAPUNIT_TYPE_RES, index, g_map_res[index].unit_index );
	return 0;
}

// 获取正在采集的部队
int map_res_getarmy( int index )
{
	if ( index < 0 || index >= g_map_res_maxcount )
		return -1;
	return g_map_res[index].army_index;
}

// 获取显示索引
int map_res_getunit( int index )
{
	if ( index < 0 || index >= g_map_res_maxcount )
		return -1;
	return g_map_res[index].unit_index;
}

// 资源点信息
int map_res_sendinfo( int actor_index, int unit_index )
{
	ACTOR_CHECK_INDEX( actor_index );

	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;

	MapUnit *unit = &g_mapunit[unit_index];
	if ( !unit )
		return -1;
	if ( unit->type != MAPUNIT_TYPE_RES )
		return -1;
	if ( unit->index < 0 || unit->index >= g_map_res_maxcount )
		return -1;

	MapResInfo *config = map_res_getconfig( g_map_res[unit->index].kind );
	if ( !config )
		return -1;

	SLK_NetS_MapResInfo pValue = { 0 };
	pValue.m_kind = g_map_res[unit->index].kind;
	pValue.m_totalnum = config->num;
	pValue.m_totalsec = config->sec;
	pValue.m_spacenum = g_map_res[unit->index].num;

	// 如果有采集的玩家
	int army_index = map_res_getarmy( unit->index );
	if ( army_index >= 0 && army_index < g_army_maxcount )
	{
		City *pCity = army_getcityptr( army_index );
		if ( pCity )
		{
			pValue.m_spacenum = g_map_res[unit->index].num - (int)round( (config->num / (float)config->sec) * g_army[army_index].statetime * (1.0f + pCity->attr.gather_per[0]) * (1.0f + pCity->attr.gather_per[1]) );
			pValue.m_actorlevel = pCity->level;
			pValue.m_herokind = g_army[army_index].herokind[0];
			int hero_index = city_hero_getindex( pCity->index, g_army[army_index].herokind[0] );
			if ( hero_index >= 0 && hero_index < HERO_CITY_MAX )
			{
				pValue.m_herolevel = pCity->hero[hero_index].level;
				pValue.m_herocolor = pCity->hero[hero_index].color;
				pValue.m_herohp = pCity->hero[hero_index].soldiers;
			}
		}
	}

	netsend_mapresinfo_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
