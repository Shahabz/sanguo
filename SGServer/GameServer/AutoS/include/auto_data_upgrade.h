#ifndef __UPGRADE_DATA_AUTO_H
#define __UPGRADE_DATA_AUTO_H
#include "define.h"

struct _upgradeinfo {
//--Automatically generated
				short level;	//
				int exp;	//
				int genius_point;	//
				int battlepower;	//
				int awardgroup;	//
};
typedef struct _upgradeinfo UpgradeInfo;

int upgradeinfo_init_auto();
int upgradeinfo_reload_auto();
int upgradeinfo_luatable_auto();

#endif
