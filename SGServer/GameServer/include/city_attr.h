#ifndef _CITY_ATTR_H_
#define _CITY_ATTR_H_
#include "define.h"

#define CITY_ATTR_ABILITY_1			1	// 提升银币每小时产量%
#define CITY_ATTR_ABILITY_2			2	// 提升木材每小时产量%
#define CITY_ATTR_ABILITY_3			3	// 提升粮食每小时产量%
#define CITY_ATTR_ABILITY_4			4	// 提升镔铁每小时产量%
#define CITY_ATTR_ABILITY_5			5	// 提升步兵每分钟招募数量%
#define CITY_ATTR_ABILITY_6			6	// 提升骑兵每分钟招募数量%
#define CITY_ATTR_ABILITY_7			7	// 提升弓兵每分钟招募数量%
#define CITY_ATTR_ABILITY_8			8	// 提升武将行军速度%（科技使用）
#define CITY_ATTR_ABILITY_9			9	// 提升仓库保护资源量%
#define CITY_ATTR_ABILITY_10		10	// 缩短建筑升级所需时间%
#define CITY_ATTR_ABILITY_11		11	// 缩短作坊生产耗时%(科技使用)
#define CITY_ATTR_ABILITY_12		12	// 单位时间采集收获加成%(科技使用)
#define CITY_ATTR_ABILITY_13		13	// 增加武将幸运突破概率%
#define CITY_ATTR_ABILITY_14		14	// 增加武将杀敌获得威望数量%
#define CITY_ATTR_ABILITY_15		15	// 对城防军伤害%
#define CITY_ATTR_ABILITY_16		16	// 武官加速募兵%
#define CITY_ATTR_ABILITY_17		17	// 缩短作坊生产耗时%（活动使用）
#define CITY_ATTR_ABILITY_18		18	// 提升武将行军速度%（军曹使用）
#define CITY_ATTR_ABILITY_19		19	// 提升武将行军速度%（风令使用）
#define CITY_ATTR_ABILITY_20		20	// 单位时间采集收获加成%(活动使用)

#define CITY_ATTR_ABILITY_100		100	// 提高所以武将攻击力（固定数值）
#define CITY_ATTR_ABILITY_101		101	// 提高步兵武将攻击力（固定数值）
#define CITY_ATTR_ABILITY_102		102	// 提高骑兵武将攻击力（固定数值）
#define CITY_ATTR_ABILITY_103		103	// 提高弓兵武将攻击力（固定数值）

#define CITY_ATTR_ABILITY_110		110	// 提高所以武将防御力（固定数值）
#define CITY_ATTR_ABILITY_111		111	// 提高步兵武将防御力（固定数值）
#define CITY_ATTR_ABILITY_112		112	// 提高骑兵武将防御力（固定数值）
#define CITY_ATTR_ABILITY_113		113	// 提高弓兵武将防御力（固定数值）

#define CITY_ATTR_ABILITY_120		120	// 提高所以武将兵力（固定数值）
#define CITY_ATTR_ABILITY_121		121	// 提高步兵武将兵力（固定数值）
#define CITY_ATTR_ABILITY_122		122	// 提高骑兵武将兵力（固定数值）
#define CITY_ATTR_ABILITY_123		123	// 提高弓兵武将兵力（固定数值）

#define CITY_ATTR_ABILITY_130		130	// 增加上阵武将数量（固定数值）（科技使用）
#define CITY_ATTR_ABILITY_131		131	// 增加武将带兵排数（固定数值）
#define CITY_ATTR_ABILITY_132		132	// 国器暴击：开启国N倍暴击（固定数值）
#define CITY_ATTR_ABILITY_133		133	// 侦查：可侦查敌国玩家城池信息（固定数值）
#define CITY_ATTR_ABILITY_134		134	// 立即建造加速秒（固定数值）
#define CITY_ATTR_ABILITY_135		135	// 体力购买次数上限（固定数值）
#define CITY_ATTR_ABILITY_136		136	// 自动建造免费购买次数（固定数值）
#define CITY_ATTR_ABILITY_137		137	// 每日免费行军召回（固定数值）
#define CITY_ATTR_ABILITY_138		138	// 增加武将带兵排数（固定数值）（内阁上阵）
#define CITY_ATTR_ABILITY_139		139	// 增加武将带兵排数（固定数值）（内阁采集）

#define CITY_ATTR_ABILITY_201		201	// 秘技：金色以上装备技能满级且相同，可开启秘技（开启功能）
#define CITY_ATTR_ABILITY_202		202 // 极品装备：打造装备有概率直接获得秘技装备（开启功能）
#define CITY_ATTR_ABILITY_203		203	// 改土辟地：可拆除建筑并选择指定类型建筑进行重建（开启功能）
#define CITY_ATTR_ABILITY_204		204	// 自动补兵：开启自动补兵（开启功能）

void city_attr_reset( City *pCity );
void city_attr_calc( CityAttr *pAttr, short ability, int value, float digit );
void city_attr_sendinfo( int actor_index );

// 战力变化途径
#define  BATTLEPOWER_PATH_HERO		1
#define  BATTLEPOWER_PATH_EQUIP		2
#define  BATTLEPOWER_PATH_TECH		3
#define  BATTLEPOWER_PATH_NEQUIP	4
#define  BATTLEPOWER_PATH_PLACE		5
#define  BATTLEPOWER_PATH_GIRL		6
#define  BATTLEPOWER_PATH_BUILDING	7

// 重置总战力
void city_battlepower_reset( City *pCity );

// 总战力计算
void city_battlepower_calc( City *pCity, char path );

// 英雄提供战力
void city_battlepower_hero_calc( City *pCity );

// 装备提供战力
void city_battlepower_equip_calc( City *pCity );

// 科技提供战力
void city_battlepower_tech_calc( City *pCity );

// 国器提供战力
void city_battlepower_nequip_calc( City *pCity );

// 爵位提供战力
void city_battlepower_place_calc( City *pCity );

// 女将提供战力
void city_battlepower_girl_calc( City *pCity );

// 建筑提供战力
void city_battlepower_building_calc( City *pCity );


// buff
#define CITY_BUFF_MAX		8 // buff数量
#define CITY_BUFF_MARCH		0 // 军曹管buff 行军耗时降低15%
#define CITY_BUFF_TRAIN		1 // 武卒管buff 招募耗时降低15%
#define CITY_BUFF_WIND		2 // 风字令buff 行军耗时降低15%
#define CITY_BUFF_FIRE		3 // 火字令buff 士兵攻击+5%
#define CITY_BUFF_MOUNTAIN	4 // 山字令buff 士兵防御+5%
#define CITY_BUFF_FOREST	5 // 林字令buff 在兵营中额外增加20%伤病恢复
// buff
int city_change_buff( int city_index, int index, int sec, short path );

#endif
