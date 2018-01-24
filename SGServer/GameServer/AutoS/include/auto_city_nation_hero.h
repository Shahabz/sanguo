#ifndef __NATIONHERO_AUTO_H
#define __NATIONHERO_AUTO_H
#include "define.h"

struct _city_nation_hero {
//--Automatically generated
				char herokind;	//名将英雄
				char state;	//1关押,2已经劝降
				char forever;	//是否永久拥有
				unsigned char loyal;	//忠诚度
};
typedef struct _city_nation_hero NationHero;

typedef NationHero * (*LPCB_GETNATIONHERO)( int actorid, int offset );
int city_nation_hero_load_auto( int actorid, int city_index, LPCB_GETNATIONHERO pCB_GetNationHero, const char *pTab );
int city_nation_hero_save_auto( int actorid, int offset, NationHero *pNationHero, const char *pTab, FILE *fp );
int city_nation_hero_batch_save_auto( int actorid, NationHero *pNationHero, int maxcount, const char *pTab, FILE *fp );

#endif
