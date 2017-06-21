#ifndef _EQUIP_H_
#define _EQUIP_H_
#include "define.h"

// 将offset转换成对应的属性结构
Equip *actor_equip_getptr( int actor_index, int offset );
Equip *city_equip_getptr( int city_index, int offset );
int equip_load( int actor_index );
int equip_save( int actor_index, FILE *fp );
#endif
