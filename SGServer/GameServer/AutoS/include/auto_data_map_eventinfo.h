#ifndef __MAP_EVENTINFO_DATA_AUTO_H
#define __MAP_EVENTINFO_DATA_AUTO_H
#include "define.h"

struct _mapeventinfo {
//--Automatically generated
				short kind;	//
				short type;	//
				int waitsec;	//
				short costtype;	//
				int costnum;	//
				int awardkind;	//
				int awardnum;	//
};
typedef struct _mapeventinfo MapEventInfo;

int mapeventinfo_init_auto();
int mapeventinfo_reload_auto();

#endif
