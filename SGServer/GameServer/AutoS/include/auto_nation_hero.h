#ifndef __NATIONHERO_AUTO_H
#define __NATIONHERO_AUTO_H
#include "define.h"

struct _nation_hero {
//--Automatically generated
				short kind;	//国家名将kind
				int actorid;	//当前所属玩家
				short posx;	//位置
				short posy;	//位置
				int runsec;	//逃跑倒计时
				char color;	//颜色
				short level;	//等级
				int exp;	//经验
				int soldiers;	//当前兵力
				short attack_wash;	//洗髓攻击资质
				short defense_wash;	//洗髓防御资质
				short troops_wash;	//洗髓兵力资质
				short colorup;	//良将突破值
				Equip equip[6];	//装备
				int attack;	//攻击
				int defense;	//防御
				int troops;	//兵力
				short attack_increase;	//攻击增强
				short defense_increase;	//防御增强
				short assault;	//攻城
				short defend;	//守城
				int unit_index;	//显示索引
				int city_index;	//当前所属城池索引
};
typedef struct _nation_hero NationHero;

typedef NationHero * (*LPCB_GETNATIONHERO)( int kind );
typedef int (*LPCB_LOADNATIONHERO)( int kind );
int nation_hero_load_auto( LPCB_GETNATIONHERO pCB_GetNationHero, LPCB_LOADNATIONHERO pCB_LoadNationHero, const char *pTab );
int nation_hero_save_auto( NationHero *pNationHero, const char *pTab, FILE *fp );
int nation_hero_batch_save_auto( NationHero *pNationHero, int maxcount,  const char *pTab, FILE *fp );

#endif
