#ifndef __TRAINLONG_DATA_AUTO_H
#define __TRAINLONG_DATA_AUTO_H
#include "define.h"

struct _trainlonginfo {
//--Automatically generated
				short level;	//
				int timelong;	//
				int silver;	//
};
typedef struct _trainlonginfo TrainlongInfo;

int trainlonginfo_init_auto();
int trainlonginfo_reload_auto();

#endif
