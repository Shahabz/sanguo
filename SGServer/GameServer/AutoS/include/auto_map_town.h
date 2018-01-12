#ifndef __MAPTOWN_AUTO_H
#define __MAPTOWN_AUTO_H
#include "define.h"

struct _map_town {
//--Automatically generated
				int townid;	//城镇id
				char nation;	//所属
				char name[22];	//修改后的名称
				int protect_sec;	//保护时间
				short produce_num;	//征收次数
				int produce_sec;	//生产时间
				int own_actorid;	//城主角色id
				int own_sec;	//城主任期倒计时
				int join_actorid[128];	//参战人员
				int ask_actorid[128];	//申请人员
				int monster[16];	//守卫
				int soldier[16];	//守卫血量存档
				char soldier_update;	//守卫血量存档更新
				char dev_level;	//都城开发等级
				int dev_exp;	//都城开发经验
				int attackcd;	//攻击倒计时
				int total_soldier;	//守卫总数
				int total_soldier_max;	//守卫总数上限
				int dev_expmax;	//都城开发经验上限
				char fightstate;	//是否有战斗
				short pre_townid[8];	//上一级townid列表
				int ask_city_index[128];	//竞选者城池索引
				int underfire_groupindex[4];	//集结索引
				int own_city_index;	//城主索引
				int unit_index;	//显示索引
				char zoneid;	//所属地区
};
typedef struct _map_town MapTown;

typedef MapTown * (*LPCB_GETMAPTOWN)( int townid );
typedef int (*LPCB_LOADMAPTOWN)( int townid );
int map_town_load_auto( LPCB_GETMAPTOWN pCB_GetMapTown, LPCB_LOADMAPTOWN pCB_LoadMapTown, const char *pTab );
int map_town_save_auto( MapTown *pMapTown, const char *pTab, FILE *fp );
int map_town_batch_save_auto( MapTown *pMapTown, int maxcount,  const char *pTab, FILE *fp );

#endif
