#ifndef __HERO_DATA_AUTO_H
#define __HERO_DATA_AUTO_H
#include "define.h"

struct _heroinfo_config {
//--Automatically generated
				short kind;	//
				short color;	//
				short corps;	//
				short attack_bas[2];	//
				short attack_wash;	//
				short attack_wash_limit;	//
				short defense_bas[2];	//
				short defense_wash;	//
				short defense_wash_limit;	//
				short troops_bas[2];	//
				short troops_wash;	//
				short troops_wash_limit;	//
				short total_wash;	//
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
int heroinfo_luatable_auto();

#endif