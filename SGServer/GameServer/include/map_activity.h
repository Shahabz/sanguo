#ifndef _MAP_ACTIVITY_H_
#define _MAP_ACTIVITY_H_
#include "define.h"

int map_activity_load();
int map_activity_save( FILE *fp );
int map_activity_maxcount_set( int maxcount );
MapActivity *map_activity_getptr( int index );
MapActivityInfo *map_activity_getconfig( int kind );

void map_activity_makeunit( int index, SLK_NetS_AddMapUnit *pAttr );
void map_activity_getpos( int index, short *posx, short *posy );
int map_activity_create( short kind, short posx, short posy, int deltime, int actorid );
int map_activity_delete( int index );
int map_activity_delete_withactivityid( int activityid );
int map_activity_num( int zoneid, int kind );
int map_activity_num_withactivityid( int zoneid, int activityid );
// 根据一个点的范围进行生成
int map_activity_range_brush( short kind, short posx, short posy, int range, int deltime, int actorid );

// 获取野怪奖励
int map_activity_sendaward( int actor_index, int kind );
#endif
