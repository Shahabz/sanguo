#ifndef __ACTIVITY_06_DATA_AUTO_H
#define __ACTIVITY_06_DATA_AUTO_H
#include "define.h"

struct _activityinfo06 {
//--Automatically generated
				int id;	//
				int token;	//
				int awardkind[4];	//
				int awardnum[4];	//
};
typedef struct _activityinfo06 ActivityInfo06;

int activityinfo06_init_auto();
int activityinfo06_reload_auto();

#endif
