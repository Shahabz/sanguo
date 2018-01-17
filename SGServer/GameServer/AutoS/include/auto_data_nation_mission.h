#ifndef __NATION_MISSION_DATA_AUTO_H
#define __NATION_MISSION_DATA_AUTO_H
#include "define.h"

struct _nationmission_config {
//--Automatically generated
				int level;	//
				short kind;	//
				int needvalue[3];	//
				int awardkind[2];	//
				int awardnum[2];	//
};
typedef struct _nationmission_config NationMissionConfig;

struct _nationmission {
//--Automatically generated
	NationMissionConfig *config;
	int	maxnum;
};
typedef struct _nationmission NationMission;

int nationmission_init_auto();
int nationmission_reload_auto();

#endif
