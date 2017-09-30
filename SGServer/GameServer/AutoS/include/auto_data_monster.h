#ifndef __MONSTER_DATA_AUTO_H
#define __MONSTER_DATA_AUTO_H
#include "define.h"

struct _monsterinfo {
//--Automatically generated
				int monsterid;	//
				short shape;	//
				short level;	//
				short color;	//
				short corps;	//
				short attack;	//
				short defense;	//
				int troops;	//
				short attack_increase;	//
				short defense_increase;	//
				short assault;	//
				short defend;	//
				short line;	//
				short skill;	//
};
typedef struct _monsterinfo MonsterInfo;

int monsterinfo_init_auto();
int monsterinfo_reload_auto();

#endif
