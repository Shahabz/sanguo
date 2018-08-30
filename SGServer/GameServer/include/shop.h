#ifndef _SHOP_H_
#define _SHOP_H_
#include "define.h"

#define SHOPTYPE_FIGHT	1	// 军事
#define SHOPTYPE_OTHER	2	// 其他
#define SHOPTYPE_EVERYDAY	3	// 活跃度商店（不用函数，只用数据）
int shop_list( int actor_index, int type );
int shop_checksale( int actor_index, int awardkind );
int shop_buy( int actor_index, int type, int offset, int awardkind, int count, int buyuse );
#endif
