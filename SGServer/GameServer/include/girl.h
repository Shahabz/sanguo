#ifndef _GIRL_H_
#define _GIRL_H_
#include "define.h"
#define ACTOR_GIRL_MAX	80

// 将offset转换成对应的属性结构
Girl *girl_getptr( int city_index, int kind );
GirlInfoConfig *girl_getconfig( int kind, int color );
void girl_makestruct( City *pCity, Girl *pGirl, SLK_NetS_Girl *pValue );
int girl_getgirl( City *pCity, int kind, char path ); // 玩家获得女将
int girl_getsoul( City *pCity, int kind, int soul, char path ); // 玩家获得女将碎片
int girl_info( City *pCity, Girl *pGirl ); // 女将信息
int girl_list( int actor_index ); // 女将列表
void girl_gm_getall( City *pCity );
#endif