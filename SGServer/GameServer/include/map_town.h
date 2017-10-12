#ifndef _MAP_TOWN_H_
#define _MAP_TOWN_H_
#include "define.h"

#define MAP_TOWN_JOIN_MAX	128	// 参战人员
#define MAP_TOWN_MONSTER_MAX	16	// 守卫数量

int map_town_load();
int map_town_save( FILE *fp );
MapTown *map_town_getptr( int townid );

// 显示单元属性
void map_town_makeunit( int index, SLK_NetS_AddMapUnit *pAttr );

// 位置
void map_town_getpos( int index, short *posx, short *posy );

// 重置守军
void map_town_monster_reset( int townid );

// 所有城镇每秒逻辑
void map_town_alllogic();

// 申请城主
int map_town_ask_owner( int actor_index, int townid );

// 分配城主
int map_town_alloc_owner( int townid );

// 放弃城主
int map_town_owner_leave( int actor_index, int townid );

// 补兵
int map_town_soldiers_repair( int actor_index, int townid );

// 城镇征收
int map_town_levy( int actor_index, int townid );

// 获取城镇奖励
int map_town_sendaward( int actor_index, int townid );

// 获取城镇信息
int map_town_sendinfo( int actor_index, int townid );

// 申请列表
int map_town_ask_owner_sendlist( int actor_index, int townid );
#endif
