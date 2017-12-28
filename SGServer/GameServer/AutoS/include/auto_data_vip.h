#ifndef __VIP_DATA_AUTO_H
#define __VIP_DATA_AUTO_H
#include "define.h"

struct _vipinfo {
//--Automatically generated
				int level;	//
				int exp;	//
				int buildfree;	//
				int bodybuy;	//
				int autobuild;	//
				int skipfight;	//
				int marchcall;	//
				int train;	//
				int palace;	//
				int storysweep;	//
};
typedef struct _vipinfo VipInfo;

int vipinfo_init_auto();
int vipinfo_reload_auto();

#endif
