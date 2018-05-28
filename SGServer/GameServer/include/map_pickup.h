#ifndef _MAP_PICKUP_H_
#define _MAP_PICKUP_H_
#include "define.h"

int map_pickup_load();
int map_pickup_save( FILE *fp );
int map_pickup_maxcount_set( int maxcount );
MapPickup *map_pickup_getptr( int index );
MapPickupInfo *map_pickup_getconfig( int kind );

// 显示单元属性
void map_pickup_makeunit( int index, SLK_NetS_AddMapUnit *pAttr );

// 位置
void map_pickup_getpos( int index, short *posx, short *posy );

// 创建一个拾取物品
int map_pickup_create( short kind, short posx, short posy );

// 删除一个拾取物品
int map_pickup_delete( int index );

// 拾取物品数量
int map_pickup_num( int zoneid, int kind );

// 拾取
int map_pickup_get( int actor_index, int unit_index );
#endif
