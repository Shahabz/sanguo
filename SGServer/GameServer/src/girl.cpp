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

extern ItemKind *g_itemkind;
extern int g_itemkind_maxnum;

extern GirlInfo *g_girlinfo;
extern int g_girlinfo_maxnum;

extern GirlLove *g_girllove;
extern int g_girllove_maxnum;

extern GirlSon *g_girlson;
extern int g_girlson_maxnum;

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

GirlLove *girl_love_getconfig( int level )
{
	if ( level <= 0 || level >= g_girllove_maxnum )
		return NULL;
	return &g_girllove[level];
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
	Girl *pGirl = girl_getptr( pCity->index, girlkind );
	if ( !pGirl )
		return -1;
	if ( pGirl->herokind > 0 )
		return -1;
	/*if ( pHero->girlkind > 0 && pGirl->herokind > 0 )
		return -1;*/
	pHero->girlkind = (char)girlkind;
	pHero->sontime = 0;
	pGirl->herokind = herokind;
	pGirl->love_level = 1;
	pGirl->love_exp = 0;
	pGirl->love_today = 0;
	pGirl->sflag = 0;
	hero_attr_calc( pCity, pHero );
	city_battlepower_hero_calc( pCity );
	hero_sendinfo( pCity->actor_index, pHero );
	girl_info( pCity, pGirl );
	return 0;
}

// 解除委派
int girl_unallot( int actor_index, short kind )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Girl *pGirl = girl_getptr( pCity->index, kind );
	if ( !pGirl )
		return -1;
	Hero *pHero = hero_getptr( actor_index, pGirl->herokind );
	if ( !pHero )
		return -1;
	pHero->girlkind = 0;
	pHero->sontime = 0;
	pGirl->herokind = 0;
	hero_attr_calc( pCity, pHero );
	city_battlepower_hero_calc( pCity );
	hero_sendinfo( pCity->actor_index, pHero );
	girl_info( pCity, pGirl );
	return 0;
}

// 突破
int girl_colorup( int actor_index, short kind )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Girl *pGirl = girl_getptr( pCity->index, kind );
	if ( !pGirl )
		return -1;
	if ( pGirl->color >= ITEM_COLOR_LEVEL_PURPLE )
		return -1;
	GirlInfoConfig *config = girl_getconfig( kind, pGirl->color );
	if ( !config )
		return -1;
	if ( config->soul <= 0 )
		return -1;
	if ( pGirl->soul < config->soul )
	{
		actor_notify_pop( actor_index, 3404 );
		return -1;
	}
	pGirl->soul -= config->soul;
	pGirl->color += 1;
	if ( pGirl->herokind > 0 )
	{
		Hero *pHero = hero_getptr( actor_index, pGirl->herokind );
		if ( pHero )
		{
			hero_attr_calc( pCity, pHero );
			city_battlepower_hero_calc( pCity );
			hero_sendinfo( pCity->actor_index, pHero );
		}
	}
	wlog( 0, LOGOP_GIRLSOUL, PATH_GIRLCOLORUP, kind, -config->soul, pGirl->soul, pCity->actorid, city_mainlevel( pCity->index ) );
	girl_info( pCity, pGirl );
	return 0;
}

// 亲密度道具使用
int girl_loveitemuse( int actor_index, short kind, short itemkind )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Girl *pGirl = girl_getptr( pCity->index, kind );
	if ( !pGirl )
		return -1;
	if ( pGirl->herokind <= 0 )
		return -1;
	if ( itemkind <= 0 || itemkind >= g_itemkind_maxnum )
		return -1;
	short ability0 = item_get_base_ability( itemkind, 0 );
	short ability1 = item_get_base_ability( itemkind, 1 );
	int value0 = item_get_base_value( itemkind, 0 );
	int value1 = item_get_base_value( itemkind, 1 );
	if ( ability0 != ITEM_ABILITY_GIRLTYPE || ability1 != ITEM_ABILITY_GIRLLOVEEXP )
		return -1;

	if ( item_lost( actor_index, itemkind, 1, PATH_ITEMUSE ) < 0 )
	{ // 没有道具，那么使用钻石
		int price = item_gettoken( itemkind );
		if ( actor_change_token( actor_index, -price, PATH_ITEMUSE, itemkind ) == -2 )
		{
			return -1;
		}
	}

	// 添加亲密度
	girl_addloveexp( pCity, pGirl, value1, PATH_ITEMUSE );
	return 0;
}

// 喜结连理
int girl_marry( int actor_index, short kind )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Girl *pGirl = girl_getptr( pCity->index, kind );
	if ( !pGirl )
		return -1;
	if ( pGirl->herokind <= 0 )
		return -1;
	if ( pGirl->love_level < global.girl_marry_lovelevel )
		return -1;
	pGirl->sflag |= (1 << GIRL_SFLAG_MARRY);
	girl_info( pCity, pGirl );
	return 0;
}

// 亲密互动
int girl_makelove( int actor_index, short kind )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Girl *pGirl = girl_getptr( pCity->index, kind );
	if ( !pGirl )
		return -1;
	if ( pGirl->herokind <= 0 )
		return -1;
	if ( (pGirl->sflag & (1 << GIRL_SFLAG_MARRY)) == 0 )
		return -1; // 结婚以后才能亲密互动
	if ( (pGirl->sflag & (1 << GIRL_SFLAG_MAKELOVE)) == 1 )
		return -1; // 今天已经亲密互动
	if ( pGirl->love_level >= g_girllove_maxnum )
		return -1;
	GirlLove *loveconfig = girl_love_getconfig( pGirl->love_level );
	if ( !loveconfig )
		return -1;
	Hero *pHero = hero_getptr( actor_index, pGirl->herokind );
	if ( !pHero )
		return -1;
	if ( pHero->sontime > 0 )
		return -1;

	// 添加亲密度
	girl_addloveexp( pCity, pGirl, loveconfig->makelove_exp, PATH_GIRLMAKELOVE );

	// 生孩子
	int born = 0;
	if ( loveconfig->soncount > 0 && pHero->sonnum < loveconfig->soncount )
	{
		if ( pHero->kind < g_girlson_maxnum && pHero->sonnum < 5 )
		{
			int odds = g_girlson[pHero->kind].config[pHero->sonnum].born_odds;
			if ( rand()%100 < odds )
			{
				pHero->sontime = (int)time( NULL ) + g_girlson[pHero->kind].config[pHero->sonnum].grow_sec;
				hero_sendinfo( pCity->actor_index, pHero );
				born = 1;
			}
		}
	}
	pGirl->sflag |= (1 << GIRL_SFLAG_MAKELOVE);
	SLK_NetS_GirlLoveResult pValue = { 0 };
	pValue.m_kind = (char)kind;
	pValue.m_sflag = pGirl->sflag;
	pValue.m_born = born;
	pValue.m_makelove_exp = loveconfig->makelove_exp;
	netsend_girlloveresult_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 亲密度增长速率
float girl_loveexp_speed( short herokind, short kind, char color )
{
	GirlInfoConfig *config = girl_getconfig( kind, color );
	if ( !config )
		return 1.0f;
	short star = 0;
	if ( config->private_herokind == herokind )
	{
		star = config->love_star + config->private_love_star;
	}
	else
	{
		star = config->love_star;
	}
	return (float)(1 + (star - 1) * 0.5);
}

// 出征获取亲昵度
int girl_addloveexp_killenemy( City *pCity, short herokind, short kind )
{
	if ( !pCity )
		return -1;
	if ( herokind <= 0 || kind <= 0 )
		return -1;
	Girl *pGirl = girl_getptr( pCity->index, kind );
	if ( !pGirl )
		return -1;
	if ( pGirl->love_level < 0 || pGirl->love_level >= g_girllove_maxnum - 1 )
		return -1;
	float speed = girl_loveexp_speed( herokind, pGirl->kind, pGirl->color );
	int maxexp = (int)ceil(g_girllove[pGirl->love_level].today_maxexp*speed);
	if ( pGirl->love_today >= maxexp )
		return -1;
	int add = (int)ceil( g_girllove[pGirl->love_level].killenemy_exp*speed );
	girl_addloveexp( pCity, pGirl, add, PATH_FIGHT );
	return 0;
}
int girl_addloveexp_citywar( City *pCity, short herokind, short kind )
{
	if ( !pCity )
		return -1;
	if ( herokind <= 0 || kind <= 0 )
		return -1;
	Girl *pGirl = girl_getptr( pCity->index, kind );
	if ( !pGirl )
		return -1;
	if ( pGirl->love_level < 0 || pGirl->love_level >= g_girllove_maxnum - 1 )
		return -1;
	float speed = girl_loveexp_speed( herokind, pGirl->kind, pGirl->color );
	int maxexp = (int)ceil( g_girllove[pGirl->love_level].today_maxexp*speed );
	if ( pGirl->love_today >= maxexp )
		return -1;
	int add = (int)ceil( g_girllove[pGirl->love_level].citywar_exp*speed );
	girl_addloveexp( pCity, pGirl, add, PATH_FIGHT );
	return 0;
}
int girl_addloveexp_nationwar( City *pCity, short herokind, short kind )
{
	if ( !pCity )
		return -1;
	if ( herokind <= 0 || kind <= 0 )
		return -1;
	Girl *pGirl = girl_getptr( pCity->index, kind );
	if ( !pGirl )
		return -1;
	if ( pGirl->love_level < 0 || pGirl->love_level >= g_girllove_maxnum - 1 )
		return -1;
	float speed = girl_loveexp_speed( herokind, pGirl->kind, pGirl->color );
	int maxexp = (int)ceil( g_girllove[pGirl->love_level].today_maxexp*speed );
	if ( pGirl->love_today >= maxexp )
		return -1;
	int add = (int)ceil( g_girllove[pGirl->love_level].nationwar_exp*speed );
	girl_addloveexp( pCity, pGirl, add, PATH_FIGHT );
	return 0;
}
int girl_addloveexp_kingwar( City *pCity, short herokind, short kind )
{
	if ( !pCity )
		return -1;
	if ( herokind <= 0 || kind <= 0 )
		return -1;
	Girl *pGirl = girl_getptr( pCity->index, kind );
	if ( !pGirl )
		return -1;
	if ( pGirl->love_level < 0 || pGirl->love_level >= g_girllove_maxnum - 1 )
		return -1;
	float speed = girl_loveexp_speed( herokind, pGirl->kind, pGirl->color );
	int maxexp = (int)ceil( g_girllove[pGirl->love_level].today_maxexp*speed );
	if ( pGirl->love_today >= maxexp )
		return -1;
	int add = (int)ceil( g_girllove[pGirl->love_level].kingwar_exp*speed );
	girl_addloveexp( pCity, pGirl, add, PATH_FIGHT );
	return 0;
}

// 添加亲昵度
int girl_addloveexp( City *pCity, Girl *pGirl, int exp, short path )
{
	if ( !pCity )
		return -1;
	if ( !pGirl )
		return -1;
	if ( pGirl->love_level < 0 || pGirl->love_level >= g_girllove_maxnum-1 )
		return -1;
	
	pGirl->love_exp += exp;
	if ( path != PATH_ITEMUSE && path != PATH_GIRLMAKELOVE )
	{
		pGirl->love_today += exp;
	}
	if ( pGirl->love_exp >= g_girllove[pGirl->love_level].exp )
	{
		pGirl->love_exp -= g_girllove[pGirl->love_level].exp;
		pGirl->love_level += 1;
	}

	if ( pCity->actor_index >= 0 )
	{
		SLK_NetS_GirlLove pValue = { 0 };
		pValue.m_kind = pGirl->kind;
		pValue.m_love_level = pGirl->love_level;
		pValue.m_love_exp = pGirl->love_exp;
		pValue.m_love_today = pGirl->love_today;
		pValue.m_add = exp;
		pValue.m_path = path;
		netsend_girllove_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
	}

	return 0;
}

// 子女出师
int girl_son_growth( int actor_index, short kind )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Girl *pGirl = girl_getptr( pCity->index, kind );
	if ( !pGirl )
		return -1;
	if ( pGirl->herokind <= 0 )
		return -1;
	Hero *pHero = hero_getptr( actor_index, pGirl->herokind );
	if ( !pHero )
		return -1;
	if ( pHero->sontime <= 0 )
		return -1;
	if ( pHero->kind >= g_girlson_maxnum )
		return -1;
	if ( pHero->sonnum < g_girlson[pHero->kind].maxnum-1 )
	{
		pHero->sonnum += 1;
	}
	pHero->sontime = 0;
	hero_attr_calc( pCity, pHero );
	city_battlepower_hero_calc( pCity );
	hero_sendinfo( pCity->actor_index, pHero );
	girl_info( pCity, pGirl );
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
