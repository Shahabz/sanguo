#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "db.h"
#include "global.h"
#include "actor.h"
#include "gameproc.h"
#include "item.h"
#include "system.h"
#include "map.h"
#include "server_netsend_auto.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "actor_times.h"
#include "hero.h"
#include "city.h"
#include "building.h"
#include "city_attr.h"
#include "mail.h"
#include "girl.h"

extern Global global;
extern MYSQL *myData;
extern MYSQL *myGame;

extern Actor *g_actors;
extern int g_maxactornum;
extern Actor g_temp_actor[2];

extern int g_city_maxindex;
extern City *g_city;
extern int g_city_maxcount;

extern GirlInfo *g_girlinfo;
extern int g_girlinfo_maxnum;
i64 g_maxgirlid;

Girl *girl_getptr( int city_index, int kind )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	if ( kind <= 0 || kind >= ACTOR_GIRL_MAX )
		return NULL;
	return &g_city[city_index].girl[kind];
}

GirlInfoConfig *girl_getconfig( int kind, int color )
{
	if ( kind <= 0 || kind >= g_girlinfo_maxnum )
		return NULL;
	if ( color < 0 || color >= g_girlinfo[kind].maxnum )
		return NULL;
	return &g_girlinfo[kind].config[color];
}

void girl_makestruct( City *pCity, Girl *pGirl, SLK_NetS_Girl *pValue )
{
	pValue->m_kind = pGirl->kind;
	pValue->m_color = pGirl->color;
	pValue->m_soul = pGirl->soul;
	pValue->m_sflag = pGirl->sflag;
	pValue->m_herokind = pGirl->herokind;
	pValue->m_love_exp = pGirl->love_exp;
	pValue->m_love_level = pGirl->love_level;
	pValue->m_love_today = pGirl->love_today;
	pValue->m_love_today_max = pGirl->love_today;
}

// 玩家获得女将
int girl_getgirl( City *pCity, int kind, char path )
{
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind >= g_girlinfo_maxnum )
		return -1;

	// 检查是否已经有这个女将了
	Girl *pHasGirl = girl_getptr( pCity->index, kind );
	if ( pHasGirl && pHasGirl->color > 0 )
	{
		GirlInfoConfig *config = girl_getconfig( kind, pHasGirl->color );
		if ( !config )
			return -1;
		// 寻访即使获得了，也要发过去，界面需要显示
		SLK_NetS_GirlGet pValue = { 0 };
		pValue.m_kind = kind;
		pValue.m_path = path;
		if ( config )
		{
			pValue.m_soulnum = config->soulchange;
			pHasGirl->soul += config->soulchange;
		}
		girl_makestruct( pCity, pHasGirl, &pValue.m_girl );
		netsend_girlget_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
		wlog( 0, LOGOP_GIRLSOUL, path, kind, config->soulchange, pHasGirl->soul, pCity->actorid, city_mainlevel( pCity->index ) );
		return -1;
	}
	if ( g_girlinfo[kind].config[0].init_color <= 0 )
		return -1;
	
	pCity->girl[kind].actorid = pCity->actorid;
	pCity->girl[kind].kind = kind;
	pCity->girl[kind].color = (char)g_girlinfo[kind].config[0].init_color;
	pCity->girl[kind].sflag = 0;
	pCity->girl[kind].herokind = 0;
	pCity->girl[kind].love_level = 1;
	pCity->girl[kind].love_exp = 0;
	pCity->girl[kind].love_today = 0;

	SLK_NetS_GirlGet pValue = { 0 };
	pValue.m_kind = kind;
	pValue.m_path = path;
	girl_makestruct( pCity, &pCity->girl[kind], &pValue.m_girl );
	netsend_girlget_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );

	// 保存
	actor_girl_save_auto( pCity->actorid, kind, &pCity->girl[kind], "actor_girl", NULL );
	wlog( 0, LOGOP_GIRL, path, kind, pCity->girl[kind].color, 0, pCity->actorid, city_mainlevel( pCity->index ) );
	return 0;
}

// 玩家获得女将碎片
int girl_getsoul( City *pCity, int kind, int soul, char path )
{
	if ( !pCity )
		return -1;
	if ( kind <= 0 || kind >= g_girlinfo_maxnum || kind >= ACTOR_GIRL_MAX )
		return -1;

	pCity->girl[kind].actorid = pCity->actorid;
	pCity->girl[kind].kind = kind;
	pCity->girl[kind].soul += soul;
	girl_info( pCity, &pCity->girl[kind] );

	char v1[32] = { 0 };
	char v2[32] = { 0 };
	sprintf( v1, "%s%d", TAG_GIRL, kind );
	sprintf( v2, "%d", soul );
	actor_notify_pop_v( pCity->actor_index, 3339, v1, v2 );


	wlog( 0, LOGOP_GIRLSOUL, path, kind, soul, pCity->girl[kind].soul, pCity->actorid, city_mainlevel( pCity->index ) );
	if ( pCity->girl[kind].color == 0 && pCity->girl[kind].soul >= g_girlinfo[kind].config[0].soul )
	{ // 自动合成
		pCity->girl[kind].soul -= g_girlinfo[kind].config[0].soul;
		girl_getgirl( pCity, kind, path );
	}
	return 0;
}

// 女将信息
int girl_info( City *pCity, Girl *pGirl )
{
	if ( !pCity || !pGirl )
		return -1;
	ACTOR_CHECK_INDEX( pCity->actor_index );
	SLK_NetS_Girl pValue = { 0 };
	girl_makestruct( pCity, pGirl, &pValue );
	netsend_girl_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 女将列表
int girl_list( int actor_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return 0;
	SLK_NetS_GirlList pValue = { 0 };
	for ( char kind = 1; kind < ACTOR_GIRL_MAX; kind++ )
	{
		if ( kind >= g_girlinfo_maxnum )
			break;
		if ( pCity->girl[kind].kind <= 0 )
			continue;
		girl_makestruct( pCity, &pCity->girl[kind], &pValue.m_list[pValue.m_count] );
		pValue.m_count += 1;
	}
	netsend_girllist_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

//  每天零点重置
void girl_update()
{
	for ( int city_index = 0; city_index < g_city_maxindex/*注意：使用索引位置，为了效率*/; city_index++ )
	{
		if ( g_city[city_index].actorid <= 0 )
			continue;
		for ( char kind = 1; kind < ACTOR_GIRL_MAX; kind++ )
		{
			if ( g_city[city_index].girl[kind].kind > 0 )
			{
				g_city[city_index].girl[kind].sflag &= ~(1 << GIRL_SFLAG_MAKELOVE);
				g_city[city_index].girl[kind].love_today = 0;
			}
		}
	}
}

// 委派男将
int girl_allot( int actor_index, short herokind, short girlkind )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( girlkind <= 0 || girlkind >= g_girlinfo_maxnum || girlkind >= ACTOR_GIRL_MAX )
		return -1;
	Hero *pHero = hero_getptr( actor_index, herokind );
	if ( !pHero )
		return -1;
	if ( pHero->girlkind > 0 )
		return -1;
	if ( pCity->girl[girlkind].herokind > 0 )
		return -1;
	pHero->girlkind = (char)girlkind;
	pCity->girl[girlkind].herokind = herokind;
	hero_attr_calc( pCity, pHero );
	city_battlepower_hero_calc( pCity );
	hero_sendinfo( pCity->actor_index, pHero );
	girl_info( pCity, &pCity->girl[girlkind] );
	return 0;
}

// 解除委派
int girl_unallot( int actor_index, short herokind )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Hero *pHero = hero_getptr( actor_index, herokind );
	if ( !pHero )
		return -1;
	short girlkind = pHero->girlkind;
	if ( girlkind <= 0 || girlkind >= g_girlinfo_maxnum || girlkind >= ACTOR_GIRL_MAX )
	{
		pHero->girlkind = 0;
		return -1;
	}
	pHero->girlkind = 0;
	pCity->girl[girlkind].herokind = 0;
	hero_attr_calc( pCity, pHero );
	city_battlepower_hero_calc( pCity );
	hero_sendinfo( pCity->actor_index, pHero );
	girl_info( pCity, &pCity->girl[girlkind] );
	return 0;
}

void girl_gm_getall( City *pCity )
{
	if ( !pCity )
		return;
	for ( int kind = 1; kind < g_girlinfo_maxnum; kind++ )
	{
		girl_getgirl( pCity, kind, PATH_GM );
	}
}
