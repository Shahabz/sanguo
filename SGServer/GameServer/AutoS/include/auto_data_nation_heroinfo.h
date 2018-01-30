#ifndef __NATION_HEROINFO_DATA_AUTO_H
#define __NATION_HEROINFO_DATA_AUTO_H
#include "define.h"

struct _nationheroinfo {
//--Automatically generated
				short herokind;	//
				short nation;	//
				short nationlevel;	//
				short offset;	//
				short visit_token;	//
				short other_visit_token;	//
				float catch_odds;	//
				float other_catch_odds;	//
				float rob_odds;	//
				float other_rob_odds;	//
				int awardgroup;	//
				short monsterid[4];	//
				int call_silver;	//
				short call_itemkind[4];	//
				short call_itemnum;	//
				short loyal_itemnum;	//
};
typedef struct _nationheroinfo NationHeroInfo;

int nationheroinfo_init_auto();
int nationheroinfo_reload_auto();

#endif
