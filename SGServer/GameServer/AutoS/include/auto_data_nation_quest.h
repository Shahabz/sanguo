#ifndef __NATION_QUEST_DATA_AUTO_H
#define __NATION_QUEST_DATA_AUTO_H
#include "define.h"

struct _nationquest_config {
//--Automatically generated
				int level;	//
				short kind;	//
				int needvalue;	//
				int awardkind[5];	//
				int awardnum[5];	//
				int other_awardkind[6];	//
				int other_awardnum[6];	//
				int other_awardodds[6];	//
};
typedef struct _nationquest_config NationQuestConfig;

struct _nationquest {
//--Automatically generated
	NationQuestConfig *config;
	int	maxnum;
};
typedef struct _nationquest NationQuest;

int nationquest_init_auto();
int nationquest_reload_auto();

#endif
