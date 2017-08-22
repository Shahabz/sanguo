#ifndef __MAPTOWN_AUTO_H
#define __MAPTOWN_AUTO_H
#include "define.h"

struct _map_town {
//--Automatically generated
				int townid;	//城镇id
				char nation;	//所属
				int unit_index;	//显示索引
};
typedef struct _map_town MapTown;

typedef MapTown * (*LPCB_GETMAPTOWN)( int townid );
typedef int (*LPCB_LOADMAPTOWN)( int townid );
int map_town_load_auto( LPCB_GETMAPTOWN pCB_GetMapTown, LPCB_LOADMAPTOWN pCB_LoadMapTown, const char *pTab );
int map_town_save_auto( MapTown *pMapTown, const char *pTab, FILE *fp );
int map_town_batch_save_auto( MapTown *pMapTown, int maxcount,  const char *pTab, FILE *fp );

#endif
