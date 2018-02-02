#ifndef _WISHING_H_
#define _WISHING_H_
#include "define.h"

#define WISHINGSHOP_ITEM_MAX	9
#define WISHINGCHANGE_V			10000

int wishing_shop_update( int actor_index );
int wishing_shop_sendinfo( int actor_index );
int wishing_shop_open( int actor_index, int id );
int wishing_shop_buy( int actor_index, int id );

int wishing_change_sendinfo( int actor_index );
int wishing_change_buy( int actor_index, int kind, int to_kind );

int wishing_pack_sendinfo( int actor_index );
int wishing_pack_buy( int actor_index, int id );
#endif
