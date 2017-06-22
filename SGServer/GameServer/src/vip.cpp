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

// vip经验
int vip_exp( int city_index, int value, short path )
{
	CITY_CHECK_INDEX( city_index );
	if ( g_city[city_index].vipexp > INT_MAX - value )
		g_city[city_index].vipexp = INT_MAX;
	else
		g_city[city_index].vipexp += value;

	char isup = 0;
	char oldlevel = g_city[city_index].viplevel;
	vip_calclevel( city_index );
	if ( oldlevel > 0 && g_city[city_index].viplevel > oldlevel )
	{ // 升级了
		isup = 1;
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