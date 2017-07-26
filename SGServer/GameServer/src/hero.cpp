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
#include "item.h"
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

extern EquipInfo *g_equipinfo;
extern int g_equipinfo_maxnum;

extern UpgradeInfo *g_upgradeinfo;
extern int g_upgradeinfo_maxnum;

extern HeroInfo *g_heroinfo;
extern int g_heroinfo_maxnum;
i64 g_maxheroid;

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
	short hero_index = offset - HERO_BASEOFFSET;
	if ( hero_index < 0 || hero_index >= HERO_CITY_MAX )
		return NULL;
	return &g_city[city_index].hero[hero_index];
}

// 是否有这个武将
char actor_hashero( int actor_index, int herokind )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( actor_hero_getindex( actor_index, herokind ) >= 0 )
		return 1;
	if ( city_hero_getindex( g_actors[actor_index].city_index, herokind ) >= 0 )
		return 1;
	return 0;
}

// 缺省颜色
char hero_defaultcolor( int kind )
{
	if ( kind <= 0 || kind >= g_heroinfo_maxnum )
		return 0;
	for ( int color = 0; color < g_heroinfo[kind].maxnum; color++ )
	{
		if ( g_heroinfo[kind].config[color].kind >= 0 )
		{
			return color;
		}
	}
	
	return 0;
}

HeroInfoConfig *hero_getconfig( int kind, int color )
{
	if ( kind <= 0 || kind >= g_heroinfo_maxnum )
		return NULL;
	if ( color < 0 || color >= g_heroinfo[kind].maxnum )
		return NULL;
	return &g_heroinfo[kind].config[color];
}

int hero_maxid_init()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];

	sprintf( szSQL, "select max(id) from actor_hero" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	if ( !((row = mysql_fetch_row( res ))) )
	{
		mysql_free_result( res );
		return -1;
	}
	if ( row[0] )
		g_maxheroid = atoll( row[0] );
	else
		g_maxheroid = 1;
	g_maxheroid++;
	mysql_free_result( res );
	return 0;
}

int hero_gethero( int actor_index, int kind, short path )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( kind <= 0 || kind >= g_heroinfo_maxnum )
		return -1;
	// 检查是否已经有这个武将了
	if ( actor_hashero( actor_index, kind ) == 1 )
		return -1;

	// 找空余位置
	int offset = -1;
	for ( int tmpi = 0; tmpi < HERO_ACTOR_MAX; tmpi++ )
	{
		if ( g_actors[actor_index].hero[tmpi].id <= 0 )
		{
			offset = tmpi;
			break;
		}
	}
	if ( offset < 0 )
		return -1;
	char color = hero_defaultcolor( kind );
	HeroInfoConfig *config = hero_getconfig( kind, color );
	if ( !config )
		return -1;
	g_actors[actor_index].hero[offset].id = g_maxheroid++;
	g_actors[actor_index].hero[offset].actorid = g_actors[actor_index].actorid;
	g_actors[actor_index].hero[offset].offset = offset;
	g_actors[actor_index].hero[offset].kind = (short)kind;	//英雄种类
	g_actors[actor_index].hero[offset].color = color;	//颜色
	g_actors[actor_index].hero[offset].level = 1;	//等级
	g_actors[actor_index].hero[offset].exp = 0;	//经验
	g_actors[actor_index].hero[offset].soldiers = TROOPS( 1, config->troops, (config->troops + config->troops_wash) );	//兵力
	g_actors[actor_index].hero[offset].attack_wash = config->attack_wash;	//洗髓攻击资质
	g_actors[actor_index].hero[offset].defense_wash = config->defense_wash;	//洗髓防御资质
	g_actors[actor_index].hero[offset].troops_wash = config->troops_wash;	//洗髓兵力资质
	//Equip equip[6];	//装备
	actor_hero_save_auto( &g_actors[actor_index].hero[offset], "actor_hero", NULL );

	SLK_NetS_HeroGet pValue = { 0 };
	pValue.m_kind = kind;
	pValue.m_path = path;
	hero_makestruct( offset, &g_actors[actor_index].hero[offset], &pValue.m_hero );
	netsend_heroget_S( actor_index, SENDTYPE_ACTOR, &pValue );

	// 上阵
	hero_up_auto( actor_index, offset );
	return 0;
}

// 自动上阵，只找空余
int hero_up_auto( int actor_index, int offset )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( offset < 0 || offset >= HERO_ACTOR_MAX )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	int index = -1;
	for ( int tmpi = 0; tmpi < 2; tmpi++ )
	{
		if ( pCity->hero[tmpi].id <= 0 )
		{
			index = tmpi;
			break;
		}
	}
	if ( index < 0 )
		return -1;
	
	memset( &pCity->hero[index], 0, sizeof( Hero ) );
	memcpy( &pCity->hero[index], &g_actors[actor_index].hero[offset], sizeof( Hero ) );
	pCity->hero[index].offset = HERO_BASEOFFSET + index;
	memset( &g_actors[actor_index].hero[offset], 0, sizeof( Hero ) );

	SLK_NetS_HeroReplace pValue = { 0 };
	pValue.m_up_kind = pCity->hero[index].kind;
	netsend_heroreplace_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 上阵带替换效果
int hero_up( int actor_index, int offset )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( offset <= 0 || offset >= HERO_ACTOR_MAX )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	int index = -1;
	for ( int tmpi = 0; tmpi < 2; tmpi++ )
	{
		if ( pCity->hero[tmpi].id <= 0 )
		{
			index = tmpi;
			break;
		}
	}
	if ( index < 0 )
		return -1;


	return 0;
}

// 下阵
int hero_down( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( kind <= 0 || kind >= g_heroinfo_maxnum )
		return -1;
}

int hero_getexp_max( int level, int color )
{
	if ( level <= 0 || level >= g_upgradeinfo_maxnum )
		return -1;
	if ( level >= global.actorlevel_max )
		return -1;
	if ( color < 0 || color >= ITEM_COLOR_LEVEL_MAX )
		return -1;
	return g_upgradeinfo[level].heroexp[color];
}

// 拼属性
void hero_makestruct( int offset, Hero *pHero, SLK_NetS_Hero *pValue )
{
	HeroInfoConfig *config = hero_getconfig( pHero->kind, pHero->color );
	if ( !config )
		return;
	pValue->m_kind = pHero->kind;
	pValue->m_color = pHero->color;
	pValue->m_level = pHero->level;
	pValue->m_exp = pHero->exp;
	pValue->m_exp_max = hero_getexp_max( pHero->level, pHero->color );
	pValue->m_soldiers = pHero->soldiers;
	pValue->m_state = 0;
	pValue->m_corps = (char)config->corps;
	pValue->m_attack_base = config->attack_base;
	pValue->m_attack_wash = pHero->attack_wash;
	pValue->m_defense_base = config->defense_base;
	pValue->m_defense_wash = pHero->defense_wash;
	pValue->m_troops_base = config->troops_base;
	pValue->m_troops_wash = pHero->troops_wash;
	pValue->m_attack = ATTACK( pHero->level, config->attack, (pValue->m_attack_base + pValue->m_attack_wash) );
	pValue->m_defense = DEFENSE( pHero->level, config->defense, (pValue->m_defense_base + pValue->m_defense_wash) );
	pValue->m_troops = TROOPS( pHero->level, config->troops, (pValue->m_troops_base + pValue->m_troops_wash) );
	pValue->m_offset = offset;
}

int hero_list( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	// 上阵英雄
	{
		SLK_NetS_HeroList pValue = { 0 };
		pValue.m_type = 0;
		for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
		{
			if ( pCity->hero[tmpi].id <= 0 )
				continue;
			hero_makestruct( tmpi, &pCity->hero[tmpi], &pValue.m_list[pValue.m_count] );
			pValue.m_count += 1;
		}
		netsend_herolist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}

	// 背包英雄
	{
		SLK_NetS_HeroList pValue = { 0 };
		pValue.m_type = 1;
		for ( int tmpi = 0; tmpi < HERO_ACTOR_MAX; tmpi++ )
		{
			if ( g_actors[actor_index].hero[tmpi].id <= 0 )
				continue;
			hero_makestruct( tmpi, &g_actors[actor_index].hero[tmpi], &pValue.m_list[pValue.m_count] );
			pValue.m_count += 1;
			if ( pValue.m_count >= 30 )
			{
				netsend_herolist_S( actor_index, SENDTYPE_ACTOR, &pValue );
				pValue.m_count = 0;
			}
			
		}
		netsend_herolist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}
	return 0;
}
