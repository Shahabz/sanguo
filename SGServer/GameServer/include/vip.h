#ifndef _VIP_H_
#define _VIP_H_
#include "define.h"

#define VIP_ABILITY_MAXNUM 6

// 计算VIP等级
int vip_calclevel( int city_index );

// VIP经验升级值
int vip_expmax_get( int level );

// 给予VIP经验
int vip_exp( int city_index, int value, short path );
#endif
