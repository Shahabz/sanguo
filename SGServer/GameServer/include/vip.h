#ifndef _VIP_H_
#define _VIP_H_
#include "define.h"

// 计算VIP等级
int vip_calclevel( int city_index );

// VIP经验升级值
int vip_expmax_get( int level );

// 给予VIP经验
int vip_exp( int city_index, int value, short path );

// 立即建造加速
int vip_attr_buildfree( City *pCity );

// 体力购买次数
int vip_attr_bodybuy( City *pCity );

// 自动建造次数
int vip_attr_autobuild( City *pCity );

// 每日免费行军召回次数
int vip_attr_marchcall( City *pCity );

// 武卒官加速募兵
int vip_attr_train( City *pCity );

// VIP礼包
int vipbag_list( int actor_index );
int vipbag_buy( int actor_index, int level );

// VIP特价商店
int vipshop_list( int actor_index );
int vipshop_buy( int actor_index, int id, int awardkind );
int vipshop_set_useitembuy( int actor_index, int close );
#endif
