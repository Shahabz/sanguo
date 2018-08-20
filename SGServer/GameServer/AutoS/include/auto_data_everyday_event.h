#ifndef __EVERYDAY_EVENT_DATA_AUTO_H
#define __EVERYDAY_EVENT_DATA_AUTO_H
#include "define.h"

struct _everydayevent {
//--Automatically generated
				int id;	//
				int textid;	//
				int talkid[2];	//
				int awardkind[2];	//
				int awardnum[2];	//
};
typedef struct _everydayevent EverydayEvent;

int everydayevent_init_auto();
int everydayevent_reload_auto();

#endif
