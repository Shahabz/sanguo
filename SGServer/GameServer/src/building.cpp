#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "global.h"
#include "actor.h"
#include "actor_notify.h"
#include "city.h"
#include "building.h"
#include "mapunit.h"
#include "map.h"
#include "equip.h"
#include "quest.h"
#include "city_attr.h"
#include "item.h"
#include "hero.h"
#include "vip.h"
#include "quest.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;
extern Map g_map;

extern Actor *g_actors;
extern int g_maxactornum;

extern int g_city_maxindex;
extern City *g_city;
extern int g_city_maxcount;

extern BuildingUpgrade *g_building_upgrade;
extern int g_building_upgrade_maxnum;

extern BuildingResUnlock *g_building_res_unlock;
extern int g_building_res_unlock_maxnum;

// ��ȡ����ָ��
inline Building* building_getptr( int city_index, int offset )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	if ( offset < 0 || offset >= BUILDING_MAXNUM )
		return NULL;
	return &g_city[city_index].building[offset];
}

inline Building* building_getptr_kind( int city_index, int kind )
{
	for ( int tmpi = 0; tmpi < BUILDING_MAXNUM; tmpi++ )
	{
		if ( g_city[city_index].building[tmpi].kind == kind )
		{
			return &g_city[city_index].building[tmpi];
		}
	}
	return NULL;
}

inline BuildingBarracks* buildingbarracks_getptr( int city_index, int offset )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	if ( offset < 0 || offset >= BUILDING_BARRACKS_MAXNUM )
		return NULL;
	return &g_city[city_index].building_barracks[offset];
}

inline BuildingBarracks* buildingbarracks_getptr_kind( int city_index, int kind )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	for ( int tmpi = 0; tmpi < BUILDING_BARRACKS_MAXNUM; tmpi++ )
	{
		if ( g_city[city_index].building_barracks[tmpi].kind == kind )
		{
			return &g_city[city_index].building_barracks[tmpi];
		}
	}
	return NULL;
}

inline BuildingRes* buildingres_getptr( int city_index, int offset )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	if ( offset < 0 || offset >= BUILDING_RES_MAXNUM )
		return NULL;
	return &g_city[city_index].building_res[offset];
}

BuildingRes *buildingres_getptr_number( int city_index, int kind, int no )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	if ( no < 0 || no >= 16 )
		return NULL;
	if ( kind == BUILDING_Silver )
	{
		return &g_city[city_index].building_res[no];
	}
	else if ( kind == BUILDING_Wood )
	{
		return &g_city[city_index].building_res[16+no];
	}
	else if ( kind == BUILDING_Food )
	{
		return &g_city[city_index].building_res[32 + no];
	}
	else if ( kind == BUILDING_Iron )
	{
		return &g_city[city_index].building_res[48 + no];
	}
	return NULL;
}

// ��ȡ������������Ϣ
inline BuildingUpgradeConfig* building_getconfig( int kind, int level )
{
	if ( kind < 0 || kind >= g_building_upgrade_maxnum )
		return NULL;
	if ( level < 0 || level >= g_building_upgrade[kind].maxnum )
		return NULL;
	return &g_building_upgrade[kind].config[level];
}

// ��������������
inline int building_sec( City *pCity, BuildingUpgradeConfig *config )
{
	return (int)( config->sec / (float)( 1.0f + pCity->attr.buildingsec_per) );
}

inline int building_offset2no( int kind, int offset )
{
	if ( offset <= 0 )
		return 0;
	return offset%16;
}

int building_getindex( int city_index, int kind )
 {
	CITY_CHECK_INDEX( city_index );
	int index = -1;
	if ( kind < BUILDING_Infantry )
	{ // ��ͨ����
		for ( int tmpi = 0; tmpi < BUILDING_MAXNUM; tmpi++ )
		{
			if ( g_city[city_index].building[tmpi].kind == kind )
			{
				index = tmpi;
				break;
			}
		}
	}
	else if ( kind < BUILDING_Silver )
	{ // ��Ӫ����
		for ( int tmpi = 0; tmpi < BUILDING_BARRACKS_MAXNUM; tmpi++ )
		{
			if ( g_city[city_index].building_barracks[tmpi].kind == kind )
			{
				index = tmpi;
				break;
			}
		}
	}
	return index;
}


// ��ȡ�����ȼ�
int building_getlevel( int city_index, int kind, int no )
{
	CITY_CHECK_INDEX( city_index );
	if ( kind < BUILDING_Infantry )
	{ // ��ͨ����
		for ( int tmpi = 0; tmpi < BUILDING_MAXNUM; tmpi++ )
		{
			if ( g_city[city_index].building[tmpi].kind == kind )
			{
				return g_city[city_index].building[tmpi].level;
			}
		}
	}
	else if ( kind < BUILDING_Silver )
	{ // ��Ӫ����
		for ( int tmpi = 0; tmpi < BUILDING_BARRACKS_MAXNUM; tmpi++ )
		{
			if ( g_city[city_index].building_barracks[tmpi].kind == kind )
			{
				return g_city[city_index].building_barracks[tmpi].level;
			}
		}
	}
	else if ( kind >= BUILDING_Silver && kind <= BUILDING_Iron )
	{
		BuildingRes *pRes = buildingres_getptr_number( city_index, kind, no );
		if ( pRes )
			return pRes->level;
	}
	return 0;
}

// ����һ������
int building_give( int city_index, int kind, int num )
{
	CITY_CHECK_INDEX( city_index );
	if ( kind < BUILDING_Infantry )
	{ // ��ͨ����
		int offset = building_create( city_index, kind, -1 );
		if ( offset >= 0 )
		{
			if ( kind == BUILDING_Craftsman )
			{ // ��ò�������ʱ��������˿�
				BuildingUpgradeConfig *config = building_getconfig( BUILDING_Main, g_city[city_index].building[0].level );
				if ( config )
				{
					g_city[city_index].people = config->value[0];
				}
			}
			SLK_NetS_BuildingGet pValue = { 0 };
			building_makestruct( &g_city[city_index].building[offset], offset, &pValue.m_building );
			netsend_buildingget_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
			city_building_save_auto( g_city[city_index].actorid, offset, &g_city[city_index].building[offset], "city_building", NULL );
		}
	}
	else if ( kind < BUILDING_Silver )
	{ // ��Ӫ����
		int offset = building_create( city_index, kind, -1 );
		if ( offset >= 0 )
		{
			SLK_NetS_BuildingBarracksGet pValue = { 0 };
			building_barracks_makestruct( &g_city[city_index].building_barracks[offset], offset, &pValue.m_barracks );
			netsend_buildingbarracksget_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
			city_building_barracks_save_auto( g_city[city_index].actorid, offset, &g_city[city_index].building_barracks[offset], "city_building_barracks", NULL );
		}
	}
	else if ( kind < BUILDING_Smithy )
	{ // ��Դ����
		num = num <= 0 ? 1 : num;
		for ( int tmpi = 0; tmpi < num; tmpi++ )
		{
			int offset = building_create( city_index, kind, -1 );
			if ( offset >= 0 )
			{
				SLK_NetS_BuildingResGet pValue = { 0 };
				building_res_makestruct( &g_city[city_index].building_res[offset], offset, &pValue.m_res );
				netsend_buildingresget_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
				city_building_res_save_auto( g_city[city_index].actorid, offset, &g_city[city_index].building_res[offset], "city_building_res", NULL );
			}
		}
		
	}
	else
	{
		building_create( city_index, kind, -1 );
	}
	return 0;
}

// ����һ����Դ�㣨δ����״̬�ģ�
int building_giveres( int city_index, int kind )
{
	if ( kind < BUILDING_Silver || kind > BUILDING_Iron )
		return -1;
	int offset = -1;
	if ( kind == BUILDING_Silver )
	{
		for ( int tmpi = 0; tmpi < 16; tmpi++ )
		{
			if ( g_city[city_index].building_res[tmpi].kind <= 0 )
			{
				g_city[city_index].building_res[tmpi].kind = kind;
				g_city[city_index].building_res[tmpi].level = 0;
				offset = tmpi;
				break;
			}
		}
	}
	else if ( kind == BUILDING_Wood )
	{
		for ( int tmpi = 16; tmpi < 32; tmpi++ )
		{
			if ( g_city[city_index].building_res[tmpi].kind <= 0 )
			{
				g_city[city_index].building_res[tmpi].kind = kind;
				g_city[city_index].building_res[tmpi].level = 0;
				offset = tmpi;
				break;
			}
		}
	}
	else if ( kind == BUILDING_Food )
	{
		for ( int tmpi = 32; tmpi < 48; tmpi++ )
		{
			if ( g_city[city_index].building_res[tmpi].kind <= 0 )
			{
				g_city[city_index].building_res[tmpi].kind = kind;
				g_city[city_index].building_res[tmpi].level = 0;
				offset = tmpi;
				break;
			}
		}
	}
	else if ( kind == BUILDING_Iron )
	{
		for ( int tmpi = 48; tmpi < 64; tmpi++ )
		{
			if ( g_city[city_index].building_res[tmpi].kind <= 0 )
			{
				g_city[city_index].building_res[tmpi].kind = kind;
				g_city[city_index].building_res[tmpi].level = 0;
				offset = tmpi;
				break;
			}
		}
	}
	if ( offset < 0 )
		return -1;

	SLK_NetS_BuildingResGet pValue = { 0 };
	building_res_makestruct( &g_city[city_index].building_res[offset], offset, &pValue.m_res );
	netsend_buildingresget_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	city_building_res_save_auto( g_city[city_index].actorid, offset, &g_city[city_index].building_res[offset], "city_building_res", NULL );
	return 0;
}

int building_giveres_build( int actor_index, int kind, int offset )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( kind < BUILDING_Silver || kind > BUILDING_Iron )
		return -1;
	if ( kind >= g_building_res_unlock_maxnum )
		return -1;
	int no = building_offset2no( kind, offset );
	if ( no < 0 || no >= g_building_res_unlock[kind].maxnum )
		return -1;
	BuildingRes *pBuilding = buildingres_getptr( pCity->index, offset );
	if ( !pBuilding )
		return -1;
	if ( pBuilding->kind != kind )
		return -1;
	int itemkind = g_building_res_unlock[kind].config[no].itemkind;
	int itemnum = g_building_res_unlock[kind].config[no].itemnum;
	if ( itemkind <= 0 )
	{
		if ( pBuilding->level == 0 )
			pBuilding->level = 1;
		building_sendinfo_res( actor_index, offset );
		return -1;
	}

	if ( item_getitemnum( actor_index, itemkind ) < itemnum )
	{
		return -1;
	}
	item_lost( actor_index, itemkind, itemnum, PATH_SYSTEM );
	pBuilding->level = 1;
	building_sendinfo_res( actor_index, offset );

	// �Զ�������
	building_upgrade_autocheck( pCity->index );
	return 0;
}

// ��������
int building_create( int city_index, int kind, int offset )
{
	CITY_CHECK_INDEX( city_index );
	if ( kind < BUILDING_Infantry )
	{ // ��ͨ����
		int level = building_getlevel( city_index, kind, -1 );
		if ( level > 0 )
			return -1;

		Building *pBuilding = building_getptr( city_index, offset );
		if ( pBuilding )
		{ // ָ����λ��
			pBuilding->kind = kind;
			pBuilding->level = 1;
		}
		else
		{ // û��ָ����һ����λ
			for ( int tmpi = 0; tmpi < BUILDING_MAXNUM; tmpi++ )
			{
				if ( g_city[city_index].building[tmpi].kind <= 0 )
				{
					g_city[city_index].building[tmpi].kind = kind;
					g_city[city_index].building[tmpi].level = 1;
					offset = tmpi;
					break;
				}
			}
		}
	}
	else if ( kind < BUILDING_Silver )
	{ // ��Ӫ����
		int level = building_getlevel( city_index, kind, -1 );
		if ( level > 0 )
			return -1;

		BuildingBarracks *pBuilding = buildingbarracks_getptr( city_index, offset );
		if ( pBuilding )
		{ // ָ����λ��
			pBuilding->kind = kind;
			pBuilding->level = 1;
		}
		else
		{ // û��ָ����һ����λ
			for ( int tmpi = 0; tmpi < BUILDING_BARRACKS_MAXNUM; tmpi++ )
			{
				if ( g_city[city_index].building_barracks[tmpi].kind <= 0 )
				{
					g_city[city_index].building_barracks[tmpi].kind = kind;
					g_city[city_index].building_barracks[tmpi].level = 1;
					offset = tmpi;
					break;
				}
			}
		}
	}
	else if ( kind < BUILDING_Smithy )
	{ // ��Դ����
		BuildingRes *pBuilding = buildingres_getptr( city_index, offset );
		if ( pBuilding )
		{ // ָ����λ��
			pBuilding->kind = kind;
			pBuilding->level = 1;
		}
		else
		{ // û��ָ����һ����λ
			if ( kind == BUILDING_Silver )
			{
				for ( int tmpi = 0; tmpi < 16; tmpi++ )
				{
					if ( g_city[city_index].building_res[tmpi].kind <= 0 )
					{
						g_city[city_index].building_res[tmpi].kind = kind;
						g_city[city_index].building_res[tmpi].level = 1;
						offset = tmpi;
						break;
					}
				}
			}
			else if ( kind == BUILDING_Wood )
			{
				for ( int tmpi = 16; tmpi < 32; tmpi++ )
				{
					if ( g_city[city_index].building_res[tmpi].kind <= 0 )
					{
						g_city[city_index].building_res[tmpi].kind = kind;
						g_city[city_index].building_res[tmpi].level = 1;
						offset = tmpi;
						break;
					}
				}
			}
			else if ( kind == BUILDING_Food )
			{
				for ( int tmpi = 32; tmpi < 48; tmpi++ )
				{
					if ( g_city[city_index].building_res[tmpi].kind <= 0 )
					{
						g_city[city_index].building_res[tmpi].kind = kind;
						g_city[city_index].building_res[tmpi].level = 1;
						offset = tmpi;
						break;
					}
				}
			}
			else if ( kind == BUILDING_Iron )
			{
				for ( int tmpi = 48; tmpi < 64; tmpi++ )
				{
					if ( g_city[city_index].building_res[tmpi].kind <= 0 )
					{
						g_city[city_index].building_res[tmpi].kind = kind;
						g_city[city_index].building_res[tmpi].level = 1;
						offset = tmpi;
						break;
					}
				}
				city_set_sflag( &g_city[city_index], CITY_FUNCTION_Iron, 1 );
			}
			
		}
	}
	else
	{ // ��ڹ���
		if ( kind == BUILDING_Smithy )
			city_function_open( &g_city[city_index], CITY_FUNCTION_SMITHY );
		else if ( kind == BUILDING_Wash )
			city_function_open( &g_city[city_index], CITY_FUNCTION_WASH );
		else if ( kind == BUILDING_Fangshi )
			city_function_open( &g_city[city_index], CITY_FUNCTION_FANGSHI );
		else if ( kind == BUILDING_Shop )
			city_function_open( &g_city[city_index], CITY_FUNCTION_SHOP );
		else if ( kind == BUILDING_Hero )
			city_function_open( &g_city[city_index], CITY_FUNCTION_HERO );
		else if ( kind == BUILDING_Wishing )
			city_function_open( &g_city[city_index], CITY_FUNCTION_WISHING );
	}
	return offset;
}

// ��������
int building_upgrade( int city_index, int kind, int offset )
{
	CITY_CHECK_INDEX( city_index );
	int level = 0;
	if ( kind < BUILDING_Infantry )
	{ // ��ͨ����
		Building *pBuilding = building_getptr_kind( city_index, kind );
		if ( !pBuilding )
			return -1;
		if ( pBuilding->sec > 0 )
			return -1;
		level = pBuilding->level;
	}
	else if ( kind < BUILDING_Silver )
	{ // ��Ӫ����
		BuildingBarracks *pBuilding = buildingbarracks_getptr_kind( city_index, kind );
		if ( !pBuilding )
			return -1;
		if ( pBuilding->trainsec > 0 )
			return -1;
		level = pBuilding->level;
	}
	else if ( kind < BUILDING_Smithy )
	{ // ��Դ����
		BuildingRes *pBuilding = buildingres_getptr( city_index, offset );
		if ( !pBuilding )
			return -1;
		level = pBuilding->level;
	}
	else
	{
		return -1;
	}

	if ( level >= g_building_upgrade[kind].maxnum - 1 )
	{
		return -1;
	}
	
	BuildingUpgradeConfig *config = building_getconfig( kind, level+1 );
	if ( !config )
		return -1;

	// �ٸ��ȼ��Ƿ�����
	if ( city_mainlevel( city_index ) < config->citylevel )
		return -1;

	// ��ɫ�ȼ��Ƿ�����
	if ( g_city[city_index].level < config->actorlevel )
		return -1;

	// ��Դ�Ƿ�����
	if ( g_city[city_index].silver < config->silver )
		return -1;
	if ( g_city[city_index].wood < config->wood )
		return -1;
	if ( g_city[city_index].food < config->food )
		return -1;
	if ( g_city[city_index].iron < config->iron )
		return -1;

	char ok = 0;
	if ( g_city[city_index].worker_sec <= 0 )
	{ // ��ͨ�������
		g_city[city_index].worker_op = BUILDING_OP_UPGRADE;
		g_city[city_index].worker_sec = building_sec( &g_city[city_index], config );
		g_city[city_index].worker_kind = kind;
		g_city[city_index].worker_offset = offset;
		g_city[city_index].worker_free = 1;
		g_city[city_index].wnsec = g_city[city_index].worker_sec;
		g_city[city_index].wnquick = 0;
		ok = 1;
	}
	else if ( g_city[city_index].worker_expire_ex > 0 && g_city[city_index].worker_sec_ex <= 0 )
	{ // ���ý������
		g_city[city_index].worker_op_ex = BUILDING_OP_UPGRADE;
		g_city[city_index].worker_sec_ex = building_sec( &g_city[city_index], config );
		g_city[city_index].worker_kind_ex = kind;
		g_city[city_index].worker_offset_ex = offset;
		g_city[city_index].worker_free_ex = 1;
		g_city[city_index].wnsec_ex = g_city[city_index].worker_sec_ex;
		g_city[city_index].wnquick_ex = 0;
		ok = 1;
	}
	else
	{ // ������з�æ
		return -1;
	}

	if ( ok )
	{
		// �ۼ���Դ
		city_changesilver( city_index, -config->silver, PATH_BUILDING_UPGRADE );
		city_changewood( city_index, -config->wood, PATH_BUILDING_UPGRADE );
		city_changefood( city_index, -config->food, PATH_BUILDING_UPGRADE );
		city_changeiron( city_index, -config->iron, PATH_BUILDING_UPGRADE );

		// ֪ͨ��������״̬
		if ( kind < BUILDING_Infantry )
		{
			building_sendinfo( g_city[city_index].actor_index, kind );
		}
		else if ( kind < BUILDING_Silver )
		{
			building_sendinfo_barracks( g_city[city_index].actor_index, kind );
		}
		else if ( kind < BUILDING_Smithy )
		{
			building_sendinfo_res( g_city[city_index].actor_index, offset );
		}
		building_sendworker( g_city[city_index].actor_index );

		// ����
		quest_addvalue( &g_city[city_index], QUEST_DATATYPE_BUILDING_UPGRADE, kind, offset, level+1 );
	}
	return 0;
}

// �����Զ�����
int building_upgrade_auto( int city_index, int kind, int offset )
{
	CITY_CHECK_INDEX( city_index );
	if ( g_city[city_index].autobuildopen == 0 )
		return -1;
	if ( g_city[city_index].autobuild <= 0 )
	{
		g_city[city_index].autobuildopen = 0;
		city_change_autobuild( city_index, 0, PATH_SYSTEM );
		return -1;
	}
	if ( building_upgrade( city_index, kind, offset ) >= 0 )
	{
		city_change_autobuild( city_index, -1, PATH_SYSTEM );
	}
	return 0;
}

// �Զ�������
int building_upgrade_autocheck( int city_index )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return -1;
	if ( g_city[city_index].autobuildopen == 0 )
		return -1;
	if ( g_city[city_index].autobuild <= 0 )
	{
		g_city[city_index].autobuildopen = 0;
		return -1;
	}

	// ���н�����Ƿ�����
	char worker = 0;
	if ( g_city[city_index].worker_sec <= 0 )
	{
		worker = 1;
	}
	else if ( g_city[city_index].worker_sec_ex <= 0 && g_city[city_index].worker_expire_ex > 0 )
	{
		worker = 2;
	}

	if ( worker == 0 )
		return -1;

	char kind = 0;
	char offset = -1;
	// ����������
	QuestInfo *questinfo = quest_config( g_city[city_index].questid[0] );
	if ( questinfo )
	{
		if ( questinfo->datatype == QUEST_DATATYPE_BUILDING_LEVEL ||
			questinfo->datatype == QUEST_DATATYPE_BUILDING_UPGRADE ||
			questinfo->datatype == QUEST_DATATYPE_BUILDING_SILVER ||
			questinfo->datatype == QUEST_DATATYPE_BUILDING_WOOD ||
			questinfo->datatype == QUEST_DATATYPE_BUILDING_FOOD ||
			questinfo->datatype == QUEST_DATATYPE_BUILDING_IRON )
		{
			if ( questinfo->datakind < BUILDING_Infantry )
			{ // ��ͨ����
				for ( int tmpi = 0; tmpi < BUILDING_MAXNUM; tmpi++ )
				{
					if ( g_city[city_index].building[tmpi].kind <= 0 )
						continue;
					if ( g_city[city_index].building[tmpi].kind == g_city[city_index].worker_kind || g_city[city_index].building[tmpi].kind == g_city[city_index].worker_kind_ex )
						continue;
					if ( g_city[city_index].building[tmpi].kind != questinfo->datakind )
						continue;
					BuildingUpgradeConfig *config = building_getconfig( g_city[city_index].building[tmpi].kind, g_city[city_index].building[tmpi].level + 1 );
					if ( config )
					{
						// ��ɫ�ȼ��Ƿ�����
						if ( g_city[city_index].level < config->actorlevel )
							continue;
						// �ٸ��ȼ��Ƿ�����
						if ( city_mainlevel( city_index ) < config->citylevel )
							continue;
						// ��Դ�Ƿ�����
						if ( g_city[city_index].silver < config->silver )
							continue;
						if ( g_city[city_index].wood < config->wood )
							continue;
						//if ( g_city[city_index].food < config->food )
						//	continue;
						//if ( g_city[city_index].iron < config->iron )
						//	continue;
						kind = g_city[city_index].building[tmpi].kind;
						offset = tmpi;
						break;
					}
				}
			}
			else if ( questinfo->datakind < BUILDING_Silver )
			{ // ��Ӫ����
				for ( int tmpi = 0; tmpi < BUILDING_BARRACKS_MAXNUM; tmpi++ )
				{
					if ( g_city[city_index].building_barracks[tmpi].kind <= 0 || g_city[city_index].building_barracks[tmpi].level <= 0 )
						continue;
					if ( g_city[city_index].building_barracks[tmpi].kind == g_city[city_index].worker_kind || g_city[city_index].building_barracks[tmpi].kind == g_city[city_index].worker_kind_ex )
						continue;
					if ( g_city[city_index].building_barracks[tmpi].kind != questinfo->datakind )
						continue;
					if ( g_city[city_index].building_barracks[tmpi].trainsec > 0 )
						continue;
					BuildingUpgradeConfig *config = building_getconfig( g_city[city_index].building_barracks[tmpi].kind, g_city[city_index].building_barracks[tmpi].level + 1 );
					if ( config )
					{
						// ��ɫ�ȼ��Ƿ�����
						if ( g_city[city_index].level < config->actorlevel )
							continue;
						// �ٸ��ȼ��Ƿ�����
						if ( city_mainlevel( city_index ) < config->citylevel )
							continue;
						// ��Դ�Ƿ�����
						if ( g_city[city_index].silver < config->silver )
							continue;
						if ( g_city[city_index].wood < config->wood )
							continue;
						//if ( g_city[city_index].food < config->food )
						//	continue;
						//if ( g_city[city_index].iron < config->iron )
						//	continue;
						kind = g_city[city_index].building_barracks[tmpi].kind;
						offset = tmpi;
						break;
					}
				}
			}
			else if ( questinfo->datakind < BUILDING_Smithy )
			{ // ��Դ����
				for ( int tmpi = 0; tmpi < BUILDING_RES_MAXNUM; tmpi++ )
				{
					if ( g_city[city_index].building_res[tmpi].kind <= 0 || g_city[city_index].building_res[tmpi].level <= 0 )
						continue;
					if ( g_city[city_index].building_res[tmpi].kind == g_city[city_index].worker_kind || g_city[city_index].building_res[tmpi].kind == g_city[city_index].worker_kind_ex )
						continue;
					if ( g_city[city_index].building_res[tmpi].kind != questinfo->datakind )
						continue;
					BuildingUpgradeConfig *config = building_getconfig( g_city[city_index].building_res[tmpi].kind, g_city[city_index].building_res[tmpi].level + 1 );
					if ( config )
					{
						// ��ɫ�ȼ��Ƿ�����
						if ( g_city[city_index].level < config->actorlevel )
							continue;
						// �ٸ��ȼ��Ƿ�����
						if ( city_mainlevel( city_index ) < config->citylevel )
							continue;
						// ��Դ�Ƿ�����
						if ( g_city[city_index].silver < config->silver )
							continue;
						if ( g_city[city_index].wood < config->wood )
							continue;
						//if ( g_city[city_index].food < config->food )
						//	continue;
						//if ( g_city[city_index].iron < config->iron )
						//	continue;
						kind = g_city[city_index].building_res[tmpi].kind;
						offset = tmpi;
						break;
					}
				}
			}
		}
	}

	// ������ͨ����
	if ( kind <= 0 )
	{
		for ( int tmpi = 0; tmpi < BUILDING_MAXNUM; tmpi++ )
		{
			if ( g_city[city_index].building[tmpi].kind <= 0 )
				continue;
			if ( g_city[city_index].building[tmpi].kind == g_city[city_index].worker_kind || g_city[city_index].building[tmpi].kind == g_city[city_index].worker_kind_ex )
				continue;
			BuildingUpgradeConfig *config = building_getconfig( g_city[city_index].building[tmpi].kind, g_city[city_index].building[tmpi].level + 1 );
			if ( config )
			{
				// ��ɫ�ȼ��Ƿ�����
				if ( g_city[city_index].level < config->actorlevel )
					continue;
				// �ٸ��ȼ��Ƿ�����
				if ( city_mainlevel( city_index ) < config->citylevel )
					continue;
				// ��Դ�Ƿ�����
				if ( g_city[city_index].silver < config->silver )
					continue;
				if ( g_city[city_index].wood < config->wood )
					continue;
				//if ( g_city[city_index].food < config->food )
				//	continue;
				//if ( g_city[city_index].iron < config->iron )
				//	continue;
				kind = g_city[city_index].building[tmpi].kind;
				offset = tmpi;
				break;
			}
		}
	}

	// ��Ӫ����
	if ( kind <= 0 )
	{
		for ( int tmpi = 0; tmpi < BUILDING_BARRACKS_MAXNUM; tmpi++ )
		{
			if ( g_city[city_index].building_barracks[tmpi].kind <= 0 || g_city[city_index].building_barracks[tmpi].level <= 0 )
				continue;
			if ( g_city[city_index].building_barracks[tmpi].kind == g_city[city_index].worker_kind || g_city[city_index].building_barracks[tmpi].kind == g_city[city_index].worker_kind_ex )
				continue;
			if ( g_city[city_index].building_barracks[tmpi].trainsec > 0 )
				continue;
			BuildingUpgradeConfig *config = building_getconfig( g_city[city_index].building_barracks[tmpi].kind, g_city[city_index].building_barracks[tmpi].level + 1 );
			if ( config )
			{
				// ��ɫ�ȼ��Ƿ�����
				if ( g_city[city_index].level < config->actorlevel )
					continue;
				// �ٸ��ȼ��Ƿ�����
				if ( city_mainlevel( city_index ) < config->citylevel )
					continue;
				// ��Դ�Ƿ�����
				if ( g_city[city_index].silver < config->silver )
					continue;
				if ( g_city[city_index].wood < config->wood )
					continue;
				//if ( g_city[city_index].food < config->food )
				//	continue;
				//if ( g_city[city_index].iron < config->iron )
				//	continue;
				kind = g_city[city_index].building_barracks[tmpi].kind;
				offset = tmpi;
				break;
			}
		}
	}

	// ��Դ����
	if ( kind <= 0 )
	{
		for ( int tmpi = 0; tmpi < BUILDING_RES_MAXNUM; tmpi++ )
		{
			if ( g_city[city_index].building_res[tmpi].kind <= 0 || g_city[city_index].building_res[tmpi].level <= 0 )
				continue;
			if ( g_city[city_index].building_res[tmpi].kind == g_city[city_index].worker_kind || g_city[city_index].building_res[tmpi].kind == g_city[city_index].worker_kind_ex )
				continue;
			BuildingUpgradeConfig *config = building_getconfig( g_city[city_index].building_res[tmpi].kind, g_city[city_index].building_res[tmpi].level + 1 );
			if ( config )
			{
				// ��ɫ�ȼ��Ƿ�����
				if ( g_city[city_index].level < config->actorlevel )
					continue;
				// �ٸ��ȼ��Ƿ�����
				if ( city_mainlevel( city_index ) < config->citylevel )
					continue;
				// ��Դ�Ƿ�����
				if ( g_city[city_index].silver < config->silver )
					continue;
				if ( g_city[city_index].wood < config->wood )
					continue;
				//if ( g_city[city_index].food < config->food )
				//	continue;
				//if ( g_city[city_index].iron < config->iron )
				//	continue;
				kind = g_city[city_index].building_res[tmpi].kind;
				offset = tmpi;
				break;
			}
		}
	}
	
	if ( kind > 0 )
	{
		building_upgrade( city_index, kind, offset );
		city_change_autobuild( city_index, -1, PATH_SYSTEM );
	}
	else
		return -1;
	return 0;
}

// �������
int building_delete( int city_index, int kind, int offset )
{
	return 0;
}

// ����
int building_build( int city_index, int kind, int offset )
{
	CITY_CHECK_INDEX( city_index );
	if ( kind == BUILDING_Militiaman_Infantry || kind == BUILDING_Militiaman_Cavalry || kind == BUILDING_Militiaman_Archer )
	{
		if ( city_function_check( &g_city[city_index], CITY_FUNCTION_Militiaman ) == 0 )
			return -1;

		for ( int tmpi = 0; tmpi < BUILDING_BARRACKS_MAXNUM; tmpi++ )
		{
			if ( g_city[city_index].building_barracks[tmpi].kind <= 0 )
			{
				offset = tmpi;
				break;
			}
		}
		if ( offset < 0 || offset >= BUILDING_BARRACKS_MAXNUM )
			return -1;
		if ( g_city[city_index].building_barracks[offset].kind > 0 )
			return -1;
		BuildingUpgradeConfig *config = building_getconfig( kind,  1 );
		if ( !config )
			return -1;

		// ��Դ�Ƿ�����
		if ( g_city[city_index].silver < config->silver )
			return -1;
		if ( g_city[city_index].wood < config->wood )
			return -1;
		if ( g_city[city_index].food < config->food )
			return -1;
		if ( g_city[city_index].iron < config->iron )
			return -1;

		char ok = 0;
		if ( g_city[city_index].worker_sec <= 0 )
		{ // ��ͨ�������
			if ( config->sec > 0 )
			{
				g_city[city_index].worker_op = BUILDING_OP_BUILD;
				g_city[city_index].worker_sec = config->sec;
				g_city[city_index].worker_kind = kind;
				g_city[city_index].worker_offset = offset;
				g_city[city_index].wnsec = config->sec;
				g_city[city_index].worker_free = 0;
				g_city[city_index].wnquick = 0;
			}
			ok = 1;
		}
		else if ( g_city[city_index].worker_expire_ex > 0 && g_city[city_index].worker_sec_ex <= 0 )
		{ // ���ý������
			if ( config->sec > 0 )
			{
				g_city[city_index].worker_op_ex = BUILDING_OP_BUILD;
				g_city[city_index].worker_sec_ex = config->sec;
				g_city[city_index].worker_kind_ex = kind;
				g_city[city_index].worker_offset_ex = offset;
				g_city[city_index].wnsec_ex = config->sec;
				g_city[city_index].worker_free_ex = 0;
				g_city[city_index].wnquick_ex = 0;
			}
			ok = 1;
		}
		else
		{ // ������з�æ
			return -1;
		}

		if ( ok )
		{
			// �ۼ���Դ
			city_changesilver( city_index, -config->silver, PATH_BUILDING_UPGRADE );
			city_changewood( city_index, -config->wood, PATH_BUILDING_UPGRADE );
			city_changefood( city_index, -config->food, PATH_BUILDING_UPGRADE );
			city_changeiron( city_index, -config->iron, PATH_BUILDING_UPGRADE );
		
			g_city[city_index].building_barracks[offset].kind = kind;
			if ( config->sec > 0 )
				g_city[city_index].building_barracks[offset].level = 0;
			else
			{
				g_city[city_index].building_barracks[offset].level = 1;
			}

			// ֪ͨ��������״̬
			if ( kind < BUILDING_Infantry )
			{
				building_sendinfo( g_city[city_index].actor_index, kind );
			}
			else if ( kind < BUILDING_Silver )
			{
				building_sendinfo_barracks( g_city[city_index].actor_index, kind );
			}
			else if ( kind < BUILDING_Smithy )
			{
				building_sendinfo_res( g_city[city_index].actor_index, offset );
			}

			if ( config->sec > 0 )
			{
				building_sendworker( g_city[city_index].actor_index );
			}
			else
			{
				// ����
				quest_checkcomplete( g_city[city_index].actor_index );

				// ֪ͨ�ͻ��˸���״̬
				building_action( g_city[city_index].actor_index, kind, offset, 1 );

				// �¼�
				city_event_add( city_index, CITY_EVENT_BUILDING, kind, 1 );

				city_battlepower_building_calc( &g_city[city_index] );
			}

		}
	}
	return 0;
}

// �����Ľ�
int building_rebuild( int city_index, int kind, int offset, int rebuild_kind )
{
	CITY_CHECK_INDEX( city_index );
	if ( kind == rebuild_kind )
	{
		actor_notify_alert( g_city[city_index].actor_index, 1473 );
		return -1;
	}
	if ( kind <= 0 )
	{
		building_build( city_index, rebuild_kind, offset );
		return -1;
	}

	int level = 0;
	if ( kind == BUILDING_Militiaman_Infantry || kind == BUILDING_Militiaman_Cavalry || kind == BUILDING_Militiaman_Archer )
	{ // ���Ӫ
		if ( !(rebuild_kind == BUILDING_Militiaman_Infantry || rebuild_kind == BUILDING_Militiaman_Cavalry || rebuild_kind == BUILDING_Militiaman_Archer) )
			return -1;
		BuildingBarracks *pBuilding = buildingbarracks_getptr_kind( city_index, kind );
		if ( !pBuilding )
			return -1;
		if ( pBuilding->trainsec > 0 )
			return -1;
		level = pBuilding->level;
	}
	else if ( kind >= BUILDING_Silver && kind <= BUILDING_Smithy )
	{ // ��Դ����
		if ( !(rebuild_kind >= BUILDING_Silver && rebuild_kind <= BUILDING_Smithy) )
			return -1;
		// ����4��
		int hasnum = 0;
		for ( int tmpi = 0; tmpi < BUILDING_RES_MAXNUM; tmpi++ )
		{
			if ( g_city[city_index].building_res[tmpi].kind == kind )
			{
				hasnum += 1;
			}
		}
		if ( hasnum <= 4 )
		{
			actor_notify_alert( g_city[city_index].actor_index, 4231 );
			return -1;
		}

		BuildingRes *pBuilding = buildingres_getptr( city_index, offset );
		if ( !pBuilding )
			return -1;
		level = pBuilding->level;
	}
	else
	{
		return -1;
	}

	if ( rebuild_kind <= 0 || rebuild_kind >= g_building_upgrade_maxnum )
	{
		return -1;
	}

	if ( level >= g_building_upgrade[rebuild_kind].maxnum )
	{
		return -1;
	}

	BuildingUpgradeConfig *config = building_getconfig( rebuild_kind, level );
	if ( !config )
		return -1;

	// ��Դ�Ƿ�����
	//if ( g_city[city_index].silver < config->silver )
	//	return -1;
	//if ( g_city[city_index].wood < config->wood )
	//	return -1;
	//if ( g_city[city_index].food < config->food )
	//	return -1;
	//if ( g_city[city_index].iron < config->iron )
	//	return -1;

	char ok = 0;
	if ( g_city[city_index].worker_sec <= 0 )
	{ // ��ͨ�������
		g_city[city_index].worker_op = BUILDING_OP_REBUILD;
		g_city[city_index].worker_sec = global.building_delete_cd;
		g_city[city_index].worker_kind = rebuild_kind;
		g_city[city_index].worker_offset = offset;
		g_city[city_index].wnsec = global.building_delete_cd;
		g_city[city_index].wnquick = 0;
		ok = 1;
	}
	else if ( g_city[city_index].worker_expire_ex > 0 && g_city[city_index].worker_sec_ex <= 0 )
	{ // ���ý������
		g_city[city_index].worker_op_ex = BUILDING_OP_REBUILD;
		g_city[city_index].worker_sec_ex = global.building_delete_cd;
		g_city[city_index].worker_kind_ex = rebuild_kind;
		g_city[city_index].worker_offset_ex = offset;
		g_city[city_index].wnsec_ex = global.building_delete_cd;
		g_city[city_index].wnquick_ex = 0;
		ok = 1;
	}
	else
	{ // ������з�æ
		return -1;
	}

	if ( ok )
	{
		// �ۼ���Դ
		//city_changesilver( city_index, -config->silver, PATH_BUILDING_UPGRADE );
		//city_changewood( city_index, -config->wood, PATH_BUILDING_UPGRADE );
		//city_changefood( city_index, -config->food, PATH_BUILDING_UPGRADE );
		//city_changeiron( city_index, -config->iron, PATH_BUILDING_UPGRADE );

		if ( kind == BUILDING_Militiaman_Infantry || kind == BUILDING_Militiaman_Cavalry || kind == BUILDING_Militiaman_Archer )
		{ // ���Ӫ
			if ( !(rebuild_kind == BUILDING_Militiaman_Infantry || rebuild_kind == BUILDING_Militiaman_Cavalry || rebuild_kind == BUILDING_Militiaman_Archer) )
				return -1;
			BuildingBarracks *pBuilding = buildingbarracks_getptr_kind( city_index, kind );
			if ( !pBuilding )
				return -1;
			if ( pBuilding->trainsec > 0 )
				return -1;
			pBuilding->kind = rebuild_kind;
		}
		else if ( kind >= BUILDING_Silver && kind <= BUILDING_Smithy )
		{ // ��Դ����
			if ( !(rebuild_kind >= BUILDING_Silver && rebuild_kind <= BUILDING_Smithy) )
				return -1;
			BuildingRes *pBuilding = buildingres_getptr( city_index, offset );
			if ( !pBuilding )
				return -1;
			pBuilding->kind = rebuild_kind;
		}

		// ֪ͨ�ͻ��˸���״̬
		building_action( g_city[city_index].actor_index, kind, offset, 2 );

		// ֪ͨ��������״̬
		if ( kind < BUILDING_Silver )
		{
			building_sendinfo_barracks( g_city[city_index].actor_index, rebuild_kind );
		}
		else if ( kind < BUILDING_Smithy )
		{
			building_sendinfo_res( g_city[city_index].actor_index, offset );
		}
		building_sendworker( g_city[city_index].actor_index );
	}
	return 0;
}

// �������������������
int building_finish( int city_index, int op, int kind, int offset )
{
	CITY_CHECK_INDEX( city_index );
	int level = 0;
	if ( op == BUILDING_OP_BUILD || op == BUILDING_OP_UPGRADE )
	{
		if ( kind < BUILDING_Infantry )
		{ // ��ͨ����
			Building *pBuilding = building_getptr_kind( city_index, kind );
			if ( !pBuilding )
				return -1;
			level = pBuilding->level;
			pBuilding->level += 1;
			building_sendinfo( g_city[city_index].actor_index, kind );
		}
		else if ( kind < BUILDING_Silver )
		{ // ��Ӫ����
			BuildingBarracks *pBuilding = buildingbarracks_getptr_kind( city_index, kind );
			if ( !pBuilding )
				return -1;
			level = pBuilding->level;
			pBuilding->level += 1;
			building_sendinfo_barracks( g_city[city_index].actor_index, kind );
		}
		else if ( kind < BUILDING_Smithy )
		{ // ��Դ����
			BuildingRes *pBuilding = buildingres_getptr( city_index, offset );
			if ( !pBuilding )
				return -1;
			level = pBuilding->level;
			pBuilding->level += 1;
			building_sendinfo_res( g_city[city_index].actor_index, offset );
		}

		BuildingUpgradeConfig *config = building_getconfig( kind, level+1 );
		if ( config )
		{ // ������
			city_actorexp( city_index, config->exp, PATH_BUILDING_UPGRADE );
			// ���Ǹ�����������
			if ( config->awardkind > 0 )
			{
				if ( g_city[city_index].actor_index >= 0 )
				{
					award_getaward( g_city[city_index].actor_index, config->awardkind, config->awardnum, -1, PATH_BUILDING_UPGRADE, NULL );
				}
				else
				{
					gift( g_city[city_index].actorid, config->awardkind, config->awardnum, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, PATH_BUILDING_UPGRADE );
				}
			}
		}

		// ��������
		if ( kind == BUILDING_Main )
		{
			if ( g_city[city_index].level >= global.fangshi_actorlevel && g_city[city_index].building[0].level >= global.fangshi_citylevel )
			{
				city_function_open( &g_city[city_index], CITY_FUNCTION_FANGSHI );
			}
		}
		else if ( kind == BUILDING_Wall )
		{ // ��ǽ�Զ���ļ
			if ( g_city[city_index].atgu_op == 1 && g_city[city_index].atgu > 0 )
			{
				if ( city_guard_call( city_index ) >= 0 )
				{
					city_change_autoguard( city_index, -1, PATH_SYSTEM );
				}
			}
		}
		else if ( kind == BUILDING_StoreHouse )
		{// �ֿ����߼��ؽ�����
			g_city[city_index].rb_num += 1;
		}
		// ����
		quest_checkcomplete( g_city[city_index].actor_index );

		// ֪ͨ�ͻ��˸���״̬
		building_action( g_city[city_index].actor_index, kind, offset, 1 );

		// �¼�
		city_event_add( city_index, CITY_EVENT_BUILDING, kind, level+1 );

		city_battlepower_building_calc( &g_city[city_index] );
	}
	else if ( op == BUILDING_OP_DELETE )
	{
		if ( kind == BUILDING_Militiaman_Infantry || kind == BUILDING_Militiaman_Cavalry || kind == BUILDING_Militiaman_Archer )
		{ // ���Ӫ����
			BuildingBarracks *pBuilding = buildingbarracks_getptr_kind( city_index, kind );
			if ( !pBuilding )
				return -1;
			pBuilding->level = 0;
		}
		else if ( kind == BUILDING_Silver || kind == BUILDING_Wood || kind == BUILDING_Food || kind == BUILDING_Iron )
		{ // ��Դ����
			BuildingRes *pBuilding = buildingres_getptr( city_index, offset );
			if ( !pBuilding )
				return -1;
			pBuilding->level = 0;
		}

		// ֪ͨ�ͻ��˸���״̬
		city_battlepower_building_calc( &g_city[city_index] );
	}
	else if ( op == BUILDING_OP_REBUILD )
	{
		if ( kind == BUILDING_Militiaman_Infantry || kind == BUILDING_Militiaman_Cavalry || kind == BUILDING_Militiaman_Archer )
		{ // ���Ӫ����
			BuildingBarracks *pBuilding = buildingbarracks_getptr_kind( city_index, kind );
			if ( !pBuilding )
				return -1;
		}
		else if ( kind == BUILDING_Silver || kind == BUILDING_Wood || kind == BUILDING_Food || kind == BUILDING_Iron )
		{ // ��Դ����
			BuildingRes *pBuilding = buildingres_getptr( city_index, offset );
			if ( !pBuilding )
				return -1;
		}

		// ����
		quest_checkcomplete( g_city[city_index].actor_index );

		// ֪ͨ�ͻ��˸���״̬
		building_action( g_city[city_index].actor_index, kind, offset, 1 );

		city_battlepower_building_calc( &g_city[city_index] );
	}

	// ��֪ͨ
	if ( g_city[city_index].actor_index >= 0 )
	{
		int value[4] = { 0 };
		value[0] = op;
		value[1] = kind;
		value[2] = building_offset2no( kind, offset );
		value[3] = level + 1;
		actor_notify_value( g_city[city_index].actor_index, NOTIFY_BUILDINGFINISH, 4, value, NULL );
	}
	
	// �������Ѱ��
	if ( kind == BUILDING_Main && (level + 1) == global.hero_visit_mainlevel )
	{
		hero_visit_snedflag( g_city[city_index].actor_index );
	}
	return 0;
}

// ����
int building_workerquick( int actor_index, int kind, int offset, int sec )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->worker_kind == kind )
	{
		char hasquick = 1;
		if ( kind >= BUILDING_Silver && kind <= BUILDING_Iron )
		{
			if ( pCity->worker_offset != offset )
			{
				hasquick = 0;
			}
		}
		
		if ( hasquick == 1 )
		{
			pCity->worker_sec -= sec;
			if ( pCity->worker_sec <= 0 )
			{
				pCity->worker_sec = 0;
				building_finish( pCity->index, pCity->worker_op, pCity->worker_kind, pCity->worker_offset );
				pCity->worker_op = 0;
				pCity->worker_sec = 0;
				pCity->wnsec = 0;
				pCity->wnquick = 0;
				pCity->worker_kind = 0;
				pCity->worker_offset = -1;
				building_sendworker( pCity->actor_index );

				// �Զ�����
				building_upgrade_autocheck( pCity->index );
			}
			else
			{
				pCity->wnquick = 0;
				building_sendinfo( pCity->actor_index, kind );
				building_sendworker( pCity->actor_index );
			}
			return 0;
		}
	}

	if ( pCity->worker_kind_ex == kind )
	{
		char hasquick = 1;
		if ( kind >= BUILDING_Silver && kind <= BUILDING_Iron )
		{
			if ( pCity->worker_offset_ex != offset )
			{
				hasquick = 0;
			}
		}

		if ( hasquick == 1 )
		{
			pCity->worker_sec_ex -= sec;
			if ( pCity->worker_sec_ex <= 0 )
			{
				pCity->worker_sec_ex = 0;
				building_finish( pCity->index, pCity->worker_op_ex, pCity->worker_kind_ex, pCity->worker_offset_ex );
				pCity->worker_op_ex = 0;
				pCity->worker_sec_ex = 0;
				pCity->wnsec_ex = 0;
				pCity->wnquick_ex = 0;
				pCity->worker_kind_ex = 0;
				pCity->worker_offset_ex = -1;
				building_sendworker( pCity->actor_index );

				// �Զ�����
				building_upgrade_autocheck( pCity->index );
			}
			else
			{
				pCity->wnquick_ex = 0;
				building_sendinfo( pCity->actor_index, kind );
				building_sendworker( pCity->actor_index );
			}
			return 0;
		}
	}

	return 0;
}

// ��Ѽ���
int building_workerfree( int actor_index, int kind, int offset )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int freetime = pCity->attr.free_sec + vip_attr_buildfree( pCity )*60;
	if ( freetime < global.worker_freetime )
	{
		freetime = global.worker_freetime;
	}
	
	if ( pCity->worker_kind == kind )
	{
		char hasquick = 1;
		if ( kind >= BUILDING_Silver && kind <= BUILDING_Iron )
		{
			if ( pCity->worker_offset != offset )
			{
				hasquick = 0;
			}
		}

		if ( hasquick == 1 )
		{
			if ( pCity->worker_free == 1 )
			{
				pCity->worker_free = 0;
				pCity->worker_sec -= freetime;
				if ( pCity->worker_sec <= 0 )
				{
					pCity->worker_sec = 0;
					building_finish( pCity->index, pCity->worker_op, pCity->worker_kind, pCity->worker_offset );
					pCity->worker_op = 0;
					pCity->worker_sec = 0;
					pCity->wnsec = 0;
					pCity->wnquick = 0;
					pCity->worker_kind = 0;
					pCity->worker_offset = -1;
					building_sendworker( pCity->actor_index );

					// �Զ�����
					building_upgrade_autocheck( pCity->index );
				}
				else
				{
					building_sendinfo( pCity->actor_index, kind );
					building_sendworker( pCity->actor_index );
				}

			}
			return 0;
		}
	}

	if ( pCity->worker_kind_ex == kind )
	{
		char hasquick = 1;
		if ( kind >= BUILDING_Silver && kind <= BUILDING_Iron )
		{
			if ( pCity->worker_offset_ex != offset )
			{
				hasquick = 0;
			}
		}

		if ( hasquick == 1 )
		{
			if ( pCity->worker_free_ex == 1 )
			{
				pCity->worker_free_ex = 0;
				pCity->worker_sec_ex -= freetime;
				if ( pCity->worker_sec_ex <= 0 )
				{
					pCity->worker_sec_ex = 0;
					building_finish( pCity->index, pCity->worker_op_ex, pCity->worker_kind_ex, pCity->worker_offset_ex );
					pCity->worker_op_ex = 0;
					pCity->worker_sec_ex = 0;
					pCity->wnsec_ex = 0;
					pCity->wnquick_ex = 0;
					pCity->worker_kind_ex = 0;
					pCity->worker_offset_ex = -1;
					building_sendworker( pCity->actor_index );

					// �Զ�����
					building_upgrade_autocheck( pCity->index );
				}
				else
				{
					building_sendinfo( pCity->actor_index, kind );
					building_sendworker( pCity->actor_index );
				}
			}
			return 0;
		}
	}
	return 0;
}

// �����ļ���
int building_awardquick( int city_index, int sec )
{
	CITY_CHECK_INDEX( city_index );
	if ( g_city[city_index].worker_sec > 0 && g_city[city_index].worker_sec_ex > 0 )
	{
		if ( g_city[city_index].wnquick > 0 && g_city[city_index].wnquick_ex <= 0 )
		{
			g_city[city_index].wnquick_ex = sec;
		}
		else if ( g_city[city_index].wnquick_ex > 0 && g_city[city_index].wnquick <= 0 )
		{
			g_city[city_index].wnquick = sec;
		}
		else
		{
			if ( rand() % 2 == 0 )
			{
				g_city[city_index].wnquick = sec;
			}
			else
			{
				g_city[city_index].wnquick_ex = sec;
			}
		}
		building_sendworker( g_city[city_index].actor_index );
	}
	else if( g_city[city_index].worker_sec > 0 )
	{
		g_city[city_index].wnquick = sec;
		building_sendworker( g_city[city_index].actor_index );
	}
	else if ( g_city[city_index].worker_sec_ex > 0 )
	{
		g_city[city_index].wnquick_ex = sec;
		building_sendworker( g_city[city_index].actor_index );
	}
	return 0;
}

// �����ļ���
int building_awardquick_get( int actor_index, int kind, int offset )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int quicksec = 0;
	if ( pCity->worker_kind == kind && pCity->wnquick > 0 )
	{
		char hasquick = 1;
		if ( kind >= BUILDING_Silver && kind <= BUILDING_Iron )
		{
			if ( pCity->worker_offset != offset )
			{
				hasquick = 0;
			}
		}

		if ( hasquick == 1 )
		{
			building_workerquick( actor_index, kind, offset, pCity->wnquick );
			pCity->wnquick = 0;
			return 0;
		}
	}

	if ( pCity->worker_kind_ex == kind && pCity->wnquick_ex > 0 )
	{
		char hasquick = 1;
		if ( kind >= BUILDING_Silver && kind <= BUILDING_Iron )
		{
			if ( pCity->worker_offset_ex != offset )
			{
				hasquick = 0;
			}
		}

		if ( hasquick == 1 )
		{
			building_workerquick( actor_index, kind, offset, pCity->wnquick_ex );
			pCity->wnquick_ex = 0;
			return 0;
		}
	}
	return 0;
}

// �������ý������
int building_workerbuy( int actor_index, int type )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( type == 0 )
	{ // 1��
		if ( actor_change_token( actor_index, -global.worker_token_1, PATH_BUILDING_WORKER, 0 ) < 0 )
		{
			return -1;
		}
		pCity->worker_expire_ex += global.worker_expire_1;
		
	}
	else if ( type == 1 )
	{ // 7��
		if ( actor_change_token( actor_index, -global.worker_token_7, PATH_BUILDING_WORKER, 0 ) < 0 )
		{
			return -1;
		}
		pCity->worker_expire_ex += global.worker_expire_7;
	}
	else if ( type == 2 )
	{
		pCity->worker_expire_ex = 1;
	} 
	building_sendworker( actor_index );

	// �Զ�������
	building_upgrade_autocheck( pCity->index );
	return 0;
}

// ��������Ϣ
int building_smithy_send( int city_index )
{
	CITY_CHECK_INDEX( city_index );
	SLK_NetS_BuildingSmithy pValue = { 0 };
	pValue.m_forgingkind = g_city[city_index].forgingkind;
	pValue.m_forgingsec = g_city[city_index].forgingsec;
	pValue.m_forgingsec_need = equip_forgingtime( city_index, g_city[city_index].forgingkind );
	netsend_buildingsmithy_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

void building_makestruct( Building *pBuilding, int offset, SLK_NetS_Building *pValue )
{
	pValue->m_kind = pBuilding->kind;
	pValue->m_offset = offset;
	pValue->m_level = pBuilding->level;
	pValue->m_sec = pBuilding->sec;
	pValue->m_needsec = pBuilding->needsec;
	pValue->m_value = pBuilding->value;
	pValue->m_overvalue = pBuilding->overvalue;
	if ( pBuilding->quicksec > 0 )
		pValue->m_quick = 1;
}

void building_barracks_makestruct( BuildingBarracks *pBuilding, int offset, SLK_NetS_BuildingBarracks *pValue )
{
	pValue->m_kind = pBuilding->kind;
	pValue->m_offset = offset;
	pValue->m_level = pBuilding->level;
	pValue->m_sec = pBuilding->trainsec;
	pValue->m_needsec = pBuilding->trainsec_need;
	pValue->m_overvalue = pBuilding->overnum;
	if ( pBuilding->quicksec > 0 )
		pValue->m_quick = 1;
}

void building_res_makestruct( BuildingRes *pBuilding, int offset, SLK_NetS_BuildingRes *pValue )
{
	pValue->m_kind = pBuilding->kind;
	pValue->m_offset = offset;
	pValue->m_level = pBuilding->level;
}

int building_sendinfo( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_Building pValue = { 0 };
	int offset = building_getindex( pCity->index, kind );
	if ( offset >= 0 && offset < BUILDING_MAXNUM )
	{
		building_makestruct( &pCity->building[offset], offset, &pValue );
		netsend_building_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}
	return 0;
}

int building_sendinfo_barracks( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_BuildingBarracks pValue = { 0 };
	int offset = building_getindex( pCity->index, kind );
	if ( offset >= 0 && offset < BUILDING_BARRACKS_MAXNUM )
	{
		building_barracks_makestruct( &pCity->building_barracks[offset], offset, &pValue );
		netsend_buildingbarracks_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}
	return 0;
}

int building_sendinfo_res( int actor_index, int offset )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_BuildingRes pValue = { 0 };
	if ( offset >= 0 && offset < BUILDING_RES_MAXNUM )
	{
		building_res_makestruct( &pCity->building_res[offset], offset, &pValue );
		netsend_buildingres_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}
	return 0;
}

int building_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_BuildingList pValue = { 0 };

	// ��ͨ����
	for ( int tmpi = 0; tmpi < BUILDING_MAXNUM; tmpi++ )
	{
		if ( pCity->building[tmpi].kind <= 0 )
			continue;
		if ( pCity->building[tmpi].kind == BUILDING_Craftsman )
		{
			city_material_updatebuilding( pCity );
		}
		building_makestruct( &pCity->building[tmpi], tmpi, &pValue.m_building[pValue.m_building_count] );
		pValue.m_building_count += 1;
	}

	// ��Ӫ����
	for ( int tmpi = 0; tmpi < BUILDING_BARRACKS_MAXNUM; tmpi++ )
	{
		if ( pCity->building_barracks[tmpi].kind <= 0 )
			continue;
		building_barracks_makestruct( &pCity->building_barracks[tmpi], tmpi, &pValue.m_barracks[pValue.m_barracks_count] );
		pValue.m_barracks_count += 1;
	}

	// ��Դ����
	for ( int tmpi = 0; tmpi < BUILDING_RES_MAXNUM; tmpi++ )
	{
		if ( pCity->building_res[tmpi].kind <= 0 )
			continue;
		building_res_makestruct( &pCity->building_res[tmpi], tmpi, &pValue.m_res[pValue.m_res_count] );
		pValue.m_res_count += 1;
	}

	// �������
	pValue.m_worker_kind = pCity->worker_kind;	
	pValue.m_worker_offset = pCity->worker_offset;
	pValue.m_worker_op = pCity->worker_op;
	pValue.m_worker_sec = pCity->worker_sec;
	pValue.m_worker_needsec = pCity->wnsec;
	pValue.m_worker_free = pCity->worker_free;
	pValue.m_worker_kind_ex = pCity->worker_kind_ex;
	pValue.m_worker_offset_ex = pCity->worker_offset_ex;
	pValue.m_worker_op_ex = pCity->worker_op_ex;
	pValue.m_worker_sec_ex = pCity->worker_sec_ex;
	pValue.m_worker_needsec_ex = pCity->wnsec_ex;
	pValue.m_worker_free_ex = pCity->worker_free_ex;
	pValue.m_worker_expire_ex = pCity->worker_expire_ex;
	pValue.m_wnquick = pCity->wnquick;
	pValue.m_wnquick_ex = pCity->wnquick_ex;
	pValue.m_levynum = pCity->levynum;
	if ( pCity->questid[0] >= 80 && city_function_check( pCity, CITY_FUNCTION_WORLD ) == 0 )
	{
		city_function_open( pCity, CITY_FUNCTION_WORLD );
	}
	pValue.m_function= pCity->function;
	pValue.m_forgingkind = pCity->forgingkind;
	pValue.m_forgingsec = pCity->forgingsec;
	pValue.m_forgingsec_need = equip_forgingtime( pCity->index, pCity->forgingkind );
	netsend_buildinglist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// ���ͽ��������Ϣ
int building_sendworker( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_Worker pValue = { 0 };
	pValue.m_worker_kind = pCity->worker_kind;
	pValue.m_worker_offset = pCity->worker_offset;
	pValue.m_worker_op = pCity->worker_op;
	pValue.m_worker_sec = pCity->worker_sec;
	pValue.m_worker_needsec = pCity->wnsec;
	pValue.m_worker_free = pCity->worker_free;
	pValue.m_worker_kind_ex = pCity->worker_kind_ex;
	pValue.m_worker_offset_ex = pCity->worker_offset_ex;
	pValue.m_worker_op_ex = pCity->worker_op_ex;
	pValue.m_worker_sec_ex = pCity->worker_sec_ex;
	pValue.m_worker_needsec_ex = pCity->wnsec_ex;
	pValue.m_worker_free_ex = pCity->worker_free_ex;
	pValue.m_worker_expire_ex = pCity->worker_expire_ex;
	pValue.m_wnquick = pCity->wnquick;
	pValue.m_wnquick_ex = pCity->wnquick_ex;
	netsend_worker_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// ����������Ϣ
int building_send_upgradeinfo( int actor_index, int kind, int offset )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_BuildingUpgradeInfo pValue = { 0 };
	int level = building_getlevel( pCity->index, kind, building_offset2no( kind, offset ) );
	if ( level >= g_building_upgrade[kind].maxnum - 1 )
	{
		pValue.m_maxlevel = g_building_upgrade[kind].maxnum;
		netsend_buildingupgradeinfo_S( actor_index, SENDTYPE_ACTOR, &pValue );
		return 0;
	}
	
	BuildingUpgradeConfig *config = building_getconfig( kind, level+1 );
	if ( config )
	{
		pValue.m_actorlevel = config->actorlevel;
		pValue.m_citylevel = config->citylevel;
		pValue.m_sec = building_sec( pCity, config );
		pValue.m_silver = config->silver;
		pValue.m_food = config->food;
		pValue.m_wood = config->wood;
		pValue.m_iron = config->iron;
	}
	pValue.m_maxlevel = g_building_upgrade[kind].maxnum;
	netsend_buildingupgradeinfo_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// ��������
int building_action( int actor_index, short kind, short offset, short action )
{
	ACTOR_CHECK_INDEX( actor_index );
	SLK_NetS_BuildingAction pValue = { 0 };
	pValue.m_kind = kind;
	pValue.m_offset = offset;
	pValue.m_action = action;
	netsend_buildingaction_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// �ֿⱣ����
int building_store_protect( City *pCity, int restype )
{
	if ( !pCity )
		return 0;
	if ( restype <= 0 || restype > 3 )
		return 0;

	int level = building_getlevel( pCity->index, BUILDING_StoreHouse, -1 );
	if ( level <= 0 )
		return 0;

	BuildingUpgradeConfig *config = building_getconfig( BUILDING_StoreHouse, level );
	if ( !config )
		return 0;
	int protect = config->value[restype - 1] + (int)(config->value[restype - 1] * pCity->attr.protectres_per);
	return protect;
}

// �˿�����
int building_people_max( City *pCity )
{
	if ( !pCity )
		return 0;
	int level = building_getlevel( pCity->index, BUILDING_Main, -1 );
	if ( level <= 0 )
		return 0;

	BuildingUpgradeConfig *config = building_getconfig( BUILDING_Main, level );
	if ( !config )
		return 0;

	return config->value[1];
}

// GM
int building_gm( City *pCity )
{
	if ( !pCity )
		return -1;
	building_give( pCity->index, BUILDING_Main, 1 );
	building_give( pCity->index, BUILDING_Wall, 1 );
	building_give( pCity->index, BUILDING_StoreHouse, 1 );
	building_give( pCity->index, BUILDING_Tech, 1 );
	building_give( pCity->index, BUILDING_Craftsman, 1 );
	building_give( pCity->index, BUILDING_Cabinet, 1 );
	building_give( pCity->index, BUILDING_Infantry, 1 );
	building_give( pCity->index, BUILDING_Cavalry, 1 );
	building_give( pCity->index, BUILDING_Archer, 1 );
	building_give( pCity->index, BUILDING_Smithy, 1 );
	building_give( pCity->index, BUILDING_Wash, 1 );
	building_give( pCity->index, BUILDING_Shop, 1 );
	building_give( pCity->index, BUILDING_Hero, 1 );
	building_give( pCity->index, BUILDING_Wishing, 1 );
	building_give( pCity->index, BUILDING_Silver, 16 );
	building_give( pCity->index, BUILDING_Wood, 16 );
	building_give( pCity->index, BUILDING_Food, 16 );
	building_give( pCity->index, BUILDING_Iron, 16 );

	// ��ͨ����
	for ( int tmpi = 0; tmpi < BUILDING_MAXNUM; tmpi++ )
	{
		if ( pCity->building[tmpi].kind <= 0 )
			continue;
		
		pCity->building[tmpi].level = g_building_upgrade[pCity->building[tmpi].kind].maxnum - 1;
		building_sendinfo( pCity->actor_index, pCity->building[tmpi].kind );
	}

	// ��Ӫ����
	for ( int tmpi = 0; tmpi < BUILDING_BARRACKS_MAXNUM; tmpi++ )
	{
		if ( pCity->building_barracks[tmpi].kind <= 0 || pCity->building_barracks[tmpi].level <= 0 )
			continue;
		pCity->building_barracks[tmpi].level = g_building_upgrade[pCity->building_barracks[tmpi].kind].maxnum - 1;
		building_sendinfo_barracks( pCity->actor_index, pCity->building[tmpi].kind );
	}

	// ��Դ����
	for ( int tmpi = 0; tmpi < BUILDING_RES_MAXNUM; tmpi++ )
	{
		if ( pCity->building_res[tmpi].kind <= 0 || pCity->building_res[tmpi].level <= 0 )
			continue;
		pCity->building_res[tmpi].level = g_building_upgrade[pCity->building_res[tmpi].kind].maxnum - 1;
		building_sendinfo_res( pCity->actor_index, pCity->building[tmpi].kind );
	}


	return 0;
}

// �����ȼ�
int building_level_gm( int city_index, int kind, int newlevel )
{
	CITY_CHECK_INDEX( city_index );
	int level = 0;

	if ( kind < BUILDING_Infantry )
	{ // ��ͨ����
		Building *pBuilding = building_getptr( city_index, -1 );
		if ( !pBuilding )
			return -1;
		level = pBuilding->level;
		pBuilding->level = newlevel;
		building_sendinfo( g_city[city_index].actor_index, kind );
	}
	else if ( kind < BUILDING_Silver )
	{ // ��Ӫ����
		BuildingBarracks *pBuilding = buildingbarracks_getptr( city_index, -1 );
		if ( !pBuilding )
			return -1;
		level = pBuilding->level;
		pBuilding->level = newlevel;
		building_sendinfo_barracks( g_city[city_index].actor_index, kind );
	}
	else if ( kind < BUILDING_Smithy )
	{ // ��Դ����
		for ( int tmpi = 0; tmpi < BUILDING_RES_MAXNUM; tmpi++ )
		{
			if ( g_city[city_index].building_res[tmpi].kind != kind || g_city[city_index].building_res[tmpi].level <= 0 )
				continue;
			g_city[city_index].building_res[tmpi].level = newlevel;
			building_sendinfo_res( g_city[city_index].actor_index, g_city[city_index].building[tmpi].kind );
		}
	}

	// �ֿ����߼��ؽ�����
	if ( kind == BUILDING_StoreHouse )
	{
		g_city[city_index].rb_num += newlevel;
	}
	// ����
	quest_checkcomplete( g_city[city_index].actor_index );

	// ֪ͨ�ͻ��˸���״̬
	building_action( g_city[city_index].actor_index, kind, -1, 1 );

	city_battlepower_building_calc( &g_city[city_index] );
	
	// �������Ѱ��
	if ( kind == BUILDING_Main && (level + 1) == global.hero_visit_mainlevel )
	{
		hero_visit_snedflag( g_city[city_index].actor_index );
	}
	return 0;
}
