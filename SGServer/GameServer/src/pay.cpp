#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#include <time.h>
#endif
#include <mysql.h>
#include "actor.h"
#include "db.h"
#include "pay.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "server_netsend_auto.h"
#include "client.h"
#include "gamelog.h"
#include "login.h"
#include "activity.h"
#include "mail.h"
#include "login.h"
#include "system.h"
#include "actor_times.h"
#include "script_auto.h"
#include "city.h"
#include "global.h"
#include "activity.h"
#include "activity_04.h"
#include "vip.h"
#include "quest.h"
#include "auto_data_platinfo.h"
#include "auto_data_paygoods.h"
#include "auto_data_paystore.h"

extern Actor *g_actors;
extern int g_maxactornum;
extern MYSQL *myGame;
extern MYSQL *myData;

extern SConfig g_Config;
extern Global global;

extern City * g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern AwardGroup *g_awardgroup;
extern int g_awardgroup_count;

extern ActivityItem *g_activity_item;
extern int g_activity_count;

extern int g_serverpoint;
extern int g_server_citylevel5_count;

extern PlatInfo *g_platinfo;
extern int g_platinfo_maxnum;

PayPrice *g_PayPrice = NULL;
int g_PayPriceCount = 0;

PayBag g_paybag[CITY_BAG_MAX] = { 0 };

extern PayGoods *g_paygoods;
extern int g_paygoods_maxnum;

extern PayStore *g_paystore;
extern int g_paystore_maxnum;

// 获取货币简称对应的索引
int paycoin_getindex_withplat( int platid )
{
	if ( platid == 27 || platid == 28 || platid == 38 || platid == 31 || platid == 32 )
	{ // 国外
		return 0;
	}
	else
	{ // 国内
		return 1;
	}
	return 0;
}

int payprice_init()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];

	// 获得表格中最大的任务ID
	sprintf( szSQL, "select max(Tier) from payprice" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	if ( (row = mysql_fetch_row( res )) )
	{
		if ( row[0] )
			g_PayPriceCount = atoi( row[0] ) + 1;
		else
			g_PayPriceCount = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	// 分配空间
	g_PayPrice = (PayPrice *)malloc( sizeof(PayPrice)*g_PayPriceCount );
	memset( g_PayPrice, 0, sizeof(PayPrice)*g_PayPriceCount );

	sprintf( szSQL, "select * from payprice" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );

	while ( (row = mysql_fetch_row( res )) )
	{
		int index = 0;
		int tier = atoi( row[index++] );
		if ( tier <= 0 || tier >= g_PayPriceCount )
		{
			continue;
		}

		for ( int coin = 0; coin < PAYCOINMAX; coin++ )
		{
			g_PayPrice[tier].price[coin] = atoi( row[index++] );
		}
	}
	mysql_free_result( res );
	return 0;
}

// 本服务器推送的礼包
int paybag_load()
{
	MYSQL_RES *res;
	MYSQL_ROW  row;
	char szSQL[512];
	int offset = 0;
	sprintf( szSQL, "select goodsid, begintime, endtime, count, pushcity from pay_baglist where '%d'>=`begintime` and '%d'<`endtime`", (int)time( NULL ), (int)time( NULL ) );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( row = mysql_fetch_row( res ) )
	{
		g_paybag[offset].goodsid = atoi( row[0] );
		g_paybag[offset].begintime = atoi( row[1] );
		g_paybag[offset].endtime = atoi( row[2] );
		g_paybag[offset].count = atoi( row[3] );
		g_paybag[offset].pushcity = atoi( row[4] );
		offset += 1;
		if ( offset >= CITY_BAG_MAX )
		{
			break;
		}
	}
	mysql_free_result( res );
	return 0;
}
int paybag_insert( short goodsid, int begintime, int endtime, int count )
{
	char szSQL[512];
	if ( goodsid <= 0 || goodsid >= g_paygoods_maxnum )
		return -1;
	if ( begintime <= 0 || endtime <= 0 || count <= 0 )
		return -1;

	int nowtime = (int)time( NULL );
	for ( int tmpi = 0; tmpi < CITY_BAG_MAX; tmpi++ )
	{
		if ( g_paybag[tmpi].goodsid > 0 && nowtime >= g_paybag[tmpi].endtime )
		{
			memset( &g_paybag[tmpi], 0, sizeof( PayBag ) );
		}
	}

	int offset = -1;
	for ( int tmpi = 0; tmpi < CITY_BAG_MAX; tmpi++ )
	{
		if ( g_paybag[tmpi].goodsid == goodsid )
		{
			offset = tmpi;
			break;
		}
	}

	if ( offset == -1 )
	{
		for ( int tmpi = 0; tmpi < CITY_BAG_MAX; tmpi++ )
		{
			if ( g_paybag[tmpi].goodsid <= 0 )
			{
				offset = tmpi;
				break;
			}
		}
	}
	if ( offset < 0 || offset >= CITY_BAG_MAX )
	{
		return -1;
	}

	// 推送到缓存
	g_paybag[offset].goodsid = goodsid;
	g_paybag[offset].begintime = begintime;
	g_paybag[offset].endtime = endtime;
	g_paybag[offset].count = count;
	g_paybag[offset].pushcity = 0;
	if ( nowtime >= begintime )
	{
		// 给所有玩家推送
		for ( int tmpi = 0; tmpi < g_city_maxcount; tmpi++ )
		{
			if ( g_city[tmpi].actorid < MINACTORID )
				continue;
			city_paybag_add( &g_city[tmpi], goodsid, endtime - (int)time( NULL ), count, 1 );
		}
		g_paybag[offset].pushcity = 1;
	}

	sprintf( szSQL, "replace into pay_baglist( goodsid, begintime, endtime, count, pushcity ) values( '%d','%d','%d','%d','%d' )", goodsid, begintime, endtime, count, g_paybag[offset].pushcity );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	return 0;
}
int paybag_delete( short goodsid )
{
	for ( int tmpi = 0; tmpi < CITY_BAG_MAX; tmpi++ )
	{
		if ( g_paybag[tmpi].goodsid == goodsid )
		{
			memset( &g_paybag[tmpi], 0, sizeof( PayBag ) );
			break;
		}
	}
	// 删除掉
	char szSQL[256];
	sprintf( szSQL, "delete from pay_baglist where goodsid='%d'", goodsid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
	}

	// 所有玩家删除这个礼包
	for ( int tmpi = 0; tmpi < g_city_maxcount; tmpi++ )
	{
		if ( g_city[tmpi].actorid < MINACTORID )
			continue;
		city_paybag_del( &g_city[tmpi], goodsid );
	}
	return 0;
}

void paybag_logic()
{
	int nowtime = (int)time( NULL );
	for ( int offset = 0; offset < CITY_BAG_MAX; offset++ )
	{
		if ( g_paybag[offset].goodsid <= 0 )
			continue;
		if ( g_paybag[offset].pushcity == 1 )
			continue;
		if ( nowtime >= g_paybag[offset].begintime )
		{
			// 给所有玩家推送
			for ( int tmpi = 0; tmpi < g_city_maxcount; tmpi++ )
			{
				if ( g_city[tmpi].actorid < MINACTORID )
					continue;
				city_paybag_add( &g_city[tmpi], g_paybag[offset].goodsid, g_paybag[offset].endtime - (int)time( NULL ), g_paybag[offset].count, 1 );
			}
			g_paybag[offset].pushcity = 1;

			char szSQL[256];
			sprintf( szSQL, "update pay_baglist set pushcity='%d' where goodsid='%d'", g_paybag[offset].pushcity, g_paybag[offset].goodsid );
			if ( mysql_query( myGame, szSQL ) )
			{
				printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
				write_gamelog( "%s", szSQL );
				if ( mysql_ping( myGame ) != 0 )
					db_reconnect_game();
			}
		}
	}
}

// 支付商店列表
int paystore_list( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int coinindex = paycoin_getindex_withplat( client_getplatid( actor_index ) );
	if ( coinindex < 0 || coinindex > PAYCOINMAX )
		coinindex = 0;
	int paymode = world_data_getcache( WORLD_DATA_PAYMODE );

	SLK_NetS_PayStore store = { 0 };
	store.m_paymode = (char)paymode;
	for ( short id = 1; id < g_paystore_maxnum; id++ )
	{
		short goodsid = g_paystore[id].goodsid;
		if ( goodsid <= 0 || goodsid >= g_paygoods_maxnum )
			continue;
		int tier = g_paygoods[goodsid].tier;
		if ( tier <= 0 || tier >= g_PayPriceCount )
			continue;
		store.m_list[store.m_count].m_goodsid = goodsid;
		store.m_list[store.m_count].m_price = (unsigned int)(g_PayPrice[tier].price[coinindex]);
		store.m_list[store.m_count].m_token = g_paygoods[goodsid].token;
		store.m_list[store.m_count].m_gift_token = g_paystore[id].gift_token;
		//// 永久限购
		//if ( g_paystore[id].limitbuy_count > 0 )
		//{
		//	int surplus = g_paystore[id].limitbuy_count - actor_limitybuy_getnum( actor_index, g_paystore[id].limitbuy_saveindex );
		//	if ( surplus < 0 )
		//		surplus = 0;
		//	if ( surplus > 0 )
		//	{
		//		store.m_list[store.m_count].m_limitbuy_gifttoken = g_paystore[id].limitbuy_gifttoken;
		//		store.m_list[store.m_count].m_limitbuy_surplus = surplus;
		//	}
		//}

		//// 每日限购
		//if ( g_paystore[id].everyday_count > 0 )
		//{
		//	int surplus = g_paystore[id].everyday_count - actor_everyday_limitybuy_getnum( actor_index, g_paystore[id].everyday_saveindex );
		//	if ( surplus < 0 )
		//		surplus = 0;
		//	if ( surplus > 0 )
		//	{
		//		store.m_list[store.m_count].m_everyday_gifttoken = g_paystore[id].everyday_gifttoken;
		//		store.m_list[store.m_count].m_everyday_surplus = surplus;
		//	}
		//}
		
		if ( g_paygoods[goodsid].type == PAY_GOODSTYPE_MONTHCARD )
		{// 月卡
			store.m_list[store.m_count].m_day = pCity->mcard;
		}
		else if ( g_paygoods[goodsid].type == PAY_GOODSTYPE_WEEKCARD )
		{// 周卡
			store.m_list[store.m_count].m_day = pCity->wcard;
		}
		store.m_list[store.m_count].m_nameid = g_paygoods[goodsid].nameid;
		store.m_list[store.m_count].m_icon = g_paygoods[goodsid].icon;
		store.m_count += 1;
	}
	
	netsend_paystore_S( actor_index, SENDTYPE_ACTOR, &store );
	return 0;
}

// 礼包信息
int pay_goodsinfo( int actor_index, int type, int goodsid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( goodsid <= 0 || goodsid >= g_paygoods_maxnum )
		return -1;

	int coinindex = paycoin_getindex_withplat( client_getplatid( actor_index ) );
	if ( coinindex < 0 || coinindex > PAYCOINMAX )
		coinindex = 0;

	SLK_NetS_GoodsInfo pValue = { 0 };
	int tier = g_paygoods[goodsid].tier;
	if ( tier <= 0 || tier >= g_PayPriceCount )
		return -1;

	pValue.m_goodsid = goodsid;
	pValue.m_price = g_PayPrice[tier].price[coinindex];
	pValue.m_nameid = g_paygoods[goodsid].nameid;
	pValue.m_descid = g_paygoods[goodsid].descid;
	pValue.m_icon = g_paygoods[goodsid].icon;
	pValue.m_sale = g_paygoods[goodsid].sale;
	pValue.m_worth = g_paygoods[goodsid].worth;
	pValue.m_bag_time = 0;
	pValue.m_bag_num = 0;
	int awardgroup = paygoods_getawardgroup( actor_index, goodsid );
	if ( awardgroup > 0 && awardgroup < g_awardgroup_count )
	{
		for ( int i = 0; i < g_awardgroup[awardgroup].allcount; i++ )
		{
			if ( g_awardgroup[awardgroup].show[i] == 0 )
				continue;
			int awardcount = pValue.m_awardcount;
			if ( awardcount >= 16 )
				break;
			pValue.m_award[awardcount].m_kind = g_awardgroup[awardgroup].kind[i];
			pValue.m_award[awardcount].m_num = g_awardgroup[awardgroup].minnum[i];
			pValue.m_awardcount += 1;
		}
	}
	netsend_goodsinfo_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 通过type获取ID列表
int paystore_goodslist_withtype( int type, int *goodsid, int count )
{
	if ( type < 0 || goodsid == NULL )
		return -1;
	int index = 0;
	for ( int tmpi = 0; tmpi < g_paygoods_maxnum; tmpi++ )
	{
		if ( g_paygoods[tmpi].type < 0 )
			continue;
		if ( g_paygoods[tmpi].type == type && index < count )
		{
			goodsid[index] = tmpi;
			index++;
		}
	}
	return 0;
}

// 获取商品编号
int paystore_get_productid( int platid, char paymode, int goodsid, char *out )
{
	char paystr[2] = { 0 };
	if ( paymode == 0 )
	{
		strncpy( paystr, g_platinfo[platid].paymode_sdk, 2 );
	}
	else
	{
		strncpy( paystr, g_platinfo[platid].paymode_web, 2 );
	}
	if ( strncmp( paystr, "a", 2 ) == 0 )
	{
		strncpy( out, g_paygoods[goodsid].productid_a, 63 );
	}
	else if ( strncmp( paystr, "b", 2 ) == 0 )
	{
		strncpy( out, g_paygoods[goodsid].productid_b, 63 );
	}
	else if ( strncmp( paystr, "c", 2 ) == 0 )
	{
		strncpy( out, g_paygoods[goodsid].productid_c, 63 );
	}
	else if ( strncmp( paystr, "d", 2 ) == 0 )
	{
		strncpy( out, g_paygoods[goodsid].productid_d, 63 );
	}
	else if ( strncmp( paystr, "e", 2 ) == 0 )
	{
		strncpy( out, g_paygoods[goodsid].productid_e, 63 );
	}
	else if ( strncmp( paystr, "f", 2 ) == 0 )
	{
		strncpy( out, g_paygoods[goodsid].productid_f, 63 );
	}
	else if ( strncmp( paystr, "g", 2 ) == 0 )
	{
		strncpy( out, g_paygoods[goodsid].productid_g, 63 );
	}
	else if ( strncmp( paystr, "h", 2 ) == 0 )
	{
		strncpy( out, g_paygoods[goodsid].productid_h, 63 );
	}
	else if ( strncmp( paystr, "i", 2 ) == 0 )
	{
		strncpy( out, g_paygoods[goodsid].productid_i, 63 );
	}
	else if ( strncmp( paystr, "j", 2 ) == 0 )
	{
		strncpy( out, g_paygoods[goodsid].productid_j, 63 );
	}
	else if ( strncmp( paystr, "k", 2 ) == 0 )
	{
		strncpy( out, g_paygoods[goodsid].productid_k, 63 );
	}
	else if ( strncmp( paystr, "l", 2 ) == 0 )
	{
		strncpy( out, g_paygoods[goodsid].productid_l, 63 );
	}
	else if ( strncmp( paystr, "m", 2 ) == 0 )
	{
		strncpy( out, g_paygoods[goodsid].productid_m, 63 );
	}
	else if ( strncmp( paystr, "n", 2 ) == 0 )
	{
		strncpy( out, g_paygoods[goodsid].productid_n, 63 );
	}
	else if ( strncmp( paystr, "o", 2 ) == 0 )
	{
		strncpy( out, g_paygoods[goodsid].productid_o, 63 );
	}
	return 0;
}

// 购买商品
int paystore_buy( int actor_index, int goodsid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( goodsid <= 0 || goodsid >= g_paygoods_maxnum )
		return -1;
	int platid = client_getplatid( actor_index );
	int coinindex = paycoin_getindex_withplat( platid );
	if ( coinindex < 0 || coinindex > PAYCOINMAX )
		coinindex = 0;
	int goodstype = g_paygoods[goodsid].type;
	// 发送去支付
	SLK_NetS_PayOrder info = { 0 };
	int paymode = world_data_getcache( WORLD_DATA_PAYMODE );
	paystore_get_productid( platid, paymode, goodsid, info.m_productid );
	info.m_productidlen = strlen( info.m_productid );

	// 生成一份订单
	// 订单构成：服务器ID_角色ID_时间随机值
	char pOrderID[32] = { 0 };
	snprintf( pOrderID, 32, "%d_%d_%d%d%d%d", g_Config.server_code, g_actors[actor_index].actorid, (int)time(NULL), rand() % 10, rand() % 10, rand() % 10 );

	// 订单插入数据库，支付成功以后以订单为准
	if ( paystore_addorder( actor_index, goodsid, pOrderID ) < 0 )
		return -1;

	// 透传参数构成：服务器编号#角色编号#平台产品编号#游戏商品编号
	char pOrderExt[64] = { 0 };
	snprintf( pOrderExt, 64, "%d#%d#%s#%d", g_Config.server_code, g_actors[actor_index].actorid, info.m_productid, goodsid );

	strncpy( info.m_orderid, pOrderID, 32 );
	info.m_orderid_len = strlen( info.m_orderid );

	strncpy( info.m_ext, pOrderExt, 64 );
	info.m_ext_len = strlen( info.m_ext );
	info.m_paymode = (char)paymode;

	info.m_goodsid = goodsid;
	info.m_nameid = g_paygoods[goodsid].nameid;
	info.m_descid = g_paygoods[goodsid].descid;
	info.m_price = (int)( g_PayPrice[g_paygoods[goodsid].tier].price[coinindex] );
	netsend_payorder_S( actor_index, SENDTYPE_ACTOR, &info );
	return 0;
}

// 添加订单
int paystore_addorder( int actor_index, int goodsid, char *pOrderID )
{
	char szSQL[2048];
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( goodsid <= 0 || goodsid >= g_paygoods_maxnum )
		return -1;
	if ( pOrderID == NULL )
		return -1;

	char szUserID[21];
	i64 userid = client_getuserid( actor_index );
	lltoa( userid, szUserID, 10 );

	int awardgroup = paygoods_getawardgroup( actor_index, goodsid );
	int paymode = world_data_getcache( WORLD_DATA_PAYMODE );
	char productid[64] = { 0 };
	paystore_get_productid( client_getplatid( actor_index ), paymode, goodsid, productid );
	sprintf( szSQL, "insert into pay_order( orderid, platid, userid, actorid, actorlevel, citylevel, productid, goodsid, awardgroup, ip, status, optime ) values( '%s','%d','%s','%d','%d','%d','%s','%d','%d','%s','%d','%d' )", 
		pOrderID, client_getplatid( actor_index ), szUserID, g_actors[actor_index].actorid, g_actors[actor_index].level, city_mainlevel( g_actors[actor_index].city_index ), productid, goodsid, awardgroup, client_getip( actor_index ), 0, (int)time( NULL ) );

	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	return 0;
}

// 添加离线订单
int paystore_payoffline( int actorid, int goodsid, char *pOrderID, int money, int token )
{
	char szSQL[2048];
	if ( goodsid <= 0 || goodsid >= g_paygoods_maxnum )
		return -1;
	if ( pOrderID == NULL )
		return -1;

	sprintf( szSQL, "insert into pay_offline( actorid, orderid, goodsid, money, token, optime ) values( '%d','%s','%d','%d','%d','%d' )", actorid, pOrderID, goodsid, money, token, ( int )time( NULL ) );

	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	return 0;
}

int paystore_payoffline_get( int actor_index )
{
	MYSQL_RES *res;
	MYSQL_ROW  row;
	char szSQL[1024];
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	int paycount = 0;
	sprintf( szSQL, "select count(*) from pay_offline where actorid='%d'", g_actors[actor_index].actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( row = mysql_fetch_row( res ) )
	{
		paycount = atoi( row[0] );
		mysql_free_result( res );
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	if ( paycount > 0 )
	{
		sprintf( szSQL, "select orderid, goodsid, money, token from pay_offline where actorid='%d'", g_actors[actor_index].actorid );
		if ( mysql_query( myGame, szSQL ) )
		{
			printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
			write_gamelog( "%s", szSQL );
			if ( mysql_ping( myGame ) != 0 )
				db_reconnect_game();
			return -1;
		}
		res = mysql_store_result( myGame );
		while ( row = mysql_fetch_row( res ) )
		{
			char szOrderid[128] = { 0 };
			strncpy( szOrderid, row[0], 127 );

			int goodsid = atoi( row[1] );
			int money = atoi( row[2] );
			int token = atoi( row[3] );
			if ( goodsid <= 0 || goodsid >= g_paygoods_maxnum )
				continue;
			int goodstype = g_paygoods[goodsid].type;
			if ( goodstype == PAY_GOODSTYPE_RECHARGE_RMB || goodstype == PAY_GOODSTYPE_RECHARGE_DOLLAR )
			{
				// 充值
				actor_recharge( g_actors[actor_index].actorid, goodsid, szOrderid, money, token );
			}
			else
			{
				// 充值
				actor_pay( g_actors[actor_index].actorid, goodsid, szOrderid, "0", "0" );
			}

			// 删除掉
			sprintf( szSQL, "delete from pay_offline where orderid='%s'", szOrderid );
			if ( mysql_query( myGame, szSQL ) )
			{
				printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
				write_gamelog( "%s", szSQL );
				if ( mysql_ping( myGame ) != 0 )
					db_reconnect_game();
			}
		}
		mysql_free_result( res );
	}
	return 0;
}

// 限购添加次数
int actor_limitybuy_addnum( int actor_index, int index )
{
	//if ( actor_index < 0 || actor_index >= g_maxactornum )
		//return -1;
	//if ( index < 0 || index >= 8 )
	//	return -1;
	//if ( g_actors[actor_index].base_limitbuy[index] < 100 )
	//	g_actors[actor_index].base_limitbuy[index] += 1;
	return 0;
}
int actor_limitybuy_getnum( int actor_index, int index )
{
	//if ( actor_index < 0 || actor_index >= g_maxactornum )
	//	return -1;
	//if ( index < 0 || index >= 8 )
	//	return -1;
	//return g_actors[actor_index].base_limitbuy[index];
	return 0;
}
int actor_limitybuy_addnum_db( int actorid, int index )
{
	/*if ( index < 0 || index >= 8 )
		return -1;
	char szSQL[1024];
	MYSQL_RES *res;
	MYSQL_ROW row;
	char base_limitbuy[8] = { 0 };

	sprintf( szSQL, "select `base_limitbuy` from actor where actorid='%d'", actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	if ( (row = mysql_fetch_row( res )) )
	{
		memcpy( base_limitbuy, row[0], 8 );
	}
	mysql_free_result( res );

	if ( base_limitbuy[index] < 100 )
	{	
		char szText_base_limitbuy[17]={0};
		base_limitbuy[index] += 1;
		sprintf( szSQL, "update actor set base_limitbuy='%s' where actorid='%d'", db_escape((const char *)base_limitbuy,szText_base_limitbuy,0), actorid );
		if ( mysql_query( myGame, szSQL ) )
		{
			printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
			write_gamelog( "%s", szSQL );
			if ( mysql_ping( myGame ) != 0 )
				db_reconnect_game();
			return 0;
		}
	}	*/
	return 0;
}
int actor_limitybuy_getnum_db( int actorid, int index )
{
	/*if ( index < 0 || index >= 8 )
		return -1;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char	szSQL[1024];
	char base_limitbuy[8] = { 0 };

	sprintf( szSQL, "select `base_limitbuy` from actor where actorid='%d'", actorid );
	if( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error(myGame) );
		write_gamelog( "%s", szSQL );
		if( mysql_ping(myGame) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	
	if( ( row = mysql_fetch_row( res ) ) )
	{
		memcpy( base_limitbuy, row[0], 8 );
		mysql_free_result( res );
		return base_limitbuy[index];
	}
	mysql_free_result( res );*/
	return 0;
}

// 每日限购添加次数
int actor_everyday_limitybuy_addnum( int actor_index, int index )
{
	/*if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( index < 0 || index >= 8 )
		return -1;
	if ( g_actors[actor_index].everyday_limitbuy[index] < 100 )
		g_actors[actor_index].everyday_limitbuy[index] += 1;*/
	return 0;
}
int actor_everyday_limitybuy_getnum( int actor_index, int index )
{
	/*if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( index < 0 || index >= 8 )
		return -1;
	return g_actors[actor_index].everyday_limitbuy[index];*/
	return 0;
}
int actor_everyday_limitybuy_addnum_db( int actorid, int index )
{
	/*if ( index < 0 || index >= 8 )
		return -1;
	char szSQL[1024];
	MYSQL_RES *res;
	MYSQL_ROW row;
	char everyday_limitbuy[8] = { 0 };

	sprintf( szSQL, "select `everyday_limitbuy` from actor where actorid='%d'", actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	if ( (row = mysql_fetch_row( res )) )
	{
		memcpy( everyday_limitbuy, row[0], 8 );
	}
	mysql_free_result( res );

	if ( everyday_limitbuy[index] < 100 )
	{
		char szText_everyday_limitbuy[17] = { 0 };
		everyday_limitbuy[index] += 1;
		sprintf( szSQL, "update actor set everyday_limitbuy='%s' where actorid='%d'", db_escape( (const char *)everyday_limitbuy, szText_everyday_limitbuy, 0 ), actorid );
		if ( mysql_query( myGame, szSQL ) )
		{
			printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
			write_gamelog( "%s", szSQL );
			if ( mysql_ping( myGame ) != 0 )
				db_reconnect_game();
			return 0;
		}
	}*/
	return 0;
}
int actor_everyday_limitybuy_getnum_db( int actorid, int index )
{
	/*if ( index < 0 || index >= 8 )
		return -1;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char	szSQL[1024];
	char everyday_limitbuy[8] = { 0 };

	sprintf( szSQL, "select `everyday_limitbuy` from actor where actorid='%d'", actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	if ( (row = mysql_fetch_row( res )) )
	{
		memcpy( everyday_limitbuy, row[0], 8 );
		mysql_free_result( res );
		return everyday_limitbuy[index];
	}
	mysql_free_result( res );*/
	return 0;
}

// 获取这个商品在商店里的ID
int paystore_getid( int goodsid )
{
	if ( goodsid <= 0 || goodsid >= g_paygoods_maxnum )
		return -1;
	for ( int id = 1; id < g_paystore_maxnum; id++ )
	{
		if ( goodsid == g_paystore[id].goodsid )
		{
			return id;
		}
	}
	return -1;
}

// 支付成功
int actor_pay( int actorid, int goodsid, char *pOrderID, char *money, char *currency )
{
	char szSQL[1024];
	int token = 0;
	int gifttoken = 0;
	int limitbuy_gifttoken = 0;
	int everyday_gifttoken = 0;
	int awardgroup = 0;
	if ( !pOrderID )
		return -1;
	if ( goodsid <= 0 || goodsid >= g_paygoods_maxnum )
		return -1;
	int goodstype = g_paygoods[goodsid].type;

	// 玩家在线状态
	int actor_index = actor_getindex_withid( actorid );
	if ( actor_index < 0 || actor_index >= g_maxactornum )
	{
		// 不在线插入数据库，上线以后读取
		paystore_payoffline( actorid, goodsid, pOrderID, 0, 0 );
		return -1;
	}
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	if ( goodstype == PAY_GOODSTYPE_BASE )
	{ // 基础商店的商品
		token = g_paygoods[goodsid].token;
		int id = paystore_getid( goodsid );
		if ( id > 0 )
		{
			// 赠送
			if ( g_paystore[id].gift_token > 0 )
			{
				gifttoken = g_paystore[id].gift_token;
			}

			// 永久限购
			if ( g_paystore[id].limitbuy_count > 0 )
			{
				if ( actor_index >= 0 )
				{
					if ( actor_limitybuy_getnum( actor_index, g_paystore[id].limitbuy_saveindex ) < g_paystore[id].limitbuy_count )
					{
						actor_limitybuy_addnum( actor_index, g_paystore[id].limitbuy_saveindex );
						limitbuy_gifttoken = g_paystore[id].limitbuy_gifttoken;
					}
				}
				else
				{
					if ( actor_limitybuy_getnum_db( actor_index, g_paystore[id].limitbuy_saveindex ) < g_paystore[id].limitbuy_count )
					{
						actor_limitybuy_addnum_db( actor_index, g_paystore[id].limitbuy_saveindex );
						limitbuy_gifttoken = g_paystore[id].limitbuy_gifttoken;
					}
				}
			}

			// 每日限购
			if ( g_paystore[id].everyday_count > 0 )
			{
				if ( actor_index >= 0 )
				{
					if ( actor_everyday_limitybuy_getnum( actor_index, g_paystore[id].everyday_saveindex ) < g_paystore[id].everyday_count )
					{
						actor_everyday_limitybuy_addnum( actor_index, g_paystore[id].everyday_saveindex );
						everyday_gifttoken = g_paystore[id].everyday_gifttoken;
					}
				}
				else
				{
					if ( actor_everyday_limitybuy_getnum_db( actor_index, g_paystore[id].everyday_saveindex ) < g_paystore[id].everyday_count )
					{
						actor_everyday_limitybuy_addnum_db( actor_index, g_paystore[id].everyday_saveindex );
						everyday_gifttoken = g_paystore[id].everyday_gifttoken;
					}
				}
			}
		}
	}
	else if ( goodstype == PAY_GOODSTYPE_PUSH )
	{ // 精准推送的礼包
		token = g_paygoods[goodsid].token;
	}
	else if ( goodstype == PAY_GOODSTYPE_DEFAULT )
	{ // 轮播图礼包
		token = g_paygoods[goodsid].token;
	}
	else if ( goodstype == PAY_GOODSTYPE_ACTIVITY )
	{ // 活动的礼包
		token = g_paygoods[goodsid].token;
	}

	// 给钻石
	if ( token > 0 )
	{
		actor_change_token( actor_index, token, PATH_PAY, 0 );
	}
	if ( gifttoken > 0 )
	{
		actor_change_token( actor_index, gifttoken, PATH_PAY_GIFT, 0 );
	}

	// 带礼包的
	awardgroup = paygoods_getawardgroup( actor_index, goodsid );
	AwardGetInfo awardGet = { 0 };
	if ( goodstype == PAY_GOODSTYPE_MONTHCARD )
	{ // 月卡累计时间，因为当时给了一次，所以每次都是29
		if ( pCity )
		{
			actor_change_token( actor_index, g_paygoods[goodsid].token, PATH_PAY, 0 );
			pCity->mcard += (PAY_INT_MONTH_DAY - 1);
			// 发邮件
			char v1[64] = { 0 };
			AwardGetInfo getinfo = { 0 };
			char award_content[256] = { 0 };
			sprintf( v1, "%d", pCity->mcard );
			awardgroup_random( PAY_MONTH_AWARDGROUP, 0, &getinfo );
			awardgroup_makestr( &getinfo, award_content );
			mail_system( actor_index, pCity->actorid, 5036, 5532, v1, NULL, NULL, award_content, 0 );
			//actor_notify_alert( actor_index, 2201 );
		}
	}
	else if ( goodstype == PAY_GOODSTYPE_WEEKCARD )
	{// 周卡
		if ( pCity )
		{
			actor_change_token( actor_index, g_paygoods[goodsid].token, PATH_PAY, 0 );
			pCity->wcard += (PAY_INT_WEEK_DAY - 1);
			// 发邮件
			char v1[64] = { 0 };
			AwardGetInfo getinfo = { 0 };
			char award_content[256] = { 0 };
			sprintf( v1, "%d", pCity->wcard );
			awardgroup_random( PAY_WEEK_AWARDGROUP, 0, &getinfo );
			awardgroup_makestr( &getinfo, award_content );
			mail_system( actor_index, g_actors[actor_index].actorid, 5037, 5533, v1, NULL, NULL, award_content, 0 );
			//actor_notify_alert( actor_index, 2202 );
		}
	}
	else if ( awardgroup > 0 )
	{
		awardgroup_withid( actorid, awardgroup, PATH_PAY, &awardGet );
		actor_notify_pop( actor_index, 2203 );
	}
	else if ( token > 0 )
	{ // 通知一下
		actor_notify_pop( actor_index, 2203 );
	}

	// 更新订单状态
	sprintf( szSQL, "update pay_order set status='1' where orderid='%s'", pOrderID );
	if( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
	}	

	if ( goodstype == PAY_GOODSTYPE_BASE || goodstype == PAY_GOODSTYPE_MONTHCARD || goodstype == PAY_GOODSTYPE_WEEKCARD )
	{
		paystore_list( actor_index );
	}
	else if ( goodstype == PAY_GOODSTYPE_PUSH )
	{
		// 推送下一个礼包
		short next_goodsid = 0;
		int next_goodcd = 0;
		short next_goodstrategy = 0;
		/*sc_PushBagNextGoodsID( goodstype, goodsid, g_actors[actor_index].push_goodsid_strategy , &next_goodsid, &next_goodcd, &next_goodstrategy );

		if ( goodstype == PAY_GOODSTYPE_PUSH )
		{
			g_actors[actor_index].push_goodsid = next_goodsid;
			g_actors[actor_index].push_goodsid_strategy = (char)next_goodstrategy;
			if ( next_goodcd > 0 )
			{
				g_actors[actor_index].push_goodsid_time = (int)time( NULL ) + next_goodcd;
			}
			else
			{
				g_actors[actor_index].push_goodsid_time = 0;
			}
			actor_pushbaginfo( actor_index );
		}*/
	}
	else if ( goodstype == PAY_GOODSTYPE_DEFAULT || goodstype == PAY_GOODSTYPE_ACTIVITY )
	{
		city_paybag_sub( city_getptr( actor_index ), goodsid, goodstype );
		activity_paybag_list( actor_index, 1 );
		// 七日狂欢
		activity_04_addvalue( g_actors[actor_index].city_index, ACTIVITY_SEVENDAY_TYPE6, 1 );

	}

	int platid = client_getplatid( actor_index );
	if ( platid == 27 || platid == 28 || platid == 38 || platid == 31 || platid == 32 )
	{// 总充值（美分）
		int coinindex = paycoin_getindex_withplat( platid );
		if ( coinindex >= 0 && coinindex < PAYCOINMAX )
		{
			int tier = g_paygoods[goodsid].tier;
			if ( tier > 0 && tier < g_PayPriceCount )
			{
				g_actors[actor_index].charge_dollar += g_PayPrice[tier].price[coinindex];
				// 首日免费
				if ( (int)time( NULL ) < g_actors[actor_index].createtime + 86400 )
				{
					g_actors[actor_index].act25_point += g_PayPrice[tier].price[coinindex];
					activity_25_sendinfo( actor_index );
				}
			}
		}
	}
	else
	{ // 总充值（人民币）
		g_actors[actor_index].charge_point += g_paygoods[goodsid].point;
		// 首日免费
		if ( (int)time( NULL ) < g_actors[actor_index].createtime + 86400 )
		{
			g_actors[actor_index].act25_point += g_paygoods[goodsid].point;
			activity_25_sendinfo( actor_index );
		}
	}

	// 充值排行
	activity_33_addvalue( actor_index, g_paygoods[goodsid].point );
	//// 发送购买成功通知邮件
	//char szItemGet[512] = { 0 };
	//char szTempGet[64] = { 0 };
	//if ( token > 0 )
	//{
	//	sprintf( szTempGet, "%d,%d@", 100005, token );
	//	strcat( szItemGet, szTempGet );
	//}
	//for ( int i = 0 ; i < awardGet.count; i++ )
	//{
	//	sprintf( szTempGet, "%d,%d@", awardGet.kind[i], awardGet.num[i] );
	//	strcat( szItemGet, szTempGet );
	//}
	//char szNameID[32] = { 0 };
	//sprintf( szNameID, "%s%d", TAG_CONTENTID , g_paygoods[goodsid].nameid );
	//mail_send_message_item( actorid, MAIL_TYPE_SYSTEM, 99, 6313, 6243 , szItemGet, szNameID, NULL, NULL, 0, 0 );


	// 充过值
	city_set_sflag( pCity, CITY_SFLAG_FRISTPAY, 1 );
	int fristpay_awardget = actor_get_sflag( actor_index, ACTOR_SFLAG_FRISTPAY_AWARDGET );
	if ( fristpay_awardget == 0 )
	{
		activity_01_sendinfo( actor_index );
	}

	// 单笔付费
	if ( g_paygoods[goodsid].tier > g_actors[actor_index].pay_maxtier )
	{
		g_actors[actor_index].pay_maxtier = g_paygoods[goodsid].tier;
	}

	return 0;
}

// 任意额度充值
int actor_recharge( int actorid, int goodsid, char *pOrderID, int money, int token )
{
	char szSQL[1024];
	if ( !pOrderID )
		return -1;

	// 玩家在线状态
	int actor_index = actor_getindex_withid( actorid );
	if ( actor_index < 0 || actor_index >= g_maxactornum )
	{
		// 不在线插入数据库，上线以后读取
		paystore_payoffline( actorid, goodsid, pOrderID, money, token );
		return -1;
	}
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	// 给钻石
	if ( token > 0 )
	{
		actor_change_token( actor_index, token, PATH_PAY, 0 );
	}


	// 更新订单状态
	char szUserID[21];
	i64 userid = client_getuserid( actor_index );
	lltoa( userid, szUserID, 10 );

	int awardgroup = paygoods_getawardgroup( actor_index, goodsid );
	int paymode = world_data_getcache( WORLD_DATA_PAYMODE );
	char productid[64] = { 0 };
	paystore_get_productid( client_getplatid( actor_index ), paymode, goodsid, productid );
	sprintf( szSQL, "insert into pay_order( orderid, platid, userid, actorid, actorlevel, citylevel, productid, goodsid, awardgroup, ip, status, money, token, optime ) values( '%s','%d','%s','%d','%d','%d','%s','%d','%d','%s','%d','%d','%d','%d' )",
		pOrderID, client_getplatid( actor_index ), szUserID, g_actors[actor_index].actorid, g_actors[actor_index].level, city_mainlevel( g_actors[actor_index].city_index ), productid, goodsid, awardgroup, client_getip( actor_index ), 1, money, token, ( int )time( NULL ) );

	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}

	int platid = client_getplatid( actor_index );
	if ( platid == 27 || platid == 28 || platid == 38 || platid == 31 || platid == 32 )
	{// 总充值（美分）
		g_actors[actor_index].charge_dollar += money; // 美分
		// 首日免费
		if ( (int)time( NULL ) < g_actors[actor_index].createtime + 86400 )
		{
			g_actors[actor_index].act25_point += money;
			activity_25_sendinfo( actor_index );
		}
	}
	else
	{ // 总充值（人民币）
		g_actors[actor_index].charge_point += g_paygoods[goodsid].point;
		// 首日免费
		if ( (int)time( NULL ) < g_actors[actor_index].createtime + 86400 )
		{
			g_actors[actor_index].act25_point += money;
			activity_25_sendinfo( actor_index );
		}
	}

	// 充值排行
	activity_33_addvalue( actor_index, token/10 );

	// 充过值
	city_set_sflag( pCity, CITY_SFLAG_FRISTPAY, 1 );
	int fristpay_awardget = actor_get_sflag( actor_index, ACTOR_SFLAG_FRISTPAY_AWARDGET );
	if ( fristpay_awardget == 0 )
	{
		activity_01_sendinfo( actor_index );
	}

	return 0;
}

// 推送礼包信息
int actor_pushbaginfo( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	/*SLK_NetS_PushBagInfo info = { 0 };
	info.m_push_goodsid = g_actors[actor_index].push_goodsid;
	info.m_push_goodsid_time = g_actors[actor_index].push_goodsid_time;
	info.m_push_speedbag_goodsid = g_actors[actor_index].push_speedbag_goodsid;
	info.m_push_resbag_goodsid[0] = g_actors[actor_index].push_resbag_goodsid[0];
	info.m_push_resbag_goodsid[1] = g_actors[actor_index].push_resbag_goodsid[1];
	info.m_push_resbag_goodsid[2] = g_actors[actor_index].push_resbag_goodsid[2];
	info.m_push_resbag_goodsid[3] = g_actors[actor_index].push_resbag_goodsid[3];
	info.m_push_callbag_goodsid = g_actors[actor_index].push_callbag_goodsid;
	info.m_push_healbag_goodsid = g_actors[actor_index].push_healbag_goodsid;
	info.m_push_techbag_goodsid = g_actors[actor_index].push_techbag_goodsid;
	info.m_push_forgingbag_goodsid = g_actors[actor_index].push_forgingbag_goodsid;
	info.m_push_tfbag_goodsid = g_actors[actor_index].push_tfbag_goodsid;
	netsend_pushbaginfo_S( actor_index, SENDTYPE_ACTOR, &info );*/
	return 0;
}

// 添加推送礼包
int city_paybag_add( City *pCity, short goodsid, int cd, int limitcount, char path )
{
	if ( !pCity )
		return -1;
	if ( goodsid <= 0 || goodsid >= g_paygoods_maxnum )
		return -1;
	for ( int tmpi = 0; tmpi < CITY_BAG_MAX; tmpi++ )
	{
		if ( pCity->bag_gid[tmpi] == goodsid )
		{ // 如果有这个礼包，检查时间
			if ( path == 1 || (int)time( NULL ) >= pCity->bag_time[tmpi] )
			{
				pCity->bag_gid[tmpi] = 0;
				pCity->bag_time[tmpi] = 0;
				pCity->bag_num[tmpi] = 0;
				break;
			}
			else
			{
				return 0;
			}
		}
	}

	int has = 0;
	for ( int tmpi = 0; tmpi < CITY_BAG_MAX; tmpi++ )
	{
		if ( pCity->bag_gid[tmpi] <= 0 )
		{
			pCity->bag_gid[tmpi] = goodsid;
			if ( cd > 0 )
				pCity->bag_time[tmpi] = (int)time( NULL ) + cd;
			else
				pCity->bag_time[tmpi] = 0;
			pCity->bag_num[tmpi] = limitcount;
			has = 1;
			break;
		}
	}

	if ( has == 0 )
		return -1;

	return 0;
}

// 礼包剩余次数-1
int city_paybag_sub( City *pCity, short goodsid, short goodstype )
{
	if ( !pCity )
		return -1;
	if ( goodsid <= 0 || goodsid >= g_paygoods_maxnum )
		return -1;

	for ( int tmpi = 0; tmpi < CITY_BAG_MAX; tmpi++ )
	{
		if ( pCity->bag_gid[tmpi] == goodsid )
		{
			pCity->bag_num[tmpi] -= 1;
			if ( pCity->bag_num[tmpi] <= 0 )
			{
				if ( goodstype == PAY_GOODSTYPE_DEFAULT )
				{
					pCity->bag_gid[tmpi] = g_paygoods[goodsid].nextid;
					pCity->bag_num[tmpi] = (char)g_paygoods[goodsid].nextcount;
					if ( g_paygoods[goodsid].nextid <= 0 )
					{
						pCity->bag_gid[tmpi] = 0;
						pCity->bag_num[tmpi] = 0;
					}
				}
				else if ( goodstype == PAY_GOODSTYPE_ACTIVITY )
				{
					pCity->bag_gid[tmpi] = g_paygoods[goodsid].nextid;
					if ( g_paygoods[goodsid].nextid > 0 )
					{
						pCity->bag_num[tmpi] = (char)g_paygoods[goodsid].nextcount;
					}
					else
					{
						pCity->bag_time[tmpi] = 0;
						pCity->bag_num[tmpi] = 0;
					}
				}
			}
			break;
		}
	}
	return 0;
}

// 删除推送礼包
int city_paybag_del( City *pCity, short goodsid )
{
	if ( !pCity )
		return -1;
	if ( goodsid <= 0 || goodsid >= g_paygoods_maxnum )
		return -1;
	for ( int tmpi = 0; tmpi < CITY_BAG_MAX; tmpi++ )
	{
		if ( pCity->bag_gid[tmpi] == goodsid )
		{
			pCity->bag_gid[tmpi] = 0;
			pCity->bag_time[tmpi] = 0;
			pCity->bag_num[tmpi] = 0;
			break;
		}
	}
	return 0;
}

// 根据类型删除推送礼包
int city_paybag_del_withtype( City *pCity, int type )
{
	if ( !pCity )
		return -1;
	for ( int tmpi = 0; tmpi < CITY_BAG_MAX; tmpi++ )
	{
		int goodsid = pCity->bag_gid[tmpi];
		if ( goodsid <= 0 || goodsid >= g_paygoods_maxnum )
			continue;
		if ( g_paygoods[goodsid].type == type )
		{
			pCity->bag_gid[tmpi] = 0;
			pCity->bag_time[tmpi] = 0;
			pCity->bag_num[tmpi] = 0;
		}
	}
	return 0;
}

// 检查是否有这个推送礼包
char city_paybag_has( City *pCity, short goodsid )
{
	if ( !pCity )
		return 0;
	if ( goodsid <= 0 || goodsid >= g_paygoods_maxnum )
		return 0;
	for ( int tmpi = 0; tmpi < CITY_BAG_MAX; tmpi++ )
	{
		if ( pCity->bag_gid[tmpi] == goodsid )
		{
			return 1;
		}
	}
	return 0;
}

// 活动礼包开启
int activity_paybag_open( int activityid )
{
	if ( activityid <= 0 || activityid >= g_activity_count )
		return -1;

	//short goodsid = g_activity_item[activityid].m_value[0];
	//int cd = activity_lefttime( activityid );
	//int limitcount = g_activity_item[activityid].m_value[1];

	//if ( goodsid <= 0 )
	//	return -1;

	//for ( int tmpi = 0; tmpi < g_city_maxcount; tmpi++ )
	//{
	//	if ( g_city[tmpi].actorid < MINACTORID )
	//		continue;
	//	city_paybag_add( &g_city[tmpi], goodsid, cd, limitcount, 1 );
	//}
	return 0;
}

// 活动礼包关闭
int activity_paybag_close( int activityid )
{
	if ( activityid <= 0 || activityid >= g_activity_count )
		return -1;

	//short goodsid = g_activity_item[activityid].m_value[0];
	//for ( int tmpi = 0; tmpi < g_city_maxcount; tmpi++ )
	//{
	//	if ( g_city[tmpi].actorid < MINACTORID )
	//		continue;
	//	city_paybag_del( &g_city[tmpi], goodsid );
	//}
	return 0;
}

// 创建后读取部分
int activity_paybag_citynew( City* pCity )
{
	if ( !pCity )
		return -1;
	if ( pCity->actorid < MINACTORID )
		return -1;
	//for ( int activityid = ACTIVITY_PAYBAG1; activityid <= ACTIVITY_PAYBAG5; activityid++ )
	//{
	//	if ( activity_intime( activityid ) )
	//	{
	//		short goodsid = g_activity_item[activityid].m_value[0];
	//		int cd = activity_lefttime( activityid );
	//		int limitcount = g_activity_item[activityid].m_value[1];
	//		city_paybag_add( pCity, goodsid, cd, limitcount, 0 );
	//	}
	//}

	int nowtime = (int)time( NULL );
	for ( int tmpi = 0; tmpi <= CITY_BAG_MAX; tmpi++ )
	{
		if ( g_paybag[tmpi].goodsid > 0 && nowtime >= g_paybag[tmpi].begintime && nowtime < g_paybag[tmpi].endtime )
		{
			city_paybag_add( pCity, g_paybag[tmpi].goodsid, g_paybag[tmpi].endtime - (int)time( NULL ), g_paybag[tmpi].count, 0 );
		}
	}
	return 0;
}

// 活动礼包列表
int activity_paybag_list( int actor_index, int path )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int coinindex = paycoin_getindex_withplat( client_getplatid( actor_index ) );
	if ( coinindex < 0 || coinindex > PAYCOINMAX )
		coinindex = 0;
	SLK_NetS_PayStoreActivity store = { 0 };
	store.m_path = (char)path;
	for ( int tmpi = 0; tmpi < CITY_BAG_MAX; tmpi++ )
	{
		short goodsid = pCity->bag_gid[tmpi];
		if ( goodsid <= 0 || goodsid >= g_paygoods_maxnum )
			continue;

		int actionbag_endtime = pCity->bag_time[tmpi];
		if ( actionbag_endtime > 0 && (int)time( NULL ) >= actionbag_endtime )
		{
			city_paybag_del( pCity, goodsid );
			continue;
		}

		char actionbag_count = pCity->bag_num[tmpi];
		if ( actionbag_count <= 0 )
		{
			city_paybag_del( pCity, goodsid );
			continue;
		}

		int tier = g_paygoods[goodsid].tier;
		if ( tier <= 0 || tier >= g_PayPriceCount )
			continue;
		int worthtier = g_paygoods[goodsid].worth;
		if ( worthtier < 0 || worthtier >= g_PayPriceCount )
			continue; // 此处小于0

		store.m_list[store.m_count].m_goodsid = goodsid;
		store.m_list[store.m_count].m_price = g_PayPrice[tier].price[coinindex];
		store.m_list[store.m_count].m_nameid = g_paygoods[goodsid].nameid;
		store.m_list[store.m_count].m_descid = g_paygoods[goodsid].descid;
		store.m_list[store.m_count].m_icon = g_paygoods[goodsid].icon;
		store.m_list[store.m_count].m_sale = g_paygoods[goodsid].sale;
		store.m_list[store.m_count].m_worth = g_PayPrice[worthtier].price[coinindex];
		store.m_list[store.m_count].m_bag_time = actionbag_endtime;
		store.m_list[store.m_count].m_bag_num = actionbag_count;

		if ( path == 1 )
		{
			int awardgroup = paygoods_getawardgroup( actor_index, goodsid );
			if ( awardgroup > 0 && awardgroup < g_awardgroup_count )
			{
				for ( int i = 0; i < g_awardgroup[awardgroup].allcount; i++ )
				{
					if ( g_awardgroup[awardgroup].show[i] == 0 )
						continue;
					int awardcount = store.m_list[store.m_count].m_awardcount;
					if ( awardcount >= 16 )
						break;
					store.m_list[store.m_count].m_award[awardcount].m_kind = g_awardgroup[awardgroup].kind[i];
					store.m_list[store.m_count].m_award[awardcount].m_num = g_awardgroup[awardgroup].minnum[i];
					store.m_list[store.m_count].m_awardcount += 1;
				}
			}
		}
		store.m_count += 1;
	}
	netsend_paystoreactivity_S( actor_index, SENDTYPE_ACTOR, &store );
	return 0;
}

// 获取这个商品应得的礼包
int paygoods_getawardgroup( int actor_index, short goodsid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( goodsid <= 0 || goodsid >= g_paygoods_maxnum )
		return -1;
	return g_paygoods[goodsid].awardgroup;
}

// 月卡或周卡每天的发放
int paycard_give()
{
	char v1[64] = { 0 };
	AwardGetInfo m_getinfo = { 0 };
	AwardGetInfo w_getinfo = { 0 };
	char m_award[256] = { 0 };
	char w_award[256] = { 0 };

	awardgroup_random( PAY_MONTH_AWARDGROUP, 0, &m_getinfo );
	awardgroup_random( PAY_WEEK_AWARDGROUP, 0, &w_getinfo );

	awardgroup_makestr( &m_getinfo, m_award );
	awardgroup_makestr( &w_getinfo, w_award );

	for ( int city_index = 0; city_index < g_city_maxindex/*注意：使用索引位置，为了效率*/; city_index++ )
	{
		if ( g_city[city_index].actorid <= 0 )
			continue;
		if ( g_city[city_index].mcard > 0 )
		{
			sprintf( v1, "%d", g_city[city_index].mcard );
			mail_system( g_city[city_index].actor_index, g_city[city_index].actorid, 5036, 5532, v1, NULL, NULL, m_award, 1 );
			g_city[city_index].mcard -= 1;
		}
		if ( g_city[city_index].wcard > 0 )
		{
			sprintf( v1, "%d", g_city[city_index].wcard );
			mail_system( g_city[city_index].actor_index, g_city[city_index].actorid, 5037, 5533, v1, NULL, NULL, w_award, 1 );
			g_city[city_index].wcard -= 1;
		}
	}
	return 0;
}
