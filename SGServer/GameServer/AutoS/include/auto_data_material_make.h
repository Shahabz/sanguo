#ifndef __MATERIAL_MAKE_DATA_AUTO_H
#define __MATERIAL_MAKE_DATA_AUTO_H
#include "define.h"

struct _materialmakeinfo {
//--Automatically generated
				short id;	//
				int sec;	//
				int silver;	//
				int wood;	//
				short itemkind[6];	//
				short materialkind[4];	//
};
typedef struct _materialmakeinfo MaterialMakeInfo;

int materialmakeinfo_init_auto();
int materialmakeinfo_reload_auto();

#endif
