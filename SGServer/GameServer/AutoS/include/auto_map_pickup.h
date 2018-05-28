#ifndef __MAPPICKUP_AUTO_H
#define __MAPPICKUP_AUTO_H
#include "define.h"

struct _map_pickup {
//--Automatically generated
				int index;	//拾取物索引
				short posx;	//位置
				short posy;	//位置
				short kind;	//种类
				int value;	//数值
				int unit_index;	//显示索引
};
typedef struct _map_pickup MapPickup;

typedef MapPickup * (*LPCB_GETMAPPICKUP)( int index );
typedef int (*LPCB_LOADMAPPICKUP)( int index );
int map_pickup_load_auto( LPCB_GETMAPPICKUP pCB_GetMapPickup, LPCB_LOADMAPPICKUP pCB_LoadMapPickup, const char *pTab );
int map_pickup_save_auto( MapPickup *pMapPickup, const char *pTab, FILE *fp );
int map_pickup_batch_save_auto( MapPickup *pMapPickup, int maxcount,  const char *pTab, FILE *fp );

#endif
