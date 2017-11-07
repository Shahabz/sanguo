#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "netserver.h"

#ifndef WIN32
#include <sys/time.h>
#endif

#include "server_netsend_auto.h"
#include "auto_actor.h"
#include "utils.h"
#include "actor.h"
#include "actor_send.h"
#include "actor_times.h"
#include "wqueue.h"
#include "define.h"
#include "client.h"
#include "account.h"
#include "item.h"
#include "mapunit.h"
#include "zoneunit.h"
#include "system.h"
#include "global.h"
#include "actor_notify.h"
#include "map.h"
#include "award.h"
#include "script_auto.h"
#include "activity.h"
#include "city.h"
#include "city_attr.h"
#include "building.h"
#include "hero.h"
#include "equip.h"
#include "quest.h"
#include "world_quest.h"
#include "chat.h"
#include "mail.h"
#include "army.h"
#include "army_group.h"
#include "map_zone.h"
#include "map_town.h"
#include "map_enemy.h"
#include "map_res.h"
#include "map_event.h"
#include "nation.h"
#include "world_boss.h"
#include "king_war.h"

extern Global global;
extern SConfig g_Config;
extern int g_maxactornum;
extern int g_save_wait;
extern MYSQL *myGame;
extern MYSQL *myData;

extern City *g_city;
extern int g_city_maxcount;

Actor *g_actors = NULL;
int g_actornum = 0;
int g_maxactorid = 0;
int g_nation_actornum[3] = { 0 };

extern UpgradeInfo *g_upgradeinfo;
extern int g_upgradeinfo_maxnum;

//-----------------------------------------------------------------------------
// actors_init
// 函数说明: 角色数据初始化
//-----------------------------------------------------------------------------
int actors_init()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];

	sprintf( szSQL, "select max(actorid) from actor" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myGame );

	if ( !((row = mysql_fetch_row( res ))) )
	{
		mysql_free_result( res );
		return -1;
	}
	if ( row[0] )
		g_maxactorid = atoi( row[0] ) + 1;
	else
		g_maxactorid = 1;

	mysql_free_result( res );

	// 设定不能低于最小的角色ID值
	if ( g_maxactorid < MINACTORID )
	{ // 只要第一次就可以了，之后都会读最大的
		g_maxactorid = MINACTORID;
	}

	g_actors = (Actor *)malloc( sizeof(Actor)*g_maxactornum );
	memset( g_actors, 0, sizeof(Actor)*g_maxactornum );
	printf_msg( "Actor  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_maxactornum, (sizeof(Actor)*g_maxactornum) / 1024.0 / 1024.0, sizeof(Actor) / 1024.0 );

	// 玩家在线情况清0，放置意外情况
	sprintf( szSQL, "UPDATE `actor_list` SET `online`=0" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
	}


	// 各个国家的人数
	for ( int tmpi = 0; tmpi < 3; tmpi++ )
	{
		sprintf( szSQL, "select count(*) from actor where nation=%d", tmpi+1 );
		if ( mysql_query( myGame, szSQL ) )
		{
			printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
			write_gamelog( "%s", szSQL );
			continue;
		}
		res = mysql_store_result( myGame );
		if ( !((row = mysql_fetch_row( res ))) )
		{
			mysql_free_result( res );
			return -1;
		}
		if ( row[0] )
			g_nation_actornum[tmpi] = atoi( row[0] );
		else
			g_nation_actornum[tmpi] = 0;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// actors_save_everyone
// 函数说明: 所有在线角色保存
// 参数说明：【type==0 队列保存】 【type==1 立即保存】
//-----------------------------------------------------------------------------
int actors_save_everyone( char type )
{
	int tmpi;
	for ( tmpi = 0; tmpi < g_maxactornum; tmpi++ )
	{
		if ( g_actors[tmpi].actorid > 0 )
		{
			if ( type == 0 )
				actor_send_save( tmpi );
			else
				actor_real_saveall( tmpi, NULL );
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// actors_on_core
// 函数说明: 服务器崩溃之后的存档
//-----------------------------------------------------------------------------
void actors_on_core()
{
	int tmpi;
	FILE *fp;
	char szSaveSqlFile[MAX_PATH];

	sprintf( szSaveSqlFile, "savesqlfile.sql" );
	fp = fopen( szSaveSqlFile, "wb" );
	if ( fp == NULL )
		return;

	// 所有角色保存
	for ( tmpi = 0; tmpi < g_maxactornum; tmpi++ )
	{
		if ( g_actors[tmpi].actorid > 0 )
		{
			actor_real_saveall( tmpi, fp );
		}
	}
	// 所有城池保存
	city_save( fp );
	// 所有国家保存
	nation_save( fp );	
	// 所有部队保存
	army_save( fp );
	// 所有集结保存
	armygroup_save( fp );
	// 所有地区保存
	map_zone_save( fp );
	// 所有城镇保存
	map_town_save( fp );
	// 所有流寇保存
	map_enemy_save( fp );
	// 所有资源点保存
	map_res_save( fp );
	// 所有随机事件点保存
	map_event_save( fp );
	// 所有世界boss
	world_boss_save( fp );
	// 所有血战皇城据点保存
	kingwar_town_save( fp );
	if ( fp )
	{
		char szSQL[512] = { 0 };
		sprintf( szSQL, "replace into world_data ( id, value, strvalue) values('%d',unix_timestamp(now()),'');", WORLD_DATA_SAVEFILE_FLAG );
		fprintf( fp, "%s;\n", szSQL );
	}
	fclose( fp );
	return;
}

//-----------------------------------------------------------------------------
// actor_save
// 函数说明: 角色保存数据
//-----------------------------------------------------------------------------
int actor_save( int actor_index, int savecity, FILE *fp )
{
	// 角色基本信息保存
	actor_save_auto( &g_actors[actor_index], "actor", fp );

	// 角色列表信息保存
	actor_update_list( actor_index );

	// 未上阵英雄保存
	actor_hero_batch_save_auto( g_actors[actor_index].hero, HERO_ACTOR_MAX, "actor_hero", fp );

	// 未上阵英雄装备保存
	for ( int tmpi = 0; tmpi < HERO_ACTOR_MAX; tmpi++ )
	{
		if ( g_actors[actor_index].hero[tmpi].id <= 0 )
			continue;
		actor_equip_batch_save_auto( g_actors[actor_index].hero[tmpi].equip, EQUIP_TYPE_MAX, "actor_equip", fp );
	}

	// 上阵英雄和装备也保存
	City *pCity = city_getptr( actor_index );
	if ( pCity )
	{
		// 英雄
		actor_hero_batch_save_auto( pCity->hero, HERO_CITY_MAX, "actor_hero", fp );

		// 装备
		for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
		{
			if ( pCity->hero[tmpi].kind <= 0 )
				continue;
			actor_equip_batch_save_auto( pCity->hero[tmpi].equip, EQUIP_TYPE_MAX, "actor_equip", fp );
		}
	}

	// 保存道具
	item_save( actor_index, fp );

	// 保存装备
	equip_save( actor_index, fp );

	return 0;
}

//-----------------------------------------------------------------------------
// actor_real_saveall
// 函数说明: 角色数据真正开始保存
//-----------------------------------------------------------------------------
int actor_real_saveall( int actor_index, FILE *fp )
{
	// 保存角色属性(慢)
	actor_save( actor_index, 0, fp );
	g_actors[actor_index].savetime = 0;
	g_save_wait = 1;
	return 0;
}

//-----------------------------------------------------------------------------
// actor_send_save
// 函数说明: 发送保存信息到队列
//-----------------------------------------------------------------------------
int actor_send_save( int actor_index )
{
	if ( g_save_wait )
	{
		if ( add_wqueue( actor_index, WCMD_SAVE, NULL, 0, 0, 1 ) < 0 )
		{
			write_gamelog( "add_wqueue full" );
			return actor_real_saveall( actor_index, NULL );
		}
		return 0;
	}
	else
	{
		return actor_real_saveall( actor_index, NULL );
	}
}

//-----------------------------------------------------------------------------
// actor_send_remove
// 函数说明: 发送下线信息到队列
//-----------------------------------------------------------------------------
int actor_send_remove( int actor_index )
{
	if ( g_actors[actor_index].isexit == 2 )
		return -1;
	g_actors[actor_index].isexit = 2;
	if ( g_save_wait )
	{
		if ( add_wqueue( actor_index, WCMD_LEAVE, NULL, 0, 0, 1 ) < 0 )
		{
			write_gamelog( "add_wqueue full" );
			return actor_remove( actor_index );
		}
		return 0;
	}
	else
	{
		return actor_remove( actor_index );
	}
}

//-----------------------------------------------------------------------------
// actor_remove
// 函数说明: 角色清理并断开客户端
//-----------------------------------------------------------------------------
int actor_remove( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( g_actors[actor_index].actorid <= 0 )
		return -1;
	if ( g_actors[actor_index].isexit == 0 )
		return -1;

	// 角色更新在线状态
	actor_update_online( g_actors[actor_index].actorid, 0 );

	// 登出log
	account_out( actor_index, 0 );

	// 保存角色属性(慢)
	actor_save( actor_index, 1, NULL );

	// 城池关联索引清除
	if ( g_actors[actor_index].city_index >= 0 && g_actors[actor_index].city_index < g_city_maxcount )
		g_city[g_actors[actor_index].city_index].actor_index = -1;

	// 清理actor_index位置的数据
	memset( &(g_actors[actor_index]), 0, sizeof(Actor) );
	g_actors[actor_index].city_index = -1;
	g_actors[actor_index].view_areaindex = -1;

	// 总角色数自减
	g_actornum--;

	// 关闭这个socket
	net_closesock_direct( actor_index );
	g_save_wait = 1;
	return 0;
}

//-----------------------------------------------------------------------------
// actor_update_list
// 函数说明: 更新角色列表数据
//-----------------------------------------------------------------------------
int actor_update_list( int actor_index )
{
	char szSQL[1024];
	int actorid;
	char ActorName[64];

	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	actorid = g_actors[actor_index].actorid;

	// 插入记录到actor
	db_escape( (const char *)g_actors[actor_index].name, ActorName, 0 );
	sprintf( szSQL, "update actor_list set name='%s',level='%d',logout_time='%d' where actorid='%d'", ActorName, g_actors[actor_index].level, (int)time( NULL ), actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -3;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// actor_update_online
// 函数说明: 更新数据库中角色在线情况
//-----------------------------------------------------------------------------
int actor_update_online( int actorid, char isonline )
{
	char szSQL[256];
	sprintf( szSQL, "UPDATE actor_list SET online='%d' WHERE actorid=%d", isonline, actorid );
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

//-----------------------------------------------------------------------------
// actor_del
// 函数说明: 清除客户端
//-----------------------------------------------------------------------------
int actor_del( int client_index )
{
	if ( client_index < 0 )
		return -1;
	if ( g_actors[client_index].actorid != 0 )
	{
		if ( g_actors[client_index].isexit == 0 )
			g_actors[client_index].isexit = 1;
		return 0;
	}
	// 退出标志
	return 0;
}

//-----------------------------------------------------------------------------
// actor_delete
// 函数说明: 真正的删除角色
//-----------------------------------------------------------------------------
int actor_delete( i64 userid, int actorid )
{
	char	szSQL[1024];
	char szUserID[21];
	lltoa( userid, szUserID, 10 );

	// 如果该角色在线
	if ( actor_getindex_withid( actorid ) >= 0 )
	{
		return -1;
	}

	// 更新数据库
	sprintf( szSQL, "update actor_list set offset='-1' where userid='%s' and actorid='%d'", szUserID, actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// actor_delete_stoptime
// 函数说明: 设置删除标记时间
//-----------------------------------------------------------------------------
int actor_delete_stoptime( i64 userid, int actorid )
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];

	int timedate = 0; // 当前时间
	char szUserID[21];
	int level = 0;
	lltoa( userid, szUserID, 10 );

	sprintf( szSQL, "select level from actor_list where userid='%s' and actorid='%d'", szUserID, actorid );
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
		level = atoi( row[0] );
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	if ( actor_getindex_withid( actorid ) >= 0 )
	{
		return -1;
	}

#ifdef _DEBUG
	return actor_delete( userid, actorid );
#else
	if ( level < 20 )
		return actor_delete( userid, actorid );
	else if ( level < 30 )
		timedate = (int)time( NULL ) + (60 * 10); // 30级10分钟
	else if ( level < 50 )
		timedate = (int)time( NULL ) + (60 * 60 * 1); // 50级1小时
	else if ( level < 60 )
		timedate = (int)time( NULL ) + (60 * 60 * 2); // 60级2小时
	else if ( level < 70 )
		timedate = (int)time( NULL ) + (60 * 60 * 3); // 70级3小时
	else if ( level < 80 )
		timedate = (int)time( NULL ) + (60 * 60 * 4); // 80级6小时
	else if ( level < 150 )
		timedate = (int)time( NULL ) + (60 * 60 * 24 * 1); // 80级以上1天
#endif

	sprintf( szSQL, "update actor_list set delete_stoptime='%d' where userid='%s' and actorid='%d'", timedate, szUserID, actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}

	// 更新删除时间
	return timedate;
}

//-----------------------------------------------------------------------------
// actor_delete_cancel
// 函数说明: 放弃删除
//-----------------------------------------------------------------------------
int actor_delete_cancel( int actorid )
{
	char szSQL[1024];
	sprintf( szSQL, "UPDATE actor_list SET delete_stoptime=0 WHERE actorid=%d", actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	if ( mysql_affected_rows( myGame ) == 0 )
	{
		// 如果没有更新到任一行,则说明没删除过
		return -1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// actors_logic
// 函数说明: 所有角色逻辑 5帧一次
//-----------------------------------------------------------------------------
void actors_logic()
{
	int tmpi;
	for ( tmpi = 0; tmpi < g_maxactornum; tmpi++ )
	{
		if ( g_actors[tmpi].actorid > 0 )
		{
			if ( client_isexited( tmpi ) && g_actors[tmpi].isexit <= 0 )
				g_actors[tmpi].isexit = 1;
			actor_logic( tmpi );
		}
		// 退出标志
		else if ( client_isexited( tmpi ) )
		{
			net_closesock_direct( tmpi );
		}
	}
}

//-----------------------------------------------------------------------------
// actor_logic
// 函数说明: 角色逻辑 5帧一次
//-----------------------------------------------------------------------------
void actor_logic( int actor_index )
{
	if ( g_actors[actor_index].actorid == 0 )
		return;
	if ( g_actors[actor_index].isexit == 2 )
		return;
	if ( g_actors[actor_index].talkspeed_frame > 0 )
	{
		g_actors[actor_index].talkspeed_frame -= 1;
	}
	// 除GM外，控制加速
	//if ( g_actors[actor_index].admin < 90 && g_actors[actor_index].checkspeed_time == 0 )
	//{
	//	if ( g_actors[actor_index].checkspeed_frame > 0 )
	//		g_actors[actor_index].checkspeed_frame--;
	//	if ( g_actors[actor_index].checkspeed_frame <= 0 )
	//	{
	//		SLK_NetS_Heart Value = {};
	//		int timestamp = 3 + rand() % 4;
	//		Value.m_value[0] = timestamp * 35;	// 给5帧的误差
	//		netsend_heart_S( actor_index, SENDTYPE_ACTOR, &Value );
	//		g_actors[actor_index].checkspeed_time = (int)time( NULL ) + timestamp;
	//	}
	//}

	if ( g_actors[actor_index].isexit == 1 /*&&
		(g_actors[actor_index].stat != ACTOR_STAT_FIGHT || g_actors[actor_index].fight_index < 0)*/ )
	{
		actor_send_remove( actor_index );
		return;
	}
}

//-----------------------------------------------------------------------------
// actor_change_index
// 函数说明: 重登陆的索引转换
//-----------------------------------------------------------------------------
int actor_change_index( int old_index, int new_index )
{
	ACTOR_CHECK_INDEX( old_index );
	ACTOR_CHECK_INDEX( new_index );
	memcpy( &g_actors[new_index], &g_actors[old_index], sizeof(Actor) );
	g_actors[new_index].isexit = 0;

	// 城池索引关联
	g_city[g_actors[old_index].city_index].actor_index = new_index;

	// 清空旧角色
	memset( &(g_actors[old_index]), 0, sizeof(Actor) );
	g_actors[old_index].city_index = -1;
	g_actors[old_index].view_areaindex = -1;
	return 0;
}

//-----------------------------------------------------------------------------
// actor_entergame
// 函数说明: 玩家进入游戏
//-----------------------------------------------------------------------------
int actor_entergame( int client_index, int actorid )
{
	// 已经进入游戏的不能再次进入
	if ( client_getstat( client_index ) >= CLIENT_STAT_ENTERGAME )
		return -1;

	// 验证一下这个角色id是不是属于登陆的这个用户
	if ( actor_verifyid( client_getuserid( client_index ), actorid ) < 0 )
		return -1;

	// 这里判断玩家是否在游戏中
	int actor_index = actor_getindex_withid( actorid );
	if ( actor_index < 0 )
	{
		actor_enterworld( client_index, actorid, -1 );
	}
	else
	{
		actor_enterworld( client_index, actorid, actor_index );
	}
	client_setstat( client_index, CLIENT_STAT_ENTERGAME );
	return 0;
}

//-----------------------------------------------------------------------------
// actor_enterworld
// 函数说明: 角色进入游戏世界
//-----------------------------------------------------------------------------
int actor_enterworld( int client_index, int actorid, int actor_index )
{
	ACTOR_CHECK_INDEX( client_index );
	SLK_NetS_EnterInfo Value = {};
	// 如果玩家已经存在
	if ( actor_index >= 0 )
	{
		// 索引对应数据转换，踢旧玩家
		if ( actor_index != client_index )
		{
			account_out( actor_index, 1 );
			actor_change_index( actor_index, client_index );
			net_closesock_direct( actor_index );
		}
		// 赋予新连接
		actor_index = client_index;

	}
	else
	{
		// 获取一个空闲的角色索引
		actor_index = client_index;

		if ( actor_load( actor_index, actorid ) < 0 )
			return -1;

		// 角色总数增加
		g_actornum++;
		actor_update_online( actorid, 1 );
		write_gamelog( "[进入游戏(actor_enterworld)][%s]", g_actors[actor_index].name );
	}	

	// 登入log
	g_actors[actor_index].accountid = account_in( actor_index, 0 );
	g_actors[actor_index].cdkeywait = 0;

	// 发送进入游戏
	Value.m_actorid = g_actors[actor_index].actorid;
	Value.m_serverid = g_Config.server_code;
	Value.m_servertime = (int)time( NULL );
	Value.m_createtime = g_actors[actor_index].createtime;
	Value.m_data_int = 0;
	Value.m_data_short = 0;
	Value.m_data_char = 0;
	netsend_enterinfo_S( client_index, SENDTYPE_ACTOR, &Value );

	// 以上信息发送之后才可让玩家进入场景

	// 以下是进入场景后发送即可的信息
	return 0;
}

//-----------------------------------------------------------------------------
// actor_entercity
// 函数说明: 角色进入城池后发送的信息
//-----------------------------------------------------------------------------
int actor_entercity( int actor_index )
{
	// 角色信息
	actor_getinfo( actor_index );

	// 属性信息
	city_attr_sendinfo( actor_index );

	// 建筑信息
	building_sendlist( actor_index );

	// 角色配置信息
	actor_configinfo( actor_index );

	// 英雄列表
	hero_list( actor_index );

	// 物品列表
	item_list( actor_index );

	// 装备列表
	equip_list( actor_index );

	// 聊天缓存
	chat_cache_sendlist( actor_index );

	// 邮件
	mail_noread_check( actor_index );

	// 军情
	city_underfire_sendlist( actor_index );

	// 离线奖励-角色ID
	gift_check( actor_index );

	// 离线奖励-用户ID
	gift_uid_check( actor_index );

	// 任务相关（不发）
	quest_give( actor_index );
	worldquest_give( actor_index );

	// 
	sc_OnActorIn( actor_index );

	// 重建家园
	city_lost_rebuild( city_getptr( actor_index ) );

	// 登陆奖励
	actor_loginaward( actor_index );
	return 0;
}

//-----------------------------------------------------------------------------
// actor_load
// 函数说明: 角色进入游戏后读取数据
//-----------------------------------------------------------------------------
int actor_load( int actor_index, int actorid )
{
	ACTOR_CHECK_INDEX( actor_index );
	g_actors[actor_index].actorid = actorid;
	g_actors[actor_index].userid = client_getuserid( actor_index );

	// 读取玩家信息
	actor_load_auto( actorid, &g_actors[actor_index], "actor" );
	g_actors[actor_index].admin = client_getusertype( actor_index );
	memcpy( g_actors[actor_index].lastip, client_getip( actor_index ), 15 );

	// 找到自己的城池
	g_actors[actor_index].city_index = city_getindex_withactorid( actorid );

	// 如果没找到,并且等级为0级，是第一次进入游戏
	char newfail = 0;
	if ( g_actors[actor_index].city_index < 0 )
	{
		if ( g_actors[actor_index].level == 0 )
		{
			newfail = actor_new( actor_index );
		}
		else
		{
			// 他的城没了，什么情况
			newfail = actor_new( actor_index );
			write_gamelog( "city_index < 0 actorid=%d", actorid );
		}
	}

	// 城已经满了，或者地图没有地方了
	if ( newfail == 1 )
	{
		int value[1] = { 0 };
		value[0] = 8;
		//actor_notify_value( actor_index, NOTIFY_CITY, 1, value, NULL );
		return -1;
	}

	// 关联索引
	int city_index = g_actors[actor_index].city_index;
	g_city[city_index].actor_index = actor_index;
	g_city[city_index].lastlogin = (int)time( NULL );
	g_city[city_index].language = client_getlanguage( actor_index );
	g_city[city_index].platid = client_getplatid( actor_index );
	g_city[city_index].os = (char)client_getos( actor_index );
	g_city[city_index].type = CityLairdType_Player;
	g_city[city_index].country = client_getcountry( actor_index );

	strncpy( g_city[city_index].name, g_actors[actor_index].name, NAME_SIZE );
	g_city[city_index].name[NAME_SIZE - 1] = 0;

	strncpy( g_city[city_index].ipcountry, client_getipcountry( actor_index ), 2 );
	g_city[city_index].ipcountry[2] = 0;

	g_actors[actor_index].nation = g_city[city_index].nation;
	g_actors[actor_index].shape = g_city[city_index].shape;
	g_actors[actor_index].level = g_city[city_index].level;

	// 初始化一些不需要存档的数据/
	g_actors[actor_index].view_areaindex = -1;

	// 读取未上阵英雄
	actor_hero_load_auto( g_actors[actor_index].actorid, actor_index, actor_hero_getptr, "actor_hero" );

	// 道具背包数据
	item_load( actor_index );

	// 为上阵英雄装备和装备栏数据
	equip_load( actor_index );

	// 计未上阵英雄属性
	for ( int tmpi = 0; tmpi < HERO_ACTOR_MAX; tmpi++ )
	{
		if ( g_actors[actor_index].hero[tmpi].kind <= 0 )
			continue;
		hero_attr_calc( &g_city[city_index], &g_actors[actor_index].hero[tmpi] );
	}
	return 0;
}

//-----------------------------------------------------------------------------
// actor_load
// 函数说明: 第一次进入游戏要初始化角色信息
//-----------------------------------------------------------------------------
int actor_new( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	// 所有的非字符串类型的varbin类型在这里初始化为0
	memset( g_actors[actor_index].quest_complete, 0, sizeof( char )*ACTOR_QUEST_MAX );
	// 上线之后加的varbin字段必须有这一步初始化

	// 初始信息
	g_actors[actor_index].level = 1;
	g_actors[actor_index].token = 0;

	// 给这个玩家创建一个城池
	City city = { 0 };
	city.actorid = g_actors[actor_index].actorid;
	city.type = CityLairdType_Player;
	city.createtime = (int)time( NULL );
	city.lastlogin = (int)time( NULL );
	city.language = client_getlanguage( actor_index );
	city.platid = client_getplatid( actor_index );
	city.os = (char)client_getos( actor_index );
	city.country = client_getcountry( actor_index );

	strncpy( city.name, g_actors[actor_index].name, sizeof(char)*NAME_SIZE );
	city.name[NAME_SIZE - 1] = 0;

	strncpy( city.ipcountry, client_getipcountry( actor_index ), 2 );
	city.ipcountry[2] = 0;

	city.nation = g_actors[actor_index].nation;
	city.shape = g_actors[actor_index].shape;
	city.level = g_actors[actor_index].level;
	city.body = global.body_max;
	city.equip_washnum = global.equip_wash_max;
	city.hero_washnum = global.hero_wash_max;
	if ( map_getrandcitypos( &city.posx, &city.posy ) < 0 )
	{ // 没有可用位置了
		return -1;
	}
	g_actors[actor_index].city_index = city_new( &city );
	if ( g_actors[actor_index].city_index < 0 )
	{
		// 没有可用索引了，那么清除一个机器人
		//city_delrobot();
		g_actors[actor_index].city_index = city_new( &city );
		if ( g_actors[actor_index].city_index < 0 )
			return -1;
	}

	int city_index = g_actors[actor_index].city_index;
	// 主城
	building_create( city_index, BUILDING_Main, -1 );
	g_city[city_index].building[0].level = 2;

	g_city[city_index].unit_index = mapunit_add( MAPUNIT_TYPE_CITY, city_index );
	g_city[city_index].zoneunit_index = zoneunit_add( MAPUNIT_TYPE_CITY, city_index );
	map_addobject( MAPUNIT_TYPE_CITY, g_city[city_index].unit_index, city.posx, city.posy );
	g_city[city_index].zone = map_zone_getid( city.posx, city.posy );

	// 所选的阵营是否有奖励
	if ( g_actors[actor_index].nation == client_getnationaward( actor_index ) )
	{
		actor_change_token( actor_index, global.nation_award_token, PATH_SYSTEM, 0 );
	}

	// 刷事件
	g_city[city_index].eventsec = global.mapevent_sec;
	exec_queue_add( EXEC_QUEUE_TYPE_MAPEVENT_CREATE, city_index, g_city[city_index].zone );
	//item_getitem( actor_index, 2041, 1, -1, PATH_SYSTEM );
	//item_getitem( actor_index, 2071, 1, -1, PATH_SYSTEM );
	//item_getitem( actor_index, 745, 1, -1, PATH_SYSTEM );
	//item_getitem( actor_index, 248, 2, -1, PATH_SYSTEM );
	return 0;
}

//-----------------------------------------------------------------------------
// actor_create
// 函数说明: 创建角色
//-----------------------------------------------------------------------------
int actor_create( int client_index, char nation, char *szActorName )
{
	int result = 0;
	i64 userid;
	int platid;
	char *username;
	if ( nation <= 0 || nation > 3 )
		return -1;
	client_setwait( client_index, 0 );

	SLK_ListInfo ListInfo = { 0 };
	/* 获取创建角色所必须的数据库用信息 */
	userid = client_getuserid( client_index );
	platid = client_getplatid( client_index );
	username = client_getusername( client_index );
	strcpy( ListInfo.m_name, szActorName );
	ListInfo.m_nation = nation;
	ListInfo.m_offset = 0;
	result = actor_db_create( client_index, platid, userid, username, &ListInfo );
	if ( result > 0 )
		result = 0;
	/* 发送到客户端创建是否成功 */
	SLK_NetS_Create Value = { 0 };
	Value.m_actorid = ListInfo.m_actorid;
	Value.m_result = result;
	Value.m_createtime = (int)time(NULL);
	netsend_create_S( client_index, SENDTYPE_ACTOR, &Value );

	/* 测试模式成功直接进入游戏 */

	return 0;
}

//-----------------------------------------------------------------------------
// actor_db_create
// 函数说明: 创建角色,数据库
//-----------------------------------------------------------------------------
int actor_db_create( int client_index, int platid, i64 userid, char *username, SLK_ListInfo *pListInfo )
{
	char szSQL[1024];
	int actorid;
	char ActorName[64];
	int thistime;
	char szUserID[21];
	char szText_devdata[1024] = {0};
	thistime = (int)time( NULL );
	lltoa( userid, szUserID, 10 );

	// 获得角色ID
	actorid = g_maxactorid++;

	// 创建角色初始名字
	if ( g_Config.use_zh == 1 )
	{
		snprintf( pListInfo->m_name, NAME_SIZE, "%d", actorid );
		pListInfo->m_name[NAME_SIZE - 1] = 0;
	}
	else
	{
		snprintf( pListInfo->m_name, NAME_SIZE, "sg%d", actorid );
		pListInfo->m_name[NAME_SIZE - 1] = 0;
	}

	// 插入记录到 actor_list
	db_escape( (const char *)pListInfo->m_name, ActorName, 0 );
	db_escape( (const char *)client_getdevdata( client_index ), szText_devdata, 0 );
	sprintf( szSQL, "insert into actor_list (actorid,platid,userid,username,offset,create_time,name,nation,channelid,os,devdata) values('%d','%d','%s','%s','%d','%d','%s','%d','%d','%d','%s')",
					actorid, platid, szUserID, username, 0, thistime, ActorName, pListInfo->m_nation, client_getchannelid( client_index ), client_getos( client_index ), szText_devdata );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
		{
			db_reconnect_game();
			return -1;
		}
		return -3;// 可能是角色重名了
	}

	// 插入记录到 actor
	sprintf( szSQL, "insert into actor (actorid,name,nation,lastip,createtime) values('%d','%s','%d','%s','%d')",
					actorid, ActorName, pListInfo->m_nation, client_getip( client_index ), thistime );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -4;	// 可能是id错误同名了
	}

	pListInfo->m_actorid = actorid;
	return actorid;
}

// 角色重新开始
int actor_restart( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	actor_del( actor_index );

	// offset = -1 代表已经废弃
	char szSQL[1024];
	char szUserID[21];
	lltoa( client_getuserid( actor_index ), szUserID, 10 );
	sprintf( szSQL, "update actor_list set offset='-1' where userid='%s' and actorid='%d'", szUserID, g_actors[actor_index].actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	return 0;
}

// 角色重新开始
int actor_restart_notonline( int actorid )
{
	// offset = -1 代表已经废弃
	char szSQL[1024];
	sprintf( szSQL, "update actor_list set offset='-1' where actorid='%d'", actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	return 0;
}

// 踢人
int actor_kick( int actorid, int kicktime )
{
	/*int actor_index = actor_getindex_withid( actorid );
	int city_index = city_getindex_withactorid( actorid );
	if ( city_index < 0 || city_index >= g_city_maxcount )
	{
		return -1;
	}
	City *pCity = &g_city[city_index];
	int locktime = 0;
	if ( kicktime > 0 )
	{
		locktime = kicktime*60 + (int)time( NULL );

		pCity->buff_id[1] = 1;
		pCity->buff_duration[1] = kicktime * 60;
		city_attr_reset( pCity );
		mapunit_update( MAPUNIT_TYPE_CITY, -1, pCity->unit_index );
	}
	else if ( kicktime == 0 )
	{
		locktime = 0;

		pCity->buff_id[1] = 1;
		pCity->buff_duration[1] = 3600;
		city_attr_reset( pCity );
		mapunit_update( MAPUNIT_TYPE_CITY, -1, pCity->unit_index );
	}
	else if ( kicktime < 0 )
	{
		locktime = -1;

		pCity->buff_id[1] = 1;
		pCity->buff_duration[1] = 31536000;
		city_attr_reset( pCity );
		mapunit_update( MAPUNIT_TYPE_CITY, -1, pCity->unit_index );
	}

	char szSQL[256];
	sprintf( szSQL, "UPDATE actor_list SET lock_time='%d' WHERE actorid=%d", locktime, actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	if ( actor_index >= 0 )
	{
		actor_send_remove( actor_index );
	}*/
	return 0;
}

// 禁言
int actor_forbidtalk( int actor_index, int forbidtime )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( forbidtime > 0 )
	{
		g_actors[actor_index].forbidtime = forbidtime*60 + (int)time( NULL );
	}
	else if ( forbidtime == 0 )
	{
		g_actors[actor_index].forbidtime = 0;
	}
	else if ( forbidtime < 0 )
	{
		g_actors[actor_index].forbidtime = -1;
	}
	return 0;
}

// 订阅一个及时通知的消息
int actor_subscribecmd_add( int actor_index, short cmd )
{
	ACTOR_CHECK_INDEX( actor_index );
	for ( int tmpi = 0; tmpi < 8; tmpi++ )
	{
		if ( g_actors[actor_index].subscribe_cmd[tmpi] <= 0 )
		{
			g_actors[actor_index].subscribe_cmd[tmpi] = cmd;
			break;
		}
	}
	return 0;
}

// 删除一个及时通知的消息
int actor_subscribecmd_del( int actor_index, short cmd )
{
	ACTOR_CHECK_INDEX( actor_index );
	for ( int tmpi = 0; tmpi < 8; tmpi++ )
	{
		if ( g_actors[actor_index].subscribe_cmd[tmpi] == cmd )
		{
			g_actors[actor_index].subscribe_cmd[tmpi] = 0;
		}
	}
	return 0;
}

// 检查是否存在及时通知的消息
int actor_subscribecmd_check( int actor_index, short cmd )
{
	ACTOR_CHECK_INDEX( actor_index );
	for ( int tmpi = 0; tmpi < 8; tmpi++ )
	{
		if ( g_actors[actor_index].subscribe_cmd[tmpi] == cmd )
		{
			return 1;
		}
	}
	return 0;
}

// 功能获取
int actor_function_open( int actor_index, int offset )
{
	ACTOR_CHECK_INDEX( actor_index );
	g_actors[actor_index].function |= (1 << offset);
	return 0;
}
int actor_function_check( int actor_index, int offset )
{
	ACTOR_CHECK_INDEX( actor_index );
	if (g_actors[actor_index].function & (1 << offset) )
		return 1;
	return 0;
}