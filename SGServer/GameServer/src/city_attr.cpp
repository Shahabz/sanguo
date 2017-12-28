#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "db.h"
#include "define.h"
#include "global.h"
#include "actor.h"
#include "actor_notify.h"
#include "server_netsend_auto.h"
#include "city.h"
#include "city_tech.h"
#include "city_attr.h"
#include "building.h"
#include "item.h"
#include "army.h"
#include "vip.h"
#include "hero.h"

extern Global global;
extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern BuildingUpgrade *g_BuildingUpgrade;
extern int g_BuildingUpgradeCount;

extern City *g_city;
extern int g_city_maxcount;

extern VipInfo *g_vipinfo;
extern int g_vipinfo_maxnum;

extern ItemKind *g_itemkind;
extern int g_itemkindnum;

extern TechInfo *g_techinfo;
extern int g_techinfo_maxnum;

extern NationEquipInfo *g_nationequip;
extern int g_nationequip_maxnum;

extern NationPlace *g_nation_place;
extern int g_nation_place_maxnum;

extern Actor *g_actors;
extern int g_maxactornum;

void city_attr_reset( City *pCity )
{
	if ( !pCity )
		return;

	// 先重置
	memset( &pCity->attr, 0, sizeof( CityAttr ) );

	// 科技影响
	for ( int kind = 1; kind < CITY_TECH_MAX; kind++ )
	{
		char tech_level = pCity->techlevel[kind];
		if ( tech_level > 0 && tech_level < g_techinfo[kind].maxnum )
		{
			city_attr_calc( &pCity->attr, g_techinfo[kind].config[tech_level].ability, g_techinfo[kind].config[tech_level].value, 100.0f );
		}
	}
	city_attr_sendinfo( pCity->actor_index );
}

void city_attr_calc( CityAttr *pAttr, short ability, int value, float digit )
{
	if ( !pAttr )
		return;
	switch ( ability )
	{
	case CITY_ATTR_ABILITY_1:// 提升银币每小时产量%
		pAttr->silver_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_2:// 提升木材每小时产量%
		pAttr->wood_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_3:// 提升粮食每小时产量%
		pAttr->food_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_4:// 提升镔铁每小时产量%
		pAttr->iron_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_5:// 提升步兵每分钟招募数量%
		pAttr->train_per[0] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_6:// 提升骑兵每分钟招募数量%
		pAttr->train_per[1] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_7:// 提升弓兵每分钟招募数量%
		pAttr->train_per[2] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_8:// 提升武将行军速度%
		pAttr->movespeed_per[0] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_9:// 提升仓库保护资源量%
		pAttr->protectres_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_10:// 缩短建筑升级所需时间%
		pAttr->buildingsec_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_11:// 缩短作坊生产耗时%
		pAttr->materialsec_per[0] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_12://单位时间采集收获加成%(科技)
		pAttr->gather_per[0] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_13:// 增加武将幸运突破概率%
		pAttr->heroluck_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_14:// 增加武将杀敌获得威望数量%
		pAttr->prestige_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_15:// 对城防军伤害%
		pAttr->damageguard_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_16:// 武官加速募兵
		pAttr->trainspeed_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_17:// 缩短作坊生产耗时%（活动使用）
		pAttr->materialsec_per[1] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_18:// 提升武将行军速度%（军曹使用）
		pAttr->movespeed_per[1] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_19://  提升武将行军速度%（风令使用）
		pAttr->movespeed_per[2] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_20://单位时间采集收获加成%(活动)
		pAttr->gather_per[1] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_100:// 提高所以武将攻击力（固定数值）
		pAttr->hero_attack[0] += value;
		pAttr->hero_attack[1] += value;
		pAttr->hero_attack[2] += value;
		break;
	case CITY_ATTR_ABILITY_101:// 提高步兵武将攻击力（固定数值）
		pAttr->hero_attack[0] += value;
		break;
	case CITY_ATTR_ABILITY_102:// 提高骑兵武将攻击力（固定数值）
		pAttr->hero_attack[1] += value;
		break;
	case CITY_ATTR_ABILITY_103:// 提高弓兵武将攻击力（固定数值）
		pAttr->hero_attack[2] += value;
		break;
	case CITY_ATTR_ABILITY_110:// 提高所以武将防御力（固定数值）
		pAttr->hero_defense[0] += value;
		pAttr->hero_defense[1] += value;
		pAttr->hero_defense[2] += value;
		break;
	case CITY_ATTR_ABILITY_111:// 提高步兵武将防御力（固定数值）
		pAttr->hero_defense[0] += value;
		break;
	case CITY_ATTR_ABILITY_112:// 提高骑兵武将防御力（固定数值）
		pAttr->hero_defense[1] += value;
		break;
	case CITY_ATTR_ABILITY_113:// 提高弓兵武将防御力（固定数值）
		pAttr->hero_defense[2] += value;
		break;
	case CITY_ATTR_ABILITY_120:// 提高所以武将兵力（固定数值）
		pAttr->hero_troops[0] += value;
		pAttr->hero_troops[1] += value;
		pAttr->hero_troops[2] += value;
		break;
	case CITY_ATTR_ABILITY_121:// 提高步兵武将兵力（固定数值）
		pAttr->hero_troops[0] += value;
		break;
	case CITY_ATTR_ABILITY_122:// 提高骑兵武将兵力（固定数值）
		pAttr->hero_troops[1] += value;
		break;
	case CITY_ATTR_ABILITY_123:// 提高弓兵武将兵力（固定数值）
		pAttr->hero_troops[2] += value;
		break;
	case CITY_ATTR_ABILITY_130:// 增加上阵武将数量（固定数值）
		pAttr->hero_up_num += value;
		break;
	case CITY_ATTR_ABILITY_131:// 增加武将带兵排数（固定数值）
		pAttr->hero_row_num += value;
		break;
	case CITY_ATTR_ABILITY_132:// 国器暴击：开启国N倍暴击（固定数值）
		pAttr->nequip_crit += value;
		break;
	case CITY_ATTR_ABILITY_133:// 侦查：可侦查敌国玩家城池信息（固定数值）
		pAttr->spy_city += value;
		break;
	case CITY_ATTR_ABILITY_134:// 立即建造加速秒（固定数值）
		pAttr->free_sec += value;
		break;
	case CITY_ATTR_ABILITY_135:// 体力购买次数上限（固定数值）
		pAttr->everyday_body_buymax += value;
		break;
	case CITY_ATTR_ABILITY_136:// 自动建造免费购买次数（固定数值）
		pAttr->everyday_autobuild_buynum += value;
		break;
	case CITY_ATTR_ABILITY_137:// 每日免费行军召回（固定数值）
		pAttr->everyday_army_recall += value;
		break;
	case CITY_ATTR_ABILITY_138:// 增加武将带兵排数（固定数值）（内阁上阵）
		pAttr->hero_row_fight += value;
		break;
	case CITY_ATTR_ABILITY_139:// 增加武将带兵排数（固定数值）（内阁采集）
		pAttr->hero_row_gather += value;
		break;
	case CITY_ATTR_ABILITY_201:// 秘技：金色以上装备技能满级且相同，可开启秘技（开启功能）
		pAttr->ability_open_201 = 1;
		break;
	case CITY_ATTR_ABILITY_202:// 极品装备：打造装备有概率直接获得秘技装备（开启功能）
		pAttr->ability_open_202 = 1;
		break;
	case CITY_ATTR_ABILITY_203:// 改土辟地：可拆除建筑并选择指定类型建筑进行重建（开启功能）
		pAttr->ability_open_203 = 1;
		break;
	case CITY_ATTR_ABILITY_204:// 自动补兵：开启自动补兵（开启功能）
		pAttr->ability_open_204 = 1;
		break;
	case CITY_ATTR_ABILITY_205:// 扫荡副本（开启功能）
		pAttr->ability_open_205 = 1;
		break;
	case CITY_ATTR_ABILITY_206:// 跳过战斗（开启功能）
		pAttr->ability_open_206 = 1;
		break;
	case CITY_ATTR_ABILITY_207:// 开启皇宫内院（开启功能）
		pAttr->ability_open_207 = 1;
		break;
	}
}

void city_attr_sendinfo( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return;
	SLK_NetS_CityAttr pValue = { 0 };
	pValue.m_protectres_per = (short)(pCity->attr.protectres_per * 100);
	pValue.m_buildingsec_per = (short)(pCity->attr.buildingsec_per * 100);
	pValue.m_materialsec_per[0] = (short)(pCity->attr.materialsec_per[0] * 100);
	pValue.m_materialsec_per[1] = (short)(pCity->attr.materialsec_per[1] * 100);
	pValue.m_movespeed_per[0] = (short)(pCity->attr.movespeed_per[0] * 100);
	pValue.m_movespeed_per[1] = (short)(pCity->attr.movespeed_per[1] * 100);
	pValue.m_movespeed_per[2] = (short)(pCity->attr.movespeed_per[2] * 100);
	pValue.m_hero_up_num = pCity->attr.hero_up_num;
	pValue.m_hero_row_num = pCity->attr.hero_up_num;
	pValue.m_everyday_body_buymax = pCity->attr.everyday_body_buymax;
	pValue.m_everyday_autobuild_buynum = pCity->attr.everyday_autobuild_buynum;
	pValue.m_everyday_army_recall = pCity->attr.everyday_army_recall;
	pValue.m_ability_open_201 = pCity->attr.ability_open_201;
	pValue.m_ability_open_203 = pCity->attr.ability_open_203;
	pValue.m_ability_open_204 = pCity->attr.ability_open_204;
	pValue.m_ability_open_205 = pCity->attr.ability_open_205;
	pValue.m_ability_open_206 = pCity->attr.ability_open_206;
	pValue.m_ability_open_207 = pCity->attr.ability_open_207;
	netsend_cityattr_S( actor_index, SENDTYPE_ACTOR, &pValue );
}

// 重置总战力
void city_battlepower_reset( City *pCity )
{
	if ( !pCity )
		return;
	city_battlepower_hero_calc( pCity );
	city_battlepower_equip_calc( pCity );
	city_battlepower_tech_calc( pCity );
	city_battlepower_nequip_calc( pCity );
	city_battlepower_place_calc( pCity );
	city_battlepower_girl_calc( pCity );
	city_battlepower_building_calc( pCity );
}

// 总战力计算
void city_battlepower_calc( City *pCity, char path )
{
	if ( !pCity )
		return;
	int old_battlepower = pCity->battlepower;
	pCity->battlepower = pCity->battlepower_hero + pCity->battlepower_equip + pCity->battlepower_tech + pCity->battlepower_nequip + pCity->battlepower_place + pCity->battlepower_girl + pCity->battlepower_building;

	// 有变化
	if ( old_battlepower != pCity->battlepower )
	{
		if ( pCity->actor_index >= 0 )
		{
			SLK_NetS_Battlepower pValue = { 0 };
			pValue.m_total = pCity->battlepower;
			pValue.m_path = path;
			netsend_battlepower_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
		}
	}
}

// 英雄提供战力
void city_battlepower_hero_calc( City *pCity )
{
	if ( !pCity )
		return;
	int battlepower = 0;
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind <= 0 )
			continue;
		battlepower += pCity->hero[tmpi].bp_hero;
	}
	pCity->battlepower_hero = battlepower;
	city_battlepower_calc( pCity, BATTLEPOWER_PATH_HERO );
}

// 装备提供战力
void city_battlepower_equip_calc( City *pCity )
{
	if ( !pCity )
		return;
	int battlepower = 0;
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind <= 0 )
			continue;
		battlepower += pCity->hero[tmpi].bp_equip;
	}
	pCity->battlepower_equip = battlepower;
	city_battlepower_calc( pCity, BATTLEPOWER_PATH_EQUIP );
}

// 科技提供战力
void city_battlepower_tech_calc( City *pCity )
{
	if ( !pCity )
		return;
	int battlepower = 0;
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind <= 0 )
			continue;
		battlepower += pCity->hero[tmpi].bp_tech;
	}
	pCity->battlepower_tech = battlepower;
	city_battlepower_calc( pCity, BATTLEPOWER_PATH_TECH );
}

// 国器提供战力
void city_battlepower_nequip_calc( City *pCity )
{
	if ( !pCity )
		return;
	int battlepower = 0;
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind <= 0 )
			continue;
		battlepower += pCity->hero[tmpi].bp_nequip;
	}
	pCity->battlepower_nequip = battlepower;
	city_battlepower_calc( pCity, BATTLEPOWER_PATH_NEQUIP );
}

// 爵位提供战力
void city_battlepower_place_calc( City *pCity )
{
	if ( !pCity )
		return;
	if ( pCity->place < 0 || pCity->place >= g_nationequip_maxnum - 1 )
		return;
	float battlepower = 0;
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind <= 0 )
			continue;
		battlepower += g_nation_place[pCity->place + 1].value * global.battlepower_attack;
	}
	pCity->battlepower_place = (int)battlepower;
	city_battlepower_calc( pCity, BATTLEPOWER_PATH_PLACE );
}

// 女将提供战力
void city_battlepower_girl_calc( City *pCity )
{
	if ( !pCity )
		return;
	int battlepower = 0;
	pCity->battlepower_girl = battlepower;
	city_battlepower_calc( pCity, BATTLEPOWER_PATH_GIRL );
}

// 建筑提供战力
void city_battlepower_building_calc( struct _city *pCity )
{
	if ( !pCity )
		return;
	int battlepower = 0;
	// 普通建筑
	for ( int tmpi = 0; tmpi < BUILDING_MAXNUM; tmpi++ )
	{
		if ( pCity->building[tmpi].kind <= 0 )
			continue;
		BuildingUpgradeConfig *config = building_getconfig( pCity->building[tmpi].kind, pCity->building[tmpi].level );
		if ( config )
		{
			battlepower += config->battlepower;
		}
	}

	// 兵营建筑
	for ( int tmpi = 0; tmpi < BUILDING_BARRACKS_MAXNUM; tmpi++ )
	{
		if ( pCity->building_barracks[tmpi].kind <= 0 || pCity->building_barracks[tmpi].level <= 0 )
			continue;
		BuildingUpgradeConfig *config = building_getconfig( pCity->building_barracks[tmpi].kind, pCity->building_barracks[tmpi].level );
		if ( config )
		{
			battlepower += config->battlepower;
		}
	}

	// 资源建筑
	for ( int tmpi = 0; tmpi < BUILDING_RES_MAXNUM; tmpi++ )
	{
		if ( pCity->building_res[tmpi].kind <= 0 || pCity->building_res[tmpi].level <= 0 )
			continue;
		BuildingUpgradeConfig *config = building_getconfig( pCity->building_res[tmpi].kind, pCity->building_res[tmpi].level );
		if ( config )
		{
			battlepower += config->battlepower;
		}
	}
	int old_battlepower = pCity->battlepower_building;
	pCity->battlepower_building = battlepower;
	city_battlepower_calc( pCity, BATTLEPOWER_PATH_BUILDING );
}
