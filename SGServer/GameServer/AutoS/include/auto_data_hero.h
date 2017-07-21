#ifndef __HERO_DATA_AUTO_H
#define __HERO_DATA_AUTO_H
#include "define.h"

struct _heroinfo_config {
//--Automatically generated
				short kind;	//
				short color;	//
				short corps;	//
				short attack_base;	//
				short attack_wash;	//
				short attack_wash_limit;	//
				short defense_base;	//
				short defense_wash;	//
				short defense_wash_limit;	//
				short troops_base;	//
				short troops_wash;	//
				short troops_wash_limit;	//
				short total_wash;	//
				short attack;	//
				short defense;	//
				short troops;	//
};
typedef struct _heroinfo_config HeroInfoConfig;

struct _heroinfo {
//--Automatically generated
	HeroInfoConfig *config;
	int	maxnum;
};
typedef struct _heroinfo HeroInfo;

int heroinfo_init_auto();
int heroinfo_reload_auto();

#endif
