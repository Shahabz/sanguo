#ifndef __PAYSTORE_DATA_AUTO_H
#define __PAYSTORE_DATA_AUTO_H
#include "define.h"

struct _paystore {
//--Automatically generated
				int id;	//
				short goodsid;	//
				int limitbuy_gifttoken;	//
				short limitbuy_saveindex;	//
				short limitbuy_count;	//
				int everyday_gifttoken;	//
				short everyday_saveindex;	//
				short everyday_count;	//
};
typedef struct _paystore PayStore;

int paystore_init_auto();
int paystore_reload_auto();

#endif
