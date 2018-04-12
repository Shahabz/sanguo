#ifndef __FANGSHI_PALACE_DATA_AUTO_H
#define __FANGSHI_PALACE_DATA_AUTO_H
#include "define.h"

struct _fangshipalace_config {
//--Automatically generated
				short type;	//
				short index;	//
				int awardkind;	//
				int awardnum;	//
				int weight;	//
				int visit_weight;	//
				short cond_herokind;	//
				short cond_girlkind;	//
				short cond_vip;	//
				short cond_actorlevel;	//
				short cond_citylevel;	//
};
typedef struct _fangshipalace_config FangshiPalaceConfig;

struct _fangshipalace {
//--Automatically generated
	FangshiPalaceConfig *config;
	int	maxnum;
};
typedef struct _fangshipalace FangshiPalace;

int fangshipalace_init_auto();
int fangshipalace_reload_auto();

#endif
