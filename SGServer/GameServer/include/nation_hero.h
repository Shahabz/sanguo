#ifndef _NATION_HERO_H_
#define _NATION_HERO_H_
#include "define.h"

int nation_hero_load();
int nation_hero_save( FILE *fp );
NationHero *nation_hero_getptr( int kind );
NationHeroInfo *nation_hero_getconfig( int kind );
void nation_hero_makeunit( int kind, SLK_NetS_AddMapUnit *pAttr );
void nation_hero_getpos( int kind, short *posx, short *posy );
void nation_hero_check( char nation );
void nation_hero_showmap( int kind );
void nation_hero_attrcalc( int kind, Hero *pHero );
int nation_hero_sendlist( int actor_index );
#endif
