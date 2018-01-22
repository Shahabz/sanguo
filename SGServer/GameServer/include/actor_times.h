#ifndef _ACTOR_TIMES_H_
#define _ACTOR_TIMES_H_

// 32位整数，标志位操作0或1 标示开没开，有没有，做没做（永久存档）
#define ACTOR_SFLAG_BODY_FREEBUY	0	// 首次购买体力免费
#define ACTOR_SFLAG_MAPZONE_GO_ZC	1	// 首次免费前往州城
#define ACTOR_SFLAG_QUEST_AUTOBUILD	2	// 新手任务是否领取过自动建造
#define ACTOR_SFLAG_SKIPFIGHT		3	// 是否可以跳过战斗
#define ACTOR_SFLAG_PALACE			4	// 是否开启皇宫内院
#define ACTOR_SFLAG_STORYSWEEP		5	// 是否开启扫荡副本
#define	ACTOR_SFLAG_OFFICIAL_TECH	6   // 是否开启紫色研究员
#define ACTOR_SFLAG_EQUPIPDRAWING	7	// 是否开启装备分解获得图纸
#define	ACTOR_SFLAG_MATERIAL_MAKEWILL 8 // 是否开启作坊预设
void actor_set_sflag( int actor_index, int offset, char value );
int actor_get_sflag( int actor_index, int offset );

// char 类型的每日数据（每天更新）
#define MAX_TODAY_COUNT		127
#define TODAY_CHAR_BUYBODY			0 // 今天购买体力次数
#define	TODAY_CHAR_LOGINAWARD		1 // 今天登陆奖励
#define TODAY_CHAR_WORLDBOSS		2 // 今天是否打过世界boss
#define TODAY_CHAR_SHOP_SALEITEM	3 // 今天是否随机了打折物品
#define TODAY_CHAR_ITEMUSE_BODY		4 // 今天使用体力道具多少次了
#define TODAY_CHAR_NATION_DONATE	5 // 今天国家建设次数
#define TODAY_CHAR_NATION_MISSION_AWARD1	6 // 今天是否领取国家荣誉任务奖励
#define TODAY_CHAR_NATION_MISSION_AWARD2	7 // 今天是否领取国家荣誉任务奖励
#define TODAY_CHAR_NATION_MISSION_AWARD3	8 // 今天是否领取国家荣誉任务奖励
#define TODAY_CHAR_MAPCALLNUM		9	// 今天召唤次数	

int actor_get_today_char_times( int actor_index, int offset );
int actor_add_today_char_times( int actor_index, int offset );
int actor_set_today_char_times( int actor_index, int offset, char value );

// short 类型的每日数据（每天更新）
#define MAX_TODAY_SHORT_COUNT		32
#define TODAY_SHORT_VIPSHOP_BUYNUM_BASE		0	// 今天购买指定道具次数
#define TODAY_SHORT_VIPSHOP_BUYNUM_END		24	// 今天购买指定道具次数
int actor_get_today_short_times( int actor_index, int offset );
int actor_add_today_short_times( int actor_index, int offset, short value );
int actor_set_today_short_times( int actor_index, int offset, short value );

// int 类型的每日数据（每天更新）
#define MAX_TODAY_INTNUM	8
int actor_get_today_int_times( int actor_index, int offset );
int actor_add_today_int_times( int actor_index, int offset );
int actor_set_today_int_times( int actor_index, int offset, int value );

// CD时间
#define USELIMIT_CD_MAXCOUNT 8
#define USELIMIT_CD_TOWN_DEV	0 // 都城开发冷却
int actor_set_uselimit_cd( int actor_index, char index, int cdtime );
int actor_add_uselimit_cd( int actor_index, char index, int cdtime );
int actor_check_uselimit_cd( int actor_index, char index );
int actor_clear_uselimit_cd( int actor_index, char index );

void _check_fday( int actor_index );

// 每日登陆奖励
int actor_loginaward( int actor_index );
#endif
