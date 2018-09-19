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
#include "actor_times.h"
#include "server_netsend_auto.h"
#include "script_auto.h"
#include "mapunit.h"
#include "city.h"
#include "city_attr.h"
#include "global.h"
#include "building.h"
#include "vip.h"
#include "auto_data_item.h"

extern MYSQL *myData;
extern MYSQL *myGame;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern ItemKind *g_itemkind;
extern int g_itemkind_maxnum;

//-----------------------------------------------------------------------------
// ����˵��: ���ߵ�ʹ��
// ����    : actor_index - 
//           hero_index	- Ӣ��index
//           target_index	- Ŀ��index
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

	// �����Ʒitemkind
	itemkind = g_actors[actor_index].item[itemindex].m_kind;
	if ( g_actors[actor_index].item[itemindex].m_kind <= 0 )
		return -1;

	// ��ȡ��Ʒ����
	item_type = item_gettype( itemkind );

	// ���ȼ�
	item_level = item_getlevel( itemkind );
	if ( item_level > 0 )
	{ // ���ǵȼ�
		if ( city_mainlevel( g_actors[actor_index].city_index ) < -item_level )
			return -1;
	}
	else if ( item_level < 0 )
	{ // ��ɫ�ȼ�
		if ( pActor->level < item_level )
			return -1;
	}

	// ʹ�ø���
	if ( itemnum == 0 )// ȫ��
		true_usenum = g_actors[actor_index].item[itemindex].m_num;
	else if ( itemnum > 0 )
		true_usenum = itemnum;
	else
		return -1;

	// ʹ��������Ʒ����Ʒ��ʵʹ������
	if ( g_actors[actor_index].item[itemindex].m_num < true_usenum )
	{
		true_usenum = g_actors[actor_index].item[itemindex].m_num;
	}
	if ( true_usenum <= 0 )
		return -1;

	// ��������Ʒ�Ƿ��ǽ�ֹһ��ʹ�ö����
	if ( true_usenum > 1 )
	{
		if ( item_getsituation( itemkind ) & ITEM_SITUATION_NOUSEMORE )
			return -1;
	}

	// ��ͨ��Ʒ�������ͽ��в�ͬ��ʹ�÷�ʽ
	if ( item_type == ITEM_TYPE_NORMAL_USE )
	{ // ���ʹ�������
		int ability1 = item_get_base_ability( g_actors[actor_index].item[itemindex].m_kind, 0 );
		int value1 = item_get_base_value( g_actors[actor_index].item[itemindex].m_kind, 0 );
		if ( ability1 == ITEM_ABILITY_AWARDGROUP )
		{ // �����
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
		{ // �ű�����ID

		}
		else if ( ability1 == ITEM_ABILITY_ADDBODY )
		{ // ������
			int usetimes = actor_get_today_char_times( actor_index, TODAY_CHAR_ITEMUSE_BODY );
			int lefttimes = 20 - usetimes;
			true_usenum = min( true_usenum, lefttimes );
			if ( true_usenum <= 0 )
			{
				actor_notify_alert( actor_index, 2323 );
				return -1;
			}
			actor_set_today_char_times( actor_index, TODAY_CHAR_ITEMUSE_BODY, usetimes + true_usenum );
			city_changebody( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_ADDEXP )
		{ // ����������
			city_actorexp( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_ADDHEROEXP )
		{ // ��Ӣ�۾���

		}
		else if ( ability1 == ITEM_ABILITY_CITYRES_SILVER )
		{ // ��������
			city_changesilver( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_CITYRES_WOOD )
		{ // ľ������
			city_changewood( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_CITYRES_FOOD )
		{ // ��ʳ����
			city_changefood( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_CITYRES_IRON )
		{ // ������
			city_changeiron( g_actors[actor_index].city_index, value1*true_usenum, PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_BUFF )
		{ // buff
			int buffsec = item_get_base_value( itemkind, 1 );
			if ( value1 == 100 )
			{ // �ǳر���ʱ��
				city_changeprotect( g_actors[actor_index].city_index, buffsec, PATH_ITEMUSE );
			}
			else if ( value1 >= 0  && value1 < CITY_BUFF_MAX )
			{
				city_change_buff( g_actors[actor_index].city_index, value1, buffsec, PATH_ITEMUSE );
			}
			
		}
		else if ( ability1 == ITEM_ABILITY_CITYMOVE )
		{ // �ͼ�Ǩ��
			return city_move_actor( actor_index, -1, -1, itemkind );
		}
		else if ( ability1 == ITEM_ABILITY_CORPS1 )
		{ // �Ӳ���
			city_changesoldiers( g_actors[actor_index].city_index, 0, item_get_base_value( itemkind, 0 ), PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_CORPS2 )
		{ // �����
			city_changesoldiers( g_actors[actor_index].city_index, 1, item_get_base_value( itemkind, 0 ), PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_CORPS3 )
		{ // �ӹ���
			city_changesoldiers( g_actors[actor_index].city_index, 2, item_get_base_value( itemkind, 0 ), PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_VIPEXP )
		{ // vip ����
			vip_exp( g_actors[actor_index].city_index, item_get_base_value( itemkind, 0 ), PATH_ITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_AWARDID )
		{ // ����id
			int awardkind = item_get_base_value( itemkind, 0 );
			int awardnum = item_get_base_value( itemkind, 1 );
			award_getaward( actor_index, awardkind, awardnum, -1, PATH_ITEMUSE, NULL );
		}
	}
	else
		return item_sendnotuse( actor_index, itemindex, -2 );

	// ��ȥ����
	g_actors[actor_index].item[itemindex].m_num -= true_usenum;

	// ��¼��Ʒʹ����־
	wlog( 0, LOGOP_ITEMLOST, PATH_ITEMUSE, g_actors[actor_index].item[itemindex].m_kind, true_usenum, g_actors[actor_index].item[itemindex].id, g_actors[actor_index].actorid, city_mainlevel( g_actors[actor_index].city_index ) );

	// �������Ϊ0����ô��ɾ��
	if ( g_actors[actor_index].item[itemindex].m_num <= 0 )
	{
		item_deletebox( actor_index, itemindex );
		memset( &g_actors[actor_index].item[itemindex], 0, sizeof(Item) );
	}

	// ����ʹ����Ʒ���
	SLK_NetS_ItemUse Value = {};
	Value.m_itemoffset = itemindex;
	Value.m_usenum = true_usenum;
	Value.m_effres = eff_result;
	netsend_itemuse_S( actor_index, SENDTYPE_ACTOR, &Value );
	return eff_result;
}

//-----------------------------------------------------------------------------
// ����˵��: ֱ�ӻ�Ǯ���ߵ�ʹ��
// ����    : actor_index - 
//           hero_index	- Ӣ��index
//           target_index	- Ŀ��index
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

	// ��ȡ��Ʒ����
	item_type = item_gettype( itemkind );

	pActor = &g_actors[actor_index];

	// ���ȼ�
	item_level = item_getlevel( itemkind );
	if ( item_level > 0 )
	{ // ���ǵȼ�
		if ( city_mainlevel( g_actors[actor_index].city_index ) < -item_level )
			return -1;
	}
	else if ( item_level < 0 )
	{ // ��ɫ�ȼ�
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
	{ // ���ʹ�������
		int ability1 = item_get_base_ability( itemkind, 0 );
		int value1 = item_get_base_value( itemkind, 0 );
		if ( ability1 == ITEM_ABILITY_ADDBODY )
		{ // ������
			int usetimes = actor_get_today_char_times( actor_index, TODAY_CHAR_ITEMUSE_BODY );
			int lefttimes = 20 - usetimes;
			true_usenum = min( true_usenum, lefttimes );
			if ( true_usenum <= 0 )
			{
				actor_notify_alert( actor_index, 2323 );
				return -1;
			}
			actor_set_today_char_times( actor_index, TODAY_CHAR_ITEMUSE_BODY, usetimes + true_usenum );
			city_changebody( g_actors[actor_index].city_index, value1*true_usenum, PATH_TOKENITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_ADDEXP )
		{ // ����������
			city_actorexp( g_actors[actor_index].city_index, value1*true_usenum, PATH_TOKENITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_ADDHEROEXP )
		{ // ��Ӣ�۾���

		}
		else if ( ability1 == ITEM_ABILITY_CITYRES_SILVER )
		{ // ��������
			city_changesilver( g_actors[actor_index].city_index, value1*true_usenum, PATH_TOKENITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_CITYRES_WOOD )
		{ // ľ������
			city_changewood( g_actors[actor_index].city_index, value1*true_usenum, PATH_TOKENITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_CITYRES_FOOD )
		{ // ��ʳ����
			city_changefood( g_actors[actor_index].city_index, value1*true_usenum, PATH_TOKENITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_CITYRES_IRON )
		{ // ������
			city_changeiron( g_actors[actor_index].city_index, value1*true_usenum, PATH_TOKENITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_BUFF )
		{ // buff
			int buffsec = item_get_base_value( itemkind, 1 );
			if ( value1 == 100 )
			{ // �ǳر���ʱ��
				city_changeprotect( g_actors[actor_index].city_index, buffsec, PATH_TOKENITEMUSE );
			}
			else if ( value1 >= 0 && value1 < CITY_BUFF_MAX )
			{
				city_change_buff( g_actors[actor_index].city_index, value1, buffsec, PATH_TOKENITEMUSE );
			}
		}
		else if ( ability1 == ITEM_ABILITY_CITYMOVE )
		{ // �ͼ�Ǩ��
			return city_move_actor( actor_index, -1, -1, itemkind );
		}
		else if ( ability1 == ITEM_ABILITY_CORPS1 )
		{ // �Ӳ���
			city_changesoldiers( g_actors[actor_index].city_index, 0, item_get_base_value( itemkind, 0 ), PATH_TOKENITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_CORPS2 )
		{ // �����
			city_changesoldiers( g_actors[actor_index].city_index, 1, item_get_base_value( itemkind, 0 ), PATH_TOKENITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_CORPS3 )
		{ // �ӹ���
			city_changesoldiers( g_actors[actor_index].city_index, 2, item_get_base_value( itemkind, 0 ), PATH_TOKENITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_VIPEXP )
		{ // vip ����
			vip_exp( g_actors[actor_index].city_index, item_get_base_value( itemkind, 0 ), PATH_TOKENITEMUSE );
		}
		else if ( ability1 == ITEM_ABILITY_AWARDID )
		{ // ����id
			int awardkind = item_get_base_value( itemkind, 0 );
			int awardnum = item_get_base_value( itemkind, 1 );
			award_getaward( actor_index, awardkind, awardnum, -1, PATH_TOKENITEMUSE, NULL );
		}
	}
	else
	{
		return -1;
	}

	// ������ʯ
	actor_change_token( actor_index, -token * itemnum, PATH_TOKENITEMUSE, itemkind );

	// ��¼��Ʒʹ����־
	wlog( 0, LOGOP_ITEMLOST, PATH_TOKENITEMUSE, itemkind, 1, 0, g_actors[actor_index].actorid, 0 );

	// ����ʹ�õ�����Ϣ
	int Value[3] = {0};
	Value[0] = 0;
	Value[1] = itemkind;
	Value[2] = itemnum;
	actor_notify_value( actor_index, NOTIFY_ITEM, 3, Value, NULL );
	return 0;
}

// ����ͨ�ü���
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
	if ( op == 1 || op == 3 )
	{ // ��������
		if ( value1 == -1 )
		{
			int sec = 0;
			if ( pCity->worker_kind == buildingkind )
			{
				sec = pCity->worker_sec;
				if ( buildingkind >= BUILDING_Silver && buildingkind <= BUILDING_Iron )
				{
					if ( pCity->worker_offset != buildingoffset )
					{
						sec = 0;
					}
				}
			}
			if ( pCity->worker_kind_ex == buildingkind )
			{
				sec = pCity->worker_sec_ex;
				if ( buildingkind >= BUILDING_Silver && buildingkind <= BUILDING_Iron )
				{
					if ( pCity->worker_offset_ex != buildingoffset )
					{
						sec = 0;
					}
				}
			}
			int token = (int)ceil( (sec / 60 + 1) * global.upgradequick_token );
			if ( actor_change_token( actor_index, -token, PATH_BUILDING_UPGRADE, 0 ) < 0 )
				return -1;
			building_workerquick( actor_index, buildingkind, buildingoffset, sec );
		}
		else
		{
			if ( item_lost( actor_index, itemkind, 1, PATH_BUILDING_UPGRADE ) < 0 )
			{
				int token = item_gettoken( itemkind );
				if ( actor_change_token( actor_index, -token, PATH_BUILDING_UPGRADE, 0 ) < 0 )
					return -1;
			}
			building_workerquick( actor_index, buildingkind, buildingoffset, value1 );
		}
	}
	else if ( op == 2 )
	{ // ļ������
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
			{
				int token = item_gettoken( itemkind );
				if ( actor_change_token( actor_index, -token, PATH_TRAIN, 0 ) < 0 )
					return -1;
			}
			city_train_quick( actor_index, buildingkind, value1 );
		}
	}
	else
		return -1;
	

	// ����ʹ�ü��ٵ�����Ϣ
	int Value[3] = { 0 };
	Value[0] = 1;
	Value[1] = value1;
	actor_notify_value( actor_index, NOTIFY_ITEM, 3, Value, NULL );
	return 0;
}

//-----------------------------------------------------------------------------
// ����˵��: ��������
// ����    : actor_index - 
//           itemindex	- λ��
//           dropcount	- ����
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
	// ������߲��ɶ���
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

	// ��¼log
	wlog( 0, LOGOP_ITEMLOST, PATH_SELL, pitem->m_kind, dropcount, pitem->m_num, g_actors[actor_index].actorid, city_mainlevel( g_actors[actor_index].city_index ) );

	pitem->m_num -= dropcount;
	// �������Ϊ0����ô��ɾ��
	if ( pitem->m_num <= 0 )
	{
		item_deletebox( actor_index, itemindex );
		memset( &g_actors[actor_index].item[itemindex], 0, sizeof(Item) );
	}
	// ���ͽ�ɫʧȥ��Ʒ
	item_sendlost( actor_index, -1, itemindex, dropcount, PATH_SELL );
	return 0;
}

// ��������Ӧ���߲���-����
int item_buy( int actor_index, short itemkind, short itemnum )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( itemkind <= 0 || itemkind >= g_itemkind_maxnum )
		return -1;
	int token = item_gettoken( itemkind );
	if ( token <= 0 )
		return -1;
	token *= itemnum;

	if ( actor_change_token( actor_index, -token, PATH_TOKENITEMUSE, 0 ) < 0 )
		return -1;
	item_getitem( actor_index, itemkind, itemnum, -1, PATH_TOKENITEMUSE );
	return 0;
}

// ������ʹ�ã��ж�˳��
static unsigned int item_get_order( int item_type, short item_level )
{
	unsigned int rt = 0;
	int order_rule[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	int order_count;
	int tmpi;

	if ( item_type < 0 )
		return 0xffffffff; // ���û��,�Ǹ��ո���,�򷵻������

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

// �������ߺϲ�
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
// ����˵��: װ������
// ����    : actor_index - 
//           itemindex	- λ��
//           dropcount	- ����
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
			{ // Ҫ����λ����
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
