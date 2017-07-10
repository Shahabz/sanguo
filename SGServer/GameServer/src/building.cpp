#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "global.h"
#include "actor.h"
#include "city.h"
#include "building.h"
#include "mapunit.h"
#include "map.h"

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

// 获取建筑指针
Building* building_getptr( int city_index, int offset )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	if ( offset < 0 || offset >= BUILDING_MAXNUM )
		return NULL;
	return &g_city[city_index].building[offset];
}

BuildingBarracks* buildingbarracks_getptr( int city_index, int offset )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	if ( offset < 0 || offset >= BUILDING_BARRACKS_MAXNUM )
		return NULL;
	return &g_city[city_index].building_barracks[offset];
}

BuildingRes* buildingres_getptr( int city_index, int offset )
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

// 获取建筑的配置信息
BuildingUpgradeConfig* building_getconfig( int kind, int level )
{
	if ( kind < 0 || kind >= g_building_upgrade_maxnum )
		return NULL;
	if ( level < 0 || level >= g_building_upgrade[kind].maxnum )
		return NULL;
	return &g_building_upgrade[kind].config[level];
}

int building_getindex( int city_index, int kind )
 {
	CITY_CHECK_INDEX( city_index );
	int index = -1;
	if ( kind < BUILDING_Infantry )
	{ // 普通建筑
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
	{ // 兵营建筑
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


// 获取建筑等级
int building_getlevel( int city_index, int kind, int no )
{
	CITY_CHECK_INDEX( city_index );
	if ( kind < BUILDING_Infantry )
	{ // 普通建筑
		for ( int tmpi = 0; tmpi < BUILDING_MAXNUM; tmpi++ )
		{
			if ( g_city[city_index].building[tmpi].kind == kind )
			{
				return g_city[city_index].building[tmpi].level;
			}
		}
	}
	else if ( kind < BUILDING_Silver )
	{ // 兵营建筑
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
		BuildingRes *pRes = buildingres_getptr_number( city_index, kind, no-1 );
		if ( pRes )
			return pRes->level;
	}
	return 0;
}

// 给予一个建筑
int building_give( int city_index, int kind, int num )
{
	CITY_CHECK_INDEX( city_index );
	if ( kind < BUILDING_Infantry )
	{ // 普通建筑
		int offset = building_create( city_index, kind, -1 );
		if ( offset >= 0 )
		{
			SLK_NetS_BuildingGet pValue = { 0 };
			building_makestruct( &g_city[city_index].building[offset], offset, &pValue.m_building );
			netsend_buildingget_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
		}
	}
	else if ( kind < BUILDING_Silver )
	{ // 兵营建筑
		int offset = building_create( city_index, kind, -1 );
		if ( offset >= 0 )
		{
			SLK_NetS_BuildingBarracksGet pValue = { 0 };
			building_barracks_makestruct( &g_city[city_index].building_barracks[offset], offset, &pValue.m_barracks );
			netsend_buildingbarracksget_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
		}
	}
	else if ( kind < BUILDING_Smithy )
	{ // 资源建筑
		num = num <= 0 ? 1 : num;
		for ( int tmpi = 0; tmpi < num; tmpi++ )
		{
			int offset = building_create( city_index, kind, -1 );
			if ( offset >= 0 )
			{
				SLK_NetS_BuildingResGet pValue = { 0 };
				building_res_makestruct( &g_city[city_index].building_res[offset], offset, &pValue.m_res );
				netsend_buildingresget_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
			}
		}
		
	}
	else
	{

	}
	return 0;
}

// 创建建筑
int building_create( int city_index, int kind, int offset )
{
	CITY_CHECK_INDEX( city_index );
	if ( kind < BUILDING_Infantry )
	{ // 普通建筑
		int level = building_getlevel( city_index, kind, -1 );
		if ( level > 0 )
			return -1;

		Building *pBuilding = building_getptr( city_index, offset );
		if ( pBuilding )
		{ // 指定了位置
			pBuilding->kind = kind;
			pBuilding->level = 1;
		}
		else
		{ // 没有指定找一个空位
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
	{ // 兵营建筑
		int level = building_getlevel( city_index, kind, -1 );
		if ( level > 0 )
			return -1;

		BuildingBarracks *pBuilding = buildingbarracks_getptr( city_index, offset );
		if ( pBuilding )
		{ // 指定了位置
			pBuilding->kind = kind;
			pBuilding->level = 1;
		}
		else
		{ // 没有指定找一个空位
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
	{ // 资源建筑
		BuildingRes *pBuilding = buildingres_getptr( city_index, offset );
		if ( pBuilding )
		{ // 指定了位置
			pBuilding->kind = kind;
			pBuilding->level = 1;
		}
		else
		{ // 没有指定找一个空位
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
			}
			
		}
	}
	else
	{ // 入口功能
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

// 建筑升级
int building_upgrade( int city_index, int kind, int offset )
{
	CITY_CHECK_INDEX( city_index );
	int level = 0;
	if ( kind < BUILDING_Infantry )
	{ // 普通建筑
		Building *pBuilding = building_getptr( city_index, offset );
		if ( !pBuilding )
			return -1;
		if ( pBuilding->sec > 0 )
			return -1;
		level = pBuilding->level;
	}
	else if ( kind < BUILDING_Silver )
	{ // 兵营建筑
		BuildingBarracks *pBuilding = buildingbarracks_getptr( city_index, offset );
		if ( !pBuilding )
			return -1;
		if ( pBuilding->trainsec > 0 )
			return -1;
		level = pBuilding->level;
	}
	else if ( kind < BUILDING_Smithy )
	{ // 资源建筑
		BuildingRes *pBuilding = buildingres_getptr( city_index, offset );
		if ( !pBuilding )
			return -1;
		level = pBuilding->level;
	}
	else
	{
		return -1;
	}

	BuildingUpgradeConfig *config = building_getconfig( kind, level );
	if ( !config )
		return -1;

	// 官府等级是否满足
	if ( city_mainlevel( city_index ) < config->citylevel )
		return -1;

	// 角色等级是否满足
	if ( g_city[city_index].level < config->actorlevel )
		return -1;

	// 资源是否满足
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
	{ // 普通建造队列
		g_city[city_index].worker_op = BUILDING_OP_UPGRADE;
		g_city[city_index].worker_sec = config->sec;
		g_city[city_index].worker_kind = kind;
		g_city[city_index].worker_offset = offset;
		ok = 1;
	}
	else if ( g_city[city_index].worker_expire_ex > 0 && g_city[city_index].worker_sec_ex <= 0 )
	{ // 商用建造队列
		g_city[city_index].worker_op_ex = BUILDING_OP_UPGRADE;
		g_city[city_index].worker_sec_ex = config->sec;
		g_city[city_index].worker_kind_ex = kind;
		g_city[city_index].worker_offset_ex = offset;
		ok = 1;
	}
	else
	{ // 建造队列繁忙
		return -1;
	}

	if ( ok )
	{
		// 扣减资源
		city_changesilver( city_index, -config->silver, PATH_BUILDING_UPGRADE );
		city_changewood( city_index, -config->wood, PATH_BUILDING_UPGRADE );
		city_changefood( city_index, -config->food, PATH_BUILDING_UPGRADE );
		city_changeiron( city_index, -config->iron, PATH_BUILDING_UPGRADE );

		// 通知建筑更新状态
	}
	return 0;
}

// 建筑拆除
int building_delete( int city_index, int kind, int offset )
{
	CITY_CHECK_INDEX( city_index );
	if ( kind == BUILDING_Militiaman_Infantry || kind == BUILDING_Militiaman_Cavalry || kind == BUILDING_Militiaman_Archer )
	{ // 民兵营
		BuildingBarracks *pBuilding = buildingbarracks_getptr( city_index, offset );
		if ( !pBuilding )
			return -1;
		if ( pBuilding->trainsec > 0 )
			return -1;
	}
	else if ( kind == BUILDING_Silver || kind == BUILDING_Wood || kind == BUILDING_Food || kind == BUILDING_Iron )
	{ // 农田
	}
	else
	{
		return -1;
	}

	char ok = 0;
	if ( g_city[city_index].worker_sec <= 0 )
	{ // 普通建造队列
		g_city[city_index].worker_op = BUILDING_OP_DELETE;
		g_city[city_index].worker_sec = global.building_delete_cd;
		g_city[city_index].worker_kind = kind;
		g_city[city_index].worker_offset = offset;
		ok = 1;
	}
	else if ( g_city[city_index].worker_expire_ex > 0 && g_city[city_index].worker_sec_ex <= 0 )
	{ // 商用建造队列
		g_city[city_index].worker_op_ex = BUILDING_OP_DELETE;
		g_city[city_index].worker_sec_ex = global.building_delete_cd;
		g_city[city_index].worker_kind_ex = kind;
		g_city[city_index].worker_offset_ex = offset;
		ok = 1;
	}
	else
	{ // 建造队列繁忙
		return -1;
	}

	if ( ok )
	{

	}
	return 0;
}

// 建筑升级或拆除操作完成
int building_finish( int city_index, int op, int kind, int offset )
{
	CITY_CHECK_INDEX( city_index );
	int level = 0;
	if ( op == BUILDING_OP_BUILD )
	{
		building_create( city_index, kind, offset );
	}
	else if ( op == BUILDING_OP_UPGRADE )
	{
		if ( kind < BUILDING_Infantry )
		{ // 普通建筑
			Building *pBuilding = building_getptr( city_index, offset );
			if ( !pBuilding )
				return -1;
			level = pBuilding->level;
			pBuilding->level += 1;
		}
		else if ( kind < BUILDING_Silver )
		{ // 兵营建筑
			BuildingBarracks *pBuilding = buildingbarracks_getptr( city_index, offset );
			if ( !pBuilding )
				return -1;
			level = pBuilding->level;
			pBuilding->level += 1;
		}
		else if ( kind < BUILDING_Smithy )
		{ // 资源建筑
			BuildingRes *pBuilding = buildingres_getptr( city_index, offset );
			if ( !pBuilding )
				return -1;
			level = pBuilding->level;
			pBuilding->level += 1;
		}

		BuildingUpgradeConfig *config = building_getconfig( kind, level );
		if ( config )
		{ // 给经验
			city_actorexp( city_index, config->exp, PATH_BUILDING_UPGRADE );
		}

		// 通知客户端更新状态
	}
	else if ( op == BUILDING_OP_DELETE )
	{
		if ( kind == BUILDING_Militiaman_Infantry || kind == BUILDING_Militiaman_Cavalry || kind == BUILDING_Militiaman_Archer )
		{ // 民兵营建筑
			BuildingBarracks *pBuilding = buildingbarracks_getptr( city_index, offset );
			if ( !pBuilding )
				return -1;
			pBuilding->level = 0;
		}
		else if ( kind == BUILDING_Silver || kind == BUILDING_Wood || kind == BUILDING_Food || kind == BUILDING_Iron )
		{ // 资源建筑
			BuildingRes *pBuilding = buildingres_getptr( city_index, offset );
			if ( !pBuilding )
				return -1;
			pBuilding->level = 0;
		}

		// 通知客户端更新状态
	}
	return 0;
}

// 获取士兵数量
int building_soldiers_total( int city_index, char kind )
{
	CITY_CHECK_INDEX( city_index );
	int total = 0;
	for ( int tmpi = 0; tmpi < BUILDING_BARRACKS_MAXNUM; tmpi++ )
	{
		if ( g_city[city_index].building_barracks[tmpi].kind == kind || 
			g_city[city_index].building_barracks[tmpi].kind == kind + 3 )
		{
			total += g_city[city_index].building_barracks[tmpi].soldiers;
		}
	}
	return total;
}

void building_makestruct( Building *pBuilding, int offset, SLK_NetS_Building *pValue )
{
	pValue->m_kind = pBuilding->kind;
	pValue->m_offset = offset;
	pValue->m_level = pBuilding->level;
	pValue->m_sec = pBuilding->sec;
	pValue->m_needsec = pBuilding->needsec;
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

	// 普通建筑
	for ( int tmpi = 0; tmpi < BUILDING_MAXNUM; tmpi++ )
	{
		if ( pCity->building[tmpi].kind <= 0 )
			continue;
		building_makestruct( &pCity->building[tmpi], tmpi, &pValue.m_building[pValue.m_building_count] );
		pValue.m_building_count += 1;
	}

	// 兵营建筑
	for ( int tmpi = 0; tmpi < BUILDING_BARRACKS_MAXNUM; tmpi++ )
	{
		if ( pCity->building_barracks[tmpi].kind <= 0 || pCity->building_barracks[tmpi].level <= 0 )
			continue;
		building_barracks_makestruct( &pCity->building_barracks[tmpi], tmpi, &pValue.m_barracks[pValue.m_barracks_count] );
		pValue.m_barracks_count += 1;
	}

	// 资源建筑
	for ( int tmpi = 0; tmpi < BUILDING_BARRACKS_MAXNUM; tmpi++ )
	{
		if ( pCity->building_res[tmpi].kind <= 0 || pCity->building_res[tmpi].level <= 0 )
			continue;
		building_res_makestruct( &pCity->building_res[tmpi], tmpi, &pValue.m_res[pValue.m_res_count] );
		pValue.m_res_count += 1;
	}

	// 建造队列
	pValue.m_worker_kind = pCity->worker_kind;	
	pValue.m_worker_offset = pCity->worker_offset;
	pValue.m_worker_op = pCity->worker_op;
	pValue.m_worker_sec = pCity->worker_sec;
	pValue.m_worker_free = pCity->worker_free;
	pValue.m_worker_kind_ex = pCity->worker_kind_ex;
	pValue.m_worker_offset_ex = pCity->worker_offset_ex;
	pValue.m_worker_op_ex = pCity->worker_op_ex;
	pValue.m_worker_sec_ex = pCity->worker_sec_ex;
	pValue.m_worker_free_ex = pCity->worker_free_ex;
	pValue.m_worker_expire_ex = pCity->worker_expire_ex;
	pValue.m_levynum = pCity->levynum;
	pValue.m_function= pCity->function;
	netsend_buildinglist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
