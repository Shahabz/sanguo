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

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Actor *g_actors;
extern int g_maxactornum;

extern Global global;
extern Map g_map;
extern int g_world_data[WORLD_DATA_MAX];

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
	//sprintf( szSQL, "select actorid,name,aclass,level,lock_time,delete_stoptime from actor_list where userid='%s' and platid='%d' and offset >= 0 ", szUserID, platid );
	sprintf( szSQL, "select actorid,name,aclass,level,lock_time,delete_stoptime from actor_list where userid='%s' and offset >= 0 ", szUserID );
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
		pValue->m_listinfo[pValue->m_actor_num].m_aclass = atoi( row[index++] );
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

// 角色经验升级
int actor_exp( int actor_index, int experience, char path )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( experience == 0 )
		return 0;
	char isup = 0;
	//AwardGetInfo getinfo = { 0 };
	//if ( experience > 0 )
	//{
	//	// 计算经验
	//	g_actors[actor_index].experience += experience;
	//	wlog( 0, LOGOP_ACTOREXP, path, experience, g_actors[actor_index].level, g_actors[actor_index].experience, g_actors[actor_index].actorid, city_mainlevel( city_getptr( actor_index ) ) );
	//	// 检查升级
	//	while ( g_actors[actor_index].experience >= g_upgradeexp[g_actors[actor_index].level] )
	//	{
	//		int curlevel = g_actors[actor_index].level;
	//		// 可以升级
	//		if ( actor_upgrade( actor_index, path, &getinfo ) < 0 )
	//			break;
	//		g_actors[actor_index].experience -= g_upgradeexp[curlevel];
	//		isup = 1;
	//	}
	//}
	//else if ( experience < 0 )
	//{
	//	// 扣减经验
	//	g_actors[actor_index].experience += experience;
	//	if ( g_actors[actor_index].experience < 0 )
	//		g_actors[actor_index].experience = 0;
	//	wlog( 0, LOGOP_ACTOREXP, path, experience, g_actors[actor_index].level, g_actors[actor_index].experience, g_actors[actor_index].actorid, city_mainlevel( city_getptr( actor_index ) ) );
	//}

	//SLK_NetS_Experience Value = {};
	//Value.m_addexp = experience;
	//Value.m_curexp = g_actors[actor_index].experience;
	//Value.m_isup = isup;
	//Value.m_member = -1;
	//Value.m_path = path;
	//if ( isup == 1 )
	//{
	//	for ( int tmpi = 0; tmpi < getinfo.count; tmpi++ )
	//	{
	//		Value.m_awardlist[Value.m_awardcount].m_kind = getinfo.kind[tmpi];
	//		Value.m_awardlist[Value.m_awardcount].m_num = getinfo.num[tmpi];
	//		Value.m_awardcount += 1;
	//	}
	//	Value.m_genius_point = g_upgrade_geniuspoint[g_actors[actor_index].level] - g_upgrade_geniuspoint[g_actors[actor_index].level - 1];
	//}
	//netsend_experience_S( actor_index, SENDTYPE_ACTOR, &Value );
	return isup;
}

// 角色升级
int actor_upgrade( int actor_index, char path, AwardGetInfo *getinfo )
{
	//if ( actor_index < 0 || actor_index >= g_maxactornum )
	//	return -1;
	//if ( g_actors[actor_index].level >= 60/* 暂定 */ )
	//	return -1;
	//int lastlevel = g_actors[actor_index].level;
	//
	//g_actors[actor_index].level += 1;
	//City *pCity = city_getptr( actor_index );
	//if ( pCity )
	//	pCity->laird_level = g_actors[actor_index].level;

	//actor_getinfo( actor_index );
	//wlog( 0, LOGOP_UPGRADE, path, 0, g_actors[actor_index].level, 0, g_actors[actor_index].actorid, city_mainlevel( city_getptr( actor_index ) ) );

	//// 体力补满
	//city_change_body( city_getptr( actor_index ), global.actorupgrade_body, PATH_ACTORUPGRADEAWARD );

	//// 等级奖励
	//awardgroup_withindex( actor_index, g_upgrade_awardgroup[lastlevel], city_mainlevel( city_getptr( actor_index ) ), PATH_ACTORUPGRADEAWARD, getinfo );

	//// 等级小于11自动加天赋
	//if( g_actors[actor_index].level <= 10 )
	//{
	//	actor_genius_learn( actor_index, 2, 1 );
	//	actor_genius_learn( actor_index, 3, 1 );
	//	actor_genius_learn( actor_index, 4, 1 );
	//	actor_genius_learn( actor_index, 5, 1 );
	//	actor_genius_learn( actor_index, 6, 1 );
	//}

	//// 玩家等级触发事件
	//attack_event_trigger_actorlevel( pCity, g_actors[actor_index].level );
	//web_up( actor_index, 0, lastlevel, g_actors[actor_index].level, 0 );
	return 0;
}

// 角色钻石变化
int actor_change_token( int actor_index, int token, char path, int path_value )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	//if ( g_actors[actor_index].token + token < 0 )
	//{
	//	int value = 0;
	//	actor_notify_value( actor_index, NOTIFY_WARNING, 1, &value, NULL ); // 钻石不够
	//	return -2;
	//}
	//g_actors[actor_index].token += token;
	//if ( token > 0 && path == PATH_PAY )
	//{ // 充值量
	//	int viplevel = 0;
	//	g_actors[actor_index].total_charge += token;
	//	data_record_addvalue( city_getptr( actor_index ), DATA_RECORD_RECHARGE_TOKEN, token );
	//}

	//if ( token > 0 )
	//{ // 总拥有
	//	data_record_addvalue( city_getptr( actor_index ), DATA_RECORD_TOTAL_TOKEN, token );
	//}
	//else if ( token < 0 )
	//{ // 消耗量
	//	// 任务数值
	//	quest_addvalue( city_getptr( actor_index ), QUEST_DATAINDEX_COSTTOKEN, 0, -token );
	//	// 总统计
	//	data_record_addvalue( city_getptr( actor_index ), DATA_RECORD_COST_TOKEN, -token );
	//	// 记录消耗
	//	wlog_token( 0, LOGOP_TOKEN, path, g_actors[actor_index].token, token, g_actors[actor_index].total_charge, g_actors[actor_index].actorid, path_value, g_actors[actor_index].userid );
	//}

	// 发送变化
	//SLK_NetS_Token Value = {};
	//Value.m_token = g_actors[actor_index].token;
	//Value.m_total_charge = g_actors[actor_index].total_charge;
	//Value.m_viplevel = g_actors[actor_index].vip;
	//Value.m_path = path;
	//netsend_token_S( actor_index, SENDTYPE_ACTOR, &Value );

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
int actor_changeshape( int actor_index, int shape )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	//if ( shape < 0 || shape > SHAPE_MAXID )
	//	return -1;
	//
	//int itemkind = 251;
	//// 扣道具
	//if ( item_lost( actor_index, itemkind, 1, PATH_ACTOR_BUY ) < 0 )
	//{
	//	if( actor_change_token( actor_index, -item_getprice( itemkind ), PATH_ACTOR_BUY, TOKEN_ACTOR_BUY_CHANGESHAPE ) < 0 )
	//		return -1;
	//}


	//g_actors[actor_index].shape = (unsigned char)shape;
	//City *pCity = city_getptr( actor_index );
	//if ( pCity )
	//{
	//	pCity->laird_shape = g_actors[actor_index].shape;

	//	// 取消自定义头像
	//	if ( pCity->headid > 0 )
	//	{
	//		pCity->headid = 0;
	//		g_actors[actor_index].headreview = 0;
	//	}

	//	// 发更新
	//	int value[4] = { 0 };
	//	value[0] = 15;
	//	value[1] = shape;
	//	value[2] = pCity->headid;
	//	value[3] = g_actors[actor_index].headreview;
	//	actor_notify_value( actor_index, NOTIFY_ACTOR, 4, value, NULL );
	//}

	return 0;
}

//-----------------------------------------------------------------------------
// actor_getinfo
// 函数说明: 角色信息
//-----------------------------------------------------------------------------
int actor_getinfo( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	SLK_NetS_ActorInfo info = { 0 };
	info.m_actorid = g_actors[actor_index].actorid;
	memcpy( info.m_name, g_actors[actor_index].name, sizeof(char)*NAME_SIZE );
	info.m_aclass = g_actors[actor_index].aclass;
	info.m_shape = g_actors[actor_index].shape;
	info.m_level = g_actors[actor_index].level;
	//info.m_experience_max = g_upgradeexp[g_actors[actor_index].level];
	info.m_token = g_actors[actor_index].token;
	/*City *pCity = city_getptr( actor_index );
	if ( pCity )
	{
	g_actors[actor_index].vip = pCity->viplevel;
	info.m_viplevel = pCity->viplevel;
	info.m_vipexp = pCity->vipexp;
	info.m_vipduration = pCity->vipduration;
	info.m_vipexptoday = actor_get_today_int_times( actor_index, TODAY_INT_VIPEXP );
	}*/
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
