#ifndef _ARMY_H_
#define _ARMY_H_
#include "define.h"

// 单支部队状态
#define ARMY_STATE_IDLE				0	// 空闲
#define ARMY_STATE_GROUP_END		3	// 集结完毕
#define ARMY_STATE_READY			4	// 准备中
#define ARMY_STATE_MARCH			10	// 行军中
#define ARMY_STATE_FIGHT			20	// 战斗中
#define ARMY_STATE_OCCUPY			21	// 驻扎中
#define ARMY_STATE_GATHER			22	// 采集中
#define ARMY_STATE_SHELTER			23	// 庇护中
#define ARMY_STATE_HELP				24	// 援助中
#define ARMY_STATE_TALK				25	// 交谈中
#define ARMY_STATE_REBACK			30	// 返程中
#define ARMY_STATE_KINGWAR_READY	40	// 皇城战已到集结点
#define ARMY_STATE_KINGWAR_FIGHT	41	// 皇城战排队战斗中
#define ARMY_STATE_KINGWAR_WAITSOS	42	// 皇城战死亡等待救援中
#define ARMY_STATE_KINGWAR_DEAD		43	// 皇城战战斗已经死亡

// 行为
#define ARMY_ACTION_IDLE				0	// 空闲
#define ARMY_ACTION_FIGHT				1	// 去战斗
#define ARMY_ACTION_OCCUPY				2	// 去驻扎
#define ARMY_ACTION_GATHER				3	// 去采集
#define ARMY_ACTION_HELP_TROOP			6	// 去士兵援助
#define ARMY_ACTION_GROUP_CREATE		10	// 创建集结
#define ARMY_ACTION_GROUP_ATTACK		11	// 参与集结(攻击方)
#define ARMY_ACTION_GROUP_DEFENSE		12	// 参与集结(防御方)
#define ARMY_ACTION_NATION_ATTACK		13	// 参与国战(攻击方)
#define ARMY_ACTION_NATION_DEFENSE		14	// 参与国战(防御方)
#define ARMY_ACTION_KINGWAR				20	// 皇城战

#define ARMY_REBACK_NORMAL			0	// 返程类型-正常
#define ARMY_REBACK_RETURN			1	// 返程类型-操作返回
#define ARMY_REBACK_CALLBACK		2	// 返程类型-操作召回
#define ARMY_REBACK_FIGHTWIN		3	// 返程类型-战斗胜利返回
#define ARMY_REBACK_FIGHTLOSE		4	// 返程类型-战斗失败打回
#define ARMY_REBACK_REPATRIAT		5	// 返程类型-他人遣回

#define ARMY_STATE_FIGHT_TIME		2	// 战斗时长(S)
#define ARMY_CARRY_ITEMMAX			8	// 部队携带的道具
#define ARMY_HERO_MAX				4	// 部队最多武将

int army_load();
int army_save( FILE *fp );
Army *army_getptr( int army_index );
Army *army_getptr_cityhero( City *pCity, int herokind );

// 显示单元属性
void army_makeunit( int army_index, SLK_NetS_AddMapUnit *pAttr );

// 部队位置
void army_getpos( int army_index, short *posx, short *posy );

// 出征
int army_battle( City *pCity, SLK_NetC_MapBattle *info );

// 创建一支部队
int army_create( char from_type, int from_id, char to_type, int to_id, char stat, SLK_NetC_MapBattle *info );

// 删除一支部队
void army_delete( int army_index );

// 部队信息还原到城池
int army_tocity( int army_index );

// 设置部队状态
void army_setstate( int army_index, char state );

// 部队逻辑
void army_alllogic();

// 单只部队逻辑
void army_logic( int army_index );

// 获取部队所属城池
City *army_getcityptr( int army_index );

// 获取部队目标所属城池
City *army_getcityptr_target( int army_index );

// 获取部队名称
char *army_getname( int army_index );

// 获取部队目标名称
char *army_getname_target( int army_index );

// 获取部队所属城池
int army_getcityid( int army_index );

// 获取部队目标城池
int army_getcityid_target( int army_index );

// 获取部队所属国家
int army_getnation( int army_index );

// 获取部队目标国家
int army_getnation_target( int army_index );

// 部队添加携带道具
int army_carry_additem( int army_index, int itemkind, int itemnum );

// 采集时间
int army_gather_time( int army_index );
int army_gather_time_reset( City *pCity );

// 部队采集量计算
int army_gather_calc( int army_index );

// 部队采集中
int army_gather( int army_index );

// 部队庇护中
void army_shelter( int army_index );

// 部队援助中
void army_help( int army_index );

// 驻扎中
void army_occupy( int army_index );

// 部队交谈中
void army_talk( int army_index );

// 部队战斗中
void army_fight( int army_index );

// 检查部队是不是自己的
int army_myself( int actor_index, int army_index );

// 皇城血战队列待战
void army_kingwar_queue( int army_index );

// 皇城血战死亡CD
void army_kingwar_waitsos( int army_index );
#endif
