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
#include "nation.h"
#include "nation_equip.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern City * g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern TechInfo *g_techinfo;
extern int g_techinfo_maxnum;

extern NationEquipOpen *g_nequip_open;
extern int g_nequip_open_maxnum;

extern NationEquipInfo *g_nationequip;
extern int g_nationequip_maxnum;

extern NationEquipRemake *g_nequip_remake;
extern int g_nequip_remake_maxnum;

// 开启
int nation_equip_open( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind >= g_nequip_open_maxnum )
		return -1;
	int index = kind - 1;
	int level = pCity->neq_lv[index];
	if ( level > 0 )
		return -1;
	if ( pCity->level < g_nequip_open[kind].actorlevel )
		return -1;
	if ( pCity->silver < g_nequip_open[kind].silver )
		return -1;
	if ( g_nequip_open[kind].itemkind > 0 )
	{
		if ( item_lost( actor_index, g_nequip_open[kind].itemkind, g_nequip_open[kind].itemnum, PATH_NATIONEQUIP ) < 0 )
			return -1;
	}
	city_changesilver( pCity->index, -g_nequip_open[kind].silver, PATH_NATIONEQUIP );
	pCity->neq_sec[index] = g_nequip_open[kind].sec; 
	g_actors[actor_index].neq_crit[index] = 0;
	nation_equip_sendinfo( actor_index, kind );
	return 0;
}

// 升级
int nation_equip_upgrade( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind >= g_nationequip_maxnum )
		return -1;
	int index = kind - 1;
	int level = pCity->neq_lv[index];
	if ( level == 0 )
		return -1;
	if ( pCity->neq_sec[index] > 0 )
		return -1;

	// 满级了
	if ( level >= g_nationequip[kind].maxnum - 1 )
		return -1;

	// 需要改造了
	int star = g_nationequip[kind].config[level].remake_star;
	if ( star > 0 )
	{
		if ( pCity->neq_star[index] < star )
			return -1;
	}

	if ( pCity->iron < g_nationequip[kind].config[level].iron )
		return -1;
	city_changeiron( pCity->index, -g_nationequip[kind].config[level].iron, PATH_NATIONEQUIP );

	// 原始经验
	int exp = pCity->neq_exp[index];

	// 增加经验
	int addexp = g_nationequip[kind].config[level].exp;
	if ( g_actors[actor_index].neq_crit[index] > 0 )
	{
		addexp *= g_actors[actor_index].neq_crit[index];
		g_actors[actor_index].neq_crit[index] = 0;
	}

	if ( exp + addexp >= 100 )
	{
		pCity->neq_exp[index] = exp + addexp - 100;
		pCity->neq_lv[index] += 1;
	}
	else
	{
		pCity->neq_exp[index] += addexp;
	}
	
	// 下一轮国器暴击科技等级
	int oddsvalue[5] = { 0 };
	int count = 0;
	int totalvalue = 0;
	int techlevel = pCity->techlevel[18];
	if ( techlevel > 0 && techlevel < g_techinfo[18].maxnum )
	{
		for ( int tmpi = 0; tmpi < 5; tmpi++ )
		{
			if ( tmpi <= techlevel )
			{
				oddsvalue[count] = global.nequip_crit_odds[tmpi];
				count += 1;
				totalvalue += global.nequip_crit_odds[tmpi];
			}
		}

		int critlevel = 0;
		int curvalue = 0;
		int odds = totalvalue > 0 ? rand() % totalvalue : 0;
		for ( int tmpi = 0; tmpi < count; tmpi++ )
		{// 按照评价值方式随机
			curvalue = oddsvalue[tmpi];
			if ( curvalue > 0 && curvalue > odds )
			{
				critlevel = tmpi;
				break;
			}
			odds -= curvalue;
		}

		if ( critlevel > 0 && critlevel < g_techinfo[18].maxnum )
		{
			g_actors[actor_index].neq_crit[index] = g_techinfo[18].config[critlevel].value;
		}
	}

	nation_equip_sendinfo( actor_index, kind );
	return 0;
}

// 改造
int nation_equip_remake( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind >= g_nequip_remake_maxnum )
		return -1;
	int index = kind - 1;
	int level = pCity->neq_lv[index];
	if ( level <= 0 )
		return -1;
	if ( pCity->neq_sec[index] > 0 )
		return -1;

	// 满级了
	if ( level >= g_nationequip[kind].maxnum - 1 )
		return -1;

	// 不需要改造
	int star = g_nationequip[kind].config[level].remake_star;
	if ( star <= 0 || star >= g_nequip_remake[kind].maxnum )
		return -1;
	if ( pCity->neq_star[index] >= star )
		return -1;

	if ( pCity->silver < g_nequip_remake[kind].config[star].silver )
		return -1;
	city_changesilver( pCity->index, -g_nequip_remake[kind].config[star].silver, PATH_NATIONEQUIP );
	pCity->neq_sec[index] = g_nequip_remake[kind].config[star].sec;
	nation_equip_sendinfo( actor_index, kind );
	return 0;
}

// 加速
int nation_equip_quick( int actor_index, int kind, int type )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind >= g_nationequip_maxnum )
		return -1;
	int index = kind - 1;
	int level = pCity->neq_lv[index];
	if ( pCity->neq_sec[index] <= 0 )
		return -1;

	
	int token = 0;
	if ( type == 0 )
	{
		token = ( int )ceil( (pCity->neq_sec[index] / 60 + 1) * global.nequip_make_quick_token );
	}
	else if ( type == 1 )
	{
		token = (int)ceil( (pCity->neq_sec[index] / 60 + 1) * global.nequip_remake_quick_token );
	}
	if ( actor_change_token( actor_index, -token, PATH_NATIONEQUIP, 0 ) < 0 )
		return -1;
	pCity->neq_sec[index] = 0;
	nation_equip_over( pCity, index );
	return 0;
}

// 操作完成
void nation_equip_over( City *pCity, int index )
{
	if ( !pCity )
		return;
	if ( index < 0 || index >= 6 )
		return;

	if ( pCity->neq_lv[index] <= 0 )
	{ // 开启完毕
		pCity->neq_lv[index] = 1;
	}
	else
	{ // 改造完毕
		int kind = index + 1;
		int level = pCity->neq_lv[index];
		if ( level <= 0 || level >= g_nationequip[kind].maxnum - 1 )
			return;
		int star = g_nationequip[kind].config[level].remake_star;
		if ( star <= 0 || star >= g_nequip_remake[kind].maxnum )
			return;
		pCity->neq_pro[index] += 1;
		if ( pCity->neq_pro[index] >= g_nequip_remake[kind].config[star].progress )
		{
			pCity->neq_pro[index] = 0;
			pCity->neq_star[index] = star;
		}
	}

	if ( pCity->actor_index >= 0 )
	{
		nation_equip_sendinfo( pCity->actor_index, index + 1 );
	}
}

void nation_equip_makestruct( int index, City *pCity, SLK_NetS_NationEquip *pValue )
{
	if ( index < 0 || index >= 6 )
		return;
	pValue->m_kind = index + 1;
	pValue->m_neq_lv = pCity->neq_lv[index];
	pValue->m_neq_exp = pCity->neq_exp[index];
	pValue->m_neq_star = pCity->neq_star[index];
	pValue->m_neq_pro = pCity->neq_pro[index];
	pValue->m_neq_sec = pCity->neq_sec[index];

	int actor_index = pCity->actor_index;
	if ( actor_index >= 0 && actor_index < g_maxactornum )
	{
		pValue->m_neq_crit = g_actors[actor_index].neq_crit[index];
	}
}

// 发送信息
int nation_equip_sendinfo( int actor_index, int kind )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind > 6 )
		return -1;
	SLK_NetS_NationEquip pValue = { 0 };
	nation_equip_makestruct( kind - 1, pCity, &pValue );
	netsend_nationequip_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 发送列表
int nation_equip_sendlist( int actor_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_NationEquipList pValue = { 0 };
	for ( int tmpi = 0; tmpi < 6; tmpi++ )
	{
		nation_equip_makestruct( tmpi, pCity, &pValue.m_list[pValue.m_count] );
		pValue.m_count += 1;
	}
	netsend_nationequiplist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
