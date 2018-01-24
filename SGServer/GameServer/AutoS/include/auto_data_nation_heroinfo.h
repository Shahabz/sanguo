#ifndef __NATION_HEROINFO_DATA_AUTO_H
#define __NATION_HEROINFO_DATA_AUTO_H
#include "define.h"

struct _nationheroinfo {
//--Automatically generated
				short herokind;	//
				short nation;	//
				short nationlevel;	//
				short visit_token;	//
				short other_visit_token;	//
				short catch_odds;	//
				short catch_visit_odds;	//
				short run_loyal[10];	//
				short run_odds[10];	//
				int awardgroup;	//
				short monsterid[4];	//
};
typedef struct _nationheroinfo NationHeroInfo;

int nationheroinfo_init_auto();
int nationheroinfo_reload_auto();

#endif
