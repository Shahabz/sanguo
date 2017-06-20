#ifndef __HEROEQUIP_AUTO_H
#define __HEROEQUIP_AUTO_H
#include "define.h"

struct _city_hero_equip {
//--Automatically generated
				int kind;	//物品种类
				short ability[4];	//属性
				int value[4];	//属性值
};
typedef struct _city_hero_equip HeroEquip;

typedef HeroEquip * (*LPCB_GETHEROEQUIP)( int city_index, int herokind, int offset );
int city_hero_equip_load_auto( int cityid, int city_index, LPCB_GETHEROEQUIP pCB_GetHeroEquip, char *pTab );
int city_hero_equip_save_auto( int cityid, int herokind, int offset, HeroEquip *pHeroEquip, char *pTab, FILE *fp );
int city_hero_equip_batch_save_auto( int cityid, int herokind, HeroEquip *pHeroEquip, int maxcount,  char *pTab, FILE *fp );

#endif
