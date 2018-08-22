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
#include "dserver.h"
#include "building.h"
#include "actor.h"
#include "hero.h"
#include "area.h"
#include "actor_send.h"
#include "actor_times.h"
#include "server_netsend_auto.h"
#include "mapunit.h"
#include "fight.h"
#include "actor_notify.h"
#include "item.h"
#include "award.h"
#include "global.h"
#include "quest.h"
#include "world_quest.h"
#include "activity.h"
#include "system.h"
#include "army.h"
#include "army_group.h"
#include "army_march.h"
#include "city.h"
#include "map.h"
#include "map_zone.h"
#include "map_town.h"
#include "map_enemy.h"
#include "map_res.h"
#include "mail.h"
#include "chat.h"
#include "nation.h"
#include "nation_hero.h"
#include "activity_04.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern HeroInfo *g_HeroInfo;
extern short g_HeroCount;


extern City * g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;

extern ItemKind *g_itemkind;
extern int g_itemkindnum;

extern MapZoneInfo *g_zoneinfo;
extern int g_zoneinfo_maxnum;

extern MapEnemyInfo *g_enemyinfo;
extern int g_enemyinfo_maxnum;

extern MapEnemy *g_map_enemy;
extern int g_map_enemy_maxcount;

extern MapRes *g_map_res;
extern int g_map_res_maxcount;

extern MapTownInfo *g_towninfo;
extern int g_towninfo_maxnum;

extern MapTown *g_map_town;
extern int g_map_town_maxcount;

extern Map g_map;
extern Fight g_fight;

extern Army *g_army;
extern int g_army_maxcount;
//extern int g_army_count;
//extern int g_army_maxindex;

ArmyGroup *g_armygroup = NULL;
int g_armygroup_maxcount = 0;
int g_armygroup_count = 0;
extern int g_army_group_maxindex;
extern char g_test_mod;

extern MonsterInfo *g_monster;
extern int g_monster_maxnum;

extern FightHelper *g_fight_helper;
extern int g_fight_helper_maxnum;

// ������ϵĻص�
int armygroup_loadcb( int group_index )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	if ( g_armygroup[group_index].from_type == MAPUNIT_TYPE_CITY )
	{ // �����ǳǳ�
		int city_index = city_getindex_withactorid( g_armygroup[group_index].from_id );
		g_armygroup[group_index].from_index = city_index;
		g_armygroup[group_index].from_nation = city_getnation( city_index );
	}
	else if ( g_armygroup[group_index].from_type == MAPUNIT_TYPE_TOWN )
	{ // �����ǳ���
		int index = g_armygroup[group_index].from_id;
		g_armygroup[group_index].from_index = index;
		g_armygroup[group_index].from_nation = map_town_getnation( index );
	}

	// Ŀ������
	if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_CITY )
	{ // Ŀ���ǳǳ�
		int city_index = city_getindex_withactorid( g_armygroup[group_index].to_id );
		g_armygroup[group_index].to_index = city_index;
		g_armygroup[group_index].to_nation = city_getnation( city_index );
		City *pTargetCity = city_indexptr( city_index );
		if ( pTargetCity )
		{
			// ��ӱ������б�
			city_underfire_groupadd( pTargetCity, group_index );

			// ��ս״̬
			city_setstate( pTargetCity, CITY_STATE_ARMYGROUP );
		}

		// ֪ͨ�����ҵĹ���
		nation_city_war_add( g_armygroup[group_index].from_nation, group_index );
		nation_city_war_add( g_armygroup[group_index].to_nation, group_index );
	}
	else if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_TOWN )
	{ // Ŀ���ǳ���
		int index = g_armygroup[group_index].to_id;
		g_armygroup[group_index].to_index = index;
		g_armygroup[group_index].to_nation = map_town_getnation( index );

		// ��ӱ������б�
		map_town_underfire_groupadd( index, group_index );

		// ��ӵ�������Ϣ
		nation_town_war_add( g_armygroup[group_index].from_nation, group_index );
		nation_town_war_add( g_armygroup[group_index].to_nation, group_index );
	}

	// ��鲿�ӺϷ��ԣ���Ϊ�п��ܲ��Ӳ�������
	for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
	{
		int army_index = g_armygroup[group_index].attack_armyindex[tmpi];
		if ( army_index < 0 || army_index >= g_army_maxcount )
			continue;
		if ( g_army[army_index].group_id != g_armygroup[group_index].id )
		{
			g_armygroup[group_index].attack_armyindex[tmpi] = -1;

			// ����ǰ��
			if ( tmpi < ARMYGROUP_MAXCOUNT - 1 )
			{
				memmove( &g_armygroup[group_index].attack_armyindex[tmpi], &g_armygroup[group_index].attack_armyindex[tmpi + 1], sizeof( int )*(ARMYGROUP_MAXCOUNT - 1 - tmpi) );
				g_armygroup[group_index].attack_armyindex[ARMYGROUP_MAXCOUNT-1] = -1;
			}
		}
	}
	for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
	{
		int army_index = g_armygroup[group_index].defense_armyindex[tmpi];
		if ( army_index < 0 || army_index >= g_army_maxcount )
			continue;
		if ( g_army[army_index].group_id != g_armygroup[group_index].id )
		{
			g_armygroup[group_index].defense_armyindex[tmpi] = -1;

			// ����ǰ��
			if ( tmpi < ARMYGROUP_MAXCOUNT - 1 )
			{
				memmove( &g_armygroup[group_index].defense_armyindex[tmpi], &g_armygroup[group_index].defense_armyindex[tmpi + 1], sizeof( int )*(ARMYGROUP_MAXCOUNT - 1 - tmpi) );
				g_armygroup[group_index].defense_armyindex[ARMYGROUP_MAXCOUNT-1] = -1;
			}
		}
	}
	g_armygroup_count += 1;
	return 0;
}

int armygroup_load()
{
	g_armygroup_maxcount = g_Config.max_citycount * 1;
	g_armygroup = (ArmyGroup*)malloc( sizeof( ArmyGroup ) * g_armygroup_maxcount );
	memset( g_armygroup, 0, sizeof( ArmyGroup ) * g_armygroup_maxcount );
	printf_msg( "ArmyGroup  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_armygroup_maxcount, (sizeof( ArmyGroup )*g_armygroup_maxcount) / 1024.0 / 1024.0, sizeof( ArmyGroup ) / 1024.0 );
	army_group_load_auto( armygroup_getptr, armygroup_loadcb, "map_army_group" );
	return 0;
}

int armygroup_save( FILE *fp )
{
	for ( int tmpi = 0; tmpi < g_armygroup_maxcount; tmpi++ )
	{
		if ( g_armygroup[tmpi].id <= 0 )
			continue;
		army_group_save_auto( &g_armygroup[tmpi], "map_army_group", fp );
	}
	return 0;
}

// ���ݿ�ɾ��һֻ����
void armygroup_del_db( int group_index )
{
	char szSQL[256] = { 0 };
	sprintf( szSQL, "DELETE FROM `map_army_group` WHERE `index`='%d'", group_index );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
	}
	return;
}

ArmyGroup *armygroup_getptr( int group_index )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return NULL;
	return &g_armygroup[group_index];
}

// ��ȡ����army_index
int armygroup_getfreeindex()
{
	for ( int tmpi = 0; tmpi < g_armygroup_maxcount; tmpi++ )
	{
		if ( g_armygroup[tmpi].id <= 0 )
		{
			return tmpi;
		}
	}
	return -1;
}

// ����һ������
int armygroup_create( char type, char from_type, int from_id, char to_type, int to_id, int stateduration )
{
	int group_index = armygroup_getfreeindex();
	if ( group_index < 0 )
		return -1;
	// Ϊ�˱�����Ч�ʣ�������������
	if ( group_index >= g_army_group_maxindex )
	{
		g_army_group_maxindex = group_index + 1;
	}

	memset( &g_armygroup[group_index], 0, sizeof( ArmyGroup ) );
	g_armygroup[group_index].index = group_index;
	g_armygroup[group_index].from_index = -1;
	g_armygroup[group_index].to_index = -1;
	for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
	{
		g_armygroup[group_index].attack_armyindex[tmpi] = -1;
		g_armygroup[group_index].defense_armyindex[tmpi] = -1;
	}

	g_armygroup[group_index].id = (int)time( NULL );
	g_armygroup[group_index].state = ARMYGROUP_STATE_ING;
	g_armygroup[group_index].statetime = 0;
	g_armygroup[group_index].stateduration = stateduration;
	g_armygroup[group_index].type = type;

	// ��������
	g_armygroup[group_index].from_type = from_type;
	g_armygroup[group_index].from_id = from_id;
	if ( g_armygroup[group_index].from_type == MAPUNIT_TYPE_CITY )
	{ // �����ǳǳ�
		int city_index = city_getindex_withactorid( from_id );
		g_armygroup[group_index].from_index = city_index;
		g_armygroup[group_index].from_nation = city_getnation( city_index );
		city_getpos( city_index, &g_armygroup[group_index].from_posx, &g_armygroup[group_index].from_posy );
	}
	else if ( g_armygroup[group_index].from_type == MAPUNIT_TYPE_TOWN )
	{ // �����ǳ���
		int index = g_armygroup[group_index].from_id;
		g_armygroup[group_index].from_index = index;
		g_armygroup[group_index].from_nation = map_town_getnation( index );
		map_town_getpos( index, &g_armygroup[group_index].from_posx, &g_armygroup[group_index].from_posy );
	}

	// Ŀ������
	g_armygroup[group_index].to_type = to_type;
	g_armygroup[group_index].to_id = to_id;
	if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_CITY )
	{ // Ŀ���ǳǳ�
		int city_index = city_getindex_withactorid( g_armygroup[group_index].to_id );
		g_armygroup[group_index].to_index = city_index;
		g_armygroup[group_index].to_nation = city_getnation( city_index );
		city_getpos( city_index, &g_armygroup[group_index].to_posx, &g_armygroup[group_index].to_posy );

		City *pTargetCity = city_indexptr( city_index );
		if ( pTargetCity )
		{
			// ��ӱ������б�
			city_underfire_groupadd( pTargetCity, group_index );

			// ��ս״̬
			city_setstate( pTargetCity, CITY_STATE_ARMYGROUP );
		}

		// ֪ͨ�����ҵĹ���
		nation_city_war_add( g_armygroup[group_index].from_nation, group_index );
		nation_city_war_add( g_armygroup[group_index].to_nation, group_index );
	}
	else if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_TOWN )
	{ // Ŀ���ǳ���
		int index = g_armygroup[group_index].to_id;
		g_armygroup[group_index].to_index = index;
		g_armygroup[group_index].to_nation = map_town_getnation( index );
		map_town_getpos( index, &g_armygroup[group_index].to_posx, &g_armygroup[group_index].to_posy );

		// ��ӱ������б�
		map_town_underfire_groupadd( index, group_index );

		// ��ӵ�������Ϣ
		nation_town_war_add( g_armygroup[group_index].from_nation, group_index );
		nation_town_war_add( g_armygroup[group_index].to_nation, group_index );

		// ֪ͨ�ͻ���UI����
		ui_update( map_town_getzone( index ), SENDTYPE_NATION + g_armygroup[group_index].from_nation, UI_UPDATE_NATIONFIGHT );
		ui_update( map_town_getzone( index ), SENDTYPE_NATION + g_armygroup[group_index].to_nation, UI_UPDATE_NATIONFIGHT );
	}

	g_armygroup_count += 1;
	return group_index;
}

// ɾ��һ������
void armygroup_delete( int group_index )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return;
	if ( g_armygroup[group_index].id <= 0 )
		return;

	int city_index = -1;
	// Ŀ�������
	if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_CITY )
	{
		city_index = g_armygroup[group_index].to_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
		{ // ɾ���������б�
			city_underfire_groupdel( &g_city[city_index], group_index );
		}

		// �ӹ�����Ϣ��ɾ��
		nation_city_war_del( g_armygroup[group_index].from_nation, group_index );
		nation_city_war_del( g_armygroup[group_index].to_nation, group_index );
	}
	else if( g_armygroup[group_index].to_type == MAPUNIT_TYPE_TOWN )
	{
		// ɾ���������б�
		map_town_underfire_groupdel( g_armygroup[group_index].to_index, group_index );

		// �ӹ�����Ϣ��ɾ��
		nation_town_war_del( g_armygroup[group_index].from_nation, group_index );
		nation_town_war_del( g_armygroup[group_index].to_nation, group_index );

		// ֪ͨ�ͻ���UI����
		ui_update( map_town_getzone( g_armygroup[group_index].to_index ), SENDTYPE_NATION + g_armygroup[group_index].from_nation, UI_UPDATE_NATIONFIGHT );
		ui_update( map_town_getzone( g_armygroup[group_index].to_index ), SENDTYPE_NATION + g_armygroup[group_index].to_nation, UI_UPDATE_NATIONFIGHT );
	}
	for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
	{
		if ( g_armygroup[group_index].attack_armyindex[tmpi] >= 0 )
		{
			army_setstate( g_armygroup[group_index].attack_armyindex[tmpi], ARMY_STATE_REBACK );
		}

		if ( g_armygroup[group_index].defense_armyindex[tmpi] >= 0 )
		{
			army_setstate( g_armygroup[group_index].defense_armyindex[tmpi], ARMY_STATE_REBACK );
		}
	}
	armygroup_del_db( group_index );
	memset( &g_armygroup[group_index], 0, sizeof( ArmyGroup ) );
	g_armygroup[group_index].index = -1;
	g_armygroup[group_index].from_index = -1;
	g_armygroup[group_index].to_index = -1;
	for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
	{
		g_armygroup[group_index].attack_armyindex[tmpi] = -1;
		g_armygroup[group_index].defense_armyindex[tmpi] = -1;
	}
	g_armygroup_count -= 1;
}

// ���м����߼�
void armygroup_alllogic()
{
	for ( int tmpi = 0; tmpi < g_armygroup_maxcount; tmpi++ )
	{
		if ( g_armygroup[tmpi].id <= 0 )
			continue;
		armygroup_logic( tmpi );
	}
}

// ��ֻ�����߼�
void armygroup_logic( int group_index )
{
	g_armygroup[group_index].statetime += 1;
	if ( g_armygroup[group_index].statetime == g_armygroup[group_index].stateduration-2 )
	{// ��Ҫ����������Ч
		mappos_action( g_armygroup[group_index].to_posx, g_armygroup[group_index].to_posy, 1 );
	}
	if ( g_armygroup[group_index].statetime >= g_armygroup[group_index].stateduration )
	{ // ս����ʼ
		if ( fight_start_armygroup( group_index ) >= 0 )
		{
			// ս��
			fight_lost_calc();

			if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_CITY )
			{
				armygroup_vs_city( group_index, &g_fight );
			}
			else if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_TOWN )
			{
				armygroup_vs_town( group_index, &g_fight );
			}
		}
		armygroup_delete( group_index );
	}
}

// ��Ӳ���
int armygroup_addarmy( int army_index, char zone )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	int group_index = g_army[army_index].group_index;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	if ( g_army[army_index].group_id != g_armygroup[group_index].id )
		return -1;
	int index = -1;
	if ( g_army[army_index].action == ARMY_ACTION_GROUP_CREATE || g_army[army_index].action == ARMY_ACTION_GROUP_ATTACK || g_army[army_index].action == ARMY_ACTION_NATION_ATTACK )
	{
		for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
		{
			if ( g_armygroup[group_index].attack_armyindex[tmpi] < 0 )
			{
				g_armygroup[group_index].attack_armyindex[tmpi] = army_index;
				g_army[army_index].group_pos = tmpi;
				index = tmpi;
				break;
			}
		}
	}
	else if ( g_army[army_index].action == ARMY_ACTION_GROUP_DEFENSE || g_army[army_index].action == ARMY_ACTION_NATION_DEFENSE )
	{
		for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
		{
			if ( g_armygroup[group_index].defense_armyindex[tmpi] < 0 )
			{
				g_armygroup[group_index].defense_armyindex[tmpi] = army_index;
				g_army[army_index].group_pos = tmpi;
				index = tmpi;
				break;
			}
		}
	}
	if ( index >= 0 )
	{
		// ֪ͨ�ͻ���UI����
		ui_update( zone, SENDTYPE_NATION + g_armygroup[group_index].from_nation, UI_UPDATE_FIGHTINFO );
		ui_update( zone, SENDTYPE_NATION + g_armygroup[group_index].to_nation, UI_UPDATE_FIGHTINFO );
	}
	return -1;
}

// ɾ������
void armygroup_delarmy( int army_index, char zone )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	int group_index = g_army[army_index].group_index;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return;
	if ( g_army[army_index].group_id != g_armygroup[group_index].id )
		return;
	int index = -1;
	if ( g_army[army_index].action == ARMY_ACTION_GROUP_CREATE || g_army[army_index].action == ARMY_ACTION_GROUP_ATTACK || g_army[army_index].action == ARMY_ACTION_NATION_ATTACK )
	{
		for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
		{
			if ( g_armygroup[group_index].attack_armyindex[tmpi] == army_index )
			{
				g_armygroup[group_index].attack_armyindex[tmpi] = -1;
				g_army[army_index].group_pos = -1;
				index = tmpi;
				// ����ǰ��
				if ( tmpi < ARMYGROUP_MAXCOUNT - 1 )
				{
					memmove( &g_armygroup[group_index].attack_armyindex[tmpi], &g_armygroup[group_index].attack_armyindex[tmpi + 1], sizeof( int )*(ARMYGROUP_MAXCOUNT - 1 - tmpi) );
					g_armygroup[group_index].attack_armyindex[ARMYGROUP_MAXCOUNT - 1] = -1;
				}
				break;
			}
		}
	}
	else if ( g_army[army_index].action == ARMY_ACTION_GROUP_DEFENSE || g_army[army_index].action == ARMY_ACTION_NATION_DEFENSE )
	{
		for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
		{
			if ( g_armygroup[group_index].defense_armyindex[tmpi] == army_index )
			{
				g_armygroup[group_index].defense_armyindex[tmpi] = -1;
				g_army[army_index].group_pos = -1;
				index = tmpi;
				// ����ǰ��
				if ( tmpi < ARMYGROUP_MAXCOUNT - 1 )
				{
					memmove( &g_armygroup[group_index].defense_armyindex[tmpi], &g_armygroup[group_index].defense_armyindex[tmpi + 1], sizeof( int )*(ARMYGROUP_MAXCOUNT - 1 - tmpi) );
					g_armygroup[group_index].defense_armyindex[ARMYGROUP_MAXCOUNT - 1] = -1;
				}
				break;
			}
		}
	}
	if ( index >= 0 )
	{
		// ֪ͨ�ͻ���UI����
		ui_update( zone, SENDTYPE_NATION + g_armygroup[group_index].from_nation, UI_UPDATE_FIGHTINFO );
		ui_update( zone, SENDTYPE_NATION + g_armygroup[group_index].to_nation, UI_UPDATE_FIGHTINFO );
	}
}

// ���öӳ�
void armygroup_setleader( int group_index, int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return;
	g_army[army_index].group_index = group_index;
	g_army[army_index].group_id = g_armygroup[group_index].id;
	g_armygroup[group_index].leader_index = army_index;
}

// ����Ƿ�߱��Զ���ɢ����
int armygroup_dismiss( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	int group_index = g_army[army_index].group_index;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	if ( g_army[army_index].group_id != g_armygroup[group_index].id )
		return -1;
	if ( g_armygroup[group_index].from_type != MAPUNIT_TYPE_CITY )
		return -1;

	City *pArmyCity = army_getcityptr( army_index );
	if ( !pArmyCity )
		return -1;
	
	City *pCity = city_indexptr( g_armygroup[group_index].from_index );
	if ( !pCity )
		return -1;

	// ���Ƿ����ߵĲ��ӣ��Ͳ������
	if ( pArmyCity->actorid != pCity->actorid )
		return -1;
	
	// ��鷢���ߵĲ��ӣ�����������ﻹ���ڲ��ˣ������ڣ���ɢ����
	char has = 0;
	for ( int tmpi = 0; tmpi < CITY_BATTLEQUEUE_MAX; tmpi++ )
	{
		int index = pCity->battle_armyindex[tmpi];
		if ( index < 0 || index >= g_army_maxcount )
			continue;
		if ( g_army[index].state == ARMY_STATE_MARCH || g_army[index].state == ARMY_STATE_GROUP_END )
		{
			if ( g_army[index].to_type == MAPUNIT_TYPE_CITY && g_army[index].to_id == g_armygroup[group_index].to_id )
			{
				has = 1;
				break;
			}
		}
	}

	if ( has == 0 )
	{
		// ��˫�����ͼ��᳷�˵��ʼ�

		armygroup_delete( group_index );
	}
	return 0;
}

int armygroup_statetime( int group_index )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return 0;
	return g_armygroup[group_index].statetime;
}             

int armygroup_stateduration( int group_index )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return 0;
	return g_armygroup[group_index].stateduration;
}

// ����������
int armygroup_from_totals( int group_index )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return 0;
	int total = 0;

	for ( int kind = 0; kind < ARMYGROUP_FIGHTHELPER; kind++ )
	{
		int num = g_armygroup[group_index].attack_helpernum[kind];
		if ( num > 0 )
		{
			int monsterid = g_fight_helper[kind + 1].monsterid;
			if ( monsterid <= 0 || monsterid >= g_monster_maxnum )
				continue;
			MonsterInfo *pMonster = &g_monster[monsterid];
			if ( !pMonster )
				continue;
			total += pMonster->troops;
		}
	}

	for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
	{
		int army_index = g_armygroup[group_index].attack_armyindex[tmpi];
		if ( army_index < 0 )
			continue;
		total += g_army[army_index].totals;
	}

	return total;
}

// ����������
int armygroup_to_totals( int group_index )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return 0;
	int total = 0;

	if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_CITY )
	{
		City *pCity = city_indexptr( g_armygroup[group_index].to_index );
		if ( pCity )
		{
			// ��ǽ����
			for ( int tmpi = 0; tmpi < CITY_GUARD_MAX; tmpi++ )
			{
				int monsterid = pCity->guard[tmpi].monsterid;
				if ( monsterid <= 0 )
					continue;
				total += pCity->guard[tmpi].soldiers;
			}

			// �����������
			for ( int tmpi = 0; tmpi < 4; tmpi++ )
			{
				total += pCity->hero[tmpi].soldiers;
			}

			// ����������
			for ( int tmpi = 8; tmpi < 12; tmpi++ )
			{
				total += pCity->hero[tmpi].soldiers;
			}

			// פ������
			for ( int index = 0; index < CITY_HELPDEFENSE_MAX; index++ )
			{
				int army_index = pCity->help_armyindex[index];
				if ( army_index < 0 )
					continue;
				if ( g_army[army_index].state != ARMY_STATE_HELP )
					continue;
				total += g_army[army_index].totals;
			}
		}
	}
	else if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_TOWN )
	{ // ������Ϊ����
		MapTown *pTown = map_town_getptr( g_armygroup[group_index].to_id );
		if ( pTown )
		{
			for ( int kind = 0; kind < ARMYGROUP_FIGHTHELPER; kind++ )
			{
				int num = g_armygroup[group_index].defense_helpernum[kind];
				if ( num > 0 )
				{
					int monsterid = g_fight_helper[kind + 1].monsterid;
					if ( monsterid <= 0 || monsterid >= g_monster_maxnum )
						continue;
					MonsterInfo *pMonster = &g_monster[monsterid];
					if ( !pMonster )
						continue;
					total += pMonster->troops;
				}
			}

			for ( int tmpi = 0; tmpi < MAP_TOWN_MONSTER_MAX; tmpi++ )
			{
				int monsterid = pTown->monster[tmpi];
				if ( monsterid <= 0 )
					continue;
				total += pTown->soldier[tmpi];
			}
		}
	}
	// Э������
	for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
	{
		int army_index = g_armygroup[group_index].defense_armyindex[tmpi];
		if ( army_index < 0 )
			continue;
		total += g_army[army_index].totals;
	}
	return total;
}

// ��ս���
int armygroup_vs_city( int group_index, Fight *pFight )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;

	City *pCity = city_indexptr( g_armygroup[group_index].from_index );
	if ( !pCity )
		return -1;

	City *pTargetCity = city_indexptr( g_armygroup[group_index].to_index );
	if ( !pTargetCity )
		return -1;

	i64 mailid = 0;
	char title[MAIL_TITLE_MAXSIZE] = { 0 };
	char content[MAIL_CONTENT_MAXSIZE] = { 0 };

	if ( pFight->result == FIGHT_WIN )
	{
		// �Ӷ�
		int atk_protect = 0;
		int def_protect = 0;
		int lost_silver = 0;
		int lost_wood = 0;
		int lost_food = 0;
		int lost_people = 0;
		int rob_silver = 0;
		int rob_wood = 0;
		int rob_food = 0;
		int rob_people = 0;

		// ����
		atk_protect = building_store_protect( pCity, 1 );
		def_protect = building_store_protect( pTargetCity, 1 );
		if ( pTargetCity->silver - def_protect <= 0 )
			lost_silver = 0;
		else
			lost_silver = min( (int)ceil( (pTargetCity->silver - def_protect ) * global.cityfight_rob_v1 ), atk_protect );
		if ( pCity->silver > atk_protect )
		{
			rob_silver = (int)ceil( lost_silver * global.cityfight_rob_v2 );
		}
		else
		{
			rob_silver = lost_silver;
		}

		// ľ��
		atk_protect = building_store_protect( pCity, 2 );
		def_protect = building_store_protect( pTargetCity, 2 );
		if ( pTargetCity->wood - def_protect <= 0 )
			lost_wood = 0;
		else
			lost_wood = min( (int)ceil( ( pTargetCity->wood - def_protect ) * global.cityfight_rob_v1 ), atk_protect );
		if ( pCity->wood > atk_protect )
		{
			rob_wood = (int)ceil( lost_wood * global.cityfight_rob_v2 );
		}
		else
		{
			rob_wood = lost_wood;
		}

		// ��ʳ
		atk_protect = building_store_protect( pCity, 3 );
		def_protect = building_store_protect( pTargetCity, 3 );
		if ( pTargetCity->food - def_protect <= 0 )
			lost_food = 0;
		else
			lost_food = min( (int)ceil( ( pTargetCity->food - def_protect ) * global.cityfight_rob_v1 ), atk_protect );
		if ( pCity->food > atk_protect )
		{
			rob_food = (int)ceil( lost_food * global.cityfight_rob_v2 );
		}
		else
		{
			rob_food = lost_food;
		}

		// �˿�
		lost_people = (int)min( ceil( pTargetCity->people * global.cityfight_rob_v3 ), building_people_max( pTargetCity ) );
		rob_people = (int)ceil( lost_people * global.cityfight_rob_v4 );
		if ( lost_people < 0 )
			lost_people = 0;
		if ( rob_people < 0 )
			rob_people = 0;
		// ��ʧ
		city_changesilver( pTargetCity->index, -lost_silver, PATH_FIGHT );
		city_changewood( pTargetCity->index, -lost_wood, PATH_FIGHT );
		city_changefood( pTargetCity->index, -lost_food, PATH_FIGHT );
		city_changepeople( pTargetCity->index, -lost_people, PATH_FIGHT );
		if ( pTargetCity->rb_silver <= 0 )
		{
			if ( pTargetCity->rb_num > 0 )
			{ // �и߼��ؽ���԰
				// ����� = Max��20000�����ط������ʧ��Դ��*0.9��
				// ����Ƿ����� = Min��10�����ط���ǽ�ȼ�����ֻ�����ط���ǽϵͳ������Ÿ��裩
				pTargetCity->rb_num -= 1;
				pTargetCity->rb_silver = (int)max( global.lost_rebuild_v1, lost_silver * global.lost_rebuild_v2 );
				pTargetCity->rb_wood = (int)max( global.lost_rebuild_v1, lost_wood * global.lost_rebuild_v2 );
				pTargetCity->rb_food = (int)max( global.lost_rebuild_v1, lost_food * global.lost_rebuild_v2 );
				Building *pBuilding = building_getptr_kind( pTargetCity->index, BUILDING_Wall );
				if ( pBuilding && pBuilding->level >= global.city_guard_level )
				{
					pTargetCity->rb_df = min( global.lost_rebuild_v3, pBuilding->level );
					city_change_autoguard( pTargetCity->index, pTargetCity->rb_df, PATH_FIGHT );
				}
			}
			else
			{ // ��ͨ�ؽ���԰
				// ����� = ���ط������ʧ��Դ��*0.3
				pTargetCity->rb_silver = (int)ceil( lost_silver * global.lost_rebuild_v4 );
				pTargetCity->rb_wood = (int)ceil( lost_wood * global.lost_rebuild_v4 );
				pTargetCity->rb_food = (int)ceil( lost_food * global.lost_rebuild_v4 );
			}
			city_lost_rebuild( pTargetCity );
		}

		// �Ӷ�
		for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
		{ // �ҵ�����ǳصĲ���
			int army_index = g_armygroup[group_index].attack_armyindex[tmpi];
			if ( army_index < 0 )
				continue;
			City *pArmyCity = army_getcityptr( army_index );
			if ( !pArmyCity )
				continue;
			if ( pArmyCity->actorid == pCity->actorid )
			{
				army_carry_additem( army_index, AWARDKIND_SILVER, rob_silver );
				army_carry_additem( army_index, AWARDKIND_WOOD, rob_wood );
				army_carry_additem( army_index, AWARDKIND_FOOD, rob_food );
				army_carry_additem( army_index, AWARDKIND_PEOPLE, rob_people );
				break;
			}
		}

		// �����Ӷ�
		char nhjson[256] = { 0 };
		nation_hero_rob( pCity, pTargetCity, nhjson );

		// �����ɹ��ʼ�
		sprintf( title, "%s%d", TAG_TEXTID, 5019 );// ��ս����ʤ��
		sprintf( content, "{\"my\":1,\"win\":1,\"na\":\"%s\",\"n\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tna\":\"%s\",\"tn\":%d,\"tlv\":%d,\"tpos\":\"%d,%d\",\"silver\":%d,\"wood\":%d,\"food\":%d,\"people\":%d,\"ws0\":%d,\"ws1\":%d,\"ws2\":%d,\"nhero\":[%s]}",
			pCity->name, pCity->nation, pCity->level, pCity->posx, pCity->posy, pTargetCity->name, pTargetCity->nation, pTargetCity->level, g_armygroup[group_index].to_posx, g_armygroup[group_index].to_posy, rob_silver, rob_wood, rob_food, rob_people, pCity->temp_wounded_soldiers[0], pCity->temp_wounded_soldiers[1], pCity->temp_wounded_soldiers[2], nhjson );

		// ���������˷����ʼ�
		armygroup_mail( group_index, 1, NULL, MAIL_TYPE_FIGHT_CITY, title, content, "", pFight, pTargetCity->name );


		// ����ʧ���ʼ�
		sprintf( title, "%s%d", TAG_TEXTID, 5022 );// ��ս����ʧ��
		sprintf( content, "{\"my\":2,\"win\":0,\"na\":\"%s\",\"n\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tna\":\"%s\",\"tn\":%d,\"tlv\":%d,\"tpos\":\"%d,%d\",\"silver\":%d,\"wood\":%d,\"food\":%d,\"people\":%d,\"ws0\":%d,\"ws1\":%d,\"ws2\":%d,\"nhero\":[%s]}",
			pTargetCity->name, pTargetCity->nation, pTargetCity->level, g_armygroup[group_index].to_posx, g_armygroup[group_index].to_posy, pCity->name, pCity->nation, pCity->level, pCity->posx, pCity->posy, lost_silver, lost_wood, lost_food, lost_people, pTargetCity->temp_wounded_soldiers[0], pTargetCity->temp_wounded_soldiers[1], pTargetCity->temp_wounded_soldiers[2], nhjson );

		// ���������˷����ʼ�
		armygroup_mail( group_index, 2, pTargetCity, MAIL_TYPE_FIGHT_CITY, title, content, "", pFight, pCity->name );

		// ���Ǩ�Ƴ�����ͼ
		char zoneid = map_zone_getid( pTargetCity->posx, pTargetCity->posy );
		if ( zoneid > 0 && zoneid < g_zoneinfo_maxnum )
		{
			short move_posx, move_posy;
			if ( zoneid == MAPZONE_CENTERID )
			{ // �������ڻʳ�����
				int odds = rand() % 100;
				if ( odds < 85 )
				{
					map_zone_nation_randpos( pCity->nation, &move_posx, &move_posy, 4096 );
				}
				else
				{
					map_zone_randpos( zoneid, &move_posx, &move_posy, 4096 );
				}
			}
			else
			{
				map_zone_randpos( zoneid, &move_posx, &move_posy, 4096 );
			}
			if ( map_canmove( move_posx, move_posy ) == 0 )
			{
				map_zone_randpos( zoneid, &move_posx, &move_posy, 4096 );
			}
			if ( map_canmove( move_posx, move_posy ) == 1 )
			{
				city_move( pTargetCity, move_posx, move_posy );

				// ���������ͻ����ʼ�
				mail_system( pTargetCity->actor_index, pTargetCity->actorid, 5023, 5521, pCity->name, NULL, NULL, "", 0 );

				// �����йص�ս��ȡ��
				for ( int tmpi = 0; tmpi < CITY_UNDERFIRE_GROUP_MAX; tmpi++ )
				{
					int index = pCity->underfire_groupindex[tmpi];
					if ( index >= 0 && index != group_index )
					{
						armygroup_delete( index );
					}
				}
			}
			// �Ż�
			city_setstate( pTargetCity, CITY_STATE_FIRE );
		}
		// ���뱣��ʱ��
		city_changeprotect( pTargetCity->index, global.fight_protect, PATH_FIGHT );

		//  ���ݼ�¼ɱ�ǳ�����
		for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
		{
			int army_index = g_armygroup[group_index].attack_armyindex[tmpi];
			if ( army_index < 0 || army_index >= g_army_maxcount )
				continue;
			City *pArmyCity = army_getcityptr( army_index );
			if ( !pArmyCity )
				continue;

			// ��������
			data_record_addvalue( pArmyCity, DATA_RECORD_KILLCITY, 1 );

			// ��������
			nation_quest_addvalue( pArmyCity, NATION_QUESTKIND_CITY, 1 );

			// ������������
			nation_rank_addvalue( pArmyCity, NATION_RANK_CITY, 1 );

			// ���տ�
			activity_04_addvalue_cityfight( pArmyCity->index, pTargetCity ->level );
		}

		// ������������
		nation_mission_addvalue( pCity->nation, NATION_MISSIONKIND_CITY, 1 );
	}
	else
	{
		// ����ʧ���ʼ�
		sprintf( title, "%s%d", TAG_TEXTID, 5020 ); // ��ս����ʧ��
		sprintf( content, "{\"my\":1,\"win\":0,\"na\":\"%s\",\"n\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tna\":\"%s\",\"tn\":%d,\"tlv\":%d,\"tpos\":\"%d,%d\",\"ws0\":%d,\"ws1\":%d,\"ws2\":%d}",
			pCity->name, pCity->nation, pCity->level, pCity->posx, pCity->posy, pTargetCity->name, pTargetCity->nation, pTargetCity->level, g_armygroup[group_index].to_posx, g_armygroup[group_index].to_posy, pCity->temp_wounded_soldiers[0], pCity->temp_wounded_soldiers[1], pCity->temp_wounded_soldiers[2] );

		// ���������˷����ʼ�
		armygroup_mail( group_index, 1, NULL, MAIL_TYPE_FIGHT_CITY, title, content, "", pFight, pCity->name );

		// ���سɹ��ʼ�
		sprintf( title, "%s%d", TAG_TEXTID, 5021 );// ��ս����ʤ��
		sprintf( content, "{\"my\":2,\"win\":1,\"na\":\"%s\",\"n\":%d,\"lv\":%d,\"pos\":\"%d,%d\",\"tna\":\"%s\",\"tn\":%d,\"tlv\":%d,\"tpos\":\"%d,%d\",\"ws0\":%d,\"ws1\":%d,\"ws2\":%d}",
			pTargetCity->name, pTargetCity->nation, pTargetCity->level, g_armygroup[group_index].to_posx, g_armygroup[group_index].to_posy, pCity->name, pCity->nation, pCity->level, pCity->posx, pCity->posy, pTargetCity->temp_wounded_soldiers[0], pTargetCity->temp_wounded_soldiers[1], pTargetCity->temp_wounded_soldiers[2] );

		// ���������˷����ʼ�
		armygroup_mail( group_index, 2, pTargetCity, MAIL_TYPE_FIGHT_CITY, title, content, "", pFight, pTargetCity->name );

		// �¼�
		city_battle_event_add( pCity->index, CITY_BATTLE_EVENT_ASSAULT, pTargetCity->name, 0, mailid );
		city_battle_event_add( pTargetCity->index, CITY_BATTLE_EVENT_DEFEND, pCity->name, 1, mailid );
	}

	if ( pTargetCity )
	{
		// ��ǽ����
		city_guard_sendnum( pTargetCity->actor_index );

		// �Զ�����
		hero_addsoldiers_audo( pTargetCity );
	}
	return 0;
}

// ��սս���
int armygroup_vs_town( int group_index, Fight *pFight )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	int townid = g_armygroup[group_index].to_id;
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;

	i64 mailid = 0;
	char title[MAIL_TITLE_MAXSIZE] = { 0 };
	char content[MAIL_CONTENT_MAXSIZE] = { 0 };

	char attackName[64] = { 0 };
	char attackNation = 0;
	short posx = 0, posy = 0;

	// �ݵ�
	MapTown *pTown = map_town_getptr( townid );
	if ( !pTown )
		return -1;

	// ��ս������
	if ( g_armygroup[group_index].from_type == MAPUNIT_TYPE_CITY )
	{// ��ս�����������
		City *pCity = city_indexptr( g_armygroup[group_index].from_index );
		if ( !pCity )
			return -1;
		strncpy( attackName, pCity->name, NAME_SIZE );
		attackNation = pCity->nation;
		posx = pCity->posx;
		posy = pCity->posy;
	}
	else if ( g_armygroup[group_index].from_type == MAPUNIT_TYPE_TOWN )
	{ // ��ս�������ǳ���
		MapTown *pTown = map_town_getptr( g_armygroup[group_index].from_id );
		if ( !pTown )
			return -1;
		sprintf( attackName, "%s%d", TAG_TOWNID, g_armygroup[group_index].from_id );
		attackNation = pTown->nation;
		posx = g_towninfo[townid].posx;
		posy = g_towninfo[townid].posy;
	}
	else
		return -1;

	char v1[16] = { 0 };
	sprintf( v1, "%d", pTown->townid );

	if ( pFight->result == FIGHT_WIN )
	{
		// �����ɹ��ʼ�
		sprintf( title, "%s%d", TAG_TEXTID, 5027 );// ��ս����ʤ��

		// ���������˷����ʼ�
		int actorid_list[ARMYGROUP_MAXCOUNT] = { 0 };
		int actorid_count = 0;
		for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
		{
			int army_index = g_armygroup[group_index].attack_armyindex[tmpi];
			if ( army_index < 0 || army_index >= g_army_maxcount )
				continue;
			City *pArmyCity = army_getcityptr( army_index );
			if ( !pArmyCity )
				continue;

			char issend = 0;
			for ( int i = 0; i < actorid_count; i++ )
			{
				if ( actorid_list[i] == pArmyCity->actorid )
				{
					issend = 1;
					break;
				}
			}

			if ( issend == 0 )
			{
				// ����ƴ����
				sprintf( content, "{\"my\":1,\"win\":1,\"na\":\"%s\",\"n\":%d,\"pos\":\"%d,%d\",\"townid\":%d,\"tn\":%d,\"silver\":%d,\"wood\":%d,\"food\":%d,\"ws0\":%d,\"ws1\":%d,\"ws2\":%d}",
					attackName, attackNation, posx, posy, pTown->townid, pTown->nation, pArmyCity->temp_silver, pArmyCity->temp_wood, pArmyCity->temp_food, pArmyCity->temp_wounded_soldiers[0], pArmyCity->temp_wounded_soldiers[1], pArmyCity->temp_wounded_soldiers[2] );

				i64 mailid = mail( pArmyCity->actor_index, pArmyCity->actorid, MAIL_TYPE_FIGHT_NATION, title, content, "", 0, 0 );
				mail_fight( mailid, pArmyCity->actorid, pFight->unit_json );

				// ��¼
				if ( g_towninfo[townid].type >= MAPUNIT_TYPE_TOWN_XIAN && g_towninfo[townid].type <= MAPUNIT_TYPE_TOWN_ZHFD )
				{
					data_record_addvalue( pArmyCity, 2+g_towninfo[townid].type, 1 );
					data_record_addvalue( pArmyCity, DATA_RECORD_KILLTOWN, 1 );
				}

				pArmyCity->temp_silver = 0;
				pArmyCity->temp_wood = 0;
				pArmyCity->temp_food = 0;
				pArmyCity->temp_wounded_soldiers[0] = 0;
				pArmyCity->temp_wounded_soldiers[1] = 0;
				pArmyCity->temp_wounded_soldiers[2] = 0;

				actorid_list[actorid_count] = pArmyCity->actorid;
				actorid_count += 1;
			}

			// �¼�
			city_battle_event_add( pArmyCity->index, CITY_BATTLE_EVENT_NATION_ASSAULT, v1, pFight->result, mailid );
		}

		// ����ʧ���ʼ�
		sprintf( title, "%s%d", TAG_TEXTID, 5030 );// ��ս����ʧ��
		sprintf( content, "{\"my\":2,\"win\":0,\"na\":\"%s\",\"n\":%d,\"pos\":\"%d,%d\",\"townid\":%d,\"tn\":%d,\"silver\":%d,\"wood\":%d,\"food\":%d,\"ws0\":%d,\"ws1\":%d,\"ws2\":%d}",
			attackName, attackNation, posx, posy, pTown->townid, pTown->nation, 0, 0, 0, 0, 0, 0 );

		// ���������˷����ʼ�
		armygroup_mail( group_index, 2, NULL, MAIL_TYPE_FIGHT_NATION, title, content, "", pFight, v1 );
	}
	else
	{
		// ����ʧ���ʼ�
		sprintf( title, "%s%d", TAG_TEXTID, 5028 ); // ��ս����ʧ��

		// ���������˷����ʼ�
		int actorid_list[ARMYGROUP_MAXCOUNT] = { 0 };
		int actorid_count = 0;
		for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
		{
			int army_index = g_armygroup[group_index].attack_armyindex[tmpi];
			if ( army_index < 0 )
				continue;
			City *pArmyCity = army_getcityptr( army_index );
			if ( !pArmyCity )
				continue;

			char issend = 0;
			for ( int i = 0; i < actorid_count; i++ )
			{
				if ( actorid_list[i] == pArmyCity->actorid )
				{
					issend = 1;
					break;
				}
			}

			if ( issend == 0 )
			{
				// ����ƴ����
				sprintf( content, "{\"my\":1,\"win\":0,\"na\":\"%s\",\"n\":%d,\"pos\":\"%d,%d\",\"townid\":%d,\"tn\":%d,\"silver\":%d,\"wood\":%d,\"food\":%d,\"ws0\":%d,\"ws1\":%d,\"ws2\":%d}",
					attackName, attackNation, posx, posy, pTown->townid, pTown->nation, pArmyCity->temp_silver, pArmyCity->temp_wood, pArmyCity->temp_food, pArmyCity->temp_wounded_soldiers[0], pArmyCity->temp_wounded_soldiers[1], pArmyCity->temp_wounded_soldiers[2] );

				i64 mailid = mail( pArmyCity->actor_index, pArmyCity->actorid, MAIL_TYPE_FIGHT_NATION, title, content, "", 0, 0 );
				mail_fight( mailid, pArmyCity->actorid, pFight->unit_json );

				pArmyCity->temp_silver = 0;
				pArmyCity->temp_wood = 0;
				pArmyCity->temp_food = 0; 
				pArmyCity->temp_wounded_soldiers[0] = 0;
				pArmyCity->temp_wounded_soldiers[1] = 0;
				pArmyCity->temp_wounded_soldiers[2] = 0;

				actorid_list[actorid_count] = pArmyCity->actorid;
				actorid_count += 1;
			}

			// �¼�
			city_battle_event_add( pArmyCity->index, CITY_BATTLE_EVENT_NATION_ASSAULT, v1, pFight->result, mailid );
		}

		// ���سɹ��ʼ�
		sprintf( title, "%s%d", TAG_TEXTID, 5029 );// ��ս����ʤ��
		sprintf( content, "{\"my\":2,\"win\":1,\"na\":\"%s\",\"n\":%d,\"pos\":\"%d,%d\",\"townid\":%d,\"tn\":%d,\"silver\":%d,\"wood\":%d,\"food\":%d,\"ws0\":%d,\"ws1\":%d,\"ws2\":%d}",
			attackName, attackNation, posx, posy, pTown->townid, pTown->nation, 0, 0, 0, 0, 0, 0 );

		// ���������˷����ʼ�
		armygroup_mail( group_index, 2, NULL, MAIL_TYPE_FIGHT_NATION, title, content, "", pFight, v1 );

		// ������־
		nationlog_add( pTown->nation, NATION_LOG_TYPE_DEFENSE_WIN, pTown->townid, attackName, attackNation );
	}

	// �����ʼ����������
	if ( pFight->result == FIGHT_WIN )
	{
		// ϵͳ����
		char notify = 0;
		char v1[64] = { 0 };
		char v2[64] = { 0 };
		char v3[64] = { 0 };
		char v4[64] = { 0 };
		char v5[64] = { 0 };
		// 6000	{0}��{1}<color=d95df4ff><url={2}>[{2}]</url></color>��{3}���ڹ���
		// 6001	�ҹ�{ 0 }��{ 1 }<color = d95df4ff> < url = { 2 } > [{2}]< / url>< / color>��[{3}]{4}���ڹ���
		// 6002	{0}��{1}<color=d95df4ff><url={2}>[{2}]</url></color>���ҹ���{3}���ڹ��£����ҹ�ռ��ʳǳǳ��Ѵ�7�����ʷ���ռ�죬�ĳ���Ⱥ����������
		sprintf( v1, "%s%d", TAG_ZONEID, g_towninfo[townid].zoneid );
		sprintf( v2, "%s%d", TAG_TOWNID, townid );
		sprintf( v3, "%d,%d", g_towninfo[townid].posx, g_towninfo[townid].posy );
		sprintf( v4, "%s", attackName );
		sprintf( v5, "%s%d", TAG_NATION, g_map_town[townid].nation );
		system_talkjson( 0, pTown->nation, 6001, v1, v2, v3, v5, v4, NULL, 1 );

		if ( g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_XIAN )
		{
			worldquest_setvalue( WORLDQUEST_ID4, attackNation );
		}
		else if ( g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_JUN )
		{
			worldquest_setvalue( WORLDQUEST_ID5, attackNation );
		}
		else if ( g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_ZHISUO )
		{
			worldquest_setvalue( WORLDQUEST_ID6, attackNation );
		}
		else if ( g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_ZHFD )
		{// ���������أ���ôһ������ֻ����7��
			if ( nation_town_num( attackNation, MAPUNIT_TYPE_TOWN_ZHFD ) >= 7 )
			{
				system_talkjson( 0, attackNation, 6002, v1, v2, v3, v4, NULL, NULL, 1 );

				// ������־
				nationlog_add( attackNation, NATION_LOG_TYPE_ATTACK_GIVEUP, pTown->townid, attackName, pTown->nation );
				nationlog_add( pTown->nation, NATION_LOG_TYPE_DEFENSE_LOSE, pTown->townid, attackName, attackNation );
				notify = 1;
				attackNation = 0;
			}

			//// ����Ƕ���ֱ������
			//int capid = 0;
			//for ( int tmpid = 301; tmpid <= 304; tmpid++ )
			//{
			//	for ( int tmpi = 0; tmpi < 8; tmpi++ )
			//	{
			//		if ( townid == g_map_town[tmpid].pre_townid[tmpi] )
			//		{
			//			capid = tmpid;
			//			break;;
			//		}
			//	}
			//	if ( capid > 0 )
			//		break;
			//}
			//if ( capid > 0 )
			//{
			//	map_town_dev_addexp( capid, global.town_dev_occupytown );
			//}
		}
		else if ( g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_GJFD )
		{ // ����Ƕ���
			nation_capital_townid( attackNation, townid );
			nation_people_capital_set( attackNation, 0 );
			//map_town_attack_checkstart();
		}
		
		if ( notify == 0 )
		{
			system_talkjson( 0, attackNation, 6000, v1, v2, v3, v4, NULL, NULL, 1 );
			// ������־
			nationlog_add( attackNation, NATION_LOG_TYPE_ATTACK_WIN, pTown->townid, attackName, pTown->nation );
			nationlog_add( pTown->nation, NATION_LOG_TYPE_DEFENSE_LOSE, pTown->townid, attackName, attackNation );
		}

		if ( pTown->own_city_index >= 0 && pTown->own_city_index < g_city_maxcount )
		{
			g_city[pTown->own_city_index].own_townid = 0;
		}

		pTown->nation = attackNation;
		pTown->protect_sec = g_towninfo[townid].protect_maxsec;
		pTown->produce_num = 0;
		pTown->produce_sec = g_towninfo[townid].produce_maxsec;
		pTown->name[0] = 0;
		pTown->own_actorid = 0;
		pTown->own_sec = 0;
		pTown->own_city_index = -1;
		for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
		{
			pTown->join_actorid[tmpi] = 0;
			pTown->ask_actorid[tmpi] = 0;
			pTown->ask_city_index[tmpi] = -1;
		}
		int joincount = 0;
		for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
		{
			int army_index = g_armygroup[group_index].attack_armyindex[tmpi];
			if ( army_index < 0 )
				continue;
			City *pTmp = army_getcityptr( army_index );
			if ( !pTmp )
				continue;
			pTown->join_actorid[joincount++] = pTmp->actorid;

			// ��������
			nation_quest_addvalue( pTmp, NATION_QUESTKIND_TOWN, 1 );

			// ������������
			nation_rank_addvalue( pTmp, NATION_RANK_TOWN, 1 );
		}

		// ������������
		nation_mission_addvalue( pTown->nation, NATION_MISSIONKIND_TOWN, 1 );

		// �����ؾ�
		map_town_monster_reset( townid, 1 );

		// �����˿�
		nation_people_famous_calc( g_armygroup[group_index].from_nation );
		nation_people_famous_calc( g_armygroup[group_index].to_nation );

		// �������Ȧ����
		map_tile_setnation( g_towninfo[townid].posx, g_towninfo[townid].posy, g_towninfo[townid].range, townid, pTown->nation );

		// ���Ľ���������������
		if ( g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_ZHISUO || g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_LUOYANG )
		{
			map_zone_setnation( map_zone_getid( g_towninfo[townid].posx, g_towninfo[townid].posy ), pTown->nation );
		}
		// �����ʳ����򶼳Ǻ����Ǹ���
		map_zone_center_townchange( townid );

		// ����������·�ϵ��Զ�����
		for ( int tmpi = 0; tmpi < MAP_TOWN_UNDERFIRE_GROUP_MAX; tmpi++ )
		{
			int tmp_index = g_map_town[townid].underfire_groupindex[tmpi];
			if ( tmp_index < 0 || tmp_index >= g_armygroup_maxcount )
				continue;
			if ( tmp_index == group_index )
				continue;
			armygroup_delete( tmp_index );
		}
	}
	return 0;
}

// ��ս�б�
int armygroup_city_sendlist( int actor_index, int unit_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	MapUnit *unit = &g_mapunit[unit_index];
	if ( !unit )
		return -1;
	if ( unit->type != MAPUNIT_TYPE_CITY )
		return -1;
	if ( unit->index < 0 || unit->index >= g_city_maxcount )
		return -1;
	City *pTargetCity = &g_city[unit->index];
	if ( !pTargetCity )
		return -1;

	SLK_NetS_CityArmyGroupList pValue = { 0 };
	pValue.m_unit_index = unit_index;
	pValue.m_nation = pTargetCity->nation;
	
	// ���Ϳ�ʼ
	pValue.m_flag = 0;
	netsend_cityarmygrouplist_S( actor_index, SENDTYPE_ACTOR, &pValue );

	for ( int tmpi = 0; tmpi < g_armygroup_maxcount; tmpi++ )
	{
		if ( g_armygroup[tmpi].id <= 0 )
			continue;
		if ( g_armygroup[tmpi].from_type != MAPUNIT_TYPE_CITY || g_armygroup[tmpi].to_type != MAPUNIT_TYPE_CITY )
			continue;
		// ���������
		City *pAtkCity = city_indexptr( g_armygroup[tmpi].from_index );
		if ( !pAtkCity )
			continue;

		// ���������
		City *pDefCity = city_indexptr( g_armygroup[tmpi].to_index ); 
		if ( !pDefCity )
			continue;

		// �ҼȲ��ǹ�����Ҳ���Ƿ�����
		if ( pCity->nation != pAtkCity->nation && pCity->nation != pDefCity->nation )
			continue;

		if ( g_armygroup[tmpi].from_id == pTargetCity->actorid && g_armygroup[tmpi].from_nation == pCity->nation )
		{ // �ҿ��������ڹ����� �������ҵĹ���

			City *pDefCity = city_indexptr( g_armygroup[tmpi].to_index );
			if ( !pDefCity )
				continue;
			pValue.m_list[pValue.m_count].m_attack = 1;

			pValue.m_list[pValue.m_count].m_nation = pTargetCity->nation;
			pValue.m_list[pValue.m_count].m_level = city_mainlevel( pTargetCity->index );
			strncpy( pValue.m_list[pValue.m_count].m_name, pTargetCity->name, NAME_SIZE );
			pValue.m_list[pValue.m_count].m_name_length = strlen( pValue.m_list[pValue.m_count].m_name );
			pValue.m_list[pValue.m_count].m_posx = pTargetCity->posx;
			pValue.m_list[pValue.m_count].m_posy = pTargetCity->posy;
			pValue.m_list[pValue.m_count].m_actorid = pTargetCity->actorid;
			pValue.m_list[pValue.m_count].m_total = armygroup_from_totals( tmpi );

			pValue.m_list[pValue.m_count].m_t_nation = pDefCity->nation;
			pValue.m_list[pValue.m_count].m_t_level = city_mainlevel( pDefCity->index );
			strncpy( pValue.m_list[pValue.m_count].m_t_name, pDefCity->name, NAME_SIZE );
			pValue.m_list[pValue.m_count].m_t_name_length = strlen( pValue.m_list[pValue.m_count].m_t_name );
			pValue.m_list[pValue.m_count].m_t_posx = pDefCity->posx;
			pValue.m_list[pValue.m_count].m_t_posy = pDefCity->posy;
			pValue.m_list[pValue.m_count].m_t_actorid = pDefCity->actorid;
			pValue.m_list[pValue.m_count].m_t_total = armygroup_to_totals( tmpi );
		}
		else if ( g_armygroup[tmpi].to_id == pTargetCity->actorid )
		{ // �ҿ��������ڷ�����
			City *pAtkCity = city_indexptr( g_armygroup[tmpi].from_index );
			if ( !pAtkCity )
				continue;
			pValue.m_list[pValue.m_count].m_attack = 2;

			pValue.m_list[pValue.m_count].m_nation = pTargetCity->nation;
			pValue.m_list[pValue.m_count].m_level = city_mainlevel( pTargetCity->index );
			strncpy( pValue.m_list[pValue.m_count].m_name, pTargetCity->name, NAME_SIZE );
			pValue.m_list[pValue.m_count].m_name_length = strlen( pValue.m_list[pValue.m_count].m_name );
			pValue.m_list[pValue.m_count].m_posx = pTargetCity->posx;
			pValue.m_list[pValue.m_count].m_posy = pTargetCity->posy;
			pValue.m_list[pValue.m_count].m_actorid = pTargetCity->actorid;
			pValue.m_list[pValue.m_count].m_total = armygroup_to_totals( tmpi );

			pValue.m_list[pValue.m_count].m_t_nation = pAtkCity->nation;
			pValue.m_list[pValue.m_count].m_t_level = city_mainlevel( pAtkCity->index );
			strncpy( pValue.m_list[pValue.m_count].m_t_name, pAtkCity->name, NAME_SIZE );
			pValue.m_list[pValue.m_count].m_t_name_length = strlen( pValue.m_list[pValue.m_count].m_t_name );
			pValue.m_list[pValue.m_count].m_t_posx = pAtkCity->posx;
			pValue.m_list[pValue.m_count].m_t_posy = pAtkCity->posy;
			pValue.m_list[pValue.m_count].m_t_actorid = pAtkCity->actorid;
			pValue.m_list[pValue.m_count].m_t_total = armygroup_from_totals( tmpi );
		}
		else
			continue;

		pValue.m_list[pValue.m_count].m_group_index = tmpi;
		pValue.m_list[pValue.m_count].m_group_id = g_armygroup[tmpi].id;
		pValue.m_list[pValue.m_count].m_statetime = g_armygroup[tmpi].statetime;
		pValue.m_list[pValue.m_count].m_stateduration = g_armygroup[tmpi].stateduration;
		pValue.m_list[pValue.m_count].m_type = g_armygroup[tmpi].type;
		pValue.m_totalcount += 1;
		pValue.m_count += 1;
		if ( pValue.m_count > 10 )
		{
			pValue.m_flag = 1;
			netsend_cityarmygrouplist_S( actor_index, SENDTYPE_ACTOR, &pValue );
			pValue.m_count = 0;
		}
	}

	if ( pValue.m_count > 0 )
	{
		pValue.m_flag = 1;
		netsend_cityarmygrouplist_S( actor_index, SENDTYPE_ACTOR, &pValue );
		pValue.m_count = 0;
	}

	pValue.m_flag = 2;
	netsend_cityarmygrouplist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// ��ս�б�
int armygroup_town_sendlist( int actor_index, int unit_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	MapUnit *unit = &g_mapunit[unit_index];
	if ( !unit )
		return -1;
	if ( unit->type != MAPUNIT_TYPE_TOWN )
		return -1;
	int townid = unit->index;
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;

	SLK_NetS_TownArmyGroupList pValue = { 0 };
	pValue.m_unit_index = unit_index;
	pValue.m_nation = g_map_town[townid].nation;
	pValue.m_townid = townid;

	// ���Ϳ�ʼ
	pValue.m_flag = 0;
	netsend_townarmygrouplist_S( actor_index, SENDTYPE_ACTOR, &pValue );

	for ( int tmpi = 0; tmpi < g_armygroup_maxcount; tmpi++ )
	{
		if ( g_armygroup[tmpi].id <= 0 )
			continue;
		if ( g_armygroup[tmpi].to_type != MAPUNIT_TYPE_TOWN )
			continue;
		if ( g_armygroup[tmpi].to_id != townid )
			continue;

		if ( pCity->nation == g_map_town[townid].nation )
		{ // �ҿ��ľݵ������ҵĹ���

			// ��ô�����ڷ�����
			pValue.m_list[pValue.m_count].m_attack = 2; 
			pValue.m_list[pValue.m_count].m_nation = g_map_town[townid].nation;
			pValue.m_list[pValue.m_count].m_total = armygroup_to_totals( tmpi );

			// ��������Ϣ
			pValue.m_list[pValue.m_count].m_t_nation = g_armygroup[tmpi].from_nation;
			pValue.m_list[pValue.m_count].m_t_total = armygroup_from_totals( tmpi );
		}
		else if ( pCity->nation == g_armygroup[tmpi].from_nation )
		{ // �����ҹ��ҵľݵ�, �������ǲ��ǹ�������

			// ��ô�����ڹ�����
			pValue.m_list[pValue.m_count].m_attack = 1;
			pValue.m_list[pValue.m_count].m_nation = pCity->nation;
			pValue.m_list[pValue.m_count].m_total = armygroup_from_totals( tmpi );

			pValue.m_list[pValue.m_count].m_t_nation = g_armygroup[tmpi].to_nation;
			pValue.m_list[pValue.m_count].m_t_total = armygroup_to_totals( tmpi );
		}
		else
		{ // ������
			//pValue.m_list[pValue.m_count].m_attack = 3;
			//pValue.m_list[pValue.m_count].m_nation = pCity->nation;
			//pValue.m_list[pValue.m_count].m_total = armygroup_from_totals( tmpi );

			//// ��������Ϣ
			//if ( g_armygroup[tmpi].to_type == MAPUNIT_TYPE_CITY )
			//{ // �����������
			//	City *pDefCity = city_indexptr( g_armygroup[tmpi].to_index );
			//	if ( !pDefCity )
			//		continue;
			//	pValue.m_list[pValue.m_count].m_t_nation = pDefCity->nation;
			//	pValue.m_list[pValue.m_count].m_t_total = armygroup_to_totals( tmpi );
			//}
			//else if ( g_armygroup[tmpi].to_type == MAPUNIT_TYPE_TOWN )
			//{ // �������ǳ���
			//	MapTown *pTown = map_town_getptr( g_armygroup[tmpi].to_index );
			//	if ( !pTown )
			//		continue;
			//	pValue.m_list[pValue.m_count].m_t_nation = pTown->nation;
			//	pValue.m_list[pValue.m_count].m_t_total = armygroup_to_totals( tmpi );
			//}
			//else
				continue;
		}

		pValue.m_list[pValue.m_count].m_group_index = tmpi;
		pValue.m_list[pValue.m_count].m_group_id = g_armygroup[tmpi].id;
		pValue.m_list[pValue.m_count].m_statetime = g_armygroup[tmpi].statetime;
		pValue.m_list[pValue.m_count].m_stateduration = g_armygroup[tmpi].stateduration;
		pValue.m_list[pValue.m_count].m_type = g_armygroup[tmpi].type;
		pValue.m_totalcount += 1;
		pValue.m_count += 1;
		if ( pValue.m_count > 10 )
		{
			pValue.m_flag = 1;
			netsend_townarmygrouplist_S( actor_index, SENDTYPE_ACTOR, &pValue );
			pValue.m_count = 0;
		}
	}

	if ( pValue.m_count > 0 )
	{
		pValue.m_flag = 1;
		netsend_townarmygrouplist_S( actor_index, SENDTYPE_ACTOR, &pValue );
		pValue.m_count = 0;
	}

	pValue.m_flag = 2;
	netsend_townarmygrouplist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// ��ս����
int armygroup_nation_askcreate( int actor_index, int townid )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;

	char zonetype = map_zone_gettype( pCity->zone );
	char target_zonetype = map_zone_gettype( (char)g_towninfo[townid].zoneid );
	if ( zonetype == MAPZONE_TYPE_ZHOU )
	{ // �����ݵ�
		if ( pCity->zone != g_towninfo[townid].zoneid )
		{
			actor_notify_alert( pCity->actor_index, 2365 );// �㵱ǰ���ڵ����޷��Ը�����ս
			return -1;
		}
	}
	else if ( zonetype == MAPZONE_TYPE_SILI )
	{ // ����˾��
		if ( target_zonetype == MAPZONE_TYPE_SILI )
		{ // �Է�˾��
			if ( nation_official_right( pCity->official, NATION_OFFICIAL_RIGHT_FIGHT ) == 0 )
			{
				actor_notify_alert( pCity->actor_index, 2366 );// ��Ҫ��Ա����ս��Ȩ�ſɶԸõ���ս
				return -1;
			}
		}
	}

	/*if ( g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_ZHFD )
	{
		if ( nation_official_right( pCity->official, NATION_OFFICIAL_RIGHT_FIGHT ) == 0 )
		{
			actor_notify_alert( actor_index, 1852 );
			return -1;
		}
	}*/
	if ( pCity->level < global.nationfight_actorlevel )
	{
		char v1[32] = { 0 };
		sprintf( v1, "%d", global.nationfight_actorlevel );
		actor_notify_alert_v( actor_index, 1343, v1, NULL );
		return -1;
	}
	if ( g_map_town[townid].protect_sec > 0 )
	{ // ����ʱ�䣬���ɱ���ս
		actor_notify_alert( actor_index, 1379 );
		return -1;
	}
	// ������Ŀ���Ƿ��Ѿ������ҹ��Ĺ�ս
	for ( int tmpi = 0; tmpi < MAP_TOWN_UNDERFIRE_GROUP_MAX; tmpi++ )
	{
		int group_index = g_map_town[townid].underfire_groupindex[tmpi];
		if ( group_index < 0 || group_index >= g_armygroup_maxcount )
			continue;
		if ( g_armygroup[group_index].from_nation == pCity->nation )
		{
			return -1;
		}
	}
	if ( g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_ZHFD && g_map_town[townid].nation == 0 )
	{// ��������ǳأ���ôһ������ֻ����7��
		if ( nation_town_num( pCity->nation, MAPUNIT_TYPE_TOWN_ZHFD ) >= 7 )
		{
			actor_notify_alert( actor_index, 1363 );
			return -1;
		}
	}

	// ��������ս��
	int group_index = -1;
	if ( g_test_mod )
		group_index = armygroup_create( 10, MAPUNIT_TYPE_CITY, pCity->actorid, MAPUNIT_TYPE_TOWN, townid, g_towninfo[townid].fight_maxsec/5 );
	else
		group_index = armygroup_create( 10, MAPUNIT_TYPE_CITY, pCity->actorid, MAPUNIT_TYPE_TOWN, townid, g_towninfo[townid].fight_maxsec );

	if ( group_index < 0 )
		return -1;

	// ֻ�б�������ҿɼ�
	int zoneid = map_zone_getid( g_towninfo[townid].posx, g_towninfo[townid].posy );
	char v1[64] = { 0 };
	char v2[64] = { 0 };
	char v3[64] = { 0 };
	char v4[64] = { 0 };
	char v5[64] = { 0 };

	// �ҹ���{0}��{1}{2}{3}�����˹�ս������ǧ������һʱ������ӻԾ���롣���λ��������<color=#bbddf3><url={4}>[����������ս����ս]</url></color>
	sprintf( v1, "%s", pCity->name );
	sprintf( v2, "%s%d", TAG_NATION, g_map_town[townid].nation );
	sprintf( v3, "%s%d", TAG_ZONEID, g_towninfo[townid].zoneid );
	sprintf( v4, "%s%d", TAG_TOWNID, townid );
	sprintf( v5, "%d,%d", g_towninfo[townid].posx, g_towninfo[townid].posy );
	system_talkjson( 0/*zoneid*/, pCity->nation, 1294, v1, v2, v3, v4, v5, NULL, 1 );

	// �����˲��ڸõ�ͼ���
	//if ( pCity->zone != zoneid )
	//{
	//	system_talkjson_to( actor_index, 1294, v1, v2, v3, v4, v5, NULL, 1 );
	//}

	// {0}{1}���ҹ���{2}{3}�����˹�ս���й��������ҳ��Ƶ����£�������λͬ�����Ԯ�֡�<color=#bbddf3><url={4}>[����������ս����ս]</url></color>
	sprintf( v1, "%s%d", TAG_NATION, pCity->nation );
	sprintf( v2, "%s", pCity->name );
	sprintf( v3, "%s%d", TAG_ZONEID, g_towninfo[townid].zoneid );
	sprintf( v4, "%s%d", TAG_TOWNID, townid );
	sprintf( v5, "%d,%d", g_towninfo[townid].posx, g_towninfo[townid].posy );
	system_talkjson( 0/*zoneid*/, g_map_town[townid].nation, 1295, v1, v2, v3, v4, v5, NULL, 1 );
	return 0;
}

// ��ս����-AI���򴴽�
int armygroup_nation_askcreate_ai( int from_townid, int to_townid )
{
	if ( from_townid <= 0 || from_townid >= g_map_town_maxcount )
		return -1;
	if ( to_townid <= 0 || to_townid >= g_map_town_maxcount )
		return -1;
	// ������Ŀ���Ƿ��Ѿ������ҹ��Ĺ�ս
	for ( int tmpi = 0; tmpi < MAP_TOWN_UNDERFIRE_GROUP_MAX; tmpi++ )
	{
		int group_index = g_map_town[to_townid].underfire_groupindex[tmpi];
		if ( group_index < 0 || group_index >= g_armygroup_maxcount )
			continue;
		if ( g_armygroup[group_index].from_nation == g_map_town[from_townid].nation )
		{
			return -1;
		}
	}

	// ��������ս��
#ifdef _WIN32
	int group_index = armygroup_create( 20, MAPUNIT_TYPE_TOWN, from_townid, MAPUNIT_TYPE_TOWN, to_townid, g_towninfo[to_townid].fight_maxsec / 5 );
#else
	int group_index = armygroup_create( 20, MAPUNIT_TYPE_TOWN, from_townid, MAPUNIT_TYPE_TOWN, to_townid, g_towninfo[to_townid].fight_maxsec );
#endif // !_WIN32
	if ( group_index < 0 )
		return -1;

	// ֻ�б�������ҿɼ�
	int zoneid = map_zone_getid( g_towninfo[to_townid].posx, g_towninfo[to_townid].posy );
	char v1[64] = { 0 };
	char v2[64] = { 0 };
	char v3[64] = { 0 };
	char v4[64] = { 0 };
	char v5[64] = { 0 };

	// �ҹ���{0}��{1}{2}{3}�����˹�ս������ǧ������һʱ������ӻԾ���롣���λ��������<color=#bbddf3><url={4}>[����������ս����ս]</url></color>
	sprintf( v1, "%s%d", TAG_TOWNID, from_townid );
	sprintf( v2, "%s%d", TAG_NATION, g_map_town[to_townid].nation );
	sprintf( v3, "%s%d", TAG_ZONEID, g_towninfo[to_townid].zoneid );
	sprintf( v4, "%s%d", TAG_TOWNID, to_townid );
	sprintf( v5, "%d,%d", g_towninfo[to_townid].posx, g_towninfo[to_townid].posy );
	system_talkjson( 0/*zoneid*/, g_map_town[from_townid].nation, 1294, v1, v2, v3, v4, v5, NULL, 1 );

	// {0}{1}���ҹ���{2}{3}�����˹�ս���й��������ҳ��Ƶ����£�������λͬ�����Ԯ�֡�<color=#bbddf3><url={4}>[����������ս����ս]</url></color>
	sprintf( v1, "%s%d", TAG_NATION, g_map_town[from_townid].nation );
	sprintf( v2, "%s%d", TAG_TOWNID, from_townid );
	sprintf( v3, "%s%d", TAG_ZONEID, g_towninfo[to_townid].zoneid );
	sprintf( v4, "%s%d", TAG_TOWNID, to_townid );
	sprintf( v5, "%d,%d", g_towninfo[to_townid].posx, g_towninfo[to_townid].posy );
	system_talkjson( 0/*zoneid*/, g_map_town[to_townid].nation, 1295, v1, v2, v3, v4, v5, NULL, 1 );
	return group_index;
}

// ���й�ս��Ϣ
int armygroup_nation_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	Nation *pNation = nation_getptr( pCity->nation );
	if ( !pNation )
		return -1;
	SLK_NetS_TownFightList pValue = { 0 };
	for ( int tmpi = 0; tmpi < NATION_TOWN_WAR_MAX; tmpi++ )
	{
		int group_index = pNation->town_war_index[tmpi];
		if ( group_index < 0 || group_index >= g_armygroup_maxcount )
			continue;
		if ( g_armygroup[group_index].to_type != MAPUNIT_TYPE_TOWN )
			continue;
		MapTown *pTown = map_town_getptr( g_armygroup[group_index].to_id );
		if ( !pTown )
			continue;
		if ( pCity->zone != pTown->zoneid )
			continue;
		pValue.m_list[pValue.m_count].m_townid = pTown->townid;
		pValue.m_list[pValue.m_count].m_nation = pTown->nation;
		pValue.m_list[pValue.m_count].m_statetime = g_armygroup[group_index].stateduration - g_armygroup[group_index].statetime;
		if ( pTown->nation == pNation->nation )
		{ // ������
			pValue.m_list[pValue.m_count].m_attack = 2;
		}
		else
		{ // ������
			pValue.m_list[pValue.m_count].m_attack = 1;
		}
		pValue.m_count += 1;
		if ( pValue.m_count >= 200 )
		{
			break;
		}
	}
	netsend_townfightlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// �������
int armygroup_askhelp( int actor_index, int group_index, int group_id )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	if ( g_armygroup[group_index].id != group_id )
		return -1;

	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	char msg[127] = { 0 };
	if ( pCity->actorid == g_armygroup[group_index].from_id )
	{// �������ǹ�����
		if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_CITY )
		{
			// �Է��ĳǳ�
			City *pTargetCity = city_indexptr( g_armygroup[group_index].to_index );
			if ( !pTargetCity )
				return -1;
			// �Ҷ�{ 0 }��{ 1 }<color = d95df4ff><url = { 2 }>[{2}]< / url>< / color>�����˳�ս������֧Ԯ��
			snprintf( msg, 127, "{\"n\":%d,\"na\":\"%s\",\"pos\":\"%d,%d\"}", pTargetCity->nation, pTargetCity->name, pTargetCity->posx, pTargetCity->posy );
			chat_actortalk( actor_index, CHAT_CHANNEL_NATION, CHAT_MSGTYPE_ATTACK_ASKHELP, msg );
		}

	}
	else if ( pCity->actorid == g_armygroup[group_index].to_id )
	{// �������Ƿ�����
		if ( g_armygroup[group_index].from_type == MAPUNIT_TYPE_CITY )
		{
			// �Է��ĳǳ�
			City *pTargetCity = city_indexptr( g_armygroup[group_index].from_index );
			if ( !pTargetCity )
				return -1;

			// �ҵĳǳ�<color = d95df4ff><url = { 0 }>[{0}]< / url>< / color>��{ 1 }��{ 2 }�����˳�ս������֧Ԯ��
			snprintf( msg, 127, "{\"n\":%d,\"na\":\"%s\",\"pos\":\"%d,%d\"}", pTargetCity->nation, pTargetCity->name, pCity->posx, pCity->posy );
			chat_actortalk( actor_index, CHAT_CHANNEL_NATION, CHAT_MSGTYPE_DEFENSE_ASKHELP, msg );
		}
	}
	return 0;
}

// ���������˷����ʼ�
int armygroup_mail( int group_index, char attack, City *defenseCity, char type, char *title, char *content, char *attach, Fight *fight, char *name )
{
	int *pArmyIndex = NULL;
	if ( attack == 1 )
	{
		pArmyIndex = g_armygroup[group_index].attack_armyindex;
	}
	else if ( attack == 2 )
	{
		pArmyIndex = g_armygroup[group_index].defense_armyindex;
	}

	// ��Ҫ���˵���ͬ����Ҳ���
	int actorid_list[ARMYGROUP_MAXCOUNT] = { 0 };
	int actorid_count = 0;
	for ( int tmpi = 0; tmpi < ARMYGROUP_MAXCOUNT; tmpi++ )
	{
		int army_index = pArmyIndex[tmpi];
		if ( army_index < 0 )
			continue;
		City *pArmyCity = army_getcityptr( army_index );
		if ( !pArmyCity )
			continue;

		char issend = 0;
		for ( int i = 0; i < actorid_count; i++ )
		{
			if ( actorid_list[i] == pArmyCity->actorid )
			{
				issend = 1;
				break;
			}
		}

		if ( issend == 0 )
		{
			i64 mailid = mail( pArmyCity->actor_index, pArmyCity->actorid, type, title, content, "", 0, 0 );
			if ( fight )
			{
				mail_fight( mailid, pArmyCity->actorid, fight->unit_json );
				if ( type == MAIL_TYPE_FIGHT_CITY )
				{ // ��ս
					if ( attack == 1 )
					{
						city_battle_event_add( pArmyCity->index, CITY_BATTLE_EVENT_ASSAULT, name, fight->result, mailid );
					}
					else if ( attack == 2 )
					{
						city_battle_event_add( pArmyCity->index, CITY_BATTLE_EVENT_DEFEND, name, fight->result, mailid );
					}
				}
				if ( type == MAIL_TYPE_FIGHT_NATION )
				{ // ��ս
					if ( attack == 2 )
					{
						city_battle_event_add( pArmyCity->index, CITY_BATTLE_EVENT_NATION_DEFEND, name, fight->result, mailid );
						data_record_addvalue( pArmyCity, DATA_RECORD_KILLTOWN, 1 );
					}
				}
			}

			actorid_list[actorid_count] = pArmyCity->actorid;
			actorid_count += 1;
		}
	}
	
	if ( defenseCity )
	{
		i64 mailid = mail( defenseCity->actor_index, defenseCity->actorid, type, title, content, "", 0, 0 );
		if ( fight )
		{
			mail_fight( mailid, defenseCity->actorid, fight->unit_json );
			if ( type == MAIL_TYPE_FIGHT_CITY )
			{ // ��ս
				city_battle_event_add( defenseCity->index, CITY_BATTLE_EVENT_DEFEND, name, fight->result, mailid );
			}
		}
	}
	return 0;
}

// ��Ӷ��
int armygroup_fighthelper_sendlist( int actor_index, int group_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	SLK_NetS_FightHelperList pValue = { 0 };
	pValue.m_group_index = group_index;
	for ( int kind = 0; kind < ARMYGROUP_FIGHTHELPER; kind++ )
	{
		pValue.m_list[kind].m_kind = kind+1;
		pValue.m_list[kind].m_token = g_fight_helper[kind + 1].token;
		pValue.m_list[kind].m_sort = (char)g_fight_helper[kind + 1].sort;
		int monsterid = g_fight_helper[kind + 1].monsterid;
		if ( monsterid <= 0 || monsterid >= g_monster_maxnum )
			continue;
		pValue.m_list[kind].m_shape = (char)g_monster[monsterid].shape;
		pValue.m_list[kind].m_color = (char)g_monster[monsterid].color;
		pValue.m_list[kind].m_corps = (char)g_monster[monsterid].corps;
		pValue.m_list[kind].m_attack = g_monster[monsterid].attack;
		pValue.m_list[kind].m_defense = g_monster[monsterid].defense;
		pValue.m_list[kind].m_troops = g_monster[monsterid].troops;
		if ( g_armygroup[group_index].from_nation == pCity->nation )
		{ // ���ǹ�����
			pValue.m_list[kind].m_buynum = g_armygroup[group_index].attack_helpernum[kind];
		}
		else if ( g_armygroup[group_index].to_nation == pCity->nation )
		{ // ���Ƿ�����
			pValue.m_list[kind].m_buynum = g_armygroup[group_index].defense_helpernum[kind];
		}
	}
	pValue.m_count = ARMYGROUP_FIGHTHELPER;
	netsend_fighthelperlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

int armygroup_fighthelper_buy( int actor_index, int group_index, int kind )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	if ( kind <= 0 || kind > ARMYGROUP_FIGHTHELPER )
		return -1;
	if ( g_armygroup[group_index].from_nation == pCity->nation )
	{ // ���ǹ�����
		if ( g_armygroup[group_index].attack_helpernum[kind - 1] > 0 )
			return -1;
	}
	else if ( g_armygroup[group_index].to_nation == pCity->nation )
	{ // ���Ƿ�����
		if ( g_armygroup[group_index].defense_helpernum[kind - 1] > 0 )
			return -1;
	}

	int monsterid = g_fight_helper[kind].monsterid;
	if ( monsterid <= 0 || monsterid >= g_monster_maxnum )
		return -1;

	if ( actor_change_token( actor_index, -g_fight_helper[kind].token, PATH_FIGHTHELPER, 0 ) < 0 )
		return -1;

	char v1[64] = { 0 };
	char v2[64] = { 0 };
	char v3[64] = { 0 };
	char v4[64] = { 0 };
	char v5[64] = { 0 };
	char v6[64] = { 0 };

	if ( g_armygroup[group_index].to_type == MAPUNIT_TYPE_TOWN )
	{
		int townid = g_armygroup[group_index].to_index;
		if ( townid <= 0 || townid >= g_map_town_maxcount )
			return -1;

		sprintf( v1, "%s%d", TAG_ZONEID, g_towninfo[townid].zoneid );
		sprintf( v2, "%s%d", TAG_TOWNID, townid );
		sprintf( v3, "%d,%d", g_towninfo[townid].posx, g_towninfo[townid].posy );
		sprintf( v4, "%s%d", TAG_NATION, pCity->nation );
		sprintf( v5, "%s", pCity->name );
		sprintf( v6, "%s%d", TAG_MONSTERNAME, monsterid );

		if ( g_armygroup[group_index].from_nation == pCity->nation )
		{ // ���ǹ�����
			g_armygroup[group_index].attack_helpernum[kind - 1] += 1;

			// 6030	<color=03DE27FF>[{3}]{4}</color>��ļ�˹�Ӷ��<color=E80017FF>{5}</color>�������<color=D95DF4FF>{0}</color><color=FFDE00FF>{1}</color><color=25C9FFFF><url={2}>[{2}]</url></color>
			system_talkjson( 0, g_armygroup[group_index].from_nation, 6030, v1, v2, v3, v4, v5, v6, 1 );
			system_talkjson( 0, g_armygroup[group_index].to_nation, 6030, v1, v2, v3, v4, v5, v6, 1 );

		}
		else if ( g_armygroup[group_index].to_nation == pCity->nation )
		{ // ���Ƿ�����
			g_armygroup[group_index].defense_helpernum[kind - 1] += 1;

			// 6031 <color=03DE27FF>[{3}]{4}</color>��ļ�˹�Ӷ��<color=E80017FF>{5}</color>�������<color=D95DF4FF>{0}</color><color=FFDE00FF>{1}</color><color=25C9FFFF><url={2}>[{2}]</url></color>
			system_talkjson( 0, g_armygroup[group_index].from_nation, 6031, v1, v2, v3, v4, v5, v6, 1 );
			system_talkjson( 0, g_armygroup[group_index].to_nation, 6031, v1, v2, v3, v4, v5, v6, 1 );
		}

		// ֪ͨ�ͻ���UI����
		ui_update( 0, SENDTYPE_NATION + g_armygroup[group_index].from_nation, UI_UPDATE_FIGHTINFO );
		ui_update( 0, SENDTYPE_NATION + g_armygroup[group_index].to_nation, UI_UPDATE_FIGHTINFO );
	}
	else
	{

	}
	armygroup_fighthelper_sendlist( actor_index, group_index );
	return 0;
}
