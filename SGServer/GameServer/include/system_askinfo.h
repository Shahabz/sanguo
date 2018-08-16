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
#define ASKINFO_MAPTOWN			26 // 城镇
#define ASKINFO_MAPZONE			27 // 地区
#define ASKINFO_LOSTREBUILD		28 // 家园重建
#define ASKINFO_KINGWAR			29 // 皇城血战
#define ASKINFO_MSGBOX_CALLBACK 30 // 消息框点击确定回调
#define ASKINFO_AUTOBUILD		31 // 自动建造
#define ASKINFO_HERO_VISIT		32 // 武将寻访
#define ASKINFO_CHANGESIGN		33 // 个性签名
#define ASKINFO_SHOP			34 // 商店
#define ASKINFO_PAY				35 // 支付
#define ASKINFO_VIPSHOP			36 // VIP商店
#define ASKINFO_VIPBAG			37 // VIP礼包
#define ASKINFO_ACTIVITY		38 // 活动
#define ASKINFO_NATIONEQUIP		39 // 国器
#define ASKINFO_NATION			40 // 国家
#define ASKINFO_NATIONLOG		41 // 国家日志
#define ASKINFO_NATIONHERO		42 // 国家名将
#define ASKINFO_WISHING			43 // 聚宝盆
#define ASKINFO_TIANCEQUEST		44 // 天策府任务
#define ASKINFO_TEACHER			45 // 师徒
#define ASKINFO_GUAID			46 // 引导完成
#define	ASKINFO_CDKEY			47 // CDKEY
#define	ASKINFO_GIRL			48 // 女将
#define	ASKINFO_FANGSHI			49 // 坊市
#define	ASKINFO_GIRLSHOP		50 // 女将商店
#define	ASKINFO_FIGHTHELPER		51 // 雇佣兵
#define ASKINFO_INVITECODE		52 // 邀请码
#define ASKINFO_BUYRES			53 // 购买资源

int system_askinfo( int actor_index, int msgid, char *pstr, int *pvalue );

#endif
