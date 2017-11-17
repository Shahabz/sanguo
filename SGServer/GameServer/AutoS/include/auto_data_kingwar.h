#ifndef __KINGWAR_DATA_AUTO_H
#define __KINGWAR_DATA_AUTO_H
#include "define.h"

struct _kingwarconfig {
//--Automatically generated
				int id;	//
				int exp;	//
				short drawing_num;	//
				int drawing_other;	//
				int rank_award_point;	//
				int totalkill;	//
				int totalkill_award_point;	//
				short treasure_donum;	//
				int treasure_costpoint;	//
				int treasure_costtoken;	//
				int treasure_award;	//
				int treasure_equipodds;	//
				short exchange_item;	//
				int exchange_point;	//
};
typedef struct _kingwarconfig KingWarConfig;

int kingwarconfig_init_auto();
int kingwarconfig_reload_auto();

#endif
