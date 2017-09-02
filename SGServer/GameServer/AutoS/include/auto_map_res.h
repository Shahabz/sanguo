#ifndef __MAPRES_AUTO_H
#define __MAPRES_AUTO_H
#include "define.h"

struct _map_res {
//--Automatically generated
				int index;	//索引
				short posx;	//位置
				short posy;	//位置
				short kind;	//资源种类
				int num;	//已经采集资源量
				int idlesec;	//空闲时长
				int army_index;	//正在采集的部队
				int unit_index;	//显示索引
};
typedef struct _map_res MapRes;

typedef MapRes * (*LPCB_GETMAPRES)( int index );
typedef int (*LPCB_LOADMAPRES)( int index );
int map_res_load_auto( LPCB_GETMAPRES pCB_GetMapRes, LPCB_LOADMAPRES pCB_LoadMapRes, const char *pTab );
int map_res_save_auto( MapRes *pMapRes, const char *pTab, FILE *fp );
int map_res_batch_save_auto( MapRes *pMapRes, int maxcount,  const char *pTab, FILE *fp );

#endif
