#ifndef _ARMY_H_
#define _ARMY_H_
#include "define.h"

// 显示单元属性
void army_makeunit( int army_index, SLK_NetS_AddMapUnit *pAttr );

// 部队位置
void army_getpos( int army_index, short *posx, short *posy );

#endif
