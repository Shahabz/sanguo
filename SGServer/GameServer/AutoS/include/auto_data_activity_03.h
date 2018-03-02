#ifndef __ACTIVITY_03_DATA_AUTO_H
#define __ACTIVITY_03_DATA_AUTO_H
#include "define.h"

struct _activityinfo03 {
//--Automatically generated
				int id;	//
				int nameid;	//
				short record_offset;	//
				short needvalue;	//
				int awardkind[5];	//
				int awardnum[5];	//
};
typedef struct _activityinfo03 ActivityInfo03;

int activityinfo03_init_auto();
int activityinfo03_reload_auto();

#endif
