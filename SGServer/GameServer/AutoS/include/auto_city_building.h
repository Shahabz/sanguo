#ifndef __BUILDING_AUTO_H
#define __BUILDING_AUTO_H
#include "define.h"

struct _city_building {
//--Automatically generated
				char kind;	//建筑种类
				char level;	//建筑等级
				int sec;	//时间
				int needsec;	//需要时间
				int value;	//通用值
				int overvalue;	//完成的值
				short quicksec;	//加速时间
};
typedef struct _city_building Building;

typedef Building * (*LPCB_GETBUILDING)( int actorid, int offset );
int city_building_load_auto( int actorid, int city_index, LPCB_GETBUILDING pCB_GetBuilding, const char *pTab );
int city_building_save_auto( int actorid, int offset, Building *pBuilding, const char *pTab, FILE *fp );
int city_building_batch_save_auto( int actorid, Building *pBuilding, int maxcount, const char *pTab, FILE *fp );

#endif
