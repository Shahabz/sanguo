#ifndef _GIRL_H_
#define _GIRL_H_
#include "define.h"
#define ACTOR_GIRL_MAX	80

#define GIRL_SFLAG_MARRY	0	// 是否结婚
#define GIRL_SFLAG_MAKELOVE	1	// 今天是否亲密互动过

#define FANGSHI_AWARDNUM	14

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
int girl_unallot( int actor_index, short kind ); // 解除委派
int girl_colorup( int actor_index, short kind ); // 突破
int girl_loveitemuse( int actor_index, short kind, short itemkind );// 亲密度道具使用
int girl_marry( int actor_index, short kind ); // 喜结连理
int girl_makelove( int actor_index, short kind ); // 亲密互动
float girl_loveexp_speed( short herokind, short kind, char color );
int girl_addloveexp_killenemy( City *pCity, short herokind, short kind );
int girl_addloveexp_citywar( City *pCity, short herokind, short kind );
int girl_addloveexp_nationwar( City *pCity, short herokind, short kind );
int girl_addloveexp_kingwar( City *pCity, short herokind, short kind );
int girl_addloveexp( City *pCity, Girl *pGirl, int exp, short path ); // 添加亲昵度
int girl_son_growth( int actor_index, short kind ); // 子女出师
void girl_gm_getall( City *pCity );

int fangshi_sendinfo( int actor_index );
int fangshi_visit( int actor_index, int isfree );
int fangshi_visit_getaward( int actor_index );
#endif
