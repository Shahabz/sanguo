#ifndef __NATION_EQUIP_DATA_AUTO_H
#define __NATION_EQUIP_DATA_AUTO_H
#include "define.h"

struct _nationequipinfo_config {
//--Automatically generated
				short kind;	//
				short level;	//
				short actorlevel;	//
				short itemkind;	//
				int itemnum;	//
				short progress;	//
				int iron;	//
				int sec;	//
				short ability;	//
				int value;	//
				int battlepower;	//
};
typedef struct _nationequipinfo_config NationEquipInfoConfig;

struct _nationequipinfo {
//--Automatically generated
	NationEquipInfoConfig *config;
	int	maxnum;
};
typedef struct _nationequipinfo NationEquipInfo;

int nationequipinfo_init_auto();
int nationequipinfo_reload_auto();

#endif
