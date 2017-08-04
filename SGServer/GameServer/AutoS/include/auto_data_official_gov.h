#ifndef __OFFICIAL_GOV_DATA_AUTO_H
#define __OFFICIAL_GOV_DATA_AUTO_H
#include "define.h"

struct _officialgov {
//--Automatically generated
				short kind;	//
				short level;	//
				short color;	//
				int buildinglevel;	//
				int produce;	//
				short haveiron;	//
				int duration;	//
				int silver;	//
				int token;	//
				short free;	//
				short shape;	//
};
typedef struct _officialgov OfficialGov;

int officialgov_init_auto();
int officialgov_reload_auto();

#endif
