#ifndef __BODYTOKEN_DATA_AUTO_H
#define __BODYTOKEN_DATA_AUTO_H
#include "define.h"

struct _bodytokeninfo {
//--Automatically generated
				short count;	//
				int token;	//
};
typedef struct _bodytokeninfo BodyTokenInfo;

int bodytokeninfo_init_auto();
int bodytokeninfo_reload_auto();

#endif
