#ifndef __CITY_AUTO_H
#define __CITY_AUTO_H
#include "define.h"

struct _city {
//--Automatically generated
				int cityid;	//城池ID
				int actorid;	//角色ID
				char name[22];	//名称
				char type;	//城池类型
				char shape;	//领主形象(选取)
				int headid;	//领主头像(自定义)
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
				short level;	//玩家等级
				char viplevel;	//VIP等级
				int vipexp;	//VIP经验
				char nation;	//国家(魏蜀吴)
				char official;	//官职
				char place;	//爵位
				char zone;	//区域
				int battlepower;	//战力
				char mokilllv;	//击杀野怪最高级别
				short body;	//体力
				short bodysec;	//体力恢复时间(每秒-1)
				int silver;	//银币
				int wood;	//木材
				int food;	//粮草
				int iron;	//镔铁
				unsigned char levynum;	//当前征收次数
				short levysec;	//下次征收时间(每秒-1)
				int people;	//人口
				CityHero hero[64];	//英雄
				Building building[8];	//普通建筑
				BuildingBarracks building_barracks[4];	//兵营建筑
				BuildingRes building_res[64];	//资源建筑
				char worker_type;	//当前升级建筑类型
				char worker_offset;	//当前升级建筑索引
				int worker_sec;	//建造剩余时间(每秒-1)
				char worker_type_ex;	//当前升级建筑类型(商用)
				char worker_offset_ex;	//当前升级建筑索引(商用)
				int worker_sec_ex;	//建造剩余时间(每秒-1)(商用)
				int worker_expire_ex;	//商用建造队列到期时间
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

#endif
