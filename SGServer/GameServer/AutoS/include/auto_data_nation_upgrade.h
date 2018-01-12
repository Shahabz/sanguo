#ifndef __NATION_UPGRADE_DATA_AUTO_H
#define __NATION_UPGRADE_DATA_AUTO_H
#include "define.h"

struct _nationupgrade_config {
//--Automatically generated
				int level;	//
				short num;	//
				int silver;	//
				int wood;	//
				int food;	//
				int exp;	//
				int maxexp;	//
				int prestige;	//
};
typedef struct _nationupgrade_config NationUpgradeConfig;

struct _nationupgrade {
//--Automatically generated
	NationUpgradeConfig *config;
	int	maxnum;
};
typedef struct _nationupgrade NationUpgrade;

int nationupgrade_init_auto();
int nationupgrade_reload_auto();

#endif
