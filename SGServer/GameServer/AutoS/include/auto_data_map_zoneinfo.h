#ifndef __MAP_ZONEINFO_DATA_AUTO_H
#define __MAP_ZONEINFO_DATA_AUTO_H
#include "define.h"

struct _mapzoneinfo {
//--Automatically generated
				short id;	//
				short open;	//
				short center_posx;	//
				short center_posy;	//
				short top_left_posx;	//
				short top_left_posy;	//
				short top_right_posx;	//
				short top_right_posy;	//
				short bottom_left_posx;	//
				short bottom_left_posy;	//
				short bottom_right_posx;	//
				short bottom_right_posy;	//
				char enemykind[256];	//
				char enemynum[512];	//
				char reskind[256];	//
				char resnum[512];	//
};
typedef struct _mapzoneinfo MapZoneInfo;

int mapzoneinfo_init_auto();
int mapzoneinfo_reload_auto();
int mapzoneinfo_luatable_auto();

#endif
