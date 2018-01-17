#ifndef _MAP_TOWN_H_
#define _MAP_TOWN_H_
#include "define.h"

#define MAPUNIT_TYPE_TOWN_TYPE1  1
#define MAPUNIT_TYPE_TOWN_TYPE2  2
#define MAPUNIT_TYPE_TOWN_TYPE3  3
#define MAPUNIT_TYPE_TOWN_TYPE4  4
#define MAPUNIT_TYPE_TOWN_TYPE5  5
#define MAPUNIT_TYPE_TOWN_TYPE6  6
#define MAPUNIT_TYPE_TOWN_TYPE7  7 // 名城
#define MAPUNIT_TYPE_TOWN_TYPE8  8 // 都城
#define MAPUNIT_TYPE_TOWN_TYPE9  9 // 皇城

#define MAPUNIT_KING_TOWNID 500

#define MAP_TOWN_JOIN_MAX	128	// 参战人员
#define MAP_TOWN_MONSTER_MAX	16	// 守卫数量
#define MAP_TOWN_UNDERFIRE_GROUP_MAX 4

int map_town_load();
int map_town_save( FILE *fp );
MapTown *map_town_getptr( int townid );

// 显示单元属性
void map_town_makeunit( int index, SLK_NetS_AddMapUnit *pAttr );

// 位置
void map_town_getpos( int index, short *posx, short *posy );

// 获取国家
char map_town_getnation( int townid );

// 获取所在区域
char map_town_getzone( int townid );

// 重置守军
void map_town_monster_reset( int townid, char reset );

// 所有城镇每秒逻辑
void map_town_alllogic();

// 申请城主
int map_town_ask_owner( int actor_index, int townid );

// 分配城主
int map_town_alloc_owner( int townid );

// 放弃城主
int map_town_owner_leave( int actor_index, int townid );

// 补兵
int map_town_soldiers_repair( int actor_index, int townid );

// 城镇征收
int map_town_levy( int actor_index, int townid );

// 城镇领主奖励
int map_town_owner_award();

// 都城出动禁卫军功能启动
int map_town_attack_checkstart();
// 都城出动禁卫军
int map_town_attack( int townid );

// 都城开发所需经验
int map_town_dev_expmax( int townid );

// 添加都城开发经验
int map_town_dev_addexp( int townid, int exp );

// 都城开发
int map_town_dev( int actor_index, int townid );

// 都城开发消除cd
int map_town_dev_delcd( int actor_index, int townid );

// 修改名称
int map_town_changename( int actor_index, int townid, char *pname );

// 获取城镇奖励
int map_town_sendaward( int actor_index, int townid );

// 获取城镇信息
void map_town_info_makestruct( SLK_NetS_MapTownInfo *pValue, MapTown *pTown, int actorid, char path );
int map_town_sendinfo( int actor_index, int townid );

// 获取都城信息
int map_town_ex_sendinfo( int actor_index, int townid );

// 申请列表
int map_town_ask_owner_sendlist( int actor_index, int townid );

// 城镇被攻击信息添加
int map_town_underfire_groupadd( int townid, int group_index );

// 城镇被攻击信息移除
int map_town_underfire_groupdel( int townid, int group_index );
#endif
