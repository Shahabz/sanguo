#ifndef _WISHING_H_
#define _WISHING_H_
#include "define.h"

#define WISHINGSHOP_ITEM_MAX	9

int wishing_shop_update( int actor_index );
int wishing_shop_sendinfo( int actor_index );
int wishing_shop_open( int actor_index, int id );
int wishing_shop_buy( int actor_index, int id );
#endif
