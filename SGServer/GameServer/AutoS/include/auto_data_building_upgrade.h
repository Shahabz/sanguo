#ifndef __BUILDING_UPGRADE_DATA_AUTO_H
#define __BUILDING_UPGRADE_DATA_AUTO_H
#include "define.h"

struct _buildingupgrade_config {
//--Automatically generated
				int kind;	//
				short level;	//
				short citylevel;	//
				short actorlevel;	//
				int silver;	//
				int wood;	//
				int food;	//
				int iron;	//
				int sec;	//
				int value[8];	//
				int exp;	//
				int battlepower;	//
};
typedef struct _buildingupgrade_config BuildingUpgradeConfig;

struct _buildingupgrade {
//--Automatically generated
	BuildingUpgradeConfig *config;
	int	maxnum;
};
typedef struct _buildingupgrade BuildingUpgrade;

int buildingupgrade_init_auto();
int buildingupgrade_reload_auto();

#endif
