#ifndef __TRAINQUEUE_DATA_AUTO_H
#define __TRAINQUEUE_DATA_AUTO_H
#include "define.h"

struct _trainqueueinfo {
//--Automatically generated
				short queue;	//
				int token;	//
};
typedef struct _trainqueueinfo TrainqueueInfo;

int trainqueueinfo_init_auto();
int trainqueueinfo_reload_auto();

#endif
