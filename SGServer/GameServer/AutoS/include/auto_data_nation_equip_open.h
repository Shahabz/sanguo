#ifndef __NATION_EQUIP_OPEN_DATA_AUTO_H
#define __NATION_EQUIP_OPEN_DATA_AUTO_H
#include "define.h"

struct _nationequipopen {
//--Automatically generated
				short kind;	//
				short actorlevel;	//
				short itemkind;	//
				int itemnum;	//
				int silver;	//
				int sec;	//
};
typedef struct _nationequipopen NationEquipOpen;

int nationequipopen_init_auto();
int nationequipopen_reload_auto();

#endif
