#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "db.h"
#include "global.h"
#include "actor.h"
#include "gameproc.h"
#include "item.h"
#include "system.h"
#include "map.h"
#include "server_netsend_auto.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "hero.h"
#include "equip.h"

extern Global global;
extern MYSQL *myData;
extern MYSQL *myGame;

extern Actor *g_actors;
extern int g_maxactornum;

extern ItemKind *g_itemkind;
extern int g_itemkind_maxnum;
extern i64 g_maxitemid;

extern int g_city_maxindex;
extern City *g_city;
extern int g_city_maxcount;

// 玩家背包和未上阵英雄的装备
Equip *actor_equip_getptr( int actor_index, int offset )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return NULL;
	if ( offset >= 0 && offset < MAX_ACTOR_EQUIPNUM )
	{ // 装备栏
		return &g_actors[actor_index].equip[offset];
	}
	else if ( offset >= EQUIP_BASEOFFSET )
	{ // 未上阵英雄身上的装备 例：1002 kind=1 equip=2 装备预留3位
		int herokind = offset / EQUIP_BASEOFFSET;
		int equip_offset = offset % EQUIP_BASEOFFSET;
		int hero_index = actor_hero_getindex( actor_index, herokind );
		if ( hero_index >= 0 && hero_index < HERO_ACTOR_MAX )
		{ // 只读物未上阵的
			return &g_actors[actor_index].hero[hero_index].equip[equip_offset];
		}
	}
	return NULL;
}

// 已经上阵的英雄装备
Equip *city_equip_getptr( int city_index, int offset )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	if ( offset < EQUIP_BASEOFFSET )
		return NULL;
	int herokind = offset / EQUIP_BASEOFFSET;
	int equip_offset = offset % EQUIP_BASEOFFSET;
	int hero_index = city_hero_getindex( city_index, herokind );
	if ( hero_index >= 0 && hero_index < HERO_CITY_MAX )
	{ // 只读物未上阵的
		return &g_city[city_index].hero[hero_index].equip[equip_offset];
	}
	return NULL;
}

int equip_load( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	actor_equip_load_auto( g_actors[actor_index].actorid, actor_index, actor_equip_getptr, "actor_equip" );
	return 0;
}

int equip_save( int actor_index, FILE *fp )
{
	ACTOR_CHECK_INDEX( actor_index );
	// 装备栏
	int max_equipnum = MAX_DEFAULT_EQUIPNUM + g_actors[actor_index].equipext;
	if ( max_equipnum > MAX_ACTOR_EQUIPNUM )
		max_equipnum = MAX_ACTOR_EQUIPNUM;
	actor_equip_batch_save_auto( g_actors[actor_index].equip, max_equipnum, "actor_equip", fp );
	return 0;
}
