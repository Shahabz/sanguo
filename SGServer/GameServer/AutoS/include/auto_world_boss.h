#ifndef __WORLDBOSS_AUTO_H
#define __WORLDBOSS_AUTO_H
#include "define.h"

struct _world_boss {
//--Automatically generated
				char bossid;	//bossid
				int hp;	//当前血量
				int maxhp;	//总血量
				int monster[16];	//部队
};
typedef struct _world_boss WorldBoss;

typedef WorldBoss * (*LPCB_GETWORLDBOSS)( int bossid );
typedef int (*LPCB_LOADWORLDBOSS)( int bossid );
int world_boss_load_auto( LPCB_GETWORLDBOSS pCB_GetWorldBoss, LPCB_LOADWORLDBOSS pCB_LoadWorldBoss, const char *pTab );
int world_boss_save_auto( WorldBoss *pWorldBoss, const char *pTab, FILE *fp );
int world_boss_batch_save_auto( WorldBoss *pWorldBoss, int maxcount,  const char *pTab, FILE *fp );

#endif
