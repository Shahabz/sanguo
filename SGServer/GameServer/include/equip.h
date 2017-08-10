#ifndef _EQUIP_H_
#define _EQUIP_H_
#include "define.h"

#define EQUIP_TYPE_EQUIP1  1	// 武器
#define EQUIP_TYPE_EQUIP2  2	// 坐骑
#define EQUIP_TYPE_EQUIP3  3	// 盔甲
#define EQUIP_TYPE_EQUIP4  4	// 头盔
#define EQUIP_TYPE_EQUIP5  5	// 将印
#define EQUIP_TYPE_EQUIP6  6	// 兵符
#define EQUIP_TYPE_MAX		6
#define EQUIP_BASEOFFSET		1000	// 穿到英雄身上的装备索引偏移
#define EQUIP_WASHMAX		4	// 最多4条洗练属性

#define EQUIP_ABILITY_ATTACK			1
#define EQUIP_ABILITY_DEFENSE			2
#define EQUIP_ABILITY_TROOPS			3
#define EQUIP_ABILITY_ATTACK_INCREASE	4
#define EQUIP_ABILITY_DEFENSE_INCREASE	5
#define EQUIP_ABILITY_ASSAULT			6
#define EQUIP_ABILITY_DEFEND			7

// 创建道具成功后返回的结构
typedef struct _equipout
{
	int m_equip_kind;		// 物品种类
	int m_equip_offset;	// 物品栏的位置
}EquipOut;

// 将offset转换成对应的属性结构
Equip *actor_equip_getptr( int actor_index, int offset );
Equip *city_equip_getptr( int city_index, int offset );
int equip_load( int actor_index );
int equip_save( int actor_index, FILE *fp );
int equip_maxid_init();

int equip_gettype( short kind );
int equip_insert( int actor_index, int offset ); // 创建装备的时候调用，保存刚创建的装备
int equip_create( int actor_index, short equipkind, EquipOut *pOut ); // 玩家创建装备
int equip_getequip( int actor_index, int equipkind, char path ); // 玩家获得装备
int equip_lostequip( int actor_index, int equipoffset, char path );
int equip_resolve( int actor_index, int equipoffset ); // 分解
int equip_buyext( int actor_index ); // 购买扩展栏

int equip_up( int actor_index, short herokind, int equip_offset ); // 装备上,带有替换效果
int equip_down( int actor_index, short herokind, int index ); // 卸身上的装备

int equip_list( int actor_index ); // 发送装备列表
int equip_sendbag( int actor_index, int offset ); // 更新背包里的装备
int equip_sendhero( int actor_index, Hero *pHero, int index ); // 更新英雄的装备
int equip_sendget( int actor_index, int offset, char path );
int equip_sendlost( int actor_index, short equipkind, int offset, char path );
int equip_heroupdate( int actor_index, Hero *pHero );// 更新英雄装备列表

int equip_forgingtime( int city_index, short kind );
int equip_forging( int actor_index, short kind );
int equip_forging_quick( int actor_index );
int equip_forging_freequick( int actor_index );
int equip_forging_get( int actor_index );
#endif
