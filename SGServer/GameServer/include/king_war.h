#ifndef _KING_WAR_H_
#define _KING_WAR_H_
#include "define.h"

#define KINGWAR_TOWN_TYPE1  1 // 集结点
#define KINGWAR_TOWN_TYPE2  2 // 关隘
#define KINGWAR_TOWN_TYPE3  3 // 洛阳

#define KINGWAR_TOWN_QUEUE_MAX	4096	// 战斗队列长度
#define KINGWAR_TOWN_ATTACK		1
#define KINGWAR_TOWN_DEFENSE	2

int kingwar_town_load();
int kingwar_town_save( FILE *fp );
KingwarTown *kingwar_town_getptr( int id );

int kingwar_town_queueadd( int id, int army_index );
int kingwar_town_queuedel( int id, int army_index );
int kingwar_town_queuefetch( int id, char attack );
int kingwar_town_queueget( int id, char attack );
int kingwar_town_queueswap( int id );

void kingwar_activity_logic();
int kingwar_activity_onopen();
int kingwar_activity_onclose();
int kingwar_activity_sendinfo( int actor_index );

int kingwar_sendinfo( int id );
int kingwar_sendlist( int actor_index );
int kingwar_subscribe_cancel( int actor_index );

#endif
