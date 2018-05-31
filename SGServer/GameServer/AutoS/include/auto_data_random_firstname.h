#ifndef __RANDOM_FIRSTNAME_DATA_AUTO_H
#define __RANDOM_FIRSTNAME_DATA_AUTO_H
#include "define.h"

struct _firstname {
//--Automatically generated
				int id;	//
				char str[16];	//
};
typedef struct _firstname FirstName;

int firstname_init_auto();
int firstname_reload_auto();

#endif
