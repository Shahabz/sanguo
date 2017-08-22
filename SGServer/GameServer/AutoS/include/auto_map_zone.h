#ifndef __MAPZONE_AUTO_H
#define __MAPZONE_AUTO_H
#include "define.h"

struct _map_zone {
//--Automatically generated
				int zoneid;	//地区id
				char allow;	//是否允许
};
typedef struct _map_zone MapZone;

typedef MapZone * (*LPCB_GETMAPZONE)( int zoneid );
typedef int (*LPCB_LOADMAPZONE)( int zoneid );
int map_zone_load_auto( LPCB_GETMAPZONE pCB_GetMapZone, LPCB_LOADMAPZONE pCB_LoadMapZone, const char *pTab );
int map_zone_save_auto( MapZone *pMapZone, const char *pTab, FILE *fp );
int map_zone_batch_save_auto( MapZone *pMapZone, int maxcount,  const char *pTab, FILE *fp );

#endif
