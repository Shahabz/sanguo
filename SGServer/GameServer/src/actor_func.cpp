#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <limits.h>
#include <math.h>
#include "db.h"
#include "define.h"
#include "actor.h"
#include "actor_send.h"
#include "server_netsend_auto.h"
#include "actor_notify.h"
#include "system.h"
#include "award.h"
#include "item.h"
#include "global.h"
#include "mapunit.h"
#include "map.h"
#include "script.h"
#include "city.h"
#include "building.h"
#include "vip.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Actor *g_actors;
extern int g_maxactornum;

extern Global global;
extern Map g_map;
extern int g_world_data[WORLD_DATA_MAX];

extern UpgradeInfo *g_upgradeinfo;
extern int g_upgradeinfo_maxnum;


//-----------------------------------------------------------------------------
// actor_getoffline_userid
// 函数说明: 根据索引获取用户id
//-----------------------------------------------------------------------------
i64 actor_getoffline_userid( int actorid )
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];
	i64 userid = -1;

	sprintf( szSQL, "select userid from actor_list where actorid='%d'", actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	if ( (row = mysql_fetch_row( res )) )
	{
		userid = atoll( row[0] );
	}
	mysql_free_result( res );
	return userid;
}

//-----------------------------------------------------------------------------
// actor_getindex_withid
// 函数说明: 根据角色ID返回角色索引
//-----------------------------------------------------------------------------
int actor_getindex_withid( int actorid )
{
	int tmpi;

	if ( actorid <= 0 )
		return -1;
	for ( tmpi = 0; tmpi < g_maxactornum; tmpi++ )
	{
		if ( g_actors[tmpi].actorid == actorid )
			return tmpi;
	}
	return -1;
}

//-----------------------------------------------------------------------------
// actor_getlist
// 函数说明: 返回他的角色列表
//-----------------------------------------------------------------------------
int actor_getlist( int platid, i64 userid, SLK_NetS_List *pValue )
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];
	char reconnect_flag = 0;
	int thistime = (int)time( NULL );
	int locktime;
	char szUserID[21];
	char num = 0;
	int index;
	pValue->m_actor_num = 0;
	lltoa( userid, szUserID, 10 );
RE_QUERY:
	//sprintf( szSQL, "select actorid,name,nation,level,lock_time,delete_stoptime from actor_list where userid='%s' and platid='%d' and offset >= 0 ", szUserID, platid );
	sprintf( szSQL, "select actorid,name,nation,level,lock_time,delete_stoptime from actor_list where userid='%s' and offset >= 0 ", szUserID );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( reconnect_flag )
			return -1;
		if ( mysql_ping( myGame ) != 0 )
		{
			db_reconnect_game();
			reconnect_flag = 1;
			goto RE_QUERY;
		}
		return -1;
	}
	res = mysql_store_result( myGame );

	while ( (row = mysql_fetch_row( res )) )
	{
		index = 0;
		pValue->m_listinfo[pValue->m_actor_num].m_actorid = atoi( row[index++] );
		memcpy( pValue->m_listinfo[pValue->m_actor_num].m_name, row[index++], NAME_SIZE*sizeof(char) );
		pValue->m_listinfo[pValue->m_actor_num].m_nation = atoi( row[index++] );
		pValue->m_listinfo[pValue->m_actor_num].m_level = atoi( row[index++] );
		locktime = atoi( row[index++] );
		pValue->m_listinfo[pValue->m_actor_num].m_delete_stoptime = atoi( row[index++] );
		if ( pValue->m_listinfo[pValue->m_actor_num].m_delete_stoptime > 0 && thistime >= pValue->m_listinfo[pValue->m_actor_num].m_delete_stoptime )
		{
			// 如果时间到了,则删除这个角色,并不发送
			if ( actor_delete( userid, pValue->m_listinfo[pValue->m_actor_num].m_actorid ) >= 0 ) // 删除这个角色
			{
				continue;
			}
		}
		if ( locktime == -1 || locktime > thistime )
			pValue->m_listinfo[pValue->m_actor_num].m_lockstat = 1;
		else
			pValue->m_listinfo[pValue->m_actor_num].m_lockstat = 0;
		pValue->m_listinfo[pValue->m_actor_num].m_lock_endtime = locktime;

		pValue->m_actor_num++;
		if ( pValue->m_actor_num >= 8 )
		{
			break;
		}
	}
	mysql_free_result( res );
	return 0;
}

//-----------------------------------------------------------------------------
// actor_verifyid
// 函数说明: 验证一下这个角色id是不是属于登陆的这个用户
//-----------------------------------------------------------------------------
int actor_verifyid( i64 userid, int actorid )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[1024];
	int verify = 0;
	char szUserID[21] = {0};
	lltoa( userid, szUserID, 10 );
	sprintf( szSQL, "select actorid from actor_list where actorid='%d' and userid='%s'", actorid, szUserID );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( (row = mysql_fetch_row( res )) )
	{
		verify = 0;
	}
	else
	{
		verify = -1;
	}
	mysql_free_result( res );
	return verify;
}

// 角色钻石变化
int actor_change_token( int actor_index, int token, char path, int path_value )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( g_actors[actor_index].token + token < 0 )
	{
		int value = 0;
		//actor_notify_value( actor_index, NOTIFY_WARNING, 1, &value, NULL ); // 钻石不够
		return -2;
	}
	g_actors[actor_index].token += token;
	if ( token > 0 && path == PATH_PAY )
	{ // 充值量
		int viplevel = 0;
		g_actors[actor_index].total_charge += token;
	}

	if ( token > 0 )
	{ // 总拥有
	}
	else if ( token < 0 )
	{ // 消耗量
		// 记录消耗
		wlog_token( 0, LOGOP_TOKEN, path, g_actors[actor_index].token, token, g_actors[actor_index].total_charge, g_actors[actor_index].actorid, path_value, g_actors[actor_index].userid );
	}

	// 发送变化
	SLK_NetS_Token Value = {};
	Value.m_total = g_actors[actor_index].token;
	Value.m_add = token;
	Value.m_path = path;
	netsend_changtoken_S( actor_index, SENDTYPE_ACTOR, &Value );

	// 记录log
	wlog( 0, LOGOP_TOKEN, path, token, g_actors[actor_index].token, g_actors[actor_index].total_charge, g_actors[actor_index].actorid, path_value );
	return 0;
}

// 修改名称
int actor_changename( int actor_index, char *pname )
{
	//if ( actor_index < 0 || actor_index >= g_maxactornum )
	//	return -1;
	//if ( !pname )
	//	return -1;
	//MYSQL_RES * res;
	//MYSQL_ROW	row;
	//char szSQL[1024];

	//int namelen = (int)strlen( pname );
	//if ( namelen <= 0 || namelen >= NAME_SIZE )
	//	return -1;
	//int costtype = 0;
	//int itemkind = 216;
	//if ( item_getitemnum( actor_index, itemkind ) <= 0 )
	//{
	//	if ( g_actors[actor_index].token < item_getprice( itemkind ) )
	//	{			
	//		int value = 0;
	//		actor_notify_value( actor_index, NOTIFY_WARNING, 1, &value, NULL ); // 钻石不够
	//		return -1;
	//	}
	//	costtype = 1;
	//}

	//char szText_newname[NAME_SIZE * 2 + 1] = {0};
	//db_escape( (const char *)pname, szText_newname, 0 );
	//sprintf( szSQL, "SELECT count(*) FROM actor_list WHERE name='%s'", szText_newname );
	//if ( mysql_query( myGame, szSQL ) )
	//{
	//	printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
	//	write_gamelog( "%s", szSQL );
	//	return -1;
	//}
	//res = mysql_store_result( myGame );
	//if ( row = mysql_fetch_row( res ) )
	//{
	//	if ( atoi( row[0] ) >= 1 )
	//	{
	//		actor_system_message( actor_index, 63 ); // 该名字已经有人使用
	//		mysql_free_result( res );
	//		return -1;
	//	}
	//}
	//mysql_free_result( res );

	//strncpy( g_actors[actor_index].name, pname, NAME_SIZE );
	//g_actors[actor_index].name[NAME_SIZE - 1] = 0;

	//City *pCity = city_getptr( actor_index );
	//if ( !pCity )
	//	return -1;

	//strncpy( pCity->laird_name, pname, NAME_SIZE );
	//pCity->laird_name[NAME_SIZE - 1] = 0;

	//sprintf( szSQL, "UPDATE actor_list SET name='%s' WHERE actorid=%d", szText_newname, g_actors[actor_index].actorid );
	//if ( mysql_query( myGame, szSQL ) )
	//{
	//	printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
	//	write_gamelog( "%s", szSQL );
	//	return -1;
	//}
	//sprintf( szSQL, "UPDATE club_asklist SET name='%s' WHERE cityid=%d", szText_newname, pCity->cityid );
	//if ( mysql_query( myGame, szSQL ) )
	//{
	//	printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
	//	write_gamelog( "%s", szSQL );
	//	return -1;
	//}

	//Club *pClub = club_getptr( actor_index );
	//if ( pClub )
	//{
	//	if ( pClub->m_leaderid == g_actors[actor_index].actorid )
	//	{
	//		strncpy( pClub->m_leadername, pname, NAME_SIZE );
	//		pClub->m_leadername[NAME_SIZE - 1] = 0;
	//	}

	//}
	//actor_system_message( actor_index, 64 ); // 更名成功！
	//if ( costtype == 0 )
	//{
	//	item_lost( actor_index, itemkind, 1, PATH_SYSTEM );
	//}
	//else
	//{
	//	actor_change_token( actor_index, -item_getprice( itemkind ), PATH_ACTOR_BUY, TOKEN_ACTOR_BUY_CHANGENAME );
	//}
	//actor_getinfo( actor_index );
	//// 通知到城外
	//mapunit_update( MAPUNIT_TYPE_CITY, -1, pCity->unit_index );
	return 0;
}

// 修改名称
int actor_changename_gm( int actorid, char *pname )
{
	//if ( !pname )
	//	return -1;
	//MYSQL_RES * res;
	//MYSQL_ROW	row;
	//char szSQL[1024];

	//int namelen = (int)strlen( pname );
	//if ( namelen <= 0 || namelen >= NAME_SIZE )
	//	return -1;
	//char szText_newname[NAME_SIZE * 2 + 1] = {0};
	//db_escape( (const char *)pname, szText_newname, 0 );
	//sprintf( szSQL, "SELECT count(*) FROM actor_list WHERE name='%s'", szText_newname );
	//if ( mysql_query( myGame, szSQL ) )
	//{
	//	printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
	//	write_gamelog( "%s", szSQL );
	//	return -1;
	//}
	//res = mysql_store_result( myGame );
	//if ( row = mysql_fetch_row( res ) )
	//{
	//	if ( atoi( row[0] ) >= 1 )
	//	{
	//		// 该名字已经有人使用
	//		mysql_free_result( res );
	//		return -1;
	//	}
	//}
	//mysql_free_result( res );

	//City *pCity = city_getptr_withactorid( actorid );
	//if ( !pCity )
	//	return -1;

	//int actor_index = actor_getindex_withid( actorid );
	//if ( actor_index >= 0 && actor_index < g_maxactornum )
	//{
	//	strncpy( g_actors[actor_index].name, pname, NAME_SIZE );
	//	g_actors[actor_index].name[NAME_SIZE - 1] = 0;
	//	actor_getinfo( actor_index );
	//}

	//strncpy( pCity->laird_name, pname, NAME_SIZE );
	//pCity->laird_name[NAME_SIZE - 1] = 0;

	//sprintf( szSQL, "UPDATE actor_list SET name='%s' WHERE actorid=%d", szText_newname, actorid );
	//if ( mysql_query( myGame, szSQL ) )
	//{
	//	printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
	//	write_gamelog( "%s", szSQL );
	//	return -1;
	//}
	//sprintf( szSQL, "UPDATE club_asklist SET name='%s' WHERE cityid=%d", szText_newname, pCity->cityid );
	//if ( mysql_query( myGame, szSQL ) )
	//{
	//	printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
	//	write_gamelog( "%s", szSQL );
	//	return -1;
	//}

	//Club *pClub = club_getptr_withcity( pCity );
	//if ( pClub )
	//{
	//	if ( pClub->m_leaderid == pCity->laird_actorid )
	//	{
	//		strncpy( pClub->m_leadername, pname, NAME_SIZE );
	//		pClub->m_leadername[NAME_SIZE - 1] = 0;
	//	}

	//}

	//// 通知到城外
	//mapunit_update( MAPUNIT_TYPE_CITY, -1, pCity->unit_index );
	return 0;
}

// 修改头像
int actor_changeshape( int actor_index, char shape )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	if ( shape < 0 || shape >= SHAPE_MAX )
		return -1;

	g_actors[actor_index].shape = shape;
	City *pCity = city_getptr( actor_index );
	if ( pCity )
	{
		pCity->shape = g_actors[actor_index].shape;

		// 发更新
		int value[2] = { 0 };
		value[0] = 0;
		value[1] = shape;
		actor_notify_value( actor_index, NOTIFY_CHANGESHAPE, 2, value, NULL );
	}
	return 0;
}

//-----------------------------------------------------------------------------
// actor_getinfo
// 函数说明: 角色信息
//-----------------------------------------------------------------------------
int actor_getinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	SLK_NetS_ActorInfo info = { 0 };
	info.m_actorid = g_actors[actor_index].actorid;
	memcpy( info.m_name, g_actors[actor_index].name, sizeof(char)*NAME_SIZE );
	info.m_nation = g_actors[actor_index].nation;
	info.m_shape = g_actors[actor_index].shape;
	info.m_level = g_actors[actor_index].level;
	info.m_token = g_actors[actor_index].token;
	City *pCity = city_getptr( actor_index );
	if ( pCity )
	{
		info.m_exp = pCity->exp;
		info.m_exp_max = g_upgradeinfo[pCity->level].exp;
		info.m_viplevel =  pCity->viplevel;
		info.m_vipexp =  pCity->vipexp;
		info.m_vipexp_max = vip_expmax_get( pCity->viplevel );
		info.m_body =  pCity->body;
		info.m_place =  pCity->place;
		info.m_official =  pCity->official;
		info.m_zone =  pCity->zone;
		info.m_battlepower =  pCity->battlepower;
		info.m_silver =  pCity->silver;
		info.m_wood =  pCity->wood;
		info.m_food =  pCity->food;
		info.m_iron =  pCity->iron;
		info.m_infantry_num =  building_soldiers_total( pCity->index, BUILDING_Infantry );
		info.m_cavalry_num = building_soldiers_total( pCity->index, BUILDING_Cavalry );
		info.m_archer_num = building_soldiers_total( pCity->index, BUILDING_Archer );
		info.m_mokilllv =  pCity->mokilllv;
		info.m_sflag =  pCity->sflag;
	}
	netsend_actorinfo_S( actor_index, SENDTYPE_ACTOR, &info );
	return 0;
}

// 角色配置
int actor_configinfo( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	//SLK_NetS_ActorConfig info = { 0 };
	//for ( int tmpi = 0; tmpi < 8; tmpi++ )
	//{
	//	info.m_config[tmpi] = g_actors[actor_index].config[tmpi];
	//}
	//info.m_sysopenlist[0] = g_Config.showflag;
	//info.m_sysopenlist[1] = g_Config.showcdkey;
	//info.m_sysopenlist[2] = g_Config.showvip;
	//info.m_sysopenlist[3] = g_Config.showmcard;
	//info.m_syscount = 4;
	////info.m_remote_push = city_get_sflag( city_getptr( actor_index ), CITY_SFLAG_CLOSEPUSH ); // 0开启 1关闭
	//netsend_actorconfig_S( actor_index, SENDTYPE_ACTOR, &info );
	return 0;
}
int actor_setconfig( int actor_index, int index, char value )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( index < 0 || index >= 8 )
		return -1;
	g_actors[actor_index].config[index] = value;
	return 0;
}
int actor_getconfig( int actor_index, int index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( index < 0 || index >= 8 )
		return -1;
	return g_actors[actor_index].config[index];
}
