#ifndef _GIRL_H_
#define _GIRL_H_
#include "define.h"
#define ACTOR_GIRL_MAX	80

#define GIRL_SFLAG_MARRY	0	// 是否结婚
#define GIRL_SFLAG_MAKELOVE	1	// 今天是否亲密互动过

// 将offset转换成对应的属性结构
Girl *girl_getptr( int city_index, int kind );
GirlInfoConfig *girl_getconfig( int kind, int color );
void girl_makestruct( City *pCity, Girl *pGirl, SLK_NetS_Girl *pValue );
int girl_getgirl( City *pCity, int kind, char path ); // 玩家获得女将
int girl_getsoul( City *pCity, int kind, int soul, char path ); // 玩家获得女将碎片
int girl_info( City *pCity, Girl *pGirl ); // 女将信息
int girl_list( int actor_index ); // 女将列表
void girl_update(); //  每天零点重置
int girl_allot( int actor_index, short herokind, short girlkind ); // 委派男将
int girl_unallot( int actor_index, short herokind ); // 解除委派
void girl_gm_getall( City *pCity );
#endif