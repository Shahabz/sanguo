#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "db.h"
#include "city.h"
#include "city_attr.h"
#include "actor.h"
#include "actor_times.h"
#include "global.h"
#include "actor_send.h"
#include "server_netsend_auto.h"
#include "vip.h"
#include "system.h"
#include "item.h"
#include "shop.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

extern City *g_city;
extern int g_city_maxcount;

extern VipInfo *g_vipinfo;
extern int g_vipinfo_maxnum;

extern Shop *g_shop;
extern int g_shop_maxnum;

int shop_list( int actor_index, int type )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( type <= 0 || type >= g_shop_maxnum )
		return -1;

	if ( actor_get_today_char_times( actor_index, TODAY_CHAR_SHOP_SALEITEM ) == 0 )
	{ // 随机打折物品
		actor_set_today_char_times( actor_index, TODAY_CHAR_SHOP_SALEITEM, 1 );
		memset( g_actors[actor_index].shop_saleitem, 0, sizeof( short ) * 6 );
		// 随机军事
		short fightkind[32] = { 0 };
		short fightcount = 0;
		for ( int tmpi = 0; tmpi < g_shop[SHOPTYPE_FIGHT].maxnum; tmpi++ )
		{
			if ( g_shop[SHOPTYPE_FIGHT].config[tmpi].awardkind <= 0 )
				continue;
			if ( g_actors[actor_index].level < g_shop[SHOPTYPE_FIGHT].config[tmpi].actorlevel_min || g_actors[actor_index].level > g_shop[SHOPTYPE_FIGHT].config[tmpi].actorlevel_max )
				continue;
			fightkind[fightcount] = (short)g_shop[SHOPTYPE_FIGHT].config[tmpi].awardkind;
			fightcount += 1;
			if ( fightcount >= 32 )
				break;
		}
		// 打乱
		ruffle_short( fightkind, fightcount );
		int random_fightcount = random( 2, 3 );
		for ( int tmpi = 0; tmpi < random_fightcount; tmpi++ )
		{
			g_actors[actor_index].shop_saleitem[tmpi] = fightkind[tmpi];
		}

		// 随机其它
		short otherkind[32] = { 0 };
		short othercount = 0;
		for ( int tmpi = 0; tmpi < g_shop[SHOPTYPE_OTHER].maxnum; tmpi++ )
		{
			if ( g_shop[SHOPTYPE_OTHER].config[tmpi].awardkind <= 0 )
				continue;
			if ( g_actors[actor_index].level < g_shop[SHOPTYPE_OTHER].config[tmpi].actorlevel_min || g_actors[actor_index].level > g_shop[SHOPTYPE_OTHER].config[tmpi].actorlevel_max )
				continue;
			otherkind[othercount] = (short)g_shop[SHOPTYPE_OTHER].config[tmpi].awardkind;
			othercount += 1;
			if ( othercount >= 32 )
				break;
		}
		// 打乱
		ruffle_short( otherkind, othercount );
		int random_othercount = random( 2, 3 );
		for ( int tmpi = 0; tmpi < random_othercount; tmpi++ )
		{
			g_actors[actor_index].shop_saleitem[tmpi+3] = otherkind[tmpi];
		}
	}

	SLK_NetS_ShopList pValue = { 0 };
	pValue.m_type = type;
	for ( int tmpi = 0; tmpi < g_shop[type].maxnum; tmpi++ )
	{
		if ( g_shop[type].config[tmpi].awardkind <= 0 )
			continue;
		if ( g_actors[actor_index].level < g_shop[type].config[tmpi].actorlevel_min || g_actors[actor_index].level > g_shop[type].config[tmpi].actorlevel_max )
			continue;
		pValue.m_list[pValue.m_count].m_offset = g_shop[type].config[tmpi].index;
		pValue.m_list[pValue.m_count].m_awardkind = g_shop[type].config[tmpi].awardkind;
		pValue.m_list[pValue.m_count].m_awardnum = g_shop[type].config[tmpi].awardnum;
		pValue.m_list[pValue.m_count].m_buyuse = g_shop[type].config[tmpi].buyuse;
		if ( shop_checksale( actor_index, g_shop[type].config[tmpi].awardkind ) == 1 )
		{
			pValue.m_list[pValue.m_count].m_sale = g_shop[type].config[tmpi].sale;
			pValue.m_list[pValue.m_count].m_token = g_shop[type].config[tmpi].token;
		}
		else
		{
			pValue.m_list[pValue.m_count].m_sale = 0;
			pValue.m_list[pValue.m_count].m_token = g_shop[type].config[tmpi].original_token;
		}
		pValue.m_count += 1;
		if ( pValue.m_count >= 64 )
		{
			netsend_shoplist_S( actor_index, SENDTYPE_ACTOR, &pValue );
			pValue.m_count = 0;
		}
		
	}
	if ( pValue.m_count > 0 )
	{
		netsend_shoplist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}
	return 0;
}

int shop_checksale( int actor_index, int awardkind )
{
	ACTOR_CHECK_INDEX( actor_index );
	for ( int tmpi = 0; tmpi < 6; tmpi++ )
	{
		if ( g_actors[actor_index].shop_saleitem[tmpi] == awardkind )
			return 1;
	}
	return 0;
}

int shop_buy( int actor_index, int type, int offset, int awardkind, int buyuse )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( type <= 0 || type >= g_shop_maxnum )
		return -1;
	if ( offset < 0 || offset >= g_shop[type].maxnum )
		return -1;
	if ( g_shop[type].config[offset].awardkind != awardkind )
		return -1;
	
	if ( shop_checksale( actor_index, awardkind ) == 1 )
	{
		if ( actor_change_token( actor_index, -g_shop[type].config[offset].token, PATH_SHOP, 0 ) < 0 )
			return -1;
	}
	else
	{
		if ( actor_change_token( actor_index, -g_shop[type].config[offset].original_token, PATH_SHOP, 0 ) < 0 )
			return -1;
	}
	
	if ( buyuse == 1 )
	{
	}
	else
	{
		award_getaward( actor_index, g_shop[type].config[offset].awardkind, g_shop[type].config[offset].awardnum, -1, PATH_SHOP, NULL );
	}
	return 0;
}


