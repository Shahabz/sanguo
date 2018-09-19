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
#include "nation.h"
#include "auto_data_vip.h"
#include "auto_data_item.h"
#include "auto_data_city_tech.h"
#include "auto_data_nation_place.h"
#include "auto_data_nation_equip.h"
#include "auto_data_tiance_quest.h"

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

extern TianceQuest *g_tiance_quest;
extern int g_tiance_quest_maxnum;

extern Actor *g_actors;
extern int g_maxactornum;

void city_attr_reset( City *pCity )
{
	if ( !pCity )
		return;

	// ������
	memset( &pCity->attr, 0, sizeof( CityAttr ) );

	// �Ƽ�Ӱ��
	for ( int kind = 1; kind < CITY_TECH_MAX; kind++ )
	{
		char tech_level = pCity->techlevel[kind];
		if ( tech_level > 0 && tech_level < g_techinfo[kind].maxnum )
		{
			city_attr_calc( &pCity->attr, g_techinfo[kind].config[tech_level].ability, g_techinfo[kind].config[tech_level].value, 100.0f );
		}
	}

	// buff Ӱ��
	if ( pCity->buffsec[CITY_BUFF_MARCH] > 0 )
	{
		city_attr_calc( &pCity->attr, CITY_ATTR_ABILITY_18, global.buff_train, 100.0f );
	}
	if ( pCity->buffsec[CITY_BUFF_WIND] > 0 )
	{
		city_attr_calc( &pCity->attr, CITY_ATTR_ABILITY_19, global.buff_wind, 100.0f );
	}

	// ��߸���������
	for ( short kind = 1; kind < g_tiance_quest_maxnum; kind++ )
	{
		char tc_level = pCity->tc_level[kind-1];
		if ( tc_level > 0 && tc_level < g_tiance_quest[kind].maxnum )
		{
			city_attr_calc( &pCity->attr, g_tiance_quest[kind].config[tc_level].ability, g_tiance_quest[kind].config[tc_level].showvalue, 100.0f );
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
	case CITY_ATTR_ABILITY_1:// ��������ÿСʱ����%
		pAttr->silver_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_2:// ����ľ��ÿСʱ����%
		pAttr->wood_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_3:// ������ʳÿСʱ����%
		pAttr->food_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_4:// ��������ÿСʱ����%
		pAttr->iron_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_5:// ��������ÿ������ļ����%
		pAttr->train_per[0] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_6:// �������ÿ������ļ����%
		pAttr->train_per[1] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_7:// ��������ÿ������ļ����%
		pAttr->train_per[2] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_8:// �����佫�о��ٶ�%
		pAttr->movespeed_per[0] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_9:// �����ֿⱣ����Դ��%
		pAttr->protectres_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_10:// ���̽�����������ʱ��%
		pAttr->buildingsec_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_11:// ��������������ʱ%
		pAttr->materialsec_per[0] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_12://��λʱ��ɼ��ջ�ӳ�%(�Ƽ�)
		pAttr->gather_per[0] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_13:// �����佫����ͻ�Ƹ���%
		pAttr->heroluck_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_14:// �����佫ɱ�л����������%
		pAttr->prestige_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_15:// �ԳǷ����˺�%
		pAttr->damageguard_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_16:// ��ټ���ļ��
		pAttr->trainspeed_per += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_17:// ��������������ʱ%���ʹ�ã�
		pAttr->materialsec_per[1] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_18:// �����佫�о��ٶ�%������ʹ�ã�
		pAttr->movespeed_per[1] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_19://  �����佫�о��ٶ�%������ʹ�ã�
		pAttr->movespeed_per[2] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_20://��λʱ��ɼ��ջ�ӳ�%(�)
		pAttr->gather_per[1] += (float)(value / digit);
		break;
	case CITY_ATTR_ABILITY_100:// ��������佫���������̶���ֵ��
		pAttr->hero_attack[0] += value;
		pAttr->hero_attack[1] += value;
		pAttr->hero_attack[2] += value;
		break;
	case CITY_ATTR_ABILITY_101:// ��߲����佫���������̶���ֵ��
		pAttr->hero_attack[0] += value;
		break;
	case CITY_ATTR_ABILITY_102:// �������佫���������̶���ֵ��
		pAttr->hero_attack[1] += value;
		break;
	case CITY_ATTR_ABILITY_103:// ��߹����佫���������̶���ֵ��
		pAttr->hero_attack[2] += value;
		break;
	case CITY_ATTR_ABILITY_110:// ��������佫���������̶���ֵ��
		pAttr->hero_defense[0] += value;
		pAttr->hero_defense[1] += value;
		pAttr->hero_defense[2] += value;
		break;
	case CITY_ATTR_ABILITY_111:// ��߲����佫���������̶���ֵ��
		pAttr->hero_defense[0] += value;
		break;
	case CITY_ATTR_ABILITY_112:// �������佫���������̶���ֵ��
		pAttr->hero_defense[1] += value;
		break;
	case CITY_ATTR_ABILITY_113:// ��߹����佫���������̶���ֵ��
		pAttr->hero_defense[2] += value;
		break;
	case CITY_ATTR_ABILITY_120:// ��������佫�������̶���ֵ��
		pAttr->hero_troops[0] += value;
		pAttr->hero_troops[1] += value;
		pAttr->hero_troops[2] += value;
		break;
	case CITY_ATTR_ABILITY_121:// ��߲����佫�������̶���ֵ��
		pAttr->hero_troops[0] += value;
		break;
	case CITY_ATTR_ABILITY_122:// �������佫�������̶���ֵ��
		pAttr->hero_troops[1] += value;
		break;
	case CITY_ATTR_ABILITY_123:// ��߹����佫�������̶���ֵ��
		pAttr->hero_troops[2] += value;
		break;
	case CITY_ATTR_ABILITY_130:// ���������佫�������̶���ֵ��
		pAttr->hero_up_num += value;
		break;
	case CITY_ATTR_ABILITY_131:// �����佫�����������̶���ֵ��
		pAttr->hero_row_num += value;
		break;
	case CITY_ATTR_ABILITY_132:// ����������������N���������̶���ֵ��
		pAttr->nequip_crit += value;
		break;
	case CITY_ATTR_ABILITY_133:// ��飺�����й���ҳǳ���Ϣ���̶���ֵ��
		pAttr->spy_city += value;
		break;
	case CITY_ATTR_ABILITY_134:// ������������루�̶���ֵ��
		pAttr->free_sec += value;
		break;
	case CITY_ATTR_ABILITY_135:// ��������������ޣ��̶���ֵ��
		pAttr->everyday_body_buymax += value;
		break;
	case CITY_ATTR_ABILITY_136:// �Զ�������ѹ���������̶���ֵ��
		pAttr->everyday_autobuild_buynum += value;
		break;
	case CITY_ATTR_ABILITY_137:// ÿ������о��ٻأ��̶���ֵ��
		pAttr->everyday_army_recall += value;
		break;
	case CITY_ATTR_ABILITY_138:// �����佫�����������̶���ֵ��������
		pAttr->hero_row_fight += value;
		break;
	case CITY_ATTR_ABILITY_139:// �����佫�����������̶���ֵ�����ڸ�ɼ���
		pAttr->hero_row_gather += value;
		break;
	case CITY_ATTR_ABILITY_201:// �ؼ�����ɫ����װ��������������ͬ���ɿ����ؼ����������ܣ�
		pAttr->ability_open_201 = 1;
		break;
	case CITY_ATTR_ABILITY_202:// ��Ʒװ��������װ���и���ֱ�ӻ���ؼ�װ�����������ܣ�
		pAttr->ability_open_202 = 1;
		break;
	case CITY_ATTR_ABILITY_203:// �����ٵأ��ɲ��������ѡ��ָ�����ͽ��������ؽ����������ܣ�
		pAttr->ability_open_203 = 1;
		break;
	case CITY_ATTR_ABILITY_204:// �Զ������������Զ��������������ܣ�
		pAttr->ability_open_204 = 1;
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
	pValue.m_gather_per[0] = (short)(pCity->attr.gather_per[0] * 100);
	pValue.m_gather_per[1] = (short)(pCity->attr.gather_per[1] * 100);
	pValue.m_movespeed_per[0] = (short)(pCity->attr.movespeed_per[0] * 100);
	pValue.m_movespeed_per[1] = (short)(pCity->attr.movespeed_per[1] * 100);
	pValue.m_movespeed_per[2] = (short)(pCity->attr.movespeed_per[2] * 100);
	pValue.m_hero_up_num = pCity->attr.hero_up_num;
	pValue.m_hero_row_num = pCity->attr.hero_up_num;
	pValue.m_everyday_body_buymax = pCity->attr.everyday_body_buymax + vip_attr_bodybuy( pCity );
	pValue.m_everyday_autobuild_buynum = pCity->attr.everyday_autobuild_buynum;
	pValue.m_everyday_army_recall = pCity->attr.everyday_army_recall;
	pValue.m_ability_open_201 = pCity->attr.ability_open_201;
	pValue.m_ability_open_203 = pCity->attr.ability_open_203;
	pValue.m_ability_open_204 = pCity->attr.ability_open_204;
	netsend_cityattr_S( actor_index, SENDTYPE_ACTOR, &pValue );
}

// ������ս��
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

// ��ս������
void city_battlepower_calc( City *pCity, char path )
{
	if ( !pCity )
		return;
	int old_battlepower = pCity->battlepower;
	pCity->battlepower = pCity->battlepower_hero + pCity->battlepower_equip + pCity->battlepower_tech + pCity->battlepower_nequip + pCity->battlepower_place + pCity->battlepower_girl + pCity->battlepower_building;

	// �б仯
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

// Ӣ���ṩս��
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

// װ���ṩս��
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

// �Ƽ��ṩս��
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

// �����ṩս��
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

// ��λ�ṩս��
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

// Ů���ṩս��
void city_battlepower_girl_calc( City *pCity )
{
	if ( !pCity )
		return;
	int battlepower = 0;
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind <= 0 )
			continue;
		battlepower += pCity->hero[tmpi].bp_girl;
	}
	pCity->battlepower_girl = battlepower;
	city_battlepower_calc( pCity, BATTLEPOWER_PATH_GIRL );
}

// �����ṩս��
void city_battlepower_building_calc( struct _city *pCity )
{
	if ( !pCity )
		return;
	int battlepower = 0;
	// ��ͨ����
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

	// ��Ӫ����
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

	// ��Դ����
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


// buff
int city_change_buff( int city_index, int index, int sec, short path )
{
	CITY_CHECK_INDEX( city_index );
	if ( index < 0 || index >= CITY_BUFF_MAX )
		return -1;
	g_city[city_index].buffsec[index] += sec;
	city_attr_reset( &g_city[city_index] );
	if ( index == CITY_BUFF_TRAIN )
	{
		if ( sec <= 0 )
		{
			g_city[city_index].bufftrain = 0;
		}
	}
	else if ( index == CITY_BUFF_FIRE || index == CITY_BUFF_MOUNTAIN )
	{
		hero_attr_calc_all( &g_city[city_index], 0 );
	}
	if ( g_city[city_index].actor_index >= 0 )
	{
		SLK_NetS_BuffChange pValue = { 0 };
		pValue.m_buffkind = index;
		if ( g_city[city_index].buffsec[index] == 0 )
		{
			pValue.m_endtime = 0;
		}
		else
		{
			pValue.m_endtime = (int)time( NULL ) + g_city[city_index].buffsec[index];
		}
		pValue.m_bufftrain = g_city[city_index].bufftrain;
		pValue.m_path = path;
		netsend_buffchange_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	}
	return 0;
}
