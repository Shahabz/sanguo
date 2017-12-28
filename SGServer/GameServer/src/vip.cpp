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
#include "server_netsend_auto.h"
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
		city_attr_reset( &g_city[city_index] );
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

// VIP礼包
int vipbag_list( int actor_index )
{
	return 0;
}
int vipbag_buy( int actor_index, int level )
{
	return 0;
}

// VIP特价商店
int vipshop_list( int actor_index )
{
	return 0;
}
int vipshop_buy( int actor_index, int id, int awardkind )
{
	return 0;
}
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
	return 0;
}
