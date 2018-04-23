#ifndef __ROBOT_AI_DATA_AUTO_H
#define __ROBOT_AI_DATA_AUTO_H
#include "define.h"

struct _robotai {
//--Automatically generated
				int id;	//
				float actorexp_add;	//
				float silver_init;	//
				float silver_add;	//
				float silver_max;	//
				float wood_init;	//
				float wood_add;	//
				float wood_max;	//
				float food_init;	//
				float food_add;	//
				float food_max;	//
				float iron_init;	//
				float iron_add;	//
				float iron_max;	//
				float soldiers_add[3];	//
				float soldiers_max[3];	//
				short herokind[4];	//
				short equiplevel_min;	//
				short equiplevel_max;	//
};
typedef struct _robotai RobotAi;

int robotai_init_auto();
int robotai_reload_auto();

#endif
