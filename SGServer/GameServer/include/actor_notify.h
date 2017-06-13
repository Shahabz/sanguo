#ifndef __ACTOR_NOTIFY_H
#define __ACTOR_NOTIFY_H
#include "server_structsend_auto.h"

#define NOTIFY_NORMAL		0	// 服务端发文字信息
#define	NOTIFY_TEXTTABLE	1	// 
#define	NOTIFY_ITEM			2
#define	NOTIFY_ITEMNUM		3
#define	NOTIFY_LOGIN_QUEUE	4

// 发送给角色短消息 无参数
int actor_notify( int actor_index, short msgid, const char *msg );

// 发送给角色短消息 带参数
int actor_notify_value( int actor_index, short msgid, char count, const int *data, const char *msg );

// 发消息提示，通过消息ID
int actor_system_message( int actor_index, int msgid );

#endif
