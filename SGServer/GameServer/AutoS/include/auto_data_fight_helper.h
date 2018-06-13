#ifndef __FIGHT_HELPER_DATA_AUTO_H
#define __FIGHT_HELPER_DATA_AUTO_H
#include "define.h"

struct _fighthelper {
//--Automatically generated
				short kind;	//
				int monsterid;	//
				int token;	//
				short sort;	//
};
typedef struct _fighthelper FightHelper;

int fighthelper_init_auto();
int fighthelper_reload_auto();

#endif
