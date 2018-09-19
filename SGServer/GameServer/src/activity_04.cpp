#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>

#include "actor.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "script.h"
#include "db.h"
#include "script_auto.h"
#include "server_netsend_auto.h"
#include "activity.h"
#include "timegmcmd.h"
#include "gmcmd.h"
#include "system.h"
#include "global.h"
#include "pay.h"
#include "actor_times.h"
#include "city.h"
#include "quest.h"
#include "hero.h"
#include "nation.h"
#include "mail.h"
#include "item.h"
#include "equip.h"
#include "map_town.h"
#include "activity_04.h"
#include "auto_data_activity_04.h"
#include "auto_data_equip.h"
#include "auto_data_equip_wash.h"
#include "auto_data_equip_washrule.h"
#include "auto_data_map_towninfo.h"

extern MYSQL *myGame;
extern Actor *g_actors;
extern int g_maxactornum;

extern Global global;
extern CommandEntry *g_command_entry;
extern int g_command_count;

extern City * g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern Actor *g_actors;
extern int g_maxactornum;

extern EquipInfo *g_equipinfo;
extern int g_equipinfo_maxnum;

extern EquipWashInfo *g_equipwash;
extern int g_equipwash_maxnum;

extern EquipWashRule *g_equip_washrule;
extern int g_equip_washrule_maxnum;

extern MapZone *g_map_zone;
extern int g_map_zone_maxcount;

extern MapTownInfo *g_towninfo;
extern int g_towninfo_maxnum;

extern MapTown *g_map_town;
extern int g_map_town_maxcount;

extern ActivityItem *g_activity_item;
extern int g_activity_count;

extern ActivityInfo04 *g_activity_04;
extern int g_activity_04_maxnum;

int activity_04_addvalue( int city_index, int type, int value )
{
	CITY_CHECK_INDEX( city_index );
	int endtime = g_city[city_index].createtime + ACTIVITY_SEVENDAY_TIME;
	if ( (int)time( NULL ) > endtime )
		return -1;
	if ( type < 0 || type >= ACTIVITY_SEVENDAY_TYPE_MAXNUM )
		return -1;
	if ( g_city[city_index].actor_index >= 0 && g_city[city_index].actor_index < g_maxactornum )
	{
		g_actors[g_city[city_index].actor_index].act04_value[type] += value;
	}
	else
	{
		gift( g_city[city_index].actorid, AWARDKIND_ACTIVITY04_VALUE_ADD, value, type, 0, 0, 0, 0, 0, 0, 0, 0, 0, PATH_ACTIVITY );
	}
	return 0;
}

int activity_04_setvalue( int city_index, int type, int value )
{
	CITY_CHECK_INDEX( city_index );
	int endtime = g_city[city_index].createtime + ACTIVITY_SEVENDAY_TIME;
	if ( (int)time( NULL ) > endtime )
		return -1;
	if ( type < 0 || type >= ACTIVITY_SEVENDAY_TYPE_MAXNUM )
		return -1;
	if ( g_city[city_index].actor_index >= 0 && g_city[city_index].actor_index < g_maxactornum )
	{
		g_actors[g_city[city_index].actor_index].act04_value[type] = value;
	}
	else
	{
		gift( g_city[city_index].actorid, AWARDKIND_ACTIVITY04_VALUE_SET, value, type, 0, 0, 0, 0, 0, 0, 0, 0, 0, PATH_ACTIVITY );
	}
	return 0;
}

// ��ս
int activity_04_addvalue_cityfight( int city_index, int targetlevel )
{
	CITY_CHECK_INDEX( city_index );
	int endtime = g_city[city_index].createtime + ACTIVITY_SEVENDAY_TIME;
	if ( (int)time( NULL ) > endtime )
		return -1;
	//	����5������ȼ�����ҳ�
	activity_04_addvalue( city_index, ACTIVITY_SEVENDAY_TYPE16, 1 );

	//	�������4������8������ҳ�
	if ( targetlevel >= 8 )
	{
		activity_04_addvalue( city_index, ACTIVITY_SEVENDAY_TYPE17, 1 );
	}

	//	�������3������10������ҳ�
	if ( targetlevel >= 10 )
	{
		activity_04_addvalue( city_index, ACTIVITY_SEVENDAY_TYPE18, 1 );
	}

	//	�������2������11������ҳ�
	if ( targetlevel >= 11 )
	{
		activity_04_addvalue( city_index, ACTIVITY_SEVENDAY_TYPE19, 1 );
	}
	//	�������1������12������ҳ�
	if ( targetlevel >= 12 )
	{
		activity_04_addvalue( city_index, ACTIVITY_SEVENDAY_TYPE20, 1 );
	}
	return 0;
}

// װ��
int activity_04_addvalue_equip( int city_index, int equiptype, int color )
{
	CITY_CHECK_INDEX( city_index );
	int endtime = g_city[city_index].createtime + ACTIVITY_SEVENDAY_TIME;
	if ( (int)time( NULL ) > endtime )
		return -1;

	//	�ռ�����{ 0 }����ɫ������Ʒ��װ��
	if ( color >= ITEM_COLOR_LEVEL_GREEN )
	{
		activity_04_addvalue( city_index, ACTIVITY_SEVENDAY_TYPE21, 1 );
	}

	//	�ռ�����{ 0 }����ɫ������Ʒ��װ��
	if ( color >= ITEM_COLOR_LEVEL_BLUE )
	{
		activity_04_addvalue( city_index, ACTIVITY_SEVENDAY_TYPE22, 1 );
	}

	//	�ռ�{ 0 }����ɫ������Ʒ�ʱ���
	if ( color >= ITEM_COLOR_LEVEL_GREEN && equiptype == EQUIP_TYPE_EQUIP6 )
	{
		activity_04_addvalue( city_index, ACTIVITY_SEVENDAY_TYPE23, 1 );
	}
	
	//	�ռ�{ 0 }����ɫ������Ʒ��ͷ��
	if ( color >= ITEM_COLOR_LEVEL_GREEN && equiptype == EQUIP_TYPE_EQUIP4 )
	{
		activity_04_addvalue( city_index, ACTIVITY_SEVENDAY_TYPE24, 1 );
	}
	//	�ռ�{ 0 }����ɫ������Ʒ��ͷ��
	if ( color >= ITEM_COLOR_LEVEL_BLUE && equiptype == EQUIP_TYPE_EQUIP4 )
	{
		activity_04_addvalue( city_index, ACTIVITY_SEVENDAY_TYPE25, 1 );
	}
	return 0;
}

// װ��ϴ��
int activity_04_addvalue_equipwash( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	int endtime = g_actors[actor_index].createtime + ACTIVITY_SEVENDAY_TIME;
	if ( (int)time( NULL ) > endtime )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	int old_type32 = g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE32];
	int old_type33 = g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE33];
	g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE32] = 0;
	g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE33] = 0;
	
	short kind = 0;
	short color = 0;
	int new_type32 = 0;
	int new_type33 = 0;
	for ( int tmpi = 0; tmpi < MAX_ACTOR_EQUIPNUM; tmpi++ )
	{
		kind = g_actors[actor_index].equip[tmpi].kind;
		if ( kind <= 0 )
			continue;
		color = g_equipinfo[kind].color;

		char fulllevel = 0;
		for ( int i = 0; i < g_equip_washrule[color].valuenum; i++ )
		{
			char washlevel = g_actors[actor_index].equip[tmpi].washid[i] % 10;
			if ( washlevel >= g_equip_washrule[color].levellimit )
			{
				fulllevel += 1;
			}
		}

		if ( fulllevel >= g_equip_washrule[color].valuenum )
		{
			//	{0}����ɫ������Ʒ��װ��ϴ��������
			if ( color >= ITEM_COLOR_LEVEL_BLUE )
				new_type32 += 1;

			//	{0}����ɫ������Ʒ��װ��ϴ��������
			if ( color >= ITEM_COLOR_LEVEL_PURPLE )
				new_type33 += 1;
		}	
	}

	// �Ѿ�װ����-δ�����
	for ( int heroindex = 0; heroindex < HERO_ACTOR_MAX; heroindex++ )
	{
		if ( g_actors[actor_index].hero[heroindex].kind <= 0 )
			continue;

		for ( int equipoffset = 0; equipoffset < 6; equipoffset++ )
		{
			kind = g_actors[actor_index].hero[heroindex].equip[equipoffset].kind;
			if ( kind <= 0 )
				continue;	
			color = g_equipinfo[kind].color;

			char fulllevel = 0;
			for ( int i = 0; i < g_equip_washrule[color].valuenum; i++ )
			{
				char washlevel = g_actors[actor_index].equip[equipoffset].washid[i] % 10;
				if ( washlevel >= g_equip_washrule[color].levellimit )
				{
					fulllevel += 1;
				}
			}

			if ( fulllevel >= g_equip_washrule[color].valuenum )
			{
				//	{0}����ɫ������Ʒ��װ��ϴ��������
				if ( color >= ITEM_COLOR_LEVEL_BLUE )
					new_type32 += 1;

				//	{0}����ɫ������Ʒ��װ��ϴ��������
				if ( color >= ITEM_COLOR_LEVEL_PURPLE )
					new_type33 += 1;
			}
		}
	}

	// �Ѿ�װ����-�����
	for ( int heroindex = 0; heroindex < HERO_CITY_MAX; heroindex++ )
	{
		if ( pCity->hero[heroindex].kind <= 0 )
			continue;

		for ( int equipoffset = 0; equipoffset < 6; equipoffset++ )
		{
			kind = pCity->hero[heroindex].equip[equipoffset].kind;
			if ( kind <= 0 )
				continue;
			color = g_equipinfo[kind].color;

			char fulllevel = 0;
			for ( int i = 0; i < g_equip_washrule[color].valuenum; i++ )
			{
				char washlevel = g_actors[actor_index].equip[equipoffset].washid[i] % 10;
				if ( washlevel >= g_equip_washrule[color].levellimit )
				{
					fulllevel += 1;
				}
			}

			if ( fulllevel >= g_equip_washrule[color].valuenum )
			{
				//	{0}����ɫ������Ʒ��װ��ϴ��������
				if ( color >= ITEM_COLOR_LEVEL_BLUE )
					new_type32 += 1;

				//	{0}����ɫ������Ʒ��װ��ϴ��������
				if ( color >= ITEM_COLOR_LEVEL_PURPLE )
					new_type33 += 1;
			}
		}
	}

	if ( new_type32 >= old_type32 )
	{
		g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE32] = new_type32;
	}
	else
	{
		g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE32] = old_type32;
	}

	if ( new_type33 >= old_type33 )
	{
		g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE33] = new_type33;
	}
	else
	{
		g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE33] = old_type33;
	}
	return 0;
}

// �佫
int activity_04_addvalue_hero( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	int endtime = g_actors[actor_index].createtime + ACTIVITY_SEVENDAY_TIME;
	if ( (int)time( NULL ) > endtime )
		return -1;
	g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE26] = 0;
	g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE27] = 0;
	g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE28] = 0;
	g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE29] = 0;

	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind <= 0 )
			continue;
		if ( pCity->hero[tmpi].color >= ITEM_COLOR_LEVEL_GREEN )
		{//	����ӵ��{ 0 }����ɫ�����Ͻ�
			g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE26] += 1;
		}
		if ( pCity->hero[tmpi].color >= ITEM_COLOR_LEVEL_BLUE )
		{//	����ӵ��{ 0 }����ɫ�����Ͻ�
			g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE27] += 1;
		}
		if ( pCity->hero[tmpi].color >= ITEM_COLOR_LEVEL_PURPLE )
		{//	����ӵ��{ 0 }����ɫ�����Ͻ�
			g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE28] += 1;
		}
		if ( pCity->hero[tmpi].color >= ITEM_COLOR_LEVEL_GOLD )
		{//	����ӵ��{ 0 }����ɫ�����Ͻ�
			g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE29] += 1;
		}
	}

	for ( int tmpi = 0; tmpi < HERO_ACTOR_MAX; tmpi++ )
	{
		if ( g_actors[actor_index].hero[tmpi].kind <= 0 )
			continue;
		if ( g_actors[actor_index].hero[tmpi].color >= ITEM_COLOR_LEVEL_GREEN )
		{//	����ӵ��{ 0 }����ɫ�����Ͻ�
			g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE26] += 1;
		}
		if ( g_actors[actor_index].hero[tmpi].color >= ITEM_COLOR_LEVEL_BLUE )
		{//	����ӵ��{ 0 }����ɫ�����Ͻ�
			g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE27] += 1;
		}
		if ( g_actors[actor_index].hero[tmpi].color >= ITEM_COLOR_LEVEL_PURPLE )
		{//	����ӵ��{ 0 }����ɫ�����Ͻ�
			g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE28] += 1;
		}
		if ( g_actors[actor_index].hero[tmpi].color >= ITEM_COLOR_LEVEL_GOLD )
		{//	����ӵ��{ 0 }����ɫ�����Ͻ�
			g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE29] += 1;
		}
	}

	return 0;
}

// �佫ϴ��
int activity_04_addvalue_herowash( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	int endtime = g_actors[actor_index].createtime + ACTIVITY_SEVENDAY_TIME;
	if ( (int)time( NULL ) > endtime )
		return -1;
	g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE30] = 0;
	g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE31] = 0;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind <= 0 )
			continue;
		HeroInfoConfig *config = hero_getconfig( pCity->hero[tmpi].kind, pCity->hero[tmpi].color );
		if ( !config )
			continue;
		// ��ϴ������
		int total_wash = pCity->hero[tmpi].attack_wash + pCity->hero[tmpi].defense_wash + pCity->hero[tmpi].troops_wash;
		// ��ϴ����������
		//int total_wash_limit = config->attack_wash_limit + config->defense_wash_limit + config->troops_wash_limit;
		int total_wash_limit = config->total_wash;

		if ( total_wash >= total_wash_limit )
		{//	����{ 0 }���佫������ϴ�赽����
			g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE30] += 1;

			if ( pCity->hero[tmpi].color >= ITEM_COLOR_LEVEL_GOLD )
			{//	����{ 0 }����ɫ��������ϴ�赽����
				g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE31] += 1;
			}
		}
	}

	for ( int tmpi = 0; tmpi < HERO_ACTOR_MAX; tmpi++ )
	{
		Hero *pHero = &g_actors[actor_index].hero[tmpi];
		if ( pHero->kind <= 0 )
			continue;
		HeroInfoConfig *config = hero_getconfig( pHero->kind, pHero->color );
		if ( !config )
			continue;
		// ��ϴ������
		int total_wash = pHero->attack_wash + pHero->defense_wash + pHero->troops_wash;
		// ��ϴ����������
		//int total_wash_limit = config->attack_wash_limit + config->defense_wash_limit + config->troops_wash_limit;
		int total_wash_limit = config->total_wash;

		if ( total_wash >= total_wash_limit )
		{//	����{ 0 }���佫������ϴ�赽����
			g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE30] += 1;

			if ( pHero->color >= ITEM_COLOR_LEVEL_GOLD )
			{//	����{ 0 }����ɫ��������ϴ�赽����
				g_actors[actor_index].act04_value[ACTIVITY_SEVENDAY_TYPE31] += 1;
			}
		}
	}
	return 0;
}

int activity_04_getvalue( int actor_index, int type, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( type < 0 || type >= ACTIVITY_SEVENDAY_TYPE_MAXNUM )
		return -1;
	if ( id <= 0 || id >= g_activity_04_maxnum )
		return -1;
	int value = 0;
	switch ( type )
	{
	case ACTIVITY_SEVENDAY_TYPE2: //	����о��Ƽ�{ 0 }
		{
			int techkind = g_activity_04[id].value;
			if ( pCity->techlevel[techkind] > 0 || pCity->techprogress[techkind] > 0 )
			{
				value = 1;
			}
		}
		break;
	case ACTIVITY_SEVENDAY_TYPE5: //	�����ﵽ{ 0 }��
		value = pCity->level;
		break;
	case ACTIVITY_SEVENDAY_TYPE8: //	���ڵ�ͼ�������ӵ��{ 0 }���س�
		for ( int townid = 1; townid < g_map_town_maxcount; townid++ )
		{
			if ( g_towninfo[townid].id <= 0 )
				continue;
			if ( g_map_town[townid].zoneid != pCity->zone )
				continue;
			if ( g_map_town[townid].nation != pCity->nation )
				continue;
			if ( g_towninfo[townid].type != MAPUNIT_TYPE_TOWN_XIAN )
				continue;
			value += 1;
		}
		break;
	case ACTIVITY_SEVENDAY_TYPE9: //	���ڵ�ͼ�������ӵ��{ 0 }������
		for ( int townid = 1; townid < g_map_town_maxcount; townid++ )
		{
			if ( g_towninfo[townid].id <= 0 )
				continue;
			if ( g_map_town[townid].zoneid != pCity->zone )
				continue;
			if ( g_map_town[townid].nation != pCity->nation )
				continue;
			if ( g_towninfo[townid].type != MAPUNIT_TYPE_TOWN_JUN )
				continue;
			value += 1;
		}
		break;
	case ACTIVITY_SEVENDAY_TYPE10: //	���빥��{ 0 }���س�
		value = data_record_getvalue( pCity, DATA_RECORD_KILLTOWNTYPE1 );
		break;
	case ACTIVITY_SEVENDAY_TYPE11: //	���빥��{ 0 }������
		value = data_record_getvalue( pCity, DATA_RECORD_KILLTOWNTYPE2 );
		break;
	case ACTIVITY_SEVENDAY_TYPE12: //	���빥��{ 0 }������
		value = data_record_getvalue( pCity, DATA_RECORD_KILLTOWNTYPE3 );
		break;
	case ACTIVITY_SEVENDAY_TYPE14: //	���߸���ͨ�ص�{ 0 }��
		value = g_actors[actor_index].storyid;
		break;
	case ACTIVITY_SEVENDAY_TYPE15: //	�ٸ��ȼ��ﵽ{ 0 }��
		value = pCity->building[0].level;
		break;
	case ACTIVITY_SEVENDAY_TYPE35:	//	����{0}������20��
		{
			for ( int tmpi = 0; tmpi < 6; tmpi++ )
			{
				if ( pCity->neq_lv[tmpi] >= 20 )
					value += 1;
			}
		}
		break;
	case ACTIVITY_SEVENDAY_TYPE36:	//	����{0}������30��
		{
			for ( int tmpi = 0; tmpi < 6; tmpi++ )
			{
				if ( pCity->neq_lv[tmpi] >= 30 )
					value += 1;
			}
		}
		break;
	case ACTIVITY_SEVENDAY_TYPE37:	//	����{0}������40��
		{
			for ( int tmpi = 0; tmpi < 6; tmpi++ )
			{
				if ( pCity->neq_lv[tmpi] >= 40 )
					value += 1;
			}
		}
		break;
	case ACTIVITY_SEVENDAY_TYPE38:	//	ս���ﵽ{ 0 }
		value = pCity->battlepower;
		break;
	case ACTIVITY_SEVENDAY_TYPE7:	//	������ȡ��˳
	case ACTIVITY_SEVENDAY_TYPE39:	//	���յ�������
		value = system_getfday() - system_getfday_withstamp( g_actors[actor_index].createtime ) + 1;
		break;
	default:
		value = g_actors[actor_index].act04_value[type];
		break;
	}
	return value;
}

int activity_04_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_Activity04List pValue = { 0 };
	for ( int id = 1; id < g_activity_04_maxnum; id++ )
	{
		pValue.m_list[pValue.m_count].m_value = activity_04_getvalue( actor_index, g_activity_04[id].type, id );
		pValue.m_list[pValue.m_count].m_state = g_actors[actor_index].act04_state[id];
		pValue.m_count += 1;
	}
	pValue.m_myday = system_getfday() - system_getfday_withstamp( g_actors[actor_index].createtime ) + 1;
	netsend_activity04list_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

int activity_04_get( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( id <= 0 || id >= g_activity_04_maxnum )
		return -1;
	int type = g_activity_04[id].type;
	if ( type < 0 || type >= ACTIVITY_SEVENDAY_TYPE_MAXNUM )
		return -1;
	if ( g_actors[actor_index].act04_state[id] == 1 )
		return -1;

	int value = activity_04_getvalue( actor_index, g_activity_04[id].type, id );
	if ( g_activity_04[id].type == ACTIVITY_SEVENDAY_TYPE2 )
	{
		if ( value == 0 )
			return -1;
	}
	else
	{
		if ( value < g_activity_04[id].value )
			return -1;
	}

	for ( int tmpi = 0; tmpi < 3; tmpi++ )
	{
		if ( g_activity_04[id].awardkind[tmpi] <= 0 )
			continue;
		award_getaward( actor_index, g_activity_04[id].awardkind[tmpi], g_activity_04[id].awardnum[tmpi], -1, PATH_ACTIVITY, NULL );
	}
	g_actors[actor_index].act04_state[id] = 1;
	activity_04_sendinfo( actor_index );
	return 0;
}
