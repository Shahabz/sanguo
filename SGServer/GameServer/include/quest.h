#ifndef _QUEST_H_
#define _QUEST_H_
#include "define.h"

#define QUEST_MAXNUM		2000		// 最大任务数量

#define QUEST_COMPLETEFLAG_NORMAL	0	// 缺省
#define QUEST_COMPLETEFLAG_SUCCESS	1	// 完成
#define QUEST_COMPLETEFLAG_AWARDED	2	// 已经领取奖励

#define QUEST_DATAINDEX_WELCOME				0	// 欢迎任务
#define QUEST_DATAINDEX_ACTOR_LEVEL			1	// 主公等级 datatype=1 datakind=0 needvalue=等级
#define	QUEST_DATAINDEX_BUILDING_LEVEL		2	// 建筑等级 datatype=2 datakind=建筑kind dataoffset=资源建筑编号 needvalue=建筑等级
#define	QUEST_DATAINDEX_BUILDING_SILVER		3	// N个民居达到N级 datatype=3 datakind=等级 needvalue=建筑数量
#define	QUEST_DATAINDEX_BUILDING_WOOD		4	// N个木场达到N级 datatype=4 datakind=等级 needvalue=建筑数量
#define	QUEST_DATAINDEX_BUILDING_FOOD		5	// N个农田达到N级 datatype=5 datakind=等级 needvalue=建筑数量
#define	QUEST_DATAINDEX_BUILDING_IRON		6	// N个矿场达到N级 datatype=6 datakind=等级 needvalue=建筑数量
#define QUEST_DATAINDEX_LEVYNUM				7	// 征收次数 datatype=7 datakind=0 needvalue=数量
#define QUEST_DATAINDEX_HISTORY				8	// 副本 datatype=8 datakind=副本编号 needvalue=1
#define	QUEST_DATAINDEX_KILLMONSTER			9	// 打流寇N次 datatype=9 datakind=0 needvalue=数量
#define	QUEST_DATAINDEX_KILLMONSTER_LEVEL	10	// 打流寇N级N次 datatype=10 datakind=等级 needvalue=数量
#define	QUEST_DATAINDEX_EQUIP_FORGING		11	// 打造N装备N数量 datatype=11 datakind=装备kind needvalue=数量
#define	QUEST_DATAINDEX_EQUIP_WASH			12	// 装备洗炼次数 datatype=12 datakind=0 needvalue=数量
#define	QUEST_DATAINDEX_EQUIP_COLORWASH		13	// 将N品质装备的属性洗炼至满级 datatype=13 datakind=颜色 needvalue=1
#define	QUEST_DATAINDEX_TRAIN				14	// 募兵N兵种N数量 datatype=14 datakind=兵种（1，2，3） needvalue=数量
#define	QUEST_DATAINDEX_TRAINCOUNT			15	// 募兵N次 datatype=15 datakind=0 needvalue=数量
#define	QUEST_DATAINDEX_CITY_TECH			16	// 研究N科技N级 datatype=16 datakind=科技ID needvalue=等级
#define	QUEST_DATAINDEX_CITY_TECHONE		17	// 研究一次N科技 datatype=17 datakind=科技ID needvalue=1
#define	QUEST_DATAINDEX_HERO_WASHCOUNT		18	// 武将洗髓N次 datatype=18 datakind=0 needvalue=次数
#define	QUEST_DATAINDEX_HERO_COLORWASH		19	// 将一名N品质的武将属性洗满 datatype=19 datakind=颜色 needvalue=1
#define	QUEST_DATAINDEX_HERO_CALL			20	// 招募N名武将 datatype=20 datakind=0 needvalue=数量
#define	QUEST_DATAINDEX_WISHING				21	// 聚宝盆N次 datatype=21 datakind=0 needvalue=次数
#define	QUEST_DATAINDEX_CITYFIGHT			22	// 成功击败N名敌国玩家 datatype=22 datakind=0 needvalue=次数

// 任务类型
enum 
{
	QUEST_TYPE_MAIN = 0,	// 主线任务
	QUEST_TYPE_BRANCH,		// 支线任务
	QUEST_TYPE_MAXNUM
};


// 任务列表信息初始化
int questlist_init();

//重读
int questlist_reload();

// 获取任务
int quest_give( int actor_index );

// 设置任务值
int quest_addvalue( struct _city *pCity, int dataindex, int datakind, int value );

// 获取任务状态
int quest_getcomplete( int actor_index, int questid, int *value );

// 设置任务状态
int quest_setcomplete( int actor_index, int questid, int flag );

// 任务检查
int quest_check( int actor_index, int questid, int *value );

// 任务检查
int quest_checkcomplete( struct _city *pCity );

// 重置所有任务
int quest_reset( int actor_index );

// 领取任务奖励
int quest_getaward( int actor_index, int questid, char type );

// 任务列表
int quest_sendlist( int actor_index );

// 任务奖励
int quest_sendawardinfo( int actor_index, int questid );

// 发送任务提示
//int quest_sendnotify( int actor_index );

#endif
