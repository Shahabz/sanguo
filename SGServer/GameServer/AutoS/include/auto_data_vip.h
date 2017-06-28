#ifndef __VIP_DATA_AUTO_H
#define __VIP_DATA_AUTO_H
#include "define.h"

struct _vipinfo {
//--Automatically generated
				int level;	//
				int exp;	//
				int ability[6];	//
				int value[6];	//
				short goodsid;	//
};
typedef struct _vipinfo VipInfo;

int vipinfo_init_auto();
int vipinfo_reload_auto();
int vipinfo_luatable_auto();

#endif
