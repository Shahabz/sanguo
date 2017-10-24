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
#include "system.h"
#include "actor.h"
#include "server_netsend_auto.h"
#include "actor_send.h"
#include "map.h"
#include "global.h"
#include "mapunit.h"
#include "zoneunit.h"
#include "script_auto.h"
#include "city.h"
#include "map_zone.h"
#include "map_town.h"
#include "army_group.h"

extern Global global;
extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

extern MapZoneInfo *g_zoneinfo;
extern int g_zoneinfo_maxnum;

extern MapTownInfo *g_towninfo;
extern int g_towninfo_maxnum;

extern MapTown *g_map_town;
extern int g_map_town_maxcount;

extern ArmyGroup *g_armygroup;
extern int g_armygroup_maxcount;

extern Map g_map;

extern ZoneUnit *g_zoneunit;
extern int g_zoneunit_maxcount;

MapZone *g_map_zone = NULL;
int g_map_zone_maxcount = 0;

// 读档完毕的回调
int map_zone_loadcb( int zoneid )
{
	if ( zoneid <= 0 || zoneid >= g_map_zone_maxcount )
		return -1;
	return 0;
}

int map_zone_load()
{
	g_map_zone_maxcount = g_zoneinfo_maxnum;
	g_map_zone = (MapZone*)malloc( sizeof( MapZone ) * g_map_zone_maxcount );
	memset( g_map_zone, 0, sizeof( MapZone ) * g_map_zone_maxcount );
	printf_msg( "MapZone  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_map_zone_maxcount, (sizeof( MapZone )*g_map_zone_maxcount) / 1024.0 / 1024.0, sizeof( MapZone ) / 1024.0 );
	for ( int zoneid = 1; zoneid < g_map_zone_maxcount; zoneid++ )
	{
		if ( g_zoneinfo[zoneid].id <= 0 )
			continue;
		g_map_zone[zoneid].zoneid = zoneid;
		g_map_zone[zoneid].unit_head = -1;
		g_map_zone[zoneid].unit_tail = -1;
		if ( g_zoneinfo[zoneid].open == 1 )
		{
			g_map_zone[zoneid].allow = 1;
		}
	}
	map_zone_load_auto( map_zone_getptr, map_zone_loadcb, "map_zone" );
	return 0;
}

int map_zone_save( FILE *fp )
{
	map_zone_batch_save_auto( g_map_zone, g_map_zone_maxcount, "map_zone", fp );
	return 0;
}

MapZone *map_zone_getptr( int zoneid )
{
	if ( zoneid <= 0 || zoneid >= g_map_zone_maxcount )
		return NULL;
	return &g_map_zone[zoneid];
}

// 坐标是否在指定地区里
char map_zone_inrange( int zoneid, short posx, short posy )
{
	if ( zoneid <= 0 || zoneid >= g_zoneinfo_maxnum )
		return 0;
	if ( posx >= g_zoneinfo[zoneid].top_left_posx && posx <= g_zoneinfo[zoneid].bottom_right_posx &&
		posy >= g_zoneinfo[zoneid].top_left_posy && posy <= g_zoneinfo[zoneid].bottom_right_posy )
	{
		return 1;
	}
	return 0;
}

// 获取地区id
char map_zone_getid( short posx, short posy )
{
	int zonex, zoney;
	if ( posx >= g_map.m_nMaxWidth )
		posx = g_map.m_nMaxWidth - 1;
	if ( posy >= g_map.m_nMaxHeight )
		posy = g_map.m_nMaxHeight - 1;
	zonex = (posx) / 100;
	zoney = (posy) / 100;
	return zoney*(5) + zonex + 1;
}

// 检查区域链表，这个区域的单元坐标位置是否真的属于这个区域，有可能区域发生变化吗
int map_zone_checklist( int zoneid )
{
	int tmpid;
	int head, tail;
	MapZone *pZone;
	short posx = 0, posy = 0;
	if ( zoneid < 0 || zoneid >= g_map_zone_maxcount )
		return 0;
	pZone = &g_map_zone[zoneid];
	if ( pZone->unit_head < 0 && pZone->unit_tail < 0 )
		return 0;

	head = pZone->unit_head;
	tail = pZone->unit_tail;

	if ( head >= 0 )
	{
		zoneunit_getpos( head, &posx, &posy );
		tmpid = map_zone_getid( posx, posy );
		if ( tmpid != zoneid )
		{
			pZone->unit_head = -1;
			pZone->unit_tail = -1;
			write_gamelog( "ZoneAreaHeadError: Area[%d] Head[%d(%d)]", zoneid, tmpid, tail );
			return -1;
		}
	}
	if ( tail >= 0 )
	{
		mapunit_getpos( tail, &posx, &posy );
		tmpid = map_zone_getid( posx, posy );
		if ( tmpid != zoneid )
		{
			pZone->unit_head = -1;
			pZone->unit_tail = -1;
			write_gamelog( "ZoneAreaTailError: Area[%d] Tail[%d(%d)]", zoneid, tmpid, tail );
			return -1;
		}
	}
	return 0;
}

// 将显示单元添加到地区
int map_zone_addunit( int unit_index, int zoneid )
{
	if ( zoneid < 0 || zoneid >= g_zoneinfo_maxnum )
		return 0;
	MapZone *pZone = &g_map_zone[zoneid];
	if ( pZone == NULL || pZone->unit_tail == unit_index )
		return 0;
	// 如果之前有地区信息，先从之前地区中移除
	map_zone_delunit( unit_index );
	// 检查地区链表和地区编号
	map_zone_checklist( zoneid );

	if ( pZone->unit_head < 0 || pZone->unit_tail < 0 )
	{
		g_zoneunit[unit_index].pre_index = -1;
		g_zoneunit[unit_index].next_index = -1;
		pZone->unit_head = pZone->unit_tail = unit_index;
		g_zoneunit[unit_index].lastadd_zoneid = zoneid;
		return 0;
	}

	g_zoneunit[unit_index].pre_index = pZone->unit_tail;
	g_zoneunit[pZone->unit_tail].next_index = unit_index;
	if ( g_zoneunit[pZone->unit_tail].next_index == pZone->unit_tail )
	{
		write_gamelog( "ZOneActorListError:[%d-%d]", unit_index, pZone->unit_tail );
		g_zoneunit[pZone->unit_tail].next_index = -1;
	}
	pZone->unit_tail = unit_index;
	g_zoneunit[unit_index].lastadd_zoneid = zoneid;
	return 0;
}

// 将显示单元移除出地区
int map_zone_delunit( int unit_index )
{
	int tmpi;
	int zone_id_cur;
	short lastadd_zoneid;
	MapZone *pZoneLast;
	MapZone *pZoneCur = NULL;
	short posx = 0, posy = 0;
	if ( unit_index < 0 || unit_index >= g_zoneunit_maxcount )
		return -1;
	lastadd_zoneid = g_zoneunit[unit_index].lastadd_zoneid;
	if ( lastadd_zoneid < 0 || lastadd_zoneid >= g_zoneunit_maxcount )
		return -1;

	// 之前所在区域
	pZoneLast = &g_map_zone[lastadd_zoneid];
	if ( pZoneLast == NULL )
		return -1;

	// 获取当前所在的区域
	zoneunit_getpos( unit_index, &posx, &posy );
	zone_id_cur = map_zone_getid( posx, posy );
	if ( zone_id_cur >= 0 && zone_id_cur < g_zoneunit_maxcount )
		pZoneCur = &g_map_zone[zone_id_cur];

	// 将它前面的单元和它后面的单元连上
	if ( g_zoneunit[unit_index].pre_index >= 0 )
	{
		tmpi = g_zoneunit[unit_index].pre_index;
		g_zoneunit[tmpi].next_index = g_zoneunit[unit_index].next_index;
		if ( g_zoneunit[tmpi].next_index == tmpi )
		{
			write_gamelog( "ActorListError:%d-%d", unit_index, tmpi );
			g_zoneunit[tmpi].next_index = -1;
		}
	}
	if ( g_zoneunit[unit_index].next_index >= 0 )
	{
		tmpi = g_zoneunit[unit_index].next_index;
		g_zoneunit[tmpi].pre_index = g_zoneunit[unit_index].pre_index;
	}

	// 如果这个索引是链表头，就将链表头指向它的下一个位置
	if ( pZoneLast->unit_head == unit_index )
		pZoneLast->unit_head = g_zoneunit[unit_index].next_index;
	if ( pZoneCur && pZoneCur->unit_head == unit_index )
	{
		pZoneCur->unit_head = g_zoneunit[unit_index].next_index;
		write_gamelog( "WARNNING: ZoneArea1" );
	}

	// 如果这个索引是链表尾，就将链表尾指向它的前一个位置
	if ( pZoneLast->unit_tail == unit_index )
		pZoneLast->unit_tail = g_zoneunit[unit_index].pre_index;
	if ( pZoneCur && pZoneCur->unit_tail == unit_index )
	{
		pZoneCur->unit_tail = g_zoneunit[unit_index].pre_index;
		write_gamelog( "WARNNING: ZoneArea2" );
	}

	// 把自己的链表信息清空
	g_zoneunit[unit_index].pre_index = -1;
	g_zoneunit[unit_index].next_index = -1;
	g_zoneunit[unit_index].lastadd_zoneid = -1;
	return 0;
}

// 检查是不是在同一个区域
char map_zone_checksame( short posx, short posy, short tposx, short tposy )
{
	char n = map_zone_getid( posx, posy );
	char m = map_zone_getid( tposx, tposy );
	if ( n == m )
	{
		return 1;
	}
	return 0;
}

// 设置地区归属
int map_zone_setnation( int zoneid, char nation )
{
	if ( zoneid <= 0 || zoneid >= g_zoneinfo_maxnum )
		return 0;
	g_map_zone[zoneid].nation = nation;
	return 0;
}

// 指定地区随机一个空坐标
int map_zone_randpos( short zoneid, short *pPosx, short *pPosy )
{
	if ( zoneid <= 0 || zoneid >= g_zoneinfo_maxnum )
		return -1;
	int step = 0;

	short findlistx[256] = { 0 };
	short findlisty[256] = { 0 };
	short offset = 0;
	for ( int tmpi = g_zoneinfo[zoneid].top_left_posx; tmpi <= g_zoneinfo[zoneid].bottom_right_posx; tmpi++ )
	{
		for ( int tmpj = g_zoneinfo[zoneid].top_left_posy; tmpj <= g_zoneinfo[zoneid].bottom_right_posy; tmpj++ )
		{
			short x = tmpi;
			short y = tmpj;
			if ( x <= 0 || y <= 0 || x >= g_map.m_nMaxWidth || y >= g_map.m_nMaxHeight )
				continue;
			if ( g_map.m_aTileData[x][y].unit_type > 0 )
				continue;
			// 找到所有的空余点
			findlistx[offset] = x;
			findlisty[offset] = y;
			offset += 1;
			if ( offset >= 256 )
				break;
		}
		if ( offset >= 256 )
			break;
	}

	if ( offset > 0 )
	{
		int index = rand() % offset;
		*pPosx = findlistx[index];
		*pPosy = findlisty[index];
	}
	else
	{
		*pPosx = -1;
		*pPosy = -1;
	}
	return 0;
}

//地图地区进入
void map_zone_change( int actor_index, short posx, short posy )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return;
	if ( posx < 0 && posy < 0 )
	{
		g_actors[actor_index].view_zoneid = 0;
		return;
	}
	char zoneid = map_zone_getid( posx, posy );
	if ( g_actors[actor_index].view_zoneid == zoneid )
		return;
	g_actors[actor_index].view_zoneid = zoneid;

	// 通知客户端进入新的地区
	SLK_NetS_MapZoneChange pValue = { 0 };
	MapZone *pZone = map_zone_getptr( zoneid );
	if ( !pZone )
		return;
	pValue.m_zoneid = zoneid;
	pValue.m_open = pZone->allow;
	pValue.m_nation = pZone->nation;
	netsend_mapzonechange_S( actor_index, SENDTYPE_ACTOR, &pValue );

	if ( pZone->allow == 1 )
	{
		// 发送玩家列表
		map_zone_citylist( actor_index, zoneid );

		// 发送城镇列表
		map_zone_townlist( actor_index, zoneid );
	}
}

// 地区玩家列表
int map_zone_citylist( int actor_index, int zoneid )
{
	if ( zoneid <= 0 || zoneid >= g_zoneinfo_maxnum )
		return -1;
	City *pCity = NULL;
	SLK_NetS_MapZoneUnitList pValue = { 0 };
	pValue.m_zoneid = zoneid;
	/*for ( int tmpi = g_zoneinfo[zoneid].top_left_posx; tmpi <= g_zoneinfo[zoneid].bottom_right_posx; tmpi++ )
	{
		for ( int tmpj = g_zoneinfo[zoneid].top_left_posy; tmpj <= g_zoneinfo[zoneid].bottom_right_posy; tmpj++ )
		{
			short x = tmpi;
			short y = tmpj;
			if ( x <= 0 || y <= 0 || x >= g_map.m_nMaxWidth || y >= g_map.m_nMaxHeight )
				continue;
			if ( g_map.m_aTileData[x][y].unit_type == MAPUNIT_TYPE_CITY )
			{
				pCity = city_indexptr( g_map.m_aTileData[x][y].unit_index );
				if ( pCity )
				{
					pValue.m_list[pValue.m_count].m_posx = pCity->posx;
					pValue.m_list[pValue.m_count].m_posy = pCity->posy;
					pValue.m_list[pValue.m_count].m_nation = pCity->nation;
					pValue.m_list[pValue.m_count].m_level = (char)pCity->level;
				}
				pValue.m_count += 1;
				if ( pValue.m_count >= 255 )
				{
					netsend_mapzoneunitlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
					pValue.m_count = 0;
				}
			}

		}
	}*/
	int loopnum = 0;
	MapZone *pZone = &g_map_zone[zoneid];
	if ( pZone->unit_head < 0 && pZone->unit_tail < 0 )
		return -1;
	int head = pZone->unit_head;
	int tail = pZone->unit_tail;
	int cur_index = pZone->unit_head;
	while ( cur_index >= 0 )
	{
		loopnum += 1;
		if ( loopnum >= 10000 )
		{// 防止死循环
			break;
		}
		int next_index = g_zoneunit[cur_index].next_index;
		if ( cur_index < 0 )
		{
			cur_index = next_index;
			continue;
		}
		ZoneUnit *pZoneUnit = &g_zoneunit[cur_index];
		short target_posx = -1;
		short target_posy = -1;
		if ( pZoneUnit->type == MAPUNIT_TYPE_CITY )
		{
			pCity = city_indexptr( pZoneUnit->index );
			if ( pCity )
			{
				pValue.m_list[pValue.m_count].m_zoneunit_index = cur_index;
				pValue.m_list[pValue.m_count].m_posx = pCity->posx;
				pValue.m_list[pValue.m_count].m_posy = pCity->posy;
				pValue.m_list[pValue.m_count].m_nation = pCity->nation;
				pValue.m_list[pValue.m_count].m_level = (char)pCity->level;
			}
			pValue.m_count += 1;
			if ( pValue.m_count >= 128 )
			{
				netsend_mapzoneunitlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
				pValue.m_count = 0;
			}
		}
		cur_index = next_index;
	}
	if ( pValue.m_count > 0 )
	{
		netsend_mapzoneunitlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}
	return 0;
}

// 地区城镇列表
int map_zone_townlist( int actor_index, int zoneid )
{
	if ( zoneid <= 0 || zoneid >= g_zoneinfo_maxnum )
		return -1;
	SLK_NetS_MapZoneTownList pValue = { 0 };
	pValue.m_zoneid = zoneid;
	for ( int townid = 1; townid < g_map_town_maxcount; townid++ )
	{
		if ( g_towninfo[townid].id <= 0 )
			continue;
		if ( g_map_town[townid].zoneid != zoneid )
			continue;
		pValue.m_list[pValue.m_count].m_townid = townid;
		pValue.m_list[pValue.m_count].m_nation = g_map_town[townid].nation;
		pValue.m_list[pValue.m_count].m_protect_sec = g_map_town[townid].protect_sec;
		for ( int tmpi = 0; tmpi < MAP_TOWN_UNDERFIRE_GROUP_MAX; tmpi++ )
		{
			int group_index = g_map_town[townid].underfire_groupindex[tmpi];
			if ( group_index < 0 || group_index >= g_armygroup_maxcount )
			{
				pValue.m_list[pValue.m_count].m_from_nation[tmpi] = -1;
				continue;
			}
			pValue.m_list[pValue.m_count].m_from_nation[tmpi] = g_armygroup[group_index].from_nation;
		}
		pValue.m_count += 1;
		if ( pValue.m_count >= 32 )
		{
			netsend_mapzonetownlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
			pValue.m_count = 0;
		}
	}
	if ( pValue.m_count > 0 )
	{
		netsend_mapzonetownlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}
	return 0;
}

// 地区城镇更新
int map_zone_townchange( short townid )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	SLK_NetS_MapZoneTown pValue = { 0 };
	pValue.m_townid = townid;
	pValue.m_nation = g_map_town[townid].nation;
	pValue.m_protect_sec = g_map_town[townid].protect_sec;
	netsend_mapzonetown_S( g_map_town[townid].zoneid, SENDTYPE_INZONE, &pValue );
	return 0;
}

// 地区皇城区域都城和名城所属列表
int map_zone_center_townlist( int actor_index )
{
	SLK_NetS_MapCenterTownList pValue = { 0 };
	for ( int townid = 1; townid < g_map_town_maxcount; townid++ )
	{
		if ( g_towninfo[townid].id <= 0 )
			continue;
		if ( g_map_town[townid].zoneid != 13 )
			continue;
		pValue.m_list[pValue.m_count].m_townid = townid;
		pValue.m_list[pValue.m_count].m_nation = g_map_town[townid].nation;
		pValue.m_count += 1;
		if ( pValue.m_count >= 32 )
		{
			netsend_mapcentertownlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
			pValue.m_count = 0;
		}
	}
	if ( pValue.m_count > 0 )
	{
		netsend_mapcentertownlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}
	return 0;
}

// 地区皇城区域都城和名城更新
int map_zone_center_townchange( int townid )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	if ( g_map_town[townid].zoneid != 13 )
		return -1;
	SLK_NetS_MapCenterTown pValue = { 0 };
	pValue.m_townid = townid;
	pValue.m_nation = g_map_town[townid].nation;
	netsend_mapcentertown_S( 0, SENDTYPE_WORLDMAP, &pValue );
	return 0;
}
