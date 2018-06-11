#ifndef _WORLD_BOSS_H
#define _WORLD_BOSS_H
#include "define.h"

#define WORLDBOSS_MONSTER_MAX	16

int world_boss_load();
int world_boss_save( FILE *fp );
WorldBoss *world_boss_getptr( int bossid );
int worldboss_sendinfo( int actor_index, int bossid );
int worldboss_sendbattleinfo( int actor_index, int bossid );
int worldboss_battle( int actor_index, SLK_NetC_WorldBossBattle *pValue );
int worldboss_reset( int actor_index, int bossid );
int worldboss_gm( int bossid, int value );
#endif

