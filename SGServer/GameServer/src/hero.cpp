#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <math.h>
#include "db.h"
#include "global.h"
#include "system.h"
#include "actor.h"
#include "city.h"
#include "building.h"
#include "hero.h"
#include "mapunit.h"
#include "map.h"
#include "item.h"
#include "equip.h"
#include "server_netsend_auto.h"
#include "actor_notify.h"
#include "city_attr.h"
#include "actor_times.h"

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

extern EquipWashInfo *g_equipwash;
extern int g_equipwash_maxnum;

extern UpgradeInfo *g_upgradeinfo;
extern int g_upgradeinfo_maxnum;

extern HeroInfo *g_heroinfo;
extern int g_heroinfo_maxnum;

extern HeroColorupInfo *g_hero_colorup;
extern int g_hero_colorup_maxnum;

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
	if ( herokind == 0 )
		return index;
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

Hero *hero_getptr( int actor_index, int herokind )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return NULL;
	if ( herokind <= 0 )
		return NULL;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return NULL;
	for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind == herokind )
		{
			return &pCity->hero[tmpi];
		}
	}

	for ( int tmpi = 0; tmpi < HERO_ACTOR_MAX; tmpi++ )
	{
		if ( g_actors[actor_index].hero[tmpi].kind == herokind )
		{
			return &g_actors[actor_index].hero[tmpi];
		}
	}
	return NULL;
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
		if ( g_heroinfo[kind].config[color].kind > 0 )
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
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
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
	memset( &g_actors[actor_index].hero[offset], 0, sizeof( Hero ) );
	g_actors[actor_index].hero[offset].id = g_maxheroid++;
	g_actors[actor_index].hero[offset].actorid = g_actors[actor_index].actorid;
	g_actors[actor_index].hero[offset].offset = offset;
	g_actors[actor_index].hero[offset].kind = (short)kind;	//英雄种类
	g_actors[actor_index].hero[offset].color = color;	//颜色
	g_actors[actor_index].hero[offset].level = 1;	//等级
	g_actors[actor_index].hero[offset].exp = 0;	//经验
	g_actors[actor_index].hero[offset].attack_wash = config->attack_wash;	//洗髓攻击资质
	g_actors[actor_index].hero[offset].defense_wash = config->defense_wash;	//洗髓防御资质
	g_actors[actor_index].hero[offset].troops_wash = config->troops_wash;	//洗髓兵力资质
	
	// 重算英雄属性
	hero_attr_calc( pCity, &g_actors[actor_index].hero[offset] );

	// 兵力补满
	g_actors[actor_index].hero[offset].soldiers = g_actors[actor_index].hero[offset].troops;

	SLK_NetS_HeroGet pValue = { 0 };
	pValue.m_kind = kind;
	pValue.m_path = path;
	hero_makestruct( city_getptr( actor_index ), offset, &g_actors[actor_index].hero[offset], &pValue.m_hero );
	netsend_heroget_S( actor_index, SENDTYPE_ACTOR, &pValue );

	// 上阵
	hero_up_auto( actor_index, offset );

	// 重算英雄战力
	city_battlepower_hero_calc( pCity );
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
	int maxhero = 2 + pCity->attr.hero_up_num;
	if ( maxhero > 4 )
		maxhero = 4;

	int index = -1;
	for ( int tmpi = 0; tmpi < maxhero; tmpi++ )
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
int hero_up( int actor_index, int selectkind, int upkind, int replace_equip )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Hero *pHero = city_hero_getptr( pCity->index, HERO_BASEOFFSET + city_hero_getindex( pCity->index, selectkind ) );
	if ( !pHero )
	{ // 没有需要替换的,找空位
		int maxhero = 2 + pCity->attr.hero_up_num;
		if ( maxhero > 4 )
			maxhero = 4;
		int index = -1;
		for ( int tmpi = 0; tmpi < maxhero; tmpi++ )
		{
			if ( pCity->hero[tmpi].id <= 0 )
			{
				index = tmpi;
				break;
			}
		}
		if ( index < 0 )
			return -1;
		pHero = &pCity->hero[index];
		pHero->offset = HERO_BASEOFFSET + index;
	}
	if ( !pHero )
	{
		return -1;
	}
	if ( pHero->state != HERO_STATE_NORMAL )
	{
		POP( actor_index, 845 );
		return -1;
	}

	// 需要上阵的英雄
	Hero *pUpHero = actor_hero_getptr( actor_index, actor_hero_getindex( actor_index, upkind ) );
	if ( !pUpHero )
	{
		return -1;
	}
	
	int hero_offset = pHero->offset;
	int up_hero_offset = pUpHero->offset;

	Hero tmp = { 0 };
	memcpy( &tmp, pHero, sizeof( Hero ) );
	memcpy( pHero, pUpHero, sizeof( Hero ) );
	memcpy( pUpHero, &tmp, sizeof( Hero ) );
	pHero->offset = hero_offset;
	pUpHero->offset = up_hero_offset;

	if ( replace_equip == 1 )
	{
		// 替换就拷贝回去
		Equip tmp_equip[6] = { 0 };
		memcpy( &tmp_equip, &pHero->equip, sizeof( Equip ) * 6 );
		memcpy( &pHero->equip, &pUpHero->equip, sizeof( Equip ) * 6 );
		memcpy( &pUpHero->equip, &tmp_equip, sizeof( Equip ) * 6 );

		for ( int tmpi = 0; tmpi < 6; tmpi++ )
		{
			if ( pHero->equip[tmpi].kind > 0 )
				pHero->equip[tmpi].offset = pHero->kind * 1000 + tmpi;
			if ( pUpHero->equip[tmpi].kind > 0 )
				pUpHero->equip[tmpi].offset = pUpHero->kind * 1000 + tmpi;
		}
	}
	
	SLK_NetS_HeroReplace pValue = { 0 };
	pValue.m_up_kind = upkind;
	pValue.m_down_kind = selectkind;
	netsend_heroreplace_S( actor_index, SENDTYPE_ACTOR, &pValue );

	// 重算英雄属性
	hero_attr_calc( pCity, pHero );
	hero_attr_calc( pCity, pUpHero );

	// 更新英雄信息
	hero_sendinfo( actor_index, pHero );
	hero_sendinfo( actor_index, pUpHero );

	// 更新英雄装备信息
	equip_heroupdate( actor_index, pHero );
	equip_heroupdate( actor_index, pUpHero );

	city_battlepower_hero_calc( pCity );
	return 0;
}

// 下阵
int hero_down( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( kind <= 0 || kind >= g_heroinfo_maxnum )
		return -1;
	return 0;
}

// 使用经验道具
int hero_useexpitem( int actor_index, int herokind, int itemkind )
{
	Hero *pHero = NULL;
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	pHero = hero_getptr( actor_index, herokind );
	if ( !pHero )
		return -1;
	if ( pHero->state != HERO_STATE_NORMAL )
	{
		POP( actor_index, 845 );
		return -1;
	}
	if ( pHero->level >= g_actors[actor_index].level )
		return -1;

	// 使用道具模式
	int costtype = 0;

	// 有道具使用道具，无道具使用钻石
	if ( item_getitemnum( actor_index, itemkind ) <= 0 )
	{
		if ( g_actors[actor_index].token < item_gettoken( itemkind ) )
		{
			return -1;
		}
		costtype = 1; // 使用钻石模式
	}

	int exp = item_get_base_value( itemkind, 0 );

	if ( costtype == 0 )
	{
		item_lost( actor_index, itemkind, 1, PATH_SYSTEM );
		hero_addexp( pCity, pHero, exp, PATH_ITEMUSE );
	}
	else
	{
		actor_change_token( actor_index, -item_gettoken( itemkind ), PATH_HERO_ADDEXP, 0 );
		hero_addexp( pCity, pHero, exp, PATH_TOKENITEMUSE );
	}
	return 0;
}

static int _hero_upgrade( Hero *pHero, short path )
{
	if ( !pHero )
		return -1;
	if ( pHero->level >= global.actorlevel_max )
		return -1;
	int lastlevel = pHero->level;
	pHero->level += 1;
	wlog( 0, LOGOP_HEROUPGRADE, path, pHero->kind, pHero->level, 0, pHero->actorid, 0 );
	return 0;
}

// 加经验
int hero_addexp( City *pCity, Hero *pHero, int exp, short path )
{
	if ( !pCity || !pHero )
		return -1;

	char isup = 0;
	pHero->exp += exp;
	// 检查升级
	while ( pHero->exp >= hero_getexp_max( pHero->level, pHero->color ) )
	{
		int curlevel = pHero->level;
		// 可以升级
		if ( pHero->level >= pCity->level || _hero_upgrade( pHero, path ) < 0 )
			break;
		pHero->exp -= hero_getexp_max( curlevel, pHero->color );
		isup = 1;
	}

	if ( pCity->actor_index >= 0 )
	{
		SLK_NetS_HeroExp pValue = { 0 };
		pValue.m_kind = pHero->kind;
		pValue.m_add = exp;
		pValue.m_exp = pHero->exp;
		pValue.m_exp_max = hero_getexp_max( pHero->level, pHero->color );
		pValue.m_level = pHero->level;
		pValue.m_isup = isup;
		pValue.m_path = path;
		netsend_heroexp_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
	}

	if ( isup )
	{
		// 重算英雄属性
		hero_attr_calc( pCity, pHero );
		// 重算战力
		city_battlepower_hero_calc( pCity );
	}
	return 0;
}

// 加兵力
int hero_addsoldiers( int actor_index, int herokind )
{
	Hero *pHero = NULL;
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	pHero = hero_getptr( actor_index, herokind );
	if ( !pHero )
		return -1;
	if ( pHero->state != HERO_STATE_NORMAL )
	{
		POP( actor_index, 845 );
		return -1;
	}

	int troops = pHero->troops;
	int add = troops - pHero->soldiers;
	if ( add <= 0 )
		return -1;

	HeroInfoConfig *config = hero_getconfig( herokind, pHero->color );
	if ( !config )
		return -1;

	int has = city_soldiers( pCity->index, config->corps );
	if ( has <= 0 )
		return -1;

	if ( add > has )
		add = has;

	pHero->soldiers += add;
	SLK_NetS_HeroSoldiers pValue = { 0 };
	pValue.m_kind = herokind;
	pValue.m_add = add;
	pValue.m_soldiers = pHero->soldiers;
	pValue.m_soldiers_max = troops;
	pValue.m_path = PATH_HERO_ADDSOLDIERS;
	netsend_herosoldiers_S( actor_index, SENDTYPE_ACTOR, &pValue );

	city_changesoldiers( pCity->index, config->corps, -add, PATH_HERO_ADDSOLDIERS );
	return pHero->soldiers;
}

int hero_changesoldiers( City *pCity, Hero *pHero, int value, short path )
{
	if ( !pCity )
		return -1;
	if ( !pHero )
		return -1;
	if ( value == 0 )
		return -1;

	pHero->soldiers += value;
	if ( pHero->soldiers < 0 )
	{
		pHero->soldiers = 0;
		value = -pHero->soldiers;
	}
	else if ( pHero->soldiers > pHero->troops )
	{
		value = pHero->troops - pHero->soldiers;
		pHero->soldiers = pHero->troops;
	}

	if ( pCity->actor_index >= 0 )
	{
		SLK_NetS_HeroSoldiers pValue = { 0 };
		pValue.m_kind = pHero->kind;
		pValue.m_add = value;
		pValue.m_soldiers = pHero->soldiers;
		pValue.m_soldiers_max = pHero->troops;
		pValue.m_path = PATH_HERO_ADDSOLDIERS;
		netsend_herosoldiers_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
	}
	return 0;
}

// 英雄状态
int hero_changestate( int city_index, int herokind, char state )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return -1;
	if ( herokind <= 0 )
		return -1;
	int hero_index = city_hero_getindex( city_index, herokind );
	if ( hero_index < 0 || hero_index >= HERO_CITY_MAX )
		return -1;
	if ( g_city[city_index].hero[hero_index].state == state )
		return -1;
	g_city[city_index].hero[hero_index].state = state;
	if ( g_city[city_index].actor_index >= 0 )
	{
		SLK_NetS_HeroState pValue = { 0 };
		pValue.m_kind = herokind;
		pValue.m_state = state;
		netsend_herostate_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	}
	return 0;
}

// 武将带兵排数
char hero_getline( City *pCity )
{
	if ( !pCity )
		return 2;
	return 2 + pCity->attr.hero_row_num;
}

// 计算装备加成
int hero_equip_calc( Hero *pHero, int ability )
{
	if ( !pHero )
		return 0;
	int value = 0;
	for ( int tmpi = 0; tmpi < EQUIP_TYPE_MAX; tmpi++ )
	{
		int kind = pHero->equip[tmpi].kind;
		if ( kind > 0 && kind < g_equipinfo_maxnum )
		{
			// 装备基础属性
			if ( g_equipinfo[kind].ability == ability )
			{
				value += g_equipinfo[kind].value;
			}

			// 装备洗练属性
			for ( int i = 0; i < EQUIP_WASHMAX; i++ )
			{
				int id = pHero->equip[tmpi].washid[i];
				if ( id > 0 && id < g_equipwash_maxnum )
				{
					if ( g_equipwash[id].ability == ability )
					{
						value += g_equipwash[id].value;
					}
				}
			}
		}
	}
	return value;
}

//// 英雄攻击力
//int hero_attack( City *pCity, Hero *pHero )
//{
//	if ( !pCity || !pHero )
//		return 0;
//	HeroInfoConfig *config = hero_getconfig( pHero->kind, pHero->color );
//	if ( !config )
//		return 0;
//	// 基础
//	int base = ATTACK( pHero->level, config->attack, (config->attack_base + pHero->attack_wash) );
//	// 装备
//	int equip = hero_equip_calc( pHero, EQUIP_ABILITY_ATTACK );
//	// 属性
//	int attr = pCity->attr.hero_attack[config->corps];
//	// 总
//	int total = base + equip + attr;
//	return total;
//}
//
//// 英雄防御力
//int hero_defense( City *pCity, Hero *pHero )
//{
//	if ( !pCity || !pHero )
//		return 0;
//	HeroInfoConfig *config = hero_getconfig( pHero->kind, pHero->color );
//	if ( !config )
//		return 0;
//	// 基础
//	int base = DEFENSE( pHero->level, config->defense, (config->defense_base + pHero->defense_wash) );
//	// 装备
//	int equip = hero_equip_calc( pHero, EQUIP_ABILITY_DEFENSE );
//	// 属性
//	int attr = pCity->attr.hero_defense[config->corps];
//	// 总
//	int total = base + equip + attr;
//	return total;
//}
//
//// 英雄兵力
//int hero_troops( City *pCity, Hero *pHero )
//{
//	if ( !pCity || !pHero )
//		return 0;
//	HeroInfoConfig *config = hero_getconfig( pHero->kind, pHero->color );
//	if ( !config )
//		return 0;
//	// 基础
//	int base = TROOPS( pHero->level, config->troops, (config->troops_base + pHero->troops_wash) );
//	// 装备
//	int equip = hero_equip_calc( pHero, EQUIP_ABILITY_TROOPS );
//	// 属性
//	int attr = pCity->attr.hero_troops[config->corps];
//	// 总
//	int total = base + equip + attr;
//	return total;
//}
//
//// 强攻
//int hero_attack_increase( City *pCity, Hero *pHero )
//{
//	if ( !pCity || !pHero )
//		return 0;
//	// 装备
//	int equip = hero_equip_calc( pHero, EQUIP_ABILITY_ATTACK_INCREASE );
//	// 总
//	int total = equip;
//	return total;
//}
//
//// 强防
//int hero_defense_increase( City *pCity, Hero *pHero )
//{
//	if ( !pCity || !pHero )
//		return 0;
//	// 装备
//	int equip = hero_equip_calc( pHero, EQUIP_ABILITY_DEFENSE_INCREASE );
//	// 总
//	int total = equip;
//	return total;
//}
//
//// 攻城
//int hero_assault( City *pCity, Hero *pHero )
//{
//	if ( !pCity || !pHero )
//		return 0;
//	// 装备
//	int equip = hero_equip_calc( pHero, EQUIP_ABILITY_ASSAULT );
//	// 总
//	int total = equip;
//	return total;
//}
//
//// 守城
//int hero_defend( City *pCity, Hero *pHero )
//{
//	if ( !pCity || !pHero )
//		return 0;
//	// 装备
//	int equip = hero_equip_calc( pHero, EQUIP_ABILITY_DEFEND );
//	// 总
//	int total = equip;
//	return total;
//}

// 英雄属性计算
int hero_attr_calc( City *pCity, Hero *pHero )
{
	if ( !pCity || !pHero )
		return -1;
	HeroInfoConfig *config = hero_getconfig( pHero->kind, pHero->color );
	if ( !config )
		return 0;
	// 基础攻击
	int base_attack = ATTACK( pHero->level, config->attack, (config->attack_base + pHero->attack_wash) );
	int base_defense = DEFENSE( pHero->level, config->defense, (config->defense_base + pHero->defense_wash) );
	int base_troops = TROOPS( pHero->level, config->troops, (config->troops_base + pHero->troops_wash) );

	// 属性附加攻击
	int attr_attack = pCity->attr.hero_attack[config->corps];
	int attr_defense = pCity->attr.hero_defense[config->corps];
	int attr_troops = pCity->attr.hero_troops[config->corps];

	// 装备附加
	int equip_attack = 0;
	int equip_defense = 0;
	int equip_troops = 0;
	int equip_attack_increase = 0;
	int equip_defense_increase = 0;
	int equip_assault = 0;
	int equip_defend = 0;
	for ( int tmpi = 0; tmpi < EQUIP_TYPE_MAX; tmpi++ )
	{
		int kind = pHero->equip[tmpi].kind;
		if ( kind > 0 && kind < g_equipinfo_maxnum )
		{
			// 装备基础属性
			switch ( g_equipinfo[kind].ability )
			{
			case EQUIP_ABILITY_ATTACK:
				equip_attack += g_equipinfo[kind].value;
				break;
			case EQUIP_ABILITY_DEFENSE:
				equip_defense += g_equipinfo[kind].value;
				break;
			case EQUIP_ABILITY_TROOPS:
				equip_troops += g_equipinfo[kind].value;
				break;
			case EQUIP_ABILITY_ATTACK_INCREASE:
				equip_attack_increase += g_equipinfo[kind].value;
				break;
			case EQUIP_ABILITY_DEFENSE_INCREASE:
				equip_defense_increase += g_equipinfo[kind].value;
				break;
			case EQUIP_ABILITY_ASSAULT:
				equip_assault += g_equipinfo[kind].value;
				break;
			case EQUIP_ABILITY_DEFEND:
				equip_defend += g_equipinfo[kind].value;
				break;
			}
			
			// 装备洗练属性
			for ( int i = 0; i < EQUIP_WASHMAX; i++ )
			{
				int id = pHero->equip[tmpi].washid[i];
				if ( id > 0 && id < g_equipwash_maxnum )
				{
					switch ( g_equipwash[id].ability )
					{
					case EQUIP_ABILITY_ATTACK:
						equip_attack += g_equipwash[id].value;
						break;
					case EQUIP_ABILITY_DEFENSE:
						equip_defense += g_equipwash[id].value;
						break;
					case EQUIP_ABILITY_TROOPS:
						equip_troops += g_equipwash[id].value;
						break;
					case EQUIP_ABILITY_ATTACK_INCREASE:
						equip_attack_increase += g_equipwash[id].value;
						break;
					case EQUIP_ABILITY_DEFENSE_INCREASE:
						equip_defense_increase += g_equipwash[id].value;
						break;
					case EQUIP_ABILITY_ASSAULT:
						equip_assault += g_equipwash[id].value;
						break;
					case EQUIP_ABILITY_DEFEND:
						equip_defend += g_equipwash[id].value;
						break;
					}
				}
			}
		}
	}

	// 综合计算
	pHero->attack = base_attack + attr_attack + equip_attack;
	pHero->defense = base_defense + attr_defense + equip_defense;
	pHero->troops = base_troops + (attr_troops + equip_troops)/4;
	pHero->attack_increase = equip_attack_increase;
	pHero->defense_increase = equip_defense_increase;
	pHero->assault = equip_assault;
	pHero->defend = equip_defend;
	return 0;
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
void hero_makestruct( City *pCity, int offset, Hero *pHero, SLK_NetS_Hero *pValue )
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
	pValue->m_state = pHero->state;
	pValue->m_corps = (char)config->corps;
	pValue->m_attack_base = config->attack_base;
	pValue->m_attack_wash = pHero->attack_wash;
	pValue->m_defense_base = config->defense_base;
	pValue->m_defense_wash = pHero->defense_wash;
	pValue->m_troops_base = config->troops_base;
	pValue->m_troops_wash = pHero->troops_wash;
	pValue->m_attack = pHero->attack;
	pValue->m_defense = pHero->defense;
	pValue->m_troops = pHero->troops;
	pValue->m_attack_increase = pHero->attack_increase;
	pValue->m_defense_increase = pHero->defense_increase;
	pValue->m_offset = offset;
}

int hero_sendinfo( int actor_index, Hero *pHero )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_Hero pValue = { 0 };
	hero_makestruct( pCity, pHero->offset, pHero, &pValue );
	netsend_hero_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
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
			hero_makestruct( pCity, tmpi, &pCity->hero[tmpi], &pValue.m_list[pValue.m_count] );
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
			hero_makestruct( pCity, tmpi, &g_actors[actor_index].hero[tmpi], &pValue.m_list[pValue.m_count] );
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

int hero_wash_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_HeroWash pValue = { 0 };
	pValue.m_hero_washnum = pCity->hero_washnum;
	pValue.m_hero_washsec = pCity->hero_washsec;
	netsend_herowash_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 免费洗髓
int hero_wash_free( int actor_index, int herokind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->hero_washnum <= 0 )
		return -1;

	Hero *pHero = hero_getptr( actor_index, herokind );
	if ( !pHero )
		return -1;
	if ( pHero->state != HERO_STATE_NORMAL )
	{
		POP( actor_index, 845 );
		return -1;
	}

	HeroInfoConfig *config = hero_getconfig( pHero->kind, pHero->color );
	if ( !config )
		return -1;

	// 总洗髓资质
	int total_wash = pHero->attack_wash + pHero->defense_wash + pHero->troops_wash;
	
	// 总洗髓资质上限
	int total_wash_limit = config->attack_wash_limit + config->defense_wash_limit + config->troops_wash_limit;

	// 洗髓进度
	float x = total_wash / (float)config->total_wash;

	// 每次增加
	int add = (int)(random( 1, global.hero_wash_free_rand )*(1.0f - x));
	total_wash += add;
	if ( total_wash > config->total_wash )
		total_wash = config->total_wash;

	// 攻防分配比例
	int attack_min = (int)max( 0, (int)ceil( (total_wash - config->defense_wash_limit - config->troops_wash_limit) / (float)total_wash * 100 + 1 ) );
	int attack_max = (int)(min( 1.0, config->attack_wash_limit / (float)total_wash ) * 100 - 1);
	int attack_ratio = random( attack_min, attack_max );
	pHero->attack_wash = (short)min( config->attack_wash_limit, (total_wash*attack_ratio / 100.0f) );

	int defense_min = (int)max( 0, (int)((total_wash - pHero->attack_wash - config->troops_wash_limit) / (float)total_wash * 100 + 1) );
	int defense_max = (int)ceil( min( 1.0, config->defense_wash_limit / (float)total_wash ) * 100 - 1 );
	short defense_ratio = random( defense_min, defense_max );
	pHero->defense_wash = (short)min( config->defense_wash_limit, (total_wash*defense_ratio / 100.0f) );

	pHero->troops_wash = total_wash - pHero->attack_wash - pHero->defense_wash;

	// 重算英雄属性
	hero_attr_calc( pCity, pHero );
	// 重算战力
	city_battlepower_hero_calc( pCity );
	// 更新英雄
	hero_sendinfo( actor_index, pHero );

	pCity->hero_washnum -= 1;
	if ( pCity->hero_washsec <= 0 )
		pCity->hero_washsec = global.hero_wash_sec;
	hero_wash_sendinfo( actor_index );
	return 0;
}

// 至尊洗髓
int hero_wash_token( int actor_index, int herokind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	Hero *pHero = hero_getptr( actor_index, herokind );
	if ( !pHero )
		return -1;
	if ( pHero->state != HERO_STATE_NORMAL )
	{
		POP( actor_index, 845 );
		return -1;
	}

	HeroInfoConfig *config = hero_getconfig( pHero->kind, pHero->color );
	if ( !config )
		return -1;

	if ( actor_change_token( actor_index, -global.hero_wash_token, PATH_HERO_WASH, 0 ) < 0 )
		return -1;

	// 总洗髓资质
	int total_wash = pHero->attack_wash + pHero->defense_wash + pHero->troops_wash;

	// 总洗髓资质上限
	int total_wash_limit = config->attack_wash_limit + config->defense_wash_limit + config->troops_wash_limit;

	// 洗髓进度
	float x = total_wash / (float)config->total_wash;

	// 每次增加
	int add = max( (int)(random( 1, global.hero_wash_token_rand )*(1.0f - x)), 1 );
	total_wash += add;
	if ( total_wash > config->total_wash )
		total_wash = config->total_wash;

	// 特殊处理
	int odds = rand() % 100;
	int s = (int)(x * 100);
	if ( s >= global.hero_wash_token_full_base && odds < 15 )
	{
		pHero->attack_wash = config->attack_wash_limit;
		pHero->defense_wash = config->defense_wash_limit;
	}
	else
	{
		// 攻防分配比例
		//short hero_wash_ratio_down = (short)ceil( 50 - config->troops_wash_limit / (float)config->total_wash * 50 );
		//short attack_ratio = random( hero_wash_ratio_down, global.hero_wash_ratio_up );
		//short defense_ratio = random( hero_wash_ratio_down, global.hero_wash_ratio_up );
		//pHero->attack_wash = (short)min( config->attack_wash_limit, (total_wash*attack_ratio / 100.0f) );
		//pHero->defense_wash = (short)min( config->defense_wash_limit, (total_wash*defense_ratio / 100.0f) );

		// 攻防分配比例
		int attack_min = (int)max( 0, (int)ceil( (total_wash - config->defense_wash_limit - config->troops_wash_limit) / (float)total_wash * 100 + 1 ) );
		int attack_max = (int)(min( 1.0, config->attack_wash_limit / (float)total_wash ) * 100 - 1);
		int attack_ratio = random( attack_min, attack_max );
		pHero->attack_wash = (short)min( config->attack_wash_limit, (total_wash*attack_ratio / 100.0f) );

		int defense_min = (int)max( 0, (int)((total_wash - pHero->attack_wash - config->troops_wash_limit) / (float)total_wash * 100 + 1) );
		int defense_max = (int)ceil( min( 1.0, config->defense_wash_limit / (float)total_wash ) * 100 - 1 );
		short defense_ratio = random( defense_min, defense_max );
		pHero->defense_wash = (short)min( config->defense_wash_limit, (total_wash*defense_ratio / 100.0f) );
	}
	pHero->troops_wash = total_wash - pHero->attack_wash - pHero->defense_wash;

	// 重算英雄属性
	hero_attr_calc( pCity, pHero );
	// 重算战力
	city_battlepower_hero_calc( pCity );
	// 更新英雄
	hero_sendinfo( actor_index, pHero );
	return 0;
}

// 突破值
int hero_colorup_sendinfo( int actor_index, int herokind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	_check_fday( actor_index );

	Hero *pHero = hero_getptr( actor_index, herokind );
	if ( !pHero )
		return -1;
	if ( pHero->state != HERO_STATE_NORMAL )
	{
		POP( actor_index, 845 );
		return -1;
	}

	SLK_NetS_HeroColorup pValue = { 0 };
	pValue.m_value = pHero->colorup;
	pValue.m_isup = 0;
	netsend_herocolorup_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 重置突破值
int hero_colorup_reset( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind <= 0 )
			continue;
		pCity->hero[tmpi].colorup = 0;
	}

	for ( int tmpi = 0; tmpi < HERO_ACTOR_MAX; tmpi++ )
	{
		if ( g_actors[actor_index].hero[tmpi].kind <= 0 )
			continue;
		g_actors[actor_index].hero[tmpi].colorup = 0;
	}
	return 0;
}

// 良将突破
int hero_colorup( int actor_index, int herokind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	Hero *pHero = hero_getptr( actor_index, herokind );
	if ( !pHero )
		return -1;
	if ( pHero->state != HERO_STATE_NORMAL )
	{
		POP( actor_index, 845 );
		return -1;
	}

	HeroInfoConfig *config = hero_getconfig( pHero->kind, pHero->color );
	if ( !config )
		return -1;

	if ( pHero->color < ITEM_COLOR_LEVEL_GREEN || pHero->color >= g_hero_colorup_maxnum )
		return -1;
	_check_fday( actor_index );

	if ( item_getitemnum( actor_index, g_hero_colorup[pHero->color].itemkind ) < g_hero_colorup[pHero->color].itemnum )
		return -1;
	item_lost( actor_index, g_hero_colorup[pHero->color].itemkind, g_hero_colorup[pHero->color].itemnum, PATH_SYSTEM );

	// 先计算是否直接突破
	char isup = 0;
	int odds = rand() % 1000;
	int oddsvalue = g_hero_colorup[pHero->color].odds + (int)ceil( g_hero_colorup[pHero->color].odds * pCity->attr.heroluck_per );
	if ( odds < oddsvalue )
	{
		isup = 1;
	}
	

	pHero->colorup += g_hero_colorup[pHero->color].value;
	if ( pHero->colorup >= g_hero_colorup[pHero->color].needvalue || isup == 1 )
	{
		isup = 1;
		pHero->colorup = 0;
		pHero->color = (char)g_hero_colorup[pHero->color].nextcolor;
		// 重算英雄属性
		hero_attr_calc( pCity, pHero );
		// 重算战力
		city_battlepower_hero_calc( pCity );
		// 更新英雄
		hero_sendinfo( actor_index, pHero );
	}

	// 更新客户端
	SLK_NetS_HeroColorup pValue = { 0 };
	pValue.m_value = pHero->colorup;
	pValue.m_isup = isup;
	netsend_herocolorup_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

int hero_gm_level( City *pCity, int level )
{
	for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind <= 0 )
			continue;
		pCity->hero[tmpi].level = level;
		// 重算英雄属性
		hero_attr_calc( pCity, &pCity->hero[tmpi] );
		// 重算战力
		city_battlepower_hero_calc( pCity );
		// 更新英雄
		hero_sendinfo( pCity->actor_index, &pCity->hero[tmpi] );
	}
	return 0;
}

int hero_gm_soldiers( City *pCity )
{
	for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind <= 0 )
			continue;
		pCity->hero[tmpi].soldiers = pCity->hero[tmpi].troops;
		// 重算英雄属性
		hero_attr_calc( pCity, &pCity->hero[tmpi] );
		// 重算战力
		city_battlepower_hero_calc( pCity );
		// 更新英雄
		hero_sendinfo( pCity->actor_index, &pCity->hero[tmpi] );
	}
	return 0;
}
