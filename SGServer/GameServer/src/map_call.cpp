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
#include "actor.h"
#include "hero.h"
#include "area.h"
#include "actor_send.h"
#include "actor_times.h"
#include "server_netsend_auto.h"
#include "mapunit.h"
#include "mail.h"
#include "actor_notify.h"
#include "item.h"
#include "global.h"
#include "system.h"
#include "city.h"
#include "chat.h"
#include "nation.h"
#include "map.h"
#include "map_zone.h"
#include "map_call.h"

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

MapCall *g_map_call = NULL;
int g_map_call_maxcount = 0;
extern int g_map_call_maxindex;

// 读档完毕的回调
int map_call_loadcb( int index )
{
	if ( index < 0 || index >= g_map_call_maxcount )
		return -1;
	for ( int tmpi = 0; tmpi < g_map_call_maxcount; tmpi++ )
	{
		if ( g_map_call[tmpi].actorid > 0 )
		{
			g_map_call[tmpi].city_index = city_getindex_withactorid( g_map_call[tmpi].actorid );
			City *pCity = city_indexptr( g_map_call[tmpi].city_index );
			if ( pCity )
			{
				pCity->mapcall = index;
			}
		}
	}
	return 0;
}

int map_call_load()
{
	g_map_call_maxcount = 100;
	g_map_call = (MapCall*)malloc( sizeof( MapCall ) * g_map_call_maxcount );
	memset( g_map_call, 0, sizeof( MapCall ) * g_map_call_maxcount );
	for ( int tmpi = 0; tmpi < g_map_call_maxcount; tmpi++ )
	{
		g_map_call[tmpi].city_index = -1;
	}
	map_call_load_auto( map_call_getptr, map_call_loadcb, "map_call" );
	return 0;
}

int map_call_save( FILE *fp )
{
	map_call_batch_save_auto( g_map_call, g_map_call_maxcount, "map_call", fp );
	return 0;
}

inline MapCall *map_call_getptr( int index )
{
	if ( index < 0 || index >= g_map_call_maxcount )
		return NULL;
	return &g_map_call[index];
}

// 每秒一次检查
void map_call_logic()
{
	int nowtime = (int)time( NULL );
	for ( int tmpi = 1; tmpi < g_map_call_maxcount; tmpi++ )
	{
		if ( g_map_call[tmpi].actorid <= 0 )
			continue;
		if ( nowtime >= g_map_call[tmpi].endtimestamp )
		{
			map_call_delete( tmpi );
		}
	}
}

void map_call_delete( int index )
{
	if ( index <= 0 || index >= g_map_call_maxcount )
		return;
	City *pCity = city_indexptr( g_map_call[index].city_index );
	if ( pCity )
	{
		pCity->mapcall = 0;
		mapunit_update( MAPUNIT_TYPE_CITY, pCity->index, pCity->unit_index );
	}

	g_map_call[index].actorid = 0;
	g_map_call[index].city_index = -1;
	g_map_call[index].limitlevel = 0;
	g_map_call[index].endtimestamp = 0;
	g_map_call[index].notifycd = 0;
	g_map_call[index].num = 0;
	g_map_call[index].maxnum = 0;
}

// 地图召唤
int map_call( int actor_index, int level )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->mapcall > 0 )
	{
		return -1;
	}
	int callnummax = nation_official_right( pCity->official, NATION_OFFICIAL_RIGHT_CALLNUM );
	if ( callnummax == 0 )
		return -1;

	int today_callnum = actor_get_today_char_times( actor_index, TODAY_CHAR_MAPCALLNUM );
	if ( today_callnum >= callnummax )
	{
		actor_notify_alert( actor_index, 1855 );
		return -1;
	}

	// 拥有免费次数
	int itemkind = 131;
	int costnum = 4;
	int itemnum = item_getitemnum( actor_index, itemkind );
	if ( g_actors[actor_index].mapcallfree <= 0 )
	{
		if ( itemnum < costnum )
		{
			actor_notify_alert( actor_index, 779 );
			return -1;
		}
	}
		 
	int index = -1;
	for ( int tmpi = 1; tmpi < g_map_call_maxcount; tmpi++ )
	{
		if ( g_map_call[tmpi].actorid <= 0 )
		{
			index = tmpi;
			break;
		}
	}

	if ( index < 0 )
		return -1;
	if ( level <= 0 )
		level = 1;
	else if ( level >= global.actorlevel_max )
		level = global.actorlevel_max;

	g_map_call[index].index = index;
	g_map_call[index].actorid = pCity->actorid;
	g_map_call[index].city_index = pCity->index;
	g_map_call[index].limitlevel = level;
	g_map_call[index].endtimestamp = (int)time( NULL ) + 600;
	g_map_call[index].notifycd = 0;
	g_map_call[index].num = 0;
	g_map_call[index].maxnum = nation_official_right( pCity->official, NATION_OFFICIAL_RIGHT_CALLACTORNUM );;
	pCity->mapcall = index;
	mapunit_update( MAPUNIT_TYPE_CITY, pCity->index, pCity->unit_index );
	actor_add_today_char_times( actor_index, TODAY_CHAR_MAPCALLNUM );

	if ( g_actors[actor_index].mapcallfree <= 0 )
	{
		item_lost( actor_index, itemkind, costnum, PATH_MAPCALL );
	}
	else
	{
		g_actors[actor_index].mapcallfree -= 1;
	}

	// 公告
	map_call_notify( actor_index );
	return 0;
}

// 召唤公告
int map_call_notify( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	int index = pCity->mapcall;
	if ( index <= 0 || index >= g_map_call_maxcount )
		return -1;

	int nowtime = (int)time( NULL );
	if ( nowtime < g_map_call[index].notifycd )
	{
		actor_notify_alert( actor_index, 1858 );
		return -1;
	}
	g_map_call[index].notifycd = (int)time( NULL ) + 10;

	// 6022	{0}<color=d95df4ff>{1}</color>在{2}[{3}]对{4}{5}级以上勇士发起召唤， <color=25c9ffff><url={3}>[点击查看]</url></color>
	char v1[32] = { 0 };
	char v2[32] = { 0 };
	char v3[32] = { 0 };
	char v4[32] = { 0 };
	char v5[32] = { 0 };
	char v6[32] = { 0 };
	sprintf( v1, "%s%d", TAG_OFFICIAL, pCity->official );
	sprintf( v2, "%s", pCity->name );
	sprintf( v3, "%s%d", TAG_ZONEID, pCity->zone );
	sprintf( v4, "%d,%d", pCity->posx, pCity->posy );
	sprintf( v5, "%s%d", TAG_NATION, pCity->nation );
	sprintf( v5, "%d", g_map_call[index].limitlevel );
	system_talkjson( 0, pCity->nation, 6022, v1, v2, v3, v4, v5, v6, 1 );
	return 0;
}

// 响应召唤
int map_call_response( int actor_index, int index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	
	// 武将有出征的
	for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
	{
		if ( pCity->hero[tmpi].state > 0 )
		{
			actor_notify_pop( pCity->actor_index, 1001 ); // 召回在外征战的武将
			return -1;
		}
	}

	MapCall *pMapCall = map_call_getptr( index );
	if ( !pMapCall )
		return -1;

	City *pTargetCity = city_indexptr( pMapCall->city_index );
	if ( !pTargetCity )
		return -1;

	int zoneid = map_zone_getid( pTargetCity->posx, pTargetCity->posy );
	if ( zoneid <= 0 || zoneid >= g_zoneinfo_maxnum )
		return -1;

	//if ( pCity->level < g_zoneinfo[zoneid].actorlevel )
	//{ // 需要玩家等级{0}才可迁移到该地图
	//	char v1[32] = { 0 };
	//	sprintf( v1, "%d", g_zoneinfo[zoneid].actorlevel );
	//	actor_notify_alert_v( actor_index, 1366, v1, NULL );
	//	return -1;
	//}
	if ( pCity->mokilllv < g_zoneinfo[zoneid].killenemy )
	{ // 需要击败{ 0 }级流寇才可迁移到该地图
		char v1[32] = { 0 };
		sprintf( v1, "%d", g_zoneinfo[zoneid].killenemy );
		actor_notify_alert_v( actor_index, 1367, v1, NULL );
		return -1;
	}
	if ( map_distance( pCity->posx, pCity->posy, pTargetCity->posx, pTargetCity->posy ) <= global.mapcall_nearby )
	{ // 您已经在附近
		actor_notify_alert( actor_index, 1868 );
		return -1;
	}

	int range = global.mapcall_range;
	short pPosx = -1;
	short pPosy = -1;
	map_getrandpos_withrange( pTargetCity->posx, pTargetCity->posy, range, &pPosx, &pPosy );
	if ( pPosx < 0 || pPosy < 0 )
	{
		for ( int range_ex = range + 1; range_ex < range + 32; range_ex++ )
		{
			map_getrandpos_withrange( pTargetCity->posx, pTargetCity->posy, range_ex, &pPosx, &pPosy );
			if ( pPosx >= 0 && pPosy >= 0 )
				break;
		}
	}
	if ( pPosx <= 0 || pPosy <= 0 )
	{
		actor_notify_alert( actor_index, 1857 );
		return -1;
	}
	city_move( pCity, pPosx, pPosy );

	pMapCall->num += 1;
	if ( pMapCall->num >= pMapCall->maxnum )
	{
		map_call_delete( index );
	}
	mapunit_update( MAPUNIT_TYPE_CITY, pTargetCity->index, pTargetCity->unit_index );
	return 0;
}

// 发送我的召唤信息
int map_call_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->mapcall > 0 )
	{
		return -1;
	}
	int callnummax = nation_official_right( pCity->official, NATION_OFFICIAL_RIGHT_CALLNUM );
	int callactornum = nation_official_right( pCity->official, NATION_OFFICIAL_RIGHT_CALLACTORNUM );
	int today_callnum = actor_get_today_char_times( actor_index, TODAY_CHAR_MAPCALLNUM );

	int value[4] = { 0 };
	value[0] = 0;
	value[1] = callactornum;
	value[2] = callnummax - today_callnum;
	value[3] = g_actors[actor_index].mapcallfree;
	actor_notify_value( pCity->actor_index, NOTIFY_MAPCALL, 4, value, NULL );
	return 0;
}
