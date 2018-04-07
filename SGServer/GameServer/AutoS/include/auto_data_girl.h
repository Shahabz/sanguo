#ifndef __GIRL_DATA_AUTO_H
#define __GIRL_DATA_AUTO_H
#include "define.h"

struct _girlinfo_config {
//--Automatically generated
				int kind;	//
				short color;	//
				short type;	//
				short attack_increase;	//
				short defense_increase;	//
				short attack_growth;	//
				short defense_growth;	//
				short troops_growth;	//
				short private_herokind;	//
				short private_attack_increase;	//
				short private_defense_increase;	//
				short private_attack_growth;	//
				short private_defense_growth;	//
				short private_troops_growth;	//
				int soul;	//
				int soulchange;	//
				short love_star;	//
				short private_love_star;	//
};
typedef struct _girlinfo_config GirlInfoConfig;

struct _girlinfo {
//--Automatically generated
	GirlInfoConfig *config;
	int	maxnum;
};
typedef struct _girlinfo GirlInfo;

int girlinfo_init_auto();
int girlinfo_reload_auto();

#endif
