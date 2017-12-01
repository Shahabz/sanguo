#ifndef __HERO_VISIT_DATA_AUTO_H
#define __HERO_VISIT_DATA_AUTO_H
#include "define.h"

struct _herovisit {
//--Automatically generated
				int id;	//
				int awardgroup;	//
				int kind;	//
				int num;	//
				short color;	//
				int value;	//
};
typedef struct _herovisit HeroVisit;

int herovisit_init_auto();
int herovisit_reload_auto();

#endif
