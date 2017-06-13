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

MapUnit *g_mapunit = NULL;
int g_mapunit_maxcount = 0;
int g_mapunit_lastfreeindex = -1;

int mapunit_init()
{
	g_mapunit_lastfreeindex = -1;
	if ( g_Config.max_mapunitcount <= 0 )
	{
		g_mapunit_maxcount = 800000;
	}
	else
	{
		g_mapunit_maxcount = g_Config.max_mapunitcount;
	}
	g_mapunit = (MapUnit *)malloc( sizeof( MapUnit )*g_mapunit_maxcount );
	memset( g_mapunit, 0, sizeof( MapUnit )*g_mapunit_maxcount );
	printf_msg( "MapUnit  maxcount=%d  memory=%0.2fMB\n", g_mapunit_maxcount, (sizeof( MapUnit )*g_mapunit_maxcount) / 1024.0 / 1024.0 );
	return 0;
}

// 获取显示单元对应的属性信息
int mapunit_getattr( int unit_index, SLK_NetS_AddMapUnit *pAttr )
{
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;
	MapUnit *pMapUnit = &g_mapunit[unit_index];
	pAttr->m_unit_index = unit_index;
	pAttr->m_type = pMapUnit->type;
	pAttr->m_int_value_count = 0;
	pAttr->m_short_value_count = 0;
	pAttr->m_char_value_count = 0;
	switch ( pMapUnit->type )
	{
	// 城池
	case MAPUNIT_TYPE_CITY:
	{		
		break;
	}
	//  部队
	case MAPUNIT_TYPE_ARMY:
	{
		break;
	}
	default:
		return -1;
	}
	return 0;
}

// 获取显示单元对应的位置
int mapunit_getpos( int unit_index, short *posx, short *posy )
{
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;
	MapUnit *pMapUnit = &g_mapunit[unit_index];
	switch ( pMapUnit->type )
	{
	case MAPUNIT_TYPE_CITY:
	{
		//if ( pMapUnit->index < 0 || pMapUnit->index >= g_city_maxcount )
		//	break;
		//*posx = g_city[pMapUnit->index].posx;
		//*posy = g_city[pMapUnit->index].posy;
		break;
	}
	case MAPUNIT_TYPE_ARMY:
	{
		//if ( pMapUnit->index < 0 || pMapUnit->index >= g_army_maxcount )
		//	break;
		//*posx = g_army[pMapUnit->index].posx;
		//*posy = g_army[pMapUnit->index].posy;
		break;
	}
	
	default:
		break;
	}
	return 0;
}

// 获取一个空闲的索引
int mapunit_getfreeindex()
{
	if ( g_mapunit_lastfreeindex < 0 || g_mapunit_lastfreeindex >= g_mapunit_maxcount - 1 )
	{
		for ( int tmpi = 0; tmpi < g_mapunit_maxcount; tmpi++ )
		{
			if ( g_mapunit[tmpi].type == 0 )
			{
				g_mapunit_lastfreeindex = tmpi + 1;
				return tmpi;
			}
		}
	}
	else
	{// 这里注意，用途是提升查找空闲索引效率
		for ( int tmpi = g_mapunit_lastfreeindex; tmpi < g_mapunit_maxcount; tmpi++ )
		{
			if ( g_mapunit[tmpi].type == 0 )
			{
				g_mapunit_lastfreeindex = tmpi + 1;
				return tmpi;
			}
		}
	}
	return -1;
}

// 根据类型和对应索引确定单元索引
int mapunit_getindex( char type, int index )
{
	for ( int tmpi = 0; tmpi < g_mapunit_maxcount; tmpi++ )
	{
		if ( g_mapunit[tmpi].type == type && g_mapunit[tmpi].index == index )
		{
			return tmpi;
		}
	}
	return -1;
}

// 将需要显示的城池或军队添加到显示单元
int mapunit_add( char type, int index )
{
	int unit_index = mapunit_getfreeindex();
	if ( unit_index < 0 )
		return -1;
	memset( &g_mapunit[unit_index], 0, sizeof( MapUnit ) );
	g_mapunit[unit_index].type = type;
	g_mapunit[unit_index].index = index;
	g_mapunit[unit_index].lastadd_areaindex = -1;
	g_mapunit[unit_index].pre_index = -1;
	g_mapunit[unit_index].next_index = -1;
	short posx = 0, posy = 0;
	mapunit_getpos( unit_index, &posx, &posy );
	mapunit_enterworld( unit_index, posx, posy );
	return unit_index;
}

// 将已经关联的地图单元删除掉
int mapunit_del( char type, int index, int unit_index )
{
	if ( unit_index < 0 )
		unit_index = mapunit_getindex( type, index );
	if ( unit_index < 0 )
		return -1;

	char tmpbuf[2048];
	int sizeleft = 2048;
	SLK_NetS_DelMapUnit info = { 0 };
	info.m_unit_index = unit_index;
	int tmpsize = netsend_delmapunit_S( tmpbuf, sizeleft, &info );

	short posx = 0, posy = 0;
	mapunit_getpos( unit_index, &posx, &posy );
	int area_index = area_getindex( posx, posy );
	area_sendmsg( area_index, tmpsize, tmpbuf );

	area_delmapunit( unit_index, area_index );
	memset( &g_mapunit[unit_index], 0, sizeof( MapUnit ) );
	return 0;
}

// 更新地图单元数据
int mapunit_update( char type, int index, int unit_index )
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

	// 组织信息
	char tmpbuf[2048];
	int sizeleft = 2048;
	int tmpsize = netsend_updatemapunit_S( tmpbuf, sizeleft, &updateinfo );
	int area_index = area_getindex( unitinfo.m_posx, unitinfo.m_posy );

	area_sendmsg( area_index, tmpsize, tmpbuf );
	//area_clearmsg( area_index );
	return 0;
}

// 显示单元进入世界地图，关联到区域中，并通知区域
int mapunit_enterworld( int unit_index, short posx, short posy )
{
	int area_index = area_getindex( posx, posy );
	if ( area_addmapunit( unit_index, area_index ) < 0 )
	{
		write_gamelog( "AreaAdd Error unit_index:%d", unit_index );
		return -1;
	}

	// 组织数据包发送到区域缓冲
	char tmpbuf[2048] = { 0 };
	int tmpsize = 0;
	mapunit_enterinfo( unit_index, tmpbuf, &tmpsize );
	area_sendmsg( area_index, tmpsize, tmpbuf );
	return 0;
}

// 组织一个单元真正进入区域的数据包
int mapunit_enterinfo( int unit_index, char *databuf, int *psize )
{
	char tmpbuf[2048];
	int allsize = 0;
	int sizeleft = 2048;
	int cursize = 0;
	SLK_NetS_AddMapUnit info = { 0 };
	if ( mapunit_getattr( unit_index, &info ) < 0 )
		return 0;

	if ( (cursize = netsend_addmapunit_S( tmpbuf, sizeleft, &info )) == 0 )
		return -1;
	sizeleft -= cursize;
	allsize += cursize;

	if ( *psize + allsize > 2040 )
	{
		return -1;
	}
	memcpy( databuf + (*psize), tmpbuf, allsize );
	*psize += allsize;
	return 0;
}

// 组织一个单元真正离开区域的数据包
int mapunit_leaveinfo( int unit_index, char *databuf, int *psize )
{
	char tmpbuf[2048];
	int allsize = 0;
	int sizeleft = 2048;
	int cursize = 0;
	SLK_NetS_DelMapUnit info = { 0 };
	info.m_unit_index = unit_index;
	if ( (cursize = netsend_delmapunit_S( tmpbuf, sizeleft, &info )) == 0 )
		return -1;
	sizeleft -= cursize;
	allsize += cursize;

	if ( *psize + allsize > 2040 )
	{
		return -1;
	}
	memcpy( databuf + (*psize), tmpbuf, allsize );
	*psize += allsize;

	return 0;
}


// 处理单一区域的进入事件
int mapunit_enterarea( int unit_index, int area_index )
{
	char msg[2048] = { 0 };
	int size = 0;
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;

	// 组织数据包
	mapunit_enterinfo( unit_index, msg + sizeof( short ), &size );

	// 消息发送到区域
	if ( size > 0 )
	{
		*(unsigned short *)msg = size;
		area_send( area_index, size + sizeof( short ), msg );
	}
	return 0;
}

// 处理单一区域的离开事件
int mapunit_leavearea( int unit_index, int area_index )
{
	char msg[2048] = { 0 };
	int size = 0;
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;

	// 组织数据包
	mapunit_leaveinfo( unit_index, msg + sizeof( short ), &size );

	// 消息发送到区域
	if ( size > 0 )
	{
		*(unsigned short *)msg = size;
		area_send( area_index, size + sizeof( short ), msg );
	}

	return 0;
}


// 重置坐标信息
int mapunit_resetinfo( int unit_index, char *databuf, int *psize )
{
	char tmpbuf[2048];
	int tmpsize;
	int leftsize = 2048;
	short posx = 0, posy = 0;
	mapunit_getpos( unit_index, &posx, &posy );

	SLK_NetS_MapUnitCorrdinate Value = {};
	Value.m_unit_index = unit_index;
	Value.m_posx = posx;
	Value.m_posy = posy;
	tmpsize = netsend_mapunitcorrdinate_S( tmpbuf, leftsize, &Value );

	memcpy( databuf + *psize, tmpbuf, tmpsize );
	*psize += tmpsize;
	return 0;
}

// 处理单一区域的更新信息事件
int mapunit_resetarea( int unit_index, int area_index )
{
	char msg[2048] = { 0 };
	int size = 0;

	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;

	// 组织数据包
	mapunit_resetinfo( unit_index, msg + sizeof( short ), &size );

	// 消息发送到区域
	if ( size > 0 )
	{
		*(unsigned short *)msg = size;
		area_send( area_index, size + sizeof( short ), msg );
	}
	return 0;
}

// 根据位置获取当前位置的显示单元信息
MapUnit *mapunit_getinfo_withpos( short posx, short posy, char excude_unittype, int excude_index )
{
	int unit_index = mapunit_getindex_withpos( posx, posy, excude_unittype, excude_index );
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return NULL;
	return &g_mapunit[unit_index];
}

int mapunit_getindex_withpos( short posx, short posy, char excude_unittype, int excude_index )
{
	Area *pArea = NULL;
	int area_index = area_getindex( posx, posy );
	if ( area_index < 0 || area_index >= g_map.m_nAreaMaxCount )
		return -1;
	pArea = &g_map.m_aArea[area_index];
	if ( pArea->unit_head < 0 && pArea->unit_tail < 0 )
		return -1;
	int head = pArea->unit_head;
	int tail = pArea->unit_tail;
	int cur_index = g_map.m_aArea[area_index].unit_head;
	while ( cur_index >= 0 )
	{
		int next_index = g_mapunit[cur_index].next_index;
		if ( cur_index < 0 )
		{
			cur_index = next_index;
			continue;
		}
		MapUnit *pMapUnit = &g_mapunit[cur_index];
		short target_posx = -1;
		short target_posy = -1;
		switch ( pMapUnit->type )
		{
		case MAPUNIT_TYPE_CITY:
			{
			/*if ( pMapUnit->index < 0 || pMapUnit->index >= g_city_maxcount )
				break;
			target_posx = g_city[pMapUnit->index].posx;
			target_posy = g_city[pMapUnit->index].posy;
			if ( target_posx == posx && target_posy == posy ||
				 target_posx == posx && target_posy-1 == posy || 
				 target_posx+1 == posx && target_posy-1 == posy ||
				 target_posx+1 == posx && target_posy == posy )
			{
				return cur_index;
			}*/

			break;
			}
		case MAPUNIT_TYPE_ARMY:
			{
			//if ( pMapUnit->index < 0 || pMapUnit->index >= g_army_maxcount )
			//	break;
			//target_posx = g_army[pMapUnit->index].posx;
			//target_posy = g_army[pMapUnit->index].posy;
			break;
			}
		default:
			break;
		}
		if ( target_posx != posx || target_posy != posy )
		{
			cur_index = next_index;
			continue;
		}
		else
		{
			if ( pMapUnit->type != excude_unittype )
			{
				return cur_index;
			}
			else if ( pMapUnit->index != excude_index )
			{
				return cur_index;
			}
			else
			{
				cur_index = next_index;
			}
		}
	}
	return -1;
}

