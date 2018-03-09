#ifndef __ACTIVITY_04_DATA_AUTO_H
#define __ACTIVITY_04_DATA_AUTO_H
#include "define.h"

struct _activityinfo04 {
//--Automatically generated
				int id;	//
				int day;	//
				short sortrank;	//
				short title;	//
				short type;	//
				int value;	//
				int awardkind[3];	//
				int awardnum[3];	//
};
typedef struct _activityinfo04 ActivityInfo04;

int activityinfo04_init_auto();
int activityinfo04_reload_auto();

#endif
