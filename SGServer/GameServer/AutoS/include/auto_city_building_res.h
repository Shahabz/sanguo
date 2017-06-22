#ifndef __BUILDINGRES_AUTO_H
#define __BUILDINGRES_AUTO_H
#include "define.h"

struct _city_building_res {
//--Automatically generated
				char kind;	//资源建筑种类
				char level;	//资源建筑等级
};
typedef struct _city_building_res BuildingRes;

typedef BuildingRes * (*LPCB_GETBUILDINGRES)( int actorid, int offset );
int city_building_res_load_auto( int actorid, int city_index, LPCB_GETBUILDINGRES pCB_GetBuildingRes, const char *pTab );
int city_building_res_save_auto( int actorid, int offset, BuildingRes *pBuildingRes, const char *pTab, FILE *fp );
int city_building_res_batch_save_auto( int actorid, BuildingRes *pBuildingRes, int maxcount, const char *pTab, FILE *fp );

#endif
