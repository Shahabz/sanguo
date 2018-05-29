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
#include "wqueue.h"
#include "define.h"
#include "client.h"
#include "global.h"
#include "city.h"
#include "chat.h"
#include "item.h"
#include "actor_notify.h"

extern Global global;
extern SConfig g_Config;
extern int g_maxactornum;
extern int g_save_wait;
extern MYSQL *myGame;
extern MYSQL *myData;

extern City *g_city;
extern int g_city_maxcount;

extern Actor *g_actors;
extern int g_maxactornum;

SLK_NetS_Chat g_ChatCacheNation[3][CHAT_CACHE_QUEUE_COUNT];

int chat_actortalk( int actor_index, char channel, char msgtype, char *msg )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( g_actors[actor_index].forbidtime - (int)time( NULL ) > 0 )
	{
		actor_notify_pop( actor_index, 525 );
		return -1;
	}
	SLK_NetS_Chat pValue = { 0 };
	pValue.m_actorid = g_actors[actor_index].actorid;
	pValue.m_shape = pCity->shape;
	pValue.m_level = pCity->level;
	pValue.m_frame = 0;
	pValue.m_nation = pCity->nation;
	pValue.m_zone = pCity->zone;
	pValue.m_place = pCity->place;
	strncpy( pValue.m_name, pCity->name, NAME_SIZE );
	pValue.m_namelen = strlen( pValue.m_name );
	strncpy( pValue.m_msg, msg, 127 );
	pValue.m_msglen = strlen( pValue.m_msg );
	pValue.m_optime = (int)time( NULL );
	pValue.m_channel = channel;
	pValue.m_msgtype = msgtype;

	if ( channel == CHAT_CHANNEL_NATION )
	{
		chat_send_nation( &pValue );
	}
	else if ( channel == CHAT_CHANNEL_WORLD )
	{
		// 优先检查道具，没有道具使用钻石
		int costtype = 0;// 使用道具模式
		int itemkind = 171;
		// 有道具使用道具，无道具使用钻石
		if ( item_getitemnum( actor_index, itemkind ) <= 0 )
		{
			if ( g_actors[actor_index].token < item_gettoken( itemkind ) )
			{
				return -1;
			}
			costtype = 1; // 使用钻石模式
		}

		if ( costtype == 0 )
		{
			item_lost( actor_index, itemkind, 1, PATH_ITEMUSE );
		}
		else
		{
			actor_change_token( actor_index, -item_gettoken( itemkind ), PATH_ITEMUSE, 0 );
		}

		chat_send_world( &pValue );
		system_rollchat( actor_index, pValue.m_msg );
	}
	
	return 0;
}

// 发送到国家
int chat_send_nation( SLK_NetS_Chat *pValue )
{
	netsend_chat_S( 0, SENDTYPE_NATION + pValue->m_nation, pValue );
	if ( pValue->m_nation >= 1 && pValue->m_nation <= 3 )
	{
		chat_cache_queue_add( g_ChatCacheNation[pValue->m_nation-1], pValue );
		chat_cache_queue_add_db( pValue );
	}
	return 0;
}

// 发送到世界
int chat_send_world( SLK_NetS_Chat *pValue )
{
	netsend_chat_S( 0, SENDTYPE_WORLD, pValue );
	chat_cache_queue_add( g_ChatCacheNation[0], pValue );
	chat_cache_queue_add( g_ChatCacheNation[1], pValue );
	chat_cache_queue_add( g_ChatCacheNation[2], pValue );
	chat_cache_queue_add_db( pValue );
	return 0;
}

// 添加聊天缓存队列
int chat_cache_queue_add( SLK_NetS_Chat *pCache, SLK_NetS_Chat *pValue )
{
	int tmpi = 0;
	for ( tmpi = 0; tmpi < CHAT_CACHE_QUEUE_COUNT; tmpi++ )
	{
		if ( pCache[tmpi].m_msglen <= 0 )
		{
			memcpy( &pCache[tmpi], pValue, sizeof( SLK_NetS_Chat ) );
			break;
		}
	}
	if ( tmpi >= CHAT_CACHE_QUEUE_COUNT )
	{
		memmove( &pCache[0], &pCache[1], sizeof( SLK_NetS_Chat )*(CHAT_CACHE_QUEUE_COUNT - 1) );
		memcpy( &pCache[CHAT_CACHE_QUEUE_COUNT - 1], pValue, sizeof( SLK_NetS_Chat ) );
	}
	return 0;
}

int chat_cache_queue_add_db( SLK_NetS_Chat *pValue )
{
	char szSQL[2048] = { 0 };
	char szText_name[NAME_SIZE * 2 + 1] = { 0 };
	char szContent[256 * 2 + 1] = { 0 };
	db_escape( pValue->m_name, szText_name, 0 );
	db_escape( pValue->m_msg, szContent, 0 );
	sprintf( szSQL, "INSERT INTO chat ( channel, actorid, shape, level, frame, nation, zone, place, name, msg, msgtype, optime ) VALUES ('%d','%d','%d','%d','%d','%d','%d','%d','%s','%s','%d','%d')",
		pValue->m_channel, pValue->m_actorid, pValue->m_shape, pValue->m_level, pValue->m_frame, pValue->m_nation, pValue->m_zone, pValue->m_place, szText_name, szContent, pValue->m_msgtype, pValue->m_optime );
	dbwork_addsql( szSQL, DBWORK_CMD_CHAT_CACHE, 0 );
	return 0;
}

int chat_cache_loaddb( SLK_NetS_Chat *pCache, char nation )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[1024];
	sprintf( szSQL, "SELECT channel, actorid, shape, level, frame, nation, zone, place, name, msg, msgtype, optime FROM (SELECT * FROM chat WHERE (channel='%d' and nation='%d') or channel='%d' ORDER BY optime DESC LIMIT %d ) a ORDER BY optime ASC;", CHAT_CHANNEL_NATION, nation, CHAT_CHANNEL_WORLD, CHAT_CACHE_QUEUE_COUNT );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( (row = mysql_fetch_row( res )) )
	{
		int index = 0;
		SLK_NetS_Chat info = { 0 };
		info.m_channel = atoi( row[index++] );
		info.m_actorid = atoi( row[index++] );
		info.m_shape = atoi( row[index++] );
		info.m_level = atoi( row[index++] );
		info.m_frame = atoi( row[index++] );
		info.m_nation = atoi( row[index++] );
		info.m_zone = atoi( row[index++] );
		info.m_place = atoi( row[index++] );
		strncpy( info.m_name, row[index++], NAME_SIZE );
		info.m_namelen = strlen( info.m_name );
		strncpy( info.m_msg, row[index++], 127 );
		info.m_msglen = strlen( info.m_msg );
		info.m_msgtype = atoi( row[index++] );
		info.m_optime = atoi( row[index++] );
		chat_cache_queue_add( pCache, &info );
	}
	mysql_free_result( res );
	return 0;
}

int chat_cache_load()
{
	chat_cache_loaddb( g_ChatCacheNation[0], 1 );
	chat_cache_loaddb( g_ChatCacheNation[1], 2 );
	chat_cache_loaddb( g_ChatCacheNation[2], 3 );
	return 0;
}

int chat_cache_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->nation < 1 || pCity->nation > 3 )
		return -1;
	SLK_NetS_ChatList pValue = { 0 };
	pValue.m_channel = CHAT_CHANNEL_NATION;
	for ( int tmpi = 0; tmpi < CHAT_CACHE_QUEUE_COUNT; tmpi++ )
	{
		if ( g_ChatCacheNation[pCity->nation-1][tmpi].m_actorid == 0 )
			continue;
		memcpy( &pValue.m_list[pValue.m_count], &g_ChatCacheNation[pCity->nation-1][tmpi], sizeof( SLK_NetS_Chat ) );
		pValue.m_count += 1;
		if ( pValue.m_count >= 10 )
		{
			netsend_chatlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
			pValue.m_count = 0;
		}
		
	}
	if ( pValue.m_count > 0 )
		netsend_chatlist_S( actor_index, SENDTYPE_ACTOR, &pValue );

	return 0;
}

int system_talk( const char *szMsg, char roll )
{
	SLK_NetS_Systalk pValue = { 0 };
	strncpy( pValue.m_msg, szMsg, 1023 );
	pValue.m_msglen = strlen( pValue.m_msg );
	pValue.m_optime = (int)time( NULL );
	pValue.m_roll = roll;
	netsend_systalk_S( 0, SENDTYPE_WORLD, &pValue );

	// 保存起来
	if ( pValue.m_msglen < 127 )
	{
		SLK_NetS_Chat info = { 0 };
		info.m_actorid = -1;
		info.m_channel = CHAT_CHANNEL_WORLD;
		strncpy( info.m_msg, pValue.m_msg, 127 );
		info.m_msglen = pValue.m_msglen;
		info.m_msgtype = CHAT_MSGTYPE_SYSTEM;
		info.m_optime = pValue.m_optime;
		chat_cache_queue_add( g_ChatCacheNation[0], &info );
		chat_cache_queue_add( g_ChatCacheNation[1], &info );
		chat_cache_queue_add( g_ChatCacheNation[2], &info );
		chat_cache_queue_add_db( &info );
	}
	return 0;
}

int system_talkto( int actor_index, const char *szMsg, char roll )
{
	SLK_NetS_Systalk pValue = { 0 };
	strncpy( pValue.m_msg, szMsg, 1023 );
	pValue.m_msglen = strlen( pValue.m_msg );
	pValue.m_optime = (int)time( NULL );
	pValue.m_roll = roll;
	netsend_systalk_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

int system_talktonation( int nation, const char *szMsg, char roll )
{
	SLK_NetS_Systalk pValue = { 0 };
	strncpy( pValue.m_msg, szMsg, 1023 );
	pValue.m_msglen = strlen( pValue.m_msg );
	pValue.m_optime = (int)time( NULL );
	pValue.m_roll = roll;
	netsend_systalk_S( 0, SENDTYPE_NATION + nation, &pValue );

	// 保存起来
	if ( pValue.m_msglen < 127 )
	{
		SLK_NetS_Chat info = { 0 };
		info.m_actorid = -1;
		info.m_channel = CHAT_CHANNEL_NATION;
		strncpy( info.m_msg, pValue.m_msg, 127 );
		info.m_msglen = pValue.m_msglen;
		info.m_msgtype = CHAT_MSGTYPE_SYSTEM;
		info.m_optime = pValue.m_optime;
		info.m_nation = nation;
		if ( nation > 0 && nation <= 3 )
		{
			chat_cache_queue_add( g_ChatCacheNation[nation - 1], &info );
			chat_cache_queue_add_db( &info );
		}
	}
	return 0;
}

void system_talkjson_makestruct( SLK_NetS_SystalkJson *pValue, int textid, char *v1, char *v2, char *v3, char *v4, char *v5, char *v6, char roll )
{
	if ( v1 && v2 && v3 && v4 && v5 && v6 )
	{
		sprintf( pValue->m_msg, "{\"text\":%d,\"v1\":\"%s\",\"v2\":\"%s\",\"v3\":\"%s\",\"v4\":\"%s\",\"v5\":\"%s\",\"v6\":\"%s\"}", textid, v1, v2, v3, v4, v5, v6 );
	}
	else if ( v1 && v2 && v3 && v4 && v5 )
	{
		sprintf( pValue->m_msg, "{\"text\":%d,\"v1\":\"%s\",\"v2\":\"%s\",\"v3\":\"%s\",\"v4\":\"%s\",\"v5\":\"%s\"}", textid, v1, v2, v3, v4, v5 );
	}
	else if ( v1 && v2 && v3 && v4 )
	{
		sprintf( pValue->m_msg, "{\"text\":%d,\"v1\":\"%s\",\"v2\":\"%s\",\"v3\":\"%s\",\"v4\":\"%s\"}", textid, v1, v2, v3, v4 );
	}
	else if ( v1 && v2 && v3 )
	{
		sprintf( pValue->m_msg, "{\"text\":%d,\"v1\":\"%s\",\"v2\":\"%s\",\"v3\":\"%s\"}", textid, v1, v2, v3 );
	}
	else if ( v1 && v2 )
	{
		sprintf( pValue->m_msg, "{\"text\":%d,\"v1\":\"%s\",\"v2\":\"%s\"}", textid, v1, v2 );
	}
	else if ( v1 )
	{
		sprintf( pValue->m_msg, "{\"text\":%d,\"v1\":\"%s\"}", textid, v1 );
	}
	else
	{
		sprintf( pValue->m_msg, "{\"text\":%d}", textid );
	}
	pValue->m_msglen = strlen( pValue->m_msg );
	pValue->m_optime = (int)time( NULL );
	pValue->m_roll = roll;
}

int system_talkjson_to( int actor_index, int textid, char *v1, char *v2, char *v3, char *v4, char *v5, char *v6, char roll )
{
	SLK_NetS_SystalkJson pValue = { 0 };
	system_talkjson_makestruct( &pValue, textid, v1, v2, v3, v4, v5, v6, roll );
	netsend_systalkjson_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

int system_talkjson( int zone, int nation, int textid, char *v1, char *v2, char *v3, char *v4, char *v5, char *v6, char roll )
{
	SLK_NetS_SystalkJson pValue = { 0 };
	system_talkjson_makestruct( &pValue, textid, v1, v2, v3, v4, v5, v6, roll );
	netsend_systalkjson_S( zone, SENDTYPE_NATION + nation, &pValue );

	// 保存起来
	if ( pValue.m_msglen < 127 )
	{
		SLK_NetS_Chat info = { 0 };
		info.m_actorid = -1;
		info.m_channel = CHAT_CHANNEL_NATION;
		strncpy( info.m_msg, pValue.m_msg, 127 );
		info.m_msglen = pValue.m_msglen;
		info.m_msgtype = CHAT_MSGTYPE_SYSTEMJSON;
		info.m_optime = pValue.m_optime;
		info.m_zone = zone;
		info.m_nation = nation;
		if ( nation > 0 && nation <= 3 )
		{
			chat_cache_queue_add( g_ChatCacheNation[nation - 1], &info );
			chat_cache_queue_add_db( &info );
		}
	}
	return 0;
}

int system_talkjson_world( int textid, char *v1, char *v2, char *v3, char *v4, char *v5, char *v6, char roll )
{
	SLK_NetS_SystalkJson pValue = { 0 };
	system_talkjson_makestruct( &pValue, textid, v1, v2, v3, v4, v5, v6, roll );
	netsend_systalkjson_S( 0, SENDTYPE_WORLD, &pValue );

	// 保存起来
	if ( pValue.m_msglen < 127 )
	{
		SLK_NetS_Chat info = { 0 };
		info.m_actorid = -1;
		info.m_channel = CHAT_CHANNEL_WORLD;
		strncpy( info.m_msg, pValue.m_msg, 127 );
		info.m_msglen = pValue.m_msglen;
		info.m_msgtype = CHAT_MSGTYPE_SYSTEMJSON;
		info.m_optime = pValue.m_optime;
		chat_cache_queue_add( g_ChatCacheNation[0], &info );
		chat_cache_queue_add( g_ChatCacheNation[1], &info );
		chat_cache_queue_add( g_ChatCacheNation[2], &info );
		chat_cache_queue_add_db( &info );
	}
	return 0;
}

int system_rollchat( int actor_index, const char *szMsg )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_RollMsg pValue = { 0 };
	sprintf( pValue.m_title, "{\"nation\":%d, \"name\":\"%s\"}", pCity->nation, pCity->name );
	pValue.m_titlelen = strlen( pValue.m_title );
	strncpy( pValue.m_msg, szMsg, 1023 );
	pValue.m_msglen = strlen( pValue.m_msg );
	netsend_rollmsg_S( 0, SENDTYPE_WORLD, &pValue );
	return 0;
}

int system_rollto( int actor_index, const char *szMsg )
{
	SLK_NetS_RollMsg pValue = { 0 };
	strncpy( pValue.m_msg, szMsg, 1023 );
	pValue.m_msglen = strlen( pValue.m_msg );
	netsend_rollmsg_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

int system_rolltonation( int nation, const char *szMsg )
{
	SLK_NetS_RollMsg pValue = { 0 };
	strncpy( pValue.m_msg, szMsg, 1023 );
	pValue.m_msglen = strlen( pValue.m_msg );
	netsend_rollmsg_S( 0, SENDTYPE_NATION+nation, &pValue );
	return 0;
}

void system_rolljson_makestruct( SLK_NetS_RollMsgJson *pValue, int textid, char *v1, char *v2, char *v3, char *v4, char *v5, char *v6 )
{
	if ( v1 && v2 && v3 && v4 && v5 && v6 )
	{
		sprintf( pValue->m_msg, "{\"text\":%d,\"v1\":\"%s\",\"v2\":\"%s\",\"v3\":\"%s\",\"v4\":\"%s\",\"v5\":\"%s\",\"v6\":\"%s\"}", textid, v1, v2, v3, v4, v5, v6 );
	}
	else if ( v1 && v2 && v3 && v4 && v5 )
	{
		sprintf( pValue->m_msg, "{\"text\":%d,\"v1\":\"%s\",\"v2\":\"%s\",\"v3\":\"%s\",\"v4\":\"%s\",\"v5\":\"%s\"}", textid, v1, v2, v3, v4, v5 );
	}
	else if ( v1 && v2 && v3 && v4 )
	{
		sprintf( pValue->m_msg, "{\"text\":%d,\"v1\":\"%s\",\"v2\":\"%s\",\"v3\":\"%s\",\"v4\":\"%s\"}", textid, v1, v2, v3, v4 );
	}
	else if ( v1 && v2 && v3 )
	{
		sprintf( pValue->m_msg, "{\"text\":%d,\"v1\":\"%s\",\"v2\":\"%s\",\"v3\":\"%s\"}", textid, v1, v2, v3 );
	}
	else if ( v1 && v2 )
	{
		sprintf( pValue->m_msg, "{\"text\":%d,\"v1\":\"%s\",\"v2\":\"%s\"}", textid, v1, v2 );
	}
	else if ( v1 )
	{
		sprintf( pValue->m_msg, "{\"text\":%d,\"v1\":\"%s\"}", textid, v1 );
	}
	else
	{
		sprintf( pValue->m_msg, "{\"text\":%d}", textid );
	}
	pValue->m_msglen = strlen( pValue->m_msg );
}

int system_rolljson_to( int actor_index, int textid, char *v1, char *v2, char *v3, char *v4, char *v5, char *v6 )
{
	SLK_NetS_RollMsgJson pValue = { 0 };
	system_rolljson_makestruct( &pValue, textid, v1, v2, v3, v4, v5, v6 );
	netsend_rollmsgjson_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

int system_rolljson( int zone, int nation, int textid, char *v1, char *v2, char *v3, char *v4, char *v5, char *v6 )
{
	SLK_NetS_RollMsgJson pValue = { 0 };
	system_rolljson_makestruct( &pValue, textid, v1, v2, v3, v4, v5, v6 );
	netsend_rollmsgjson_S( zone, SENDTYPE_NATION + nation, &pValue );
	return 0;
}
