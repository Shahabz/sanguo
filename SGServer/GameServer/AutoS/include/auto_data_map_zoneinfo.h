#ifndef __MAP_ZONEINFO_DATA_AUTO_H
#define __MAP_ZONEINFO_DATA_AUTO_H
#include "define.h"

struct _mapzoneinfo {
//--Automatically generated
				short id;	//
				short open;	//
};
typedef struct _mapzoneinfo MapZoneInfo;

int mapzoneinfo_init_auto();
int mapzoneinfo_reload_auto();

#endif
