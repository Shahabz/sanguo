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
#include "chat.h"
#include "actor_times.h"
#include "mail.h"
#include "nation_hero.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;
extern Map g_map;

extern Actor *g_actors;
extern int g_maxactornum;
extern Actor g_temp_actor[2];

extern int g_city_maxindex;
extern City *g_city;
extern int g_city_maxcount;

extern EquipInfo *g_equipinfo;
extern int g_equipinfo_maxnum;

extern EquipWashInfo *g_equipwash;
extern int g_equipwash_maxnum;

extern NationEquipInfo *g_nationequip;
extern int g_nationequip_maxnum;

extern NationPlace *g_nation_place;
extern int g_nation_place_maxnum;

extern UpgradeInfo *g_upgradeinfo;
extern int g_upgradeinfo_maxnum;

extern HeroInfo *g_heroinfo;
extern int g_heroinfo_maxnum;

extern HeroColorupInfo *g_hero_colorup;
extern int g_hero_colorup_maxnum;

extern HeroVisit *g_hero_visit;
extern int g_hero_visit_maxnum;

extern int g_nation_heroinfo_maxnum;
extern int g_nation_hero_maxcount;

HeroVisitAwardGroup g_herovisit_awardgroup[HERO_VISIT_AWARDGROUPMAX];
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

// 临时场景，主要获取离线玩家数据
Hero* actor_hero_getptr_temp( int actor_index, int offset )
{
	if ( actor_index < 0 || actor_index >= 2 )
		return NULL;
	if ( offset < 0 || offset >= HERO_ACTOR_MAX )
		return NULL;
	return &g_temp_actor[actor_index].hero[offset];
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

Hero* city_hero_getptr_withkind( int city_index, int herokind )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	int index = -1;
	if ( herokind == 0 )
		return NULL;
	for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
	{
		if ( g_city[city_index].hero[tmpi].kind == herokind )
		{
			return &g_city[city_index].hero[tmpi];
		}
	}
	return NULL;
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

char hero_getcolor( City *pCity, int herokind )
{
	if ( !pCity )
		return 0;
	for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind == herokind )
		{
			return pCity->hero[tmpi].color;
		}
	}
	return 0;
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
	Hero *pHasHero = hero_getptr( actor_index, kind );
	if ( pHasHero )
	{	
		// 寻访即使获得了，也要发过去，界面需要显示
		char color = hero_defaultcolor( kind );
		HeroInfoConfig *config = hero_getconfig( kind, color );
		SLK_NetS_HeroGet pValue = { 0 };
		pValue.m_kind = kind;
		pValue.m_path = path;
		if ( path == PATH_HEROVISIT )
		{
			if ( config )
			{
				pValue.m_itemnum = config->itemnum;
				item_getitem( actor_index, 173, config->itemnum, -1, PATH_HEROVISIT_CHANGE );
			}
		}
		hero_makestruct( city_getptr( actor_index ), pHasHero->offset, pHasHero, &pValue.m_hero );
		netsend_heroget_S( actor_index, SENDTYPE_ACTOR, &pValue );
		return -1;
	}

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
	g_actors[actor_index].hero[offset].soldiers = 0;
	if ( path == PATH_NATIONHERO )
	{ // 国家名将
		NationHero *nhero = nation_hero_getptr( kind );
		if ( nhero->kind == kind )
		{
			g_actors[actor_index].hero[offset].level = nhero->level;
		}
	}

	// 重算英雄属性
	hero_attr_calc( pCity, &g_actors[actor_index].hero[offset] );

	SLK_NetS_HeroGet pValue = { 0 };
	pValue.m_kind = kind;
	pValue.m_path = path;
	hero_makestruct( city_getptr( actor_index ), offset, &g_actors[actor_index].hero[offset], &pValue.m_hero );
	netsend_heroget_S( actor_index, SENDTYPE_ACTOR, &pValue );

	// 上阵
	if ( path != PATH_NATIONHERO )
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

	// 自动上阵，补满兵力
	hero_addsoldiers( actor_index, pCity->hero[index].kind, PATH_HERO_UP );
	return 0;
}

// 上阵带替换效果
int hero_up( int actor_index, int selectkind, int upkind, int replace_equip, char uptype )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Hero *pHero = city_hero_getptr( pCity->index, HERO_BASEOFFSET + city_hero_getindex( pCity->index, selectkind ) );
	if ( !pHero )
	{ // 没有需要替换的,找空位
		int index = -1;
		if ( uptype == 0 )
		{ // 战斗武将位置0-3
			int maxhero = 2 + pCity->attr.hero_up_num;
			if ( maxhero > 4 )
				maxhero = 4;
			for ( int tmpi = 0; tmpi < maxhero; tmpi++ )
			{
				if ( pCity->hero[tmpi].id <= 0 )
				{
					index = tmpi;
					break;
				}
			}
		}
		else if ( uptype == 1 )
		{ // 财富署武将位置4-7
			if ( building_getlevel( pCity->index, BUILDING_Cabinet, -1 ) < 1 )
				return -1;
			int maxhero = 5;
			if ( pCity->level >= global.hero_cabinet_level4 )
				maxhero = 8;
			else if ( pCity->level >= global.hero_cabinet_level3 )
				maxhero = 7;
			else if ( pCity->level >= global.hero_cabinet_level2 )
				maxhero = 6;
			else if ( pCity->level >= global.hero_cabinet_level1 )
				maxhero = 5;
			for ( int tmpi = 4; tmpi < maxhero; tmpi++ )
			{
				if ( pCity->hero[tmpi].id <= 0 )
				{
					index = tmpi;
					break;
				}
			}
		}
		else if ( uptype == 2 )
		{ // 羽林卫武将位置8-11
			if ( building_getlevel( pCity->index, BUILDING_Cabinet, -1 ) < 2 )
				return -1;
			int maxhero = 9;
			if ( pCity->level >= global.hero_cabinet_level4 )
				maxhero = 12;
			else if ( pCity->level >= global.hero_cabinet_level3 )
				maxhero = 11;
			else if ( pCity->level >= global.hero_cabinet_level2 )
				maxhero = 10;
			else if ( pCity->level >= global.hero_cabinet_level1 )
				maxhero = 9;
			for ( int tmpi = 8; tmpi < maxhero; tmpi++ )
			{
				if ( pCity->hero[tmpi].id <= 0 )
				{
					index = tmpi;
					break;
				}
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

	// 下阵武将，兵力回营
	HeroInfoConfig *config = hero_getconfig( pHero->kind, pHero->color );
	if ( config )
	{
		if ( pHero->offset >= HERO_BASEOFFSET && pHero->offset < HERO_BASEOFFSET + 4 )
		{ // 上阵武将
			city_changesoldiers( pCity->index, config->corps, pHero->soldiers, PATH_HERO_DOWN );
		}
		else if ( pHero->offset >= HERO_BASEOFFSET + 4 && pHero->offset <HERO_BASEOFFSET + 8 )
		{ // 财赋署武将
			city_changesoldiers( pCity->index, config->corps, pHero->soldiers, PATH_HERO_DOWN );
		}
		else if ( pHero->offset >= HERO_BASEOFFSET + 8 && pHero->offset < HERO_BASEOFFSET + 12 )
		{ // 御林卫武将
			city_changefood( pCity->index, (int)(pHero->soldiers*global.trainfood), PATH_HERO_DOWN );
		}
	}
	pHero->soldiers = 0;

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
	pValue.m_up_offset = up_hero_offset;
	pValue.m_down_offset = hero_offset - HERO_BASEOFFSET;
	netsend_heroreplace_S( actor_index, SENDTYPE_ACTOR, &pValue );

	// 重算英雄属性
	hero_attr_calc( pCity, pHero );
	hero_attr_calc( pCity, pUpHero );

	// 新上阵英雄自动补满兵
	int add = pHero->troops - pHero->soldiers;
	if ( add > 0 )
	{
		HeroInfoConfig *config = hero_getconfig( pHero->kind, pHero->color );
		if ( config )
		{
			int has = city_soldiers( pCity->index, config->corps );
			if ( has > 0 )
			{
				if ( add > has )
					add = has;
				pHero->soldiers += add;
			}
		}
	}

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
int hero_down( int actor_index, int kind, char equip_down )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind >= g_heroinfo_maxnum )
		return -1;
	int heroindex = city_hero_getindex( pCity->index, kind );
	Hero *pHero = city_hero_getptr( pCity->index, HERO_BASEOFFSET + heroindex );
	if ( !pHero )
		return -1;

	// 玩家英雄栏找到一个空位
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
	HeroInfoConfig *config = hero_getconfig( pHero->kind, pHero->color );
	if ( !config )
		return -1;

	// 下阵武将，兵力回营
	if ( heroindex >= 0 && heroindex < 4 )
	{ // 上阵武将
		city_changesoldiers( pCity->index, config->corps, pHero->soldiers, PATH_HERO_DOWN );
	}
	else if( heroindex >= 4 && heroindex < 8 )
	{ // 财赋署武将
		city_changesoldiers( pCity->index, config->corps, pHero->soldiers, PATH_HERO_DOWN );
	}
	else if ( heroindex >= 8 && heroindex < 12 )
	{ // 御林卫武将
		city_changefood( pCity->index, (int)(pHero->soldiers*global.trainfood), PATH_HERO_DOWN );
	}
	pHero->soldiers = 0;
	memset( &g_actors[actor_index].hero[offset], 0, sizeof( Hero ) );
	memcpy( &g_actors[actor_index].hero[offset], pHero, sizeof( Hero ) );
	g_actors[actor_index].hero[offset].offset = offset;
	memset( pHero, 0, sizeof( Hero ) );
	pHero->offset = -1;
	hero_attr_calc( pCity, &g_actors[actor_index].hero[offset] );

	SLK_NetS_HeroReplace pValue = { 0 };
	pValue.m_up_kind = 0;
	pValue.m_down_kind = kind;
	netsend_heroreplace_S( actor_index, SENDTYPE_ACTOR, &pValue );

	// 后面的英雄往前移动
	char send = 0;
	if ( heroindex >= 4 && heroindex < 7 )
	{ // 财赋署武将
		for ( int tmpi = 4; tmpi < 7; tmpi++ )
		{
			if ( pCity->hero[tmpi].kind <= 0 )
			{
				memcpy( &pCity->hero[tmpi], &pCity->hero[tmpi + 1], sizeof( Hero ) );
				pCity->hero[tmpi].offset = HERO_BASEOFFSET + tmpi;
				memset( &pCity->hero[tmpi + 1], 0, sizeof( Hero ) );
				pCity->hero[tmpi + 1].offset = -1;
			}
		}

		// 英雄列表更新
		SLK_NetS_HeroList pValue = { 0 };
		pValue.m_type = 11;
		for ( int tmpi = 4; tmpi < 8; tmpi++ )
		{
			if ( pCity->hero[tmpi].id <= 0 )
				continue;
			hero_makestruct( pCity, HERO_BASEOFFSET + tmpi, &pCity->hero[tmpi], &pValue.m_list[pValue.m_count] );
			pValue.m_count += 1;
		}
		if ( pValue.m_count > 0 )
			netsend_herolist_S( actor_index, SENDTYPE_ACTOR, &pValue );

		// 装备更新
		SLK_NetS_EquipList Value = { 0 };
		Value.m_equipext = g_actors[actor_index].equipext;
		Value.m_count = 0;
		for ( int tmpi = 4; tmpi < 8; tmpi++ )
		{
			if ( pCity->hero[tmpi].kind <= 0 )
				continue;
			for ( int equipoffset = 0; equipoffset < 6; equipoffset++ )
			{
				if ( pCity->hero[tmpi].equip[equipoffset].kind <= 0 )
					continue;
				Value.m_list[Value.m_count].m_offset = pCity->hero[tmpi].equip[equipoffset].offset;
				Value.m_list[Value.m_count].m_kind = pCity->hero[tmpi].equip[equipoffset].kind;
				Value.m_list[Value.m_count].m_washid[0] = pCity->hero[tmpi].equip[equipoffset].washid[0];
				Value.m_list[Value.m_count].m_washid[1] = pCity->hero[tmpi].equip[equipoffset].washid[1];
				Value.m_list[Value.m_count].m_washid[2] = pCity->hero[tmpi].equip[equipoffset].washid[2];
				Value.m_list[Value.m_count].m_washid[3] = pCity->hero[tmpi].equip[equipoffset].washid[3];
				Value.m_count++;
			}
		}
		if ( Value.m_count > 0 )
			netsend_equiplist_S( actor_index, SENDTYPE_ACTOR, &Value );
	}
	else if ( heroindex >= 8 && heroindex < 11 )
	{ // 御林卫武将
		for ( int tmpi = 8; tmpi < 11; tmpi++ )
		{
			if ( pCity->hero[tmpi].kind <= 0 )
			{
				memcpy( &pCity->hero[tmpi], &pCity->hero[tmpi + 1], sizeof( Hero ) );
				pCity->hero[tmpi].offset = HERO_BASEOFFSET + tmpi;
				memset( &pCity->hero[tmpi + 1], 0, sizeof( Hero ) );
				pCity->hero[tmpi + 1].offset = -1;
			}
		}

		// 英雄列表更新
		SLK_NetS_HeroList pValue = { 0 };
		pValue.m_type = 12;
		for ( int tmpi = 8; tmpi < 12; tmpi++ )
		{
			if ( pCity->hero[tmpi].id <= 0 )
				continue;
			hero_makestruct( pCity, HERO_BASEOFFSET + tmpi, &pCity->hero[tmpi], &pValue.m_list[pValue.m_count] );
			pValue.m_count += 1;
		}
		if ( pValue.m_count > 0 )
			netsend_herolist_S( actor_index, SENDTYPE_ACTOR, &pValue );

		// 装备更新
		SLK_NetS_EquipList Value = { 0 };
		Value.m_equipext = g_actors[actor_index].equipext;
		Value.m_count = 0;
		for ( int tmpi = 8; tmpi < 12; tmpi++ )
		{
			if ( pCity->hero[tmpi].kind <= 0 )
				continue;
			for ( int equipoffset = 0; equipoffset < 6; equipoffset++ )
			{
				if ( pCity->hero[tmpi].equip[equipoffset].kind <= 0 )
					continue;
				Value.m_list[Value.m_count].m_offset = pCity->hero[tmpi].equip[equipoffset].offset;
				Value.m_list[Value.m_count].m_kind = pCity->hero[tmpi].equip[equipoffset].kind;
				Value.m_list[Value.m_count].m_washid[0] = pCity->hero[tmpi].equip[equipoffset].washid[0];
				Value.m_list[Value.m_count].m_washid[1] = pCity->hero[tmpi].equip[equipoffset].washid[1];
				Value.m_list[Value.m_count].m_washid[2] = pCity->hero[tmpi].equip[equipoffset].washid[2];
				Value.m_list[Value.m_count].m_washid[3] = pCity->hero[tmpi].equip[equipoffset].washid[3];
				Value.m_count++;
			}
		}
		if ( Value.m_count > 0 )
			netsend_equiplist_S( actor_index, SENDTYPE_ACTOR, &Value );
	}
	
	// 更新英雄信息
	hero_sendinfo( actor_index, &g_actors[actor_index].hero[offset] );

	// 脱装备
	if ( equip_down == 1 )
	{
		equip_down_all( actor_index, kind );
	}
	return 0;
}

// 玩家离线状态下，武将下阵
int hero_down_offline( int city_index, int actorid, int kind, char equip_down )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
	{
		city_index = city_getindex_withactorid( actorid );
	}
	if ( city_index < 0 || city_index >= g_city_maxcount )
	{
		return -1;
	}

	City *pCity = city_indexptr( city_index );
	if ( !pCity )
		return -1;
	memset( &g_temp_actor[0], 0, sizeof( Actor ) );

	// 读取未上阵英雄
	actor_hero_load_auto( actorid, 0, actor_hero_getptr_temp, "actor_hero" );

	// 读取装备数据
	actor_equip_load_auto( actorid, 0, actor_equip_getptr_temp, "actor_equip" );

	int heroindex = city_hero_getindex( city_index, kind );
	Hero *pHero = city_hero_getptr( city_index, HERO_BASEOFFSET + heroindex );
	if ( !pHero )
		return -1;

	// 脱卸装备，返回到装备栏
	if ( equip_down == 1 )
	{
		for ( int index = 0; index < 6; index++ )
		{
			// 获取英雄装备
			Equip *pHeroEquip = &pHero->equip[index];
			if ( pHeroEquip->kind <= 0 )
				continue;
			int oldoffset = pHeroEquip->offset;

			// 找到一个空位
			int free_offset = -1;
			for ( int tmpi = 0; tmpi < MAX_ACTOR_EQUIPNUM; tmpi++ )
			{
				if ( g_temp_actor[0].equip[tmpi].kind <= 0 )
				{
					free_offset = tmpi;
					break;
				}
			}

			if ( free_offset < 0 )
			{ // 装备栏已满
				break;
			}

			Equip *pFreeEquip = &g_temp_actor[0].equip[free_offset];

			// 卸下
			memcpy( pFreeEquip, pHeroEquip, sizeof( Equip ) );
			memset( pHeroEquip, 0, sizeof( Equip ) );
			pFreeEquip->offset = free_offset;
		}
	}

	// 玩家英雄栏找到一个空位
	int offset = -1;
	for ( int tmpi = 0; tmpi < HERO_ACTOR_MAX; tmpi++ )
	{
		if ( g_temp_actor[0].hero[tmpi].id <= 0 )
		{
			offset = tmpi;
			break;
		}
	}
	if ( offset < 0 )
		return -1;
	HeroInfoConfig *config = hero_getconfig( pHero->kind, pHero->color );
	if ( !config )
		return -1;

	// 下阵武将，兵力回营
	if ( heroindex >= 0 && heroindex < 4 )
	{ // 上阵武将
		city_changesoldiers( city_index, config->corps, pHero->soldiers, PATH_HERO_DOWN );
	}
	else if ( heroindex >= 4 && heroindex < 8 )
	{ // 财赋署武将
		city_changesoldiers( city_index, config->corps, pHero->soldiers, PATH_HERO_DOWN );
	}
	else if ( heroindex >= 8 && heroindex < 12 )
	{ // 御林卫武将
		city_changefood( city_index, (int)(pHero->soldiers*global.trainfood), PATH_HERO_DOWN );
	}
	pHero->soldiers = 0;
	memset( &g_temp_actor[0].hero[offset], 0, sizeof( Hero ) );
	memcpy( &g_temp_actor[0].hero[offset], pHero, sizeof( Hero ) );
	g_temp_actor[0].hero[offset].offset = offset;
	memset( pHero, 0, sizeof( Hero ) );
	pHero->offset = -1;

	// 后面的英雄往前移动
	char send = 0;
	if ( heroindex >= 0 && heroindex < 3 )
	{ // 主力武将武将
		for ( int tmpi = 0; tmpi < 3; tmpi++ )
		{
			if ( pCity->hero[tmpi].kind <= 0 )
			{
				memcpy( &pCity->hero[tmpi], &pCity->hero[tmpi + 1], sizeof( Hero ) );
				pCity->hero[tmpi].offset = HERO_BASEOFFSET + tmpi;
				memset( &pCity->hero[tmpi + 1], 0, sizeof( Hero ) );
				pCity->hero[tmpi + 1].offset = -1;
			}
		}
	}
	else if ( heroindex >= 4 && heroindex < 7 )
	{ // 财赋署武将
		for ( int tmpi = 4; tmpi < 7; tmpi++ )
		{
			if ( pCity->hero[tmpi].kind <= 0 )
			{
				memcpy( &pCity->hero[tmpi], &pCity->hero[tmpi + 1], sizeof( Hero ) );
				pCity->hero[tmpi].offset = HERO_BASEOFFSET + tmpi;
				memset( &pCity->hero[tmpi + 1], 0, sizeof( Hero ) );
				pCity->hero[tmpi + 1].offset = -1;
			}
		}
	}
	else if ( heroindex >= 8 && heroindex < 11 )
	{ // 御林卫武将
		for ( int tmpi = 8; tmpi < 11; tmpi++ )
		{
			if ( pCity->hero[tmpi].kind <= 0 )
			{
				memcpy( &pCity->hero[tmpi], &pCity->hero[tmpi + 1], sizeof( Hero ) );
				pCity->hero[tmpi].offset = HERO_BASEOFFSET + tmpi;
				memset( &pCity->hero[tmpi + 1], 0, sizeof( Hero ) );
				pCity->hero[tmpi + 1].offset = -1;
			}
		}
	}

	// 重算装备战力
	city_battlepower_equip_calc( pCity );
	// 重算英雄战力
	city_battlepower_hero_calc( pCity );

	// 存档
	// 未上阵英雄保存
	actor_hero_batch_save_auto( g_temp_actor[0].hero, HERO_ACTOR_MAX, "actor_hero", NULL );
	
	// 未上阵英雄装备保存
	for ( int tmpi = 0; tmpi < HERO_ACTOR_MAX; tmpi++ )
	{
		if ( g_temp_actor[0].hero[tmpi].id <= 0 )
			continue;
		actor_equip_batch_save_auto( g_temp_actor[0].hero[tmpi].equip, EQUIP_TYPE_MAX, "actor_equip", NULL );
	}

	// 装备栏
	actor_equip_batch_save_auto( g_temp_actor[0].equip, MAX_ACTOR_EQUIPNUM, "actor_equip", NULL );

	// 英雄
	actor_hero_batch_save_auto( pCity->hero, HERO_CITY_MAX, "actor_hero", NULL );

	// 装备
	for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind <= 0 )
			continue;
		actor_equip_batch_save_auto( pCity->hero[tmpi].equip, EQUIP_TYPE_MAX, "actor_equip",  NULL );
	}
	return 0;
}

// 武将顺序
int hero_guard_sort( int actor_index, SLK_NetC_HeroGuardSort *list )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
 	
	for ( int i = 0; i < 4; i++ )
	{
		short kind = list->m_herokind[i];
		if ( kind <= 0 )
			continue;

		for ( int tmpi = 8; tmpi < 12; tmpi++ )
		{
			if ( pCity->hero[tmpi].kind == kind )
			{
				if ( i + 8 == tmpi )
					continue;

				Hero pTmp = {0};
				memcpy( &pTmp, &pCity->hero[8 + i], sizeof( Hero ) );
				memcpy( &pCity->hero[8 + i], &pCity->hero[tmpi], sizeof( Hero ) );
				memcpy( &pCity->hero[tmpi], &pTmp, sizeof( Hero ) );

				pCity->hero[tmpi].offset = HERO_BASEOFFSET + tmpi;
			}
			if ( pCity->hero[tmpi].kind > 0 )
			{
				pCity->hero[tmpi].offset = HERO_BASEOFFSET + tmpi;
			}
		}
	}


	// 英雄列表更新
	SLK_NetS_HeroList pValue = { 0 };
	pValue.m_type = 12;
	for ( int tmpi = 8; tmpi < 12; tmpi++ )
	{
		if ( pCity->hero[tmpi].id <= 0 )
			continue;
		hero_makestruct( pCity, HERO_BASEOFFSET + tmpi, &pCity->hero[tmpi], &pValue.m_list[pValue.m_count] );
		pValue.m_count += 1;
	}
	if ( pValue.m_count > 0 )
		netsend_herolist_S( actor_index, SENDTYPE_ACTOR, &pValue );

	// 装备更新
	SLK_NetS_EquipList Value = { 0 };
	Value.m_equipext = g_actors[actor_index].equipext;
	Value.m_count = 0;
	for ( int tmpi = 8; tmpi < 12; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind <= 0 )
			continue;
		for ( int equipoffset = 0; equipoffset < 6; equipoffset++ )
		{
			if ( pCity->hero[tmpi].equip[equipoffset].kind <= 0 )
				continue;
			Value.m_list[Value.m_count].m_offset = pCity->hero[tmpi].equip[equipoffset].offset;
			Value.m_list[Value.m_count].m_kind = pCity->hero[tmpi].equip[equipoffset].kind;
			Value.m_list[Value.m_count].m_washid[0] = pCity->hero[tmpi].equip[equipoffset].washid[0];
			Value.m_list[Value.m_count].m_washid[1] = pCity->hero[tmpi].equip[equipoffset].washid[1];
			Value.m_list[Value.m_count].m_washid[2] = pCity->hero[tmpi].equip[equipoffset].washid[2];
			Value.m_list[Value.m_count].m_washid[3] = pCity->hero[tmpi].equip[equipoffset].washid[3];
			Value.m_count++;
		}
	}
	if ( Value.m_count > 0 )
		netsend_equiplist_S( actor_index, SENDTYPE_ACTOR, &Value );

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
		hero_sendinfo( pCity->actor_index, pHero );
	}

	nation_hero_attrupdate( pHero );
	return 0;
}

// 加兵力
int hero_addsoldiers( int actor_index, int herokind, char path )
{
	Hero *pHero = NULL;
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	pHero = hero_getptr( actor_index, herokind );
	if ( !pHero )
		return -1;
	if ( path != PATH_KINGWAR_REBIRTH )
	{
		if ( pHero->state != HERO_STATE_NORMAL )
		{
			POP( actor_index, 845 );
			return -1;
		}
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
	pValue.m_path = path;
	netsend_herosoldiers_S( actor_index, SENDTYPE_ACTOR, &pValue );

	city_changesoldiers( pCity->index, config->corps, -add, path );
	nation_hero_attrupdate( pHero );
	return pHero->soldiers;
}

// 自动补兵
int hero_addsoldiers_audo( City *pCity )
{
	if ( !pCity )
		return -1;
	if ( city_function_check( pCity, CITY_FUNCTION_AUTO_ADDHP ) == 0 )
		return -1;
	for ( int i = 0; i < 8; i++ )
	{
		if ( pCity->hero[i].kind <= 0 )
			continue;
		if ( pCity->hero[i].state != HERO_STATE_NORMAL )
			continue;
		if ( pCity->hero[i].soldiers >= pCity->hero[i].troops )
			continue;
		HeroInfoConfig *config = hero_getconfig( pCity->hero[i].kind, pCity->hero[i].color );
		if ( !config )
			continue;
		int troops = pCity->hero[i].troops;
		int add = troops - pCity->hero[i].soldiers;
		int has = city_soldiers( pCity->index, config->corps );
		if ( has > 0 )
		{
			if ( add > has )
				add = has;
			hero_changesoldiers( pCity, &pCity->hero[i], add, PATH_BATTLEAUTOHP );
			city_changesoldiers( pCity->index, config->corps, -add, PATH_BATTLEAUTOHP );
		}
	}
	return 0;
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
	nation_hero_attrupdate( pHero );
	return 0;
}

// 御林卫自动补充士兵
void hero_guard_soldiers_auto( City *pCity )
{
	if ( !pCity )
		return;
	char foodchange = 0;
	int foodtotal = 0;
	for ( int i = 8; i < 12; i++ )
	{// 御林卫恢复兵力
		if ( pCity->hero[i].kind <= 0 )
			continue;
		if ( pCity->hero[i].soldiers < pCity->hero[i].troops )
		{
			int soldiers = (int)ceil( pCity->hero[i].troops*global.hero_cabinet_guard_hp / 100.0f );
			int food = (int)(soldiers * global.trainfood);
			if ( pCity->food < food )
				continue;
			pCity->hero[i].soldiers += soldiers;
			if ( pCity->hero[i].soldiers > pCity->hero[i].troops )
				pCity->hero[i].soldiers = pCity->hero[i].troops;
			if ( pCity->actor_index >= 0 )
			{
				SLK_NetS_HeroSoldiers pValue = { 0 };
				pValue.m_kind = pCity->hero[i].kind;
				pValue.m_add = soldiers;
				pValue.m_soldiers = pCity->hero[i].soldiers;
				pValue.m_soldiers_max = pCity->hero[i].troops;
				pValue.m_path = PATH_HERO_GUARD_AUTO;
				netsend_herosoldiers_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
			}
			nation_hero_attrupdate( &pCity->hero[i] );
			pCity->food -= food;
			foodtotal -= food;
			foodchange = 1;
		}
	}
	if ( foodchange == 1 && pCity->actor_index >= 0 )
	{
		SLK_NetS_Food pValue = { 0 };
		pValue.m_add = foodtotal;
		pValue.m_total = pCity->food;
		pValue.m_path = PATH_HERO_GUARD_AUTO;
		netsend_changefood_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
	}
}

// 御林卫钻石补充士兵
int hero_guard_soldiers_token( int actor_index, int herokind )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Hero *pHero = hero_getptr( actor_index, herokind );
	if ( !pHero )
		return -1;
	int soldiers = pHero->troops - pHero->soldiers;
	int progress = (int)ceil( soldiers / (float)pHero->troops * 100.0f );
	int token = (int)ceil( progress * (float)global.hero_cabinet_guard_token );
	if ( g_actors[actor_index].token < token )
		return -1;
	actor_change_token( actor_index, -token, PATH_HERO_GUARD, 0 );
	pHero->soldiers = pHero->troops;
	SLK_NetS_HeroSoldiers pValue = { 0 };
	pValue.m_kind = pHero->kind;
	pValue.m_add = soldiers;
	pValue.m_soldiers = pHero->soldiers;
	pValue.m_soldiers_max = pHero->troops;
	pValue.m_path = PATH_HERO_GUARD;
	netsend_herosoldiers_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
	nation_hero_attrupdate( pHero );
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
inline char hero_getline( City *pCity, char state )
{
	if ( !pCity )
		return 2;
	if ( state == HERO_STATE_NORMAL )
	{
		return 2 + pCity->attr.hero_row_num;
	}
	else if ( state == HERO_STATE_FIGHT || state == HERO_STATE_KINGWAR )
	{
		return 2 + pCity->attr.hero_row_num + pCity->attr.hero_row_fight;
	}
	else if ( state == HERO_STATE_GATHER )
	{
		return 2 + pCity->attr.hero_row_num + pCity->attr.hero_row_gather;
	}
	return 2;
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

	// 科技附加
	int tech_attack = pCity->attr.hero_attack[config->corps];
	int tech_defense = pCity->attr.hero_defense[config->corps];
	int tech_troops = pCity->attr.hero_troops[config->corps];

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

	// 国器附加
	int nequip_attack = 0;
	int nequip_defense = 0;
	int nequip_troops = 0;
	for ( int type = 1; type < g_nationequip_maxnum; type++ )
	{
		int level = pCity->neq_lv[type - 1];
		if ( level <= 0 || level >= g_nationequip[type].maxnum )
			continue;
		nequip_attack += g_nationequip[type].config[level].attack;
		nequip_defense += g_nationequip[type].config[level].defense;
		nequip_troops += g_nationequip[type].config[level].troops;
	}

	// 爵位加成
	int place_attack = 0;
	if ( pCity->place >= 0 && pCity->place < g_nationequip_maxnum - 1 )
	{
		place_attack = g_nation_place[pCity->place + 1].value;
	}

	// BUFF加成
	int buff_attack = 0;
	int buff_defense = 0;
	if ( pHero->offset >= HERO_BASEOFFSET )
	{
		if ( pCity->buffsec[CITY_BUFF_FIRE] > 0 )
		{
			buff_attack = (int)(base_attack * global.buff_fire / 100.0f);
		}
		if ( pCity->buffsec[CITY_BUFF_MOUNTAIN] > 0 )
		{
			buff_defense = (int)(base_defense * global.buff_mountain / 100.0f);
		}
	}

	// 神突加成
	int god_troops = 0;
	int god_attack = 0;
	int god_defense = 0;
	if ( pHero->god == 1 )
	{
		god_troops = global.hero_god_troops;
		god_attack = global.hero_god_attack;
		god_defense = global.hero_god_defense;
	}

	// 综合计算
	pHero->attack = base_attack + tech_attack + equip_attack + nequip_attack + place_attack + buff_attack + god_attack;
	pHero->defense = base_defense + tech_defense + equip_defense + nequip_defense + buff_defense + god_defense;
	pHero->attack_increase = equip_attack_increase;
	pHero->defense_increase = equip_defense_increase;
	pHero->assault = equip_assault;
	pHero->defend = equip_defend;

	// 顺路计算这个英雄的单独战力
	float bp_attack = base_attack * global.battlepower_attack;
	float bp_defense = base_defense * global.battlepower_defense;
	float bp_troops = 0;

	// 兵力特殊，需要分类计算
	int hero_troops = base_troops + (tech_troops + equip_troops + nequip_troops + god_troops) / 4;
	if ( pHero->offset < HERO_BASEOFFSET )
	{ // 无职务武将
		pHero->troops = hero_troops * hero_getline( pCity, HERO_STATE_NORMAL );
	}
	else if ( pHero->offset >= HERO_BASEOFFSET && pHero->offset < HERO_BASEOFFSET + 4 )
	{ // 上阵武将
		pHero->troops = hero_troops * hero_getline( pCity, HERO_STATE_FIGHT );
	}
	else if ( pHero->offset >= HERO_BASEOFFSET + 4 && pHero->offset < HERO_BASEOFFSET + 8 )
	{ // 财富署武将
		pHero->troops = hero_troops * hero_getline( pCity, HERO_STATE_GATHER );
	}
	else if ( pHero->offset >= HERO_BASEOFFSET + 8 && pHero->offset < HERO_BASEOFFSET + 12 )
	{ // 御林卫武将
		pHero->troops = hero_troops * hero_getline( pCity, HERO_STATE_NORMAL );
	}

	// 顺路计算这个英雄的单独战力
	if ( pHero->offset >= HERO_BASEOFFSET && pHero->offset < HERO_BASEOFFSET + 4 )
	{
		// 英雄基础属性战力
		float bp_attack = base_attack * global.battlepower_attack;
		float bp_defense = base_defense * global.battlepower_defense;
		float bp_troops = base_troops * hero_getline( pCity, HERO_STATE_FIGHT ) * global.battlepower_troops;
		pHero->bp_hero = (int)ceil( bp_attack + bp_defense + bp_troops );

		// 身上装备战力
		float bp_equip_attack = equip_attack * global.battlepower_attack;
		float bp_equip_defense = equip_defense * global.battlepower_defense;
		float bp_equip_attack_increase = equip_attack_increase * global.battlepower_attack_increase;
		float bp_equip_defense_increase = equip_defense_increase * global.battlepower_defense_increase;
		float bp_equip_assault = equip_assault * global.battlepower_assault;
		float bp_equip_defend = equip_defend * global.battlepower_defend;
		float bp_equip_troops = (equip_troops / 4) * hero_getline( pCity, HERO_STATE_FIGHT ) * global.battlepower_troops;
		pHero->bp_equip = (int)ceil( bp_equip_attack + bp_equip_defense + bp_equip_attack_increase + bp_equip_defense_increase + bp_equip_assault + bp_equip_defend + bp_equip_troops );

		// 国器提供战力
		float bp_nequip_attack = nequip_attack * global.battlepower_attack;
		float bp_nequip_defense = nequip_defense * global.battlepower_defense;
		float bp_nequip_troops = (nequip_troops / 4) * hero_getline( pCity, HERO_STATE_FIGHT ) * global.battlepower_troops;
		pHero->bp_nequip = (int)ceil( bp_nequip_attack + bp_nequip_defense + bp_nequip_troops );

		// 科技提供战力
		float bp_tech_attack = tech_attack * global.battlepower_attack;
		float bp_tech_defense = tech_defense * global.battlepower_defense;
		float bp_tech_troops = (tech_troops / 4) * hero_getline( pCity, HERO_STATE_FIGHT ) * global.battlepower_troops;
		pHero->bp_tech = (int)ceil( bp_tech_attack + bp_tech_defense + bp_tech_troops );
	}
	nation_hero_attrupdate( pHero );
	return 0;
}
int hero_attr_calc_all( City *pCity, char path )
{
	// 计算所有上阵英雄属性
	for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind <= 0 )
			continue;
		hero_attr_calc( pCity, &pCity->hero[tmpi] );
		hero_sendinfo( pCity->actor_index, &pCity->hero[tmpi] );
	}

	if ( path == 1 )
	{
		// 计未上阵英雄属性
		int actor_index = pCity->actor_index;
		if ( actor_index >= 0 && actor_index < g_maxactornum )
		{
			for ( int tmpi = 0; tmpi < HERO_ACTOR_MAX; tmpi++ )
			{
				if ( g_actors[actor_index].hero[tmpi].kind <= 0 )
					continue;
				hero_attr_calc( pCity, &g_actors[actor_index].hero[tmpi] );
				hero_sendinfo( actor_index, &g_actors[actor_index].hero[tmpi] );
			}
		}
	}
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
	pValue->m_god = pHero->god;
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

	// 国家名将信息
	nation_hero_citylist( actor_index );

	// 上阵英雄
	{
		SLK_NetS_HeroList pValue = { 0 };
		pValue.m_type = 0;
		for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
		{
			if ( pCity->hero[tmpi].id <= 0 )
				continue;
			hero_makestruct( pCity, HERO_BASEOFFSET + tmpi, &pCity->hero[tmpi], &pValue.m_list[pValue.m_count] );
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
	
	char oldcolor = pHero->color;
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

		// 突破增加寻访进度
		if ( oldcolor == 2 )
			hero_visit_setprogress( actor_index, global.hero_visit_progress_color1 );
		else if ( oldcolor == 3 )
			hero_visit_setprogress( actor_index, global.hero_visit_progress_color2 );
		else if ( oldcolor == 4 )
			hero_visit_setprogress( actor_index, global.hero_visit_progress_color3 );
		else if ( oldcolor == 5 )
			hero_visit_setprogress( actor_index, global.hero_visit_progress_color4 );
	}

	// 更新客户端
	SLK_NetS_HeroColorup pValue = { 0 };
	pValue.m_value = pHero->colorup;
	pValue.m_isup = isup;
	netsend_herocolorup_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 神级突破
static short s_hero_godup_equip[6] = { 5, 15, 25, 35, 45, 55 };
static short s_hero_godup_washid[6] = { 15, 15, 25, 25, 75, 75 };
int hero_godup( int actor_index, int herokind )
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
	if ( pHero->level < global.hero_god_level )
		return -1;
	if ( pHero->color < ITEM_COLOR_LEVEL_PURPLE )
		return -1;
	if ( pHero->god == 1 )
		return -1;
	char addequip_offset[6] = { -1, -1, -1, -1, -1, -1 };
	
	// 收集装备
	for ( int index = 0; index < 6; index++ )
	{
		for ( int tmpi = 0; tmpi < MAX_ACTOR_EQUIPNUM; tmpi++ )
		{
			if ( g_actors[actor_index].equip[tmpi].kind <= 0 )
				continue;
			if ( g_actors[actor_index].equip[tmpi].kind == s_hero_godup_equip[index] )
			{
				if ( g_actors[actor_index].equip[tmpi].washid[0] == s_hero_godup_washid[index] && 
					 g_actors[actor_index].equip[tmpi].washid[1] == s_hero_godup_washid[index] &&
					 g_actors[actor_index].equip[tmpi].washid[2] == s_hero_godup_washid[index] &&
					 g_actors[actor_index].equip[tmpi].washid[3] == s_hero_godup_washid[index] )
				{
					addequip_offset[index] = tmpi;
					break;
				}
			}
		}
	}

	// 检查是否齐全
	for ( int index = 0; index < 6; index++ )
	{
		if ( addequip_offset[index] < 0 )
			return -1;
	}

	// 扣除装备
	for ( int index = 0; index < 6; index++ )
	{
		equip_lostequip( actor_index, addequip_offset[index], PATH_HERO_GOD );
	}

	pHero->god = 1;
	// 重算英雄属性
	hero_attr_calc( pCity, pHero );
	// 重算战力
	city_battlepower_hero_calc( pCity );
	// 更新英雄
	hero_sendinfo( actor_index, pHero );
	// 通知客户端突破成功
	int value[2] = { 0 };
	value[0] = 0;
	value[1] = pHero->kind;
	actor_notify_value( actor_index, NOTIFY_HEROGOD, 2, value, NULL );

	// 公告
	char v1[32] = { 0 };
	char v2[32] = { 0 };
	char v3[32] = { 0 };
	sprintf( v1, "%s%d", TAG_NATION, pCity->nation );
	sprintf( v2, "%s", pCity->name );
	sprintf( v3, "%s%d", TAG_HERO, pHero->kind );
	system_talkjson_world( 6026, v1, v2, v3, NULL, NULL, NULL, 1 );
	return 0;
}

// 初始化
int hero_visit_init()
{
	memset( g_herovisit_awardgroup, 0, sizeof( HeroVisitAwardGroup )*HERO_VISIT_AWARDGROUPMAX );
	for ( int tmpi = 0; tmpi < g_hero_visit_maxnum; tmpi++ )
	{
		if ( g_hero_visit[tmpi].awardgroup <= 0 )
			continue;
		if ( g_hero_visit[tmpi].awardgroup >= HERO_VISIT_AWARDGROUPMAX )
			return -1;
		int awardgroup = g_hero_visit[tmpi].awardgroup;
		int count = g_herovisit_awardgroup[awardgroup].allcount;
		if ( count < 0 || count >= HERO_VISIT_AWARDMAX )
		{
			return -1;
		}
		g_herovisit_awardgroup[awardgroup].award[count].id = g_hero_visit[tmpi].id;
		g_herovisit_awardgroup[awardgroup].award[count].kind = g_hero_visit[tmpi].kind;
		g_herovisit_awardgroup[awardgroup].award[count].num = g_hero_visit[tmpi].num;
		g_herovisit_awardgroup[awardgroup].award[count].color = (char)g_hero_visit[tmpi].color;
		g_herovisit_awardgroup[awardgroup].award[count].value = g_hero_visit[tmpi].value;
		g_herovisit_awardgroup[awardgroup].allvalue += g_hero_visit[tmpi].value;
		g_herovisit_awardgroup[awardgroup].allcount += 1;
	}
	return 0;
}

// 设置寻访进度
int hero_visit_setprogress( int actor_index, int value )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->level < global.hero_visit_actorlevel )
		return -1;
	if ( city_mainlevel( pCity->index ) < global.hero_visit_mainlevel )
		return -1;
	if ( pCity->hv_hsec > 0 )
		return -1;// 已经开启
	g_actors[actor_index].hv_pro += value;
	if ( g_actors[actor_index].hv_pro >= global.hero_visit_progress_max )
	{ // 开启神将寻访
		g_actors[actor_index].hv_pro = 0;
		pCity->hv_hsec = global.hero_visit_high_sec;
		hero_visit_snedflag( actor_index );
	}
	return 0;
}

// 寻访信息
int hero_visit_sendinfo( int actor_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_HeroVisitInfo pValue = { 0 };
	pValue.m_hv_free_cd = pCity->hv_fcd;
	pValue.m_hv_high_sec = pCity->hv_hsec;
	pValue.m_hv_high_free = pCity->hv_hf;
	pValue.m_hv_low_num = g_actors[actor_index].hv_ln;
	pValue.m_hv_high_num = g_actors[actor_index].hv_hn;
	pValue.m_hv_progress = g_actors[actor_index].hv_pro;
	netsend_herovisitinfo_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 发送可寻访标记
int hero_visit_snedflag( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->level < global.hero_visit_actorlevel )
		return -1;
	if ( city_mainlevel( pCity->index ) < global.hero_visit_mainlevel )
		return -1;

	int value[2] = { 0 };
	value[0] = 0;
	if ( pCity->hv_fcd <= 0 )
	{ // 有免费次数
		value[1] = 1;
	}
	else if ( pCity->hv_hsec > 0 )
	{ // 有神将寻访
		value[1] = 2;
	}
	else
	{
		value[1] = 0;
	}
	actor_notify_value( pCity->actor_index, NOTIFY_HERO_VISIT, 2, value, NULL );
	return 0;
}

// 获取寻访池奖励
int hero_visit_getaward( int awardgroup, AwardGetInfo *getinfo, SLK_NetS_HeroVisitAward *netAward )
{
	if ( awardgroup <= 0 || awardgroup >= HERO_VISIT_AWARDGROUPMAX )
		return -1;
	if ( !getinfo )
		return -1;
	// 然后按照评价值随机
	int allvalue = g_herovisit_awardgroup[awardgroup].allvalue;
	int curvalue = 0;
	int odds = allvalue > 0 ? rand() % allvalue : 0;
	for ( int tmpi = 0; tmpi < g_herovisit_awardgroup[awardgroup].allcount; tmpi++ )
	{
		// 按照评价值方式随机
		curvalue = g_herovisit_awardgroup[awardgroup].award[tmpi].value;
		if ( curvalue > 0 && curvalue > odds )
		{
			int kind = g_herovisit_awardgroup[awardgroup].award[tmpi].kind;
			int num = g_herovisit_awardgroup[awardgroup].award[tmpi].num;
			char color = g_herovisit_awardgroup[awardgroup].award[tmpi].color;

			if ( kind < 0 )
				hero_visit_getaward( -kind, getinfo, netAward );
			else if ( kind > 0 )
			{
				if (  getinfo->count < AWARDGETINFO_MAXCOUNT )
				{
					getinfo->kind[getinfo->count] = kind;
					getinfo->num[getinfo->count] = num;
					getinfo->color[getinfo->count] = color;
					getinfo->count++;

					if ( netAward->m_count < 10 )
					{
						netAward->m_id[netAward->m_count] = g_herovisit_awardgroup[awardgroup].award[tmpi].id;
						netAward->m_count++;
					}
				}
				if ( getinfo->count >= AWARDGETINFO_MAXCOUNT )
					break;
			}
			break;
		}
		odds -= curvalue;
	}
	return 0;
}

// 良将寻访
int hero_visit_low( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	// 免费次数检查
	char costtype = 1;
	if ( pCity->hv_fcd > 0 )
	{
		if ( item_getitemnum( actor_index, 484 ) < global.hero_visit_low_itemnum )
		{
			if ( g_actors[actor_index].token < global.hero_visit_low_token )
				return -1;
			costtype = 2;
		}
	}
	
	// 抽取计数
	g_actors[actor_index].hv_ln += 1;

	SLK_NetS_HeroVisitAward netAward = { 0 };
	AwardGetInfo getinfo = { 0 };
	if ( g_actors[actor_index].hv_ln >= global.hero_visit_low_max )
	{ // 特殊抽取
		g_actors[actor_index].hv_ln = 0;
		hero_visit_getaward( global.hero_visit_low_hero_award, &getinfo, &netAward );
	}
	else
	{ // 普通抽取
		hero_visit_getaward( global.hero_visit_low_normal_award, &getinfo, &netAward );
	}

	// 给奖励
	for ( int tmpi = 0; tmpi < getinfo.count; tmpi++ )
	{
		award_getaward( actor_index, getinfo.kind[tmpi], getinfo.num[tmpi], -1, PATH_HEROVISIT, NULL );
	}
	// 发送获取的奖励ID
	netsend_herovisitaward_S( actor_index, SENDTYPE_ACTOR, &netAward );

	if ( pCity->hv_fcd > 0 )
	{
		if ( costtype == 1 )
			item_lost( actor_index, 484, global.hero_visit_low_itemnum, PATH_HEROVISIT );
		else
			actor_change_token( actor_index, -global.hero_visit_low_token, PATH_HEROVISIT, 0 );
	}
	else
	{
		pCity->hv_fcd = global.hero_visit_free_cd;
		hero_visit_snedflag( actor_index );
	}
	hero_visit_setprogress( actor_index, global.hero_visit_progress_normal );
	hero_visit_sendinfo( actor_index );
	return 0;
}

// 良将寻访10连
int hero_visit_low10( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	if ( g_actors[actor_index].token < global.hero_visit_low_token10 )
		return -1;

	SLK_NetS_HeroVisitAward netAward = { 0 };
	AwardGetInfo getinfo = { 0 };
	for ( int tmpi = 0; tmpi < 10; tmpi++ )
	{
		// 抽取计数
		g_actors[actor_index].hv_ln += 1;

		if ( g_actors[actor_index].hv_ln >= global.hero_visit_low_max )
		{ // 特殊抽取
			g_actors[actor_index].hv_ln = 0;
			hero_visit_getaward( global.hero_visit_low_hero_award, &getinfo, &netAward );
		}
		else
		{ // 普通抽取
			hero_visit_getaward( global.hero_visit_low_normal_award, &getinfo, &netAward );
		}
	}

	// 给奖励
	for ( int tmpi = 0; tmpi < getinfo.count; tmpi++ )
	{
		award_getaward( actor_index, getinfo.kind[tmpi], getinfo.num[tmpi], -1, PATH_HEROVISIT, NULL );
	}
	// 发送获取的奖励ID
	netsend_herovisitaward_S( actor_index, SENDTYPE_ACTOR, &netAward );
	actor_change_token( actor_index, -global.hero_visit_low_token10, PATH_HEROVISIT, 0 );
	hero_visit_setprogress( actor_index, global.hero_visit_progress_normal*10 );
	hero_visit_sendinfo( actor_index );
	return 0;
}

// 神将寻访
int hero_visit_high( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	// 免费次数检查
	char costtype = 1;
	if ( pCity->hv_hf <= 0 )
	{
		if ( item_getitemnum( actor_index, 485 ) < global.hero_visit_high_itemnum )
		{
			if ( g_actors[actor_index].token < global.hero_visit_high_token )
				return -1;
			costtype = 2;
		}
	}

	// 抽取计数
	g_actors[actor_index].hv_hn += 1;

	SLK_NetS_HeroVisitAward netAward = { 0 };
	AwardGetInfo getinfo = { 0 };
	if ( g_actors[actor_index].hv_hn >= global.hero_visit_high_max )
	{ // 特殊抽取
		g_actors[actor_index].hv_hn = 0;
		hero_visit_getaward( global.hero_visit_high_hero_award, &getinfo, &netAward );
	}
	else
	{ // 普通抽取
		hero_visit_getaward( global.hero_visit_high_normal_award, &getinfo, &netAward );
	}

	// 给奖励
	for ( int tmpi = 0; tmpi < getinfo.count; tmpi++ )
	{
		award_getaward( actor_index, getinfo.kind[tmpi], getinfo.num[tmpi], -1, PATH_HEROVISIT, NULL );

		if ( getinfo.kind[tmpi] > AWARDKIND_HEROBASE && getinfo.kind[tmpi] < AWARDKIND_BUILDINGBASE )
		{ // 武将公告
			char v1[32] = { 0 };
			char v2[32] = { 0 };
			char v3[32] = { 0 };
			sprintf( v1, "%s%d", TAG_NATION, pCity->nation );
			sprintf( v2, "%s", pCity->name );
			sprintf( v3, "%s%d", TAG_HERO, getinfo.kind[tmpi] - AWARDKIND_HEROBASE );
			system_talkjson_world( 6027, v1, v2, v3, NULL, NULL, NULL, 1 );
		}
	}
	// 发送获取的奖励ID
	netsend_herovisitaward_S( actor_index, SENDTYPE_ACTOR, &netAward );

	if ( pCity->hv_hf <= 0 )
	{
		if ( costtype == 1 )
			item_lost( actor_index, 485, global.hero_visit_high_itemnum, PATH_HEROVISIT );
		else
			actor_change_token( actor_index, -global.hero_visit_high_token, PATH_HEROVISIT, 0 );
	}
	else
	{
		pCity->hv_hf -= 1;
		hero_visit_snedflag( actor_index );
	}
	hero_visit_sendinfo( actor_index );
	return 0;
}

// 神将寻访10连
int hero_visit_high10( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	
	if ( g_actors[actor_index].token < global.hero_visit_high_token10 )
		return -1;

	SLK_NetS_HeroVisitAward netAward = { 0 };
	AwardGetInfo getinfo = { 0 };
	for ( int tmpi = 0; tmpi < 10; tmpi++ )
	{
		// 抽取计数
		g_actors[actor_index].hv_hn += 1;

		if ( g_actors[actor_index].hv_hn >= global.hero_visit_high_max )
		{ // 特殊抽取
			g_actors[actor_index].hv_hn = 0;
			hero_visit_getaward( global.hero_visit_high_hero_award, &getinfo, &netAward );
		}
		else
		{ // 普通抽取
			hero_visit_getaward( global.hero_visit_high_normal_award, &getinfo, &netAward );
		}
	}

	// 给奖励
	for ( int tmpi = 0; tmpi < getinfo.count; tmpi++ )
	{
		award_getaward( actor_index, getinfo.kind[tmpi], getinfo.num[tmpi], -1, PATH_HEROVISIT, NULL );

		if ( getinfo.kind[tmpi] > AWARDKIND_HEROBASE && getinfo.kind[tmpi] < AWARDKIND_BUILDINGBASE )
		{ // 武将公告
			char v1[32] = { 0 };
			char v2[32] = { 0 };
			char v3[32] = { 0 };
			sprintf( v1, "%s%d", TAG_NATION, pCity->nation );
			sprintf( v2, "%s", pCity->name );
			sprintf( v3, "%s%d", TAG_HERO, getinfo.kind[tmpi] - AWARDKIND_HEROBASE );
			system_talkjson_world( 6027, v1, v2, v3, NULL, NULL, NULL, 1 );
		}
	}
	// 发送获取的奖励ID
	netsend_herovisitaward_S( actor_index, SENDTYPE_ACTOR, &netAward );

	if ( pCity->hv_hf <= 0 )
	{
		actor_change_token( actor_index, -global.hero_visit_high_token10, PATH_HEROVISIT, 0 );
	}
	else
	{
		pCity->hv_hf -= 1;
		hero_visit_snedflag( actor_index );
	}
	hero_visit_sendinfo( actor_index );
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

// 获取全部武将
int hero_gm_getallhero( City *pCity )
{
	if ( !pCity )
		return -1;
	
	for ( int kind = 1; kind < g_heroinfo_maxnum; kind++ )
	{
		hero_gethero( pCity->actor_index, kind, PATH_GM );
	}
	return 0;
}
