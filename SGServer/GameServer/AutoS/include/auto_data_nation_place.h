#ifndef __NATION_PLACE_DATA_AUTO_H
#define __NATION_PLACE_DATA_AUTO_H
#include "define.h"

struct _nationplace {
//--Automatically generated
				short kind;	//
				int prestige;	//
				int silver;	//
				int cost_kind[4];	//
				int cost_num[4];	//
				short ability;	//
				int value;	//
};
typedef struct _nationplace NationPlace;

int nationplace_init_auto();
int nationplace_reload_auto();

#endif
