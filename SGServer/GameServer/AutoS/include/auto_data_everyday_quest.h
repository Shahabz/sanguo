#ifndef __EVERYDAY_QUEST_DATA_AUTO_H
#define __EVERYDAY_QUEST_DATA_AUTO_H
#include "define.h"

struct _everydayquest {
//--Automatically generated
				int id;	//
				int textid;	//
				int needvalue;	//
				int saveindex;	//
				short levelmin;	//
				short levelmax;	//
				int awardkind[2];	//
				int awardnum[2];	//
				short sort;	//
};
typedef struct _everydayquest EverydayQuest;

int everydayquest_init_auto();
int everydayquest_reload_auto();

#endif
