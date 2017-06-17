#ifndef __BUILDING_AUTO_H
#define __BUILDING_AUTO_H
#include "define.h"

struct _city_building {
//--Automatically generated
				char kind;	//资源建筑种类
				char level;	//资源建筑等级
				int finish;	//结束时间
				char official;	//官员级别
				int value;	//通用值
};
typedef struct _city_building Building;

typedef Building * (*LPCB_GETBUILDING)( int cityid, int offset );
int city_building_load_auto( int cityid, int city_index, LPCB_GETBUILDING pCB_GetBuilding, char *pTab );
int city_building_save_auto( int cityid, int offset, Building *pBuilding, char *pTab, FILE *fp );
int city_building_batch_save_auto( int cityid, Building *pBuilding, int maxcount,  char *pTab, FILE *fp );

#endif
