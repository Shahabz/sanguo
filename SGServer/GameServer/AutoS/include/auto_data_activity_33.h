#ifndef __ACTIVITY_33_DATA_AUTO_H
#define __ACTIVITY_33_DATA_AUTO_H
#include "define.h"

struct _activityinfo33 {
//--Automatically generated
				int id;	//
				int awardkind[5];	//
				int awardnum[5];	//
};
typedef struct _activityinfo33 ActivityInfo33;

int activityinfo33_init_auto();
int activityinfo33_reload_auto();

#endif
