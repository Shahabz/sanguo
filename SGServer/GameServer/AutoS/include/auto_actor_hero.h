#ifndef __HERO_AUTO_H
#define __HERO_AUTO_H
#include "define.h"

struct _actor_hero {
//--Automatically generated
				i64 id;	//主索引
				int actorid;	//城池
				int offset;	//索引
				short kind;	//英雄种类
				short level;	//等级
				int exp;	//经验
				int soldiers;	//兵力
				short attack;	//攻击资质
				short defense;	//防御资质
				short troops;	//兵力资质
				short growth;	//总资质
				Equip equip[6];	//装备
};
typedef struct _actor_hero Hero;

typedef Hero * (*LPCB_GETHERO)( int actorid, int offset );
int actor_hero_load_auto( int actorid, int actor_index, LPCB_GETHERO pCB_GetHero, char *pTab );
int actor_hero_save_auto( Hero *pHero, char *pTab, FILE *fp );
int actor_hero_batch_save_auto( Hero *pHero, int maxcount,  char *pTab, FILE *fp );

#endif
