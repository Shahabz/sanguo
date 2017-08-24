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
#include "map.h"
#include "map_res.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern MapResInfo *g_resinfo;
extern int g_resinfo_maxnum;

MapRes *g_map_res = NULL;
int g_map_res_maxcount = 0;
extern int g_map_res_maxindex;

// 读档完毕的回调
int map_res_loadcb( int index )
{
	if ( index < 0 || index >= g_map_res_maxcount )
		return -1;

	// 添加到地图显示单元
	g_map_res[index].unit_index = mapunit_add( MAPUNIT_TYPE_RES, index );

	// 占地块信息添加到世界地图
	map_addobject( MAPUNIT_TYPE_RES, index, g_map_res[index].posx, g_map_res[index].posy );
	return 0;
}

int map_res_load()
{
	g_map_res_maxcount = 30000;
	g_map_res = (MapRes*)malloc( sizeof( MapRes ) * g_map_res_maxcount );
	memset( g_map_res, 0, sizeof( MapRes ) * g_map_res_maxcount );
	printf_msg( "MapRes  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_map_res_maxcount, (sizeof( MapRes )*g_map_res_maxcount) / 1024.0 / 1024.0, sizeof( MapRes ) / 1024.0 );
	map_res_load_auto( map_res_getptr, map_res_loadcb, "map_res" );
	return 0;
}

int map_res_save( FILE *fp )
{
	map_res_batch_save_auto( g_map_res, g_map_res_maxcount, "map_res", fp );
	return 0;
}

MapRes *map_res_getptr( int index )
{
	if ( index < 0 || index >= g_map_res_maxcount )
		return NULL;
	return &g_map_res[index];
}

MapResInfo *map_res_getconfig( int kind )
{
	if ( kind < 0 || kind >= g_resinfo_maxnum )
		return NULL;
	return &g_resinfo[kind];
}

// 显示单元属性
void map_res_makeunit( int index, SLK_NetS_AddMapUnit *pAttr )
{
	if ( index < 0 || index >= g_map_res_maxcount )
		return;
	MapResInfo *config = map_res_getconfig( g_map_res[index].kind );
	if ( !config )
		return;
	pAttr->m_posx = g_map_res[index].posx;
	pAttr->m_posy = g_map_res[index].posy;
}

// 位置
void map_res_getpos( int index, short *posx, short *posy )
{
	if ( index < 0 || index >= g_map_res_maxcount )
		return;
	*posx = g_map_res[index].posx;
	*posy = g_map_res[index].posy;
}
