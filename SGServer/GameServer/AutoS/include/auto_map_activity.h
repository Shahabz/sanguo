#ifndef __MAPACTIVITY_AUTO_H
#define __MAPACTIVITY_AUTO_H
#include "define.h"

struct _map_activity {
//--Automatically generated
				int index;	//索引
				short posx;	//位置
				short posy;	//位置
				short kind;	//种类
				int deltime;	//删除时间
				int actorid;	//属于角色
				int hp;	//剩余血量
				int maxhp;	//血量
				char activityid;	//所属活动
				int unit_index;	//显示索引
};
typedef struct _map_activity MapActivity;

typedef MapActivity * (*LPCB_GETMAPACTIVITY)( int index );
typedef int (*LPCB_LOADMAPACTIVITY)( int index );
int map_activity_load_auto( LPCB_GETMAPACTIVITY pCB_GetMapActivity, LPCB_LOADMAPACTIVITY pCB_LoadMapActivity, const char *pTab );
int map_activity_save_auto( MapActivity *pMapActivity, const char *pTab, FILE *fp );
int map_activity_batch_save_auto( MapActivity *pMapActivity, int maxcount,  const char *pTab, FILE *fp );

#endif
