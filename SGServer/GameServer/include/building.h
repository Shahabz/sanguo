#ifndef _BUILDING_H_
#define _BUILDING_H_
#include "define.h"

// 可升级的普通建筑
#define	BUILDING_Main		1	// 官府
#define	BUILDING_Wall		2	// 城墙
#define	BUILDING_StoreHouse	3	// 仓库
#define	BUILDING_Tech		4	// 太学院
#define	BUILDING_Craftsman	5	// 材料作坊
#define	BUILDING_Cabinet	6	// 内阁

// 兵营
#define	BUILDING_Infantry	11	// 步兵营
#define	BUILDING_Cavalry	12	// 骑兵营
#define	BUILDING_Archer		13	// 弓兵营
#define	BUILDING_Militiaman	14	// 民兵营

// 资源建筑
#define	BUILDING_Silver		21	// 银币
#define	BUILDING_Wood		22	// 木材
#define	BUILDING_Food		23	// 食物
#define	BUILDING_Iron		24	// 镔铁

// 纯入口建筑
#define	BUILDING_Smithy		31	// 铁匠铺
#define	BUILDING_Wash		32	// 洗炼铺
#define	BUILDING_Fangshi	33	// 坊市
#define	BUILDING_Shop		34	// 商店
#define	BUILDING_Hero		35	// 聚贤馆
#define	BUILDING_Wishing	36	// 聚宝盆
#define	BUILDING_Help		37	// 帮助

// 获取建筑指针
Building* building_getptr( int city_index, int offset );
BuildingBarracks* buildingbarracks_getptr( int city_index, int offset );
BuildingRes* buildingres_getptr( int city_index, int offset );

// 创建建筑
int building_create( int city_index, int kind, int offset );

// 建筑升级
int building_upgrade( int city_index, int offset );

// 建筑等级
int building_level( int city_index, int offset );

// 获取士兵数量
int building_soldiers_total( int city_index, char kind );

// 发送所有建筑列表
int building_sendlist( int city_index );

#endif