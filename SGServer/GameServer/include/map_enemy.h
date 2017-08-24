#ifndef _MAP_ENEMY_H_
#define _MAP_ENEMY_H_
#include "define.h"

int map_enemy_load();
int map_enemy_save( FILE *fp );
MapEnemy *map_enemy_getptr( int index );

// 显示单元属性
void map_enemy_makeunit( int index, SLK_NetS_AddMapUnit *pAttr );

// 位置
void map_enemy_getpos( int index, short *posx, short *posy );

#endif
