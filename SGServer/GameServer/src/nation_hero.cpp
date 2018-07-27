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
#include "equip.h"
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
#include "map_town.h"
#include "mail.h"
#include "city.h"
#include "chat.h"
#include "army_group.h"
#include "rank.h"
#include "world_quest.h"
#include "nation.h"
#include "nation_hero.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern City * g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern Army *g_army;
extern int g_army_maxcount;

extern int g_kingwar_activity_beginstamp;
extern int g_kingwar_activity_endstamp;

extern MonsterInfo *g_monster;
extern int g_monster_maxnum;

extern HeroInfo *g_heroinfo;
extern int g_heroinfo_maxnum;

// 国家信息
extern Nation g_nation[NATION_MAX];

extern NationHeroInfo *g_nation_heroinfo;
extern int g_nation_heroinfo_maxnum;

NationHero *g_nation_hero = NULL;
int g_nation_hero_maxcount = 0;

CityNationHero *city_nation_hero_getptr( int city_index, int offset )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	if ( offset < 0 || offset >= NATIONHERO_MAX )
		return NULL;
	return &g_city[city_index].nation_hero[offset];
}

// 读档完毕的回调
int nation_hero_loadcb( int herokind )
{
	if ( herokind <= 0 || herokind >= g_nation_hero_maxcount )
		return -1;

	if ( g_nation_hero[herokind].actorid <= 0 && g_nation_hero[herokind].posx >= 0 && g_nation_hero[herokind].posy > 0 )
	{
		// 添加到地图显示单元
		g_nation_hero[herokind].unit_index = mapunit_add( MAPUNIT_TYPE_NATIONHERO, herokind );

		// 占地块信息添加到世界地图
		map_addobject( MAPUNIT_TYPE_NATIONHERO, herokind, g_nation_hero[herokind].posx, g_nation_hero[herokind].posy );

		nation_hero_attrcalc( herokind );
	}
	else if ( g_nation_hero[herokind].actorid > 0 )
	{ // 有主人
		int city_index = city_getindex_withactorid( g_nation_hero[herokind].actorid );
		g_nation_hero[herokind].city_index = city_index;
	}
	return 0;
}

int nation_hero_load()
{
	g_nation_hero_maxcount = g_heroinfo_maxnum;
	g_nation_hero = (NationHero*)malloc( sizeof( NationHero ) * g_nation_hero_maxcount );
	memset( g_nation_hero, 0, sizeof( NationHero ) * g_nation_hero_maxcount );
	for ( int tmpi = 0; tmpi < g_nation_hero_maxcount; tmpi++ )
	{
		g_nation_hero[tmpi].kind = g_nation_heroinfo[tmpi].herokind;
		g_nation_hero[tmpi].unit_index = -1;
		g_nation_hero[tmpi].city_index = -1;
		g_nation_hero[tmpi].posx = -1;
		g_nation_hero[tmpi].posy = -1;
		if ( g_nation_hero[tmpi].kind > 0 )
		{
			g_nation_hero[tmpi].level = 100;
			nation_hero_attrcalc( g_nation_hero[tmpi].kind );
		}
	}

	printf_msg( "NationHero  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_nation_hero_maxcount, (sizeof( NationHero )*g_nation_hero_maxcount) / 1024.0 / 1024.0, sizeof( NationHero ) / 1024.0 );
	nation_hero_load_auto( nation_hero_getptr, nation_hero_loadcb, "nation_hero" );
	return 0;
}

int nation_hero_save( FILE *fp )
{
	nation_hero_batch_save_auto( g_nation_hero, g_nation_hero_maxcount, "nation_hero", fp );
	return 0;
}

inline NationHero *nation_hero_getptr( int kind )
{
	if ( kind <= 0 || kind >= g_nation_hero_maxcount )
		return NULL;
	return &g_nation_hero[kind];
}

NationHeroInfo *nation_hero_getconfig( int kind )
{
	if ( kind <= 0 || kind >= g_nation_heroinfo_maxnum )
		return NULL;
	return &g_nation_heroinfo[kind];
}

// 显示单元属性
void nation_hero_makeunit( int kind, SLK_NetS_AddMapUnit *pAttr )
{
	if ( kind <= 0 || kind >= g_nation_hero_maxcount )
		return;
	if ( g_nation_hero[kind].actorid > 0 )
		return;
	pAttr->m_posx = g_nation_hero[kind].posx;
	pAttr->m_posy = g_nation_hero[kind].posy;

	pAttr->m_short_value[0] = kind;
	pAttr->m_short_value[1] = g_nation_hero[kind].level;
	pAttr->m_short_value_count = 2;
}

// 位置
void nation_hero_getpos( int kind, short *posx, short *posy )
{
	if ( kind < 0 || kind >= g_nation_hero_maxcount )
		return;
	*posx = g_nation_hero[kind].posx;
	*posy = g_nation_hero[kind].posy;
}

// 获取名将奖励奖励
int nation_hero_sendaward( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( kind <= 0 || kind >= g_nation_heroinfo_maxnum )
		return -1;
	awardgroup_sendinfo( actor_index, g_nation_heroinfo[kind].awardgroup, 4, kind, 4 );
	return 0;
}


// 国家升级，检查是否需要出现名将
void nation_hero_check( char nation )
{
	Nation *pNation = nation_getptr( nation );
	if ( !pNation )
		return;
	for ( int tmpi = 0; tmpi < g_nation_heroinfo_maxnum; tmpi++ )
	{
		short kind = g_nation_heroinfo[tmpi].herokind;
		if ( kind <= 0 || kind >= g_nation_hero_maxcount )
			continue;
		if ( nation == g_nation_heroinfo[tmpi].nation && pNation->level >= g_nation_heroinfo[tmpi].nationlevel )
		{
			nation_hero_showmap( kind );
			g_nation_hero[kind].createtime = (int)time( NULL );
		}
	}
}

// 名将出现在地图
void nation_hero_showmap( int kind )
{
	if ( kind <= 0 || kind >= g_nation_hero_maxcount )
		return;
	if ( g_nation_hero[kind].actorid > 0 )
		return;
	if ( g_nation_hero[kind].unit_index >= 0 && g_nation_hero[kind].posx >= 0 && g_nation_hero[kind].posy >= 0 )
		return;

	// 随机坐标
	int step = 0;
	while ( true )
	{
		if ( map_getrandpos_withtype( 0, &g_nation_hero[kind].posx, &g_nation_hero[kind].posy ) >= 0 )
			break;
		step += 1;
		if ( step >= 10 )
			return;
	}

	// 添加到地图显示单元
	g_nation_hero[kind].unit_index = mapunit_add( MAPUNIT_TYPE_NATIONHERO, kind );

	// 占地块信息添加到世界地图
	map_addobject( MAPUNIT_TYPE_NATIONHERO, kind, g_nation_hero[kind].posx, g_nation_hero[kind].posy );

	// 公告
	system_talkjson_world( 6023, NULL, NULL, NULL, NULL, NULL, NULL, 1 );
}

// 购买坐标
int nation_hero_buypos( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind >= g_nation_hero_maxcount )
		return -1;
	if ( g_nation_hero[kind].actorid > 0 )
	{ // 该名将已被人抓捕，无法寻访
		actor_notify_pop( actor_index, 1896 );
		nation_hero_sendlist( actor_index );
		return -1;
	}
	if ( g_nation_hero[kind].posx < 0 || g_nation_hero[kind].posy < 0 )
		return -1;

	int offset = g_nation_heroinfo[kind].offset;
	if ( offset < 0 || offset >= NATIONHERO_MAX )
		return -1;

	if ( pCity->nation_hero[offset].buypos > 0 )
		return -1;

	int costtoken = 0;
	if ( g_nation_heroinfo[kind].nation == pCity->nation )
	{
		costtoken = g_nation_heroinfo[kind].visit_token;
	}
	else
	{
		costtoken = g_nation_heroinfo[kind].other_visit_token;
	}

	if ( actor_change_token( actor_index, -costtoken, PATH_NATIONHERO, 0 ) < 0 )
		return -1;

	pCity->nation_hero[offset].kind = kind;
	pCity->nation_hero[offset].buypos = 3;
	nation_hero_sendlist( actor_index );
	return 0;
}

// 坐标变化
int nation_hero_changepos( int kind )
{
	if ( kind <= 0 || kind >= g_nation_hero_maxcount )
		return -1;

	int offset = g_nation_heroinfo[kind].offset;
	if ( offset < 0 || offset >= NATIONHERO_MAX )
		return -1;
	// 随机坐标
	short lastposx = g_nation_hero[kind].posx;
	short lastposy = g_nation_hero[kind].posy;
	int step = 0;
	while ( true )
	{
		if ( map_getrandpos_withtype( 0, &g_nation_hero[kind].posx, &g_nation_hero[kind].posy ) >= 0 )
			break;
		step += 1;
		if ( step >= 10 )
		{
			g_nation_hero[kind].posx = lastposx;
			g_nation_hero[kind].posy = lastposy;
			break;
		}
	}

	// 刷新区域格
	map_delobject( MAPUNIT_TYPE_NATIONHERO, kind, lastposx, lastposy );
	if ( g_nation_hero[kind].unit_index < 0 )
	{
		g_nation_hero[kind].unit_index = mapunit_add( MAPUNIT_TYPE_NATIONHERO, kind );
	}
	map_addobject( MAPUNIT_TYPE_NATIONHERO, kind, g_nation_hero[kind].posx, g_nation_hero[kind].posy );
	mapunit_area_change( g_nation_hero[kind].unit_index, g_nation_hero[kind].posx, g_nation_hero[kind].posy, 1 );

	// 更新其他购买坐标的玩家
	int city_index = -1;
	for ( int tmpi = 0; tmpi < g_city_maxindex/*注意：使用索引位置，为了效率*/; tmpi++ )
	{
		if ( g_city[tmpi].actorid < MINACTORID )
			continue;
		if ( g_city[tmpi].nation_hero[offset].buypos > 0 )
		{
			g_city[tmpi].nation_hero[offset].buypos -= 1;
			if ( g_city[tmpi].actor_index >= 0 )
			{// 界面更新
				ui_update( 0, SENDTYPE_WORLD, UI_UPDATE_NATIONHEROPOS );
			}
		}
	}
	return 0;
}

// 检查数量
int nation_hero_checklimit( City *pCity )
{
	if ( !pCity )
		return 0;
	// 是否超过4个名将
	int count = 0;
	for ( int tmpi = 0; tmpi < NATIONHERO_MAX; tmpi++ )
	{
		if ( pCity->nation_hero[tmpi].state > 0 )
			count += 1;
	}
	if ( count >= 4 )
	{
		return 0;
	}
	return 1;
}

// 名将抓捕成功
int nation_hero_catch( City *pCity, int kind )
{
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind >= g_nation_hero_maxcount )
		return -1;
	int offset = g_nation_heroinfo[kind].offset;
	if ( offset < 0 || offset >= NATIONHERO_MAX )
		return -1;
	if ( g_nation_hero[kind].actorid > 0 )
		return -1;
	//if ( pCity->nation_hero[offset].state > 0 )
	//	return -1; // 已经获得了

	// 是否超过4个名将
	if ( nation_hero_checklimit( pCity ) == 0 )
	{
		return -1;
	}

	if ( pCity->actor_index >= 0 && pCity->actor_index < g_maxactornum )
	{
		hero_gethero( pCity->actor_index, kind, PATH_NATIONHERO );
		nation_hero_attrupdate( hero_getptr( pCity->actor_index, kind ) );
	}
	else
	{
		gift( pCity->actorid, AWARDKIND_HEROBASE + kind, 1, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, PATH_NATIONHERO );
	}
	
	pCity->nation_hero[offset].kind = kind;
	pCity->nation_hero[offset].state = NATIONHERO_STATE_CATCH;
	pCity->nation_hero[offset].loyal = 100;
	pCity->nation_hero[offset].forever = 0;
	pCity->nation_hero[offset].buypos = 0;
	city_nation_hero_save_auto( pCity->actorid, offset, &pCity->nation_hero[offset], "city_nation_hero", NULL );

	mapunit_del( MAPUNIT_TYPE_NATIONHERO, kind, g_nation_hero[kind].unit_index );
	map_delobject( MAPUNIT_TYPE_NATIONHERO, kind, g_nation_hero[kind].posx, g_nation_hero[kind].posy );
	g_nation_hero[kind].unit_index = -1;
	g_nation_hero[kind].posx = -1;
	g_nation_hero[kind].posy = -1;
	g_nation_hero[kind].actorid = pCity->actorid;
	g_nation_hero[kind].city_index = pCity->index;
	g_nation_hero[kind].runsec = 43200;
	nation_hero_cityinfo( pCity->actor_index, kind );
	return 0;
}

// 玩家获得
int nation_hero_get( City *pCity, int kind )
{
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind >= g_nation_hero_maxcount )
		return -1;
	int offset = g_nation_heroinfo[kind].offset;
	if ( offset < 0 || offset >= NATIONHERO_MAX )
		return -1;

	// 获得
	if ( pCity->actor_index >= 0 && pCity->actor_index < g_maxactornum )
	{
		hero_gethero( pCity->actor_index, kind, PATH_NATIONHERO );
		nation_hero_attrupdate( hero_getptr( pCity->actor_index, kind ) );
	}
	else
	{
		gift( pCity->actorid, AWARDKIND_HEROBASE + kind, 1, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, PATH_NATIONHERO );
	}

	pCity->nation_hero[offset].kind = kind;
	pCity->nation_hero[offset].state = NATIONHERO_STATE_CATCH;
	pCity->nation_hero[offset].loyal = 100;
	pCity->nation_hero[offset].forever = 0;
	pCity->nation_hero[offset].buypos = 0;
	city_nation_hero_save_auto( pCity->actorid, offset, &pCity->nation_hero[offset], "city_nation_hero", NULL );

	g_nation_hero[kind].unit_index = -1;
	g_nation_hero[kind].posx = -1;
	g_nation_hero[kind].posy = -1;
	g_nation_hero[kind].actorid = pCity->actorid;
	g_nation_hero[kind].city_index = pCity->index;
	g_nation_hero[kind].runsec = 43200;
	nation_hero_cityinfo( pCity->actor_index, kind );
	return 0;
}

// 离开玩家
int nation_hero_leave( City *pCity, int kind )
{
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind >= g_nation_hero_maxcount )
		return -1;
	int offset = g_nation_heroinfo[kind].offset;
	if ( offset < 0 || offset >= NATIONHERO_MAX )
		return -1;

	pCity->nation_hero[offset].state = 0;
	pCity->nation_hero[offset].loyal = 0;
	pCity->nation_hero[offset].buypos = 0;
	city_nation_hero_save_auto( pCity->actorid, offset, &pCity->nation_hero[offset], "city_nation_hero", NULL );

	Hero *pHero = city_hero_getptr_withkind( pCity->index, kind );
	if ( pHero )
	{ // 上阵状态，战斗中需要删除部队，然后下阵，并且把装备脱下放到背包
		if ( pHero->state == HERO_STATE_FIGHT || pHero->state == HERO_STATE_GATHER )
		{
			for ( int tmpi = 0; tmpi < CITY_BATTLEQUEUE_MAX; tmpi++ )
			{
				int army_index = pCity->battle_armyindex[tmpi];
				if ( army_index >= 0 && army_index < g_army_maxcount )
				{
					char has = 0;
					for ( int i = 0; i < ARMY_HERO_MAX; i++ )
					{
						if ( g_army[army_index].herokind[i] == kind )
						{
							hero_changestate( pCity->index, g_army[army_index].herokind[tmpi], HERO_STATE_NORMAL );
							g_army[army_index].herokind[i] = 0;
							g_army[army_index].totals -= pHero->soldiers;
							if ( g_army[army_index].totals < 0 )
								g_army[army_index].totals = 0;
							has = 1;
							break;
						}
					}

					if ( has == 1 )
					{
						int leftcount = 0;
						for ( int i = 0; i < ARMY_HERO_MAX; i++ )
						{
							if ( g_army[army_index].herokind[i] > 0 )
							{
								leftcount += 1;
							}
						}

						if ( leftcount == 0 )
						{
							army_delete( army_index );
						}
						break;
					}
				}
			}
		}
		else if ( pHero->state == HERO_STATE_KINGWAR )
		{ // 血战中，不抢夺
			return -1;
		}

		if ( pCity->actor_index >= 0 && pCity->actor_index < g_maxactornum )
		{ // 玩家在线
			hero_down( pCity->actor_index, kind, 1 );
		}
		else
		{ // 不在线
			hero_down_offline( pCity->index, pCity->actorid, kind, 1 );
		}
	}
	else
	{ // 下阵状态，需要把装备脱下放到背包
		if ( pCity->actor_index >= 0 && pCity->actor_index < g_maxactornum )
		{ // 玩家在线
			equip_down_all( pCity->actor_index, kind );
		}
		else
		{ // 不在线
			equip_down_all_offline( pCity->index, pCity->actorid, kind );
		}
	}

	// 通知更新
	nation_hero_cityinfo( pCity->actor_index, kind );

	// 邮件通知
	char v1[32] = { 0 };
	sprintf( v1, "%s%d", TAG_HERO, kind );
	mail_system( pCity->actor_index, pCity->actorid, 5046, 5541, v1, NULL, NULL, "", 0 );
	return 0;
}

// 逃跑
int nation_hero_run( City *pCity, int kind )
{
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind >= g_nation_hero_maxcount )
		return -1;
	int offset = g_nation_heroinfo[kind].offset;
	if ( offset < 0 || offset >= NATIONHERO_MAX )
		return -1;

	// 离开玩家
	nation_hero_leave( pCity, kind );

	if ( pCity->nation_hero[offset].forever == 0 )
	{ // 无双
		if ( g_nation_hero[kind].actorid == pCity->actorid )
		{
			g_nation_hero[kind].actorid = 0;
			g_nation_hero[kind].city_index = -1;
			nation_hero_showmap( kind );
		}
	}
	return 0;
}

// 每秒逻辑
void nation_hero_logic()
{
	for ( int kind = 1; kind < g_nation_hero_maxcount; kind++ )
	{
		if ( g_nation_hero[kind].runsec <= 0 )
			continue;
		g_nation_hero[kind].runsec -= 1;
		if ( g_nation_hero[kind].runsec == 0 )
		{ // 逃跑
			City *pCity = city_indexptr( g_nation_hero[kind].city_index );
			if ( !pCity )
				continue;
			nation_hero_run( pCity, kind );
		}
	}
}

// 招募
int nation_hero_call( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind >= g_nation_hero_maxcount )
		return -1;
	int offset = g_nation_heroinfo[kind].offset;
	if ( offset < 0 || offset >= NATIONHERO_MAX )
		return -1;
	if ( pCity->nation_hero[offset].state != NATIONHERO_STATE_CATCH )
		return -1;

	if ( pCity->silver < g_nation_heroinfo[kind].call_silver )
		return -1;
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		if ( g_nation_heroinfo[kind].call_itemkind[tmpi] <= 0 )
			continue;;
		if ( item_getitemnum( actor_index, g_nation_heroinfo[kind].call_itemkind[tmpi] ) < g_nation_heroinfo[kind].call_itemnum )
			return -1;
	}

	Hero *pHero = hero_getptr( actor_index, kind );
	hero_attr_calc( pCity, pHero );
	nation_hero_attrupdate( pHero );

	pCity->nation_hero[offset].state = NATIONHERO_STATE_EMPLOY;
	pCity->nation_hero[offset].loyal = 100;
	g_nation_hero[kind].runsec = 0;
	city_nation_hero_save_auto( pCity->actorid, offset, &pCity->nation_hero[offset], "city_nation_hero", NULL );
	nation_hero_cityinfo( actor_index, kind );

	city_changesilver( pCity->index, -g_nation_heroinfo[kind].call_silver, PATH_NATIONHERO );
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		if ( g_nation_heroinfo[kind].call_itemkind[tmpi] <= 0 )
			continue;
		item_lost( actor_index, g_nation_heroinfo[kind].call_itemkind[tmpi], g_nation_heroinfo[kind].call_itemnum, PATH_NATIONHERO );			
	}

	return 0;
}

// 添忠诚度
int nation_hero_addloyal( int actor_index, int kind, int itemkind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind >= g_nation_hero_maxcount )
		return -1;
	int offset = g_nation_heroinfo[kind].offset;
	if ( offset < 0 || offset >= NATIONHERO_MAX )
		return -1;
	if ( pCity->nation_hero[offset].state != NATIONHERO_STATE_EMPLOY )
		return -1;
	if ( pCity->nation_hero[offset].loyal >= 100 )
		return -1;

	char itemoffset = -1;
	for ( itemoffset = 0; itemoffset < 4; itemoffset++ )
	{
		if ( g_nation_heroinfo[kind].call_itemkind[itemoffset] == itemkind )
		{
			break;
		}
	}
	if ( itemoffset < 0 || itemoffset >= 4 )
		return -1;
	if ( item_getitemnum( actor_index, g_nation_heroinfo[kind].call_itemkind[itemoffset] ) < g_nation_heroinfo[kind].loyal_itemnum )
		return -1;

	pCity->nation_hero[offset].loyal += 10;
	if ( pCity->nation_hero[offset].loyal > 100 )
		pCity->nation_hero[offset].loyal = 100;
	nation_hero_cityinfo( actor_index, kind );

	item_lost( actor_index, g_nation_heroinfo[kind].call_itemkind[itemoffset], g_nation_heroinfo[kind].loyal_itemnum, PATH_NATIONHERO );
	return 0;
}

// 定时减少忠诚
void nation_hero_subloyal()
{
	for ( int tmpi = 0; tmpi < g_city_maxindex/*注意：使用索引位置，为了效率*/; tmpi++ )
	{
		if ( g_city[tmpi].actorid < MINACTORID )
			continue;
		for ( int offset = 0; offset < NATIONHERO_MAX; offset++ )
		{
			short kind = g_city[tmpi].nation_hero[offset].kind;
			if ( kind <= 0 || kind >= g_nation_heroinfo_maxnum )
				continue;
			if ( g_city[tmpi].nation_hero[offset].state != NATIONHERO_STATE_EMPLOY )
				continue;
			g_city[tmpi].nation_hero[offset].loyal -= 2;
			if ( g_city[tmpi].nation_hero[offset].loyal < 0 )
				g_city[tmpi].nation_hero[offset].loyal = 0;
		}
	}
}

// 掠夺
int nation_hero_rob( City *pAttackCity, City *pCity, char *json )
{	
	int nowstamp = (int)time( NULL );
	if ( !pCity || nowstamp >= g_kingwar_activity_beginstamp && nowstamp <= g_kingwar_activity_endstamp )
	{
		return -1;
	}

	char szTmp[32] = { 0 };
	char first = 0;
	// 减忠诚度
	for ( int offset = 0; offset < NATIONHERO_MAX; offset++ )
	{
		short kind = pCity->nation_hero[offset].kind;
		if ( kind <= 0 || kind >= g_nation_heroinfo_maxnum )
			continue;
		if ( pCity->nation_hero[offset].state != NATIONHERO_STATE_EMPLOY )
			continue;

		int v = 0;
		if ( g_nation_heroinfo[kind].nation == pCity->nation )
			v = 5;
		else
			v = 10;

		if ( first == 0 )
		{
			sprintf( szTmp, "{\"k\":%d,\"n\":%d}", kind, -v );
			first = 1;
		}
		else
		{
			sprintf( szTmp, ",{\"k\":%d,\"n\":%d}", kind, -v );
		}
		strcat( json, szTmp );

		pCity->nation_hero[offset].loyal -= v;
		if ( pCity->nation_hero[offset].loyal < 0 )
		{
			pCity->nation_hero[offset].loyal = 0;
		}
		if ( pCity->actor_index >= 0 )
		{
			hero_sendinfo( pCity->actor_index, hero_getptr( pCity->actor_index, pCity->nation_hero[offset].kind ) );
		}
	}

	// 抢夺
	if ( nation_hero_checklimit( pAttackCity ) == 0 )
		return -1;
	for ( int offset = 0; offset < NATIONHERO_MAX; offset++ )
	{
		short kind = pCity->nation_hero[offset].kind;
		if ( kind <= 0 || kind >= g_nation_heroinfo_maxnum )
			continue;
		if ( pCity->nation_hero[offset].state != NATIONHERO_STATE_EMPLOY )
			continue;

		NationHeroInfo *config = nation_hero_getconfig( kind );
		if ( !config )
			continue;

		// 抢夺几率=（1-忠诚/100）* 系数
		float v = 0.0f;
		if ( config->nation == pAttackCity->nation )
		{
			v = config->rob_odds;
		}
		else
		{
			v = config->other_rob_odds;
		}

		int odds = (int)(((1.0f - (float)pCity->nation_hero[offset].loyal / 100.0f) * v) * 100);
		int rm = rand() % 100;
		if ( rm <= odds )
		{
			// 离开玩家
			nation_hero_leave( pCity, kind );

			// 玩家获得
			nation_hero_get( pAttackCity, kind );
			break;
		}
	}
	return 0;
}

// 重算属性
void nation_hero_attrcalc( int kind )
{
	if ( kind <= 0 || kind >= g_nation_hero_maxcount )
		return;
	HeroInfoConfig *config = hero_getconfig( kind, 5 );
	if ( !config )
		return;
	Hero pHero = { 0 };
	pHero.kind = kind;
	pHero.level = g_nation_hero[kind].level;
	pHero.color = 5;
	pHero.attack_wash = config->attack_wash;
	pHero.defense_wash = config->defense_wash;
	pHero.troops_wash = config->troops_wash;

	// 基础攻击
	int base_attack = ATTACK( pHero.level, config->attack, (config->attack_base + pHero.attack_wash) );
	int base_defense = DEFENSE( pHero.level, config->defense, (config->defense_base + pHero.defense_wash) );
	int base_troops = TROOPS( pHero.level, config->troops, (config->troops_base + pHero.troops_wash) );

	pHero.exp = 0;
	pHero.soldiers = 0;
	pHero.attack = base_attack;
	pHero.defense = base_defense;
	pHero.troops = base_troops;
	pHero.attack_increase = 0;
	pHero.defense_increase = 0;
	nation_hero_attrupdate( &pHero );
}

// 更新属性
void nation_hero_attrupdate( Hero *pHero )
{
	if ( !pHero )
		return;
	if ( pHero->kind <= 0 || pHero->kind >= g_nation_hero_maxcount )
		return;
	if ( g_nation_hero[pHero->kind].actorid != pHero->actorid )
		return;
	int kind = pHero->kind;
	g_nation_hero[kind].level = pHero->level;
	g_nation_hero[kind].color = pHero->color;
	g_nation_hero[kind].exp = pHero->exp;
	g_nation_hero[kind].soldiers = pHero->soldiers;
	g_nation_hero[kind].attack_wash = pHero->attack_wash;
	g_nation_hero[kind].defense_wash = pHero->defense_wash;
	g_nation_hero[kind].troops_wash = pHero->troops_wash;
	g_nation_hero[kind].attack = pHero->attack;
	g_nation_hero[kind].defense = pHero->defense;
	g_nation_hero[kind].troops = pHero->troops;
	g_nation_hero[kind].attack_increase = pHero->attack_increase;
	g_nation_hero[kind].defense_increase = pHero->defense_increase;
}

// 国家名将列表
int nation_hero_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	SLK_NetS_NationHeroList pValue = { 0 };
	pValue.m_nationlevel[0] = g_nation[1].level;
	pValue.m_nationlevel[1] = g_nation[2].level;
	pValue.m_nationlevel[2] = g_nation[3].level;
	for ( int tmpi = 0; tmpi < g_nation_heroinfo_maxnum; tmpi++ )
	{
		short kind = g_nation_heroinfo[tmpi].herokind;
		if ( kind <= 0 )
			continue;
		pValue.m_list[pValue.m_count].m_kind = kind;

		int offset = g_nation_heroinfo[tmpi].offset;
		if ( offset < 0 || offset >= NATIONHERO_MAX )
			continue;
		pValue.m_list[pValue.m_count].m_forever = pCity->nation_hero[offset].forever;
		if ( pCity->nation_hero[offset].forever == 0 )
		{ // 没有女将获得的非无双名将
			if ( g_nation_hero[kind].actorid > 0 )
			{ // 已经有主人
				pValue.m_list[pValue.m_count].m_actorid = g_nation_hero[kind].actorid;
				int city_index = g_nation_hero[kind].city_index;
				if ( city_index >= 0 && city_index < g_city_maxcount )
				{
					pValue.m_list[pValue.m_count].m_level = g_nation_hero[kind].level;
					strncpy( pValue.m_list[pValue.m_count].m_name, city_getname( g_nation_hero[kind].city_index ), NAME_SIZE );
					pValue.m_list[pValue.m_count].m_namelen = strlen( pValue.m_list[pValue.m_count].m_name );
					pValue.m_list[pValue.m_count].m_nation = g_city[city_index].nation;
				}
			}
			else
			{ // 无主人
				pValue.m_list[pValue.m_count].m_level = g_nation_hero[kind].level;
				pValue.m_list[pValue.m_count].m_buypos = pCity->nation_hero[offset].buypos;
				if ( pCity->nation_hero[offset].buypos > 0 )
				{
					pValue.m_list[pValue.m_count].m_posx = g_nation_hero[kind].posx;
					pValue.m_list[pValue.m_count].m_posy = g_nation_hero[kind].posy;
				}
			}
		}
		pValue.m_count += 1;
	}
	netsend_nationherolist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 名城玩家缓存信息
int nation_hero_cityinfo( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind >= g_nation_hero_maxcount )
		return -1;
	int offset = g_nation_heroinfo[kind].offset;
	if ( offset < 0 || offset >= NATIONHERO_MAX )
		return -1;

	SLK_NetS_CityNationHero pValue = { 0 };
	pValue.m_kind = kind;
	pValue.m_state = pCity->nation_hero[offset].state;
	pValue.m_forever = pCity->nation_hero[offset].forever;
	pValue.m_loyal = pCity->nation_hero[offset].loyal;
	pValue.m_runstamp = (int)time( NULL ) + g_nation_hero[kind].runsec;
	netsend_citynationhero_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 名城玩家缓存信息列表
int nation_hero_citylist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_CityNationHeroList pValue = { 0 };
	for ( int tmpi = 0; tmpi < NATIONHERO_MAX; tmpi++ )
	{
		int kind = pCity->nation_hero[tmpi].kind;
		if ( kind <= 0 || kind >= g_nation_hero_maxcount )
			continue;
		if ( pCity->nation_hero[tmpi].state <= 0 )
			continue;
		pValue.m_list[pValue.m_count].m_kind = g_nation_hero[kind].kind;
		pValue.m_list[pValue.m_count].m_state = pCity->nation_hero[tmpi].state;
		pValue.m_list[pValue.m_count].m_forever = pCity->nation_hero[tmpi].forever;
		pValue.m_list[pValue.m_count].m_loyal = pCity->nation_hero[tmpi].loyal;
		pValue.m_list[pValue.m_count].m_runstamp = (int)time( NULL ) + g_nation_hero[kind].runsec;
		pValue.m_count += 1;
	}
	if ( pValue.m_count > 0 )
	{
		netsend_citynationherolist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}
	return 0;
}

 // 查看信息
int nation_hero_view( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind >= g_nation_hero_maxcount )
		return -1;
	int offset = g_nation_heroinfo[kind].offset;
	if ( offset < 0 || offset >= NATIONHERO_MAX )
		return -1;
	HeroInfoConfig *config = hero_getconfig( kind, g_nation_hero[kind].color );
	if ( !config )
		return -1;
	SLK_NetS_NationHeroAttr pValue = { 0 };
	pValue.m_attr.m_kind = kind;
	pValue.m_attr.m_level = g_nation_hero[kind].level;
	pValue.m_attr.m_color = g_nation_hero[kind].color;
	pValue.m_attr.m_exp = g_nation_hero[kind].exp;
	pValue.m_attr.m_exp_max = hero_getexp_max( g_nation_hero[kind].level, g_nation_hero[kind].color );
	pValue.m_attr.m_soldiers = g_nation_hero[kind].soldiers;
	pValue.m_attr.m_attack_base = config->attack_base;
	pValue.m_attr.m_attack_wash = g_nation_hero[kind].attack_wash;
	pValue.m_attr.m_defense_base = config->defense_base;
	pValue.m_attr.m_defense_wash = g_nation_hero[kind].defense_wash;
	pValue.m_attr.m_troops_base = config->troops_base;
	pValue.m_attr.m_troops_wash = g_nation_hero[kind].troops_wash;
	pValue.m_attr.m_attack = g_nation_hero[kind].attack;
	pValue.m_attr.m_defense = g_nation_hero[kind].defense;
	pValue.m_attr.m_troops = g_nation_hero[kind].troops;
	pValue.m_attr.m_attack_increase = g_nation_hero[kind].attack_increase;
	pValue.m_attr.m_defense_increase = g_nation_hero[kind].defense_increase;
	if ( g_nation_hero[kind].actorid > 0 )
	{ // 已经有主人
		pValue.m_open = 1;
		pValue.m_actorid = g_nation_hero[kind].actorid;
		int city_index = g_nation_hero[kind].city_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
		{
			strncpy( pValue.m_name, city_getname( g_nation_hero[kind].city_index ), NAME_SIZE );
			pValue.m_namelen = strlen( pValue.m_name );
			pValue.m_nation = g_city[city_index].nation;
			pValue.m_loyal = g_city[city_index].nation_hero[offset].loyal;
		}
	}
	else
	{ // 检查是否开放
		short own_nation = g_nation_heroinfo[kind].nation;
		if ( own_nation > 0 && own_nation < 4 && g_nation[own_nation].level >= g_nation_heroinfo[kind].nationlevel )
		{
			pValue.m_open = 1;
		}
	}
	netsend_nationheroattr_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
