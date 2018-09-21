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
#include "wqueue.h"
#include "timegmcmd.h"
#include "gmcmd.h"
#include "system.h"
#include "global.h"
#include "pay.h"
#include "actor_times.h"
#include "city.h"
#include "chat.h"
#include "quest.h"
#include "hero.h"
#include "nation.h"
#include "mail.h"
#include "activity_04.h"
#include "activity_22.h"
#include "equip.h"
#include "map.h"
#include "map_activity.h"
#include "map_zone.h"
#include "map_town.h"
#include "mapunit.h"
#include "army.h"
#include "fight.h"
#include "auto_data_kingwar_towninfo.h"
#include "auto_data_kingwar.h"
#include "auto_data_map_towninfo.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern Actor *g_actors;
extern int g_maxactornum;

extern Global global;
extern CommandEntry *g_command_entry;
extern int g_command_count;

extern City * g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern Army *g_army;
extern int g_army_maxcount;
extern int g_army_maxindex;
extern Fight g_fight;

extern ActivityItem *g_activity_item;
extern int g_activity_count;

extern MapTownInfo *g_towninfo;
extern int g_towninfo_maxnum;

extern KingWarConfig *g_kingwar_config;
extern int g_kingwar_config_maxnum;

extern MapTown *g_map_town;
extern int g_map_town_maxcount;

Activity22Info g_activity22info;
Activity22Rank g_activity22_rank[ACTIVITY22_RANK_MAX] = { 0 }; // ���а�

#include "auto_data_kingwar_towninfo.h"
#include "auto_data_map_towninfo.h"

void activity_22_load()
{
	g_activity22info.nation = world_data_get( WORLD_DATA_ACTIVITY22_NATION, NULL );
	g_activity22info.actorid = world_data_get( WORLD_DATA_ACTIVITY22_ACTORID, g_activity22info.actorname );

	for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUE_MAX; tmpi++ )
	{
		g_activity22info.attack_queue[tmpi] = -1;
		g_activity22info.defense_queue[tmpi] = -1;
	}
	for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUEMARCH_MAX; tmpi++ )
	{
		g_activity22info.attack_march[tmpi] = -1;
		g_activity22info.defense_march[tmpi] = -1;
	}
	for ( int tmpi = 0; tmpi < ACTIVITY22_RANK_MAX; tmpi++ )
	{
		g_activity22_rank[tmpi].actorid = 0;
		g_activity22_rank[tmpi].city_index = -1;
	}
	for ( int tmpi = 0; tmpi < g_city_maxcount; tmpi++ )
	{
		if ( g_city[tmpi].actorid < MINACTORID )
			continue;
		activity22_rank_calc( &g_city[tmpi] );
	}
}
void activity_22_onwarning( int lefttime )
{
	if ( lefttime < 0 )
		return;
	int countdown = lefttime / 60; //����ʱ�����ӣ�

	// 6039	����Ѫս����{0}���Ӻ�������μ�Ѫս�����ǰ��˾����
	if ( countdown == 30 || countdown == 5 || countdown == 1 )
	{
		char v1[64] = { 0 };
		sprintf( v1, "%d", countdown );
		system_talkjson_world( 6039, v1, NULL, NULL, NULL, NULL, NULL, 2 );
	}
}
void activity_22_onopen()
{
	system_talkjson_world( 6042, NULL, NULL, NULL, NULL, NULL, NULL, 2 );
	memset( &g_activity22info, 0, sizeof( Activity22Info ) );
	for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUE_MAX; tmpi++ )
	{
		g_activity22info.attack_queue[tmpi] = -1;
		g_activity22info.defense_queue[tmpi] = -1;
	}
	for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUEMARCH_MAX; tmpi++ )
	{
		g_activity22info.attack_march[tmpi] = -1;
		g_activity22info.defense_march[tmpi] = -1;
	}
	for ( int tmpi = 0; tmpi < g_city_maxcount; tmpi++ )
	{ // ��������
		if ( g_city[tmpi].actorid < MINACTORID )
			continue;
		g_city[tmpi].act22kill = 0;
	}
	for ( int tmpi = 0; tmpi < ACTIVITY22_RANK_MAX; tmpi++ )
	{
		g_activity22_rank[tmpi].actorid = 0;
		g_activity22_rank[tmpi].city_index = -1;
	}

	// ���ûʵ�
	if ( g_map_town[MAPUNIT_KING_TOWNID].own_city_index >= 0 && g_map_town[MAPUNIT_KING_TOWNID].own_city_index < g_city_maxcount )
	{
		g_city[g_map_town[MAPUNIT_KING_TOWNID].own_city_index].own_townid = 0;
	}
	g_map_town[MAPUNIT_KING_TOWNID].own_actorid = 0;
	g_map_town[MAPUNIT_KING_TOWNID].own_city_index = -1;
	mapunit_update( MAPUNIT_TYPE_TOWN, MAPUNIT_KING_TOWNID, g_map_town[MAPUNIT_KING_TOWNID].unit_index );
	map_zone_master( g_towninfo[MAPUNIT_KING_TOWNID].zoneid );


	SLK_NetS_RedInfo pValue = { 0 };
	pValue.m_path = 8;
	pValue.m_has = 1;
	netsend_redinfo_S( 0, SENDTYPE_WORLD, &pValue );
	activity22_mapsendinfo( -1 );
}
void activity_22_onend()
{
	SLK_NetS_RedInfo pValue = { 0 };
	pValue.m_path = 8;
	pValue.m_has = 0;
	netsend_redinfo_S( 0, SENDTYPE_WORLD, &pValue );
	activity22_mapsendinfo( -1 );

	g_map_town[MAPUNIT_KING_TOWNID].nation = g_activity22info.nation;

	// ��ʤ����ɱ�˰����е�һ�ģ����ʵ�
	char hasking = 0;
	for ( int tmpi = 0; tmpi < ACTIVITY22_RANK_MAX; tmpi++ )
	{
		if ( g_activity22_rank[tmpi].actorid <= 0 )
			continue;
		int city_index = g_activity22_rank[tmpi].city_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
		{
			if ( g_activity22info.nation == g_city[city_index].nation )
			{
				g_map_town[MAPUNIT_KING_TOWNID].own_actorid = g_activity22_rank[tmpi].actorid;
				g_map_town[MAPUNIT_KING_TOWNID].own_city_index = g_activity22_rank[tmpi].city_index;

				// 6040 <color=03DE27FF>{0}</color>��Ѫս�ʳ��л�ʤ��ռ����������<color=03DE27FF>{1}</color>�ڸ�·�����ɱ����౻�ƾ�Ϊ�ʵۣ�
				char v1[32] = { 0 };
				char v2[32] = { 0 };
				sprintf( v1, "%s%d", TAG_NATION, g_map_town[MAPUNIT_KING_TOWNID].nation );
				sprintf( v2, "%s", city_getname( g_activity22_rank[tmpi].city_index ) );
				system_talkjson_world( 6040, v1, v2, NULL, NULL, NULL, NULL, 3 );
				hasking = 1;
				break;
			}
		}
	}
	if ( hasking == 0 )
	{ // û�лʵ�
		int army_index = g_activity22info.defense_queue[0];
		if ( army_index >= 0 && army_index < g_army_maxcount )
		{
			City *pCity = army_getcityptr( army_index );
			if ( pCity )
			{
				g_map_town[MAPUNIT_KING_TOWNID].own_actorid = pCity->actorid;
				g_map_town[MAPUNIT_KING_TOWNID].own_city_index = pCity->index;

				// 6040 <color=03DE27FF>{0}</color>��Ѫս�ʳ��л�ʤ��ռ����������<color=03DE27FF>{1}</color>�ڸ�·�����ɱ����౻�ƾ�Ϊ�ʵۣ�
				char v1[32] = { 0 };
				char v2[32] = { 0 };
				sprintf( v1, "%s%d", TAG_NATION, g_map_town[MAPUNIT_KING_TOWNID].nation );
				sprintf( v2, "%s", pCity->name );
				system_talkjson_world( 6040, v1, v2, NULL, NULL, NULL, NULL, 3 );
				hasking = 1;
			}
		}
	}
	
	// �������Ȧ����
	map_tile_setnation( g_towninfo[MAPUNIT_KING_TOWNID].posx, g_towninfo[MAPUNIT_KING_TOWNID].posy, g_towninfo[MAPUNIT_KING_TOWNID].range, MAPUNIT_KING_TOWNID, g_map_town[MAPUNIT_KING_TOWNID].nation );
	// ���Ľ���������������
	map_zone_setnation( map_zone_getid( g_towninfo[MAPUNIT_KING_TOWNID].posx, g_towninfo[MAPUNIT_KING_TOWNID].posy ), g_map_town[MAPUNIT_KING_TOWNID].nation );
	// �����ʳ����򶼳Ǻ����Ǹ���
	map_zone_center_townchange( MAPUNIT_KING_TOWNID );
	// �������
	mapunit_update( MAPUNIT_TYPE_TOWN, MAPUNIT_KING_TOWNID, g_map_town[MAPUNIT_KING_TOWNID].unit_index );
	// ���»ʵ�
	map_zone_master( g_towninfo[MAPUNIT_KING_TOWNID].zoneid );

	// �ۼ�ɱ�л���
	// 5551	��������ѪսԲ��������<color=03de27ff>{0}</color>ռ�����������ڱ���Ѫս�л�ɱ�о�<color=03de27ff>{1}</color>�����<color=03de27ff>����{2}</color>�����ֿ����ڶһ���ɫͼֽ��
	char v1[32] = { 0 };
	char v2[32] = { 0 };
	char v3[32] = { 0 };
	sprintf( v1, "%s%d", TAG_NATION, g_map_town[MAPUNIT_KING_TOWNID].nation );
	int point = 0;
	for ( int city_index = 0; city_index < g_city_maxindex/*ע�⣺ʹ������λ�ã�Ϊ��Ч��*/; city_index++ )
	{
		if ( g_city[city_index].actorid <= 0 )
			continue;
		for ( int i = g_kingwar_config_maxnum - 1; i >= 0; i-- )
		{
			if ( g_city[city_index].act22kill >= g_kingwar_config[i].totalkill )
			{
				if ( g_city[city_index].act22kill > 0 && g_kingwar_config[i].totalkill_award_point == 0 )
					point = 500;
				else
					point = g_kingwar_config[i].totalkill_award_point;
				city_kingwarpoint( city_index, point, PATH_KINGWAR_KILL );

				sprintf( v2, "%d", g_city[city_index].act22kill );
				sprintf( v3, "%d", point );
				mail_system( g_city[city_index].actor_index, g_city[city_index].actorid, 5058, 5551, v1, v2, v3, "", 0 );
				break;
			}
		}
	}

	// ���а����
	for ( int tmpi = 0; tmpi < ACTIVITY22_RANK_MAX; tmpi++ )
	{
		if ( g_activity22_rank[tmpi].actorid <= 0 )
			continue;
		point = g_kingwar_config[tmpi + 1].rank_award_point;
		city_kingwarpoint( g_activity22_rank[tmpi].city_index, point, PATH_KINGWAR_RANK );

		// 5552	��������Ѫս��ɱ����������<color=03de27ff>{0}</color>�����<color=03de27ff>����{1}</color>�����ֿ����ڶһ���ɫͼֽ��
		sprintf( v1, "%d", tmpi+1 );
		sprintf( v2, "%d", point );
		mail_system( MAIL_ACTORID, g_activity22_rank[tmpi].actorid, 5058, 5552, v1, v2, NULL, "", 0 );
	}

	// ���л���ӽ�ɢ
	for ( int army_index = 0; army_index < g_army_maxindex/*ע�⣺ʹ������λ�ã�Ϊ��Ч��*/; army_index++ )
	{
		if ( g_army[army_index].action == ARMY_ACTION_ACTIVITY22 )
		{
			army_delete( army_index );
		}
	}
}
void activity_22_onclose()
{

}

void activity_22_onlogic()
{
}
int activity_22_sendinfo( int actor_index )
{
	return 0;
}

// ���������Ƿ�����
char activity22_attack_or_defense( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	if ( g_activity22info.nation == army_getnation( army_index ) )
	{
		return 2;
	}
	return 1;
}

// ���һ֧����
int activity22_queueadd( char attack, int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	if ( attack == ACTIVITY22_ATTACK )
	{ // ��ӵ�������
		for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUE_MAX; tmpi++ )
		{
			if ( g_activity22info.attack_queue[tmpi] < 0 )
			{
				g_activity22info.attack_queue[tmpi] = army_index;
				activity22_sendupdate( ACTIVITY22_ATTACK );
				break;
			}
		}
	}
	else if ( attack == ACTIVITY22_DEFENSE )
	{ // ��ӵ�������
		for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUE_MAX; tmpi++ )
		{
			if ( g_activity22info.defense_queue[tmpi] < 0 )
			{
				g_activity22info.defense_queue[tmpi] = army_index;
				activity22_sendupdate( ACTIVITY22_DEFENSE );
				break;
			}
		}
	}
	return 0;
}

// ɾ��һ����ս����
int activity22_queuedel( char attack, int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	char isdelete = 0;
	if ( attack == ACTIVITY22_ATTACK )
	{
		for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUE_MAX; tmpi++ )
		{
			if ( g_activity22info.attack_queue[tmpi] == army_index )
			{
				g_activity22info.attack_queue[tmpi] = -1;
				if ( tmpi < ACTIVITY22_QUEUE_MAX - 1 )
				{
					memmove( &g_activity22info.attack_queue[tmpi], &g_activity22info.attack_queue[tmpi + 1], sizeof( int ) *(ACTIVITY22_QUEUE_MAX - 1 - tmpi) );
					g_activity22info.attack_queue[ACTIVITY22_QUEUE_MAX - 1] = -1;
				}
				isdelete = 1;
				army_delete( army_index );
				activity22_sendupdate( ACTIVITY22_ATTACK );
				break;
			}
		}
	}
	else if ( attack == ACTIVITY22_DEFENSE )
	{
		for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUE_MAX; tmpi++ )
		{
			if ( g_activity22info.defense_queue[tmpi] == army_index )
			{
				g_activity22info.defense_queue[tmpi] = -1;
				if ( tmpi < ACTIVITY22_QUEUE_MAX - 1 )
				{
					memmove( &g_activity22info.defense_queue[tmpi], &g_activity22info.defense_queue[tmpi + 1], sizeof( int ) *(ACTIVITY22_QUEUE_MAX - 1 - tmpi) );
					g_activity22info.defense_queue[ACTIVITY22_QUEUE_MAX - 1] = -1;
				}
				isdelete = 1;
				army_delete( army_index );
				activity22_sendupdate( ACTIVITY22_DEFENSE );
				break;
			}
		}
	}
	// ��ȫ���� 
	if ( isdelete == 0 )
	{
		if ( attack == ACTIVITY22_ATTACK )
		{
			for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUE_MAX; tmpi++ )
			{
				if ( g_activity22info.defense_queue[tmpi] == army_index )
				{
					g_activity22info.defense_queue[tmpi] = -1;
					if ( tmpi < ACTIVITY22_QUEUE_MAX - 1 )
					{
						memmove( &g_activity22info.defense_queue[tmpi], &g_activity22info.defense_queue[tmpi + 1], sizeof( int ) *(ACTIVITY22_QUEUE_MAX - 1 - tmpi) );
						g_activity22info.defense_queue[ACTIVITY22_QUEUE_MAX - 1] = -1;
					}
					army_delete( army_index );
					activity22_sendupdate( ACTIVITY22_ATTACK );
					break;
				}
			}
		}
		else if ( attack == ACTIVITY22_DEFENSE )
		{
			for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUE_MAX; tmpi++ )
			{
				if ( g_activity22info.attack_queue[tmpi] == army_index )
				{
					g_activity22info.attack_queue[tmpi] = -1;
					if ( tmpi < ACTIVITY22_QUEUE_MAX - 1 )
					{
						memmove( &g_activity22info.attack_queue[tmpi], &g_activity22info.attack_queue[tmpi + 1], sizeof( int ) *(ACTIVITY22_QUEUE_MAX - 1 - tmpi) );
						g_activity22info.attack_queue[ACTIVITY22_QUEUE_MAX - 1] = -1;
					}
					army_delete( army_index );
					activity22_sendupdate( ACTIVITY22_DEFENSE );
					break;
				}
			}
		}
	}
	activity22_totalcalc( attack );
	return 0;
}

// ��ȡ����ͷ����
int activity22_queueget( char attack )
{
	int army_index = -1;
	if ( attack == ACTIVITY22_ATTACK )
	{
		army_index = g_activity22info.attack_queue[0];
	}
	else if ( attack == ACTIVITY22_DEFENSE )
	{
		// �Ӷ�����ȡ��һ��
		army_index = g_activity22info.defense_queue[0];
	}
	return army_index;
}

int activity22_marchadd( char attack, int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	if ( attack == ACTIVITY22_ATTACK )
	{ // ��ӵ�������
		for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUEMARCH_MAX; tmpi++ )
		{
			if ( g_activity22info.attack_march[tmpi] < 0 )
			{
				g_activity22info.attack_march[tmpi] = army_index;
				activity22_sendupdate( ACTIVITY22_ATTACK );
				break;
			}
		}
	}
	else if ( attack == ACTIVITY22_DEFENSE )
	{ // ��ӵ�������
		for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUEMARCH_MAX; tmpi++ )
		{
			if ( g_activity22info.defense_march[tmpi] < 0 )
			{
				g_activity22info.defense_march[tmpi] = army_index;
				activity22_sendupdate( ACTIVITY22_DEFENSE );
				break;
			}
		}
	}
	activity22_totalcalc( attack );
	return 0;
}
int activity22_marchdel( char attack, int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	char isdelete = 0;
	if ( attack == ACTIVITY22_ATTACK )
	{
		for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUEMARCH_MAX; tmpi++ )
		{
			if ( g_activity22info.attack_march[tmpi] == army_index )
			{
				g_activity22info.attack_march[tmpi] = -1;
				activity22_sendupdate( ACTIVITY22_ATTACK );
				break;
			}
		}
	}
	else if ( attack == ACTIVITY22_DEFENSE )
	{
		for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUEMARCH_MAX; tmpi++ )
		{
			if ( g_activity22info.defense_march[tmpi] == army_index )
			{
				g_activity22info.defense_march[tmpi] = -1;
				activity22_sendupdate( ACTIVITY22_ATTACK );
				break;
			}
		}
	}
	activity22_totalcalc( attack );
	return 0;
}
int activity22_marcharrived( char attack, int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	if ( attack == ACTIVITY22_ATTACK )
	{
		for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUEMARCH_MAX; tmpi++ )
		{
			if ( g_activity22info.attack_march[tmpi] == army_index )
			{
				g_activity22info.attack_march[tmpi] = -1;
				activity22_queueadd( attack, army_index );
				break;
			}
		}
	}
	else if ( attack == ACTIVITY22_DEFENSE )
	{
		for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUEMARCH_MAX; tmpi++ )
		{
			if ( g_activity22info.defense_march[tmpi] == army_index )
			{
				g_activity22info.defense_march[tmpi] = -1;
				activity22_queueadd( attack, army_index );
				break;
			}
		}
	}
	activity22_totalcalc( attack );
	return 0;
}

// ���¼������
int activity22_totalcalc( char attack )
{
	if ( attack == ACTIVITY22_ATTACK )
	{ // ������
		g_activity22info.attack_total = 0;
		for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUE_MAX; tmpi++ )
		{
			int army_index = g_activity22info.attack_queue[tmpi];
			if ( army_index < 0 || army_index >= g_army_maxcount )
				break;
			g_activity22info.attack_total += g_army[army_index].totals;
		}
		for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUEMARCH_MAX; tmpi++ )
		{
			int army_index = g_activity22info.attack_march[tmpi];
			if ( army_index < 0 || army_index >= g_army_maxcount )
				break;
			g_activity22info.attack_total += g_army[army_index].totals;
		}
	}
	else if ( attack == ACTIVITY22_DEFENSE )
	{ // ������
		g_activity22info.defense_total = 0;
		for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUE_MAX; tmpi++ )
		{
			int army_index = g_activity22info.defense_queue[tmpi];
			if ( army_index < 0 || army_index >= g_army_maxcount )
				break;
			g_activity22info.defense_total += g_army[army_index].totals;
		}
		for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUEMARCH_MAX; tmpi++ )
		{
			int army_index = g_activity22info.defense_march[tmpi];
			if ( army_index < 0 || army_index >= g_army_maxcount )
				break;
			g_activity22info.defense_total += g_army[army_index].totals;
		}
	}
	return 0;
}

int activity22_change_nation( int nation )
{
	if ( g_activity22info.nation == nation )
		return -1;
	if ( g_activity22info.defense_queue[0] >= 0 )
		return -1;
	char oldnation = g_activity22info.nation;
	g_activity22info.nation = nation;
	world_data_set( WORLD_DATA_ACTIVITY22_NATION, nation, NULL, NULL );

	// ��������Ȩ
	g_map_town[MAPUNIT_KING_TOWNID].nation = g_activity22info.nation;
	// �������Ȧ����
	map_tile_setnation( g_towninfo[MAPUNIT_KING_TOWNID].posx, g_towninfo[MAPUNIT_KING_TOWNID].posy, g_towninfo[MAPUNIT_KING_TOWNID].range, MAPUNIT_KING_TOWNID, g_map_town[MAPUNIT_KING_TOWNID].nation );
	// ���Ľ���������������
	map_zone_setnation( map_zone_getid( g_towninfo[MAPUNIT_KING_TOWNID].posx, g_towninfo[MAPUNIT_KING_TOWNID].posy ), g_map_town[MAPUNIT_KING_TOWNID].nation );
	// �����ʳ����򶼳Ǻ����Ǹ���
	map_zone_center_townchange( MAPUNIT_KING_TOWNID );
	// �������
	mapunit_update( MAPUNIT_TYPE_TOWN, MAPUNIT_KING_TOWNID, g_map_town[MAPUNIT_KING_TOWNID].unit_index );

	int temp[ACTIVITY22_QUEUE_MAX];
	int temp_index = 0;
	for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUE_MAX; tmpi++ )
	{
		temp[tmpi] = -1;
	}
	// �ѹ�������������������ҵ�������,�ŵ�����������
	int index = 0;
	for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUE_MAX; tmpi++ )
	{
		int army_index = g_activity22info.attack_queue[tmpi];
		if ( army_index < 0 || army_index >= g_army_maxcount )
			continue;
		if ( army_getnation( army_index ) == nation )
		{
			g_activity22info.defense_queue[index] = army_index;
			index += 1;
		}
		else
		{
			temp[temp_index] = army_index;
			temp_index += 1;
		}
		g_activity22info.attack_queue[tmpi] = -1;
	}
	memcpy( g_activity22info.attack_queue, temp, sizeof( int )*ACTIVITY22_QUEUE_MAX );


	// �о�����
	int march_temp[ACTIVITY22_QUEUEMARCH_MAX];
	int march_temp_index = 0;
	for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUEMARCH_MAX; tmpi++ )
	{
		march_temp[tmpi] = -1;
	}
	// �ѹ�������������������ҵ�������,�ŵ�����������
	index = 0;
	for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUEMARCH_MAX; tmpi++ )
	{
		int army_index = g_activity22info.attack_march[tmpi];
		if ( army_index < 0 || army_index >= g_army_maxcount )
			continue;
		if ( army_getnation( army_index ) == nation )
		{
			g_activity22info.defense_march[index] = army_index;
			index += 1;
		}
		else
		{
			march_temp[march_temp_index] = army_index;
			march_temp_index += 1;
		}
		g_activity22info.attack_march[tmpi] = -1;
	}
	memcpy( g_activity22info.attack_march, march_temp, sizeof( int )*ACTIVITY22_QUEUEMARCH_MAX );

	activity22_totalcalc( ACTIVITY22_ATTACK );
	activity22_totalcalc( ACTIVITY22_DEFENSE );


	// 6041	����������<color=d95df4ff>{0}</color>����ȫ����û��������<color=d95df4ff>{1}</color>ռ�죡
	char v1[32] = { 0 };
	char v2[32] = { 0 };
	sprintf( v1, "%s%d", TAG_NATION, oldnation );
	sprintf( v2, "%s%d", TAG_NATION, nation );
	system_talkjson_world( 6041, v1, v2, NULL, NULL, NULL, NULL, 1 );
	//g_kingwar_town_update = 1;
	return 0;
}

int _fightstep = 0;
void activity22_fightlogic()
{
	if ( activity_intime( ACTIVITY_22 ) == 0 )
	{
		return;
	}
	_fightstep++;
	if ( _fightstep >= 5 )
	{
		activity22_fight();
		_fightstep = 0;
	}
}

void activity22_fight()
{
	int attack_army_index = -1;
	int defense_army_index = -1;

	// ��ȡ������
	attack_army_index = activity22_queueget( ACTIVITY22_ATTACK );
	if ( attack_army_index < 0 )
		return;
	if ( g_army[attack_army_index].state != ARMY_STATE_ACTIVITY22_READY )
		return;

	// ��ȡ������
	defense_army_index = activity22_queueget( ACTIVITY22_DEFENSE );
	if ( defense_army_index < 0 )
	{
		// ������û���ˣ���ɹ�������
		activity22_change_nation( army_getnation( attack_army_index ) );
		return;
	}
	if ( g_army[defense_army_index].state != ARMY_STATE_ACTIVITY22_READY )
	{
		// ������û���ˣ���ɹ�������
		activity22_change_nation( army_getnation( attack_army_index ) );
		return;
	}

	// ˫�������ˣ�ս��
	if ( fight_start( attack_army_index, MAPUNIT_TYPE_TOWN, defense_army_index ) < 0 )
	{
		return;
	}

	// ս������
	fight_lost_calc();

	// ����ʧ����
	//g_kingwar_lost_totalhp += g_fight.attack_total_damage;
	//g_kingwar_lost_totalhp += g_fight.defense_total_damage;
	activity22_addkill( attack_army_index, g_fight.attack_total_damage );
	activity22_addkill( defense_army_index, g_fight.defense_total_damage );

	if ( g_fight.result == FIGHT_WIN )
	{ // ������ʤ��
		activity22_totalcalc( ACTIVITY22_ATTACK );
		// ɾ��һ�����ط�
		army_delete( defense_army_index );
		activity22_sendupdate( ACTIVITY22_ATTACK );
	}
	else
	{ // ������ʤ��
		activity22_totalcalc( ACTIVITY22_DEFENSE );
		// ɾ��һ��������
		army_delete( attack_army_index );
		activity22_sendupdate( ACTIVITY22_DEFENSE );
	}
	
	// ս��֪ͨ
	/*SLK_NetS_KingWarNotify pValue = { 0 };
	pValue.m_id = id;
	pValue.m_result = g_fight.result;

	pValue.m_a_losthp = g_fight.defense_total_damage;
	pValue.m_d_losthp = g_fight.attack_total_damage;

	pValue.m_a_nation = army_getnation( attack_army_index );
	pValue.m_d_nation = army_getnation( defense_army_index );

	pValue.m_a_heroid = g_army[attack_army_index].herokind[0];
	pValue.m_d_heroid = g_army[defense_army_index].herokind[0];

	City *pACity = army_getcityptr( attack_army_index );
	if ( pACity )
	{
		pACity->kw_totalkill += g_fight.attack_total_damage;
		pValue.m_a_color = hero_getcolor( pACity, pValue.m_a_heroid );
		strncpy( pValue.m_a_name, pACity->name, NAME_SIZE );
		pValue.m_a_name_len = strlen( pValue.m_a_name );
	}

	City *pDCity = army_getcityptr( defense_army_index );
	if ( pDCity )
	{
		pDCity->kw_totalkill += g_fight.defense_total_damage;
		pValue.m_d_heroid = hero_getcolor( pDCity, pValue.m_d_heroid );
		strncpy( pValue.m_d_name, pDCity->name, NAME_SIZE );
		pValue.m_d_name_len = strlen( pValue.m_d_name );
	}

	netsend_kingwarnotify_S( SUBSCRIBE_CMD_KINGWARDLG, SENDTYPE_SUBSCRIBE_NATION + pValue.m_a_nation, &pValue );
	netsend_kingwarnotify_S( SUBSCRIBE_CMD_KINGWARDLG, SENDTYPE_SUBSCRIBE_NATION + pValue.m_d_nation, &pValue );*/
}

void activity22_army_makestruct( int index, Army *pArmy, SLK_NetS_Act22Army *pValue )
{
	pValue->m_army_index = pArmy->index;
	City *pCity = army_getcityptr( pArmy->index );
	if ( pCity )
	{
		strncpy( pValue->m_name, pCity->name, NAME_SIZE );
		pValue->m_namelen = strlen( pValue->m_name );
		pValue->m_nation = pCity->nation;
	}
	pValue->m_totals = pArmy->totals;
	pValue->m_herokind[0] = pArmy->herokind[0];
	pValue->m_herokind[1] = pArmy->herokind[1];
	pValue->m_herokind[2] = pArmy->herokind[2];
	pValue->m_herokind[3] = pArmy->herokind[3];
	pValue->m_index = index;
	pValue->m_state = pArmy->state;
	pValue->m_statetime = pArmy->statetime;
	pValue->m_stateduration = pArmy->stateduration;
}

int activity22_sendlist( int actor_index, char attack )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	int total = 0;
	int *queue = NULL;
	int *march = NULL;
	if ( attack == ACTIVITY22_ATTACK )
	{
		queue = g_activity22info.attack_queue;
		march = g_activity22info.attack_march;
		total = g_activity22info.attack_total;
	}
	else if ( attack == ACTIVITY22_DEFENSE )
	{
		queue = g_activity22info.defense_queue;
		march = g_activity22info.defense_march;
		total = g_activity22info.defense_total;
	}
	else
		return -1;

	SLK_NetS_Act22ArmyList pValue = { 0 };
	pValue.m_attack = attack;
	pValue.m_path = 0;
	pValue.m_count = 0;
	netsend_act22armylist_S( actor_index, SENDTYPE_ACTOR, &pValue );

	pValue.m_path = 1;
	pValue.m_count = 0;
	Army *pArmy = NULL;
	for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUE_MAX; tmpi++ )
	{
		int army_index = queue[tmpi];
		if ( army_index < 0 || army_index >= g_army_maxcount )
			continue;
		if ( g_army[army_index].action != ARMY_ACTION_ACTIVITY22 )
			continue;
		activity22_army_makestruct( tmpi, &g_army[army_index], &pValue.m_list[pValue.m_count] );
		pValue.m_count += 1;
		if ( pValue.m_count >= 24 )
		{
			netsend_act22armylist_S( actor_index, SENDTYPE_ACTOR, &pValue );
			pValue.m_count = 0;
		}
	}
	for ( int tmpi = 0; tmpi < ACTIVITY22_QUEUEMARCH_MAX; tmpi++ )
	{
		int army_index = march[tmpi];
		if ( army_index < 0 || army_index >= g_army_maxcount )
			continue;
		if ( g_army[army_index].action != ARMY_ACTION_ACTIVITY22 )
			continue;
		activity22_army_makestruct( tmpi, &g_army[army_index], &pValue.m_list[pValue.m_count] );
		pValue.m_count += 1;
		if ( pValue.m_count >= 24 )
		{
			netsend_act22armylist_S( actor_index, SENDTYPE_ACTOR, &pValue );
			pValue.m_count = 0;
		}
	}
	if ( pValue.m_count > 0 )
		netsend_act22armylist_S( actor_index, SENDTYPE_ACTOR, &pValue );

	pValue.m_path = 2;
	pValue.m_count = 0;
	netsend_act22armylist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// ����
int activity22_sendupdate( int value )
{
	SLK_NetS_Act22Update pValue = { 0 };
	pValue.m_value = value;
	netsend_act22update_S( 0, SENDTYPE_WORLD, &pValue );
	return 0;
}

int activity22_addkill( int army_index, int value )
{
	City *pArmyCity = army_getcityptr( army_index );
	if ( !pArmyCity )
		return -1;
	pArmyCity->act22kill += value;
	activity22_rank_calc( pArmyCity );
	return 0;
}

int activity22_rank_calc( City *pCity )
{
	if ( !pCity || pCity->act22kill <= 0 )
		return -1;

	// �仯֮ǰ�ĵ�һ��
	int one_city_index = g_activity22_rank[0].city_index;

	// �ҵ��ҵ������������߳�
	int my_tmpi = -1;
	for ( int tmpi = 0; tmpi < ACTIVITY22_RANK_MAX; tmpi++ )
	{
		if ( g_activity22_rank[tmpi].actorid == pCity->actorid )
		{
			my_tmpi = tmpi;
			break;
		}
	}
	if ( my_tmpi >= 0 && my_tmpi < ACTIVITY22_RANK_MAX - 1 )
	{
		// ��ǰ��
		memmove( &g_activity22_rank[my_tmpi], &g_activity22_rank[my_tmpi + 1], sizeof( Activity22Rank )*(ACTIVITY22_RANK_MAX - my_tmpi - 1) );
		g_activity22_rank[ACTIVITY22_RANK_MAX - 1].actorid = 0;
		g_activity22_rank[ACTIVITY22_RANK_MAX - 1].city_index = -1;
	}
	else if ( my_tmpi == ACTIVITY22_RANK_MAX - 1 )
	{
		g_activity22_rank[my_tmpi].actorid = 0;
		g_activity22_rank[my_tmpi].city_index = -1;
	}

	// ��ǰ30����һ�Ƚ�
	int replace_tmpi = -1;
	for ( int tmpi = 0; tmpi < ACTIVITY22_RANK_MAX; tmpi++ )
	{
		int value = 0;
		int city_index = g_activity22_rank[tmpi].city_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
		{
			value = g_city[city_index].act22kill;
		}

		if ( pCity->act22kill > value )
		{
			replace_tmpi = tmpi;
			break;
		}
	}

	if ( replace_tmpi >= 0 && replace_tmpi < ACTIVITY22_RANK_MAX - 1 )
	{
		// ������
		memmove( &g_activity22_rank[replace_tmpi + 1], &g_activity22_rank[replace_tmpi], sizeof( Activity22Rank )*(ACTIVITY22_RANK_MAX - replace_tmpi - 1) );
		g_activity22_rank[replace_tmpi].actorid = pCity->actorid;
		g_activity22_rank[replace_tmpi].city_index = pCity->index;
	}
	else if ( replace_tmpi == ACTIVITY22_RANK_MAX - 1 )
	{
		g_activity22_rank[replace_tmpi].actorid = pCity->actorid;
		g_activity22_rank[replace_tmpi].city_index = pCity->index;
	}

	if ( my_tmpi > 0 && replace_tmpi == 0 )
	{//  6032	��ϲ<color=03DE27FF>[{0}]{1}</color>�ڳ�ֵ���л����ʱλ�ӵ�һ
		char v1[64] = { 0 };
		char v2[64] = { 0 };
		sprintf( v1, "%s%d", TAG_NATION, pCity->nation );
		sprintf( v2, "%s", pCity->name );
		system_talkjson_world( 6032, v1, v2, NULL, NULL, NULL, NULL, 2 );
	}
	return 0;
}

// ���а�
int activity22_rank_sendlist( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	SLK_NetS_Act22RankList pValue = { 0 };
	for ( int tmpi = 0; tmpi < ACTIVITY22_RANK_MAX; tmpi++ )
	{
		int city_index = g_activity22_rank[tmpi].city_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
		{
			strncpy( pValue.m_list[pValue.m_count].m_name, g_city[city_index].name, NAME_SIZE );
			pValue.m_list[pValue.m_count].m_namelen = strlen( pValue.m_list[pValue.m_count].m_name );
			pValue.m_list[pValue.m_count].m_nation = g_city[city_index].nation;
			pValue.m_list[pValue.m_count].m_kill = g_city[city_index].act22kill;
			pValue.m_list[pValue.m_count].m_rank = tmpi + 1;
			pValue.m_count += 1;
		}
	}
	netsend_act22ranklist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// ���Ϣ
int activity22_mapsendinfo( int actor_index )
{
	// �δ��ʼ
	SLK_NetS_KingWarActivity pValue = { 0 };
	int nowstamp = (int)time( NULL );
	if ( nowstamp < g_activity_item[ACTIVITY_22].m_starttime )
	{
		pValue.m_state = 0;
		pValue.m_beginstamp = g_activity_item[ACTIVITY_22].m_starttime;
		pValue.m_endstamp = 0;
	}
	// ���
	else if ( nowstamp >= g_activity_item[ACTIVITY_22].m_starttime && nowstamp < g_activity_item[ACTIVITY_22].m_endtime )
	{
		pValue.m_state = 1;
		pValue.m_beginstamp = g_activity_item[ACTIVITY_22].m_starttime;
		pValue.m_endstamp = g_activity_item[ACTIVITY_22].m_endtime;
	}
	else
	{ // �����
		pValue.m_state = 2;
	}
	pValue.m_nation = g_map_town[MAPUNIT_KING_TOWNID].nation;

	if ( actor_index >= 0 && actor_index < g_maxactornum )
	{
		netsend_kingwaractivity_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}
	else
	{
		netsend_kingwaractivity_S( 0, SENDTYPE_WORLDMAP, &pValue );
	}
	return 0;
}