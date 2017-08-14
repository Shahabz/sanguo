#ifndef __EQUIP_WASHRULE_DATA_AUTO_H
#define __EQUIP_WASHRULE_DATA_AUTO_H
#include "define.h"

struct _equipwashrule {
//--Automatically generated
				short color;	//
				short valuenum;	//
				short levellimit;	//
				short superskill;	//
				short token;	//
};
typedef struct _equipwashrule EquipWashRule;

int equipwashrule_init_auto();
int equipwashrule_reload_auto();

#endif
