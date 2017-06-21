#ifndef _HERO_H_
#define _HERO_H_
#include "define.h"

int actor_hero_getindex( int actor_index, int herokind );
Hero* actor_hero_getptr( int actor_index, int offset );

int city_hero_getindex( int city_index, int herokind );
Hero* city_hero_getptr( int city_index, int offset );

#endif
