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

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern HeroInfo *g_HeroInfo;
extern short g_HeroCount;


extern City * g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;

extern ItemKind *g_itemkind;
extern int g_itemkindnum;

Army *g_army = NULL;
int g_army_maxcount = 0;
int g_army_count = 0;
extern int g_army_maxindex;

// 读档完毕的回调
int army_loadcb( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;

	// 添加到地图显示单元
	g_army[army_index].unit_index = mapunit_add( MAPUNIT_TYPE_ARMY, army_index );

	return 0;
}

int army_load()
{
	g_army_maxcount = g_Config.max_citycount * 4;
	g_army = (Army*)malloc( sizeof( Army ) * g_army_maxcount );
	memset( g_army, 0, sizeof( Army ) * g_army_maxcount );
	printf_msg( "Army  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_army_maxcount, (sizeof( Army )*g_army_maxcount) / 1024.0 / 1024.0, sizeof( Army ) / 1024.0 );
	army_load_auto( army_getptr, army_loadcb, "map_army" );
	return 0;
}

int army_save( FILE *fp )
{
	army_batch_save_auto( g_army, g_army_maxcount, "map_army", fp );
	return 0;
}

Army *army_getptr( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return NULL;
	return &g_army[army_index];
}

// 显示单元属性
void army_makeunit( int army_index, SLK_NetS_AddMapUnit *pAttr )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	pAttr->m_posx = g_army[army_index].posx;
	pAttr->m_posy = g_army[army_index].posy;
}


// 部队位置
void army_getpos( int army_index, short *posx, short *posy )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	*posx = g_army[army_index].posx;
	*posy = g_army[army_index].posy;
}
