#ifndef __EQUIP_WASH_DATA_AUTO_H
#define __EQUIP_WASH_DATA_AUTO_H
#include "define.h"

struct _equipwashinfo {
//--Automatically generated
				short washid;	//
				short level;	//
				short ability;	//
				int value;	//
				int free_odds;	//
				int token_odds;	//
};
typedef struct _equipwashinfo EquipWashInfo;

int equipwashinfo_init_auto();
int equipwashinfo_reload_auto();

#endif
