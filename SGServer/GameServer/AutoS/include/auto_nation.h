#ifndef __NATION_AUTO_H
#define __NATION_AUTO_H
#include "define.h"

struct _nation {
//--Automatically generated
				char nation;	//国家
				char level;	//等级
				int exp;	//经验
				char notice[512];	//国家公告
				int official_actorid[16];	//官员id，索引0是国王
				int people;	//国家总人口
				int people_famous;	//名城人口
				int people_capital;	//都城人口
				short capital_townid;	//都城id
				int town_war_index[256];	//国家战争
				int city_war_index[1024];	//城池战争
				int official_city_index[16];	//官员索引，索引0是国王
};
typedef struct _nation Nation;

typedef Nation * (*LPCB_GETNATION)( int nation );
typedef int (*LPCB_LOADNATION)( int nation );
int nation_load_auto( LPCB_GETNATION pCB_GetNation, LPCB_LOADNATION pCB_LoadNation, const char *pTab );
int nation_save_auto( Nation *pNation, const char *pTab, FILE *fp );

#endif
