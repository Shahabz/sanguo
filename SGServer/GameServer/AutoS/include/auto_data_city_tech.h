#ifndef __CITY_TECH_DATA_AUTO_H
#define __CITY_TECH_DATA_AUTO_H
#include "define.h"

struct _techinfo_config {
//--Automatically generated
				short kind;	//
				short level;	//
				short buildinglevel;	//
				short prekind;	//
				short prelevel;	//
				short progress;	//
				int sec;	//
				int silver;	//
				int wood;	//
				int food;	//
				int iron;	//
				short ability;	//
				int value;	//
};
typedef struct _techinfo_config TechInfoConfig;

struct _techinfo {
//--Automatically generated
	TechInfoConfig *config;
	int	maxnum;
};
typedef struct _techinfo TechInfo;

int techinfo_init_auto();
int techinfo_reload_auto();
int techinfo_luatable_auto();

#endif
