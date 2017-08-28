#ifndef __FIGHT_DATA_AUTO_H
#define __FIGHT_DATA_AUTO_H
#include "define.h"

struct _fightinfo {
//--Automatically generated
				int id;	//
				int scene;	//
				int monsterid[8];	//
};
typedef struct _fightinfo FightInfo;

int fightinfo_init_auto();
int fightinfo_reload_auto();

#endif
