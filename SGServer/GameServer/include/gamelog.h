#ifndef __GAMELOG_H
#define __GAMELOG_H

#include "utils.h"

/* 操作 */
#define LOGOP_TOKEN				1	// 钻石
#define LOGOP_HERO				2	// 英雄
#define LOGOP_ITEMGET			3	// 获得道具
#define LOGOP_ITEMLOST			4	// 丢失道具
#define LOGOP_ACTOREXP			5	// 角色经验
#define LOGOP_UPGRADE			6	// 角色升级
#define LOGOP_BODY				7	// 体力
#define LOGOP_VIPEXP			8	// vip经验
#define LOGOP_BUILDING			9	// 建筑
#define LOGOP_EQUIPGET			10	// 获得装备
#define LOGOP_EQUIPLOST			11	// 丢失装备
#define LOGOP_QUEST				12	// 任务
#define LOGOP_SILVER			13	// 银币
#define LOGOP_WOOD				14	// 木材
#define LOGOP_FOOD				15	// 粮食
#define LOGOP_IRON				16	// 镔铁

/* 途径 */
#define PATH_SYSTEM						1	// 系统
#define PATH_GM							2	// GM
#define PATH_PAY						3	// 充值
#define PATH_ITEMUSE					4	// 道具使用
#define PATH_TOKENITEMUSE				5	// 钻石直接使用
#define PATH_SELL						6	// 售卖丢弃
#define PATH_BUILDING_UPGRADE			7	// 建筑
#define PATH_QUEST						8	// 任务
#define PATH_GUARD_UPGRADE				9	// 守卫升级
#define PATH_LEVY						10	// 征收
#define PATH_TRAIN						11	// 训练
#define PATH_RESOLVE					12	// 分解
#define PATH_EQUIPEXT					13	// 购买装备容量

int log_init();
int wlog( char type, int op, short path, int object, int objectnum, i64 objectid, int source, int target );
int wlog_token( char type, int op, short path, int object, int objectnum, i64 objectid, int source, int target, i64 userid );
int wcount();

#endif

