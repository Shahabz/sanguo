#ifndef __ACTIVITY_02_DATA_AUTO_H
#define __ACTIVITY_02_DATA_AUTO_H
#include "define.h"

struct _activityinfo02 {
//--Automatically generated
				int id;	//
				short level;	//
				int unlock_nameid;	//
				int unlock_descid;	//
				short cond_kind;	//
				short cond_level;	//
				short actor_level;	//
				int awardkind[3];	//
				int awardnum[3];	//
				short sprite_buildingkind;	//
				short sprite_techkind;	//
				short sprite_color;	//
};
typedef struct _activityinfo02 ActivityInfo02;

int activityinfo02_init_auto();
int activityinfo02_reload_auto();

#endif
