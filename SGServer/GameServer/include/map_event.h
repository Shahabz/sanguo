#ifndef _MAP_EVENT_H_
#define _MAP_EVENT_H_
#include "define.h"

int map_event_load();
int map_event_save( FILE *fp );
int map_event_maxcount_set( int maxcount );
MapEvent *map_event_getptr( int index );
MapEventInfo *map_event_getconfig( int kind );

// 显示单元属性
void map_event_makeunit( int index, SLK_NetS_AddMapUnit *pAttr, int actorid );

// 位置
void map_event_getpos( int index, short *posx, short *posy );

// 创建一个事件
int map_event_create( short kind, short posx, short posy, int city_index );

// 删除一个事件
int map_event_delete( int index, int city_index );

// 改变事件坐标
int map_event_changepos( int index, short posx, short posy );
int map_event_changepos_rand( int index );
int map_event_changepos_randhaspos( int index, short posx, short posy );

// 每个玩家的事件逻辑
int map_event_citylogic( int city_index );

// 删除属于这玩家的事件
int map_event_delete_actor( int actorid, int city_index );

// 事件数量
int map_event_num( int actorid );

// 根据一个点的范围进行生成
int map_event_range_brush( short kind, short posx, short posy, int range, int city_index );

int map_event_addcity( int city_index, int index );
int map_event_deletecity( int city_index, int index );
int map_event_addcity( int city_index, int index );

int map_event_gather( int actor_index,int unit_index );
int map_event_getaward( int actor_index, int unit_index );
#endif
