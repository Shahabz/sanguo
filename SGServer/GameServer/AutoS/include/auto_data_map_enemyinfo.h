#ifndef __MAP_ENEMYINFO_DATA_AUTO_H
#define __MAP_ENEMYINFO_DATA_AUTO_H
#include "define.h"

struct _mapenemyinfo {
//--Automatically generated
				short kind;	//
				short level;	//
				short shape;	//
				int first_awardgroup;	//
				int awardgroup;	//
				short monsterid[4];	//
				short type;	//
				int nameid;	//
};
typedef struct _mapenemyinfo MapEnemyInfo;

int mapenemyinfo_init_auto();
int mapenemyinfo_reload_auto();

#endif
