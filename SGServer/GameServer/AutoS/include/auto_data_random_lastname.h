#ifndef __RANDOM_LASTNAME_DATA_AUTO_H
#define __RANDOM_LASTNAME_DATA_AUTO_H
#include "define.h"

struct _lastname {
//--Automatically generated
				int id;	//
				char str[16];	//
};
typedef struct _lastname LastName;

int lastname_init_auto();
int lastname_reload_auto();

#endif
