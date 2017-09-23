#ifndef __ACTOR_NOTIFY_H
#define __ACTOR_NOTIFY_H
#include "server_structsend_auto.h"

#define POP( actor_index, msgid ) actor_system_message( actor_index, msgid )

#define NOTIFY_NORMAL		0	// 服务端发文字信息
#define	NOTIFY_TEXTTABLE	1	// 
#define	NOTIFY_ITEM			2
#define	NOTIFY_ITEMNUM		3
#define	NOTIFY_LOGIN_QUEUE	4
#define	NOTIFY_CHANGESHAPE	5	// 玩家头像修改
#define NOTIFY_CHANGENAME	6	// 玩家修改名称
#define NOTIFY_NPCTALK		7	// NPC对话
#define NOTIFY_EQUIP		8	// 装备
#define NOTIFY_ACTOR		9	// 角色
#define NOTIFY_ACTORSFLAG	10	// 角色标志位
#define NOTIFY_WORLDMAP		11	// 世界地图
#define NOTIFY_VALUECHANGE	12	// 某些值改变的通用通知
#define NOTIFY_MAIL			13	// 邮件
#define NOTIFY_ALERT		14	// 弹出确定消息

// 发送给角色短消息 无参数
int actor_notify( int actor_index, short msgid, const char *msg );

// 发送给角色短消息 带参数
int actor_notify_value( int actor_index, short msgid, char count, const int *data, const char *msg );

// 发消息提示，通过消息ID
int actor_system_message( int actor_index, int msgid );

// npc对话
int npc_talk( int actor_index, int textid );

// 弹出确定消息
int actor_notify_alert( int actor_index, int textid );
#endif
