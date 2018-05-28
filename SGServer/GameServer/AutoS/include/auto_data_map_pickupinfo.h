#ifndef __MAP_PICKUPINFO_DATA_AUTO_H
#define __MAP_PICKUPINFO_DATA_AUTO_H
#include "define.h"

struct _mappickupinfo {
//--Automatically generated
				short kind;	//
				int awardkind;	//
				short numtype;	//
				int minnum;	//
				int maxnum;	//
};
typedef struct _mappickupinfo MapPickupInfo;

int mappickupinfo_init_auto();
int mappickupinfo_reload_auto();

#endif
