#ifndef _MAP_RES_H_
#define _MAP_RES_H_
#include "define.h"


// 显示单元属性
void map_res_makeunit( int index, SLK_NetS_AddMapUnit *pAttr );

// 位置
void map_res_getpos( int index, short *posx, short *posy );

#endif
