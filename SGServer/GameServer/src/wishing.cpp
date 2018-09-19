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
#include "building.h"
#include "global.h"
#include "actor_send.h"
#include "actor_times.h"
#include "server_netsend_auto.h"
#include "system.h"
#include "item.h"
#include "quest.h"
#include "wishing.h"
#include "auto_data_wishing_shop.h"
#include "auto_data_wishing_pack.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern City *g_city;
extern int g_city_maxcount;

extern WishingShop *g_wishing_shop;
extern int g_wishing_shop_maxnum;

extern WishingPack *g_wishing_pack;
extern int g_wishing_pack_maxnum;

int wishing_shop_random( int *kindlist, int kindnum, int *totalvalue )
{
	int outid = 0;
	int allvalue = *totalvalue;
	int curvalue = 0;
	int odds = allvalue > 0 ? rand() % allvalue : 0;
	for ( int tmpi = 0; tmpi < kindnum; tmpi++ )
	{
		// ��������ֵ��ʽ���
		int id = kindlist[tmpi];
		if ( id <= 0 || id >= g_wishing_shop_maxnum )
			continue;
		curvalue = g_wishing_shop[id].value;
		if ( curvalue > 0 && curvalue > odds )
		{
			outid = id;
			kindlist[tmpi] = 0;
			*totalvalue -= curvalue;
			break;
		}
		odds -= curvalue;
	}
	return outid;
}

int wishing_shop_update( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );

	int kindlist[6][32] = { 0 };
	int kindnum[6] = { 0 };
	int allvalue[6] = { 0 };
	// color = 2 ���ȡ4��
	// color = 3 ���ȡ2��
	// color = 4 ���ȡ2��
	// color = 5 ���ȡ1��
	for ( int id = 1; id < g_wishing_shop_maxnum; id++ )
	{
		int awardkind = g_wishing_shop[id].awardkind;
		if ( awardkind <= 0 )
			continue;
		if ( g_actors[actor_index].level < g_wishing_shop[id].level )
			continue;
		char color = (char)g_wishing_shop[id].color;
		if ( color <= 0 || color >= 6 )
			continue;
		int index = kindnum[color];
		kindnum[color]++;
		kindlist[color][index] = id;
		allvalue[color] += g_wishing_shop[id].value;
	}

	g_actors[actor_index].wishingid[0] = wishing_shop_random( kindlist[5], kindnum[5], &allvalue[5] );
	g_actors[actor_index].wishingopen[0] = 1;
	g_actors[actor_index].wishingid[1] = wishing_shop_random( kindlist[4], kindnum[4], &allvalue[4] );
	g_actors[actor_index].wishingopen[1] = 0;
	g_actors[actor_index].wishingid[2] = wishing_shop_random( kindlist[4], kindnum[4], &allvalue[4] );
	g_actors[actor_index].wishingopen[2] = 0;
	g_actors[actor_index].wishingid[3] = wishing_shop_random( kindlist[3], kindnum[3], &allvalue[3] );
	g_actors[actor_index].wishingopen[3] = 0;
	g_actors[actor_index].wishingid[4] = wishing_shop_random( kindlist[3], kindnum[3], &allvalue[3] );
	g_actors[actor_index].wishingopen[4] = 0;
	g_actors[actor_index].wishingid[5] = wishing_shop_random( kindlist[2], kindnum[2], &allvalue[2] );
	g_actors[actor_index].wishingopen[5] = 0;
	g_actors[actor_index].wishingid[6] = wishing_shop_random( kindlist[2], kindnum[2], &allvalue[2] );
	g_actors[actor_index].wishingopen[6] = 0;
	g_actors[actor_index].wishingid[7] = wishing_shop_random( kindlist[2], kindnum[2], &allvalue[2] );
	g_actors[actor_index].wishingopen[7] = 0;
	g_actors[actor_index].wishingid[8] = wishing_shop_random( kindlist[2], kindnum[2], &allvalue[2] );
	g_actors[actor_index].wishingopen[8] = 0;
	return 0;
}

int wishing_shop_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	int nowday = system_getfday();
	if ( nowday > g_actors[actor_index].wishingday )
	{ // �����Ѿ�������
		wishing_shop_update( actor_index );
		g_actors[actor_index].wishingday = nowday;
		g_actors[actor_index].wishingcd = 0;
	}

	SLK_NetS_WishingShop pValue = { 0 };
	pValue.m_openstamp = g_actors[actor_index].wishingcd;
	pValue.m_todaybuy = actor_get_today_char_times( actor_index, TODAY_CHAR_WISHINGSHOP_TODAYBUY );
	for ( int tmpi = 0; tmpi < WISHINGSHOP_ITEM_MAX; tmpi++ )
	{
		int id = g_actors[actor_index].wishingid[tmpi];
		if ( id > 0 && id < g_wishing_shop_maxnum )
		{
			pValue.m_list[pValue.m_count].m_id = id;
			if ( pValue.m_todaybuy == 1 )
				pValue.m_list[pValue.m_count].m_open = 1;
			else
				pValue.m_list[pValue.m_count].m_open = g_actors[actor_index].wishingopen[tmpi];
			pValue.m_list[pValue.m_count].m_color = (char)g_wishing_shop[id].color;
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

// �򿪱���
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

	g_actors[actor_index].wishingcd = (int)time( NULL ) + global.wishing_opencd;
	g_actors[actor_index].wishingopen[offset] = 1;
	wishing_shop_sendinfo( actor_index );
	// ����
	quest_addvalue( city_getptr( actor_index ), QUEST_DATATYPE_WISHING, 0, 0, 1 );
	return 0;
}

// ������
int wishing_shop_buy( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( id <= 0 || id >= g_wishing_shop_maxnum )
		return -1;

	// ����Ƿ����������
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

	// ���ļ��
	if ( g_wishing_shop[id].costkind == AWARDKIND_SILVER ) // ����
	{
		if ( pCity->silver < g_wishing_shop[id].costnum )
			return -1;
		city_changesilver( pCity->index, -g_wishing_shop[id].costnum, PATH_WISHINGSHOP );
	}
	else if ( g_wishing_shop[id].costkind == AWARDKIND_WOOD ) // ľ��
	{
		if ( pCity->wood < g_wishing_shop[id].costnum )
			return -1;
		city_changewood( pCity->index, -g_wishing_shop[id].costnum, PATH_WISHINGSHOP );
	}
	else if ( g_wishing_shop[id].costkind == AWARDKIND_FOOD ) // ��ʳ
	{
		if ( pCity->food < g_wishing_shop[id].costnum )
			return -1;
		city_changefood( pCity->index, -g_wishing_shop[id].costnum, PATH_WISHINGSHOP );
	}
	else if ( g_wishing_shop[id].costkind == AWARDKIND_IRON ) // ����
	{
		if ( pCity->iron < g_wishing_shop[id].costnum )
			return -1;
		city_changeiron( pCity->index, -g_wishing_shop[id].costnum, PATH_WISHINGSHOP );
	}
	else if ( g_wishing_shop[id].costkind == AWARDKIND_TOKEN ) // Ԫ��
	{
		if ( g_actors[actor_index].token < g_wishing_shop[id].costnum )
			return -1;
		actor_change_token( actor_index, -g_wishing_shop[id].costnum, PATH_WISHINGSHOP, 0 );
	}

	// ���뽱��
	award_getaward( actor_index, g_wishing_shop[id].awardkind, g_wishing_shop[id].awardnum, -1, PATH_WISHINGSHOP, NULL );
	
	// ����
	g_actors[actor_index].wishingcd = 0;

	// ˢ������ı���
	wishing_shop_update( actor_index );
	actor_add_today_char_times( actor_index, TODAY_CHAR_WISHINGSHOP_TODAYBUY );
	g_actors[actor_index].wishingday = system_getfday() + 1;
	wishing_shop_sendinfo( actor_index );
	// ����
	quest_addvalue( city_getptr( actor_index ), QUEST_DATATYPE_WISHING, 0, 0, 1 );

	// ÿ��
	everyday_quest_addvalue( pCity, 18, 1 );
	return 0;
}

// �۱���һ���Ϣ
// ʹ�����Ҷһ�	 ľ��0.24	����0.18
// ʹ�����ݶһ�   ����0.73   ľ��0.48
// ʹ��ľ�Ķһ�   ����0.55   ����0.26
int wishing_change_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	SLK_NetS_WishingChange pValue = { 0 };
	pValue.m_cd = actor_check_uselimit_cd( actor_index, USELIMIT_CD_WISHING_CHANGE );

	int silver = (int)ceil( city_yield_total( pCity, BUILDING_Silver, 1 ) / (float)WISHINGCHANGE_V ) * WISHINGCHANGE_V;
	pValue.m_silver = silver;
	pValue.m_silver_to_wood = (int)ceil( silver * global.wishing_silver_to_wood );
	pValue.m_silver_to_food = (int)ceil( silver * global.wishing_silver_to_food );

	int wood = (int)ceil( city_yield_total( pCity, BUILDING_Wood, 1 ) / (float)WISHINGCHANGE_V ) * WISHINGCHANGE_V;
	pValue.m_wood = wood;
	pValue.m_wood_to_silver = (int)ceil( wood * global.wishing_wood_to_silver );
	pValue.m_wood_to_food = (int)ceil( wood * global.wishing_wood_to_food );

	int food = (int)ceil( city_yield_total( pCity, BUILDING_Food, 1 ) / (float)WISHINGCHANGE_V ) * WISHINGCHANGE_V;
	pValue.m_food = food;
	pValue.m_food_to_silver = (int)ceil( food * global.wishing_food_to_silver );
	pValue.m_food_to_wood = (int)ceil( food * global.wishing_food_to_wood );

	netsend_wishingchange_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// �۱���һ�
int wishing_change_buy( int actor_index, int kind, int to_kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( actor_check_uselimit_cd( actor_index, USELIMIT_CD_WISHING_CHANGE ) >= global.wishing_change_cdmax )
	{ // ��ȴ����4Сʱ��
		return -1;
	}

	if ( kind == AWARDKIND_SILVER )
	{
		int silver = (int)ceil( city_yield_total( pCity, BUILDING_Silver, 1 ) / (float)WISHINGCHANGE_V ) * WISHINGCHANGE_V;
		if ( pCity->silver < silver )
			return -1;
		city_changesilver( pCity->index, -silver, PATH_WISHINGCHANGE );
		if ( to_kind == AWARDKIND_WOOD )
		{
			int silver_to_wood = (int)ceil( silver * global.wishing_silver_to_wood );
			city_changewood( pCity->index, silver_to_wood, PATH_WISHINGCHANGE );
		}
		else if ( to_kind == AWARDKIND_FOOD )
		{
			int silver_to_food = (int)ceil( silver * global.wishing_silver_to_food );
			city_changefood( pCity->index, silver_to_food, PATH_WISHINGCHANGE );
		}
	}
	else if ( kind == AWARDKIND_WOOD )
	{
		int wood = (int)ceil( city_yield_total( pCity, BUILDING_Wood, 1 ) / (float)WISHINGCHANGE_V ) * WISHINGCHANGE_V;
		if ( pCity->wood < wood )
			return -1;
		city_changewood( pCity->index, -wood, PATH_WISHINGCHANGE );
		if ( to_kind == AWARDKIND_SILVER )
		{
			int wood_to_silver = (int)ceil( wood * global.wishing_wood_to_silver );
			city_changesilver( pCity->index, wood_to_silver, PATH_WISHINGCHANGE );
		}
		else if ( to_kind == AWARDKIND_FOOD )
		{
			int wood_to_food = (int)ceil( wood * global.wishing_wood_to_food );
			city_changefood( pCity->index, wood_to_food, PATH_WISHINGCHANGE );
		}
	}
	else if ( kind == AWARDKIND_FOOD )
	{
		int food = (int)ceil( city_yield_total( pCity, BUILDING_Food, 1 ) / (float)WISHINGCHANGE_V ) * WISHINGCHANGE_V;
		if ( pCity->food < food )
			return -1;
		city_changefood( pCity->index, -food, PATH_WISHINGCHANGE );
		if ( to_kind == AWARDKIND_SILVER )
		{
			int food_to_silver = (int)ceil( food * global.wishing_food_to_silver );
			city_changesilver( pCity->index, food_to_silver, PATH_WISHINGCHANGE );
		}
		else if ( to_kind == AWARDKIND_WOOD )
		{
			int food_to_wood = (int)ceil( food * global.wishing_food_to_wood );
			city_changewood( pCity->index, food_to_wood, PATH_WISHINGCHANGE );
		}
	}
	else if ( kind == 1 || kind == 11 || kind == 21 )
	{
		if ( item_lost( actor_index, kind, global.wishing_green_to_draw, PATH_WISHINGCHANGE ) < 0 )
			return -1;
		item_getitem( actor_index, 42, 1, -1, PATH_WISHINGCHANGE );
	}
	actor_add_uselimit_cd( actor_index, USELIMIT_CD_WISHING_CHANGE, global.wishing_change_cd );
	wishing_change_sendinfo( actor_index );
	return 0;
}

// ���
int wishing_pack_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	int idlist[3] = { 0 };
	idlist[0] = actor_get_today_char_times( actor_index, TODAY_CHAR_WISHINGPACK_SILVER ) + 1;
	idlist[1] = actor_get_today_char_times( actor_index, TODAY_CHAR_WISHINGPACK_WOOD ) + 21;
	idlist[2] = actor_get_today_char_times( actor_index, TODAY_CHAR_WISHINGPACK_FOOD ) + 41;

	SLK_NetS_WishingPack pValue = { 0 };
	for ( int tmpi = 0; tmpi < 3; tmpi++ )
	{
		int id = idlist[tmpi];
		if ( id <= 0 || id >= g_wishing_pack_maxnum )
			continue;
		pValue.m_list[pValue.m_count].m_id = id;
		pValue.m_list[pValue.m_count].m_awardkind = g_wishing_pack[id].awardkind;
		pValue.m_list[pValue.m_count].m_awardnum = g_wishing_pack[id].awardnum;
		pValue.m_list[pValue.m_count].m_costkind = g_wishing_pack[id].costkind;
		pValue.m_list[pValue.m_count].m_costnum = g_wishing_pack[id].costnum;
		pValue.m_list[pValue.m_count].m_token = g_wishing_pack[id].token;
		pValue.m_count += 1;
	}

	netsend_wishingpack_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

int wishing_pack_buy( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	if ( id <= 0 || id >= g_wishing_pack_maxnum )
		return -1;

	// ���ļ��
	if ( g_wishing_pack[id].costkind == AWARDKIND_SILVER ) // ����
	{
		int num = actor_get_today_char_times( actor_index, TODAY_CHAR_WISHINGPACK_SILVER );
		int nowid = num + 1;
		if ( nowid != id )
			return -1;
		if ( g_actors[actor_index].token < g_wishing_pack[id].token )
			return -1;
		if ( pCity->silver < g_wishing_pack[id].costnum )
			return -1;
		city_changesilver( pCity->index, -g_wishing_pack[id].costnum, PATH_WISHINGPACK );
		if ( num < global.wishing_pack_maxnum )
			actor_add_today_char_times( actor_index, TODAY_CHAR_WISHINGPACK_SILVER );

	}
	else if ( g_wishing_pack[id].costkind == AWARDKIND_WOOD ) // ľ��
	{
		int num = actor_get_today_char_times( actor_index, TODAY_CHAR_WISHINGPACK_WOOD );
		int nowid = num + 21;
		if ( nowid != id )
			return -1;
		if ( g_actors[actor_index].token < g_wishing_pack[id].token )
			return -1;
		if ( pCity->wood < g_wishing_pack[id].costnum )
			return -1;
		city_changewood( pCity->index, -g_wishing_pack[id].costnum, PATH_WISHINGPACK );
		if ( num < global.wishing_pack_maxnum )
			actor_add_today_char_times( actor_index, TODAY_CHAR_WISHINGPACK_WOOD );
	}
	else if ( g_wishing_pack[id].costkind == AWARDKIND_FOOD ) // ��ʳ
	{
		int num = actor_get_today_char_times( actor_index, TODAY_CHAR_WISHINGPACK_FOOD );
		int nowid = num + 41;
		if ( nowid != id )
			return -1;
		if ( g_actors[actor_index].token < g_wishing_pack[id].token )
			return -1;
		if ( pCity->food < g_wishing_pack[id].costnum )
			return -1;
		city_changefood( pCity->index, -g_wishing_pack[id].costnum, PATH_WISHINGPACK );
		if ( num < global.wishing_pack_maxnum )
			actor_add_today_char_times( actor_index, TODAY_CHAR_WISHINGPACK_FOOD );
	}
	else
		return -1;

	actor_change_token( actor_index, -g_wishing_pack[id].token, PATH_WISHINGPACK, 0 );

	// ���뽱��
	award_getaward( actor_index, g_wishing_pack[id].awardkind, g_wishing_pack[id].awardnum, -1, PATH_WISHINGPACK, NULL );
	wishing_pack_sendinfo( actor_index );
	return 0;
}
