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
#include "map_town.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern MapTownInfo *g_towninfo;
extern int g_towninfo_maxnum;

MapTown *g_map_town = NULL;
int g_map_town_maxcount = 0;
int g_map_town_count = 0;
extern int g_map_town_maxindex;

// 读档完毕的回调
int map_town_loadcb( int townid )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	
	// 添加到地图显示单元
	g_map_town[townid].unit_index = mapunit_add( MAPUNIT_TYPE_TOWN, townid );

	// 占地块信息添加到世界地图
	map_addobject( MAPUNIT_TYPE_CITY, townid, g_towninfo[townid].posx, g_towninfo[townid].posy );
	return 0;
}

int map_town_load()
{
	g_map_town_maxcount = g_towninfo_maxnum;
	g_map_town = (MapTown*)malloc( sizeof( MapTown ) * g_map_town_maxcount );
	memset( &g_map_town, 0, sizeof( MapTown ) * g_map_town_maxcount );
	for ( int townid = 1; townid < g_map_town_maxcount; townid++ )
	{
		g_map_town[townid].townid = townid;
		g_map_town[townid].nation = 0;
	}
	map_town_load_auto( map_town_getptr, map_town_loadcb, "map_town" );
	return 0;
}

MapTown *map_town_getptr( int townid )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return NULL;
	return &g_map_town[townid];
}

int map_town_save( FILE *fp )
{
	map_town_batch_save_auto( g_map_town, g_map_town_maxcount, "map_town", fp );
	return 0;
}

// 显示单元属性
void map_town_makeunit( int index, SLK_NetS_AddMapUnit *pAttr )
{

}

// 位置
void map_town_getpos( int index, short *posx, short *posy )
{

}
