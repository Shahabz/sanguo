#ifndef __PAYGOODS_DATA_AUTO_H
#define __PAYGOODS_DATA_AUTO_H
#include "define.h"

struct _paygoods {
//--Automatically generated
				int goodsid;	//
				int productid;	//
				short tier;	//
				int token;	//
				int awardgroup[5];	//
				short type;	//
				int nameid;	//
				int icon;	//
				float sale;	//
				int be_worth;	//
				short stage;	//
				int descid;	//
				float point;	//
				short nextid;	//
				int nextcd;	//
				short nextcount;	//
				int clubaward;	//
};
typedef struct _paygoods PayGoods;

int paygoods_init_auto();
int paygoods_reload_auto();
int paygoods_luatable_auto();

#endif
