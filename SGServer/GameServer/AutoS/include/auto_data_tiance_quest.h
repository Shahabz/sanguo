#ifndef __TIANCE_QUEST_DATA_AUTO_H
#define __TIANCE_QUEST_DATA_AUTO_H
#include "define.h"

struct _tiancequest_config {
//--Automatically generated
				int kind;	//
				short level;	//
				short actor_level;	//
				short ability;	//
				int value;	//
				short showvalue;	//
				int silver;	//
				int wood;	//
				int food;	//
				int iron;	//
				int progress;	//
};
typedef struct _tiancequest_config TianceQuestConfig;

struct _tiancequest {
//--Automatically generated
	TianceQuestConfig *config;
	int	maxnum;
};
typedef struct _tiancequest TianceQuest;

int tiancequest_init_auto();
int tiancequest_reload_auto();

#endif
