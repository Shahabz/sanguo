#ifndef _NATION_HERO_H_
#define _NATION_HERO_H_
#include "define.h"
#include "auto_data_nation_heroinfo.h"

#define NATIONHERO_STATE_CATCH	1	// ±»×¥²¶
#define NATIONHERO_STATE_EMPLOY	2	// ¹éË³

CityNationHero *city_nation_hero_getptr( int city_index, int offset );
int nation_hero_load();
int nation_hero_save( FILE *fp );
NationHero *nation_hero_getptr( int kind );
NationHeroInfo *nation_hero_getconfig( int kind );
void nation_hero_makeunit( int kind, SLK_NetS_AddMapUnit *pAttr );
void nation_hero_getpos( int kind, short *posx, short *posy );
int nation_hero_sendaward( int actor_index, int kind );
void nation_hero_check( char nation );
void nation_hero_showmap( int kind );
int nation_hero_changepos( int kind );
int nation_hero_buypos( int actor_index, int kind );
int nation_hero_checklimit( City *pCity );
int nation_hero_catch( City *pCity, int kind );
int nation_hero_get( City *pCity, int kind );
int nation_hero_leave( City *pCity, int kind );
int nation_hero_run( City *pCity, int kind );
void nation_hero_logic();
int nation_hero_call( int actor_index, int kind );
int nation_hero_addloyal( int actor_index, int kind, int itemkind );
void nation_hero_subloyal();
int nation_hero_rob( City *pAttackCity, City *pCity, char *json );

void nation_hero_attrcalc( int kind ); 
void nation_hero_attrupdate( Hero *pHero );
int nation_hero_sendlist( int actor_index );
int nation_hero_cityinfo( int actor_index, int kind );
int nation_hero_citylist( int actor_index );
int nation_hero_view( int actor_index, int kind );
#endif
