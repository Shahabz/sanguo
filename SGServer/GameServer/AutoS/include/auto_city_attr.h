#ifndef __CITYATTR_AUTO_H
#define __CITYATTR_AUTO_H
#include "define.h"

struct _city_attr {
//--Automatically generated
				int actorid;	//角色ID
				short silver_per;	//银币产量增加%
				short wood_per;	//木材产量增加%
				short food_per;	//粮食产量增加%
				short iron_per;	//镔铁产量增加%
				short protectres_per;	//仓库保护增加%
				short buildingsec_per;	//升级时间减少%
				short infantry_attack;	//步兵武将增加攻击力
				short cavalry_attack;	//骑兵武将增加攻击力
				short archer_attack;	//弓兵武将增加攻击力
};
typedef struct _city_attr CityAttr;

typedef CityAttr * (*LPCB_GETCITYATTR)( int actorid );
typedef int (*LPCB_LOADCITYATTR)( int actorid );
int city_attr_load_auto( LPCB_GETCITYATTR pCB_GetCityAttr, LPCB_LOADCITYATTR pCB_LoadCityAttr, const char *pTab );
int city_attr_save_auto( CityAttr *pCityAttr, const char *pTab, FILE *fp );

#endif
