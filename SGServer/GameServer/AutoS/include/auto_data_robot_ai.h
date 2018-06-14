#ifndef __ROBOT_AI_DATA_AUTO_H
#define __ROBOT_AI_DATA_AUTO_H
#include "define.h"

struct _robotai {
//--Automatically generated
				int id;	//
				float actorexp_add;	//
				int silver_init;	//
				float silver_add;	//
				float silver_max;	//
				int wood_init;	//
				float wood_add;	//
				float wood_max;	//
				int food_init;	//
				float food_add;	//
				float food_max;	//
				int iron_init;	//
				float iron_add;	//
				float iron_max;	//
				short herokind[4];	//
};
typedef struct _robotai RobotAi;

int robotai_init_auto();
int robotai_reload_auto();

#endif
