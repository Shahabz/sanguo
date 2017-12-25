#ifndef __VIPBAG_DATA_AUTO_H
#define __VIPBAG_DATA_AUTO_H
#include "define.h"

struct _vipbag {
//--Automatically generated
				short level;	//
				int token;	//
				int original_token;	//
				int awardkind[6];	//
				int awardnum[6];	//
};
typedef struct _vipbag VipBag;

int vipbag_init_auto();
int vipbag_reload_auto();

#endif
