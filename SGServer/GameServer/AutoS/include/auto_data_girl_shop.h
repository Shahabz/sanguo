#ifndef __GIRL_SHOP_DATA_AUTO_H
#define __GIRL_SHOP_DATA_AUTO_H
#include "define.h"

struct _girlshop {
//--Automatically generated
				int id;	//
				int awardkind;	//
				int awardnum;	//
				int cost_awardkind;	//
				int cost_awardnum;	//
				short actorlevel;	//
				short citylevel;	//
				short viplevel;	//
				short girlkind;	//
				int weight;	//
				short row;	//
};
typedef struct _girlshop GirlShop;

int girlshop_init_auto();
int girlshop_reload_auto();

#endif
