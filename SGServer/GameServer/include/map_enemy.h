#ifndef _MAP_ENEMY_H_
#define _MAP_ENEMY_H_
#include "define.h"

// 显示单元属性
void map_enemy_makeunit( int index, SLK_NetS_AddMapUnit *pAttr );

// 位置
void map_enemy_getpos( int index, short *posx, short *posy );

#endif
