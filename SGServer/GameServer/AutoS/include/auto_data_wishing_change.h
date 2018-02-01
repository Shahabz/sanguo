#ifndef __WISHING_CHANGE_DATA_AUTO_H
#define __WISHING_CHANGE_DATA_AUTO_H
#include "define.h"

struct _wishingchange {
//--Automatically generated
				int id;	//
				int awardkind[2];	//
				int awardnum[2];	//
				int costkind;	//
				int costnum;	//
};
typedef struct _wishingchange WishingChange;

int wishingchange_init_auto();
int wishingchange_reload_auto();

#endif
