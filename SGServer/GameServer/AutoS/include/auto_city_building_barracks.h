#ifndef __BUILDINGBARRACKS_AUTO_H
#define __BUILDINGBARRACKS_AUTO_H
#include "define.h"

struct _city_building_barracks {
//--Automatically generated
				char kind;	//兵营种类
				char level;	//兵营等级
				int soldiers;	//本营兵力
				char queue;	//扩容队列数量
				int trainnum[8];	//训练队列士兵数
				int trainsec[8];	//训练队列剩余时间
				char trainlong;	//募兵加时级别
				short quicksec;	//募兵加速时间秒
				char value;	//民兵营使用
};
typedef struct _city_building_barracks BuildingBarracks;

typedef BuildingBarracks * (*LPCB_GETBUILDINGBARRACKS)( int actorid, int offset );
int city_building_barracks_load_auto( int actorid, int city_index, LPCB_GETBUILDINGBARRACKS pCB_GetBuildingBarracks, char *pTab );
int city_building_barracks_save_auto( int actorid, int offset, BuildingBarracks *pBuildingBarracks, char *pTab, FILE *fp );
int city_building_barracks_batch_save_auto( int actorid, BuildingBarracks *pBuildingBarracks, int maxcount,  char *pTab, FILE *fp );

#endif
