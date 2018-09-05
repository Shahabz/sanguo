#ifndef __ACTIVITY_12_DATA_AUTO_H
#define __ACTIVITY_12_DATA_AUTO_H
#include "define.h"

struct _activityinfo12 {
//--Automatically generated
				int id;	//
				short level;	//
				int marchtime;	//
				short attack;	//
				short defense;	//
				int troops;	//
				int awardkind[4];	//
				int awardnum[4];	//
};
typedef struct _activityinfo12 ActivityInfo12;

int activityinfo12_init_auto();
int activityinfo12_reload_auto();

#endif
