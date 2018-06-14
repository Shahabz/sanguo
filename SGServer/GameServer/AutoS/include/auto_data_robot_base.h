#ifndef __ROBOT_BASE_DATA_AUTO_H
#define __ROBOT_BASE_DATA_AUTO_H
#include "define.h"

struct _robotbase {
//--Automatically generated
				short level;	//
				short actorlevel_min;	//
				short actorlevel_max;	//
				int silver_add;	//
				int silver_max;	//
				int wood_add;	//
				int wood_max;	//
				int food_add;	//
				int food_max;	//
				int iron_add;	//
				int iron_max;	//
				short herokind[8];	//
				short heroup_max;	//
				short equipkind[6];	//
};
typedef struct _robotbase RobotBase;

int robotbase_init_auto();
int robotbase_reload_auto();

#endif
