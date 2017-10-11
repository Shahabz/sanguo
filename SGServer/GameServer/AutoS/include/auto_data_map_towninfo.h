#ifndef __MAP_TOWNINFO_DATA_AUTO_H
#define __MAP_TOWNINFO_DATA_AUTO_H
#include "define.h"

struct _maptowninfo {
//--Automatically generated
				int id;	//
				short type;	//
				short zoneid;	//
				short posx;	//
				short posy;	//
				short grid;	//
				short range;	//
				short level;	//
				char preid[64];	//
				int base_award;	//
				int other_award;	//
				int produce_maxnum;	//
				int produce_maxsec;	//
				int levy_prestige;	//
				int protect_maxsec;	//
				int fight_maxsec;	//
				int own_maxsec;	//
				int ask_silver;	//
				int ask_wood;	//
				int ask_food;	//
				int ask_iron;	//
				char monster[128];	//
				char monster_guard[128];	//
				int fight_silver;	//
				int fight_wood;	//
				int fight_food;	//
};
typedef struct _maptowninfo MapTownInfo;

int maptowninfo_init_auto();
int maptowninfo_reload_auto();

#endif
