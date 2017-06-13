#ifndef __CITY_AUTO_H
#define __CITY_AUTO_H
#include "define.h"

struct _city {
//--Automatically generated
				int cityid;	//城池ID
				int actorid;	//角色ID
				char name[22];	//名称
				char type;	//城池类型
				char shape;	//城池形象
				int headid;	//默认头像
				short country;	//选择的国家(真实)
				char ipcountry[3];	//IP地址所在国家
				char language;	//选择语言
				char os;	//系统1ios 2android 3win
				char platid;	//平台
				int createtime;	//创建时间
				int lastlogin;	//上次登陆时间
				int lastlogout;	//上次登出时间
				char state;	//城池当前状态
				short posx;	//位置
				short posy;	//位置
				int sflag;	//标志位
				char level;	//等级
				char viplevel;	//VIP等级
				int vipexp;	//VIP经验
				short politics;	//民生设施(一级属性)
				int politics_exp;	//民生设施经验
				short farm;	//农业设施(一级属性)
				int farm_exp;	//农业设施经验
				short business;	//商业设施(一级属性)
				int business_exp;	//商业设施经验
				short military;	//军事设施(一级属性)
				int military_exp;	//军事设施经验
				int people;	//人口(二级属性)
				int food;	//食物(二级属性)
				int money;	//金钱(二级属性)
				int soldiers;	//兵员(二级属性)
				int city_index;	//城池索引
				int actor_index;	//角色索引
				int unit_index;	//显示索引
				int army_index[5];	//部队
};
typedef struct _city City;

typedef City * (*LPCB_GETCITY)( int actorid );
typedef int (*LPCB_LOADCITY)( int actorid );
int city_load_auto( LPCB_GETCITY pCB_GetCity, LPCB_LOADCITY pCB_LoadCity, char *pTab );
int city_save_auto( City *pCity, char *pTab, FILE *fp );
int city_batch_save_auto( City *pCity, int maxcount,  char *pTab, FILE *fp );

#endif
