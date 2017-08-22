#ifndef __MAP_TOWNINFO_DATA_AUTO_H
#define __MAP_TOWNINFO_DATA_AUTO_H
#include "define.h"

struct _maptowninfo {
//--Automatically generated
				int id;	//
				short type;	//
				short zoneid;	//
				short posx;	//
				short posy;	//
				short grid;	//
				short range;	//
};
typedef struct _maptowninfo MapTownInfo;

int maptowninfo_init_auto();
int maptowninfo_reload_auto();

#endif
