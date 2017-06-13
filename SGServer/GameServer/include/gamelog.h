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

/* 途径 */
#define PATH_SYSTEM						1	// 系统
#define PATH_GM							2	// GM
#define PATH_PAY						3	// 充值
#define PATH_ITEMUSE					4	// 道具使用
#define PATH_TOKENITEMUSE				5	// 钻石直接使用
#define PATH_SELL						6	// 售卖丢弃

int log_init();
int wlog( char type, int op, char path, int object, int objectnum, i64 objectid, int source, int target );
int wlog_token( char type, int op, char path, int object, int objectnum, i64 objectid, int source, int target, i64 userid );
int wcount();

#endif

