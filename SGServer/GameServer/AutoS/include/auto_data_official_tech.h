#ifndef __OFFICIAL_TECH_DATA_AUTO_H
#define __OFFICIAL_TECH_DATA_AUTO_H
#include "define.h"

struct _officialtech {
//--Automatically generated
				short kind;	//
				short level;	//
				short color;	//
				int buildinglevel;	//
				int quick;	//
				int duration;	//
				int silver;	//
				int token;	//
				short vip;	//
				short free;	//
				short shape;	//
};
typedef struct _officialtech OfficialTech;

int officialtech_init_auto();
int officialtech_reload_auto();

#endif
