#ifndef __CITY_GUARD_DATA_AUTO_H
#define __CITY_GUARD_DATA_AUTO_H
#include "define.h"

struct _cityguardinfo_config {
//--Automatically generated
				int monsterid;	//
				short color;	//
				short attack;	//
				short defense;	//
				short troops;	//
				short attack_growth;	//
				short defense_growth;	//
				short troops_growth;	//
				short attack_increase;	//
				short defense_increase;	//
				short assault;	//
				short defend;	//
				short line;	//
				short skill;	//
};
typedef struct _cityguardinfo_config CityGuardInfoConfig;

struct _cityguardinfo {
//--Automatically generated
	CityGuardInfoConfig *config;
	int	maxnum;
};
typedef struct _cityguardinfo CityGuardInfo;

int cityguardinfo_init_auto();
int cityguardinfo_reload_auto();

#endif
