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
#include "define.h"
#include "actor.h"
#include "gameproc.h"
#include "item.h"
#include "actor_notify.h"
#include "actor_send.h"
#include "server_netsend_auto.h"
#include "script_auto.h"
#include "mapunit.h"
#include "city.h"
#include "global.h"
#include "building.h"

extern MYSQL *myData;
extern MYSQL *myGame;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern ItemKind *g_itemkind;
extern int g_itemkind_maxnum;

//-----------------------------------------------------------------------------
// 函数说明: 道具的使用
// 参数    : actor_index - 
//           hero_index	- 英雄index
//           target_index	- 目标index
//-----------------------------------------------------------------------------
int item_use( int actor_index, short itemindex, short itemnum, int herokind, int ask )
{
	Actor *pActor = NULL;
	int itemkind = 0;
	short item_level = 0;
	short item_type = 0;
	short true_usenum = 0;
	short tmp_ability = 0;
	int eff_result = 0;
	if ( itemindex < 0 || itemindex >= MAX_ACTOR_ITEMNUM )
		return -1;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	// 检查物品itemkind
	itemkind = g_actors[actor_index].item[itemindex].m_kind;
	if ( g_actors[actor_index].item[itemindex].m_kind <= 0 )
		return -1;

	// 获取物品类型
	item_type = item_gettype( itemkind );

	// 检查等级
	item_level = item_getlevel( itemkind );
	if ( item_level > 0 )
	{ // 主城等级
		if ( city_mainlevel( g_actors[actor_index].city_index ) < -item_level )
			return -1;
	}
	else if ( item_level < 0 )
	{ // 角色等级
		if ( pActor->level < item_level )
			return -1;
	}

	// 使用个数
	if ( itemnum == 0 )// 全用
		true_usenum = g_actors[actor_index].item[itemindex].m_num;
	else if ( itemnum > 0 )
		true_usenum = itemnum;
	else
		return -1;

	// 使用其它物品，物品真实使用数量
	if ( g_actors[actor_index].item[itemindex].m_num < true_usenum )
	{
		true_usenum = g_actors[actor_index].item[itemindex].m_num;
	}
	if ( true_usenum <= 0 )
		return -1;

	// 检查这个物品是否是禁止一起使用多个的
	if ( true_usenum > 1 )
	{
		if ( item_getsituation( itemkind ) & ITEM_SITUATION_NOUSEMORE )
			return -1;
	}

	// 普通物品根据类型进行不同的使用方式
	if ( item_type == ITEM_TYPE_NORMAL_USE )
	{ // 点击使用类道具
		int ability1 = item_get_base_ability( g_actors[actor_index].item[itemindex].m_kind, 0 );
		int value1 = item_get_base_value( g_actors[actor_index].item[itemindex].m_kind, 0 );
		if ( ability1 == ITEM_ABILITY_AWARDGROUP )
		{ // 掉落包
			AwardGetInfo getinfo = { 0 };
			for ( int tmpi = 0; tmpi < true_usenum; tmpi++ )
			{
				awardgroup_random( value1, 0, &getinfo );
			}
			for ( int tmpi = 0; tmpi < getinfo.count; tmpi++ )
			{
				award_getaward( actor_index, getinfo.kind[tmpi], getinfo.num[tmpi], getinfo.color[tmpi], PATH_ITEMUSE, NULL );
			}
		}
		else if ( ability1 == ITEM_ABILITY_SCRIPTID )
		{ // 脚本调用ID

		}
		else if ( ability1 == ITEM_ABILITY_ADDBODY )
		{ // 加体力
			city_changebody( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_ADDEXP )
		{ // 加主公经验
			city_actorexp( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_ADDHEROEXP )
		{ // 加英雄经验

		}
		else if ( ability1 == ITEM_ABILITY_CITYRES_SILVER )
		{ // 银币数量
			city_changesilver( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_CITYRES_WOOD )
		{ // 木材数量
			city_changewood( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_CITYRES_FOOD )
		{ // 粮食数量
			city_changefood( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_CITYRES_IRON )
		{ // 铁数量
			city_changeiron( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_BUFF )
		{ // buff
			if ( value1 == 1 )
			{ // 城池保护时间
				int ptsec = item_get_base_value(itemkind, 1 );
				city_changeprotect( g_actors[actor_index].city_index, ptsec, PATH_ITEMUSE );
			}
		}

	}
	else
		return item_sendnotuse( actor_index, itemindex, -2 );

	// 减去数量
	g_actors[actor_index].item[itemindex].m_num -= true_usenum;

	// 记录物品使用日志
	wlog( 0, LOGOP_ITEMLOST, PATH_ITEMUSE, g_actors[actor_index].item[itemindex].m_kind, true_usenum, g_actors[actor_index].item[itemindex].id, g_actors[actor_index].actorid, city_mainlevel( g_actors[actor_index].city_index ) );

	// 如果数量为0，那么就删掉
	if ( g_actors[actor_index].item[itemindex].m_num <= 0 )
	{
		item_deletebox( actor_index, itemindex );
		memset( &g_actors[actor_index].item[itemindex], 0, sizeof(Item) );
	}

	// 发送使用物品结果
	SLK_NetS_ItemUse Value = {};
	Value.m_itemoffset = itemindex;
	Value.m_usenum = true_usenum;
	Value.m_effres = eff_result;
	netsend_itemuse_S( actor_index, SENDTYPE_ACTOR, &Value );
	return eff_result;
}

//-----------------------------------------------------------------------------
// 函数说明: 直接花钱道具的使用
// 参数    : actor_index - 
//           hero_index	- 英雄index
//           target_index	- 目标index
//-----------------------------------------------------------------------------
int item_use_withtoken( int actor_index, short itemkind, short itemnum, int herokind, int ask )
{
	Actor *pActor = NULL;
	short item_level = 0;
	short item_type = 0;

	if ( itemkind <= 0 )
		return -1;
	if ( itemnum <= 0 )
		return -1;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	// 获取物品类型
	item_type = item_gettype( itemkind );

	pActor = &g_actors[actor_index];

	// 检查等级
	item_level = item_getlevel( itemkind );
	if ( item_level > 0 )
	{ // 主城等级
		if ( city_mainlevel( g_actors[actor_index].city_index ) < -item_level )
			return -1;
	}
	else if ( item_level < 0 )
	{ // 角色等级
		if ( pActor->level < item_level )
			return -1;
	}

	int abilitytype = item_get_base_ability( itemkind, 0 );
	int token = item_gettoken( itemkind );
	if ( token <= 0 )
		return -1;
	if ( g_actors[actor_index].token < token * itemnum )
		return -1;
	
	int true_usenum = itemnum;
	if ( item_type == ITEM_TYPE_NORMAL_USE )
	{ // 点击使用类道具
		int ability1 = item_get_base_ability( itemkind, 0 );
		int value1 = item_get_base_value( itemkind, 0 );
		if ( ability1 == ITEM_ABILITY_ADDBODY )
		{ // 加体力
			city_changebody( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_ADDEXP )
		{ // 加主公经验
			city_actorexp( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_ADDHEROEXP )
		{ // 加英雄经验

		}
		else if ( ability1 == ITEM_ABILITY_CITYRES_SILVER )
		{ // 银币数量
			city_changesilver( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_CITYRES_WOOD )
		{ // 木材数量
			city_changewood( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_CITYRES_FOOD )
		{ // 粮食数量
			city_changefood( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_CITYRES_IRON )
		{ // 铁数量
			city_changeiron( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_BUFF )
		{ // buff
			if ( value1 == 1 )
			{ // 城池保护时间
				int ptsec = item_get_base_value( itemkind, 1 );
				city_changeprotect( g_actors[actor_index].city_index, ptsec, PATH_ITEMUSE );
			}
		}
	}
	else
	{
		return -1;
	}

	// 消耗钻石
	actor_change_token( actor_index, -token * itemnum, PATH_ITEMUSE, itemkind );

	// 记录物品使用日志
	wlog( 0, LOGOP_ITEMLOST, PATH_TOKENITEMUSE, itemkind, 1, 0, g_actors[actor_index].actorid, 0 );

	// 发送使用道具信息
	int Value[3] = {0};
	Value[0] = 0;
	Value[1] = itemkind;
	Value[2] = itemnum;
	actor_notify_value( actor_index, NOTIFY_ITEM, 3, Value, NULL );
	return 0;
}

// 道具通用加速
int item_use_quick( int actor_index, short itemkind, char op, int buildingkind, int buildingoffset )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( itemkind <= 0 )
		return -1;
	int itemtype = item_gettype( itemkind );
	if ( itemtype != ITEM_TYPE_QUICK )
		return -1;
	int value1 = item_get_base_value( itemkind, 0 );
	if ( op == 1 )
	{ // 升级加速
		if ( value1 == -1 )
		{
			int sec = 0;
			if ( pCity->worker_kind == buildingkind && pCity->worker_offset == buildingoffset )
				sec = pCity->worker_sec;
			else if ( pCity->worker_kind_ex == buildingkind && pCity->worker_offset_ex == buildingoffset )
				sec = pCity->worker_sec_ex;
			int token = (int)ceil( (sec / 60 + 1) * global.upgradequick_token );
			if ( actor_change_token( actor_index, -token, PATH_BUILDING_UPGRADE, 0 ) < 0 )
				return -1;
			building_workerquick( actor_index, buildingkind, buildingoffset, sec );
		}
		else
		{
			if ( item_lost( actor_index, itemkind, 1, PATH_TRAIN ) < 0 )
				return -1;
			building_workerquick( actor_index, buildingkind, buildingoffset, value1 );
		}
	}
	else if ( op == 2 )
	{ // 募兵加速
		if ( value1 == -1 )
		{
			BuildingBarracks *barracks = buildingbarracks_getptr_kind( pCity->index, buildingkind );
			if ( !barracks )
				return -1;
			if ( barracks->trainsec <= 0 )
				return -1;
			int token = (int)ceil( (barracks->trainsec / 60 + 1) * global.upgradequick_token );
			if ( actor_change_token( actor_index, -token, PATH_TRAIN, 0 ) < 0 )
				return -1;
			city_train_quick( actor_index, buildingkind, barracks->trainsec );
		}
		else
		{
			if ( item_lost( actor_index, itemkind, 1, PATH_TRAIN ) < 0 )
				return -1;
			city_train_quick( actor_index, buildingkind, value1 );
		}
	}
	else
		return -1;
	

	// 发送使用加速道具信息
	int Value[3] = { 0 };
	Value[0] = 1;
	Value[1] = value1;
	actor_notify_value( actor_index, NOTIFY_ITEM, 3, Value, NULL );
	return 0;
}

//-----------------------------------------------------------------------------
// 函数说明: 丢弃道具
// 参数    : actor_index - 
//           itemindex	- 位置
//           dropcount	- 数量
//-----------------------------------------------------------------------------
int item_drop( int actor_index, short itemindex, short dropcount )
{
	Item * pitem = NULL;
	int itemtype;
	int price;

	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	if ( itemindex >= 0 && itemindex < MAX_ACTOR_ITEMNUM )
		pitem = &g_actors[actor_index].item[itemindex];
	else
		return -1;

	itemtype = item_gettype( pitem->m_kind );
	// 任务道具不可丢弃
	//if ( itemtype == ITEM_TYPE_QUEST )
	//{
	//	return -1;
	//}

	if ( dropcount > pitem->m_num )
	{
		dropcount = pitem->m_num;
	}
	price =  item_getprice( pitem->m_kind );
	if ( price <= 0 )
		price = 1;
	price *= dropcount;

	city_changesilver( g_actors[actor_index].city_index, price, PATH_SELL );

	// 记录log
	wlog( 0, LOGOP_ITEMLOST, PATH_SELL, pitem->m_kind, dropcount, pitem->m_num, g_actors[actor_index].actorid, city_mainlevel( g_actors[actor_index].city_index ) );

	pitem->m_num -= dropcount;
	// 如果数量为0，那么就删掉
	if ( pitem->m_num <= 0 )
	{
		item_deletebox( actor_index, itemindex );
		memset( &g_actors[actor_index].item[itemindex], 0, sizeof(Item) );
	}
	// 发送角色失去物品
	item_sendlost( actor_index, -1, itemindex, dropcount, PATH_SELL );
	return 0;
}

// 整理背包使用，判断顺序
static unsigned int item_get_order( int item_type, short item_level )
{
	unsigned int rt = 0;
	int order_rule[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	int order_count;
	int tmpi;

	if ( item_type < 0 )
		return 0xffffffff; // 如果没有,是个空格子,则返回最大数

	order_count = sizeof(order_rule) / sizeof(order_rule[0]);
	for ( tmpi = 0; tmpi < order_count; tmpi++ )
	{
		if ( item_type == order_rule[tmpi] )
		{
			rt = tmpi;
			break;
		}
	}
	if ( tmpi >= order_count )
	{
		rt = order_count + 1 + item_type;
	}
	return (rt << 16) | item_level;
}

// 背包道具合并
static int item_find_nextkind( Item *pItem, int itemkind, int curindex, int max_itemnum )
{
	if ( pItem == NULL || curindex >= max_itemnum || curindex < 0 )
		return -1;
	for ( int tmpi = curindex; tmpi < max_itemnum; tmpi++ )
	{
		if ( pItem[tmpi].m_num > 0 && pItem[tmpi].m_kind == itemkind )
		{
			return tmpi;
		}
	}
	return -1;
}

int item_packeg_in( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	Item *pItem = g_actors[actor_index].item;

	int max_itemnum = MAX_DEFAULT_ITEMNUM + g_actors[actor_index].itemext;

	if ( max_itemnum > MAX_ACTOR_ITEMNUM )
		max_itemnum = MAX_ACTOR_ITEMNUM;

	int overlap;
	int curindex;
	int nextindex;
	char haschange;
	int value[2];
	for ( int tmpi = 0; tmpi < max_itemnum; tmpi++ )
	{
		if ( pItem[tmpi].m_num <= 0 || pItem[tmpi].m_kind <= 0 )
			continue;
		overlap = item_get_overlap( pItem[tmpi].m_kind );
		if ( overlap <= 1 || pItem[tmpi].m_num >= overlap )
			continue;
		curindex = tmpi + 1;
		haschange = 0;
		while ( curindex < max_itemnum )
		{
			nextindex = item_find_nextkind( pItem, pItem[tmpi].m_kind, curindex, max_itemnum );
			if ( nextindex < curindex || nextindex >= max_itemnum )
				break;
			if ( pItem[nextindex].m_num + pItem[tmpi].m_num >= overlap )
			{
				haschange = 1;
				pItem[nextindex].m_num = pItem[nextindex].m_num + pItem[tmpi].m_num - overlap;
				pItem[tmpi].m_num = overlap;
				if ( pItem[nextindex].m_num <= 0 )
				{
					item_deletebox( actor_index, nextindex );
					pItem[nextindex].m_kind = 0;
				}
				value[0] = nextindex;
				value[1] = pItem[nextindex].m_num;
				actor_notify_value( actor_index, NOTIFY_ITEMNUM, 2, value, NULL );
			}
			else
			{
				pItem[tmpi].m_num += pItem[nextindex].m_num;
				haschange = 1;

				item_deletebox( actor_index, nextindex );
				pItem[nextindex].m_kind = 0;
				pItem[nextindex].m_num = 0;

				value[0] = nextindex;
				value[1] = pItem[nextindex].m_num;
				actor_notify_value( actor_index, NOTIFY_ITEMNUM, 2, value, NULL );
			}
			curindex = nextindex + 1;
		}
		if ( haschange )
		{
			value[0] = tmpi;
			value[1] = pItem[tmpi].m_num;
			actor_notify_value( actor_index, NOTIFY_ITEMNUM, 2, value, NULL );
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 函数说明: 装备整理
// 参数    : actor_index - 
//           itemindex	- 位置
//           dropcount	- 数量
//-----------------------------------------------------------------------------
int item_settle( int actor_index )
{
	Item tmp_item = { 0 };
	int new_index[MAX_ACTOR_ITEMNUM] = { 0 };
	int tmp_index = 0;
	int tmpi, tmpj;
	Item *pItemBase = NULL;

	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	pItemBase = g_actors[actor_index].item;

	item_packeg_in( actor_index );

	int max_itemnum = MAX_DEFAULT_ITEMNUM + g_actors[actor_index].itemext;
	if ( max_itemnum > MAX_ACTOR_ITEMNUM )
		max_itemnum = MAX_ACTOR_ITEMNUM;

	for ( tmpi = 0; tmpi < max_itemnum; tmpi++ )
	{
		new_index[tmpi] = tmpi;
	}

	for ( tmpi = 0; tmpi < max_itemnum; tmpi++ )
	{
		for ( tmpj = 0; tmpj < max_itemnum - 1; tmpj++ )
		{
			int item_kind_1 = 0, item_kind_2 = 0;
			int item_type_1 = -1, item_type_2 = -1;
			short item_level_1 = 0, item_level_2 = 0;
			unsigned order_1 = 0, order_2 = 0;

			item_kind_1 = pItemBase[tmpj].m_kind;
			if ( item_kind_1 > 0 )
			{
				item_type_1 = item_gettype( item_kind_1 );
				item_level_1 = item_getlevel( item_kind_1 );
			}
			item_kind_2 = pItemBase[tmpj + 1].m_kind;
			if ( item_kind_2 > 0 )
			{
				item_type_2 = item_gettype( item_kind_2 );
				item_level_2 = item_getlevel( item_kind_2 );
			}
			order_1 = item_get_order( item_type_1, item_level_1 );
			order_2 = item_get_order( item_type_2, item_level_2 );
			if ( (order_1 > order_2) ||
				(order_1 == order_2 && item_kind_1 > item_kind_2) )
			{ // 要调换位置了
				tmp_item = pItemBase[tmpj];
				pItemBase[tmpj] = pItemBase[tmpj + 1];
				pItemBase[tmpj + 1] = tmp_item;

				tmp_index = new_index[tmpj];
				new_index[tmpj] = new_index[tmpj + 1];
				new_index[tmpj + 1] = tmp_index;
			}
		}
	}

	SLK_NetS_ItemSettle Value = {};
	Value.m_itemnum = max_itemnum;
	for ( tmpi = 0; tmpi < max_itemnum; tmpi++ )
	{
		Value.m_itemoffset[tmpi] = new_index[tmpi];
	}
	netsend_itemsettle_S( actor_index, SENDTYPE_ACTOR, &Value );

	return 0;
}
