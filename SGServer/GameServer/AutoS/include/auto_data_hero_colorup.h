#ifndef __HERO_COLORUP_DATA_AUTO_H
#define __HERO_COLORUP_DATA_AUTO_H
#include "define.h"

struct _herocolorupinfo {
//--Automatically generated
				short color;	//
				short itemkind;	//
				short itemnum;	//
				short value;	//
				short needvalue;	//
				short odds;	//
				short nextcolor;	//
};
typedef struct _herocolorupinfo HeroColorupInfo;

int herocolorupinfo_init_auto();
int herocolorupinfo_reload_auto();

#endif
