#ifndef __VIPSHOP_DATA_AUTO_H
#define __VIPSHOP_DATA_AUTO_H
#include "define.h"

struct _vipshop {
//--Automatically generated
				short id;	//
				int awardkind;	//
				int awardnum;	//
				int token;	//
				short itemkind;	//
				short actorlevel;	//
				short viplevel;	//
				short viplevel_max;	//
				short vipbaglevel;	//
				short fun;	//
				short vip_token[16];	//
				short vip_buynum[16];	//
};
typedef struct _vipshop VipShop;

int vipshop_init_auto();
int vipshop_reload_auto();

#endif
