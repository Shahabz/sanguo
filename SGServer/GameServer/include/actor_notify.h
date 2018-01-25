#ifndef __ACTOR_NOTIFY_H
#define __ACTOR_NOTIFY_H
#include "server_structsend_auto.h"

#define POP( actor_index, msgid ) actor_notify_pop( actor_index, msgid )
#define SUBSCRIBE_CMD_KINGWARDLG	1

#define NOTIFY_NORMAL		0	
#define	NOTIFY_TEXTTABLE	1	
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
#define NOTIFY_LOSTREBUILD	15	// 高级重建次数
#define NOTIFY_MAPZONEGOZC	16	// 前往州城的显示和隐藏
#define NOTIFY_MSGBOX		17	// 弹出消息选择框
#define NOTIFY_BOX			18	// 弹出消息框
#define NOTIFY_POP			19	// 弹出消息，无框
#define NOTIFY_BUILDINGFINISH	20	// 建筑完成
#define NOTIFY_CITYGUARDNUM		21	// 城防军数量
#define NOTIFY_HERO_VISIT		22	// 武将寻访
#define NOTIFY_MAPCALL			23	// 地图召唤
#define NOTIFY_HEROBACK			24	// 武将回城

// 发送给角色短消息 无参数
int actor_notify( int actor_index, short msgid, const char *msg );

// 发送给角色短消息 带参数
int actor_notify_value( int actor_index, short msgid, char count, const int *data, const char *msg );

// 发消息提示，通过消息ID
int actor_notify_pop( int actor_index, int textid );
int actor_notify_pop_v( int actor_index, int textid, char *v1, char *v2 );

// npc说话
int npc_talk( int actor_index, int textid, int btntextid );

// 武将说话
int hero_talk( int actor_index, int herokind, int textid );

// 弹框消息
int actor_notify_box( int actor_index, int textid );
int actor_notify_box_v( int actor_index, int textid, char *v1, char *v2 );

// 弹出确定消息
int actor_notify_alert( int actor_index, int textid );

// 弹出确定消息带参数
int actor_notify_alert_v( int actor_index, int textid, char *v1, char *v2 );

// 消息提示框
#define MSGBOX_CALLBACK_KINGWAR_DEFENSE	1 // 血战提示前往转回防
#define MSGBOX_CALLBACK_KINGWAR_SNEAK	2 // 血城战提示进攻转偷袭
// 弹出消息选择框
int actor_notify_msgbox_v( int actor_index, int msgid, int value1, int value2, int textid, char *v1, char *v2 );
int actor_notify_msgbox_callback( int actor_index, int msgid, int value1, int value2 );

// 通知UI更新
#define UI_UPDATE_NATIONFIGHT		1 // 国战城镇有变化
#define UI_UPDATE_FIGHTINFO			2 // 战斗信息变化
#define UI_UPDATE_NATIONOFFICIAL	3 // 官员状态变化
void ui_update( int actor_index, char send_type, char ui );
#endif
