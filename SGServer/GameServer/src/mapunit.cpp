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
#include "map_pickup.h"
#include "nation_hero.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Map g_map;
extern Actor *g_actors;
extern int g_maxactornum;

// ������(�����Ϊ�����)
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
		g_mapunit_maxcount = 90000;
	}
	else
	{
		g_mapunit_maxcount = g_Config.max_mapunitcount;
	}
	g_mapunit = (MapUnit *)malloc( sizeof( MapUnit )*g_mapunit_maxcount );
	memset( g_mapunit, 0, sizeof( MapUnit )*g_mapunit_maxcount );
	for ( int tmpi = 0; tmpi < g_mapunit_maxcount; tmpi++ )
	{
		g_mapunit[tmpi].lastadd_areaindex = -1;
		g_mapunit[tmpi].pre_index = -1;
		g_mapunit[tmpi].next_index = -1;
	}
	printf_msg( "MapUnit  maxcount=%d  memory=%0.2fMB\n", g_mapunit_maxcount, (sizeof( MapUnit )*g_mapunit_maxcount) / 1024.0 / 1024.0 );
	return 0;
}

// ��ȡ��ʾ��Ԫ��Ӧ��������Ϣ
int mapunit_getattr( int unit_index, SLK_NetS_AddMapUnit *pAttr )
{
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;
	MapUnit *pMapUnit = &g_mapunit[unit_index];
	pAttr->m_unit_index = unit_index;
	pAttr->m_type = pMapUnit->type;
	pAttr->m_namelen = 0;
	pAttr->m_prefixlen = 0;
	pAttr->m_int_value_count = 0;
	pAttr->m_short_value_count = 0;
	pAttr->m_char_value_count = 0;
	switch ( pMapUnit->type )
	{
	case MAPUNIT_TYPE_CITY:	// ��ҳǳ�
		city_makeunit( pMapUnit->index, pAttr );
		break;
	case MAPUNIT_TYPE_ARMY:	//  ����
		army_makeunit( pMapUnit->index, pAttr );
		break;
	case MAPUNIT_TYPE_TOWN://  ����
		map_town_makeunit( pMapUnit->index, pAttr );
		break;
	case MAPUNIT_TYPE_ENEMY:// ����
		map_enemy_makeunit( pMapUnit->index, pAttr );
		break;
	case MAPUNIT_TYPE_RES: // ��Դ
		map_res_makeunit( pMapUnit->index, pAttr );
		break;
	case MAPUNIT_TYPE_EVENT: // �¼�
		map_event_makeunit( pMapUnit->index, pAttr, pMapUnit->actorid );
		break;
	case MAPUNIT_TYPE_NATIONHERO: // ��������
		nation_hero_makeunit( pMapUnit->index, pAttr );
		break;
	case MAPUNIT_TYPE_PICKUP: // ʰȡ��
		map_pickup_makeunit( pMapUnit->index, pAttr );
		break;
	default:
		return -1;
	}
	return 0;
}

// ��ȡ��ʾ��Ԫ��Ӧ��λ��
int mapunit_getpos( int unit_index, short *posx, short *posy )
{
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;
	MapUnit *pMapUnit = &g_mapunit[unit_index];
	switch ( pMapUnit->type )
	{
	case MAPUNIT_TYPE_CITY:// ��ҳǳ�
		city_getpos( pMapUnit->index, posx, posy );
		break;
	case MAPUNIT_TYPE_ARMY://  ����
		army_getpos( pMapUnit->index, posx, posy );
		break;
	case MAPUNIT_TYPE_TOWN://  ����
		map_town_getpos( pMapUnit->index, posx, posy );
		break;
	case MAPUNIT_TYPE_ENEMY:// ����
		map_enemy_getpos( pMapUnit->index, posx, posy );
		break;
	case MAPUNIT_TYPE_RES: // ��Դ
		map_res_getpos( pMapUnit->index, posx, posy );
		break;
	case MAPUNIT_TYPE_EVENT: // �¼�
		map_event_getpos( pMapUnit->index, posx, posy );
		break;
	case MAPUNIT_TYPE_NATIONHERO: // ��������
		nation_hero_getpos( pMapUnit->index, posx, posy );
		break;
	case MAPUNIT_TYPE_PICKUP: // ʰȡ��
		map_pickup_getpos( pMapUnit->index, posx, posy );
		break;
	default:
		break;
	}
	return 0;
}

// ��ȡһ�����е�����
inline int mapunit_getfreeindex()
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
	{// ����ע�⣬��;���������ҿ�������Ч��
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

// �������ͺͶ�Ӧ����ȷ����Ԫ����
inline int mapunit_getindex( char type, int index )
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

// ����Ҫ��ʾ�ĳǳػ������ӵ���ʾ��Ԫ
int mapunit_add( char type, int index, int actorid )
{
	int unit_index = mapunit_getfreeindex();
	if ( unit_index < 0 )
		return -1;
	//memset( &g_mapunit[unit_index], 0, sizeof( MapUnit ) );
	g_mapunit[unit_index].type = type;
	g_mapunit[unit_index].index = index;
	g_mapunit[unit_index].actorid = actorid;
	g_mapunit[unit_index].lastadd_areaindex = -1;
	g_mapunit[unit_index].pre_index = -1;
	g_mapunit[unit_index].next_index = -1;
	short posx = 0, posy = 0;
	mapunit_getpos( unit_index, &posx, &posy );
	mapunit_enterworld( unit_index, posx, posy );
	return unit_index;
}

// ���Ѿ������ĵ�ͼ��Ԫɾ����
int mapunit_del( char type, int index, int unit_index )
{
	if ( unit_index < 0 )
		unit_index = mapunit_getindex( type, index );
	if ( unit_index < 0 )
		return -1;

	short posx = 0, posy = 0;
	mapunit_getpos( unit_index, &posx, &posy );
	int area_index = area_getindex( posx, posy );

	// ���������Լ��ģ���ôֻ�����Լ�������
	if ( g_mapunit[unit_index].actorid > 0 )
	{
		int actor_index = actor_getindex_withid( g_mapunit[unit_index].actorid );
		if ( actor_index >= 0 && actor_index < g_maxactornum )
		{
			char msg[2048] = { 0 };
			int size = 0;
			mapunit_leaveinfo( unit_index, msg + sizeof( short ), &size );
			*(unsigned short *)msg = size;
			sendtoclient( actor_index, msg, size + sizeof( short ) );
		}
	}
	else
	{
		char tmpbuf[2048];
		int sizeleft = 2048;
		SLK_NetS_DelMapUnit info = { 0 };
		info.m_unit_index = unit_index;
		int tmpsize = netsend_delmapunit_S( tmpbuf, sizeleft, &info );
		area_sendmsg( area_index, tmpsize, tmpbuf );
	}

	area_delmapunit( unit_index, area_index );
	memset( &g_mapunit[unit_index], 0, sizeof( MapUnit ) );
	g_mapunit[unit_index].lastadd_areaindex = -1;
	g_mapunit[unit_index].pre_index = -1;
	g_mapunit[unit_index].next_index = -1;
	return 0;
}

// ���µ�ͼ��Ԫ����
int mapunit_update( char type, int index, int unit_index )
{
	if ( unit_index < 0 )
	     unit_index = mapunit_getindex( type, index );
	if ( unit_index < 0 )
		return -1;

	// ���������Լ��ģ���ôֻ�����Լ�������
	if ( g_mapunit[unit_index].actorid > 0 )
	{
		int actor_index = actor_getindex_withid( g_mapunit[unit_index].actorid );
		if ( actor_index >= 0 && actor_index < g_maxactornum )
		{
			char msg[2048] = { 0 };
			int size = 0;
			mapunit_updateinfo( unit_index, msg + sizeof( short ), &size );
			*(unsigned short *)msg = size;
			sendtoclient( actor_index, msg, size + sizeof( short ) );
		}
	}
	else
	{
		SLK_NetS_AddMapUnit unitinfo = { 0 };
		mapunit_getattr( unit_index, &unitinfo );

		SLK_NetS_UpdateMapUnit updateinfo = { 0 };
		memcpy( &updateinfo.m_info, &unitinfo, sizeof( SLK_NetS_AddMapUnit ) );

		// ��֯��Ϣ
		char tmpbuf[2048];
		int sizeleft = 2048;
		int tmpsize = netsend_updatemapunit_S( tmpbuf, sizeleft, &updateinfo );
		int area_index = area_getindex( unitinfo.m_posx, unitinfo.m_posy );
		area_sendmsg( area_index, tmpsize, tmpbuf );
		//area_clearmsg( area_index );
	}
	return 0;
}

// ��ʾ��Ԫ���������ͼ�������������У���֪ͨ����
int mapunit_enterworld( int unit_index, short posx, short posy )
{
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return 0;
	int area_index = area_getindex( posx, posy );
	if ( area_addmapunit( unit_index, area_index ) < 0 )
	{
		write_gamelog( "AreaAdd Error unit_index:%d", unit_index );
		return -1;
	}

	// ���������Լ��ģ���ôֻ�����Լ�������
	if ( g_mapunit[unit_index].actorid > 0 )
	{
		int actor_index = actor_getindex_withid( g_mapunit[unit_index].actorid );
		if ( actor_index >= 0 && actor_index < g_maxactornum )
		{
			char msg[2048] = { 0 };
			int size = 0;
			mapunit_enterinfo( unit_index, msg + sizeof( short ), &size );
			*(unsigned short *)msg = size;
			sendtoclient( actor_index, msg, size + sizeof( short ) );
		}
	}
	else
	{
		char tmpbuf[2048] = { 0 };
		int tmpsize = 0;
		mapunit_enterinfo( unit_index, tmpbuf, &tmpsize );
		// ��֯���ݰ����͵����򻺳�
		area_sendmsg( area_index, tmpsize, tmpbuf );
	}
	return 0;
}

// ��֯һ����Ԫ����������������ݰ�
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

// ��֯һ����Ԫ�����뿪��������ݰ�
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

// ��֯һ����Ԫ���µ����ݰ�
int mapunit_updateinfo( int unit_index, char *databuf, int *psize )
{
	char tmpbuf[2048];
	int allsize = 0;
	int sizeleft = 2048;
	int cursize = 0;
	SLK_NetS_AddMapUnit unitinfo = { 0 };
	mapunit_getattr( unit_index, &unitinfo );

	SLK_NetS_UpdateMapUnit updateinfo = { 0 };
	memcpy( &updateinfo.m_info, &unitinfo, sizeof( SLK_NetS_AddMapUnit ) );

	if ( (cursize = netsend_updatemapunit_S( tmpbuf, sizeleft, &updateinfo )) == 0 )
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

// ����һ����Ľ����¼�
int mapunit_enterarea( int unit_index, int area_index )
{
	char msg[2048] = { 0 };
	int size = 0;
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;

	// ��֯���ݰ�
	mapunit_enterinfo( unit_index, msg + sizeof( short ), &size );

	// ��Ϣ���͵�����
	if ( size > 0 )
	{
		*(unsigned short *)msg = size;
		area_send( area_index, size + sizeof( short ), msg );
	}
	return 0;
}

// ����һ������뿪�¼�
int mapunit_leavearea( int unit_index, int area_index )
{
	char msg[2048] = { 0 };
	int size = 0;
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;

	// ��֯���ݰ�
	mapunit_leaveinfo( unit_index, msg + sizeof( short ), &size );

	// ��Ϣ���͵�����
	if ( size > 0 )
	{
		*(unsigned short *)msg = size;
		area_send( area_index, size + sizeof( short ), msg );
	}

	return 0;
}


// ����������Ϣ
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

// ����һ����ĸ�����Ϣ�¼�
int mapunit_resetarea( int unit_index, int area_index )
{
	char msg[2048] = { 0 };
	int size = 0;

	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;

	// ��֯���ݰ�
	mapunit_resetinfo( unit_index, msg + sizeof( short ), &size );

	// ��Ϣ���͵�����
	if ( size > 0 )
	{
		*(unsigned short *)msg = size;
		area_send( area_index, size + sizeof( short ), msg );
	}
	return 0;
}

// ����λ�û�ȡ��ǰλ�õ���ʾ��Ԫ��Ϣ
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
		case MAPUNIT_TYPE_CITY: // �ǳ�
			city_getpos( pMapUnit->index, &target_posx, &target_posy );
			break;
		case MAPUNIT_TYPE_ARMY:// ����
			army_getpos( pMapUnit->index, &target_posx, &target_posy );
			break;
		case MAPUNIT_TYPE_TOWN://  ����
			map_town_getpos( pMapUnit->index, &target_posx, &target_posy );
			break;
		case MAPUNIT_TYPE_ENEMY:// ����
			map_enemy_getpos( pMapUnit->index, &target_posx, &target_posy );
			break;
		case MAPUNIT_TYPE_RES: // ��Դ
			map_res_getpos( pMapUnit->index, &target_posx, &target_posy );
			break;
		case MAPUNIT_TYPE_EVENT: // �¼�
			map_event_getpos( pMapUnit->index, &target_posx, &target_posy );
			break;
		case MAPUNIT_TYPE_NATIONHERO: // ��������
			nation_hero_getpos( pMapUnit->index, &target_posx, &target_posy );
			break;
		case MAPUNIT_TYPE_PICKUP: // ʰȡ��
			map_pickup_getpos( pMapUnit->index, &target_posx, &target_posy );
			break;
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

int mapunit_action( int unit_index, char action )
{
	if ( unit_index < 0 )
	{
		return -1;
	}
	short posx, posy;
	mapunit_getpos( unit_index, &posx, &posy );
	// ֪ͨ���򲥷Ŷ���
	SLK_NetS_MapUnitAction info = { 0 };
	info.m_unit_index = unit_index;
	info.m_action = action;
	int area_index = area_getindex( posx, posy );
	netsend_mapunitaction_S( area_index, SENDTYPE_AREA, &info );
	return 0;
}

int mappos_action( short posx, short posy, char action )
{
	// ֪ͨ���򲥷Ŷ���
	SLK_NetS_MapPosAction info = { 0 };
	info.m_posx = posx;
	info.m_posy = posy;
	info.m_action = action;
	int area_index = area_getindex( posx, posy );
	netsend_mapposaction_S( area_index, SENDTYPE_AREA, &info );
	return 0;
}
