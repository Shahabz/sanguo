#ifndef __GIRL_SON_DATA_AUTO_H
#define __GIRL_SON_DATA_AUTO_H
#include "define.h"

struct _girlson_config {
//--Automatically generated
				short herokind;	//
				short index;	//
				short born_odds;	//
				short grow_sec;	//
				short girlkind;	//
				short attack;	//
				short defense;	//
				short attack_increase;	//
				short defense_increase;	//
};
typedef struct _girlson_config GirlSonConfig;

struct _girlson {
//--Automatically generated
	GirlSonConfig *config;
	int	maxnum;
};
typedef struct _girlson GirlSon;

int girlson_init_auto();
int girlson_reload_auto();

#endif
