#ifndef __FANGSHI_NODE_DATA_AUTO_H
#define __FANGSHI_NODE_DATA_AUTO_H
#include "define.h"

struct _fangshinode {
//--Automatically generated
				short id;	//
				int awardgroup;	//
				short nextid[3];	//
				short nextodds[3];	//
				short preid;	//
};
typedef struct _fangshinode FangshiNode;

int fangshinode_init_auto();
int fangshinode_reload_auto();

#endif
