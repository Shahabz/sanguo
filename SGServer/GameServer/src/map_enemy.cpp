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
#include "map_enemy.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern MapEnemyInfo *g_enemyinfo;
extern int g_enemyinfo_maxnum;

MapEnemy *g_map_enemy = NULL;
int g_map_enemy_maxcount = 0;
extern int g_map_enemy_maxindex;

// 读档完毕的回调
int map_enemy_loadcb( int index )
{
	if ( index < 0 || index >= g_map_enemy_maxcount )
		return -1;

	// 添加到地图显示单元
	g_map_enemy[index].unit_index = mapunit_add( MAPUNIT_TYPE_ENEMY, index );

	// 占地块信息添加到世界地图
	map_addobject( MAPUNIT_TYPE_ENEMY, index, g_map_enemy[index].posx, g_map_enemy[index].posy );
	return 0;
}

int map_enemy_load()
{
	g_map_enemy_maxcount = 30000;
	g_map_enemy = (MapEnemy*)malloc( sizeof( MapEnemy ) * g_map_enemy_maxcount );
	memset( g_map_enemy, 0, sizeof( MapEnemy ) * g_map_enemy_maxcount );
	printf_msg( "MapEnemy  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_map_enemy_maxcount, (sizeof( MapEnemy )*g_map_enemy_maxcount) / 1024.0 / 1024.0, sizeof( MapEnemy ) / 1024.0 );
	map_enemy_load_auto( map_enemy_getptr, map_enemy_loadcb, "map_enemy" );
	return 0;
}

int map_enemy_save( FILE *fp )
{
	map_enemy_batch_save_auto( g_map_enemy, g_map_enemy_maxcount, "map_enemy", fp );
	return 0;
}

MapEnemy *map_enemy_getptr( int index )
{
	if ( index < 0 || index >= g_map_enemy_maxcount )
		return NULL;
	return &g_map_enemy[index];
}

MapEnemyInfo *map_enemy_getconfig( int kind )
{
	if ( kind < 0 || kind >= g_enemyinfo_maxnum )
		return NULL;
	return &g_enemyinfo[kind];
}

// 显示单元属性
void map_enemy_makeunit( int index, SLK_NetS_AddMapUnit *pAttr )
{
	if ( index < 0 || index >= g_map_enemy_maxcount )
		return;
	MapEnemyInfo *config = map_enemy_getconfig( g_map_enemy[index].kind );
	if ( !config )
		return;
	pAttr->m_posx = g_map_enemy[index].posx;
	pAttr->m_posy = g_map_enemy[index].posy;
}

// 位置
void map_enemy_getpos( int index, short *posx, short *posy )
{
	if ( index < 0 || index >= g_map_enemy_maxcount )
		return;
	*posx = g_map_enemy[index].posx;
	*posy = g_map_enemy[index].posy;
}
