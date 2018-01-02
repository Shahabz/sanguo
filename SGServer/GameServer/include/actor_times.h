#ifndef _ACTOR_TIMES_H_
#define _ACTOR_TIMES_H_

// 32位整数，标志位操作0或1 标示开没开，有没有，做没做（永久存档）
#define ACTOR_SFLAG_BODY_FREEBUY	0	// 首次购买体力免费
#define ACTOR_SFLAG_MAPZONE_GO_ZC	1	// 首次免费前往州城
#define ACTOR_SFLAG_QUEST_AUTOBUILD	2	// 新手任务是否领取过自动建造
void actor_set_sflag( int actor_index, int offset, char value );
int actor_get_sflag( int actor_index, int offset );

// char 类型的每日数据（每天更新）
#define MAX_TODAY_COUNT		127
#define TODAY_CHAR_BUYBODY			0 // 今天购买体力次数
#define	TODAY_CHAR_LOGINAWARD		1 // 今天登陆奖励
#define TODAY_CHAR_WORLDBOSS		2 // 今天是否打过世界boss
#define TODAY_CHAR_SHOP_SALEITEM	3 // 今天是否随机了打折物品
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
