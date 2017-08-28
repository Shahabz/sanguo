#ifndef _MAP_ENEMY_H_
#define _MAP_ENEMY_H_
#include "define.h"

int map_enemy_load();
int map_enemy_save( FILE *fp );
int map_enemy_maxcount_set( int maxcount );
MapEnemy *map_enemy_getptr( int index );
MapEnemyInfo *map_enemy_getconfig( int kind );

// 显示单元属性
void map_enemy_makeunit( int index, SLK_NetS_AddMapUnit *pAttr );

// 位置
void map_enemy_getpos( int index, short *posx, short *posy );

// 创建一个流寇
int map_enemy_create( short kind, short posx, short posy, int deltime );

// 删除一个流寇
int map_enemy_delete( int index );

// 野怪数量
int map_enemy_num( int zoneid, int kind );

// 根据一个点的范围进行生成
int map_enemy_range_brush( short kind, short posx, short posy, int range, int deltime );

// 已经被选定次数,被选定的改周期不删除
void map_enemy_addselected( int index );
void map_enemy_subselected( int index );
#endif
