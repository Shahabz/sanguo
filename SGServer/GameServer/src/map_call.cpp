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
#include "map_call.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern City * g_city;
extern int g_city_maxcount;

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
			City *pCity = city_getptr( g_map_call[tmpi].city_index );
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

// 地图召唤
int map_call( int actor_index, int type )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1;
	int callnum = nation_official_right( pCity->official, NATION_OFFICIAL_RIGHT_CALLNUM );
	if ( callnum == 0 )
		return -1;

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

	g_map_call[index].index = index;
	g_map_call[index].actorid = pCity->actorid;
	g_map_call[index].city_index = pCity->index;
	g_map_call[index].posx = pCity->posx;
	g_map_call[index].posy = pCity->posy;
	g_map_call[index].endtimestamp = (int)time( NULL ) + 600;
	g_map_call[index].invite = 5;
	g_map_call[index].num = 0;
	g_map_call[index].maxnum = 4;
	pCity->mapcall = index;
	mapunit_update( MAPUNIT_TYPE_CITY, pCity->index, pCity->unit_index );

	// 公告
	// 6022	{0}<color=d95df4ff>{1}</color>在{2}[{3}]对{4}的勇士发起召唤， <color=25c9ffff><url={3}>[点击查看]</url></color>
	char v1[32] = { 0 };
	char v2[32] = { 0 };
	char v3[32] = { 0 };
	char v4[32] = { 0 };
	char v5[32] = { 0 };
	sprintf( v1, "%s%d", TAG_OFFICIAL, pCity->official );
	sprintf( v2, "%s", pCity->name );
	sprintf( v3, "%s%d", TAG_ZONEID, pCity->zone );
	sprintf( v4, "%d,%d", pCity->posx, pCity->posy );
	sprintf( v5, "%s%d", TAG_NATION, pCity->nation );
	system_talkjson( 0, pCity->nation, 6022, v1, v2, v3, v4, v5, NULL, 1 );
	return 0;
}
