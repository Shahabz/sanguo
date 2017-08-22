#ifndef __MAPENEMY_AUTO_H
#define __MAPENEMY_AUTO_H
#include "define.h"

struct _map_enemy {
//--Automatically generated
				int index;	//流寇id
				short posx;	//位置
				short posy;	//位置
				char kind;	//种类
				int unit_index;	//显示索引
};
typedef struct _map_enemy MapEnemy;

typedef MapEnemy * (*LPCB_GETMAPENEMY)( int index );
typedef int (*LPCB_LOADMAPENEMY)( int index );
int map_enemy_load_auto( LPCB_GETMAPENEMY pCB_GetMapEnemy, LPCB_LOADMAPENEMY pCB_LoadMapEnemy, const char *pTab );
int map_enemy_save_auto( MapEnemy *pMapEnemy, const char *pTab, FILE *fp );
int map_enemy_batch_save_auto( MapEnemy *pMapEnemy, int maxcount,  const char *pTab, FILE *fp );

#endif
