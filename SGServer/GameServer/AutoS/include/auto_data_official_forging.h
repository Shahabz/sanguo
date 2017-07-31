#ifndef __OFFICIAL_FORGING_DATA_AUTO_H
#define __OFFICIAL_FORGING_DATA_AUTO_H
#include "define.h"

struct _officialforging {
//--Automatically generated
				short kind;	//
				short level;	//
				short color;	//
				int buildinglevel;	//
				int quick;	//
				int duration;	//
				int silver;	//
				int token;	//
};
typedef struct _officialforging OfficialForging;

int officialforging_init_auto();
int officialforging_reload_auto();

#endif
