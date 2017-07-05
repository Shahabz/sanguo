#ifndef __HERO_AUTO_H
#define __HERO_AUTO_H
#include "define.h"

struct _actor_hero {
//--Automatically generated
				i64 id;	//主索引
				int actorid;	//城池
				int offset;	//索引
				short kind;	//英雄种类
				char color;	//颜色
				short level;	//等级
				int exp;	//经验
				int soldiers;	//兵力
				short attack_wash;	//洗髓攻击资质
				Equip equip[6];	//装备
				short defense_wash;	//洗髓防御资质
				short troops_wash;	//洗髓兵力资质
};
typedef struct _actor_hero Hero;

typedef Hero * (*LPCB_GETHERO)( int actorid, int offset );
int actor_hero_load_auto( int actorid, int actor_index, LPCB_GETHERO pCB_GetHero, const char *pTab );
int actor_hero_save_auto( Hero *pHero, const char *pTab, FILE *fp );
int actor_hero_batch_save_auto( Hero *pHero, int maxcount,  const char *pTab, FILE *fp );

#endif
