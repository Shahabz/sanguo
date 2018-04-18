#ifndef __GIRL_SHOP_UPDATE_DATA_AUTO_H
#define __GIRL_SHOP_UPDATE_DATA_AUTO_H
#include "define.h"

struct _girlshopupdate {
//--Automatically generated
				short count;	//
				int token;	//
				short viplevel;	//
};
typedef struct _girlshopupdate GirlShopUpdate;

int girlshopupdate_init_auto();
int girlshopupdate_reload_auto();

#endif
