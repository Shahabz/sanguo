#ifndef __PAYGOODS_DATA_AUTO_H
#define __PAYGOODS_DATA_AUTO_H
#include "define.h"

struct _paygoods {
//--Automatically generated
				int goodsid;	//
				short tier;	//
				short tier_hy;	//
				int point;	//
				int point_hy;	//
				int token;	//
				int token_hy;	//
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
				char productid_a[64];	//
				char productid_b[64];	//
				char productid_c[64];	//
				char productid_d[64];	//
				char productid_e[64];	//
				char productid_f[64];	//
				char productid_g[64];	//
				char productid_h[64];	//
				char productid_i[64];	//
				char productid_j[64];	//
				char productid_k[64];	//
				char productid_l[64];	//
				char productid_m[64];	//
				char productid_n[64];	//
				char productid_o[64];	//
};
typedef struct _paygoods PayGoods;

int paygoods_init_auto();
int paygoods_reload_auto();

#endif
