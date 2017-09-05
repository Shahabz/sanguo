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
#include "army_march.h"
#include "city.h"
#include "map_town.h"
#include "map_enemy.h"
#include "map_res.h"
#include "map.h"

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

extern Map g_map;

extern Army *g_army;
extern int g_army_maxcount;
extern int g_army_count;
extern int g_army_maxindex;

// �ؼ��㲿���о�����ʱ��
int army_marchtime_calc( City *pCity, short from_posx, short from_posy, short to_posx, short to_posy )
{
	int duration = 0;
	// ���ȼ��ʳ�������ǻʳ������о�
	if ( from_posx >= g_zoneinfo[13].top_left_posx && from_posx <= g_zoneinfo[13].bottom_right_posx && 
		from_posy >= g_zoneinfo[13].top_left_posy && from_posy <= g_zoneinfo[13].bottom_right_posy )
	{
		if ( to_posx < g_zoneinfo[13].top_left_posx || to_posx > g_zoneinfo[13].bottom_right_posx ||
			to_posy < g_zoneinfo[13].top_left_posy || to_posy > g_zoneinfo[13].bottom_right_posy )
		{
			duration = global.army_move_kingzone;
			return duration;
		}
	}

	// �ܹ���Ҫ������ = ����*8s
	int total_distance = abs( from_posx - to_posx ) + abs( from_posy - to_posy );
	duration = total_distance*global.army_move;
	if ( pCity )
	{
		duration = (int)ceil( duration*(1.0f - pCity->attr.movespeed_per[0])*(1.0f - pCity->attr.movespeed_per[1])*(1.0f - pCity->attr.movespeed_per[2]) );
	}

	if ( duration <= 0 )
	{ // ���޿���
		duration = 2;
	}
	else if ( duration > 7200 )
	{ // ���޿���
		duration = 7200;
	}
	return duration;
}
int army_march_time( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	g_army[army_index].stateduration = army_marchtime_calc( army_getcityptr( army_index ), g_army[army_index].from_posx, g_army[army_index].from_posy, g_army[army_index].to_posx, g_army[army_index].to_posy );
	return 0;
}

// �����о�
void army_march( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;

	// �Ѿ��ƶ�ʱ��
	g_army[army_index].statetime += 1;

	// �Ѿ��ƶ�����=ʱ��*���ٶ�=�ܾ���/����Ҫʱ�䣩
	float move_distance = g_army[army_index].statetime * (g_army[army_index].move_total_distance / (float)g_army[army_index].stateduration);

	// ��ǰλ��
	g_army[army_index].posx = (short)((g_army[army_index].to_posx - g_army[army_index].from_posx) * move_distance / g_army[army_index].move_total_distance) + g_army[army_index].from_posx;
	g_army[army_index].posy = (short)((g_army[army_index].to_posy - g_army[army_index].from_posy) * move_distance / g_army[army_index].move_total_distance) + g_army[army_index].from_posy;

	// ����Ƿ��л�����
	mapunit_area_change( g_army[army_index].unit_index, g_army[army_index].posx, g_army[army_index].posy, 0 );

	// ����Ŀ�ĵ�
	if ( g_army[army_index].posx == g_army[army_index].to_posx && g_army[army_index].posy == g_army[army_index].to_posy || g_army[army_index].statetime > g_army[army_index].stateduration )
	{
		// ֪ͨ�������ѵִ�Ŀ�ĵ�
		City *pActorCity = army_getcityptr( army_index );
		if ( pActorCity )
		{
			//actor_system_message( pActorCity->actor_index, 67 );
		}

		army_arrived( army_index );
	}
}

// ���ӷ���
void army_reback( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;

	// �Ѿ��ƶ�ʱ��
	g_army[army_index].statetime += 1;

	// �Ѿ��ƶ�����=ʱ��*���ٶ�=�ܾ���/����Ҫʱ�䣩
	float move_distance = g_army[army_index].statetime * (g_army[army_index].move_total_distance / (float)g_army[army_index].stateduration);

	// ��ǰλ��
	g_army[army_index].posx = (short)((g_army[army_index].from_posx - g_army[army_index].to_posx) * move_distance / g_army[army_index].move_total_distance) + g_army[army_index].to_posx;
	g_army[army_index].posy = (short)((g_army[army_index].from_posy - g_army[army_index].to_posy) * move_distance / g_army[army_index].move_total_distance) + g_army[army_index].to_posy;

	// ����Ƿ��л�����
	mapunit_area_change( g_army[army_index].unit_index, g_army[army_index].posx, g_army[army_index].posy, 0 );

	// ����Ŀ�ĵ�
	if ( g_army[army_index].posx == g_army[army_index].from_posx && g_army[army_index].posy == g_army[army_index].from_posy || g_army[army_index].statetime > g_army[army_index].stateduration )
	{
		// ���ȼ��Ŀ�ĵ��Ƿ�ĸ��
		MapUnit *pUnit = mapunit_getinfo_withpos( g_army[army_index].posx, g_army[army_index].posy, MAPUNIT_TYPE_ARMY, army_index );
		if ( pUnit && pUnit->type == MAPUNIT_TYPE_CITY && pUnit->index >= 0 )
		{
			City *pUnitCity = &g_city[pUnit->index];
			City *pActorCity = army_getcityptr( army_index );
			if ( pUnitCity && pActorCity && pUnitCity->actorid == pActorCity->actorid )
			{
				// ֪ͨ�������Ѿ����س���
				//actor_system_message( pActorCity->actor_index, 68 );

				// ����Ǽ��ᷢ����,�������ӽ�ɢ
				if ( g_army[army_index].action == ARMY_ACTION_GROUP_START )
				{
					// ɾ������
					//armygroup_ungroup_withcall( army_index );
				}
				else
				{
					// ɾ������
					army_delete( army_index );
				}
				return;
			}
		}
		army_delete( army_index );
	}
}

// ����ָ���ص�
void army_arrived( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	MapUnit *pUnit = mapunit_getinfo_withpos( g_army[army_index].posx, g_army[army_index].posy, MAPUNIT_TYPE_ARMY, army_index );
	if ( pUnit )
	{
		if ( pUnit->type != g_army[army_index].to_type )
		{ // ����о���Ŀ���ǲ��ǵ�����Ŀ��
			army_setstate( army_index, ARMY_STATE_REBACK );
			//army_mail_invalid( army_index );
			return;
		}

		City *pCity = army_getcityptr( army_index );
		if ( pUnit->type == MAPUNIT_TYPE_CITY || pUnit->type == MAPUNIT_TYPE_ARMY )
		{ // Ŀ���ǳǳ�
			City *pTargetCity = army_getcityptr_target( army_index );
			if ( !pTargetCity )
			{// Ŀ���Ѿ��Ƿ�
				if ( g_army[army_index].action == ARMY_ACTION_GROUP_START )
				{
					//armygroup_reback_invalid( army_index );
				}
				else
				{
					army_delete( army_index );
				}
				return;
			}
			// ����ǲ��������Ŀ��ǳ�
			if ( pUnit->type == MAPUNIT_TYPE_CITY )
			{
				City *pUnitCity = &g_city[pUnit->index];
				if ( pUnitCity && pTargetCity->actorid != pUnitCity->actorid )
				{
					if ( g_army[army_index].action == ARMY_ACTION_GROUP_START )
					{ // ���ṥ��
						//armygroup_reback_invalid( army_index );
						//armygroup_mail_invalid( g_army[army_index].group_index );
					}
					else
					{
						if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
						{
							army_delete( army_index );
						}
						else
						{
							army_setstate( army_index, ARMY_STATE_REBACK );
							//army_mail_invalid( army_index );
						}
					}
					return;
				}
			}

			if ( g_army[army_index].action == ARMY_ACTION_HELP_TROOP )
			{
				// ����Ƿ������ߴ���
				//short buildingindex = building_getindex( pTargetCity, BUILDING_Embassy );
				//if ( buildingindex < 0 || buildingindex >= CITY_BUILDING_MAX )
				//{
				//	if ( pCity )
				//		actor_system_message( pCity->actor_index, 25 ); // ��Ԯ����û�н������ߴ���
				//	army_setstate( army_index, ARMY_STATE_REBACK );
				//	return;
				//}

				//// ʿ��Ԯ������������
				//int helptroops_free = building_gethelptroops_max( pTargetCity ) - city_helptroops_totalnumber_arrived( pTargetCity );
				//if ( g_army[army_index].troops_totalnum > helptroops_free )
				//{
				//	if ( pCity )
				//		actor_system_message( pCity->actor_index, 26 ); // ��������������Ԯ���߿����ɵ��������
				//	army_setstate( army_index, ARMY_STATE_REBACK );
				//	return;
				//}
			}

			if ( g_army[army_index].action == ARMY_ACTION_HELP_TROOP )
			{ // Ԯ��
				army_setstate( army_index, ARMY_STATE_HELP );
			}
			else if ( g_army[army_index].action == ARMY_ACTION_FIGHT )
			{ // ս��
				City *pTargetCity = city_indexptr( pUnit->index );
				if ( !pTargetCity )
				{
					if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
					{
						army_delete( army_index );
					}
					else
					{
						army_setstate( army_index, ARMY_STATE_REBACK );
						//army_mail_invalid( army_index );
					}
					return;
				}

				// ս���ػ�״̬���
				if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
				{
					if ( pUnit->type == MAPUNIT_TYPE_CITY && pTargetCity->ptsec > 0 )
					{
						army_setstate( army_index, ARMY_STATE_REBACK );
						//army_mail_invalid( army_index );
						return;
					}
				}

				army_setstate( army_index, ARMY_STATE_FIGHT );
				if ( pUnit->type == MAPUNIT_TYPE_CITY )
				{
					//city_setstate( pTargetCity, CITY_STATE_FIGHT );
				}
			}
			else if ( g_army[army_index].action == ARMY_ACTION_OCCUPY )
			{ // פ���㱻ռ��
				army_setstate( army_index, ARMY_STATE_REBACK );
				//army_mail_invalid( army_index );
			}
			else if ( g_army[army_index].action == ARMY_ACTION_GROUP_START )
			{ // ���ṥ��
				City *pTargetCity = &g_city[pUnit->index];
				if ( !pTargetCity )
				{
					//armygroup_reback_invalid( army_index );
					//armygroup_mail_invalid( army_index );
					return;
				}

				// ս���ػ�״̬���
				if ( pUnit->type == MAPUNIT_TYPE_CITY && pTargetCity->ptsec > 0 )
				{
					//armygroup_reback_invalid( army_index );
					//armygroup_mail_invalid( army_index );
					return;
				}

				army_setstate( army_index, ARMY_STATE_FIGHT );
				if ( pUnit->type == MAPUNIT_TYPE_CITY )
				{
					//city_setstate( pTargetCity, CITY_STATE_FIGHT );
				}
			}
			else
			{ // ����Ϊ
				//armygroup_reback_invalid( army_index );
				//armygroup_mail_invalid( army_index );
			}
		}
		else if ( pUnit->type == MAPUNIT_TYPE_TOWN )
		{ // Ŀ���ǳ���
			int invalid = 0;
			int townid = pUnit->index;
		}
		else if ( pUnit->type == MAPUNIT_TYPE_ENEMY )
		{ // Ŀ��������
			army_setstate( army_index, ARMY_STATE_FIGHT );			
		}
		else if ( pUnit->type == MAPUNIT_TYPE_RES )
		{ // Ŀ������Դ
			if ( g_army[army_index].action == ARMY_ACTION_GATHER )
			{ // �ɼ��� Ҫ����Ƿ����˱����ȵ�
				int gather_army_index = map_res_getarmy( pUnit->index );
				if ( gather_army_index >= 0 )
				{
					army_setstate( army_index, ARMY_STATE_REBACK );
					//army_mail_invalid( army_index );
					return;
				}
				map_res_setarmy( g_army[army_index].to_index, army_index );
				army_setstate( army_index, ARMY_STATE_GATHER );
			}
			else
			{ // ս����
				// ����Է�����
				int gather_army_index = map_res_getarmy( pUnit->index );
				if ( gather_army_index < 0 )
				{
					map_res_setarmy( g_army[army_index].to_index, army_index );
					army_setstate( army_index, ARMY_STATE_GATHER );
				}
				else
				{
					army_setstate( army_index, ARMY_STATE_FIGHT );
				}
			}
		}
	}
	else
	{
		// û��Ŀ���ˣ���ֱ�ӷ��ذ�
		if ( g_army[army_index].action == ARMY_ACTION_GROUP_START )
		{ // ���ṥ��
			//armygroup_reback_invalid( army_index );
			//armygroup_mail_invalid( g_army[army_index].group_index );
		}
		else
		{
			if ( g_army[army_index].from_type == MAPUNIT_TYPE_TOWN )
			{
				army_delete( army_index );
			}
			else
			{
				army_setstate( army_index, ARMY_STATE_REBACK );
				//army_mail_invalid( army_index );
			}
		}
	}
}

// ƴ�о�·����Ϣ
int army_marchroute_makeinfo( int army_index, SLK_NetS_AddMarchRoute *pValue )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	pValue->m_army_index = army_index;
	pValue->m_from_type = g_army[army_index].from_type;
	pValue->m_from_posx = g_army[army_index].from_posx;
	pValue->m_from_posy = g_army[army_index].from_posy;
	pValue->m_to_type = g_army[army_index].to_type;
	pValue->m_to_posx = g_army[army_index].to_posx;
	pValue->m_to_posy = g_army[army_index].to_posy;
	pValue->m_action = g_army[army_index].action;
	pValue->m_state = g_army[army_index].state;

	if ( g_army[army_index].from_type == MAPUNIT_TYPE_CITY )
	{ // �����ǳǳ�
		pValue->m_from_actorid = army_getcityid( army_index );
		pValue->m_from_nation = army_getnation( army_index );
		pValue->m_from_grid = 1;
	}

	// Ŀ������
	if ( g_army[army_index].to_type == MAPUNIT_TYPE_CITY )
	{ // Ŀ���ǳǳ�
		pValue->m_to_actorid = army_getcityid_target( army_index );
		pValue->m_to_nation = army_getnation_target( army_index );
		pValue->m_to_grid = 1;
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ARMY )
	{ // Ŀ���ǲ���
		pValue->m_to_actorid = army_getcityid( g_army[army_index].to_index );
		pValue->m_to_nation = army_getnation( g_army[army_index].to_index );
		pValue->m_to_grid = 1;
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_TOWN )
	{ // Ŀ����NPC�ǳ�
		int townid = g_army[army_index].to_index;
		MapTown *town = map_town_getptr( townid );
		if ( town )
		{
			pValue->m_to_nation = town->nation;
			pValue->m_to_grid = map_getobject_grid( MAPUNIT_TYPE_TOWN, townid );
		}
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_ENEMY )
	{ // Ŀ��������
		pValue->m_to_grid = 1;
	}
	else if ( g_army[army_index].to_type == MAPUNIT_TYPE_RES )
	{ // Ŀ������Դ
		int target_armyindex = map_res_getarmy( g_army[army_index].to_index );
		if ( target_armyindex >= 0 )
		{
			pValue->m_to_actorid = army_getcityid( target_armyindex );
			pValue->m_to_nation = army_getnation( target_armyindex );
		}
	}
	return 0;
}

// ��֯һ���о�·������������������ݰ�
int army_enterinfo( int army_index, char *databuf, int *psize )
{
	char tmpbuf[2048];
	int allsize = 0;
	int sizeleft = 2048;
	int cursize = 0;
	SLK_NetS_AddMarchRoute info = { 0 };
	army_marchroute_makeinfo( army_index, &info );

	if ( (cursize = netsend_addmarchroute_S( tmpbuf, sizeleft, &info )) == 0 )
		return -1;
	sizeleft -= cursize;
	allsize += cursize;

	if ( *psize + allsize > 2040 )
	{
		return -1;
	}
	memcpy( databuf + (*psize), tmpbuf, allsize );
	*psize += allsize;
	return 0;
}

// ��֯һ���о�·�������뿪��������ݰ�
int army_leaveinfo( int army_index, char *databuf, int *psize )
{
	char tmpbuf[2048];
	int allsize = 0;
	int sizeleft = 2048;
	int cursize = 0;
	SLK_NetS_DelMarchRoute info = { 0 };
	info.m_army_index = army_index;
	if ( (cursize = netsend_delmarchroute_S( tmpbuf, sizeleft, &info )) == 0 )
		return -1;
	sizeleft -= cursize;
	allsize += cursize;

	if ( *psize + allsize > 2040 )
	{
		return -1;
	}
	memcpy( databuf + (*psize), tmpbuf, allsize );
	*psize += allsize;

	return 0;
}

void army_area_action_add( int army_index, int areax, int areay )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;

	if ( areax < 0 || areax >= g_map.m_nAreaXNum || areay < 0 || areay >= g_map.m_nAreaYNum )
		return;

	int area_index = areay*(g_map.m_nAreaXNum) + areax;

	char tmpbuf[2048];
	int tmpsize = 0;

	area_addtoqueue( area_index, army_index );

	// ��֯���ݰ�
	army_enterinfo( army_index, tmpbuf, &tmpsize );
	area_sendmsg( area_index, tmpsize, tmpbuf );
}

void army_area_action_del( int army_index, int areax, int areay )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;

	if ( areax < 0 || areax >= g_map.m_nAreaXNum || areay < 0 || areay >= g_map.m_nAreaYNum )
		return;

	int area_index = areay*(g_map.m_nAreaXNum) + areax;

	char tmpbuf[2048];
	int tmpsize = 0;

	area_delfromqueue( area_index, army_index );
	// ��֯���ݰ�
	army_leaveinfo( army_index, tmpbuf, &tmpsize );
	area_sendmsg( area_index, tmpsize, tmpbuf );
}


// ����/�����о�·�ߣ����Ӵ����ͷ��صȸ��µ�ʱ����ã�
int army_marchroute_add( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	if ( g_army[army_index].state == ARMY_STATE_IDLE ||
		g_army[army_index].state == ARMY_STATE_GROUP_END ||
		g_army[army_index].state == ARMY_STATE_READY ||
		g_army[army_index].state == ARMY_STATE_FIGHT ||
		g_army[army_index].state == ARMY_STATE_OCCUPY ||
		g_army[army_index].state == ARMY_STATE_GATHER ||
		g_army[army_index].state == ARMY_STATE_SHELTER ||
		g_army[army_index].state == ARMY_STATE_HELP )
		return -1;

	area_effect( army_index, g_army[army_index].from_posx, g_army[army_index].from_posy, g_army[army_index].to_posx, g_army[army_index].to_posy, army_area_action_add );
	return 0;
}

// ɾ���о�·��(��֯�ñ����ӵ����ݣ����͸����й�����������)(����ɾ����ʱ�����)
int army_marchroute_del( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;

	area_effect( army_index, g_army[army_index].from_posx, g_army[army_index].from_posy, g_army[army_index].to_posx, g_army[army_index].to_posy, army_area_action_del );
	return 0;
}

// ������һ����Ľ����¼�
int army_enterarea( int army_index, int area_index )
{
	char msg[2048] = { 0 };
	int size = 0;
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;

	// ��֯���ݰ�
	army_enterinfo( army_index, msg + sizeof( short ), &size );

	// ��Ϣ���͵�����
	if ( size > 0 )
	{
		*(unsigned short *)msg = size;
		area_send( area_index, size + sizeof( short ), msg );
	}
	return 0;
}

// ������һ������뿪�¼�
int army_leavearea( int army_index, int area_index )
{
	char msg[2048] = { 0 };
	int size = 0;
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;

	// ��֯���ݰ�
	army_leaveinfo( army_index, msg + sizeof( short ), &size );

	// ��Ϣ���͵�����
	if ( size > 0 )
	{
		*(unsigned short *)msg = size;
		area_send( area_index, size + sizeof( short ), msg );
	}

	return 0;
}

// ���²����ٶ�
int army_marchspeed_update( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	SLK_NetS_ArmySpeedUpdate info = { 0 };
	info.m_unit_index = g_army[army_index].unit_index;
	info.m_state = g_army[army_index].state;
	info.m_statetime = g_army[army_index].statetime;
	info.m_stateduration = g_army[army_index].stateduration;
	short posx = -1;
	short posy = -1;
	mapunit_getpos( g_army[army_index].unit_index, &posx, &posy );
	int area_index = area_getindex( posx, posy );
	netsend_armyspeedupdate_S( area_index, SENDTYPE_AREA, &info );
	return 0;
}

// ��������
int actor_army_return( int actor_index, int army_index, int unit_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( army_index < 0 || army_index >= g_army_maxcount )
	{
		if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
			return -1;

		MapUnit *unit = &g_mapunit[unit_index];
		if ( !unit )
			return -1;

		if ( unit->type == MAPUNIT_TYPE_RES )
		{
			army_index = map_res_getarmy( unit->index );
			if ( army_index < 0 || army_index >= g_army_maxcount )
				return -1;
		}
		else
		{
			return -1;
		}
	}

	if ( army_myself( actor_index, army_index ) < 0 )
		return -1;

	if ( g_army[army_index].state == ARMY_STATE_FIGHT )
		return -1;

	if ( g_army[army_index].to_type == MAPUNIT_TYPE_RES && g_army[army_index].state == ARMY_STATE_GATHER )
	{
		city_underfire_del_equal( army_getcityptr( army_index ), army_index );
		army_gather_calc( army_index );
	}
	else if ( g_army[army_index].state == ARMY_STATE_OCCUPY )
	{
		city_underfire_del_equal( army_getcityptr( army_index ), army_index );
		map_delobject( MAPUNIT_TYPE_ARMY, army_index, g_army[army_index].posx, g_army[army_index].posy );
	}
	if ( g_army[army_index].action == ARMY_ACTION_HELP_TROOP )
	{
		city_helparmy_del( army_getcityptr_target( army_index ), army_index );
	}
	army_setstate( army_index, ARMY_STATE_REBACK );


	return 0;
}

// �����о��ٻ�
int actor_army_callback( int actor_index, int army_index, int itemkind )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	if ( g_army[army_index].state == ARMY_STATE_FIGHT )
		return -1;
	if ( army_myself( actor_index, army_index ) < 0 )
		return -1;
	if ( g_army[army_index].state != ARMY_STATE_MARCH )
	{
		return -1;
	}
	char instantly = 0;
	if ( itemkind == 139 )
	{ // ���ʹ���о��ٻ�
		instantly = 0;
	}
	else if ( itemkind == 460 )
	{ // ���ʹ�õ��Ǹ߼��о��ٻأ������سǣ�
		instantly = 1;
	}
	else
	{
		return -1;
	}


	if ( item_lost( actor_index, itemkind, 1, PATH_ITEMUSE ) < 0 )
	{ // û�е��ߣ���ôʹ����ʯ
		int price = item_gettoken( itemkind );
		if ( actor_change_token( actor_index, -price, PATH_ITEMUSE, itemkind ) == -2 )
		{
			return -1;
		}
	}

	if ( g_army[army_index].action == ARMY_ACTION_HELP_TROOP )
	{
		city_helparmy_del( army_getcityptr_target( army_index ), army_index );
	}

	if ( g_army[army_index].action == ARMY_ACTION_FIGHT )
	{
		city_underfire_del( army_getcityptr_target( army_index ), army_index );
	}

	if ( instantly == 1 )
	{
		army_delete( army_index );
	}
	else
	{
		army_march_time( army_index );
		army_setstate( army_index, ARMY_STATE_REBACK );
	}
	return 0;
}

// �����о�����
int actor_army_march_speedup( int actor_index, int army_index, int itemkind )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	if ( g_army[army_index].state != ARMY_STATE_MARCH && g_army[army_index].state != ARMY_STATE_REBACK )
		return -1;
	char instantly = 0;
	if ( itemkind == 137 )
	{ // ����߼��о�����
		instantly = 0;
	}
	else if ( itemkind == 138 )
	{ // ���ʹ�ö����о�����
		instantly = 1;
	}
	else
	{
		return -1;
	}

	if ( item_lost( actor_index, itemkind, 1, PATH_ITEMUSE ) < 0 )
	{	// û�е��ߣ���ôʹ����ʯ
		int price = item_gettoken( itemkind );
		if ( actor_change_token( actor_index, -price, PATH_ITEMUSE, itemkind ) == -2 )
		{
			return -1;
		}
	}

	if ( instantly == 0 )
	{
		g_army[army_index].stateduration -= (int)((g_army[army_index].stateduration - g_army[army_index].statetime) * 0.5f);
	}
	else
	{
		g_army[army_index].stateduration = g_army[army_index].statetime + 1;
	}

	if ( g_army[army_index].stateduration < g_army[army_index].statetime )
		g_army[army_index].stateduration = g_army[army_index].statetime;

	army_marchspeed_update( army_index );
	city_battlequeue_sendupdate( army_index );
	return 0;
}

// ����ǲ�������Ĳ���
int actor_helparmy_repatriate( int actor_index, int army_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	// ���Լ���Э���б���
	for ( int tmpi = 0; tmpi < CITY_HELPDEFENSE_MAX; tmpi++ )
	{
		int help_armyindex = pCity->help_armyindex[tmpi];
		if ( help_armyindex < 0 )
			continue;
		if ( help_armyindex == army_index )
		{
			army_setstate( army_index, ARMY_STATE_REBACK );
			city_helparmy_del( pCity, army_index );
			break;
		}
	}
	return 0;
}