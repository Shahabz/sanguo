#ifndef __UPGRADE_DATA_AUTO_H
#define __UPGRADE_DATA_AUTO_H
#include "define.h"

struct _upgradeinfo {
//--Automatically generated
				short level;	//
				int exp;	//
				int heroexp;	//
				int battlepower;	//
				int awardgroup;	//
};
typedef struct _upgradeinfo UpgradeInfo;

int upgradeinfo_init_auto();
int upgradeinfo_reload_auto();

#endif
