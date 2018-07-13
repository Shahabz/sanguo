#ifndef __PLATINFO_DATA_AUTO_H
#define __PLATINFO_DATA_AUTO_H
#include "define.h"

struct _platinfo {
//--Automatically generated
				int platid;	//
				char userhost[64];	//
				short userport;	//
				char loginpath[64];	//
				char cdkeypath[64];	//
				char paymode_sdk[2];	//
				char paymode_web[2];	//
};
typedef struct _platinfo PlatInfo;

int platinfo_init_auto();
int platinfo_reload_auto();
int platinfo_luatable_auto();

#endif
