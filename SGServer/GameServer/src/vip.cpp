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
#include "item.h"
#include "vip.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

extern City *g_city;
extern int g_city_maxcount;

extern VipInfo *g_vipinfo;
extern int g_vipinfo_maxnum;

extern VipBag *g_vipbag;
extern int g_vipbag_maxnum;

extern VipShop *g_vipshop;
extern int g_vipshop_maxnum;

// 计算VIP等级
int vip_calclevel( int city_index )
{
	CITY_CHECK_INDEX( city_index );
	for ( char level = g_vipinfo_maxnum-1; level >= 0; level-- )
	{
		if ( g_city[city_index].vipexp >= g_vipinfo[level].exp )
		{
			g_city[city_index].viplevel = level;
			break;
		}
	}
	return 0;
}

// vip获取的属性
int vip_attr_calc( int city_index )
{
	CITY_CHECK_INDEX( city_index );
	for ( int viplevel = 0; viplevel <= g_city[city_index].viplevel; viplevel++ )
	{
		if ( g_vipinfo[viplevel].skipfight > 0 )
		{
			actor_set_sflag( g_city[city_index].actor_index, ACTOR_SFLAG_SKIPFIGHT, 1 );
		}
		else if ( g_vipinfo[viplevel].palace > 0 )
		{
			actor_set_sflag( g_city[city_index].actor_index, ACTOR_SFLAG_PALACE, 1 );
		}
		else if ( g_vipinfo[viplevel].storysweep > 0 )
		{
			actor_set_sflag( g_city[city_index].actor_index, ACTOR_SFLAG_STORYSWEEP, 1 );
		}
	}
	return 0;
}

// VIP经验升级值
int vip_expmax_get( int level )
{
	if ( level < 0 || level >= g_vipinfo_maxnum )
		return 0;
	return g_vipinfo[level].exp;
}

// vip升级
int vip_upgrade( int city_index, short path )
{
	CITY_CHECK_INDEX( city_index );
	if ( g_city[city_index].viplevel >= (g_vipinfo_maxnum - 1) )
		return -1;
	int lastlevel = g_city[city_index].viplevel;
	g_city[city_index].viplevel += 1;
	return 0;
}

// vip经验
int vip_exp( int city_index, int value, short path )
{
	CITY_CHECK_INDEX( city_index );
	if ( g_city[city_index].viplevel >= (g_vipinfo_maxnum - 1) )
		return -1;
	g_city[city_index].vipexp += value;

	// 检查升级
	char isup = 0;
	while ( g_city[city_index].vipexp >= g_vipinfo[g_city[city_index].viplevel].exp )
	{
		int curlevel = g_city[city_index].viplevel;
		// 可以升级
		if ( vip_upgrade( city_index, path ) < 0 )
			break;
		g_city[city_index].vipexp -= g_vipinfo[curlevel].exp;
		isup = 1;
	}

	if ( isup == 1 )
	{ // 升级了
		vip_attr_calc( city_index );
	}
	wlog( 0, LOGOP_VIPEXP, path, value, g_city[city_index].vipexp, g_city[city_index].viplevel, g_city[city_index].actorid, city_mainlevel( city_index ) );

	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_Vip pValue = { 0 };
	pValue.m_addexp = value;
	pValue.m_curexp = g_city[city_index].vipexp;
	pValue.m_expmax = vip_expmax_get( g_city[city_index].viplevel );
	pValue.m_level = g_city[city_index].viplevel;
	pValue.m_isup = isup;
	pValue.m_path = path;
	netsend_changevip_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 立即建造加速
int vip_attr_buildfree( City *pCity )
{
	if ( !pCity )
		return 0;
	int viplevel = pCity->viplevel;
	if ( viplevel < 0 )
		viplevel = 0;
	else if ( viplevel >= g_vipinfo_maxnum )
		viplevel = g_vipinfo_maxnum - 1;
	return g_vipinfo[viplevel].buildfree;
}

// 体力购买次数
int vip_attr_bodybuy( City *pCity )
{
	if ( !pCity )
		return 0;
	int viplevel = pCity->viplevel;
	if ( viplevel < 0 )
		viplevel = 0;
	else if ( viplevel >= g_vipinfo_maxnum )
		viplevel = g_vipinfo_maxnum - 1;
	return g_vipinfo[viplevel].bodybuy;
}

// 自动建造次数
int vip_attr_autobuild( City *pCity )
{
	if ( !pCity )
		return 0;
	int viplevel = pCity->viplevel;
	if ( viplevel < 0 )
		viplevel = 0;
	else if ( viplevel >= g_vipinfo_maxnum )
		viplevel = g_vipinfo_maxnum - 1;
	return g_vipinfo[viplevel].autobuild;
}

// 每日免费行军召回次数
int vip_attr_marchcall( City *pCity )
{
	if ( !pCity )
		return 0;
	int viplevel = pCity->viplevel;
	if ( viplevel < 0 )
		viplevel = 0;
	else if ( viplevel >= g_vipinfo_maxnum )
		viplevel = g_vipinfo_maxnum - 1;
	return g_vipinfo[viplevel].marchcall;
}

// 武卒官加速募兵
int vip_attr_train( City *pCity )
{
	if ( !pCity )
		return 0;
	int viplevel = pCity->viplevel;
	if ( viplevel < 0 )
		viplevel = 0;
	else if ( viplevel >= g_vipinfo_maxnum )
		viplevel = g_vipinfo_maxnum - 1;
	return g_vipinfo[viplevel].train;
}

// VIP礼包，发送是否购买
int vipbag_list( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	SLK_NetS_VipBag pValue = { 0 };
	pValue.m_vipbag = g_actors[actor_index].vipbag;
	pValue.m_vipbag_count = g_vipbag_maxnum - 1;
	netsend_vipbag_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
int vipbag_buy( int actor_index, int level )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( level < 0 || level >= g_vipbag_maxnum )
		return -1;
	if ( level > pCity->viplevel )
		return -1;
	if ( vipbag_check( actor_index, level ) == 1 )
		return -1;
	if ( actor_change_token( actor_index, -g_vipbag[level].token, PATH_VIPBAG, 0 ) < 0 )
		return -1;
	for ( int tmpi = 0; tmpi < 6; tmpi++ )
	{
		if ( g_vipbag[level].awardkind[tmpi] > 0 )
		{
			award_getaward( actor_index, g_vipbag[level].awardkind[tmpi], g_vipbag[level].awardnum[tmpi], 0, PATH_VIPBAG, NULL );
		}
	}
	g_actors[actor_index].vipbag |= (1 << level);
	vipbag_list( actor_index );
	return 0;
}
int vipbag_check( int actor_index, int level )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return 0;
	if ( g_actors[actor_index].vipbag & (1 << level) )
		return 1;
	return 0;
}

// VIP特价商店
int vipshop_list( int actor_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int viplevel = pCity->viplevel;
	if ( viplevel < 0 || viplevel >= g_vipshop_maxnum )
		return -1;

	SLK_NetS_VipShop pValue = { 0 };
	pValue.m_useitem = g_actors[actor_index].shop_useitem;
	for ( int tmpi = 0; tmpi < g_vipshop_maxnum; tmpi++ )
	{
		if ( g_vipshop[tmpi].id <= 0 )
			continue;
		if ( pCity->level < g_vipshop[tmpi].actorlevel )
			continue;
		if ( pCity->viplevel < g_vipshop[tmpi].viplevel )
			continue;
		if ( g_vipshop[tmpi].vipbaglevel > 0 )
		{
			if ( vipbag_check( actor_index, g_vipshop[tmpi].vipbaglevel ) == 0 )
				continue;
		}
		pValue.m_list[pValue.m_count].m_id = (char)g_vipshop[tmpi].id;
		pValue.m_list[pValue.m_count].m_awardkind = g_vipshop[tmpi].awardkind;
		pValue.m_list[pValue.m_count].m_token = g_vipshop[tmpi].token;
		pValue.m_list[pValue.m_count].m_itemkind = g_vipshop[tmpi].itemkind;
		pValue.m_list[pValue.m_count].m_vip_token = g_vipshop[tmpi].vip_token[viplevel];
		pValue.m_list[pValue.m_count].m_vip_buynum_max = g_vipshop[tmpi].vip_buynum[viplevel];
		pValue.m_list[pValue.m_count].m_vip_buynum = vipshop_getbuynum( actor_index, g_vipshop[tmpi].id );
		if ( g_vipshop[tmpi].id == 5 )
		{ // 自动建造
			pValue.m_list[pValue.m_count].m_awardnum = g_vipinfo[viplevel].autobuild;
		}
		else
		{
			pValue.m_list[pValue.m_count].m_awardnum = g_vipshop[tmpi].awardnum;
		}

		pValue.m_count += 1;
		if ( pValue.m_count >= 32 )
		{
			break;
		}
	}
	netsend_vipshop_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 获取今天已经vip价格购买次数
int vipshop_getbuynum( int actor_index, int id )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return 0;
	int todaynum = actor_get_today_short_times( actor_index, TODAY_SHORT_VIPSHOP_BUYNUM_BASE + id - 1 );
	return todaynum;
}

// 添加今天已经vip价格购买次数
int vipshop_addbuynum( int actor_index, int id, short num )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return 0;
	actor_add_today_short_times( actor_index, TODAY_SHORT_VIPSHOP_BUYNUM_BASE + id - 1, num );
	return 0;
}

// 购买
int vipshop_buy( int actor_index, int id, int awardkind, int count )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int viplevel = pCity->viplevel;
	if ( viplevel < 0 || viplevel >= g_vipshop_maxnum )
		return -1;
	if ( id <= 0 || id >= g_vipshop_maxnum )
		return -1;
	if ( g_vipshop[id].awardkind != awardkind )
		return -1;

	int addtimes = 0;
	int costtoken = g_vipshop[id].token * count;
	if ( g_vipshop[id].vip_buynum[viplevel] > 0 )
	{
		int leftnum = g_vipshop[id].vip_buynum[viplevel] - vipshop_getbuynum( actor_index, id );
		if ( leftnum > 0 )
		{
			if ( count > leftnum )
				count = leftnum;
			costtoken = count * g_vipshop[id].vip_token[viplevel]; 
			addtimes = count;
		}
	}
	
	if ( actor_change_token( actor_index, -costtoken, PATH_SHOP, 0 ) < 0 )
		return -1;

	award_getaward( actor_index, g_vipshop[id].awardkind, g_vipshop[id].awardnum*count, -1, PATH_VIPSHOP, NULL );
	if ( addtimes > 0 )
	{
		vipshop_addbuynum( actor_index, id, addtimes );
	}
	vipshop_list( actor_index );
	return 0;
}

// 使用道具购买
int vipshop_useitem_buy( int actor_index, int id, int awardkind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int viplevel = pCity->viplevel;
	if ( viplevel < 0 || viplevel >= g_vipshop_maxnum )
		return -1;
	if ( id <= 0 || id >= g_vipshop_maxnum )
		return -1;
	if ( g_vipshop[id].awardkind != awardkind )
		return -1;
	if ( g_vipshop[id].itemkind <= 0 )
		return -1;
	if ( item_lost( actor_index, g_vipshop[id].itemkind, 1, PATH_VIPSHOP ) < 0 )
		return -1;
	award_getaward( actor_index, g_vipshop[id].awardkind, g_vipshop[id].awardnum, -1, PATH_VIPSHOP, NULL );
	vipshop_list( actor_index );
	return 0;
}

// 设置优先使用道具购买
int vipshop_set_useitembuy( int actor_index, int close )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( close == 1 )
	{// 1是关闭，0是开启，切记，默认值
		g_actors[actor_index].shop_useitem = 1;
	}
	else
	{
		g_actors[actor_index].shop_useitem = 0;
	}
	vipshop_list( actor_index );
	return 0;
}
