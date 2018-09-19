#ifndef __EVERYDAY_SIGNIN_DATA_AUTO_H
#define __EVERYDAY_SIGNIN_DATA_AUTO_H
#include "define.h"

struct _everydaysignin {
//--Automatically generated
				int id;	//
				int awardkind[2];	//
				int awardnum[2];	//
				int token;	//
};
typedef struct _everydaysignin everydaySignin;

int everydaysignin_init_auto();
int everydaysignin_reload_auto();

#endif
