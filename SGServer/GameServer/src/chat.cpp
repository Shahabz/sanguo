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

SLK_NetS_Chat g_ChatCacheZone[CHAT_CACHE_QUEUE_COUNT];
SLK_NetS_Chat g_ChatCacheNation[CHAT_CACHE_QUEUE_COUNT];

int chat_actortalk( int actor_index, char channel, char *msg )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
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

	if ( channel == CHAT_CHANNEL_ZONE )
	{
		chat_send_zone( &pValue );
	}
	else if ( channel == CHAT_CHANNEL_NATION )
	{
		chat_send_nation( &pValue );
	}
	else if ( channel == CHAT_CHANNEL_WORLD )
	{
		chat_send_world( &pValue );
	}
	
	return 0;
}

// 发送到本区域
int chat_send_zone( SLK_NetS_Chat *pValue )
{
	netsend_chat_S( pValue->m_zone, SENDTYPE_ZONE, pValue );
	chat_cache_queue_add( g_ChatCacheZone, pValue );
	chat_cache_queue_add_db( pValue );
	return 0;
}

// 发送到国家
int chat_send_nation( SLK_NetS_Chat *pValue )
{
	netsend_chat_S( pValue->m_nation, SENDTYPE_NATION, pValue );
	chat_cache_queue_add( g_ChatCacheNation, pValue );
	chat_cache_queue_add_db( pValue );
	return 0;
}

// 发送到世界
int chat_send_world( SLK_NetS_Chat *pValue )
{
	netsend_chat_S( 0, SENDTYPE_WORLD, pValue );
	chat_cache_queue_add( g_ChatCacheZone, pValue );
	chat_cache_queue_add( g_ChatCacheNation, pValue );
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
	sprintf( szSQL, "INSERT INTO chat ( channel, actorid, shape, level, frame, nation, zone, place, name, msg, optime ) VALUES ('%d','%d','%d','%d','%d','%d','%d','%d','%s','%s','%d')",
		pValue->m_channel, pValue->m_actorid, pValue->m_shape, pValue->m_level, pValue->m_frame, pValue->m_nation, pValue->m_zone, pValue->m_place, szText_name, szContent, pValue->m_optime );
	dbwork_addsql( szSQL, DBWORK_CMD_CHAT_CACHE, 0 );
	return 0;
}

int chat_cache_loaddb( SLK_NetS_Chat *pCache, int channel )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[1024];
	sprintf( szSQL, "SELECT channel, actorid, shape, level, frame, nation, zone, place, name, msg, optime FROM (SELECT * FROM chat WHERE channel='%d' or channel='%d' ORDER BY optime DESC LIMIT %d ) a ORDER BY optime ASC;", channel, CHAT_CHANNEL_WORLD, CHAT_CACHE_QUEUE_COUNT );
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
		strncpy( info.m_msg, row[4], 127 );
		info.m_msglen = strlen( info.m_msg );
		info.m_optime = atoi( row[index++] );
		chat_cache_queue_add( pCache, &info );
	}
	mysql_free_result( res );
	return 0;
}

int chat_cache_load()
{
	chat_cache_loaddb( g_ChatCacheZone, CHAT_CHANNEL_ZONE );
	chat_cache_loaddb( g_ChatCacheNation, CHAT_CHANNEL_NATION );
	return 0;
}
