#ifndef __WISHING_SHOP_DATA_AUTO_H
#define __WISHING_SHOP_DATA_AUTO_H
#include "define.h"

struct _wishingshop {
//--Automatically generated
				int id;	//
				short color;	//
				short level;	//
				int awardkind;	//
				int awardnum;	//
				int costkind;	//
				int costnum;	//
				int value;	//
};
typedef struct _wishingshop WishingShop;

int wishingshop_init_auto();
int wishingshop_reload_auto();

#endif
