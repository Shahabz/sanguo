#ifndef _HERO_H_
#define _HERO_H_
#include "define.h"

int actor_hero_getindex( int actor_index, int herokind );
Hero* actor_hero_getptr( int actor_index, int offset );

int city_hero_getindex( int city_index, int herokind );
Hero* city_hero_getptr( int city_index, int offset );

int hero_maxid_init();
int hero_gethero( int actor_index, int kind, short path );
int hero_up_auto( int actor_index, int offset );
int hero_up( int actor_index, int offset );
int hero_down( int actor_index, int offset );
void hero_makestruct( int offset, Hero *pHero, SLK_NetS_Hero *pValue );
int hero_list( int actor_index );
#endif
