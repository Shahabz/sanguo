#ifndef __TIANCE_QUEST_DATA_AUTO_H
#define __TIANCE_QUEST_DATA_AUTO_H
#include "define.h"

struct _tiancequest {
//--Automatically generated
				int kind;	//
				short prekind;	//
				short nextkind;	//
				short level;	//
				short tiance_level;	//
				short actor_level;	//
				short brush_enemykind;	//
				short brush_enemynum;	//
				short ability;	//
				int value;	//
				short showvalue;	//
};
typedef struct _tiancequest TianceQuest;

int tiancequest_init_auto();
int tiancequest_reload_auto();

#endif
