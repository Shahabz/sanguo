#ifndef __WISHING_PACK_DATA_AUTO_H
#define __WISHING_PACK_DATA_AUTO_H
#include "define.h"

struct _wishingpack {
//--Automatically generated
				int id;	//
				int awardkind;	//
				int awardnum;	//
				int costkind;	//
				int costnum;	//
				int token;	//
};
typedef struct _wishingpack WishingPack;

int wishingpack_init_auto();
int wishingpack_reload_auto();

#endif
