#ifndef __EQUIP_DATA_AUTO_H
#define __EQUIP_DATA_AUTO_H
#include "define.h"

struct _equipinfo {
//--Automatically generated
				short kind;	//
				short type;	//
				short actorlevel;	//
				short color;	//
				short ability;	//
				int value;	//
				int sec;	//
				int silver;	//
				short material_kind[6];	//
				short material_num[6];	//
				int prestige;	//
};
typedef struct _equipinfo EquipInfo;

int equipinfo_init_auto();
int equipinfo_reload_auto();
int equipinfo_luatable_auto();

#endif
