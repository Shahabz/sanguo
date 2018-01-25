#ifndef __CITYNATIONHERO_AUTO_H
#define __CITYNATIONHERO_AUTO_H
#include "define.h"

struct _city_nation_hero {
//--Automatically generated
				short herokind;	//名将英雄
				char state;	//1关押,2已经劝降
				char forever;	//是否永久拥有
				char loyal;	//忠诚度
				char buypos;	//购买坐标次数
				short posx;	//购买的坐标
				short posy;	//购买的坐标
};
typedef struct _city_nation_hero CityNationHero;

typedef CityNationHero * (*LPCB_GETCITYNATIONHERO)( int actorid, int offset );
int city_nation_hero_load_auto( int actorid, int city_index, LPCB_GETCITYNATIONHERO pCB_GetCityNationHero, const char *pTab );
int city_nation_hero_save_auto( int actorid, int offset, CityNationHero *pCityNationHero, const char *pTab, FILE *fp );
int city_nation_hero_batch_save_auto( int actorid, CityNationHero *pCityNationHero, int maxcount, const char *pTab, FILE *fp );

#endif
