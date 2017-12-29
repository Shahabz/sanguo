#ifndef __SHOP_DATA_AUTO_H
#define __SHOP_DATA_AUTO_H
#include "define.h"

struct _shop_config {
//--Automatically generated
				short type;	//
				short index;	//
				int awardkind;	//
				int awardnum;	//
				int token;	//
				int original_token;	//
				short actorlevel_min;	//
				short actorlevel_max;	//
				short sale;	//
				short buyuse;	//
};
typedef struct _shop_config ShopConfig;

struct _shop {
//--Automatically generated
	ShopConfig *config;
	int	maxnum;
};
typedef struct _shop Shop;

int shop_init_auto();
int shop_reload_auto();

#endif
