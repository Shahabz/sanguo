#ifndef __HERO_CORPSMUL_DATA_AUTO_H
#define __HERO_CORPSMUL_DATA_AUTO_H
#include "define.h"

struct _herocorpsmulinfo {
//--Automatically generated
				short corps;	//
				int target_corps[3];	//
};
typedef struct _herocorpsmulinfo HeroCorpsmulInfo;

int herocorpsmulinfo_init_auto();
int herocorpsmulinfo_reload_auto();

#endif
