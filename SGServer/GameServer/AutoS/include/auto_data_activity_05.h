#ifndef __ACTIVITY_05_DATA_AUTO_H
#define __ACTIVITY_05_DATA_AUTO_H
#include "define.h"

struct _activityinfo05 {
//--Automatically generated
				int id;	//
				int awardkind;	//
				int awardnum;	//
				int token;	//
				int weight;	//
};
typedef struct _activityinfo05 ActivityInfo05;

int activityinfo05_init_auto();
int activityinfo05_reload_auto();

#endif
