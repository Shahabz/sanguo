#ifndef __BUILDINGRES_AUTO_H
#define __BUILDINGRES_AUTO_H
#include "define.h"

struct _city_building_res {
//--Automatically generated
				char kind;	//资源建筑种类
				char level;	//资源建筑等级
				int finish;	//结束时间
};
typedef struct _city_building_res BuildingRes;

typedef BuildingRes * (*LPCB_GETBUILDINGRES)( int cityid, int offset );
int city_building_res_load_auto( int cityid, int city_index, LPCB_GETBUILDINGRES pCB_GetBuildingRes, char *pTab );
int city_building_res_save_auto( int cityid, int offset, BuildingRes *pBuildingRes, char *pTab, FILE *fp );
int city_building_res_batch_save_auto( int cityid, BuildingRes *pBuildingRes, int maxcount,  char *pTab, FILE *fp );

#endif
