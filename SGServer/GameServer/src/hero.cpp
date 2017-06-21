#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "global.h"
#include "actor.h"
#include "city.h"
#include "building.h"
#include "hero.h"
#include "mapunit.h"
#include "map.h"
#include "server_netsend_auto.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;
extern Map g_map;

extern Actor *g_actors;
extern int g_maxactornum;

extern int g_city_maxindex;
extern City *g_city;
extern int g_city_maxcount;

int actor_hero_getindex( int actor_index, int herokind )
{
	ACTOR_CHECK_INDEX( actor_index );
	int index = -1;
	for ( int tmpi = 0; tmpi < HERO_ACTOR_MAX; tmpi++ )
	{
		if ( g_actors[actor_index].hero[tmpi].kind == herokind )
		{
			index = tmpi;
			break;
		}
	}
	return index;
}

Hero* actor_hero_getptr( int actor_index, int offset )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return NULL;
	if ( offset < 0 || offset >= HERO_ACTOR_MAX )
		return NULL;
	return &g_actors[actor_index].hero[offset];
}


int city_hero_getindex( int city_index, int herokind )
{
	CITY_CHECK_INDEX( city_index );
	int index = -1;
	for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
	{
		if ( g_city[city_index].hero[tmpi].kind == herokind )
		{
			index = tmpi;
			break;
		}
	}
	return index;
}

Hero* city_hero_getptr( int city_index, int offset )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	if ( offset < HERO_BASEOFFSET )
		return NULL;
	short herokind = offset - HERO_BASEOFFSET;
	int hero_index = city_hero_getindex( city_index, herokind );
	if ( hero_index < 0 || hero_index >= HERO_CITY_MAX )
		return NULL;
	return &g_city[city_index].hero[hero_index];
}
