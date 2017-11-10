#ifndef _KING_WAR_H_
#define _KING_WAR_H_
#include "define.h"

#define KINGWAR_TOWN_TYPE1  1 // 集结点
#define KINGWAR_TOWN_TYPE2  2 // 关隘
#define KINGWAR_TOWN_TYPE3  3 // 洛阳

#define KINGWAR_TOWN_QUEUE_MAX	2048	// 战斗队列长度
#define KINGWAR_TOWN_ATTACK		1
#define KINGWAR_TOWN_DEFENSE	2

int kingwar_town_load();
int kingwar_town_save( FILE *fp );
KingwarTown *kingwar_town_getptr( int id );

char kingwar_town_attack_or_defense( int id, int army_index );
int kingwar_town_queueadd( int id, char attack, int army_index );
int kingwar_town_queuedel( int id, char attack, int army_index );
int kingwar_town_queueget( int id, char attack );
int kingwar_town_totalcalc( int id, char attack );
int kingwar_town_change_nation( int id, int nation );
void kingwar_town_fight( int id );
void kingwar_town_logic();

int kingwar_activity_load();
int kingwar_activity_save( FILE *fp );
void kingwar_activity_logic();
int kingwar_activity_onopen();
int kingwar_activity_onclose();
int kingwar_activity_inttime();
int kingwar_activity_sendinfo( int actor_index );

int kingwar_town_sendinfo( int id );
int kingwar_town_sendall();
int kingwar_town_sendlist( int actor_index );
int kingwar_subscribe_cancel( int actor_index );

int kingwar_army_goto( int actor_index, int army_index, int id );
int kingwar_army_attack( int actor_index, int army_index, int id );
int kingwar_army_sneak( int actor_index, int army_index, int id );
int kingwar_army_defense( int actor_index, int army_index, int id );
int kingwar_army_pk( int actor_index, int army_index, int id );
int kingwar_army_rebirth( int actor_index, int army_index );
#endif
