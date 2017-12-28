#ifndef _SHOP_H_
#define _SHOP_H_
#include "define.h"

#define SHOPTYPE_SALE	1	// 特价
#define SHOPTYPE_FIGHT	2	// 军事
#define SHOPTYPE_OTHER	3	// 其他

int shop_list( int actor_index, int type );
int shop_buy( int actor_index, int type, int offset, int awardkind );
#endif
