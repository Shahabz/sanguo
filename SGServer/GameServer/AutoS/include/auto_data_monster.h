#ifndef __MONSTER_DATA_AUTO_H
#define __MONSTER_DATA_AUTO_H
#include "define.h"

struct _monsterinfo_config {
//--Automatically generated
				int monsterid;	//
				short type;	//
				short color;	//
				short corps;	//
				short attack;	//
				short defens[2];	//
				short troops;	//
				short attack_growth;	//
				short defense_growth;	//
				short troops_growth;	//
				short attack_increase;	//
				short defense_increase;	//
				short assault;	//
				short line;	//
				short skill;	//
};
typedef struct _monsterinfo_config MonsterInfoConfig;

struct _monsterinfo {
//--Automatically generated
	MonsterInfoConfig *config;
	int	maxnum;
};
typedef struct _monsterinfo MonsterInfo;

int monsterinfo_init_auto();
int monsterinfo_reload_auto();
int monsterinfo_luatable_auto();

#endif
