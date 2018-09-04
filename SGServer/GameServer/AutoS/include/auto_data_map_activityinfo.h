#ifndef __MAP_ACTIVITYINFO_DATA_AUTO_H
#define __MAP_ACTIVITYINFO_DATA_AUTO_H
#include "define.h"

struct _mapactivityinfo {
//--Automatically generated
				short kind;	//
				short type;	//
				short level;	//
				short shape;	//
				int maxhp;	//
				short activityid;	//
				int awardgroup;	//
				short monsterid[4];	//
};
typedef struct _mapactivityinfo MapActivityInfo;

int mapactivityinfo_init_auto();
int mapactivityinfo_reload_auto();

#endif
