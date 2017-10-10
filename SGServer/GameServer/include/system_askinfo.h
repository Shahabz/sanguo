#ifndef __SYSTEM_ASKINFO_H
#define __SYSTEM_ASKINFO_H
// 客户端发过来的简单请求

#define ASKINFO_NORMAL			0
#define ASKINFO_CHANGESHAPE		1
#define ASKINFO_CHANGENAME		2 // 修改名字
#define ASKINFO_CITYGUARD		3 // 城墙守卫
#define ASKINFO_QUEST			4 // 任务
#define ASKINFO_BUILDING		5 // 建筑
#define ASKINFO_LEVY			6 // 征收
#define ASKINFO_TRAIN			7 // 训练
#define ASKINFO_EQUIPFORGING    8  // 装备打造
#define ASKINFO_EQUIPWASH       9  // 装备洗炼
#define ASKINFO_EQUIP			10 // 装备
#define ASKINFO_ITEM			11 // 道具
#define ASKINFO_HERO			12 // 英雄
#define ASKINFO_TECH			13 // 科技
#define ASKINFO_GOV				14 // 官府 
#define ASKINFO_OFFICIALHIRE	15 // 雇佣 
#define ASKINFO_QUICK			16 // 加速
#define ASKINFO_ACTOR			17 // 角色操作
#define ASKINFO_MATERIALMAKE	18 // 材料制造
#define ASKINFO_STORY			19 // 副本
#define ASKINFO_WORLDMAP		20 // 世界地图操作
#define ASKINFO_MAP_EVENT		21 // 世界地图随机事件
#define ASKINFO_SPY_CITY		22 // 侦察城池
#define ASKINFO_CITYHELP		23 // 驻防信息
#define ASKINFO_CITYARMYGROUP	24 // 城战
#define ASKINFO_NATIONARMYGROUP	25 // 国战

int system_askinfo( int actor_index, int msgid, char *pstr, int *pvalue );

#endif
