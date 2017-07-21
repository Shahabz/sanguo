#ifndef _CHAT_H_
#define _CHAT_H_
#include "server_netsend_auto.h"

#define CHAT_CACHE_QUEUE_COUNT	100

#define CHAT_CHANNEL_NATION	0	// 国家
#define CHAT_CHANNEL_ZONE	1	// 区域
#define CHAT_CHANNEL_WORLD	2	// 世界

int chat_actortalk( int actor_index, char channel, char *msg );
int chat_send_zone( SLK_NetS_Chat *pValue );
int chat_send_nation( SLK_NetS_Chat *pValue );
int chat_send_world( SLK_NetS_Chat *pValue );
int chat_cache_queue_add( SLK_NetS_Chat *pCache, SLK_NetS_Chat *pValue );
int chat_cache_queue_add_db( SLK_NetS_Chat *pValue );
int chat_cache_load();
int chat_cache_sendlist( int actor_index );

int system_talk( const char *szMsg );
int system_talkto( int actor_index, const char *szMsg );
int system_talktonation( int nation, const char *szMsg );

int system_talk( int nameid, char *v1 = NULL, char *v2 = NULL, char *v3 = NULL, char *v4 = NULL, char *v5 = NULL, char *v6 = NULL, char *v7 = NULL, char *v8 = NULL );
int system_talkto( int actor_index, int nameid, char *v1 = NULL, char *v2 = NULL, char *v3 = NULL, char *v4 = NULL, char *v5 = NULL, char *v6 = NULL, char *v7 = NULL, char *v8 = NULL );
int system_talktonation( int nation, int nameid, char *v1 = NULL, char *v2 = NULL, char *v3 = NULL, char *v4 = NULL, char *v5 = NULL, char *v6 = NULL, char *v7 = NULL, char *v8 = NULL );
#endif
