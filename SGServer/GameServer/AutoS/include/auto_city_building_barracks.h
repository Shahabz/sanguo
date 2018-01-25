#ifndef __BUILDINGBARRACKS_AUTO_H
#define __BUILDINGBARRACKS_AUTO_H
#include "define.h"

struct _city_building_barracks {
//--Automatically generated
				char kind;	//兵营种类
				char level;	//兵营等级
				int trainnum;	//当前训练士兵数
				int trainsec;	//当前训练剩余时间(秒)
				int trainsec_need;	//当前训练需要时间(秒)
				int queuenum[11];	//招募队列
				int queuesec[11];	//招募队列
				char queue;	//扩容队列数量
				char trainlong;	//募兵加时级别
				int overnum;	//已经招募完成的数量
				short quicksec;	//募兵加速时间秒
};
typedef struct _city_building_barracks BuildingBarracks;

typedef BuildingBarracks * (*LPCB_GETBUILDINGBARRACKS)( int actorid, int offset );
int city_building_barracks_load_auto( int actorid, int city_index, LPCB_GETBUILDINGBARRACKS pCB_GetBuildingBarracks, const char *pTab );
int city_building_barracks_save_auto( int actorid, int offset, BuildingBarracks *pBuildingBarracks, const char *pTab, FILE *fp );
int city_building_barracks_batch_save_auto( int actorid, BuildingBarracks *pBuildingBarracks, int maxcount, const char *pTab, FILE *fp );

#endif
