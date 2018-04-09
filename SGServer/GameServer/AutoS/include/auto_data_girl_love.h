#ifndef __GIRL_LOVE_DATA_AUTO_H
#define __GIRL_LOVE_DATA_AUTO_H
#include "define.h"

struct _girllove {
//--Automatically generated
				short level;	//
				int exp;	//
				short soncount;	//
				short makelove_exp;	//
				short killenemy_exp;	//
				short citywar_exp;	//
				short nationwar_exp;	//
				short kingwar_exp;	//
				short today_maxexp;	//
};
typedef struct _girllove GirlLove;

int girllove_init_auto();
int girllove_reload_auto();

#endif
