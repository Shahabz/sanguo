#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "db.h"
#include "define.h"
#include "city.h"
#include "building.h"
#include "actor.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "actor_friend.h"
#include "award.h"
#include "rank.h"
#include "map_zone.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

extern City *g_city;
extern int g_city_maxcount;

ActorFriend *actor_friend_getptr( int actor_index, int offset )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return NULL;
	if ( offset < 0 || offset >= ACTOR_FRIEND_MAXCOUNT )
		return NULL;
	return &g_actors[actor_index].friends[offset];
}

// 我好友列表是否有他
inline int _friend_my_has_target( int my_index, int targetid )
{
	ACTOR_CHECK_INDEX( my_index );
	for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
	{
		if ( g_actors[my_index].friends[tmpi].friend_actorid == targetid )
		{
			return 1;
		}
	}
	return 0;
}

// 他的好友列表是否有我
inline int _friend_target_has_my( int target_index, int myid )
{
	ACTOR_CHECK_INDEX( target_index );
	for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
	{
		if ( g_actors[target_index].friends[tmpi].friend_actorid == myid )
		{
			return 1;
		}
	}
	return 0;
}

// 添加离线事件
int actor_friend_addevent( int event, int actorid, int target_actorid )
{
	char szSQL[256] = { 0 };
	sprintf( szSQL, "REPLACE INTO actor_friend_event ( actorid, event, target_actorid, optime ) VALUES ('%d','%d','%d','%d')", actorid, event, target_actorid, (int)time( NULL ) );
	dbwork_addsql( szSQL, DBWORK_CMD_NORMAL, 0 );
	return 0;
}

// 读取好友离线事件
int actor_friend_loadevent( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[256] = { 0 };
	char has = 0;
	sprintf( szSQL, "select `event`,`target_actorid` from actor_friend_event where actorid='%d'", g_actors[actor_index].actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( (row = mysql_fetch_row( res )) )
	{
		int event = atoi( row[0] );
		int target_actorid = atoi( row[1] );
		actor_friend_enevt( actor_index, event, target_actorid );
		has = 1;
	}
	mysql_free_result( res );

	if ( has == 1 )
	{
		// 删除掉
		sprintf( szSQL, "delete from actor_friend_event where actorid='%d'", g_actors[actor_index].actorid );
		if ( mysql_query( myGame, szSQL ) )
		{
			printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
			write_gamelog( "%s", szSQL );
			if ( mysql_ping( myGame ) != 0 )
				db_reconnect_game();
		}
	}
	return 0;
}

// 离线事件处理
int actor_friend_enevt( int actor_index, int event, int target_actorid )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( event == ACTOR_FRIEND_EVENT_ASK )
	{
		for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
		{
			if ( g_actors[actor_index].friends[tmpi].friend_actorid <= 0 )
			{
				g_actors[actor_index].friends[tmpi].actorid = g_actors[actor_index].actorid;
				g_actors[actor_index].friends[tmpi].friend_actorid = target_actorid;
				g_actors[actor_index].friends[tmpi].offset = tmpi;
				g_actors[actor_index].friends[tmpi].city_index = -1;
				g_actors[actor_index].friends[tmpi].jointime = 0;
				break;
			}
		}
	}
	else if ( event == ACTOR_FRIEND_EVENT_AGREE )
	{
		for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
		{
			if ( g_actors[actor_index].friends[tmpi].friend_actorid <= 0 )
			{
				g_actors[actor_index].friends[tmpi].actorid = g_actors[actor_index].actorid;
				g_actors[actor_index].friends[tmpi].friend_actorid = target_actorid;
				g_actors[actor_index].friends[tmpi].offset = tmpi;
				g_actors[actor_index].friends[tmpi].city_index = -1;
				g_actors[actor_index].friends[tmpi].jointime = (int)time( NULL );
				break;
			}
		}
	}
	else if ( event == ACTOR_FRIEND_EVENT_DEL )
	{
		for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
		{
			if ( g_actors[actor_index].friends[tmpi].friend_actorid == target_actorid )
			{
				g_actors[actor_index].friends[tmpi].actorid = 0;
				g_actors[actor_index].friends[tmpi].friend_actorid = 0;
				g_actors[actor_index].friends[tmpi].offset = -1;
				g_actors[actor_index].friends[tmpi].city_index = -1;
				g_actors[actor_index].friends[tmpi].jointime = 0;
				actor_friend_delete_db( g_actors[actor_index].actorid, target_actorid );
				break;
			}
		}
	}
	return 0;
}

// 申请加好友
int actor_friend_ask( int actor_index, int target_actorid, const char *target_name )
{
	ACTOR_CHECK_INDEX( actor_index );
	int target_city_index = -1;
	if ( target_actorid > 0 )
	{
		target_city_index = city_getindex_withactorid( target_actorid );
	}
	else
	{
		target_city_index = city_getindex_withactorname( target_name );
	}
	if ( target_city_index < 0 )
	{
		actor_notify_alert( actor_index, 2055 ); // 玩家不存在
		return -1;
	}

	if ( g_actors[actor_index].actorid == target_actorid )
	{
		return -1;
	}

	if ( city_getnation( g_actors[actor_index].city_index ) != g_city[target_city_index].nation )
	{
		actor_notify_alert( actor_index, 2056 ); // 不能加其他国家的玩家为好友
		return -1;
	}
	
	// 我好友列表是否有他
	if ( _friend_my_has_target( actor_index, target_actorid ) == 1 )
	{
		actor_notify_alert( actor_index, 2051 ); // 你们已经是好友了
		return -1;
	}

	// 对方在线，直接放入申请列表
	int target_actor_index = g_city[target_city_index].actor_index;
	if ( target_actor_index >= 0 && target_actor_index < g_maxactornum )
	{ 
		if ( _friend_target_has_my( target_actor_index, g_actors[actor_index].actorid ) == 1 )
		{
			actor_notify_alert( actor_index, 2052 ); // 已经申请好友，请耐心等待对方回应
			return -1;
		}

		int asksuccess = 0;
		for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
		{
			if ( g_actors[target_actor_index].friends[tmpi].friend_actorid <= 0 )
			{
				g_actors[target_actor_index].friends[tmpi].actorid = g_actors[target_actor_index].actorid;
				g_actors[target_actor_index].friends[tmpi].friend_actorid = g_actors[actor_index].actorid;
				g_actors[target_actor_index].friends[tmpi].offset = tmpi;
				g_actors[target_actor_index].friends[tmpi].city_index = g_actors[actor_index].city_index;
				g_actors[target_actor_index].friends[tmpi].jointime = 0;
				asksuccess = 1;
				break;
			}
		}
		if ( asksuccess == 0 )
		{
			actor_notify_alert( actor_index, 2053 ); // 对方好友列表已满
			return -1;
		}
	}
	else
	{ 
		// 不在线，插入数据库
		actor_friend_addevent( ACTOR_FRIEND_EVENT_ASK, target_actorid, g_actors[actor_index].actorid );
	}

	actor_notify_alert( actor_index, 2054 ); // 已经发送添加好友请求
	return 0;
}

// 同意好友
int actor_friend_agree( int actor_index, int target_actorid, int target_city_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( target_city_index < 0 || target_city_index >= g_city_maxcount )
	{
		target_city_index = city_getindex_withactorid( target_actorid );
	}
	else if ( g_city[target_city_index].actorid != target_actorid )
	{
		target_city_index = city_getindex_withactorid( target_actorid );
	}
	if ( target_city_index < 0 || target_city_index >= g_city_maxcount )
	{
		return -1;
	}

	for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
	{
		if ( g_actors[actor_index].friends[tmpi].friend_actorid == target_actorid )
		{
			g_actors[actor_index].friends[tmpi].jointime = (int)time(NULL);
			break;
		}
	}

	// 对方是否在线
	int target_actor_index = g_city[target_city_index].actor_index;
	if ( target_actor_index >= 0 && target_actor_index < g_maxactornum )
	{
		if ( _friend_target_has_my( target_actor_index, g_actors[actor_index].actorid ) == 1 )
		{
			return -1;
		}

		for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
		{
			if ( g_actors[target_actor_index].friends[tmpi].friend_actorid <= 0 )
			{
				g_actors[target_actor_index].friends[tmpi].actorid = g_actors[target_actor_index].actorid;
				g_actors[target_actor_index].friends[tmpi].friend_actorid = g_actors[actor_index].actorid;
				g_actors[target_actor_index].friends[tmpi].offset = tmpi;
				g_actors[target_actor_index].friends[tmpi].city_index = g_actors[actor_index].city_index;
				g_actors[target_actor_index].friends[tmpi].jointime = (int)time( NULL );
				break;
			}
		}
	}
	else
	{
		// 不在线，插入数据库
		actor_friend_addevent( ACTOR_FRIEND_EVENT_AGREE, target_actorid, g_actors[actor_index].actorid );
	}
	return 0;
}

// 拒绝添加好友
int actor_friend_reject( int actor_index, int target_actorid, int target_city_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( target_city_index < 0 || target_city_index >= g_city_maxcount )
	{
		target_city_index = city_getindex_withactorid( target_actorid );
	}
	else if ( g_city[target_city_index].actorid != target_actorid )
	{
		target_city_index = city_getindex_withactorid( target_actorid );
	}
	if ( target_city_index < 0 || target_city_index >= g_city_maxcount )
	{
		return -1;
	}

	for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
	{
		if ( g_actors[actor_index].friends[tmpi].friend_actorid == target_actorid )
		{
			g_actors[actor_index].friends[tmpi].actorid = 0;
			g_actors[actor_index].friends[tmpi].friend_actorid = 0;
			g_actors[actor_index].friends[tmpi].offset = -1;
			g_actors[actor_index].friends[tmpi].city_index = -1;
			g_actors[actor_index].friends[tmpi].jointime = 0;
			actor_friend_delete_db( g_actors[actor_index].actorid, target_actorid );
			break;
		}
	}

	// 对方是否在线
	int target_actor_index = g_city[target_city_index].actor_index;
	if ( target_actor_index >= 0 && target_actor_index < g_maxactornum )
	{
		
	}
	else
	{
		// 不在线，插入数据库
		//actor_friend_addevent( ACTOR_FRIEND_EVENT_REJECT, target_actorid, g_actors[actor_index].actorid );
	}
	return 0;
}

// 删除好友
int actor_friend_delete( int actor_index, int target_actorid, int target_city_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( target_city_index < 0 || target_city_index >= g_city_maxcount )
	{
		target_city_index = city_getindex_withactorid( target_actorid );
	}
	else if ( g_city[target_city_index].actorid != target_actorid )
	{
		target_city_index = city_getindex_withactorid( target_actorid );
	}
	if ( target_city_index < 0 || target_city_index >= g_city_maxcount )
	{
		return -1;
	}

	for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
	{
		if ( g_actors[actor_index].friends[tmpi].friend_actorid == target_actorid )
		{
			g_actors[actor_index].friends[tmpi].actorid = 0;
			g_actors[actor_index].friends[tmpi].friend_actorid = 0;
			g_actors[actor_index].friends[tmpi].offset = -1;
			g_actors[actor_index].friends[tmpi].city_index = -1;
			g_actors[actor_index].friends[tmpi].jointime = 0;
			actor_friend_delete_db( g_actors[actor_index].actorid, target_actorid );
			break;
		}
	}

	// 对方是否在线
	int target_actor_index = g_city[target_city_index].actor_index;
	if ( target_actor_index >= 0 && target_actor_index < g_maxactornum )
	{
		for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
		{
			if ( g_actors[target_actor_index].friends[tmpi].friend_actorid <= g_actors[actor_index].actorid )
			{
				g_actors[target_actor_index].friends[tmpi].actorid = 0;
				g_actors[target_actor_index].friends[tmpi].friend_actorid = 0;
				g_actors[target_actor_index].friends[tmpi].offset = -1;
				g_actors[target_actor_index].friends[tmpi].city_index = -1;
				g_actors[target_actor_index].friends[tmpi].jointime = 0;
				actor_friend_delete_db( target_actorid, g_actors[actor_index].actorid );
				break;
			}
		}
	}
	else
	{
		// 不在线，插入数据库
		actor_friend_addevent( ACTOR_FRIEND_EVENT_DEL, target_actorid, g_actors[actor_index].actorid );
	}
	return 0;
}

// 数据库删除
int actor_friend_delete_db( int actorid, int target_actorid )
{
	char szSQL[256] = { 0 };
	sprintf( szSQL, "delete from actor_friend where actorid='%d' and friend_actorid='%d'", actorid, target_actorid );
	//if ( mysql_query( myGame, szSQL ) )
	//{
	//	printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
	//	write_gamelog( "%s", szSQL );
	//	if ( mysql_ping( myGame ) != 0 )
	//		db_reconnect_game();
	//}
	dbwork_addsql( szSQL, DBWORK_CMD_NORMAL, 0 );
	return 0;
}

// 好友列表
int actor_friend_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	SLK_NetS_FriendList pValue = { 0 };
	for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
	{
		if ( g_actors[actor_index].friends[tmpi].actorid <= 0 )
			continue;
		int city_index = g_actors[actor_index].friends[tmpi].city_index;
		if ( city_index < 0 || city_index >= g_city_maxcount )
		{
			city_index = city_getindex_withactorid( g_actors[actor_index].friends[tmpi].friend_actorid );
			g_actors[actor_index].friends[tmpi].city_index = city_index;
		}
		if ( city_index < 0 || city_index >= g_city_maxcount )
			continue;
		pValue.m_list[pValue.m_count].m_actorid = g_city[city_index].actorid;
		pValue.m_list[pValue.m_count].m_city_index = city_index;
		pValue.m_list[pValue.m_count].m_shape = g_city[city_index].shape;
		pValue.m_list[pValue.m_count].m_place = g_city[city_index].place;
		pValue.m_list[pValue.m_count].m_level = g_city[city_index].level;
		pValue.m_list[pValue.m_count].m_battlepower = g_city[city_index].battlepower;
		strncpy( pValue.m_list[pValue.m_count].m_name, g_city[city_index].name, sizeof( char )*NAME_SIZE );
		pValue.m_list[pValue.m_count].m_namelen = strlen( pValue.m_list[pValue.m_count].m_name );
		if ( g_actors[actor_index].friends[tmpi].jointime == 0 )
			pValue.m_list[pValue.m_count].m_ask = 1;
		else
			pValue.m_list[pValue.m_count].m_ask = 0;
		pValue.m_count += 1;
		if ( pValue.m_count >= 40 )
		{
			netsend_friendlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
			pValue.m_count = 0;
		}
	}
	if ( pValue.m_count > 0 )
	{
		netsend_friendlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}
	return 0;
}
