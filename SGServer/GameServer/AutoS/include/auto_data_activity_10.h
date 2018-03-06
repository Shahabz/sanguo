#ifndef __ACTIVITY_10_DATA_AUTO_H
#define __ACTIVITY_10_DATA_AUTO_H
#include "define.h"

struct _activityinfo10 {
//--Automatically generated
				int id;	//
				short goodsid;	//
				short buynum;	//
				int duration;	//
};
typedef struct _activityinfo10 ActivityInfo10;

int activityinfo10_init_auto();
int activityinfo10_reload_auto();

#endif
