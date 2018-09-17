#ifndef __ACTIVITY_17_DATA_AUTO_H
#define __ACTIVITY_17_DATA_AUTO_H
#include "define.h"

struct _activityinfo17 {
//--Automatically generated
				int id;	//
				int pay;	//
				int dollar;	//
				int awardkind[16];	//
				int awardnum[16];	//
};
typedef struct _activityinfo17 ActivityInfo17;

int activityinfo17_init_auto();
int activityinfo17_reload_auto();

#endif
