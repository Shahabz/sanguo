#ifndef _MAP_TOWN_H_
#define _MAP_TOWN_H_
#include "define.h"

int map_town_load();
int map_town_save( FILE *fp );
MapTown *map_town_getptr( int townid );

// 显示单元属性
void map_town_makeunit( int index, SLK_NetS_AddMapUnit *pAttr );

// 位置
void map_town_getpos( int index, short *posx, short *posy );

#endif
