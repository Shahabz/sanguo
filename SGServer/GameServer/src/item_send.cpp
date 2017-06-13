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
#include "global.h"
#include "script_auto.h"


extern MYSQL *myData;
extern MYSQL *myGame;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern ItemKind *g_itemkind;
extern int g_itemkind_maxnum;

// 发送使用失败信息
int item_sendnotuse( int actor_index, short itemindex, int err )
{
	if ( err > 0 )
		return -1;

	SLK_NetS_ItemUse Value = {};
	Value.m_itemoffset = itemindex;
	Value.m_usenum = err;
	Value.m_effres = 0;
	netsend_itemuse_S( actor_index, SENDTYPE_ACTOR, &Value );
	return err;
}

// 发送丢失物品信息
void item_sendlost( int actor_index, int target_index, short item_index, short item_num, char path )
{

	SLK_NetS_LostItem Value = {};
	Value.m_itemoffset = item_index;
	Value.m_itemnum = item_num;
	Value.m_path = path;
	if ( target_index < 0 )
		Value.m_targetid = -1;
	else
		Value.m_targetid = g_actors[target_index].actorid;

	netsend_lostitem_S( actor_index, SENDTYPE_ACTOR, &Value );
}

// 发送获取物品信息
void item_sendget( int actor_index, int target_index, short item_index, short item_num, char path )
{
	Item *pitem = NULL;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return;
	pitem = item_getptr( actor_index, item_index );
	if ( pitem == NULL )
		return;

	SLK_NetS_GetItem Value = {};
	Value.m_itemoffset = item_index;
	Value.m_kind = pitem->m_kind;
	Value.m_num = item_num;
	Value.m_color = pitem->m_color_level;
	Value.m_type = item_gettype( pitem->m_kind );
	Value.m_situation = item_getsituation( pitem->m_kind );
	Value.m_path = path;
	if ( target_index < 0 )
		Value.m_targetid = -1;
	else
		Value.m_targetid = g_actors[target_index].actorid;
	netsend_getitem_S( actor_index, SENDTYPE_ACTOR, &Value );
}

// 发送背包道具列表
int item_list( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	int max_itemnum = MAX_DEFAULT_ITEMNUM + g_actors[actor_index].itemext;
	if ( max_itemnum > MAX_ACTOR_ITEMNUM )
		max_itemnum = MAX_ACTOR_ITEMNUM;

	// 道具列表,用于显示的简单信息
	SLK_NetS_ItemList Value = {0};
	Value.m_kindnum = 0;
	for ( int tmpi = 0; tmpi < max_itemnum; tmpi++ )
	{
		if ( g_actors[actor_index].item[tmpi].m_kind <= 0 || g_actors[actor_index].item[tmpi].m_num <= 0 )
			continue;
		Value.m_item[Value.m_kindnum].m_offset = tmpi;
		Value.m_item[Value.m_kindnum].m_kind = g_actors[actor_index].item[tmpi].m_kind;
		Value.m_item[Value.m_kindnum].m_num = g_actors[actor_index].item[tmpi].m_num;
		Value.m_item[Value.m_kindnum].m_situation = item_getsituation( g_actors[actor_index].item[tmpi].m_kind );
		Value.m_item[Value.m_kindnum].m_color_level = g_actors[actor_index].item[tmpi].m_color_level;
		Value.m_kindnum++;
		if ( Value.m_kindnum >= 150 )
		{
			netsend_itemlist_S( actor_index, SENDTYPE_ACTOR, &Value );
			Value.m_kindnum = 0;
		}
	}
	netsend_itemlist_S( actor_index, SENDTYPE_ACTOR, &Value );
	return 0;
}

// 发送装备列表
int item_equip_list( int actor_index )
{
	int beginindex = 0;
	int max_itemnum = 0;
	int offsetbase = 0;
	Item *pEquip = NULL;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	
	beginindex = 0;
	max_itemnum = MAX_ACTOR_EQUIPNUM;
	offsetbase = EQUIP_OFFSETBASE;
	pEquip = g_actors[actor_index].equip;

	// 装备列表
	SLK_NetS_ItemList Value = { 0 };
	Value.m_kindnum = 0;
	for ( int tmpi = beginindex; tmpi < beginindex + MAX_ACTOR_EQUIPNUM; tmpi++ )
	{
		if ( pEquip[tmpi].m_kind <= 0 || pEquip[tmpi].m_num <= 0 )
			continue;
		Value.m_item[Value.m_kindnum].m_offset = offsetbase + tmpi;
		Value.m_item[Value.m_kindnum].m_kind = pEquip[tmpi].m_kind;
		Value.m_item[Value.m_kindnum].m_num = pEquip[tmpi].m_num;
		Value.m_item[Value.m_kindnum].m_color_level = pEquip[tmpi].m_color_level;
		Value.m_kindnum++;
	}
	netsend_itemlist_S( actor_index, SENDTYPE_ACTOR, &Value );
	return 0;
}

// 发送道具详细信息
int item_info( int actor_index, short itemindex )
{
	Item *pItem = NULL;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	pItem = item_getptr( actor_index, itemindex );
	if ( pItem == NULL )
		return -1;

	return item_info_withitem( pItem, actor_index, itemindex );
}

// 真正的发送道具信息函数
int item_info_withitem( Item *pItem, int actor_index, short itemindex )
{
	int tmpi;
	short arr_ability[ITEM_ABILITY_NUM] = { 0 };
	int arr_value[ITEM_ABILITY_NUM] = { 0 };
	int item_type = item_gettype( pItem->m_kind );

	SLK_NetS_ItemInfo Value = {};
	Value.m_itemoffset = itemindex;
	Value.m_itemkind = pItem->m_kind;
	Value.m_type = item_type;
	Value.m_price = item_getprice( pItem->m_kind );
	Value.m_level = item_getlevel( pItem->m_kind );
	Value.m_color_level = pItem->m_color_level;
	
	Value.m_attr_num = 0;
	// 基础属性
	for ( tmpi = 0; tmpi < 2; tmpi++ )
	{
		if ( item_get_base_ability( pItem->m_kind, tmpi ) <= 0 )
			continue;
		Value.m_attr[Value.m_attr_num].m_type = 0;
		Value.m_attr[Value.m_attr_num].m_ability = item_get_base_ability( pItem->m_kind, tmpi );
		Value.m_attr[Value.m_attr_num].m_value = item_get_base_value( pItem->m_kind, tmpi );
		Value.m_attr[Value.m_attr_num].m_addvalue = 0;
		Value.m_attr_num++;
	}
	// 动态属性
	for ( tmpi = 0; tmpi < ITEM_ABILITY_NUM; tmpi++ )
	{
		if ( pItem->m_ability[tmpi] <= 0 )
			continue;
		Value.m_attr[Value.m_attr_num].m_type = 1;
		Value.m_attr[Value.m_attr_num].m_ability = pItem->m_ability[tmpi];
		Value.m_attr[Value.m_attr_num].m_value = pItem->m_value[tmpi];
		Value.m_attr[Value.m_attr_num].m_addvalue = arr_value[tmpi];
		Value.m_attr_num++;
	}
	netsend_iteminfo_S( actor_index, SENDTYPE_ACTOR, &Value );
	return 0;
}
