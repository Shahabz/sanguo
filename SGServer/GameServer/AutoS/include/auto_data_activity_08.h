#ifndef __ACTIVITY_08_DATA_AUTO_H
#define __ACTIVITY_08_DATA_AUTO_H
#include "define.h"

struct _activityinfo08 {
//--Automatically generated
				int id;	//
				short level;	//
				int awardkind[5];	//
				int awardnum[5];	//
};
typedef struct _activityinfo08 ActivityInfo08;

int activityinfo08_init_auto();
int activityinfo08_reload_auto();

#endif
