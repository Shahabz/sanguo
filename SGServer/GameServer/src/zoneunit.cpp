#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include "db.h"
#include "define.h"
#include "map.h"
#include "actor.h"
#include "utils.h"
#include "system.h"
#include "actor_send.h"
#include "server_netsend_auto.h"
#include "mapunit.h"
#include "actor_notify.h"
#include "award.h"
#include "global.h"
#include "city.h"
#include "army.h"
#include "map_town.h"
#include "map_enemy.h"
#include "map_res.h"
#include "map_event.h"
#include "map_zone.h"
#include "zoneunit.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Map g_map;
extern Actor *g_actors;
extern int g_maxactornum;

// 奖励组(可理解为掉落包)
extern AwardGroup *g_awardgroup;
extern int g_awardgroup_count;

extern Global global;

ZoneUnit *g_zoneunit = NULL;
int g_zoneunit_maxcount = 0;
int g_zoneunit_lastfreeindex = -1;

int zoneunit_init()
{
	g_zoneunit_lastfreeindex = -1;
	if ( g_Config.max_mapunitcount <= 0 )
	{
		g_zoneunit_maxcount = 250000;
	}
	else
	{
		g_zoneunit_maxcount = g_Config.max_mapunitcount;
	}
	g_zoneunit = (ZoneUnit *)malloc( sizeof( ZoneUnit )*g_zoneunit_maxcount );
	memset( g_zoneunit, 0, sizeof( ZoneUnit )*g_zoneunit_maxcount );
	for ( int tmpi = 0; tmpi < g_zoneunit_maxcount; tmpi++ )
	{
		g_zoneunit[tmpi].index = -1;
		g_zoneunit[tmpi].lastadd_zoneid = -1;
		g_zoneunit[tmpi].pre_index = -1;
		g_zoneunit[tmpi].next_index = -1;
	}
	printf_msg( "ZoneUnit  maxcount=%d  memory=%0.2fMB\n", g_zoneunit_maxcount, (sizeof( ZoneUnit )*g_zoneunit_maxcount) / 1024.0 / 1024.0 );
	return 0;
}

// 获取显示单元对应的属性信息
int zoneunit_getattr( int unit_index, SLK_NetS_ZoneUnit *pAttr )
{
	if ( unit_index < 0 || unit_index >= g_zoneunit_maxcount )
		return -1;
	ZoneUnit *pMapUnit = &g_zoneunit[unit_index];
	switch ( pMapUnit->type )
	{
	case MAPUNIT_TYPE_CITY:	// 玩家城池
		city_makezoneunit( pMapUnit->index, pAttr );
		break;
	default:
		return -1;
	}
	return 0;
}

// 获取显示单元对应的位置
int zoneunit_getpos( int unit_index, short *posx, short *posy )
{
	if ( unit_index < 0 || unit_index >= g_zoneunit_maxcount )
		return -1;
	ZoneUnit *pMapUnit = &g_zoneunit[unit_index];
	switch ( pMapUnit->type )
	{
	case MAPUNIT_TYPE_CITY:// 玩家城池
		city_getpos( pMapUnit->index, posx, posy );
		break;
	default:
		break;
	}
	return 0;
}

// 获取一个空闲的索引
int zoneunit_getfreeindex()
{
	if ( g_zoneunit_lastfreeindex < 0 || g_zoneunit_lastfreeindex >= g_zoneunit_maxcount - 1 )
	{
		for ( int tmpi = 0; tmpi < g_zoneunit_maxcount; tmpi++ )
		{
			if ( g_zoneunit[tmpi].type == 0 )
			{
				g_zoneunit_lastfreeindex = tmpi + 1;
				return tmpi;
			}
		}
	}
	else
	{// 这里注意，用途是提升查找空闲索引效率
		for ( int tmpi = g_zoneunit_lastfreeindex; tmpi < g_zoneunit_maxcount; tmpi++ )
		{
			if ( g_zoneunit[tmpi].type == 0 )
			{
				g_zoneunit_lastfreeindex = tmpi + 1;
				return tmpi;
			}
		}
	}
	return -1;
}

// 根据类型和对应索引确定单元索引
int zoneunit_getindex( char type, int index )
{
	for ( int tmpi = 0; tmpi < g_zoneunit_maxcount; tmpi++ )
	{
		if ( g_zoneunit[tmpi].type == type && g_zoneunit[tmpi].index == index )
		{
			return tmpi;
		}
	}
	return -1;
}

// 将需要显示的城池或军队添加到显示单元
int zoneunit_add( char type, int index )
{
	int unit_index = zoneunit_getfreeindex();
	if ( unit_index < 0 )
		return -1;
	g_zoneunit[unit_index].type = type;
	g_zoneunit[unit_index].index = index;
	g_zoneunit[unit_index].lastadd_zoneid = -1;
	g_zoneunit[unit_index].pre_index = -1;
	g_zoneunit[unit_index].next_index = -1;
	short posx = 0, posy = 0;
	zoneunit_getpos( unit_index, &posx, &posy );
	zoneunit_enterworld( unit_index, posx, posy );
	return unit_index;
}

// 将已经关联的地图单元删除掉
int zoneunit_del( char type, int index, int unit_index )
{
	if ( unit_index < 0 )
		unit_index = zoneunit_getindex( type, index );
	if ( unit_index < 0 )
		return -1;
	short posx = 0, posy = 0;
	zoneunit_getpos( unit_index, &posx, &posy );
	char zoneid = map_zone_getid( posx, posy );
	/*SLK_NetS_DelZoneUnit info = { 0 };
	info.m_unit_index = unit_index;
	int tmpsize = netsend_delzoneunit_S( tmpbuf, sizeleft, &info );

	short posx = 0, posy = 0;
	zoneunit_getpos( unit_index, &posx, &posy );
	int area_index = area_getindex( posx, posy );
	area_sendmsg( area_index, tmpsize, tmpbuf );*/

	map_zone_delunit( unit_index );
	memset( &g_zoneunit[unit_index], 0, sizeof( ZoneUnit ) );
	g_zoneunit[unit_index].lastadd_zoneid = -1;
	g_zoneunit[unit_index].index = -1;
	g_zoneunit[unit_index].pre_index = -1;
	g_zoneunit[unit_index].next_index = -1;
	return 0;
}

// 更新地图单元数据
int zoneunit_update( char type, int index, int unit_index )
{
	if ( unit_index < 0 )
		unit_index = mapunit_getindex( type, index );
	if ( unit_index < 0 )
		return -1;
	// 已经在外部更新完毕的结构
	SLK_NetS_AddMapUnit unitinfo = { 0 };
	mapunit_getattr( unit_index, &unitinfo );

	SLK_NetS_UpdateMapUnit updateinfo = { 0 };
	memcpy( &updateinfo.m_info, &unitinfo, sizeof( SLK_NetS_AddMapUnit ) );

	//// 组织信息
	//char tmpbuf[2048];
	//int sizeleft = 2048;
	//int tmpsize = netsend_updatemapunit_S( tmpbuf, sizeleft, &updateinfo );
	//int area_index = area_getindex( unitinfo.m_posx, unitinfo.m_posy );

	//area_sendmsg( area_index, tmpsize, tmpbuf );
	return 0;
}

// 显示单元进入世界地图，关联到地区中，并通知区域
int zoneunit_enterworld( int unit_index, short posx, short posy )
{
	char zoneid = map_zone_getid( posx, posy );
	if ( map_zone_addunit( unit_index, zoneid ) < 0 )
	{
		write_gamelog( "ZoneAdd Error unit_index:%d", unit_index );
		return -1;
	}
	return 0;
}
