#ifndef __WORLD_BOSSINFO_DATA_AUTO_H
#define __WORLD_BOSSINFO_DATA_AUTO_H
#include "define.h"

struct _worldbossinfo {
//--Automatically generated
				int bossid;	//
				char monster[128];	//
				int hp;	//
				int exp;	//
				short actorlevel;	//
				int awardgroup;	//
				int kill_awardgroup;	//
};
typedef struct _worldbossinfo WorldBossInfo;

int worldbossinfo_init_auto();
int worldbossinfo_reload_auto();

#endif
