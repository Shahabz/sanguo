#ifndef _MAP_TOWN_H_
#define _MAP_TOWN_H_
#include "define.h"

#define MAP_TOWN_JOIN_MAX	128	// 参战人员

int map_town_load();
int map_town_save( FILE *fp );
MapTown *map_town_getptr( int townid );

// 显示单元属性
void map_town_makeunit( int index, SLK_NetS_AddMapUnit *pAttr );

// 位置
void map_town_getpos( int index, short *posx, short *posy );

// 所有城镇每秒逻辑
void map_town_alllogic();

// 申请城主
int map_town_ask_owner( int townid, int actor_index );

// 分配城主
int map_town_alloc_owner( int townid );

// 获取城镇奖励
int map_town_sendaward( int actor_index, int townid );

// 获取城镇信息
int map_town_sendinfo( int actor_index, int townid );


#endif
