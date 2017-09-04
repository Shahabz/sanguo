#ifndef __MAPEVENT_AUTO_H
#define __MAPEVENT_AUTO_H
#include "define.h"

struct _map_event {
//--Automatically generated
				int index;	//事件索引
				short posx;	//位置
				short posy;	//位置
				short kind;	//种类
				int waitsec;	//倒计时
				int actorid;	//属于角色
				char state;	//状态
				int unit_index;	//显示索引
};
typedef struct _map_event MapEvent;

typedef MapEvent * (*LPCB_GETMAPEVENT)( int index );
typedef int (*LPCB_LOADMAPEVENT)( int index );
int map_event_load_auto( LPCB_GETMAPEVENT pCB_GetMapEvent, LPCB_LOADMAPEVENT pCB_LoadMapEvent, const char *pTab );
int map_event_save_auto( MapEvent *pMapEvent, const char *pTab, FILE *fp );
int map_event_batch_save_auto( MapEvent *pMapEvent, int maxcount,  const char *pTab, FILE *fp );

#endif
