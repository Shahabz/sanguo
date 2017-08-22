#ifndef __MAP_RESINFO_DATA_AUTO_H
#define __MAP_RESINFO_DATA_AUTO_H
#include "define.h"

struct _mapresinfo {
//--Automatically generated
				short kind;	//
				short level;	//
};
typedef struct _mapresinfo MapResInfo;

int mapresinfo_init_auto();
int mapresinfo_reload_auto();

#endif
