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
#include "city.h"
#include "building.h"
#include "actor.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "award.h"
#include "rank.h"
#include "map_zone.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

extern City *g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern MapZoneInfo *g_zoneinfo;
extern int g_zoneinfo_maxnum;

// 全服排名
ActorRank *g_rank_server = NULL;

// 区域排名
ActorRank *g_rank_zone[MAPZONE_MAXNUM] = { NULL };
int g_rank_zone_count[MAPZONE_MAXNUM] = { 0 };

// 国家排名
ActorRank *g_rank_nation[3] = { NULL };
int g_rank_nation_count[3] = { 0 };


int rank_init()
{
	if ( g_zoneinfo_maxnum != MAPZONE_MAXNUM )
	{
		printf_msg( "rank_init g_map_zone_maxcount != MAPZONE_MAXNUM" );
		return -1;
	}
	// 全服排行榜
	g_rank_server = (ActorRank*)malloc( sizeof( ActorRank )*g_city_maxcount );

	// 区域排行榜
	for ( int tmpi = 1; tmpi < MAPZONE_MAXNUM; tmpi++ )
	{
		if ( g_zoneinfo[tmpi].open == 0 )
			continue;
		g_rank_zone[tmpi] = (ActorRank*)malloc( sizeof( ActorRank )*g_city_maxcount );
		memset( &g_rank_zone[tmpi], 0, sizeof( ActorRank )*g_city_maxcount );
		g_rank_zone_count[tmpi] = 0;
	}

	// 国家排行榜
	for ( int tmpi = 0; tmpi < 3; tmpi++ )
	{
		g_rank_nation[tmpi] = (ActorRank*)malloc( sizeof( ActorRank )*g_city_maxcount );
		memset( &g_rank_nation[tmpi], 0, sizeof( ActorRank )*g_city_maxcount );
		g_rank_nation_count[tmpi] = 0;
	}

	rank_reset();
	rank_server();
	return 0;
}

int rank_reset()
{
	for ( int tmpi = 0; tmpi < g_city_maxcount; tmpi++ )
	{
		// 默认排名,避免如果初始顺序已经正确后,不会修改排名的问题
		if ( g_city[tmpi].actorid > 0 )
		{
			g_city[tmpi].rank_server = tmpi + 1;
			g_city[tmpi].rank_zone = tmpi + 1;
			g_city[tmpi].rank_nation = tmpi + 1;
		}
		else
		{
			g_city[tmpi].rank_server = 0;
			g_city[tmpi].rank_zone = 0;
			g_city[tmpi].rank_nation = 0;
		}

		g_rank_server[tmpi].city_index = tmpi;
		g_rank_server[tmpi].level = g_city[tmpi].level;
		g_rank_server[tmpi].place = g_city[tmpi].place;
		g_rank_server[tmpi].battlepower = g_city[tmpi].battlepower;
	}
	return 0;
}

int rank_sortfunc( const void* a, const void* b )
{
	ActorRank* pa = (ActorRank*)a;
	ActorRank* pb = (ActorRank*)b;
	if ( pa == NULL || pb == NULL )
		return -1;
	if ( pa->place != pb->place )
	{
		return (pb->place - pa->place);
	}
	else if ( pa->battlepower != pb->battlepower )
	{
		return (pb->battlepower - pa->battlepower);
	}
	else
	{
		return (pb->level - pa->level);
	}
}

int rank_server()
{
	return 0;
#ifdef WIN32
	DWORD b = timeGetTime();
#else
	struct timeval tpstart, tpend;
	float timeuse;
	gettimeofday( &tpstart, NULL );
#endif
	int nation = 0;
	int zone = 0;
	if ( g_city_maxindex <= 0 )
		return -1;

	// 区域排行榜
	for ( int tmpi = 1; tmpi < MAPZONE_MAXNUM; tmpi++ )
	{
		if ( g_zoneinfo[tmpi].open == 0 )
			continue;
		if ( g_rank_zone[tmpi] )
		{
			memset( &g_rank_zone[tmpi], 0, sizeof( ActorRank )*g_city_maxcount );
			g_rank_zone_count[tmpi] = 0;
		}
	}

	// 国家排行榜
	for ( int tmpi = 0; tmpi < 3; tmpi++ )
	{
		if ( g_rank_nation[tmpi] )
		{
			memset( &g_rank_nation[tmpi], 0, sizeof( ActorRank )*g_city_maxcount );
			g_rank_nation_count[tmpi] = 0;
		}
	}

	//调用qsort排序
	qsort( g_rank_server, g_city_maxindex, sizeof( ActorRank ), rank_sortfunc );
	for ( int tmpi = 0; tmpi < g_city_maxindex/*注意：使用索引位置，为了效率*/; tmpi++ )
	{
		int city_index = g_rank_server[tmpi].city_index;
		if ( city_index < 0 || city_index >= g_city_maxcount )
			continue;

		// 我的全服排名
		g_city[city_index].rank_server = tmpi + 1;

		// 区域排名信息
		zone = g_city[city_index].zone;
		if ( zone > 0 && zone < MAPZONE_MAXNUM && g_rank_zone[zone] )
		{
			if ( g_rank_zone_count[zone] >= 0 && g_rank_zone_count[zone] < g_city_maxcount )
			{
				g_rank_zone[zone][g_rank_zone_count[zone]].city_index = city_index;
				g_rank_zone[zone][g_rank_zone_count[zone]].level = g_city[city_index].level;
				g_rank_zone[zone][g_rank_zone_count[zone]].place = g_city[city_index].place;
				g_rank_zone[zone][g_rank_zone_count[zone]].battlepower = g_city[city_index].battlepower;
				g_rank_zone_count[zone] += 1;
				g_city[city_index].rank_zone = g_rank_zone_count[zone];
			}
		}

		// 国家排名信息
		nation = g_city[city_index].nation - 1;
		if ( nation >= 0 && nation < 3 && g_rank_nation[nation] )
		{
			if ( g_rank_nation_count[nation] >= 0 && g_rank_nation_count[nation] < g_city_maxcount )
			{
				g_rank_nation[nation][g_rank_nation_count[nation]].city_index = city_index;
				g_rank_nation[nation][g_rank_nation_count[nation]].level = g_city[city_index].level;
				g_rank_nation[nation][g_rank_nation_count[nation]].place = g_city[city_index].place;
				g_rank_nation[nation][g_rank_nation_count[nation]].battlepower = g_city[city_index].battlepower;
				g_rank_nation_count[nation] += 1;
				g_city[city_index].rank_nation = g_rank_nation_count[nation];
			}
		}
	}
#ifdef WIN32
	DWORD e = timeGetTime();
	printf_msg( "sec rank_actor:%dms\n", e - b );
#else
	gettimeofday( &tpend, NULL );
	timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec;
	timeuse /= 1000000;
	printf_msg( "sec rank_actor:%fs\n", timeuse );
	write_gamelog( "sec rank_actor:%fs", timeuse );
#endif
	return 0;
}

// 数据有变化就检查排序，及时刷新
void rank_sort_check( City *pCity )
{
	if ( !pCity )
		return;


	// 检查后一位
}

// 全服排行榜列表
int rank_server_sendlist( int actor_index, int page, int myrange )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( myrange == 1 )
	{ // 获取我身边的排名
		page = (pCity->rank_server - 1) / 10;
	}

	if ( page < 0 )
	{
		page = 0;
	}

	int beginoffset = page * 10;

	SLK_NetS_RankList pValue = { 0 };
	pValue.m_type = 1;
	pValue.m_myrank = pCity->rank_server;
	pValue.m_page = page;
	for ( int tmpi = beginoffset; tmpi < g_city_maxindex/*注意：使用索引位置，为了效率*/; tmpi++ )
	{
		if ( g_rank_server[tmpi].level <= 0 )
			continue;
		int city_index = g_rank_server[tmpi].city_index;
		if ( city_index < 0 || city_index >= g_city_maxcount )
			continue;
		pValue.m_list[pValue.m_count].m_rank = tmpi + 1;
		pValue.m_list[pValue.m_count].m_place = g_rank_server[tmpi].place;
		pValue.m_list[pValue.m_count].m_battlepower = g_rank_server[tmpi].battlepower;
		pValue.m_list[pValue.m_count].m_level = g_rank_server[tmpi].level;
		pValue.m_list[pValue.m_count].m_nation = g_city[city_index].nation;
		strncpy( pValue.m_list[pValue.m_count].m_name, g_city[city_index].name, NAME_SIZE );
		pValue.m_list[pValue.m_count].m_namelen = strlen( pValue.m_list[pValue.m_count].m_name );
		pValue.m_count += 1;
		if ( pValue.m_count >= 10 )
		{
			break;
		}
	}
	netsend_ranklist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 区域排行榜列表
int rank_zone_sendlist( int actor_index, int page, int myrange )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int zone = pCity->zone;
	if ( zone <= 0 || zone >= MAPZONE_MAXNUM )
		return -1;
	if ( !g_rank_zone[zone] )
		return -1;
	if ( myrange == 1 )
	{ // 获取我身边的排名
		page = (pCity->rank_zone - 1) / 10;
	}

	if ( page < 0 )
	{
		page = 0;
	}
	int beginoffset = page * 10;

	SLK_NetS_RankList pValue = { 0 };
	pValue.m_type = 2;
	pValue.m_myrank = pCity->rank_zone;
	pValue.m_page = page;
	for ( int tmpi = beginoffset; tmpi < g_city_maxindex/*注意：使用索引位置，为了效率*/; tmpi++ )
	{
		if ( g_rank_zone[zone][tmpi].level <= 0 )
			continue;
		int city_index = g_rank_zone[zone][tmpi].city_index;
		if ( city_index < 0 || city_index >= g_city_maxcount )
			continue;
		pValue.m_list[pValue.m_count].m_rank = tmpi + 1;
		pValue.m_list[pValue.m_count].m_place = g_rank_zone[zone][tmpi].place;
		pValue.m_list[pValue.m_count].m_battlepower = g_rank_zone[zone][tmpi].battlepower;
		pValue.m_list[pValue.m_count].m_level = g_rank_zone[zone][tmpi].level;
		pValue.m_list[pValue.m_count].m_nation = g_city[city_index].nation;
		strncpy( pValue.m_list[pValue.m_count].m_name, g_city[city_index].name, NAME_SIZE );
		pValue.m_list[pValue.m_count].m_namelen = strlen( pValue.m_list[pValue.m_count].m_name );
		pValue.m_count += 1;
		if ( pValue.m_count >= 10 )
		{
			break;
		}
	}
	netsend_ranklist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}


// 国家排行榜列表
int rank_nation_sendlist( int actor_index, int page, int myrange )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int nation = pCity->nation - 1;
	if ( nation < 0 || nation >= 3 )
		return -1;
	if ( !g_rank_nation[nation] )
		return -1;
	if ( myrange == 1 )
	{ // 获取我身边的排名
		page = (pCity->rank_nation - 1) / 10;
	}

	if ( page < 0 )
	{
		page = 0;
	}
	int beginoffset = page * 10;

	SLK_NetS_RankList pValue = { 0 };
	pValue.m_type = 3;
	pValue.m_myrank = pCity->rank_nation;
	pValue.m_page = page;
	for ( int tmpi = beginoffset; tmpi < g_city_maxindex/*注意：使用索引位置，为了效率*/; tmpi++ )
	{
		if ( g_rank_nation[nation][tmpi].level <= 0 )
			continue;
		int city_index = g_rank_zone[pCity->zone][tmpi].city_index;
		if ( city_index < 0 || city_index >= g_city_maxcount )
			continue;
		pValue.m_list[pValue.m_count].m_rank = tmpi + 1;
		pValue.m_list[pValue.m_count].m_place = g_rank_nation[nation][tmpi].place;
		pValue.m_list[pValue.m_count].m_battlepower = g_rank_nation[nation][tmpi].battlepower;
		pValue.m_list[pValue.m_count].m_level = g_rank_nation[nation][tmpi].level;
		pValue.m_list[pValue.m_count].m_nation = g_city[city_index].nation;
		strncpy( pValue.m_list[pValue.m_count].m_name, g_city[city_index].name, NAME_SIZE );
		pValue.m_list[pValue.m_count].m_namelen = strlen( pValue.m_list[pValue.m_count].m_name );
		pValue.m_count += 1;
		if ( pValue.m_count >= 10 )
		{
			break;
		}
	}
	netsend_ranklist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
