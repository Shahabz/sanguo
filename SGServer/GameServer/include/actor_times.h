#ifndef _ACTOR_TIMES_H_
#define _ACTOR_TIMES_H_

// 32位整数，标志位操作0或1 标示开没开，有没有，做没做（永久存档）
#define ACTOR_SFLAG_BODY_FREEBUY	0	// 首次购买体力免费
void actor_set_sflag( int actor_index, int offset, char value );
int actor_get_sflag( int actor_index, int offset );

// char 类型的每日数据（每天更新）
#define MAX_TODAY_COUNT		127
#define TODAY_CHAR_BUYBODY		0 // 今天购买体力次数
int actor_get_today_char_times( int actor_index, int offset );
int actor_add_today_char_times( int actor_index, int offset );
int actor_set_today_char_times( int actor_index, int offset, char value );

// int 类型的每日数据（每天更新）
#define MAX_TODAY_INTNUM	8
int actor_get_today_int_times( int actor_index, int offset );
int actor_add_today_int_times( int actor_index, int offset );
int actor_set_today_int_times( int actor_index, int offset, int value );

// CD时间
#define USELIMIT_CD_MAXCOUNT 8
int actor_set_uselimit_cd( int actor_index, char index, int cdtime );
int actor_add_uselimit_cd( int actor_index, char index, int cdtime );
int actor_check_uselimit_cd( int actor_index, char index );
int actor_clear_uselimit_cd( int actor_index, char index );

#endif
