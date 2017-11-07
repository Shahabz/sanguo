#ifndef __KINGWAR_TOWNINFO_DATA_AUTO_H
#define __KINGWAR_TOWNINFO_DATA_AUTO_H
#include "define.h"

struct _kingwartowninfo {
//--Automatically generated
				int id;	//
				short type;	//
				short base_nation;	//
				short doorid;	//
};
typedef struct _kingwartowninfo KingwarTownInfo;

int kingwartowninfo_init_auto();
int kingwartowninfo_reload_auto();

#endif
