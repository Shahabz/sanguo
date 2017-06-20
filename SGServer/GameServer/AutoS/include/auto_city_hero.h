#ifndef __CITYHERO_AUTO_H
#define __CITYHERO_AUTO_H
#include "define.h"

struct _city_hero {
//--Automatically generated
				short kind;	//英雄种类
				short level;	//等级
				int exp;	//经验
				short soldiers;	//兵力
				short attack;	//攻击资质
				short defense;	//防御资质
				short troops;	//兵力资质
				short growth;	//总资质
				HeroEquip equip[6];	//装备
};
typedef struct _city_hero CityHero;

typedef CityHero * (*LPCB_GETCITYHERO)( int cityid, int offset );
int city_hero_load_auto( int cityid, int city_index, LPCB_GETCITYHERO pCB_GetCityHero, char *pTab );
int city_hero_save_auto( int cityid, int offset, CityHero *pCityHero, char *pTab, FILE *fp );
int city_hero_batch_save_auto( int cityid, CityHero *pCityHero, int maxcount,  char *pTab, FILE *fp );

#endif
