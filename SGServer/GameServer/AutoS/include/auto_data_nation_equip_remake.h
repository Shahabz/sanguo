#ifndef __NATION_EQUIP_REMAKE_DATA_AUTO_H
#define __NATION_EQUIP_REMAKE_DATA_AUTO_H
#include "define.h"

struct _nationequipremake_config {
//--Automatically generated
				short kind;	//
				short progress;	//
				int silver;	//
				int sec;	//
				short maxlevel;	//
};
typedef struct _nationequipremake_config NationEquipRemakeConfig;

struct _nationequipremake {
//--Automatically generated
	NationEquipRemakeConfig *config;
	int	maxnum;
};
typedef struct _nationequipremake NationEquipRemake;

int nationequipremake_init_auto();
int nationequipremake_reload_auto();

#endif
