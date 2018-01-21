#ifndef __NATION_AUTO_H
#define __NATION_AUTO_H
#include "define.h"

struct _nation {
//--Automatically generated
				char nation;	//国家
				char level;	//等级
				int exp;	//经验
				char notice[512];	//国家公告
				int official_actorid[10];	//官员id，索引0是国王
				int tiance_point;	//天策府统领点数
				char questlevel[3];	//国家任务
				char questkind[3];	//国家任务
				char missionlevel;	//国家荣誉任务等级
				int missionvalue[4];	//国家荣誉任务次数
				int candidate_actorid[10];	//候选人id
				int candidate_ballot[10];	//候选人选票
				short tiance_level;	//天策府统领等级
				int people;	//国家总人口
				int people_famous;	//名城人口
				int people_capital;	//都城人口
				short capital_townid;	//都城id
				int town_war_index[256];	//国家战争
				int city_war_index[1024];	//城池战争
				int official_city_index[10];	//官员索引，索引0是国王
				int candidate_city_index[10];	//候选人索引
				int notice_change_stamp;	//上次修改公告时间
				int online_notify_king_stamp;	//上次国王上线时间
				int online_notify_r4_stamp;	//上次丞相上线时间
				int online_notify_r5_stamp;	//上次军师上线时间
};
typedef struct _nation Nation;

typedef Nation * (*LPCB_GETNATION)( int nation );
typedef int (*LPCB_LOADNATION)( int nation );
int nation_load_auto( LPCB_GETNATION pCB_GetNation, LPCB_LOADNATION pCB_LoadNation, const char *pTab );
int nation_save_auto( Nation *pNation, const char *pTab, FILE *fp );

#endif
