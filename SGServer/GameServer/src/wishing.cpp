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
#include "global.h"
#include "actor_send.h"
#include "actor_times.h"
#include "server_netsend_auto.h"
#include "system.h"
#include "item.h"
#include "wishing.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

extern City *g_city;
extern int g_city_maxcount;

extern WishingShop *g_wishing_shop;
extern int g_wishing_shop_maxnum;

int wishing_shop_update( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );

	int kindlist[6][32] = { 0 };
	int kindnum[6] = { 0 };
	int allvalue[6] = { 0 };
	// color = 2 随机取4个
	// color = 3 随机取2个
	// color = 4 随机取2个
	// color = 5 随机取1个
	for ( int id = 1; id < g_wishing_shop_maxnum; id++ )
	{
		int awardkind = g_wishing_shop[id].awardkind;
		if ( awardkind <= 0 )
			continue;
		if ( g_actors[actor_index].level < g_wishing_shop[id].level )
			continue;
		char color = g_wishing_shop[id].color;
		if ( color <= 0 || color >= 6 )
			continue;
		int index = kindnum[color];
		kindnum[color]++;
		kindlist[color][index] = id;
		allvalue[color] += g_wishing_shop[id].value;
	}

	g_actors[actor_index].wishingid[0] = 51;
	g_actors[actor_index].wishingopen[0] = 1;
	g_actors[actor_index].wishingid[1] = 41;
	g_actors[actor_index].wishingid[2] = 42;
	g_actors[actor_index].wishingid[3] = 31;
	g_actors[actor_index].wishingid[4] = 32;
	g_actors[actor_index].wishingid[5] = 21;
	g_actors[actor_index].wishingid[6] = 22;
	g_actors[actor_index].wishingid[7] = 23;
	g_actors[actor_index].wishingid[8] = 24;
	return 0;
}

int wishing_shop_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	int nowday = system_getfday();
	if ( nowday > g_actors[actor_index].wishingday )
	{ // 宝物已经过期了
		wishing_shop_update( actor_index );
		g_actors[actor_index].wishingday = nowday;
	}

	SLK_NetS_WishingShop pValue = { 0 };
	pValue.m_openstamp = g_actors[actor_index].wishingcd;
	for ( int tmpi = 0; tmpi < WISHINGSHOP_ITEM_MAX; tmpi++ )
	{
		int id = g_actors[actor_index].wishingid[tmpi];
		if ( id > 0 && id < g_wishing_shop_maxnum )
		{
			pValue.m_list[pValue.m_count].m_open = g_actors[actor_index].wishingopen[tmpi];
			pValue.m_list[pValue.m_count].m_color = g_wishing_shop[id].color;
			pValue.m_list[pValue.m_count].m_awardkind = g_wishing_shop[id].awardkind;
			pValue.m_list[pValue.m_count].m_awardnum = g_wishing_shop[id].awardnum;
			pValue.m_list[pValue.m_count].m_costkind = g_wishing_shop[id].costkind;
			pValue.m_list[pValue.m_count].m_costnum = g_wishing_shop[id].costnum;
		}
		pValue.m_count += 1;
	}
	netsend_wishingshop_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 打开宝物
int wishing_shop_open( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( (int)time( NULL ) < g_actors[actor_index].wishingcd )
		return -1;

	char offset = -1;
	for ( int tmpi = 0; tmpi < WISHINGSHOP_ITEM_MAX; tmpi++ )
	{
		if ( g_actors[actor_index].wishingid[tmpi] == id )
		{
			offset = tmpi;
			break;
		}
	}
	if ( offset < 0 )
		return -1;
	if ( g_actors[actor_index].wishingopen[offset] == 1 )
		return -1;

	g_actors[actor_index].wishingcd = (int)time(NULL) + 3600;
	g_actors[actor_index].wishingopen[offset] = 1;
	wishing_shop_sendinfo( actor_index );
}

// 购买宝物
int wishing_shop_buy( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( id <= 0 || id >= g_wishing_shop_maxnum )
		return -1;

	// 检查是否有这个道具
	char has = 0;
	for ( int tmpi = 0; tmpi < WISHINGSHOP_ITEM_MAX; tmpi++ )
	{
		if ( g_actors[actor_index].wishingid[tmpi] == id )
		{
			has = 1;
			break;
		}
	}
	if ( has == 0 )
		return -1;

	// 消耗检查
	if ( g_wishing_shop[id].costkind == AWARDKIND_SILVER ) // 银币
	{
		if ( pCity->silver < g_wishing_shop[id].costnum )
			return -1;
		city_changesilver( pCity->index, -g_wishing_shop[id].costnum, PATH_WISHINGSHOP );
	}
	else if ( g_wishing_shop[id].costkind == AWARDKIND_WOOD ) // 木材
	{
		if ( pCity->wood < g_wishing_shop[id].costnum )
			return -1;
		city_changewood( pCity->index, -g_wishing_shop[id].costnum, PATH_WISHINGSHOP );
	}
	else if ( g_wishing_shop[id].costkind == AWARDKIND_FOOD ) // 粮食
	{
		if ( pCity->food < g_wishing_shop[id].costnum )
			return -1;
		city_changefood( pCity->index, -g_wishing_shop[id].costnum, PATH_WISHINGSHOP );
	}
	else if ( g_wishing_shop[id].costkind == AWARDKIND_IRON ) // 镔铁
	{
		if ( pCity->iron < g_wishing_shop[id].costnum )
			return -1;
		city_changeiron( pCity->index, -g_wishing_shop[id].costnum, PATH_WISHINGSHOP );
	}
	else if ( g_wishing_shop[id].costkind == AWARDKIND_TOKEN ) // 元宝
	{
		if ( g_actors[actor_index].token < g_wishing_shop[id].costnum )
			return -1;
		actor_change_token( actor_index, -g_wishing_shop[id].costnum, PATH_WISHINGSHOP, 0 );
	}

	// 给与奖励
	award_getaward( actor_index, g_wishing_shop[id].awardkind, g_wishing_shop[id].awardnum, -1, PATH_WISHINGSHOP, NULL );
	
	// 全部打开
	g_actors[actor_index].wishingcd = 0;
	for ( int tmpi = 0; tmpi < WISHINGSHOP_ITEM_MAX; tmpi++ )
	{
		g_actors[actor_index].wishingopen[tmpi] = 1;
	}

	// 刷新明天的宝物
	wishing_shop_update( actor_index );
	g_actors[actor_index].wishingday = system_getfday() + 1;
	wishing_shop_sendinfo( actor_index );
	return 0;
}
