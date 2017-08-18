#ifndef __HERO_SKILL_DATA_AUTO_H
#define __HERO_SKILL_DATA_AUTO_H
#include "define.h"

struct _heroskillinfo {
//--Automatically generated
				short skillid;	//
				char name[255];	//
				int value;	//
				int randmin;	//
				int randmax;	//
};
typedef struct _heroskillinfo HeroSkillInfo;

int heroskillinfo_init_auto();
int heroskillinfo_reload_auto();

#endif
