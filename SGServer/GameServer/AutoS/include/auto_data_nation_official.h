#ifndef __NATION_OFFICIAL_DATA_AUTO_H
#define __NATION_OFFICIAL_DATA_AUTO_H
#include "define.h"

struct _nationofficial {
//--Automatically generated
				short official;	//
				short dismissal;	//
				short fight;	//
				int online_notify;	//
				short notify;	//
				short cityfight_body;	//
				short callnum;	//
				short callactornum;	//
				short yield;	//
				short buildquick;	//
};
typedef struct _nationofficial NationOfficial;

int nationofficial_init_auto();
int nationofficial_reload_auto();

#endif
