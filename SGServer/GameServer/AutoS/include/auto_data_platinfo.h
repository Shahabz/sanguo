#ifndef __PLATINFO_DATA_AUTO_H
#define __PLATINFO_DATA_AUTO_H
#include "define.h"

struct _platinfo {
//--Automatically generated
				int platid;	//
				char userhost[64];	//
				short userport;	//
				short allow;	//
};
typedef struct _platinfo PlatInfo;

int platinfo_init_auto();
int platinfo_reload_auto();
int platinfo_luatable_auto();

#endif
