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
	}
	else if ( g_nation_hero[herokind].actorid > 0 )
	{ // 有主人
		int city_index = city_getindex_withactorid( g_nation_hero[herokind].actorid );
		g_nation_hero[herokind].city_index = city_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
		{
			int offset = g_nation_heroinfo[herokind].offset;
			if ( offset < 0 || offset >= NATIONHERO_MAX )
				return 0;
			Hero *pHero = city_hero_getptr_withkind( city_index, herokind );
			if ( pHero )
			{ // 上阵
				nation_hero_attrcalc( herokind, pHero );
			}
			else
			{ // 数据库找
				
			}
		}
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

inline NationHeroInfo *nation_hero_getconfig( int kind )
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

	pAttr->m_short_value[0] = g_nation_hero[kind].level;
	pAttr->m_short_value_count = 1;
}

// 位置
void nation_hero_getpos( int kind, short *posx, short *posy )
{
	if ( kind < 0 || kind >= g_nation_hero_maxcount )
		return;
	*posx = g_nation_hero[kind].posx;
	*posy = g_nation_hero[kind].posy;
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
		if ( kind <= 0 )
			continue;
		if ( pNation->level >= g_nation_heroinfo[tmpi].nationlevel )
		{
			nation_hero_showmap( kind );
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
		if ( map_getrandpos_withtype( 3, &g_nation_hero[kind].posx, &g_nation_hero[kind].posy ) >= 0 )
			break;
		step += 1;
		if ( step >= 10 )
			return;
	}

	// 添加到地图显示单元
	g_nation_hero[kind].unit_index = mapunit_add( MAPUNIT_TYPE_NATIONHERO, kind );

	// 占地块信息添加到世界地图
	map_addobject( MAPUNIT_TYPE_NATIONHERO, kind, g_nation_hero[kind].posx, g_nation_hero[kind].posy );
}

// 更新属性
void nation_hero_attrcalc( int kind, Hero *pHero )
{
	if ( kind <= 0 || kind >= g_nation_hero_maxcount )
		return;
	if ( g_nation_hero[kind].actorid != pHero->actorid )
		return;
	g_nation_hero[kind].level = pHero->level;
	g_nation_hero[kind].color = pHero->color;
	g_nation_hero[kind].exp = pHero->exp;
	g_nation_hero[kind].soldiers = pHero->soldiers;
	g_nation_hero[kind].attack_wash = pHero->attack_wash;
	g_nation_hero[kind].defense_wash = pHero->defense_wash;
	g_nation_hero[kind].troops_wash = pHero->troops_wash;
	g_nation_hero[kind].colorup = pHero->colorup;
	g_nation_hero[kind].attack = pHero->attack;
	g_nation_hero[kind].defense = pHero->defense;
	g_nation_hero[kind].troops = pHero->troops;
	g_nation_hero[kind].attack_increase = pHero->attack_increase;
	g_nation_hero[kind].defense_increase = pHero->defense_increase;
	g_nation_hero[kind].assault = pHero->assault;
	g_nation_hero[kind].defend = pHero->defend;
	memcpy( g_nation_hero[kind].equip, pHero->equip, sizeof( Equip ) * 6 );
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
					Hero *pHero = city_hero_getptr_withkind( city_index, kind );
					if ( pHero )
					{ // 是上阵的，就刷新一遍属性
						nation_hero_attrcalc( kind, pHero );
					}
					pValue.m_list[pValue.m_count].m_level = g_nation_hero[kind].level;
					strncpy( pValue.m_list[pValue.m_count].m_name, city_getname( g_nation_hero[kind].city_index ), NAME_SIZE );
					pValue.m_list[pValue.m_count].m_namelen = strlen( pValue.m_list[pValue.m_count].m_name );
				}
			}
			else
			{ // 无主人
				pValue.m_list[pValue.m_count].m_level = g_nation_hero[kind].level;
				pValue.m_list[pValue.m_count].m_buypos = pCity->nation_hero[offset].buypos;
				pValue.m_list[pValue.m_count].m_posx = pCity->nation_hero[offset].posx;
				pValue.m_list[pValue.m_count].m_posy = pCity->nation_hero[offset].posy;
			}
		}
		pValue.m_count += 1;
	}
	netsend_nationherolist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

