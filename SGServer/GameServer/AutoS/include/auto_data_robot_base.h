#ifndef __ROBOT_BASE_DATA_AUTO_H
#define __ROBOT_BASE_DATA_AUTO_H
#include "define.h"

struct _robotbase {
//--Automatically generated
				short level;	//
				int minute;	//
				short actorexp_add;	//
				short actorlevel_max;	//
				int silver_add;	//
				int silver_max;	//
				int wood_add;	//
				int wood_max;	//
				int food_add;	//
				int food_max;	//
				int iron_add;	//
				int iron_max;	//
				int soldiers_add[3];	//
				int soldiers_max[3];	//
				short herokind[8];	//
				short heroup_max;	//
};
typedef struct _robotbase RobotBase;

int robotbase_init_auto();
int robotbase_reload_auto();

#endif
