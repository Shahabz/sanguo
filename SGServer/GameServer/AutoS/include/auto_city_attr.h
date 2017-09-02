#ifndef __CITYATTR_AUTO_H
#define __CITYATTR_AUTO_H
#include "define.h"

struct _city_attr {
//--Automatically generated
				int actorid;	//角色ID
				float silver_per;	//银币产量增加%
				float wood_per;	//木材产量增加%
				float food_per;	//粮食产量增加%
				float iron_per;	//镔铁产量增加%
				float train_per[3];	//提升每分钟招募数量%
				float movespeed_per[3];	//提升武将行军速度%
				float protectres_per;	//仓库保护增加%
				float buildingsec_per;	//缩短建筑升级所需时间%
				float materialsec_per[2];	//缩短作坊生产耗时%
				float gather_per[2];	//采集收获加成%
				float heroluck_per;	//增加武将幸运突破概率%
				float prestige_per;	//增加武将杀敌获得威望数量%
				float damageguard_per;	//对城防军伤害%
				float trainspeed_per;	//加速募兵%
				short hero_attack[3];	//提高步兵武将攻击力（固定数值）
				short hero_defense[3];	//提高步兵武将防御力（固定数值）
				short hero_troops[3];	//提高骑兵武将兵力（固定数值）
				char hero_up_num;	//增加上阵武将数量（固定数值）
				char hero_row_num;	//增加武将带兵排数（固定数值）
				char nequip_crit;	//开启国N倍暴击（固定数值）
				char spy_city;	//可侦查敌国玩家城池信息（固定数值）
				int free_sec;	//立即建造加速秒
				char everyday_body_buymax;	//体力购买次数上限
				char everyday_autobuild_buynum;	//自动建造免费购买次数
				char everyday_army_recall;	//每日免费行军召回
				char ability_open_201;	//秘技：金色以上装备技能满级且相同，可开启秘技（开启功能）
				char ability_open_202;	//极品装备：打造装备有概率直接获得秘技装备（开启功能）
				char ability_open_203;	//改土辟地：可拆除建筑并选择指定类型建筑进行重建（开启功能）
				char ability_open_204;	//自动补兵：开启自动补兵（开启功能）
				char ability_open_205;	//扫荡副本（开启功能）
				char ability_open_206;	//跳过战斗（开启功能）
				char ability_open_207;	//开启皇宫内院（开启功能）
};
typedef struct _city_attr CityAttr;

typedef CityAttr * (*LPCB_GETCITYATTR)( int actorid );
typedef int (*LPCB_LOADCITYATTR)( int actorid );
int city_attr_load_auto( LPCB_GETCITYATTR pCB_GetCityAttr, LPCB_LOADCITYATTR pCB_LoadCityAttr, const char *pTab );
int city_attr_save_auto( CityAttr *pCityAttr, const char *pTab, FILE *fp );

#endif
