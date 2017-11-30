#ifndef __BUILDING_RES_UNLOCK_DATA_AUTO_H
#define __BUILDING_RES_UNLOCK_DATA_AUTO_H
#include "define.h"

struct _buildingresunlock_config {
//--Automatically generated
				short kind;	//
				short number;	//
				short itemkind;	//
				short itemnum;	//
};
typedef struct _buildingresunlock_config BuildingResUnlockConfig;

struct _buildingresunlock {
//--Automatically generated
	BuildingResUnlockConfig *config;
	int	maxnum;
};
typedef struct _buildingresunlock BuildingResUnlock;

int buildingresunlock_init_auto();
int buildingresunlock_reload_auto();

#endif
