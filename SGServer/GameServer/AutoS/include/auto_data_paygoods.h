#ifndef __PAYGOODS_DATA_AUTO_H
#define __PAYGOODS_DATA_AUTO_H
#include "define.h"

struct _paygoods {
//--Automatically generated
				int goodsid;	//
				int productid;	//
				short tier;	//
				int point;	//
				int token;	//
				int awardgroup;	//
				short type;	//
				int nameid;	//
				int descid;	//
				int icon;	//
				short sale;	//
				int worth;	//
				short day;	//
				short nextid;	//
				int nextcd;	//
				short nextcount;	//
};
typedef struct _paygoods PayGoods;

int paygoods_init_auto();
int paygoods_reload_auto();

#endif
