#ifndef __WORLD_QUESTINFO_DATA_AUTO_H
#define __WORLD_QUESTINFO_DATA_AUTO_H
#include "define.h"

struct _worldquestinfo {
//--Automatically generated
				int questid;	//
				short type;	//
				short nameid;	//
				int preid;	//
				int value;	//
				int awardkind[5];	//
				int awardnum[5];	//
};
typedef struct _worldquestinfo WorldQuestInfo;

int worldquestinfo_init_auto();
int worldquestinfo_reload_auto();

#endif
