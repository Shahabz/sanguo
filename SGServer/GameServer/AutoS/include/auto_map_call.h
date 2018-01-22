#ifndef __MAPCALL_AUTO_H
#define __MAPCALL_AUTO_H
#include "define.h"

struct _map_call {
//--Automatically generated
				char index;	//召唤事件索引
				int actorid;	//属于玩家
				short limitlevel;	//绛夌骇闄愬埗
				int endtimestamp;	//结束时间
				char num;	//当前召唤人数
				char maxnum;	//召唤人数上限
				char city_index;	//玩家索引
				int notifycd;	//邀请公告cd
};
typedef struct _map_call MapCall;

typedef MapCall * (*LPCB_GETMAPCALL)( int index );
typedef int (*LPCB_LOADMAPCALL)( int index );
int map_call_load_auto( LPCB_GETMAPCALL pCB_GetMapCall, LPCB_LOADMAPCALL pCB_LoadMapCall, const char *pTab );
int map_call_save_auto( MapCall *pMapCall, const char *pTab, FILE *fp );
int map_call_batch_save_auto( MapCall *pMapCall, int maxcount,  const char *pTab, FILE *fp );

#endif
