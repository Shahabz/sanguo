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
#include "activity.h"
#include "system.h"
#include "army.h"
#include "army_group.h"
#include "army_march.h"
#include "army_fight.h"
#include "army_mail.h"
#include "city.h"
#include "map_town.h"
#include "map_enemy.h"
#include "map_res.h"
#include "map.h"
#include "map_zone.h"
#include "king_war.h"
#include "world_quest.h"
#include "nation.h"
#include "nation_hero.h"
#include "girl.h"

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

extern MapTownInfo *g_towninfo;
extern int g_towninfo_maxnum;

extern MapTown *g_map_town;
extern int g_map_town_maxcount;

extern MapEnemyInfo *g_enemyinfo;
extern int g_enemyinfo_maxnum;

extern MapEnemy *g_map_enemy;
extern int g_map_enemy_maxcount;

extern MapRes *g_map_res;
extern int g_map_res_maxcount;

extern Fight g_fight;

extern ArmyGroup *g_armygroup;
extern int g_armygroup_maxcount;

extern KingwarTown *g_kingwar_town;
extern int g_kingwar_town_maxcount;

Army *g_army = NULL;
int g_army_maxcount = 0;
int g_army_count = 0;
extern int g_army_maxindex;

// ������ϵĻص�
int army_loadcb( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	char hero_state = HERO_STATE_FIGHT;

	if ( g_army[army_index].to_type == MAPUNIT_TYPE_CITY )
	{ // Ŀ���ǳǳ�
		g_army[army_index].to_index = city_getindex_withactorid( g_army[army_index].to_id );
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ARMY )
	{ // Ŀ���ǲ���
		g_army[army_index].to_index = g_army[army_index].to_id;
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_TOWN )
	{ // Ŀ���ǳ���
		g_army[army_index].to_index = g_army[army_index].to_id;
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ENEMY )
	{ // Ŀ����Ұ��
		g_army[army_index].to_index = g_army[army_index].to_id;
		map_enemy_addselected( g_army[army_index].to_index );
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_RES )
	{ // Ŀ������Դ��
		g_army[army_index].to_index = g_army[army_index].to_id;
		if ( g_army[army_index].state == ARMY_STATE_GATHER )
		{
			map_res_setarmy( g_army[army_index].to_index, army_index );
		}
		hero_state = HERO_STATE_GATHER;
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_NATIONHERO )
	{ // Ŀ���ǹ�������
		g_army[army_index].to_index = g_army[army_index].to_id;
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_KINGWAR_TOWN )
	{ // Ŀ����Ѫս�ʳ�
		g_army[army_index].to_index = g_army[army_index].to_id;
		if ( g_army[army_index].state == ARMY_STATE_KINGWAR_FIGHT )
		{
			kingwar_town_queueadd( g_army[army_index].to_index, kingwar_town_attack_or_defense( g_army[army_index].to_index, army_index ), army_index );
		}
		hero_state = HERO_STATE_KINGWAR;
		if ( kingwar_activity_inttime() == 0 )
		{
			army_delete( army_index );
			return 0;
		}
	}

	if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	{ // �����ǳǳ�
		int city_index = city_getindex_withactorid( g_army[army_index].from_id );
		g_army[army_index].from_index = city_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
		{
			city_battlequeue_add( &g_city[city_index], army_index );
		}
		// �佫״̬
		// Ӣ��״̬
		for ( int tmpi = 0; tmpi < ARMY_HERO_MAX; tmpi++ )
		{
			int hero_index = city_hero_getindex( city_index, g_army[army_index].herokind[tmpi] );
			if ( hero_index >= 0 && hero_index < HERO_CITY_MAX )
			{
				if ( g_city[city_index].hero[hero_index].state > HERO_STATE_NORMAL )
					return -1;
				hero_changestate( city_index, g_army[army_index].herokind[tmpi], hero_state );
			}
		}
	}
	else if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
	{ // �����ǳ���
		g_army[army_index].from_index = g_army[army_index].from_id;
	}

	// Ŀ�������
	City *pTargetCity = army_getcityptr_target( army_index );
	if ( pTargetCity && g_army[army_index].state == ARMY_STATE_HELP )
	{ // פ���б�
		city_helparmy_add( pTargetCity, army_index );
	}

	// �ܾ���(���Ǽ����о�ʱ��ľ��룬��������֮���ֱ�߾���)
	g_army[army_index].move_total_distance = (short)sqrt( pow( (float)(g_army[army_index].from_posx - g_army[army_index].to_posx), 2 ) + pow( (float)(g_army[army_index].from_posy - g_army[army_index].to_posy), 2 ) );

	// ��ӵ���ʾ��Ԫ
	if ( g_army[army_index].state == ARMY_STATE_MARCH ||
		g_army[army_index].state == ARMY_STATE_OCCUPY ||
		g_army[army_index].state == ARMY_STATE_REBACK )
	{
		g_army[army_index].unit_index = mapunit_add( MAPUNIT_TYPE_ARMY, army_index );
	}

	// ��ӵ�ռ�ø���
	if ( g_army[army_index].state == ARMY_STATE_OCCUPY )
	{
		map_addobject( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].posx, g_army[army_index].posy );
	}

	// ����о�·��
	if ( g_army[army_index].state == ARMY_STATE_MARCH ||
		g_army[army_index].state == ARMY_STATE_REBACK )
	{
		army_marchroute_add( army_index );
	}
	g_army_count += 1;
	return 0;
}

int army_load()
{
	g_army_maxcount = g_Config.max_citycount * 4;
	g_army = (Army*)malloc( sizeof( Army ) * g_army_maxcount );
	memset( g_army, 0, sizeof( Army ) * g_army_maxcount );
	printf_msg( "Army  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_army_maxcount, (sizeof( Army )*g_army_maxcount) / 1024.0 / 1024.0, sizeof( Army ) / 1024.0 );
	army_load_auto( army_getptr, army_loadcb, "map_army" );
	return 0;
}

int army_save( FILE *fp )
{
	army_batch_save_auto( g_army, g_army_maxcount, "map_army", fp );
	return 0;
}

// ���ݿ�ɾ��һֻ����
void army_del_db( int army_index )
{
	char szSQL[256] = { 0 };
	sprintf( szSQL, "DELETE FROM `map_army` WHERE `index`='%d'", army_index );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
	}
	return;
}

inline Army *army_getptr( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return NULL;
	return &g_army[army_index];
}

Army *army_getptr_cityhero( City *pCity, int herokind )
{
	if ( !pCity )
		return NULL;
	for ( int tmpi = 0; tmpi < CITY_BATTLEQUEUE_MAX; tmpi++ )
	{
		int army_index = pCity->battle_armyindex[tmpi];
		if ( army_index >= 0 && army_index < g_army_maxcount )
		{
			for ( int i = 0; i < ARMY_HERO_MAX; i++ )
			{
				if ( g_army[army_index].herokind[i] == herokind )
				{
					return &g_army[army_index];
				}
			}
		}
	}
	return NULL;
}

// ��ʾ��Ԫ����
void army_makeunit( int army_index, SLK_NetS_AddMapUnit *pAttr )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	Army *pArmy = &g_army[army_index];
	if ( pArmy->state == ARMY_STATE_IDLE ||
		pArmy->state == ARMY_STATE_GROUP_END ||
		pArmy->state == ARMY_STATE_READY ||
		pArmy->state == ARMY_STATE_GATHER ||
		pArmy->state == ARMY_STATE_HELP )
	{
		return;
	}

	pAttr->m_state = pArmy->state;
	pAttr->m_posx = pArmy->posx;
	pAttr->m_posy = pArmy->posy;
	strncpy( pAttr->m_name, army_getname( army_index ), NAME_SIZE );
	pAttr->m_namelen = strlen( pAttr->m_name );
	char from_nation = army_getnation( army_index );
	if ( from_nation == 0 )
	{ // Ⱥ�۲��Ӳ���ʵ
		return;
	}

	// �г���λ�ú�Ŀ��λ��
	if ( pArmy->state == ARMY_STATE_MARCH || pArmy->state == ARMY_STATE_REBACK )
	{
		if ( pArmy->state == ARMY_STATE_MARCH )
		{
			// ��������
			pAttr->m_char_value[0] = pArmy->from_type;
			pAttr->m_short_value[0] = pArmy->from_posx;
			pAttr->m_short_value[1] = pArmy->from_posy;
		

			// Ŀ������
			pAttr->m_char_value[2] = pArmy->to_type;
			pAttr->m_short_value[2] = pArmy->to_posx;
			pAttr->m_short_value[3] = pArmy->to_posy;

			// ��������
			if ( pArmy->from_type == MAPUNIT_TYPE_CITY || pArmy->from_type == MAPUNIT_TYPE_ARMY || pArmy->from_type == MAPUNIT_TYPE_ENEMY || pArmy->from_type == MAPUNIT_TYPE_RES || pArmy->from_type == MAPUNIT_TYPE_NATIONHERO )
				pAttr->m_char_value[1] = 1;
			else if ( pArmy->from_type == MAPUNIT_TYPE_TOWN )
				pAttr->m_char_value[1] = map_getobject_grid( MAPUNIT_TYPE_TOWN, pArmy->from_index);

			// Ŀ�ĸ���
			if ( pArmy->to_type == MAPUNIT_TYPE_CITY || pArmy->to_type == MAPUNIT_TYPE_ARMY || pArmy->to_type == MAPUNIT_TYPE_ENEMY || pArmy->to_type == MAPUNIT_TYPE_RES || pArmy->to_type == MAPUNIT_TYPE_NATIONHERO )
				pAttr->m_char_value[3] = 1;
			else if ( pArmy->to_type == MAPUNIT_TYPE_TOWN )
				pAttr->m_char_value[3] = map_getobject_grid( MAPUNIT_TYPE_TOWN, pArmy->to_index );


		}
		// �����о��෴
		else
		{
			// ��������
			pAttr->m_char_value[0] = pArmy->to_type;
			pAttr->m_short_value[0] = pArmy->to_posx;
			pAttr->m_short_value[1] = pArmy->to_posy;

			// Ŀ������
			pAttr->m_char_value[2] = pArmy->from_type;
			pAttr->m_short_value[2] = pArmy->from_posx;
			pAttr->m_short_value[3] = pArmy->from_posy;

			// ��������
			if ( pArmy->to_type == MAPUNIT_TYPE_CITY || pArmy->to_type == MAPUNIT_TYPE_ARMY || pArmy->to_type == MAPUNIT_TYPE_ENEMY || pArmy->to_type == MAPUNIT_TYPE_RES || pArmy->to_type == MAPUNIT_TYPE_NATIONHERO )
				pAttr->m_char_value[1] = 1;
			else if ( pArmy->to_type == MAPUNIT_TYPE_TOWN )
				pAttr->m_char_value[1] = map_getobject_grid( MAPUNIT_TYPE_TOWN, pArmy->to_index );

			// Ŀ�ĸ���
			if ( pArmy->from_type == MAPUNIT_TYPE_CITY || pArmy->from_type == MAPUNIT_TYPE_ARMY || pArmy->from_type == MAPUNIT_TYPE_ENEMY || pArmy->from_type == MAPUNIT_TYPE_RES || pArmy->to_type == MAPUNIT_TYPE_NATIONHERO )
				pAttr->m_char_value[3] = 1;
			else if ( pArmy->from_type == MAPUNIT_TYPE_TOWN )
				pAttr->m_char_value[3] = map_getobject_grid( MAPUNIT_TYPE_TOWN, pArmy->from_index );

		}
		// ����
		pAttr->m_char_value[4] = from_nation;
		pAttr->m_char_value_count = 5;

		// ��Ϊ
		pAttr->m_short_value[4] = pArmy->action;
		pAttr->m_short_value_count = 5;

		// �佫
		for ( int i = 0; i < ARMY_HERO_MAX; i++ )
		{
			if ( pArmy->herokind[i] > 0 )
			{
				pAttr->m_short_value[pAttr->m_short_value_count] = pArmy->herokind[i];
				pAttr->m_short_value_count += 1;
			}
		}

		// ��������
		pAttr->m_int_value[0] = army_index;
		// �ƶ���������
		pAttr->m_int_value[1] = pArmy->statetime;
		// ��Ҫ������
		pAttr->m_int_value[2] = pArmy->stateduration;
		// ����actorid
		pAttr->m_int_value[3] = army_getcityid( army_index );
		// Ŀ��actorid
		pAttr->m_int_value[4] = army_getcityid_target( army_index );
		pAttr->m_int_value_count = 5;

	}
}


// ����λ��
void army_getpos( int army_index, short *posx, short *posy )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	*posx = g_army[army_index].posx;
	*posy = g_army[army_index].posy;
}

// ��ȡ����army_index
inline int army_getfreeindex()
{
	for ( int tmpi = 0; tmpi < g_army_maxcount; tmpi++ )
	{
		if ( g_army[tmpi].from_type <= 0 )
		{
			return tmpi;
		}
	}
	return -1;
}

// ����
int army_battle( City *pCity, SLK_NetC_MapBattle *info )
{
	if ( !pCity )
		return -1;
	char hero_state = HERO_STATE_FIGHT;
	int group_index = -1;
	// �ܱ���
	int totalsoldiers = 0;
	// �Զ�����
	int autohp = city_function_check( pCity, CITY_FUNCTION_BATTLE_ADDHP );
	// Ӣ��״̬���
	for ( int tmpi = 0; tmpi < ARMY_HERO_MAX; tmpi++ )
	{
		int hero_index = city_hero_getindex( pCity->index, info->m_herokind[tmpi] );
		if ( hero_index >= 0 && hero_index < HERO_CITY_MAX )
		{
			if ( pCity->hero[hero_index].state > HERO_STATE_NORMAL )
				return -1;

			if ( autohp == 1 )
			{
				int troops = pCity->hero[hero_index].troops;
				int add = troops - pCity->hero[hero_index].soldiers;
				if ( add > 0 )
				{
					HeroInfoConfig *config = hero_getconfig( info->m_herokind[tmpi], pCity->hero[hero_index].color );
					if ( config )
					{
						int has = city_soldiers( pCity->index, config->corps );
						if ( has > 0 )
						{
							if ( add > has )
								add = has;
							hero_changesoldiers( pCity, &pCity->hero[hero_index], add, PATH_BATTLEAUTOHP );
							city_changesoldiers( pCity->index, config->corps, -add, PATH_BATTLEAUTOHP );
						}
					}
				}
			}
			totalsoldiers += pCity->hero[hero_index].soldiers;
		}
	}

	// �о�����
	int distance = abs( pCity->posx - info->m_to_posx ) + abs( pCity->posy - info->m_to_posy );
	// �о�ʱ��
	int marchtime = army_marchtime_calc( pCity, pCity->posx, pCity->posy, info->m_to_posx, info->m_to_posy );
	// ������ʳ
	int cost_food = (int)ceil( global.army_march_food_v1*totalsoldiers + global.army_march_food_v2*(marchtime*marchtime) + global.army_march_food_v3*marchtime );

	char to_type = 0;
	int to_id = 0;
	if ( info->m_to_unit_index >= 0 && info->m_to_unit_index < g_mapunit_maxcount )
	{ // ��������ʵ��
		if ( g_mapunit[info->m_to_unit_index].type == MAPUNIT_TYPE_CITY )
		{ // �ǳ�
			int city_index = g_mapunit[info->m_to_unit_index].index;
			if ( city_index < 0 || city_index >= g_city_maxcount )
				return -1;
			City *pTargetCity = &g_city[city_index];
			if ( !pTargetCity )
				return -1;

			//1���������˾�����������û���κ�����
			//2����������ݵأ���������������
			if ( pCity->zone != MAPZONE_CENTERID && pCity->zone != pTargetCity->zone )
			{// ����˾����Ҳ�����Լ������ݣ���ô��Ҫ�ж�;
				if ( map_zone_ismovezone( pCity->zone, pTargetCity->zone ) == 0 )
				{
					actor_notify_alert( pCity->actor_index, 2364 );// �㵱ǰ���ڵ����޷�������ݵĳǳ�
					return -1;
				}
			}

			if ( info->m_action == ARMY_ACTION_HELP_TROOP )
			{
				int num = city_helparmy_getnum( pTargetCity );
				if ( num >= CITY_HELPDEFENSE_MAX || num >= city_helparmy_maxnum( pTargetCity ) )
				{
					actor_notify_pop( pCity->actor_index, 1241 ); // �ɱ�פ���Ķ�������
					return -1;
				}
			}

			if ( info->m_action == ARMY_ACTION_GROUP_CREATE )
			{// ��������
				// ��������Ϣ����
				if ( city_underfire_getnum( pTargetCity ) >= CITY_UNDERFIRE_MAX )
				{
					write_gamelog( "[BATTLE_FAILED_UNDERFIRE_FULL]_cityid:%d_action:%d_tocityid:%d", pCity->actorid, info->m_action, pTargetCity->actorid );
					//return -1;
				}

				// ս���ػ�״̬���
				if ( pTargetCity->ptsec > 0 )
				{
					actor_notify_pop( pCity->actor_index, 1248 );
					return -1;
				}

				// ���Ѽ��
				if ( pCity->nation == pTargetCity->nation )
				{
					return -1;
				}

				// ��������
				int grouptype = info->m_appdata;
				int stateduration = 0;
				int cost_body = 0;
				int cost_item = 0;
				if ( grouptype == 1 )
				{// ����
					if ( marchtime > global.cityfight_sec_limit[0] )
						return -1;
					stateduration = marchtime;
					cost_body = global.cityfight_body_cost[0];
					cost_item = global.cityfight_item_cost[0];
				}
				else if ( grouptype == 2 )
				{ // ��Ϯ
					if ( marchtime > global.cityfight_sec_limit[1] )
						return -1;
					if ( marchtime < global.cityfight_sec_group[1] )
					{
						stateduration = global.cityfight_sec_group[1] + marchtime;
					}
					else
					{
						stateduration = marchtime;
					}
					cost_body = global.cityfight_body_cost[1];
					cost_item = global.cityfight_item_cost[1];
				}
				else if ( grouptype == 3 )
				{ // Զ��
					if ( marchtime > global.cityfight_sec_limit[2] )
						return -1;
					if ( marchtime < global.cityfight_sec_group[2] )
					{
						stateduration = global.cityfight_sec_group[2] + marchtime;
					}
					else
					{
						stateduration = marchtime;
					}
					cost_body = global.cityfight_body_cost[2];
					cost_item = global.cityfight_item_cost[2];
				}
				else
					return -1;

				// ��Ա������
				if ( pCity->official > 0 )
				{
					cost_body -= nation_official_right( pCity->official, NATION_OFFICIAL_RIGHT_CITYFIGHT_BODY );
					if ( cost_body < 0 )
						cost_body = 0;
				}

				// ����
				if ( cost_body > 0 )
				{
					int itemkind = 486;
					if ( item_lost( pCity->actor_index, itemkind, cost_item, PATH_FIGHT ) < 0 )
					{
						if ( pCity->body < cost_body )
							return -1;
						city_changebody( pCity->index, -cost_body, PATH_FIGHT );
					}
				}

				// ��������ս��
				group_index = armygroup_create( grouptype, MAPUNIT_TYPE_CITY, pCity->actorid, MAPUNIT_TYPE_CITY, pTargetCity->actorid, stateduration );
				if ( group_index < 0 )
					return -1;

				// ������
				city_changeprotect( pCity->index, -pCity->ptsec, PATH_FIGHT );
			}
			else if ( info->m_action == ARMY_ACTION_GROUP_ATTACK )
			{// ���뼯�����
				// ���Ѽ��
				if ( pCity->nation == pTargetCity->nation )
				{
					return -1;
				}

				if ( info->m_group_index < 0 || info->m_group_index >= g_armygroup_maxcount )
				{
					return -1;
				}

				if ( g_armygroup[info->m_group_index].id != info->m_id )
				{
					return -1;
				}

				if ( g_armygroup[info->m_group_index].type == 1 )
				{ 
					// ����ս�޷��������
					actor_notify_alert( pCity->actor_index, 1267 );
					return -1;
				}
				if ( marchtime >= (g_armygroup[info->m_group_index].stateduration - g_armygroup[info->m_group_index].statetime) )
				{
					// �˷�ǰȥ��������սʱ�䣬���Ǿ�������
					actor_notify_alert( pCity->actor_index, 1268 );
					return -1;
				}
				group_index = info->m_group_index;

				// ������
				city_changeprotect( pCity->index, -pCity->ptsec, PATH_FIGHT );

				// �Ѿ������ս��������
				actor_notify_alert( pCity->actor_index, 1269 );
			}
			else if ( info->m_action == ARMY_ACTION_GROUP_DEFENSE )
			{// ���뼯�����
				// ���Ѽ��
				if ( pCity->nation != pTargetCity->nation )
				{
					return -1;
				}
				if ( info->m_group_index < 0 || info->m_group_index >= g_armygroup_maxcount )
				{
					return -1;
				}

				if ( g_armygroup[info->m_group_index].id != info->m_id )
				{
					return -1;
				}

				if ( marchtime >= (g_armygroup[info->m_group_index].stateduration - g_armygroup[info->m_group_index].statetime) )
				{
					// �˷�ǰȥ��������սʱ�䣬���Ǿ�������
					actor_notify_alert( pCity->actor_index, 1268 );
					return -1;
				}
				group_index = info->m_group_index;

				// �Ѿ������ս���ز���
				actor_notify_alert( pCity->actor_index, 1270 );
			}

			to_type = MAPUNIT_TYPE_CITY;
			to_id = pTargetCity->actorid;
		}
		else if ( g_mapunit[info->m_to_unit_index].type == MAPUNIT_TYPE_ARMY )
		{ // ����
			int army_index = g_mapunit[info->m_to_unit_index].index;
			if ( army_index < 0 || army_index >= g_army_maxcount )
				return -1;
			if ( g_army[army_index].state != ARMY_STATE_OCCUPY )
				return -1;

			// ��ȡ��ֻ�����������л���NPC
			if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
			{
				int city_index = g_army[army_index].from_index;
				if ( city_index < 0 || city_index >= g_city_maxcount )
					return -1;
				City *pTargetCity = &g_city[city_index];
				if ( !pTargetCity )
					return -1;

				// ��������Ϣ����
				if ( city_underfire_getnum( pTargetCity ) >= CITY_UNDERFIRE_MAX )
				{
					write_gamelog( "[BATTLE_FAILED_UNDERFIRE_FULL]_cityid:%d_action:%d_tocityid:%d", pCity->actorid, info->m_action, pTargetCity->actorid );
					return -1;
				}

				// ���Ѽ��
				if ( pCity->nation == pTargetCity->nation )
				{
					return -1;
				}
			}
			else if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
			{
				int townid = g_army[army_index].from_id;
			}
			city_changeprotect( pCity->index, -pCity->ptsec, PATH_FIGHT );
			to_type = MAPUNIT_TYPE_ARMY;
			to_id = army_index;

		}
		else if ( g_mapunit[info->m_to_unit_index].type == MAPUNIT_TYPE_TOWN )
		{ // ����
			int townid = g_mapunit[info->m_to_unit_index].index;
			if ( townid <= 0 || townid >= g_map_town_maxcount )
				return -1;
			MapTown *pTown = map_town_getptr( townid );
			if ( !pTown )
				return -1;

			//char zonetype = map_zone_gettype( pCity->zone );
			//char target_zonetype = map_zone_gettype( pTown->zoneid );
			//if ( zonetype == MAPZONE_TYPE_ZHOU )
			//{ // �����ݵ�
			//	if ( pCity->zone != pTown->zoneid )
			//	{
			//		actor_notify_alert( pCity->actor_index, 2364 );// �㵱ǰ���ڵ����޷�������ݵĳǳ�
			//		return -1;
			//	}
			//}
			//else if ( zonetype == MAPZONE_TYPE_SILI )
			//{ // ����˾��
			//	if ( target_zonetype != MAPZONE_TYPE_SILI )
			//	{ // �Է������ݳ�
			//		if ( map_zone_ismovezone( pCity->zone, pTown->zoneid ) == 0 )
			//		{
			//			actor_notify_alert( pCity->actor_index, 2364 );// �㵱ǰ���ڵ����޷�������ݵĳǳ�
			//			return -1;
			//		}
			//	}
			//}

			if ( g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_GJFD )
			{
				if ( pTown->nation > 0 )
				{// ���ҷ�أ������᷸
					actor_notify_alert( pCity->actor_index, 1333 );
					return -1;
				}
			}

			if ( info->m_action == ARMY_ACTION_NATION_ATTACK )
			{ // �����ս����
				if ( pCity->level < global.nationfight_actorlevel )
				{
					char v1[32] = { 0 };
					sprintf( v1, "%d", global.nationfight_actorlevel );
					actor_notify_alert_v( pCity->actor_index, 1344, v1, NULL );
					return -1;
				}
				if ( pCity->nation == pTown->nation )
				{
					return -1;
				}

				if ( info->m_group_index < 0 || info->m_group_index >= g_armygroup_maxcount )
				{
					return -1;
				}

				if ( g_armygroup[info->m_group_index].id != info->m_id )
				{
					return -1;
				}

				if ( marchtime >= (g_armygroup[info->m_group_index].stateduration - g_armygroup[info->m_group_index].statetime) )
				{
					// �˷�ǰȥ��������սʱ�䣬���Ǿ�������
					actor_notify_alert( pCity->actor_index, 1268 );
					return -1;
				}
				group_index = info->m_group_index;

				// ������
				city_changeprotect( pCity->index, -pCity->ptsec, PATH_FIGHT );

				// �Ѿ������ս��������
				actor_notify_alert( pCity->actor_index, 1298 );
			}
			else if ( info->m_action == ARMY_ACTION_NATION_DEFENSE )
			{ // �����ս����
				if ( pCity->level < global.nationfight_actorlevel )
				{
					char v1[32] = { 0 };
					sprintf( v1, "%d", global.nationfight_actorlevel );
					actor_notify_alert_v( pCity->actor_index, 1344, v1, NULL );
					return -1;
				}
				// ���Ѽ��
				if ( pCity->nation != pTown->nation )
				{
					return -1;
				}
				if ( info->m_group_index < 0 || info->m_group_index >= g_armygroup_maxcount )
				{
					return -1;
				}

				if ( g_armygroup[info->m_group_index].id != info->m_id )
				{
					return -1;
				}

				if ( marchtime >= (g_armygroup[info->m_group_index].stateduration - g_armygroup[info->m_group_index].statetime) )
				{
					// �˷�ǰȥ��������սʱ�䣬���Ǿ�������
					actor_notify_alert( pCity->actor_index, 1268 );
					return -1;
				}
				group_index = info->m_group_index;

				// �Ѿ������ս���ز���
				actor_notify_alert( pCity->actor_index, 1299 );
			}
			to_type = MAPUNIT_TYPE_TOWN;
			to_id = g_mapunit[info->m_to_unit_index].index;
		}
		else if ( g_mapunit[info->m_to_unit_index].type == MAPUNIT_TYPE_ENEMY )
		{ // Ұ��
			MapEnemy *enemy = map_enemy_getptr( g_mapunit[info->m_to_unit_index].index );
			if ( !enemy )
				return -1;
			if ( pCity->zone != map_zone_getid( enemy->posx, enemy->posy ) )
			{// ����ͬһ����
				actor_notify_alert( pCity->actor_index, 1322 );
				return -1;
			}
			MapEnemyInfo *config = map_enemy_getconfig( enemy->kind );
			if ( !config )
				return -1;
			if ( config->level > pCity->mokilllv + 1 )
				return -1;
			// ������ʳ
			if ( cost_food > pCity->food )
				return -1;
			city_changefood( pCity->index, -cost_food, PATH_MARCH );
			
			// ��ѡ������
			map_enemy_addselected( g_mapunit[info->m_to_unit_index].index );

			to_type = MAPUNIT_TYPE_ENEMY;
			to_id = g_mapunit[info->m_to_unit_index].index;
		}
		else if ( g_mapunit[info->m_to_unit_index].type == MAPUNIT_TYPE_RES )
		{ // ��Դ��
			// ���ɼ�����������ֻ����4��
			int gather_count = 0;
			for ( int tmpi = 0; tmpi < CITY_BATTLEQUEUE_MAX; tmpi++ )
			{
				int army_index = pCity->battle_armyindex[tmpi];
				if ( army_index < 0 || army_index >= g_army_maxcount )
					continue;
				if ( g_army[army_index].action == ARMY_ACTION_GATHER )
				{
					gather_count += 1;
				}
			}
			if ( gather_count >= 4 )
			{
				actor_notify_alert( pCity->actor_index, 2413 );
				return -1;
			}
			MapRes *res = map_res_getptr( g_mapunit[info->m_to_unit_index].index );
			if ( !res )
				return -1;
			if ( pCity->zone != map_zone_getid( res->posx, res->posy ) )
			{// ����ͬһ����
				actor_notify_alert( pCity->actor_index, 1322 );
				return -1;
			}
			MapResInfo *config = map_res_getconfig( res->kind );
			if ( !config )
				return -1;
			short tile_townid = map_tile_gettownid( res->posx, res->posy );
			if ( tile_townid > 0 && tile_townid < g_map_town_maxcount )
			{
				if ( g_towninfo[tile_townid].range_gather == 1 )
				{
					char tile_nation = map_tile_getnation( res->posx, res->posy );
					if ( tile_nation > 0 && tile_nation != pCity->nation )
					{ // �Ǳ���ռ�����򣬲���óȻǰ��
						actor_notify_alert( pCity->actor_index, 1323 );
						return -1;
					}
				}
			}
			
			// �佫�ȼ����
			int hero_index = city_hero_getindex( pCity->index, info->m_herokind[0] );
			if ( hero_index < 0 || hero_index >= HERO_CITY_MAX )
				return -1;
			if ( pCity->hero[hero_index].level < config->herolevel )
			{ // ��Ҫ�佫�ȼ�{0}��
				char v1[16] = { 0 };
				sprintf( v1, "%d", config->herolevel );
				actor_notify_alert_v( pCity->actor_index, 2411, v1, NULL );
				return -1;
			}

			// ������ʳ
			if ( cost_food > pCity->food )
				return -1;

			// ����Ѿ�����ռ��
			int army_index = map_res_getarmy( g_mapunit[info->m_to_unit_index].index );
			if ( army_index >= 0 )
			{
				if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY && g_army[army_index].from_index >= 0 && g_army[army_index].from_index < g_city_maxcount )
				{
					// ���Ѽ��
					int city_index = g_army[army_index].from_index;
					if ( pCity->nation == g_city[city_index].nation )
					{
						actor_notify_alert( pCity->actor_index, 2412 );
						return -1;
					}
					city_changeprotect( pCity->index, -pCity->ptsec, PATH_FIGHT );
				}
			}
			city_changefood( pCity->index, -cost_food, PATH_MARCH );
			to_type = MAPUNIT_TYPE_RES;
			to_id = g_mapunit[info->m_to_unit_index].index;
			hero_state = HERO_STATE_GATHER;
		}
		else if ( g_mapunit[info->m_to_unit_index].type == MAPUNIT_TYPE_NATIONHERO )
		{ // ��������
			//if ( pCity->zone != map_zone_getid( info->m_to_posx, info->m_to_posy ) )
			//{// ����ͬһ����
			//	actor_notify_alert( pCity->actor_index, 1322 );
			//	return -1;
			//}
			// �Ƿ񳬹�4������
			if ( nation_hero_checklimit( pCity ) == 0 )
			{
				actor_notify_alert( pCity->actor_index, 2348 );
				return -1;
			}
			// ������ʳ
			if ( cost_food > pCity->food )
				return -1;
			city_changefood( pCity->index, -cost_food, PATH_MARCH );

			to_type = MAPUNIT_TYPE_NATIONHERO;
			to_id = g_mapunit[info->m_to_unit_index].index;
		}
	}
	else
	{	
		if ( info->m_action == ARMY_ACTION_KINGWAR )
		{ // �ʳ�Ѫս�ݵ�
			if ( pCity->level < global.nationfight_actorlevel )
			{
				char v1[32] = { 0 };
				sprintf( v1, "%d", global.nationfight_actorlevel );
				actor_notify_alert_v( pCity->actor_index, 1390, v1, NULL );
				return -1;
			}
			if ( pCity->zone != MAPZONE_CENTERID )
			{
				actor_notify_alert( pCity->actor_index, 1391 );
				return -1;
			}
			to_type = MAPUNIT_TYPE_KINGWAR_TOWN;
			to_id = pCity->nation;
			hero_state = HERO_STATE_KINGWAR;
		}
		// פ���յ�
		else if ( info->m_action != ARMY_ACTION_OCCUPY )
		{
			write_gamelog( "[BATTLE_INVALID]_actorid:%d_action:%d_info->unit_index:%d", pCity->actorid, info->m_action, info->m_to_unit_index );
			actor_notify_pop( pCity->actor_index, 450 );  // Ŀ���Ѿ���ʧ
			return -1;
		}
		else
		{
			to_type = 0;
			to_id = 0;
		}
	}

	// ��������
	int army_index;
	if ( to_type == MAPUNIT_TYPE_KINGWAR_TOWN )
	{
		army_index = army_create( MAPUNIT_TYPE_CITY, pCity->actorid, to_type, to_id, ARMY_STATE_KINGWAR_READY, info );
		if ( army_index >= 0 )
		{
			// Ů�����Ƕ�
			Hero *pHero = city_hero_getptr_withkind( pCity->index, g_army[army_index].herokind[0] );
			if ( pHero && pHero->girlkind > 0 )
			{
				girl_addloveexp_kingwar( pCity, pHero->kind, pHero->girlkind );
			}
		}
	}
	else
	{
		army_index = army_create( MAPUNIT_TYPE_CITY, pCity->actorid, to_type, to_id, ARMY_STATE_MARCH, info );
	}
	if ( army_index < 0 )
		return -1;

	// �ܱ���
	g_army[army_index].totals = totalsoldiers;

	// Ӣ��״̬����
	for ( int tmpi = 0; tmpi < ARMY_HERO_MAX; tmpi++ )
	{
		int hero_index = city_hero_getindex( pCity->index, info->m_herokind[tmpi] );
		if ( hero_index >= 0 && hero_index < HERO_CITY_MAX )
		{
			if ( pCity->hero[hero_index].state > HERO_STATE_NORMAL )
				continue;
			hero_changestate( pCity->index, info->m_herokind[tmpi], hero_state );
		}
	}

	// ���𼯽�,���öӳ�
	if ( group_index >= 0 && group_index < g_armygroup_maxcount )
	{
		if ( info->m_action == ARMY_ACTION_GROUP_CREATE )
		{
			armygroup_setleader( group_index, army_index );
		}
		else
		{
			g_army[army_index].group_index = group_index;
			g_army[army_index].group_id = g_armygroup[group_index].id;
		}
		// ���뼯���б�
		armygroup_addarmy( army_index, map_zone_getid( g_armygroup[group_index].to_posx, g_armygroup[group_index].to_posy ) );
	}

	// ��ӵ���������
	if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	{
		city_battlequeue_add( army_getcityptr( army_index ), army_index );
	}

	// Ŀ�������
	City *pTargetCity = army_getcityptr_target( army_index );
	if ( pTargetCity )
	{
		if ( info->m_action == ARMY_ACTION_FIGHT )
		{// �������б�
			city_underfire_add( pTargetCity, army_index );
		}
	}
	return army_index;
}

// ����һֻ����
int army_create( char from_type, int from_id, char to_type, int to_id, char stat, SLK_NetC_MapBattle *info )
{
	int army_index = army_getfreeindex();
	if ( army_index < 0 )
		return -1;
	// Ϊ�˱�����Ч�ʣ�������������
	if ( army_index >= g_army_maxindex )
	{
		g_army_maxindex = army_index + 1;
	}

	memset( &g_army[army_index], 0, sizeof( Army ) );
	g_army[army_index].index = army_index;
	g_army[army_index].from_index = -1;
	g_army[army_index].to_index = -1;
	g_army[army_index].unit_index = -1;
	g_army[army_index].group_index = -1;
	g_army[army_index].group_pos = -1;
	g_army_count += 1;

	// ��������
	g_army[army_index].from_type = from_type;
	g_army[army_index].from_id = from_id;
	for ( int tmpi = 0; tmpi < ARMY_HERO_MAX; tmpi++ )
	{
		g_army[army_index].herokind[tmpi] = info->m_herokind[tmpi];
	}
	if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	{ // �����ǳǳ�
		int city_index = city_getindex_withactorid( g_army[army_index].from_id );
		g_army[army_index].from_index = city_index;
		city_getpos( city_index, &g_army[army_index].from_posx, &g_army[army_index].from_posy );
	}
	else if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
	{ // �����ǳ���
		int index = g_army[army_index].from_id;
		g_army[army_index].from_index = index;
		map_town_getpos( index, &g_army[army_index].from_posx, &g_army[army_index].from_posy );
	}

	// Ŀ������
	g_army[army_index].to_type = to_type;
	g_army[army_index].to_id = to_id;
	if ( g_army[army_index].to_type == MAPUNIT_TYPE_CITY )
	{ // Ŀ���ǳǳ�
		int city_index = city_getindex_withactorid( g_army[army_index].to_id );
		g_army[army_index].to_index = city_index;
		city_getpos( city_index, &g_army[army_index].to_posx, &g_army[army_index].to_posy );
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ARMY )
	{ // Ŀ���ǲ���
		int target_armyindex = g_army[army_index].to_id;
		g_army[army_index].to_index = target_armyindex;
		if ( target_armyindex < 0 || target_armyindex >= g_army_maxcount )
			return -1;
		g_army[army_index].to_posx = g_army[target_armyindex].posx;
		g_army[army_index].to_posy = g_army[target_armyindex].posy;
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_TOWN )
	{ // Ŀ���ǳ���
		int index = g_army[army_index].to_id;
		g_army[army_index].to_index = index;
		map_town_getpos( index, &g_army[army_index].to_posx, &g_army[army_index].to_posy );
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ENEMY )
	{ // Ŀ��������
		int index = g_army[army_index].to_id;
		g_army[army_index].to_index = index;
		//MapMonsterConfig *config = map_monster_getconfig( g_army[army_index].to_index );
		//if ( config )
		//	g_army[army_index].target_mark = config->monsterid;
		map_enemy_getpos( index, &g_army[army_index].to_posx, &g_army[army_index].to_posy );
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_RES )
	{ // Ŀ������Դ��
		int index = g_army[army_index].to_id;
		g_army[army_index].to_index = index;
		map_res_getpos( index, &g_army[army_index].to_posx, &g_army[army_index].to_posy );
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_NATIONHERO )
	{ // Ŀ���ǹ�������
		int index = g_army[army_index].to_id;
		g_army[army_index].to_index = index;
		nation_hero_getpos( index, &g_army[army_index].to_posx, &g_army[army_index].to_posy );
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_KINGWAR_TOWN )
	{ // Ŀ����Ѫս�ʳǾݵ�
		int index = g_army[army_index].to_id;
		g_army[army_index].to_index = index;
	}
	else
	{ // Ŀ���ǿյ�
		g_army[army_index].to_posx = info->m_to_posx;
		g_army[army_index].to_posy = info->m_to_posy;
	}

	g_army[army_index].id = (int)time( NULL );
	g_army[army_index].state = stat;
	g_army[army_index].statetime = 0;
	g_army[army_index].action = info->m_action;
	g_army[army_index].appdata = info->m_appdata;
	g_army[army_index].posx = g_army[army_index].from_posx;
	g_army[army_index].posy = g_army[army_index].from_posy;

	if ( g_army[army_index].to_type != MAPUNIT_TYPE_KINGWAR_TOWN )
	{
		// ����(���Ǽ����о�ʱ��ľ��룬��������֮���ֱ�߾���)
		g_army[army_index].move_total_distance = (short)sqrt( pow( (float)(g_army[army_index].from_posx - g_army[army_index].to_posx), 2 ) + pow( (float)(g_army[army_index].from_posy - g_army[army_index].to_posy), 2 ) );

		// �����о�ʱ��
		army_march_time( army_index );

		// �о�·��
		army_marchroute_add( army_index );

		// ��ӵ���ͼ��ʾ��Ԫ
		g_army[army_index].unit_index = mapunit_add( MAPUNIT_TYPE_ARMY, army_index );
	}
	return army_index;
}

// ɾ��һ֧����
void army_delete( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	if ( g_army[army_index].from_type <= 0 )
		return;
	mapunit_del( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].unit_index );
	if ( g_army[army_index].state == ARMY_STATE_OCCUPY )
	{
		map_delobject( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].posx, g_army[army_index].posy );
	}
	else if ( g_army[army_index].state == ARMY_STATE_FIGHT && g_army[army_index].to_type == MAPUNIT_TYPE_ENEMY )
	{
		// ��ѡ���������
		map_enemy_subselected( g_army[army_index].to_index );
	}
	else if ( g_army[army_index].state == ARMY_STATE_KINGWAR_FIGHT )
	{
		kingwar_town_queuedel( g_army[army_index].to_index, kingwar_town_attack_or_defense( g_army[army_index].to_index, army_index ), army_index );
	}

	army_marchroute_del( army_index );
	city_underfire_del_equal( army_getcityptr( army_index ), army_index );
	city_underfire_del( army_getcityptr_target( army_index ), army_index );
	city_helparmy_del( army_getcityptr_target( army_index ), army_index );
	armygroup_delarmy( army_index, map_zone_getid( g_army[army_index].posx, g_army[army_index].posy ) );

	if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	{
		// ɾ��֮ǰ��Ҫ�����Ӻ�Я������Դ��ԭ���ǳ���
		army_tocity( army_index );

		// ɾ����������
		city_battlequeue_del( army_getcityptr( army_index ), army_index );

		// �ʼ�֪ͨ
		if ( g_army[army_index].action == ARMY_ACTION_GATHER )
		{
			// ����Ϊ�������ٻص����
			if ( g_army[army_index].to_type == MAPUNIT_TYPE_RES && g_army[army_index].state == ARMY_STATE_GATHER )
			{
				int index = g_army[army_index].to_index;
				if ( index >= 0 && index < g_map_res_maxcount && g_map_res[index].army_index == army_index )
				{
					map_res_setarmy( index, -1 );
				}
			}

			army_mail_gather( army_index );
		}
		else if ( g_army[army_index].action == ARMY_ACTION_GROUP_CREATE || g_army[army_index].action == ARMY_ACTION_GROUP_ATTACK )
		{ // ��鼯���Ƿ�ﵽ��ɢ����
			armygroup_dismiss( army_index );
		}
		else if ( g_army[army_index].action == ARMY_ACTION_KINGWAR )
		{ // ɾ����������а���ɱ
			kingwar_rankcalc( army_index );
		}
	}
	else if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
	{
		// ɾ������ĳ�����������
		//map_town_battle_armyindex_del( g_army[army_index].from_index );
	}
	army_del_db( army_index );
	memset( &g_army[army_index], 0, sizeof( Army ) );
	g_army[army_index].index = -1;
	g_army[army_index].from_id = -1;
	g_army[army_index].from_index = -1;
	g_army[army_index].to_id = -1;
	g_army[army_index].to_index = -1;
	g_army[army_index].unit_index = -1;
	g_army[army_index].group_index = -1;
	g_army[army_index].group_pos = -1;
	g_army_count -= 1;
}

// ������Ϣ��ԭ���ǳ�
int army_tocity( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	City *pCity = army_getcityptr( army_index );
	if ( !pCity )
		return -1;

	// Ӣ��״̬
	int value[ARMY_HERO_MAX] = { 0 };
	for ( int tmpi = 0; tmpi < ARMY_HERO_MAX; tmpi++ )
	{
		hero_changestate( pCity->index, g_army[army_index].herokind[tmpi], HERO_STATE_NORMAL );
		value[tmpi] = g_army[army_index].herokind[tmpi];
	}
	if ( pCity->actor_index >= 0 )
	{
		actor_notify_value( pCity->actor_index, NOTIFY_HEROBACK, ARMY_HERO_MAX, value, NULL );
	}
	
	// ;��
	char path = PATH_SYSTEM;
	if ( g_army[army_index].action == ARMY_ACTION_GATHER )
	{
		path = PATH_GATHER;
	}
	else if ( g_army[army_index].action == ARMY_ACTION_FIGHT )
	{
		path = PATH_FIGHT;
	}
	// Я����Դ��ԭ��������
	city_changesilver( pCity->index, g_army[army_index].silver, path );
	city_changewood( pCity->index, g_army[army_index].wood, path );
	city_changefood( pCity->index, g_army[army_index].food, path );
	city_changeiron( pCity->index, g_army[army_index].iron, path );

	// ���Я����ʯ
	int token = g_army[army_index].token;
	if ( token > 0 )
	{
		if ( pCity->actor_index < 0 || pCity->actor_index >= g_maxactornum )
		{ // ������
			gift( pCity->actorid, AWARDKIND_TOKEN, token, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, path );
		}
		else
		{ // ����
			actor_change_token( pCity->actor_index, token, path, 0 );
		}
	}

	// ���Я������
	for ( int tmpi = 0; tmpi < ARMY_CARRY_ITEMMAX; tmpi++ )
	{
		int awardkind = g_army[army_index].awardkind[tmpi];
		int awardnum = g_army[army_index].awardnum[tmpi];
		if ( awardkind <= 0 )
			continue;
		if ( pCity->actor_index >= 0 )
		{
			award_getaward( pCity->actor_index, awardkind, awardnum, -1, path, NULL );
		}
		else
		{
			gift( pCity->actorid, awardkind, awardnum, awardnum, 0, 0, -1, 0, 0, -1, 0, 0, -1, path );
		}
	}
	

	if ( g_army[army_index].action == ARMY_ACTION_GATHER )
	{
		if ( g_army[army_index].wood > 0 )
		{
			// ������ֵ
			//quest_addvalue( pCity, QUEST_DATAINDEX_GATHER, CityRes_Wood, (int)g_army[army_index].carry_wood );	
			// ͳ��
			//data_record_addvalue( pCity, DATA_RECORD_MAP_GATHER_WOOD, (int)g_army[army_index].carry_wood );

		}
		else if ( g_army[army_index].food > 0 )
		{
			//quest_addvalue( pCity, QUEST_DATAINDEX_GATHER, CityRes_Food, (int)g_army[army_index].carry_food );	
			//data_record_addvalue( pCity, DATA_RECORD_MAP_GATHER_FOOD, (int)g_army[army_index].carry_food );
		}
		else if ( g_army[army_index].iron > 0 )
		{
			//quest_addvalue( pCity, QUEST_DATAINDEX_GATHER, CityRes_Iron, (int)g_army[army_index].carry_iron );
			//data_record_addvalue( pCity, DATA_RECORD_MAP_GATHER_IRON, (int)g_army[army_index].carry_iron );
		
		}
		else if ( g_army[army_index].iron > 0 )
		{
			//quest_addvalue( pCity, QUEST_DATAINDEX_GATHER, CityRes_Mithril, (int)g_army[army_index].carry_mithril );
			//data_record_addvalue( pCity, DATA_RECORD_MAP_GATHER_MITHRIL, (int)g_army[army_index].carry_mithril );
		}
		else if ( g_army[army_index].token > 0 )
		{
			
		}
	}
	// �Զ�����
	hero_addsoldiers_audo( pCity );
	return 0;
}


// ���ò���״̬
void army_setstate( int army_index, char state )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	if ( state == ARMY_STATE_REBACK && (/*g_army[army_index].state == ARMY_STATE_GROUP_ING ||*/ g_army[army_index].state == ARMY_STATE_MARCH) )
	{
		// ��·����
		g_army[army_index].statetime = g_army[army_index].stateduration - g_army[army_index].statetime;
	}
	else
	{
		g_army[army_index].statetime = 0;
		g_army[army_index].stateduration = 0;
	}
	g_army[army_index].state = state;

	// ��״̬
	switch ( g_army[army_index].state )
	{
	case ARMY_STATE_FIGHT:				// ս����
		mapunit_update( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].unit_index );
		//army_marchroute_del( army_index );
		break;

	case ARMY_STATE_OCCUPY:				// פ����
		mapunit_update( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].unit_index );
		army_marchroute_del( army_index );
		break;

	case ARMY_STATE_GROUP_END:			// ����ָ������ص�
		mapunit_del( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].unit_index );
		g_army[army_index].unit_index = -1;
		army_marchroute_del( army_index );
		break;

	case ARMY_STATE_GATHER:				// �ɼ���
		army_gather_time( army_index );
		mapunit_del( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].unit_index );
		g_army[army_index].unit_index = -1;
		army_marchroute_del( army_index );
		break;

	case ARMY_STATE_HELP:				// Ԯ����
		mapunit_del( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].unit_index );
		g_army[army_index].stateduration = global.cityhelp_sec;
		g_army[army_index].unit_index = -1;
		army_marchroute_del( army_index );
		break;
	case ARMY_STATE_MARCH:				// �о���
	case ARMY_STATE_REBACK:				// ������
		army_march_time( army_index );
		if ( g_army[army_index].unit_index < 0 )
		{
			g_army[army_index].unit_index = mapunit_add( MAPUNIT_TYPE_ARMY, army_index );
		}
		else
		{
			mapunit_update( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].unit_index );
		}
		army_marchroute_add( army_index );
		break;
	default:
		break;
	}

	// ֪ͨ���������ǳظ��³�������
	if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	{
		city_battlequeue_sendupdate( army_index );
	}

}

// �����߼�
void army_alllogic()
{
	for ( int army_index = 0; army_index < g_army_maxindex/*ע�⣺ʹ������λ�ã�Ϊ��Ч��*/; army_index++ )
	{
		if ( g_army[army_index].from_type <= 0 )
			continue;
		army_logic( army_index );
	}
}

// ��ֻ�����߼�
void army_logic( int army_index )
{
	switch ( g_army[army_index].state )
	{
	case ARMY_STATE_MARCH:				// �о���
		army_march( army_index );
		break;
	case ARMY_STATE_FIGHT:				// ս����
		army_fight( army_index );
		break;
	case ARMY_STATE_GATHER:				// �ɼ���
		if ( army_gather( army_index ) < 0 )
		{
			army_setstate( army_index, ARMY_STATE_REBACK );
		}
		break;
	case ARMY_STATE_HELP:				// Ԯ����
		army_help( army_index );
		break;
	case ARMY_STATE_REBACK:				// ������
		army_reback( army_index );
		break;
	case ARMY_STATE_KINGWAR_FIGHT:
		army_kingwar_queue( army_index );
		break;
	case ARMY_STATE_KINGWAR_WAITSOS:
		army_kingwar_waitsos( army_index );
		break;
	}
}

// ��ȡ���������ǳ�
inline City *army_getcityptr( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return NULL;
	if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	{
		int city_index = g_army[army_index].from_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
		{
			return &g_city[city_index];
		}
	}
	return NULL;
}

// ��ȡ����Ŀ�������ǳ�
inline City *army_getcityptr_target( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return NULL;
	City *pCity = NULL;
	if ( g_army[army_index].to_type == MAPUNIT_TYPE_CITY )
	{ // Ŀ���ǳǳ�
		int city_index = g_army[army_index].to_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
			pCity = &g_city[city_index];
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ARMY )
	{ // Ŀ���ǲ���
		int target_armyindex = g_army[army_index].to_id;
		pCity = army_getcityptr( target_armyindex );
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_RES )
	{ // Ŀ������Դ��
		int index = g_army[army_index].to_id;
		pCity = army_getcityptr( map_res_getarmy( index ) );
	}
	return pCity;
}

// ��ȡ��������
char *army_getname( int army_index )
{
	if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	{ // �����ǳǳ�
		int city_index = g_army[army_index].from_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
			return g_city[city_index].name;
		return "";
	}
	else if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
	{ // �����ǳ���

	}
	return "";
}


// ��ȡ����Ŀ������
char *army_getname_target( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return NULL;
	if ( g_army[army_index].to_type == MAPUNIT_TYPE_CITY )
	{ // Ŀ���ǳǳ�
		int city_index = g_army[army_index].to_index;
		return g_city[city_index].name;
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ARMY )
	{ // Ŀ���ǲ���
		int target_armyindex = g_army[army_index].to_id;
		City *pCity = army_getcityptr( target_armyindex );
		if ( pCity )
		{
			return pCity->name;
		}
		else
		{
			return "";
		}
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_RES )
	{ // Ŀ������Դ��
		int index = g_army[army_index].to_id;
		City *pCity = army_getcityptr( map_res_getarmy( index ) );
		if ( pCity )
		{
			return pCity->name;
		}
		else
		{
			return "";
		}
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ARMY )
	{ // Ŀ���ǲ���
		City *pCity = army_getcityptr( g_army[army_index].to_index );
		if ( pCity )
		{
			return pCity->name;
		}
		else
		{
			return "";
		}
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ENEMY )
	{ // Ŀ���ǹ���
	
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_TOWN )
	{ // Ŀ���ǳ���
		
	}
	
	return "";
}

// ��ȡ���������ǳ�
inline int army_getcityid( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return 0;
	City *pCity = army_getcityptr( army_index );
	if ( pCity )
	{
		return pCity->actorid;
	}
	return 0;
}

// ��ȡ������������
int army_getnation( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return 0;
	if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	{
		City *pCity = army_getcityptr( army_index );
		if ( pCity )
		{
			return pCity->nation;
		}
	}
	else if( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
	{
		MapTown *pTown = map_town_getptr( g_army[army_index].from_id );
		if ( pTown )
		{
			return pTown->nation;
		}

	}
	return 0;
}

// ��ȡ����Ŀ��ǳ�
inline int army_getcityid_target( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return 0;
	City *pCity = army_getcityptr_target( army_index );
	if ( pCity )
	{
		return pCity->actorid;
	}
	return 0;
}

// ��ȡ����Ŀ�����
int army_getnation_target( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return 0;
	City *pCity = army_getcityptr_target( army_index );
	if ( pCity )
	{
		return pCity->nation;
	}
	else
	{
		if ( g_army[army_index].to_type == MAPUNIT_TYPE_TOWN )
		{
			MapTown *pTown = map_town_getptr( g_army[army_index].to_id );
			if ( pTown )
			{
				return pTown->nation;
			}
		}
	}
	return 0;
}

// �������Я������
int army_carry_additem( int army_index, int itemkind, int itemnum )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	if ( itemkind <= 0 )
		return -1;
	if ( itemnum <= 0 )
		return -1;
	char has = 0;
	for ( int tmpi = 0; tmpi < ARMY_CARRY_ITEMMAX; tmpi++ )
	{ // ��һ����ͬ�ĵ���
		if ( g_army[army_index].awardkind[tmpi] == itemkind )
		{
			g_army[army_index].awardnum[tmpi] += itemnum;
			has = 1;
			break;
		}
	}

	if ( has == 0 )
	{
		for ( int tmpi = 0; tmpi < ARMY_CARRY_ITEMMAX; tmpi++ )
		{
			if ( g_army[army_index].awardkind[tmpi] <= 0 )
			{
				g_army[army_index].awardkind[tmpi] = itemkind;
				g_army[army_index].awardnum[tmpi] = itemnum;
				break;
			}
		}
	}
	return 0;
}

// �ɼ�ʱ��
int army_gather_time( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return 0;

	if ( g_army[army_index].to_type != MAPUNIT_TYPE_RES )
		return 0;

	int index = g_army[army_index].to_index;
	if ( index < 0 || index >= g_map_res_maxcount )
		return 0;

	MapResInfo *config = map_res_getconfig( g_map_res[index].kind );
	if ( !config )
		return 0;

	City *pCity = army_getcityptr( army_index );
	if ( !pCity )
		return 0;

	int hero_index = city_hero_getindex( pCity->index, g_army[army_index].herokind[0] );
	if ( hero_index < 0 && hero_index >= HERO_CITY_MAX )
		return 0;

	char color = pCity->hero[hero_index].color;
	if ( color < 0 || color >= 6 )
		return 0;

	// ��Դ�����ʣ����
	int res_total = g_map_res[index].num;

	// ÿ��ɼ���
	float sec_gather = (config->num / (float)config->sec);

	// �ɼ�ʱ��
	g_army[army_index].stateduration = (int)ceil( res_total / (sec_gather* (1.0f + pCity->attr.gather_per[0]) * (1.0f + pCity->attr.gather_per[1])) );
	if ( g_army[army_index].stateduration > global.hero_gather_duration[color] )
	{
		g_army[army_index].stateduration = global.hero_gather_duration[color];
	}
	return 0;
}

// ���Ӳɼ�������
int army_gather_calc( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;

	int index = g_army[army_index].to_index;
	if ( index < 0 || index >= g_map_res_maxcount )
	{ // �����Ƿ�
		return -1;
	}

	MapResInfo *config = map_res_getconfig( g_map_res[index].kind );
	if ( !config )
		return -1;

	City *pCity = army_getcityptr( army_index );
	if ( !pCity )
		return -1;

	// ��ǰ�Ѿ��ɼ�����Դ����=ÿ��ɼ���*�佫�ɼ�ʱ��*(1 + �Ƽ��ӳ�%)*(1����ӳ�%)
	int gathernum_buff = (int)round( (config->num / (float)config->sec) * g_army[army_index].gatherbuff * (1.0f + pCity->attr.gather_per[0]) * (1.0f + pCity->attr.gather_per[1]) );
	int gathernum = gathernum_buff + ( int )round( (config->num / (float)config->sec) * (g_army[army_index].statetime - g_army[army_index].gatherbuff) * (1.0f + pCity->attr.gather_per[0]) );

	// ���Ӳɼ���
	switch ( config->type )
	{
	case 1:
		g_army[army_index].silver = gathernum;
		break;
	case 2:
		g_army[army_index].wood = gathernum;
		break;
	case 3:
		g_army[army_index].food = gathernum;
		break;
	case 4:
		g_army[army_index].iron = gathernum;
		break;
	case 5:
		g_army[army_index].token = gathernum;
	default:
		break;
	}

	// ��Դ�ɼ�������
	g_map_res[index].num -= gathernum;
	if ( g_map_res[index].num <= 0 )
	{ // û�ˣ�ɾ����Դ��

		// ����ǹ��ҷ�ط�Χ,��ӿ�������
		short range_townid = map_tile_gettownid( g_map_res[index].posx, g_map_res[index].posy );
		if ( range_townid > 0 && range_townid < g_towninfo_maxnum )
		{
			if ( g_towninfo[range_townid].type == MAPUNIT_TYPE_TOWN_GJFD )
			{
				map_town_dev_addexp( range_townid, global.town_dev_gather );
			}
		}

		map_res_delete( index, 0 );
	}
	else
	{ // ������ϣ���Դ���������ȡ��
		map_res_setarmy( index, -1 );
	}

	return 0;
}

// ���Ӳɼ�
int army_gather( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;

	if ( g_army[army_index].to_type != MAPUNIT_TYPE_RES )
	{ // ���ͷǷ�
		return -1;
	}

	int index = g_army[army_index].to_index;
	if ( index < 0 || index >= g_map_res_maxcount )
	{ // �����Ƿ�
		return -1;
	}

	MapResInfo *config = map_res_getconfig( g_map_res[index].kind );
	if ( !config )
		return -1;

	// �Ѿ��ɼ�ʱ��
	g_army[army_index].appdata = g_map_res[index].kind;
	g_army[army_index].statetime += 1;
	g_army[army_index].gatherbuff += 1;
	if ( g_army[army_index].statetime >= g_army[army_index].stateduration )
	{// �ɼ����
		army_gather_calc( army_index );
		return -2;
	}
	return 0;
}

// ����Ԯ����
void army_help( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;

	// �Ѿ�Ԯ��ʱ��
	g_army[army_index].statetime += 1;
	if ( g_army[army_index].statetime >= g_army[army_index].stateduration )
	{
		// �����ʿ��Ԯ��
		if ( g_army[army_index].action == ARMY_ACTION_HELP_TROOP )
		{
			army_setstate( army_index, ARMY_STATE_REBACK );
		}
	}
}

// פ����
void army_occupy( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	if ( g_army[army_index].action == ARMY_ACTION_OCCUPY )
		return;
}

// ����ս��
void army_fight( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;

	// �Ѿ�ս��ʱ��
	g_army[army_index].statetime += 1;

	// �ݶ�����N�붯��
	if ( g_army[army_index].statetime > ARMY_STATE_FIGHT_TIME )
	{
		// ɾ�����龯��
		city_underfire_del( army_getcityptr_target( army_index ), army_index );

		// ��ʼս��
		if ( fight_start( army_index, g_army[army_index].to_type, g_army[army_index].to_index ) < 0 )
		{
			if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
			{
				army_delete( army_index );
			}
			else
			{
				army_setstate( army_index, ARMY_STATE_REBACK );
			}
			write_gamelog( "[FIGHT_START_ERROR]_to_type:%d", g_army[army_index].to_type );
			return;
		}

		// ս������
		fight_lost_calc();

		// �������ǳǳصĲ���
		if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
		{
			// �������ǳǳ�
			if ( g_army[army_index].to_type == MAPUNIT_TYPE_CITY )
			{
				//army_vs_city( army_index, army_getcityptr_target( army_index ) );
			}
			// ��������פ���Ĳ���
			else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ARMY )
			{
				//army_vs_army( army_index, g_army[army_index].target_index );
			}
			// �������ǳ���
			else if ( g_army[army_index].to_type == MAPUNIT_TYPE_TOWN )
			{
				//army_vs_town( army_index, g_army[army_index].target_index );
			}
			// �������ǹ���
			else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ENEMY )
			{
				army_vs_enemy( army_index, &g_fight );
			}
			// ����������Դ��
			else if ( g_army[army_index].to_type == MAPUNIT_TYPE_RES )
			{
				army_vs_res( army_index, &g_fight );
			}
			// �������ǹ�������
			else if ( g_army[army_index].to_type == MAPUNIT_TYPE_NATIONHERO )
			{
				army_vs_nationhero( army_index, &g_fight );
			}
		}

		//	// ֪ͨ��ս�����
		//	City *pActorCity = army_getcityptr( army_index );
		//	if ( pActorCity )
		//	{
		//		if ( g_fight.result == FIGHT_WIN )
		//			actor_notify_pop( pActorCity->actor_index, 65 );
		//		else
		//			actor_notify_pop( pActorCity->actor_index, 66 );
		//	}
		//}
		//// ��������Ұ�����
		//else if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
		//{
		//	// ����������ҳǳ�
		//	City *pTargetCity = army_getcityptr_target( army_index );
		//	if ( pTargetCity && g_army[army_index].appdata <= 0 )
		//	{ // ���ڴ��ͼ����Ҫ���淨 
		//		army_vs_city( army_index, pTargetCity );

		//		// ������ʱ��
		//		int cuttime = 0;
		//		if ( g_fight.result == FIGHT_LOSE )
		//		{ // ����ʧ�ܣ�����ػ�ʤ��������������4��Сʱ����
		//			cuttime = -60 * 4;
		//		}

		//		// ��ӳ����¼�Ҫ��֮ǰ��λ�ò�Ҫ��
		//		map_town_addevent( g_army[army_index].from_index, 1, pTargetCity, g_fight.result, 4, 0 );

		//		// ���ñ���ʱ��
		//		map_town_protecttime( g_army[army_index].from_index, cuttime );
		//	}
		//	else if ( pTargetCity && g_army[army_index].appdata > 0 )
		//	{ // ����NPC�����¼�
		//		attack_event_vs_city( army_index, pTargetCity );
		//	}
		//}
		//// �������Ǳӻ���
		//else if ( g_army[army_index].from_type == MAPUNIT_TYPE_SHELTER )
		//{
		//	// ����֮ŭ�
		//	activity_dragon_fightover( army_index, &g_fight );
		//}

		//// ����Ĳ��Ӷ�ֱ��ɾ��
		//if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN || g_army[army_index].from_type == MAPUNIT_TYPE_SHELTER )
		//{
		//	army_delete( army_index );
		//	return;
		//}

		if ( g_army[army_index].state == ARMY_STATE_GATHER )
		{ // ռ��ɼ���ʤ����
			return;
		}
		army_setstate( army_index, ARMY_STATE_REBACK );
	}
}

// ��鲿���ǲ����Լ���
int army_myself( int actor_index, int army_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;

	City *pMyCity = city_getptr( actor_index );
	if ( !pMyCity )
		return -1;

	City *pCity = army_getcityptr( army_index );
	if ( !pCity )
		return -1;

	if ( pMyCity->actorid != pCity->actorid )
		return -1;
	return 0;
}

// �ʳ�Ѫս���д�ս
void army_kingwar_queue( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	if ( g_army[army_index].move_total_distance > 0 )
		g_army[army_index].move_total_distance -= 1;
}

// �ʳ�Ѫս����CD
void army_kingwar_waitsos( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	g_army[army_index].statetime += 1;
	if ( g_army[army_index].statetime >= g_army[army_index].stateduration )
	{
		army_delete( army_index );
	}
}
