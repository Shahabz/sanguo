#ifndef _BUILDING_H_
#define _BUILDING_H_
#include "define.h"
#include "server_netsend_auto.h"

#define BUILDING_MAXNUM				6 // 普通建筑数量
#define BUILDING_BARRACKS_MAXNUM	4 // 兵营建筑数量
#define BUILDING_RES_MAXNUM			64// 资源建筑数量

// 可升级的普通建筑
#define	BUILDING_Main		1	// 官府
#define	BUILDING_Wall		2	// 城墙
#define	BUILDING_StoreHouse	3	// 仓库
#define	BUILDING_Tech		4	// 太学院
#define	BUILDING_Craftsman	5	// 材料作坊
#define	BUILDING_Cabinet	6	// 内阁

// 兵营
#define	BUILDING_Infantry				11	// 步兵营
#define	BUILDING_Cavalry				12	// 骑兵营
#define	BUILDING_Archer					13	// 弓兵营
#define	BUILDING_Militiaman_Infantry	14	// 民兵营-步兵
#define	BUILDING_Militiaman_Cavalry		15	// 民兵营-骑兵
#define	BUILDING_Militiaman_Archer		16	// 民兵营-弓兵

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

#define	BUILDING_OP_BUILD	1	// 建造
#define	BUILDING_OP_UPGRADE	2	// 升级
#define	BUILDING_OP_DELETE	3	// 拆除
#define	BUILDING_OP_REBUILD	4	// 改建
#define	BUILDING_TRAIN_MAXNUM	8	// 兵营最多8个招募队列

// 获取建筑指针
Building* building_getptr( int city_index, int offset );
Building* building_getptr_kind( int city_index, int kind );
BuildingBarracks* buildingbarracks_getptr( int city_index, int offset );
BuildingBarracks* buildingbarracks_getptr_kind( int city_index, int kind );
BuildingRes* buildingres_getptr( int city_index, int offset );
BuildingRes *buildingres_getptr_number( int city_index, int kind, int no );

// 获取建筑的配置信息
BuildingUpgradeConfig* building_getconfig( int kind, int level );

// 建筑升级所需秒
int building_sec( City *pCity, BuildingUpgradeConfig *config );

// 获取建筑等级
int building_getlevel( int city_index, int kind, int no );

// 给予一个建筑
int building_give( int city_index, int kind, int num );

// 给予一个资源点（未启用状态的）
int building_giveres( int city_index, int kind );
int building_giveres_build( int actor_index, int kind, int offset );

// 创建建筑
int building_create( int city_index, int kind, int offset );

// 建筑升级
int building_upgrade( int city_index, int kind, int offset );

// 建筑自动升级
int building_upgrade_auto( int city_index, int kind, int offset );
void building_upgrade_autocheck( int city_index );

// 建筑拆除
int building_delete( int city_index, int kind, int offset );

// 建筑改建
int building_rebuild( int city_index, int kind, int offset, int rebuild_kind );

// 建筑升级或拆除操作完成
int building_finish( int city_index, int op, int kind, int offset );

// 加速
int building_workerquick( int actor_index, int kind, int offset, int sec );

// 免费加速
int building_workerfree( int actor_index, int kind, int offset );

// 奖励的加速
int building_awardquick( int city_index, int sec );

// 奖励的加速领取
int building_awardquick_get( int actor_index, int kind, int offset );

// 购买商用建造队列
int building_workerbuy( int actor_index, int type );

// 获取士兵数量
int building_soldiers_total( int city_index, char kind );

// 铁匠铺信息
int building_smithy_send( int city_index );

// 拼发送结构
void building_makestruct( Building *pBuilding, int offset, SLK_NetS_Building *pValue );
void building_barracks_makestruct( BuildingBarracks *pBuilding, int offset, SLK_NetS_BuildingBarracks *pValue );
void building_res_makestruct( BuildingRes *pBuilding, int offset, SLK_NetS_BuildingRes *pValue );

// 建筑单独信息更新
int building_sendinfo( int actor_index, int kind );
int building_sendinfo_barracks( int actor_index, int kind );
int building_sendinfo_res( int actor_index, int offset );

// 发送所有建筑列表
int building_sendlist( int city_index );

// 发送建造队列信息
int building_sendworker( int actor_index );

// 建筑升级信息
int building_send_upgradeinfo( int actor_index, int kind, int offset );

// 建筑动作
int building_action( int actor_index, short kind, short offset, short action );

// 仓库保护量
int building_store_protect( City *pCity, int restype );

// 人口上限
int building_people_max( City *pCity );

// GM
int building_gm( City *pCity );

// GM建筑等级
int building_level_gm( int city_index, int kind, int level );
#endif