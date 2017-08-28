#ifndef _ARMY_H_
#define _ARMY_H_
#include "define.h"

// 单支部队状态
#define ARMY_STATE_IDLE				0	// 空闲
#define ARMY_STATE_GROUP_START		1	// 发起集结
#define ARMY_STATE_GROUP_ING		2	// 集结中
#define ARMY_STATE_GROUP_END		3	// 到达指定集结地点,集结完毕
#define ARMY_STATE_READY			4	// 准备中
#define ARMY_STATE_MARCH			10	// 行军中
#define ARMY_STATE_FIGHT			20	// 战斗中
#define ARMY_STATE_OCCUPY			21	// 驻扎中
#define ARMY_STATE_GATHER			22	// 采集中
#define ARMY_STATE_SHELTER			23	// 庇护中
#define ARMY_STATE_HELP				24	// 援助中
#define ARMY_STATE_REBACK			30	// 返程中

// 行为
#define ARMY_ACTION_IDLE				0	// 空闲
#define ARMY_ACTION_FIGHT				1	// 去战斗
#define ARMY_ACTION_OCCUPY				2	// 去驻扎
#define ARMY_ACTION_GATHER				3	// 去采集

#define ARMY_ACTION_GROUP_START			10	// 发起集结中
#define ARMY_ACTION_GROUP_ING			11	// 去集结中


#define ARMY_STATE_FIGHT_TIME		3	// 战斗时长(S)
#define ARMY_CARRY_ITEMMAX			5	// 部队携带的道具

int army_load();
int army_save( FILE *fp );
Army *army_getptr( int army_index );

// 显示单元属性
void army_makeunit( int army_index, SLK_NetS_AddMapUnit *pAttr );

// 部队位置
void army_getpos( int army_index, short *posx, short *posy );

#endif
