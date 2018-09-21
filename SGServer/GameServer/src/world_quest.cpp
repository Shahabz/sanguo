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
#include "wqueue.h"
#include "city.h"
#include "building.h"
#include "actor.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "award.h"
#include "server_netsend_auto.h"
#include "system.h"
#include "quest.h"
#include "world_quest.h"
#include "map_zone.h"
#include "king_war.h"
#include "nation.h"
#include "map_town.h"
#include "activity_22.h"
#include "auto_data_world_questinfo.h"
#include "auto_data_map_zoneinfo.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

extern City *g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern WorldQuestInfo *g_worldquestinfo;
extern int g_worldquestinfo_maxnum;

extern MapZoneInfo *g_zoneinfo;
extern int g_zoneinfo_maxnum;

int *g_world_questlist = NULL;
int g_world_questid = 0;

char g_open_zone_sili = 0;
char g_open_zone_luoyang = 0;

int worldquest_init()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];

	// �����б�
	int questlist_index = 0;
	g_world_questlist = (int *)malloc( sizeof( int )*g_worldquestinfo_maxnum );
	memset( g_world_questlist, 0, sizeof( int )*g_worldquestinfo_maxnum );

	// ����˳���ȡ�����б�
	sprintf( szSQL, "select questid from world_questinfo order by sort asc;" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while ( (row = mysql_fetch_row( res )) )
	{
		int questid = atoi( row[0] );
		if ( questlist_index >= 0 && questlist_index <= g_worldquestinfo_maxnum )
		{
			g_world_questlist[questlist_index] = questid;
			questlist_index += 1;
		}
	}
	mysql_free_result( res );
	return 0;
}

// �ض�
int worldquest_reload()
{
	if ( g_world_questlist )
	{
		free( g_world_questlist );
		g_world_questlist = NULL;
	}
	worldquest_init();
	return 0;
}

// ��ȡ��ǰ����������
int worldquest_give( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int give_questid = 0;
	for ( int tmpi = 0; tmpi < g_worldquestinfo_maxnum; tmpi++ )
	{
		int questid = g_world_questlist[tmpi];
		if ( questid <= 0 || questid >= g_worldquestinfo_maxnum )
			continue;
	
		// �����Ѿ���ɣ��Ѿ���ȡ������
		if ( worldquest_getcomplete( actor_index, questid, NULL ) == 1 )
			continue;

		// ǰ������û���
		int preid = g_worldquestinfo[questid].preid;
		if ( preid > 0 && worldquest_getcomplete( actor_index, preid, NULL ) == 0 )
			continue;

		give_questid = questid;
		if ( g_actors[actor_index].worldquestid != questid )
		{
			g_actors[actor_index].worldquestid = questid;
			break;
		}
	}
	return 0;
}

// �������������ֵ
int worldquest_addvalue( int questid, int value )
{
	if ( questid <= 0 || questid >= g_worldquestinfo_maxnum )
		return 0;
	if ( g_worldquestinfo[questid].type != 1 )
		return 0;
	int saveindex = g_worldquestinfo[questid].saveindex;
	if ( saveindex < 0 || saveindex >= 16 )
		return 0;
	int datavalue = world_data_getcache( WORLD_DATA_WORLDQUEST_BASE + saveindex );
	datavalue += value;
	world_data_set( WORLD_DATA_WORLDQUEST_BASE + saveindex, datavalue, NULL, NULL );
	return 0;
}

// ��������������ֵ
int worldquest_setvalue( int questid, int value )
{
	if ( questid <= 0 || questid >= g_worldquestinfo_maxnum )
		return 0;
	if ( g_worldquestinfo[questid].type != 1 )
		return 0;
	int saveindex = g_worldquestinfo[questid].saveindex;
	if ( saveindex < 0 || saveindex >= 16 )
		return 0;
	int datavalue = world_data_getcache( WORLD_DATA_WORLDQUEST_BASE + saveindex );
	if ( datavalue > 0 )
	{
		return 0;
	}
	world_data_set( WORLD_DATA_WORLDQUEST_BASE + saveindex, value, NULL, NULL );

	g_open_zone_sili = worldquest_check_server( WORLDQUEST_WORLDBOSS1 );
	g_open_zone_luoyang = worldquest_check_server( WORLDQUEST_WORLDBOSS2 );

	if ( questid == WORLDQUEST_WORLDBOSS1 || questid == WORLDQUEST_WORLDBOSS2 )
	{
		// ֪ͨȫ���������
		for ( int tmpi = 0; tmpi < g_maxactornum; tmpi++ )
		{
			if ( g_actors[tmpi].actorid > 0 )
			{
				worldquest_give( tmpi );
				worldquest_sendinfo( tmpi );
			}
		}

		if ( questid == WORLDQUEST_WORLDBOSS1 )
		{// ���ܻ��ι�-ˢ˾��Ұ�ֺ���Դ��
			brush_enemy_queue_add( 0, MAPZONE_CENTERID );
			brush_enemy_queue_add( 1, MAPZONE_CENTERID );

			// ���ö���
			nation_capital_townid( 1, 191 );
			nation_people_capital_set( 1, 0 );

			nation_capital_townid( 2, 193 );
			nation_people_capital_set( 2, 0 );

			nation_capital_townid( 3, 192 );
			nation_people_capital_set( 3, 0 );

			map_town_attack_checkstart();

			// ���Ź�Աϵͳ
			nation_official_open();
		}
		else if ( questid == WORLDQUEST_WORLDBOSS2 )
		{ // ���ܶ�׿-����Ѫս�ʳ�
			kingwar_activity_open();
		}
		// ����
		worldquest_updateopen();
	}
	else
	{
		// ֪ͨȫ���������
		for ( int tmpi = 0; tmpi < g_maxactornum; tmpi++ )
		{
			if ( g_actors[tmpi].actorid > 0 )
			{
				worldquest_checkcomplete( tmpi, 1 );
			}
		}

		if ( questid == WORLDQUEST_ID6 )
		{// ��������
			// ��������ϵͳ
			weather_open();
			// ����
			worldquest_updateopen();
		}
	}
	return 0;
}

// �����������
int worldquest_setcomplete( int actor_index, int questid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return 0;
	g_actors[actor_index].worldquest_complete |= (1 << questid);
	return 0;
}

// ��ȡ�����Ƿ����
int worldquest_getcomplete( int actor_index, int questid, int *value )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return 0;
	int complete = g_actors[actor_index].worldquest_complete & (1 << questid);
	if ( complete > 0 )
	{
		return 1;
	}
	else if ( questid == WORLDQUEST_WORLDBOSS1 || questid == WORLDQUEST_WORLDBOSS2 )
	{
		if ( worldquest_check( actor_index, questid, value ) == 1 )
		{
			return 1;
		}
	}
	return 0;
}

// ������
int worldquest_check( int actor_index, int questid, int *value )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return 0;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return 0;
	if ( questid <= 0 || questid >= g_worldquestinfo_maxnum )
		return 0;
	if ( g_worldquestinfo[questid].type == 0 )
	{// ����
		int saveindex = g_worldquestinfo[questid].saveindex;
		if ( saveindex < 0 || saveindex >= 16 )
			return 0;
		int datav = data_record_getvalue( pCity, saveindex );
		if ( value )
			*value = datav;
		if ( datav >= g_worldquestinfo[questid].value )
		{
			return 1;
		}
	}
	else if ( g_worldquestinfo[questid].type == 1 )
	{ // ȫ��
		int saveindex = g_worldquestinfo[questid].saveindex;
		if ( saveindex < 0 || saveindex >= 16 )
			return 0;
		int datav = world_data_getcache( WORLD_DATA_WORLDQUEST_BASE + saveindex );
		if ( value )
			*value = datav;
		if ( datav > 0 )
		{
			return 1;
		}
	}
	return 0;
}

// ������
int worldquest_check_server( int questid )
{
	if ( questid <= 0 || questid >= g_worldquestinfo_maxnum )
		return 0;
	if ( g_worldquestinfo[questid].type == 1 )
	{ // ȫ��
		int saveindex = g_worldquestinfo[questid].saveindex;
		if ( saveindex < 0 || saveindex >= 16 )
			return 0;
		int datav = world_data_getcache( WORLD_DATA_WORLDQUEST_BASE + saveindex );
		if ( datav > 0 )
		{
			return 1;
		}
	}
	return 0;
}

// ������������
int worldquest_checkcomplete( int actor_index, char type )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return 0;
	int questid = g_actors[actor_index].worldquestid;
	if ( questid <= 0 || questid >= g_worldquestinfo_maxnum )
		return 0;
	if ( g_worldquestinfo[questid].type != type )
		return 0;
	if ( g_actors[actor_index].view_areaindex < 0 )
		return 0;
	worldquest_sendinfo( actor_index );
	return 0;
}

// ����������Ϣ
int worldquest_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_WorldQuest pValue = { 0 };
	int questid = g_actors[actor_index].worldquestid;
	if ( questid > 0 && questid < g_worldquestinfo_maxnum-1 )// -1,�����ʳ�Ѫս
	{
		int datav = 0;
		int complete = worldquest_check( actor_index, questid, &datav );
		pValue.m_questid = questid;
		pValue.m_complete = complete;
		pValue.m_value = datav;
		pValue.m_maxvalue = g_worldquestinfo[questid].value;
		int saveindex = g_worldquestinfo[questid].saveindex;
		if ( saveindex >= 0 && saveindex < 16 )
		{
			int datavalue = world_data_getcache( WORLD_DATA_WORLDQUEST_BASE + saveindex );
			if ( datavalue == pCity->nation )
			{
				pValue.m_dd = 1; // ˫��
			}
		}
	}
	else
	{
		pValue.m_questid = 0;
	}
	netsend_worldquest_S( actor_index, SENDTYPE_ACTOR, &pValue );

	// �Ƿ���Ҫ֪ͨǰ��˾��
	if ( worldquest_check( actor_index, WORLDQUEST_WORLDBOSS1, NULL ) == 1 )
	{
		map_zone_goto_sili_send( actor_index );
	}

	// �Ƿ���Ҫ֪ͨѪս�ʳ�
	if ( pValue.m_questid == 0 )
	{
		activity22_mapsendinfo( actor_index );
		//kingwar_activity_sendinfo( actor_index );
		//kingwar_treasure_sendinfo( actor_index );
	}
	return 0;
}

// ��ȡ������
int worldquest_sendaward( int actor_index, int questid )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( questid <= 0 || questid >= g_worldquestinfo_maxnum )
		return 0;
	awardgroup_sendinfo( actor_index, g_worldquestinfo[questid].awardgroup, 3, questid, 4 );
	return 0;
}

// ��ȡ����
int worldquest_getaward( int actor_index, int questid )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( questid <= 0 || questid >= g_worldquestinfo_maxnum )
		return 0;
	if ( worldquest_getcomplete( actor_index, questid, NULL ) == 1 )
		return -1;
	if ( worldquest_check( actor_index, questid, NULL ) <= 0 )
		return -1;

	char dd = 1;
	if ( questid == WORLDQUEST_ID4 || questid == WORLDQUEST_ID5 || questid == WORLDQUEST_ID6 )
	{
		int saveindex = g_worldquestinfo[questid].saveindex;
		if ( saveindex >= 0 && saveindex < 16 )
		{
			int datavalue = world_data_getcache( WORLD_DATA_WORLDQUEST_BASE + saveindex );
			if ( datavalue == pCity->nation )
			{
				dd = 2;
			}
		}
	}

	AwardGetInfo getinfo = { 0 };
	for ( int tmpi = 0; tmpi < dd; tmpi++ )
	{
		awardgroup_withindex( actor_index, g_worldquestinfo[questid].awardgroup, -1, PATH_QUEST, &getinfo );
	}
	
	worldquest_setcomplete( actor_index, questid );
	worldquest_give( actor_index );
	worldquest_sendinfo( actor_index );
	return 0;
}

int worldquest_updateopen()
{
	SLK_NetS_WorldDataOpen pValue = { 0 };
	pValue.m_open_zone_sili = g_open_zone_sili;
	pValue.m_open_zone_luoyang = g_open_zone_luoyang;
	netsend_worlddataopen_S( 0, SENDTYPE_WORLD, &pValue );
	return 0;
}

int worldquest_gmclear( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	g_actors[actor_index].worldquest_complete = 0;
	worldquest_give( actor_index );
	worldquest_sendinfo( actor_index );
	return 0;
}
