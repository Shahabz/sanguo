#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <math.h>
#include <limits.h>
#include "db.h"
#include "global.h"
#include "actor.h"
#include "city.h"
#include "city_attr.h"
#include "city_tech.h"
#include "building.h"
#include "hero.h"
#include "equip.h"
#include "mapunit.h"
#include "map.h"
#include "server_netsend_auto.h"
#include "system.h"
#include "item.h"
#include "vip.h"
#include "actor_notify.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern UpgradeInfo *g_upgradeinfo;
extern int g_upgradeinfo_maxnum;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;
extern Map g_map;

extern Actor *g_actors;
extern int g_maxactornum;

extern CityGuardInfo *g_cityguardinfo;
extern int g_cityguardinfo_maxnum;

extern TrainlongInfo *g_trainlong;
extern int g_trainlong_maxnum;

extern TrainqueueInfo *g_trainqueue;
extern int g_trainqueue_maxnum;

extern OfficialForging *g_official_forging;
extern int g_official_forging_maxnum;

extern OfficialGov *g_official_gov;
extern int g_official_gov_maxnum;

extern OfficialTech *g_official_tech;
extern int g_official_tech_maxnum;

extern TechInfo *g_techinfo;
extern int g_techinfo_maxnum;

extern int g_city_maxindex;
City *g_city = NULL;
int g_city_maxcount = 0;
char g_city_allinited = 0;

// 城池数据重置
int city_reset()
{
	memset( g_city, 0, sizeof(City)*g_city_maxcount );
	for ( int tmpi = 0; tmpi < g_city_maxcount; tmpi++ )
	{
		g_city[tmpi].index = -1;
		g_city[tmpi].unit_index = -1;
		g_city[tmpi].actor_index = -1;
	}
	return 0;
}

// 城池读档完毕的回调
int city_loadcb( int city_index )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return -1;
	// 读取普通建筑
	city_building_load_auto( g_city[city_index].actorid, city_index, building_getptr, "city_building" );

	// 读取兵营建筑
	city_building_barracks_load_auto( g_city[city_index].actorid, city_index, buildingbarracks_getptr, "city_building_barracks" );

	// 读取资源建筑
	city_building_res_load_auto( g_city[city_index].actorid, city_index, buildingres_getptr, "city_building_res" );

	// 读取上阵英雄
	actor_hero_load_auto( g_city[city_index].actorid, city_index, city_hero_getptr, "actor_hero" );

	// 读取上阵英雄的装备
	actor_equip_load_auto( g_city[city_index].actorid, city_index, city_equip_getptr, "actor_equip" );

	// 读取城墙守卫
	city_guard_load_auto( g_city[city_index].actorid, city_index, city_guard_getptr, "city_guard" );

	// 城池事件
	city_event_load( &g_city[city_index] );
	// 计算VIP等级
	vip_calclevel( city_index );
	// 计算临时属性
	city_attr_reset( &g_city[city_index] );
	// 计算战力
	city_battlepower_reset( &g_city[city_index] );

	// 添加到地图显示单元
	g_city[city_index].unit_index = mapunit_add( MAPUNIT_TYPE_CITY, city_index );

	// 占地块信息添加到世界地图
	map_addobject( MAPUNIT_TYPE_CITY, g_city[city_index].posx, g_city[city_index].posy, MAPUNIT_TYPE_CITY );
	return 0;
}

// 服务器启动读取所有城池信息到内存
int city_load()
{
	g_city_maxcount = g_Config.max_citycount;
	g_city = (City*)malloc( sizeof(City)* g_city_maxcount );
	printf_msg( "City  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_city_maxcount, (sizeof(City)*g_city_maxcount) / 1024.0 / 1024.0, sizeof(City) / 1024.0 );
	city_reset();
	city_load_auto( city_indexptr, city_loadcb, "city" );
	g_city_allinited = 1;
	return 0;
}

// 服务器关闭，所有城池信息存到数据库
int city_save( FILE *fp )
{
	for ( int city_index = 0; city_index < g_city_maxcount; city_index++ )
	{
		printf_msg( "city_save %d/%d\r", city_index + 1, g_city_maxcount );
		if ( g_city[city_index].actorid <= 0 )
			continue;
		city_single_save( &g_city[city_index], fp );
	}
	return 0;
}

// 单城池存储
int city_single_save( City *pCity, FILE *fp )
{
	// 城池基本信息
	city_save_auto( pCity, "city", fp );

	// 普通建筑
	city_building_batch_save_auto( pCity->actorid, pCity->building, BUILDING_MAXNUM, "city_building", fp );

	// 兵营建筑
	city_building_barracks_batch_save_auto( pCity->actorid, pCity->building_barracks, BUILDING_BARRACKS_MAXNUM, "city_building_barracks", fp );

	// 资源建筑
	city_building_res_batch_save_auto( pCity->actorid, pCity->building_res, BUILDING_RES_MAXNUM, "city_building_res", fp );

	// 英雄
	actor_hero_batch_save_auto( pCity->hero, HERO_CITY_MAX, "actor_hero", fp );

	// 英雄装备
	for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
	{
		if ( pCity->hero[tmpi].kind <= 0 )
			continue;
		actor_equip_batch_save_auto( pCity->hero[tmpi].equip, EQUIP_TYPE_MAX, "actor_equip", fp );
	}

	// 城墙守卫
	db_delete( pCity->actorid, "city_guard", fp );
	city_guard_batch_save_auto( pCity->actorid, pCity->guard, CITY_GUARD_MAX, "city_guard", fp );

	// 城池事件
	city_event_save( pCity, fp );
	return 0;
}

// 获取这个玩家的城池属性
City *city_indexptr( int city_index )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	return &g_city[city_index];
}

// 获取这个玩家的城池属性
City *city_getptr( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return NULL;
	int city_index = g_actors[actor_index].city_index;
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	return &g_city[city_index];
}

// 获取这个城池属性
City *city_getptr_withactorid( int actorid )
{
	int city_index = city_getindex_withactorid( actorid );
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	return &g_city[city_index];
}

// 根据玩家id找到城池索引
int city_getindex_withactorid( int actorid )
{
	int city_index = -1;
	for ( int tmpi = 0; tmpi < g_city_maxindex/*注意：使用索引位置，为了效率*/; tmpi++ )
	{
		if ( g_city[tmpi].actorid < MINACTORID )
			continue;
		if ( g_city[tmpi].actorid == actorid )
		{
			city_index = tmpi;
			break;
		}
	}
	return city_index;
}

// 创建一个新城市
int city_new( City *pCity )
{
	if ( pCity == NULL )
		return -1;
	int city_index = -1;
	for ( city_index = 0; city_index < g_city_maxcount; city_index++ )
	{
		if ( g_city[city_index].actorid <= 0 )
		{
			memcpy( &g_city[city_index], pCity, sizeof(City) );
			g_city[city_index].index = city_index;
			g_city[city_index].actor_index = -1;
			g_city[city_index].unit_index = -1;
			break;
		}
	}

	// 没创建成功
	if ( city_index >= g_city_maxcount )
		return -1;

	// 为了遍历的效率，计算最大的索引
	if ( city_index >= g_city_maxindex )
	{
		g_city_maxindex = city_index + 1;
	}

	// 存档
	city_save_auto( &g_city[city_index], "city", NULL );
	return city_index;
}

// 所有城市每秒的逻辑 , 一般是检查建筑的建造升级拆除，造兵等
void city_logic_sec()
{
	if ( !g_city_allinited )
		return;
//#ifdef WIN32
//	DWORD b = timeGetTime();
//#else
//	struct timeval tpstart, tpend;
//	float timeuse;
//	gettimeofday( &tpstart, NULL );
//#endif
	int nowtime = (int)time( NULL );
	for ( int city_index = 0; city_index < g_city_maxindex/*注意：使用索引位置，为了效率*/; city_index++ )
	{
		if ( g_city[city_index].actorid <= 0 )
			continue;
		
		// 普通建造队列
		if ( g_city[city_index].worker_sec > 0 )
		{
			g_city[city_index].worker_sec -= 1;
			if ( g_city[city_index].worker_sec <= 0 )
			{
				building_finish( city_index, g_city[city_index].worker_op, g_city[city_index].worker_kind, g_city[city_index].worker_offset );
				g_city[city_index].worker_op = 0;
				g_city[city_index].worker_sec = 0;
				g_city[city_index].worker_kind = 0;
				g_city[city_index].worker_offset = -1;
				g_city[city_index].wnsec = 0;
				building_sendworker( g_city[city_index].actor_index );
			}
		}

		// 商用建造队列
		if ( g_city[city_index].worker_sec_ex > 0 )
		{
			g_city[city_index].worker_sec_ex -= 1;
			if ( g_city[city_index].worker_sec_ex <= 0 )
			{
				building_finish( city_index, g_city[city_index].worker_op_ex, g_city[city_index].worker_kind_ex, g_city[city_index].worker_offset_ex );
				g_city[city_index].worker_op_ex = 0;
				g_city[city_index].worker_sec_ex = 0;
				g_city[city_index].worker_kind_ex = 0;
				g_city[city_index].worker_offset_ex = -1;
				g_city[city_index].wnsec_ex = 0;
				building_sendworker( g_city[city_index].actor_index );
			}
		}

		// 普通建筑逻辑
		for ( int tmpi = 0; tmpi < BUILDING_MAXNUM; tmpi++ )
		{
			char kind = g_city[city_index].building[tmpi].kind;
			if ( kind <= 0 )
				continue;
			switch ( kind )
			{
			case BUILDING_Tech:
				{
					if ( g_city[city_index].building[tmpi].sec > 0 )
					{
						g_city[city_index].building[tmpi].sec -= 1;
						if ( g_city[city_index].building[tmpi].sec <= 0 )
						{
							city_tech_finish( &g_city[city_index], &g_city[city_index].building[tmpi] );
						}		
					}
				}
				break;
			case BUILDING_Craftsman:
				{

				}
				break;
			}
			
		}

		// 兵营建筑逻辑
		for ( int tmpi = 0; tmpi < BUILDING_BARRACKS_MAXNUM; tmpi++ )
		{
			BuildingBarracks *barracks = &g_city[city_index].building_barracks[tmpi];
			if ( barracks->kind <= 0 )
				continue;
			if ( barracks->trainsec > 0 )
			{
				barracks->trainsec -= 1;
				if ( barracks->trainsec <= 0 )
				{// 招募完毕
					city_train_finish( &g_city[city_index], barracks );
				}
			}
			
		}

		// 保护状态
		if ( g_city[city_index].ptsec > 0 )
		{
			g_city[city_index].ptsec -= 1;
			if ( g_city[city_index].ptsec <= 0 )
			{
				city_changeprotect( city_index, 0, PATH_SYSTEM );
			}
		}

		// 回复体力
		if ( g_city[city_index].body < global.body_max )
		{
			g_city[city_index].bodysec -= 1;
			if ( g_city[city_index].bodysec <= 0 )
			{
				g_city[city_index].bodysec = global.body_sec;
				city_changebody( city_index, 1, PATH_SYSTEM );
			}
		}

		// 征收次数
		if ( g_city[city_index].levynum < global.levy_max )
		{
			g_city[city_index].levysec -= 1;
			if ( g_city[city_index].levysec <= 0 )
			{
				g_city[city_index].levysec = global.levy_sec;
				g_city[city_index].levynum += 1;
			}
		}

		// 装备洗炼
		if ( g_city[city_index].equip_washnum < global.equip_wash_max )
		{
			g_city[city_index].equip_washsec -= 1;
			if ( g_city[city_index].equip_washsec <= 0 )
			{
				g_city[city_index].equip_washsec = global.equip_wash_sec;
				g_city[city_index].equip_washnum += 1;
			}
		}
		
		// 英雄洗炼
		if ( g_city[city_index].hero_washnum < global.hero_wash_max )
		{
			g_city[city_index].hero_washsec -= 1;
			if ( g_city[city_index].hero_washsec <= 0 )
			{
				g_city[city_index].hero_washsec = global.hero_wash_sec;
				g_city[city_index].hero_washnum += 1;
			}
		}
		
		// 守卫冷却
		if ( g_city[city_index].guardsec > 0 )
		{
			g_city[city_index].guardsec -= 1;
		}
		
		// 装备打造
		if ( g_city[city_index].forgingsec > 0 )
		{
			g_city[city_index].forgingsec -= 1;
			if ( g_city[city_index].forgingsec <= 0 )
			{
				// 通知打造完毕
				building_smithy_send( city_index );
			}
		}

		// 国器打造
		if ( g_city[city_index].nequip_sec > 0 )
		{
			g_city[city_index].nequip_sec -= 1;
			if ( g_city[city_index].nequip_sec <= 0 )
			{
				// 通知国器打造完毕
				
			}
		}

		// 雇佣官
		for ( int i = 0; i < 3; i++ )
		{
			if ( g_city[city_index].ofsec[i] > 0 )
			{
				g_city[city_index].ofsec[i] -= 1;
				if ( g_city[city_index].ofsec[i] <= 0 )
				{
					// 通知玩家
					g_city[city_index].ofkind[i] = 0;
					g_city[city_index].ofsec[i] = 0;
					g_city[city_index].ofquick[i] = 0;
					if ( g_city[city_index].actor_index >= 0 )
						city_officialhire_sendinfo( &g_city[city_index], i );
				}
			}
		}
	}
//#ifdef WIN32
//	DWORD e = timeGetTime();
//	printf_msg( "sec city_logic_sec:%dms\n", e - b );
//#else
//	gettimeofday( &tpend, NULL );
//	timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec;
//	timeuse /= 1000000;
//	printf_msg( "sec ranking_actorlevel:%fs\n", timeuse );
//#endif
}

// 城市主城等级
int city_mainlevel( int city_index )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return 0;
	return g_city[city_index].building[0].level;
}

// 标志位
void city_set_sflag( City *pCity, int offset, char value )
{
	if ( pCity == NULL )
		return;
	if ( value == 0 )
	{
		pCity->sflag &= ~(1 << offset);
	}
	else
	{
		pCity->sflag |= (1 << offset);
	}
}
int city_get_sflag( City *pCity, int offset )
{
	if ( pCity == NULL )
		return 0;
	if ( pCity->sflag & (1 << offset) )
		return 1;
	return 0;
}

// 功能获取
void city_function_open( City *pCity, int offset )
{
	if ( pCity == NULL )
		return;
	pCity->function |= (1 << offset);

	SLK_NetS_Function pValue = { 0 };
	pValue.m_function = pCity->function;
	pValue.m_openoffset = offset;
	netsend_function_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
}

int city_function_check( City *pCity, int offset )
{
	if ( pCity == NULL )
		return 0;
	if ( pCity->function & (1 << offset) )
		return 1;
	return 0;
}

int city_event_load( City *pCity )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[1024] = { 0 };
	if ( !pCity )
		return -1;

	// 内政
	sprintf( szSQL, "select `offset`,`type`,`kind`,`value`,`optime` from city_event_c where actorid=%d", pCity->actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( (row = mysql_fetch_row( res )) )
	{
		int offset = atoi( row[0] );
		if ( offset < 0 || offset >= CITY_EVENT_MAX )
		{
			continue;
		}
		pCity->city_event[offset].m_type = atoi( row[1] );
		pCity->city_event[offset].m_kind = atoi( row[2] );
		pCity->city_event[offset].m_value = atoi( row[3] );
		pCity->city_event[offset].m_optime = atoi( row[4] );
	}
	mysql_free_result( res );

	// 军事
	sprintf( szSQL, "select `offset`,`type`,`name`,`value`,`optime` from city_event_b where actorid=%d", pCity->actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( (row = mysql_fetch_row( res )) )
	{
		int offset = atoi( row[0] );
		if ( offset < 0 || offset >= CITY_EVENT_MAX )
		{
			continue;
		}
		pCity->battle_event[offset].m_type = atoi( row[1] );
		strncpy( pCity->battle_event[offset].m_name, row[2], 21 );
		pCity->battle_event[offset].m_value = atoi( row[3] );
		pCity->battle_event[offset].m_optime = atoi( row[4] );
	}
	mysql_free_result( res );
	return 0;
}

int city_event_save( City *pCity, FILE *fp )
{
	char	szSQL[1024] = { 0 };
	char reconnect_flag = 0;
	if ( pCity == NULL )
		return -1;
	char szText_name[MAX_PATH] = { 0 };
	// 内政
	for ( int offset = 0; offset < CITY_EVENT_MAX; offset++ )
	{
		sprintf( szSQL, "REPLACE INTO city_event_c (`actorid`,`offset`,`type`,`kind`,`value`,`optime`) Values('%d','%d','%d','%d','%d','%d')", pCity->actorid, offset, pCity->city_event[offset].m_type, pCity->city_event[offset].m_kind, pCity->city_event[offset].m_value, pCity->city_event[offset].m_optime );
		if ( fp )
		{
			fprintf( fp, "%s;\n", szSQL );
		}
		else if ( mysql_query( myGame, szSQL ) )
		{
			printf( "Query failed (%s)\n", mysql_error( myGame ) );
			write_gamelog( "%s", szSQL );
		}
	}

	// 军事
	for ( int offset = 0; offset < CITY_EVENT_MAX; offset++ )
	{
		sprintf( szSQL, "REPLACE INTO city_event_b (`actorid`,`offset`,`type`,`name`,`value`,`optime`) Values('%d','%d','%d','%s','%d','%d')", pCity->actorid, offset, pCity->battle_event[offset].m_type, db_escape( (const char *)pCity->battle_event[offset].m_name, szText_name, 0 ), pCity->battle_event[offset].m_value, pCity->battle_event[offset].m_optime );
		if ( fp )
		{
			fprintf( fp, "%s;\n", szSQL );
		}
		else if ( mysql_query( myGame, szSQL ) )
		{
			printf( "Query failed (%s)\n", mysql_error( myGame ) );
			write_gamelog( "%s", szSQL );
		}
	}
	return 0;
}

// 内政事件
int city_event_add( int city_index, char type, short kind, int value )
{
	CITY_CHECK_INDEX( city_index );
	memmove( &g_city[city_index].city_event[1], &g_city[city_index].city_event[0], sizeof( SLK_NetS_CityEvent )*(CITY_EVENT_MAX - 1) );
	g_city[city_index].city_event[0].m_type = type;
	g_city[city_index].city_event[0].m_kind = kind;
	g_city[city_index].city_event[0].m_value = value;
	g_city[city_index].city_event[0].m_optime = (int)time(NULL);
	return 0;
}

// 军事事件
int city_battle_event_add( int city_index, char type, char *name, char value, i64 mailid )
{
	CITY_CHECK_INDEX( city_index );
	memmove( &g_city[city_index].battle_event[1], &g_city[city_index].battle_event[0], sizeof( SLK_NetS_BattleEvent )*(CITY_EVENT_MAX - 1) );
	g_city[city_index].battle_event[0].m_type = type;
	strncpy( g_city[city_index].battle_event[0].m_name, name, 21 );
	g_city[city_index].battle_event[0].m_name[21] = 0;
	g_city[city_index].battle_event[0].m_value = value;
	g_city[city_index].battle_event[0].m_mailid = (unsigned int )mailid;
	g_city[city_index].battle_event[0].m_optime = (int)time( NULL );
	return 0;
}

int city_event_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_EventList pValue = { 0 };
	for ( int tmpi = 0; tmpi < CITY_EVENT_MAX; tmpi++ )
	{
		memcpy( &pValue.m_cevent_list[tmpi], &pCity->city_event[tmpi], sizeof( SLK_NetS_CityEvent ) );
		pValue.m_cevent_count += 1;
	}
	for ( int tmpi = 0; tmpi < CITY_EVENT_MAX; tmpi++ )
	{
		memcpy( &pValue.m_bevent_list[tmpi], &pCity->battle_event[tmpi], sizeof( SLK_NetS_BattleEvent ) );
		pValue.m_bevent_count += 1;
	}
	netsend_cityeventlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 主角经验获取
int city_actorexp( int city_index, int exp, char path )
{
	CITY_CHECK_INDEX( city_index );
	if ( exp == 0 )
		return 0;
	char isup = 0;
	AwardGetInfo getinfo = { 0 };
	if ( exp > 0 )
	{
		// 增加经验
		g_city[city_index].exp += exp;
		wlog( 0, LOGOP_ACTOREXP, path, exp, g_city[city_index].exp, g_city[city_index].level, g_city[city_index].actorid, city_mainlevel( city_index ) );

		// 检查升级
		while ( g_city[city_index].exp >= g_upgradeinfo[g_city[city_index].level].exp )
		{
			int curlevel = g_city[city_index].level;
			// 可以升级
			if ( city_actorupgrade( city_index, path, &getinfo ) < 0 )
				break;
			g_city[city_index].exp -= g_upgradeinfo[curlevel].exp;
			isup = 1;
		}
	}
	else if ( exp < 0 )
	{	
		// 扣减经验
		g_city[city_index].exp += exp;
		wlog( 0, LOGOP_ACTOREXP, path, exp, g_city[city_index].exp, g_city[city_index].level, g_city[city_index].actorid, city_mainlevel( city_index ) );

		// 检查降级
		if ( g_city[city_index].exp < 0 )
		{
			g_city[city_index].exp = 0;
			if ( g_city[city_index].level > 1 )
				g_city[city_index].level -= 1;
		}
	}

	if ( g_city[city_index].actor_index >= 0 )
	{
		SLK_NetS_Experience Value = {};
		Value.m_addexp = exp;
		Value.m_curexp = g_city[city_index].exp;
		Value.m_expmax = g_upgradeinfo[g_city[city_index].level].exp;
		Value.m_level = g_city[city_index].level;
		Value.m_isup = isup;
		Value.m_path = path;
		netsend_experience_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &Value );
	}
	return isup;
}

// 主角升级
int city_actorupgrade( int city_index, short path, AwardGetInfo *getinfo )
{
	CITY_CHECK_INDEX( city_index );
	if ( g_city[city_index].level >= global.actorlevel_max )
		return -1;
	int lastlevel = g_city[city_index].level;
	g_city[city_index].level += 1;
	wlog( 0, LOGOP_UPGRADE, path, 0, g_city[city_index].level, 0, g_city[city_index].actorid, city_mainlevel( city_index ) );

	// 同步更新到玩家身上
	if ( g_city[city_index].actor_index >= 0 && g_city[city_index].actor_index < g_maxactornum )
	{
		g_actors[g_city[city_index].actor_index].level = g_city[city_index].level;
	}
	return 0;
}

// 体力
int city_changebody( int city_index, int value, short path )
{
	CITY_CHECK_INDEX( city_index );
	if ( g_city[city_index].body >= global.body_max && path == PATH_SYSTEM )
	{
		return -1;
	}
	if ( value > 0 && g_city[city_index].body > SHRT_MAX - value )
		g_city[city_index].body = SHRT_MAX;
	else
		g_city[city_index].body += value;
	
	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_Body pValue = { 0 };
	pValue.m_add = value;
	pValue.m_total = g_city[city_index].body;
	pValue.m_path = path;
	pValue.m_bodysec = g_city[city_index].bodysec;
	netsend_body_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
// 征收次数
int city_changelevy( int city_index, int value, short path )
{
	CITY_CHECK_INDEX( city_index );
	if ( value > 0 && g_city[city_index].levynum > CHAR_MAX - value )
		g_city[city_index].levynum = CHAR_MAX;
	else
		g_city[city_index].levynum += value;

	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_Levy pValue = { 0 };
	pValue.m_add = value;
	pValue.m_total = g_city[city_index].levynum;
	pValue.m_sec = g_city[city_index].levysec;
	pValue.m_max = global.levy_max;
	pValue.m_path = path;
	netsend_changelevy_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
// 银币
int city_changesilver( int city_index, int value, short path )
{
	if ( value == 0 )
		return -1;
	CITY_CHECK_INDEX( city_index );
	if ( value > 0 && g_city[city_index].silver > INT_MAX - value )
		g_city[city_index].silver = INT_MAX;
	else
		g_city[city_index].silver += value;

	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_Silver pValue = { 0 };
	pValue.m_add = value;
	pValue.m_total = g_city[city_index].silver;
	pValue.m_path = path;
	netsend_changesilver_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
// 木材
int city_changewood( int city_index, int value, short path )
{
	if ( value == 0 )
		return -1;
	CITY_CHECK_INDEX( city_index );
	if ( value > 0 && g_city[city_index].wood > INT_MAX - value )
		g_city[city_index].wood = INT_MAX;
	else
		g_city[city_index].wood += value;

	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_Wood pValue = { 0 };
	pValue.m_add = value;
	pValue.m_total = g_city[city_index].wood;
	pValue.m_path = path;
	netsend_changewood_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
// 粮草
int city_changefood( int city_index, int value, short path )
{
	if ( value == 0 )
		return -1;
	CITY_CHECK_INDEX( city_index );
	if ( value > 0 && g_city[city_index].food > INT_MAX - value )
		g_city[city_index].food = INT_MAX;
	else
		g_city[city_index].food += value;

	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_Food pValue = { 0 };
	pValue.m_add = value;
	pValue.m_total = g_city[city_index].food;
	pValue.m_path = path;
	netsend_changefood_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
// 镔铁
int city_changeiron( int city_index, int value, short path )
{
	if ( value == 0 )
		return -1;
	CITY_CHECK_INDEX( city_index );
	if ( value > 0 && g_city[city_index].iron > INT_MAX - value )
		g_city[city_index].iron = INT_MAX;
	else
		g_city[city_index].iron += value;

	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_Iron pValue = { 0 };
	pValue.m_add = value;
	pValue.m_total = g_city[city_index].iron;
	pValue.m_path = path;
	netsend_changeiron_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
// 人口
int city_changepeople( int city_index, int value, short path )
{
	CITY_CHECK_INDEX( city_index );
	if ( value > 0 && g_city[city_index].people > INT_MAX - value )
		g_city[city_index].people = INT_MAX;
	else
		g_city[city_index].people += value;

	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_People pValue = { 0 };
	pValue.m_add = value;
	pValue.m_total = g_city[city_index].people;
	pValue.m_path = path;
	netsend_changepeople_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
// 威望值
int city_changeprestige( int city_index, int value, short path )
{
	CITY_CHECK_INDEX( city_index );
	if ( value > 0 && g_city[city_index].prestige > INT_MAX - value )
		g_city[city_index].prestige = INT_MAX;
	else
		g_city[city_index].prestige += value;

	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_Prestige pValue = { 0 };
	pValue.m_add = value;
	pValue.m_total = g_city[city_index].prestige;
	pValue.m_path = path;
	netsend_changeprestige_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
// 友谊积分
int city_changefriendship( int city_index, int value, short path )
{
	CITY_CHECK_INDEX( city_index );
	if ( value > 0 && g_city[city_index].friendship > INT_MAX - value )
		g_city[city_index].friendship = INT_MAX;
	else
		g_city[city_index].friendship += value;

	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_Friendship pValue = { 0 };
	pValue.m_add = value;
	pValue.m_total = g_city[city_index].friendship;
	pValue.m_path = path;
	netsend_changefriendship_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
// 保护时间
int city_changeprotect( int city_index, int value, short path )
{
	CITY_CHECK_INDEX( city_index );
	int old = g_city[city_index].ptsec;
	g_city[city_index].ptsec += value;
	if ( g_city[city_index].ptsec < 0 )
		g_city[city_index].ptsec = 0;

	SLK_NetS_CityProtect pValue = { 0 };
	pValue.m_add = value;
	pValue.m_total = g_city[city_index].ptsec;
	pValue.m_path = path;
	netsend_cityprotect_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );

	if ( old == 0 || value == 0 )
	{
		mapunit_update( MAPUNIT_TYPE_CITY, city_index, g_city[city_index].unit_index );
	}
	return 0;
}

// 兵力
int city_changesoldiers( int city_index, short corps, int value, short path )
{
	CITY_CHECK_INDEX( city_index );
	int total;
	int kind[2] = { 0 };
	if ( corps == 0 )
	{
		kind[0] = BUILDING_Infantry;
		kind[1] = BUILDING_Militiaman_Infantry;
	}
	else if ( corps == 1 )
	{
		kind[0] = BUILDING_Cavalry;
		kind[1] = BUILDING_Militiaman_Cavalry;
	}
	else if ( corps == 2 )
	{
		kind[0] = BUILDING_Archer;
		kind[1] = BUILDING_Militiaman_Archer;
	}
	
	if ( value < 0 )
	{
		int sub = -value;
		for ( int tmpi = 0; tmpi < 2; tmpi++ )
		{
			BuildingBarracks *barracks = buildingbarracks_getptr_kind( city_index, kind[tmpi] );
			if ( !barracks )
				continue;
			barracks->soldiers -= sub;
			if ( barracks->soldiers < 0 )
			{
				sub = barracks->soldiers;
				barracks->soldiers = 0;
			}
			else
			{
				sub = 0;
				break;
			}
		}
	}
	else
	{
		BuildingBarracks *barracks = buildingbarracks_getptr_kind( city_index, kind[0] );
		if ( !barracks )
			return -1;
		barracks->soldiers += value;
	}

	SLK_NetS_Soldiers pValue = { 0 };
	pValue.m_corps = (char)corps;
	pValue.m_add = value;
	pValue.m_soldiers = city_soldiers( city_index, corps );
	pValue.m_path = path;
	netsend_soldiers_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

CityGuardInfoConfig *city_guard_config( int monsterid, int color )
{
	if ( monsterid <= 0 || monsterid >= g_cityguardinfo_maxnum )
		return NULL;
	if ( color < 0 || color >= g_cityguardinfo[monsterid].maxnum )
		return NULL;
	return &g_cityguardinfo[monsterid].config[color];
}

CityGuard *city_guard_getptr( int city_index, int offset )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	if ( offset < 0 || offset >= CITY_GUARD_MAX )
		return NULL;
	return &g_city[city_index].guard[offset];
}

// 添加城墙守卫
int city_guard_call( int city_index )
{
	CITY_CHECK_INDEX( city_index );
	if ( g_city[city_index].guardsec > 0 )
	{
		actor_system_message( g_city[city_index].actor_index, 795 );
		return -1;
	}
	Building *pBuilding = building_getptr_kind( city_index, BUILDING_Wall );
	if ( !pBuilding )
		return -1;
	if ( pBuilding->level < global.city_guard_level )
		return -1;
	int count = 0;
	for ( int tmpi = 0; tmpi < CITY_GUARD_MAX; tmpi++ )
	{
		if ( g_city[city_index].guard[tmpi].monsterid > 0 )
			count += 1;
	}
	if ( count >= pBuilding->level )
	{
		return -1;
	}
	
	int offset = -1;
	for ( int tmpi = 0; tmpi < CITY_GUARD_MAX; tmpi++ )
	{
		if ( g_city[city_index].guard[tmpi].monsterid <= 0 )
		{
			offset = tmpi;
			break;
		}
	}
	if ( offset < 0 )
	{
		return -1;
	}
	
	BuildingUpgradeConfig *buildingconfig = building_getconfig( BUILDING_Wall, pBuilding->level );
	if ( !buildingconfig )
		return -1;

	char monsterid = random( 1, g_cityguardinfo_maxnum-1 );
	char color = ITEM_COLOR_LEVEL_BLUE;
	char corps = random( 0, 2 );
	char shape = random( 0, 7 );
	unsigned char minlevel = buildingconfig->value[0];
	unsigned char maxlevel = buildingconfig->value[1];
	unsigned char level = random( minlevel, maxlevel );

	CityGuardInfoConfig *config = city_guard_config( monsterid, color );
	if ( config )
	{
		g_city[city_index].guard[offset].monsterid = monsterid;
		g_city[city_index].guard[offset].corps = corps;
		g_city[city_index].guard[offset].color = color;
		g_city[city_index].guard[offset].shape = shape;
		g_city[city_index].guard[offset].level = level;
		g_city[city_index].guard[offset].soldiers = TROOPS( level, config->troops, config->troops_growth );
		g_city[city_index].guardsec = global.city_guard_sec;
	}
	city_guard_send( g_city[city_index].actor_index, offset );
	city_guard_sendsec( g_city[city_index].actor_index );
	return 0;
}

// 升级城墙守卫
int city_guard_upgrade( int city_index, int offset )
{
	CITY_CHECK_INDEX( city_index );
	if ( offset < 0 || offset >= CITY_GUARD_MAX )
		return -1;
	Building *pBuilding = building_getptr_kind( city_index, BUILDING_Wall );
	if ( !pBuilding )
		return -1;
	BuildingUpgradeConfig *buildingconfig = building_getconfig( BUILDING_Wall, pBuilding->level );
	if ( !buildingconfig )
		return -1;
	char monsterid = g_city[city_index].guard[offset].monsterid;
	char color = g_city[city_index].guard[offset].color;
	char level = g_city[city_index].guard[offset].level;
	if ( color < 0 || color >= 6 )
		return -1;
	
	// 等级升级
	unsigned char maxlevel = buildingconfig->value[1];
	unsigned char randomlevel = buildingconfig->value[2];
	if ( randomlevel == 0 )
		randomlevel = 1;
	unsigned char addlevel = random( 1, randomlevel );
	if ( level + addlevel > maxlevel )
		addlevel = maxlevel - level;

	// 颜色升级
	// 取出当前等级所能达到的最高颜色
	char maxcolor = 1;
	for ( int i = 5; i >= 1; i-- )
	{
		if ( level >= global.city_guard_color_min[i] )
		{
			maxcolor = i;
			break;
		}
	}

	char coloradd = 0;
	char colorodds = buildingconfig->value[3];
	if ( colorodds <= 0 )
		colorodds = 1;
	if ( color < maxcolor && (rand() % 100 <= colorodds) )
	{
		coloradd = 1;
	}
	
	if ( actor_change_token( g_city[city_index].actor_index, -global.city_guard_up_token, PATH_GUARD_UPGRADE, 0 ) < 0 )
	{
		return -1;
	}
	g_city[city_index].guard[offset].color += coloradd;
	g_city[city_index].guard[offset].level += addlevel;

	CityGuardInfoConfig *config = city_guard_config( monsterid, g_city[city_index].guard[offset].color );
	if ( config )
	{
		g_city[city_index].guard[offset].soldiers = TROOPS( g_city[city_index].guard[offset].level, config->troops, config->troops_growth );
	}
	city_guard_send( g_city[city_index].actor_index, offset );
	return 0;
}

// 城墙守卫补血
int city_guard_soldiers( int city_index, int offset )
{
	CITY_CHECK_INDEX( city_index );
	if ( offset < 0 || offset >= CITY_GUARD_MAX )
		return -1;
	char monsterid = g_city[city_index].guard[offset].monsterid;
	char color = g_city[city_index].guard[offset].color;
	unsigned char level = g_city[city_index].guard[offset].level;
	CityGuardInfoConfig *config = city_guard_config( monsterid, color );
	if ( config )
	{
		if ( actor_change_token( g_city[city_index].actor_index, -global.city_guard_up_token, PATH_GUARD_LIFE, 0 ) < 0 )
		{
			return -1;
		}
		g_city[city_index].guard[offset].soldiers = TROOPS( level, config->troops, config->troops_growth );
		city_guard_send( g_city[city_index].actor_index, offset );
	}
	return 0;
}

// 消除冷却
int city_guard_clearcd( int city_index )
{
	CITY_CHECK_INDEX( city_index );
	if ( g_city[city_index].guardsec <= 0 )
		return -1;
	int token = (int)ceil( (g_city[city_index].guardsec / (float)global.city_guard_sec_token) );
	if ( actor_change_token( g_city[city_index].actor_index, -token, PATH_GUARD_UPGRADE, 0 ) < 0 )
		return -1;
	g_city[city_index].guardsec = 0;
	city_guard_sendsec( g_city[city_index].actor_index );
	return 0;
}

void city_guard_makestruct( City *pCity, SLK_NetS_CityGuard *pValue, char offset )
{
	if ( !pCity )
		return;
	if ( !pValue )
		return;
	if ( offset < 0 || offset >= CITY_GUARD_MAX )
		return;
	pValue->m_offset = offset;
	pValue->m_corps = pCity->guard[offset].corps;
	pValue->m_color = pCity->guard[offset].color;
	pValue->m_level = pCity->guard[offset].level;
	pValue->m_shape = pCity->guard[offset].shape;
	pValue->m_soldiers = pCity->guard[offset].soldiers;
	CityGuardInfoConfig *config = city_guard_config( pCity->guard[offset].monsterid, pCity->guard[offset].color );
	if ( config )
	{
		pValue->m_troops = TROOPS( pCity->guard[offset].level, config->troops, config->troops_growth );
	}
}

int city_guard_send( int actor_index, int offset )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( offset < 0 || offset >= CITY_GUARD_MAX )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_CityGuard pValue = { 0 };
	city_guard_makestruct( pCity, &pValue, offset );
	netsend_cityguard_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

int city_guard_sendsec( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_CityGuardSec pValue = { 0 };
	pValue.m_guardsec = pCity->guardsec;
	netsend_cityguardsec_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

int city_guard_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_CityGuardList pValue = { 0 };
	pValue.m_guardsec = pCity->guardsec;;
	for ( int tmpi = 0; tmpi < CITY_GUARD_MAX; tmpi++ )
	{
		if ( pCity->guard[tmpi].monsterid <= 0 )
			continue;
		city_guard_makestruct( pCity, &pValue.m_list[pValue.m_count], tmpi );
		pValue.m_count += 1;
	}
	netsend_cityguardlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 获取基础产量
int city_yield_base( City *pCity, int kind )
{
	if ( !pCity )
		return 0;
	int yield = 0;
	for ( int tmpi = 0; tmpi < BUILDING_RES_MAXNUM; tmpi++ )
	{
		if ( pCity->building_res[tmpi].kind == kind )
		{
			BuildingUpgradeConfig *config = building_getconfig( pCity->building_res[tmpi].kind, pCity->building_res[tmpi].level );
			if ( config )
			{
				yield += config->value[1];
			}
		}
	}
	return yield;
}

// 获取科技加成产量加成
int city_yield_tech( City *pCity, int kind )
{
	if ( !pCity )
		return 0;
	int yield = 0;
	for ( int tech_kind = 1; tech_kind < CITY_TECH_MAX; tech_kind++ )
	{
		if ( tech_kind >= g_techinfo_maxnum )
			break;
		char tech_level = pCity->techlevel[tech_kind];
		if ( tech_level > 0 && tech_level < g_techinfo[tech_kind].maxnum )
		{
			if ( kind == BUILDING_Silver && g_techinfo[tech_kind].config[tech_level].ability == CITY_ATTR_ABILITY_1 )
			{
				yield += g_techinfo[tech_kind].config[tech_level].value;
			}
			else if ( kind == BUILDING_Wood && g_techinfo[tech_kind].config[tech_level].ability == CITY_ATTR_ABILITY_2 )
			{
				yield += g_techinfo[tech_kind].config[tech_level].value;
			}
			else if ( kind == BUILDING_Food && g_techinfo[tech_kind].config[tech_level].ability == CITY_ATTR_ABILITY_3 )
			{
				yield += g_techinfo[tech_kind].config[tech_level].value;
			}
			else if ( kind == BUILDING_Iron && g_techinfo[tech_kind].config[tech_level].ability == CITY_ATTR_ABILITY_4 )
			{
				yield += g_techinfo[tech_kind].config[tech_level].value;
			}
		}
	}
	return yield;
}

// 内政官加成
int city_yield_official( City *pCity, int kind )
{
	if ( !pCity )
		return 0;
	int ofkind = pCity->ofkind[1];
	if ( ofkind <= 0 || ofkind >= g_official_gov_maxnum )
		return 0;
	if ( kind == BUILDING_Iron && g_official_gov[ofkind].haveiron == 0 )
		return 0;
	return g_official_gov[ofkind].produce;
}

int city_yield_total( City *pCity, int kind )
{
	if ( !pCity )
		return 0;
	int base_yield = city_yield_base( pCity, kind );
	int tech_yield = (int)ceil( base_yield * (city_yield_tech( pCity, kind ) / 100.0f) );
	int official_yield = (int)ceil( base_yield * (city_yield_official( pCity, kind )/100.0f) );
	int yield = base_yield + tech_yield + official_yield;
	return yield;
}

// 征收
int city_levy( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->levynum <= 0 )
		return -1;

	int silver = city_yield_total( pCity, BUILDING_Silver );
	if ( silver > 0 )
		city_changesilver( pCity->index, silver, PATH_LEVY );

	int wood = city_yield_total( pCity, BUILDING_Wood );
	if ( wood > 0 )
		city_changewood( pCity->index, wood, PATH_LEVY );

	int food = city_yield_total( pCity, BUILDING_Food );
	if ( food > 0 )
		city_changefood( pCity->index, food, PATH_LEVY );

	int iron = city_yield_total( pCity, BUILDING_Iron );
	if ( iron > 0 )
		city_changeiron( pCity->index, iron, PATH_LEVY );

	city_changelevy( pCity->index, -1, PATH_LEVY );
	return 0;
}

int city_levy_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_LevyInfo pValue = { 0 };
	pValue.m_sec = pCity->levysec;
	pValue.m_base[0] = city_yield_base( pCity, BUILDING_Silver );
	pValue.m_base[1] = city_yield_base( pCity, BUILDING_Wood );
	pValue.m_base[2] = city_yield_base( pCity, BUILDING_Food );
	pValue.m_base[3] = city_yield_base( pCity, BUILDING_Iron );

	pValue.m_tech[0] = (int)ceil( pValue.m_base[0] * (city_yield_tech( pCity, BUILDING_Silver ) / 100.0f) );
	pValue.m_tech[1] = (int)ceil( pValue.m_base[1] * (city_yield_tech( pCity, BUILDING_Wood ) / 100.0f) );
	pValue.m_tech[2] = (int)ceil( pValue.m_base[2] * (city_yield_tech( pCity, BUILDING_Food ) / 100.0f) );
	pValue.m_tech[3] = (int)ceil( pValue.m_base[3] * (city_yield_tech( pCity, BUILDING_Iron ) / 100.0f) );

		//pValue.m_weather[4]
		//pValue.m_activity[4]

	pValue.m_offical[0] = (int)ceil( pValue.m_base[0] * (city_yield_official( pCity, BUILDING_Silver ) / 100.0f) );
	pValue.m_offical[1] = (int)ceil( pValue.m_base[1] * (city_yield_official( pCity, BUILDING_Wood ) / 100.0f) );
	pValue.m_offical[2] = (int)ceil( pValue.m_base[2] * (city_yield_official( pCity, BUILDING_Food ) / 100.0f) );
	pValue.m_offical[3] = (int)ceil( pValue.m_base[3] * (city_yield_official( pCity, BUILDING_Iron ) / 100.0f) );
	netsend_levyinfo_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 招募每五分钟数
int city_trainnum( City *pCity, BuildingBarracks *barracks )
{
	if ( !pCity )
		return 0;
	if ( !barracks )
		return 0;
	BuildingUpgradeConfig *config = building_getconfig( barracks->kind, barracks->level );
	if ( !config )
		return 0;
	int corps = 0;
	if ( barracks->kind == BUILDING_Infantry || barracks->kind == BUILDING_Militiaman_Infantry )
		corps = 0;
	else if ( barracks->kind == BUILDING_Cavalry || barracks->kind == BUILDING_Militiaman_Cavalry )
		corps = 1;
	else if ( barracks->kind == BUILDING_Archer || barracks->kind == BUILDING_Militiaman_Archer )
		corps = 2;

	int base = config->value[0];
	int tech = (int)ceil(config->value[0] * pCity->attr.train_per[corps]);
	int buff = (int)ceil( config->value[0] * pCity->attr.trainspeed_per );
	int total = base + tech + buff;
	return total;
}

// 招募所需粮食单兵
int city_trainfood( City *pCity )
{
	if ( !pCity )
	{
		return global.trainfood;
	}
	int total = global.trainfood;
	return total;
}

// 兵营容量
int city_trainsoldiersmax( City *pCity, BuildingBarracks *barracks )
{
	if ( !pCity )
		return 0;
	if ( !barracks )
		return 0;
	BuildingUpgradeConfig *config = building_getconfig( barracks->kind, barracks->level );
	if ( !config )
		return 0;
	int maxnum = config->value[1] + barracks->queue * 3000;
	return maxnum;
}


// 招募
int city_train( int actor_index, int kind, int trainsec )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	BuildingBarracks *barracks = buildingbarracks_getptr_kind( pCity->index, kind );
	if ( !barracks )
		return -1;
	BuildingUpgradeConfig *config = building_getconfig( kind, barracks->level );
	if ( !config )
		return -1;
	if ( barracks->soldiers >= city_trainsoldiersmax( pCity, barracks ) )
		return -1;

	int v = trainsec / 300;
	int trainnum = v * city_trainnum( pCity, barracks );
	int trainfood = city_trainfood( pCity )*trainnum;
	if ( pCity->food < trainfood )
		return -1;

	// 如果当前正在招募，那么用招募队列
	if ( barracks->trainnum > 0 )
	{
		int queue = barracks->queue >= CITY_TRAINQUEUE_MAX ? CITY_TRAINQUEUE_MAX : barracks->queue;
		for ( int tmpi = 0; tmpi < queue; tmpi++ )
		{
			if ( barracks->queuenum[tmpi] <= 0 )
			{
				city_changefood( pCity->index, -trainfood, PATH_TRAIN );
				barracks->queuesec[tmpi] = trainsec;
				barracks->queuenum[tmpi] = trainnum;
				break;
			}
		}
	}
	else
	{
		city_changefood( pCity->index, -trainfood, PATH_TRAIN );
		barracks->trainsec = trainsec;
		barracks->trainsec_need = trainsec;
		barracks->trainnum = trainnum;
	}

	city_train_sendinfo( actor_index, kind );
	building_sendinfo_barracks( actor_index, kind ); 
	wlog( 0, LOGOP_BARRACKS, PATH_TRAIN, kind, trainnum, 0, g_actors[actor_index].actorid, city_mainlevel( g_actors[actor_index].city_index ) );
	return 0;
}

// 取消招募
int city_train_cancel( int actor_index, int kind, int queue )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	BuildingBarracks *barracks = buildingbarracks_getptr_kind( pCity->index, kind );
	if ( !barracks )
		return -1;
	BuildingUpgradeConfig *config = building_getconfig( kind, barracks->level );
	if ( !config )
		return -1;
	queue = queue - 1;
	if ( queue < 0 || queue >= CITY_TRAINQUEUE_MAX )
		return -1;
	int trainnum = barracks->queuenum[queue];
	int trainfood = city_trainfood( pCity )*trainnum;
	city_changefood( pCity->index, trainfood, PATH_TRAIN_CANCEL );
	barracks->queuesec[queue] = 0;
	barracks->queuenum[queue] = 0;
	if ( queue < CITY_TRAINQUEUE_MAX - 1 )
	{
		memmove( &barracks->queuesec[queue], &barracks->queuesec[queue + 1], sizeof( int )*(CITY_TRAINQUEUE_MAX - 1 - queue) );
		memmove( &barracks->queuenum[queue], &barracks->queuenum[queue + 1], sizeof( int )*(CITY_TRAINQUEUE_MAX - 1 - queue) );
	}
	city_train_sendinfo( actor_index, kind );
	wlog( 0, LOGOP_BARRACKS, PATH_TRAIN_CANCEL, kind, trainnum, 0, g_actors[actor_index].actorid, city_mainlevel( g_actors[actor_index].city_index ) );
	return 0;
}

// 招募完成
int city_train_finish( City *pCity, BuildingBarracks *barracks )
{
	if ( !pCity || !barracks )
		return -1;
	barracks->overnum += barracks->trainnum;
	barracks->trainsec = 0;
	barracks->trainsec_need = 0;
	barracks->trainnum = 0;

	// 招募队列有任务，那么取出任务
	int queue = barracks->queue >= CITY_TRAINQUEUE_MAX ? CITY_TRAINQUEUE_MAX : barracks->queue;
	for ( int tmpi = 0; tmpi < queue; tmpi++ )
	{
		if ( barracks->queuenum[tmpi] > 0 )
		{
			barracks->trainsec = barracks->queuesec[tmpi];
			barracks->trainsec_need = barracks->queuesec[tmpi];
			barracks->trainnum = barracks->queuenum[tmpi];
			barracks->queuesec[tmpi] = 0;
			barracks->queuenum[tmpi] = 0;
			if ( tmpi < queue - 1 )
			{
				memmove( &barracks->queuesec[tmpi], &barracks->queuesec[tmpi + 1], sizeof( int )*(queue - 1) );
				memmove( &barracks->queuenum[tmpi], &barracks->queuenum[tmpi + 1], sizeof( int )*(queue - 1) );
			}
			wlog( 0, LOGOP_BARRACKS, PATH_TRAIN, barracks->kind, barracks->trainnum, 0, pCity->actorid, city_mainlevel( pCity->index ) );
			break;
		}
	}
	
	building_sendinfo_barracks( pCity->actor_index, barracks->kind );
	city_train_sendinfo( pCity->actor_index, barracks->kind );
	return 0;
}

// 加速
int city_train_quick( int actor_index, int kind, int sec )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	BuildingBarracks *barracks = buildingbarracks_getptr_kind( pCity->index, kind );
	if ( !barracks )
		return -1;
	if ( barracks->trainsec <= 0 )
		return -1;
	barracks->trainsec -= sec;
	if ( barracks->trainsec <= 0 )
	{
		city_train_finish( pCity, barracks );
	}
	else
	{
		building_sendinfo_barracks( actor_index, kind );
		city_train_sendinfo( actor_index, kind );
	}
	return 0;
}

// 领取
int city_train_get( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	BuildingBarracks *barracks = buildingbarracks_getptr_kind( pCity->index, kind );
	if ( !barracks )
		return -1;
	if ( barracks->overnum <= 0 )
		return -1;
	int corps = 0;
	if ( kind == BUILDING_Infantry || kind == BUILDING_Militiaman_Infantry )
	{
		corps = 0;	
	}
	else if ( kind == BUILDING_Cavalry || kind == BUILDING_Militiaman_Cavalry )
	{
		corps = 1;
	}
	else if ( kind == BUILDING_Archer || kind == BUILDING_Militiaman_Archer )
	{
		corps = 2;
	}

	int overnum = barracks->overnum;
	barracks->soldiers += barracks->overnum;
	barracks->overnum = 0;
	// 事件
	city_event_add( pCity->index, CITY_EVENT_TRAIN, corps, overnum );
	building_sendinfo_barracks( actor_index, kind );

	SLK_NetS_Soldiers pValue = { 0 };
	pValue.m_corps = corps;
	pValue.m_add = overnum;
	pValue.m_soldiers = barracks->soldiers;
	netsend_soldiers_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );

	wlog( 0, LOGOP_BARRACKS, PATH_TRAIN_GET, kind, overnum, 0, pCity->actorid, city_mainlevel( pCity->index ) );
	return 0;
}

// 购买队列
int city_train_buyqueue( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	BuildingBarracks *barracks = buildingbarracks_getptr_kind( pCity->index, kind );
	if ( !barracks )
		return -1;
	if ( barracks->queue >= CITY_TRAINQUEUE_MAX )
		return -1;
	int newqueue = barracks->queue + 1;
	if ( newqueue >= g_trainqueue_maxnum )
		return -1;
	if ( actor_change_token( actor_index, -g_trainqueue[newqueue].token, PATH_TRAIN_QUEUE, 0 ) < 0 )
		return -1;
	barracks->queue += 1;
	city_train_sendinfo( actor_index, kind );

	if ( kind == BUILDING_Infantry || kind == BUILDING_Militiaman_Infantry )
		item_getitem( actor_index, 445, 1, -1, PATH_TRAIN_QUEUE );
	else if ( kind == BUILDING_Cavalry || kind == BUILDING_Militiaman_Cavalry )
		item_getitem( actor_index, 446, 1, -1, PATH_TRAIN_QUEUE );
	else if ( kind == BUILDING_Archer || kind == BUILDING_Militiaman_Archer )
		item_getitem( actor_index, 447, 1, -1, PATH_TRAIN_QUEUE );
	return 0;
}

// 购买时长
int city_train_buylong( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	BuildingBarracks *barracks = buildingbarracks_getptr_kind( pCity->index, kind );
	if ( !barracks )
		return -1;
	int newlong = barracks->trainlong + 1;
	if ( newlong >= g_trainlong_maxnum )
		return -1; 
	if ( pCity->silver < g_trainlong[newlong].silver )
		return -1;
	city_changesilver( pCity->index, -g_trainlong[newlong].silver, PATH_TRAIN_LONG );
	barracks->trainlong += 1;
	city_train_sendinfo( actor_index, kind );
	return 0;
}

int city_train_sendinfo( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	BuildingBarracks *barracks = buildingbarracks_getptr_kind( pCity->index, kind );
	if ( !barracks )
		return -1;
	BuildingUpgradeConfig *config = building_getconfig( kind, barracks->level );
	if ( !config )
		return -1;

	SLK_NetS_TrainInfo pValue = { 0 };
	pValue.m_soldiers = barracks->soldiers;
	pValue.m_soldiers_max = city_trainsoldiersmax( pCity, barracks );
	pValue.m_trainnum = barracks->trainnum;
	pValue.m_trainsec = barracks->trainsec;
	pValue.m_trainsec_need = barracks->trainsec_need;
	pValue.m_queue = barracks->queue;
	pValue.m_trainlong = barracks->trainlong;
	pValue.m_train_confnum = city_trainnum( pCity, barracks );
	pValue.m_train_confsec = g_trainlong[barracks->trainlong].timelong*60;
	pValue.m_train_conffood = city_trainfood( pCity );
	int count = 0;
	for ( int tmpi = 0; tmpi < CITY_TRAINQUEUE_MAX; tmpi++ )
	{
		if ( barracks->queuenum[tmpi] > 0 )
		{
			pValue.m_queuenum[count] = barracks->queuenum[tmpi];
			count += 1;
		}
	}
	netsend_traininfo_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 雇佣官
int city_officialhire( int actor_index, int type, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( type < 0 || type > 2 )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->ofkind[type] >= kind )
		return -1;
	if ( type == 0 )
	{
		if ( kind <= 0 || kind >= g_official_forging_maxnum )
			return -1;
		Building *pBuilding = building_getptr_kind( g_actors[actor_index].city_index, BUILDING_Main );
		if ( !pBuilding )
			return -1;
		if ( pBuilding->level < g_official_forging[kind].buildinglevel )
			return -1;

		if ( g_official_forging[kind].free == 0 || (pCity->offree[type] & (1 << kind)) )
		{ // 无免费
			if ( g_official_forging[kind].silver > 0 )
			{
				if ( pCity->silver < g_official_forging[kind].silver )
					return -1;
				city_changesilver( g_actors[actor_index].city_index, -g_official_forging[kind].silver, PATH_HIRE_FORGING );
			}
			else if ( g_official_forging[kind].token > 0 )
			{
				if ( actor_change_token( actor_index, -g_official_forging[kind].token, PATH_HIRE_FORGING, 0 ) < 0 )
					return -1;
			}
		}
		else
		{
			pCity->offree[type] |= (1 << kind);
		}


		pCity->ofkind[type] = kind;
		pCity->ofsec[type] += g_official_gov[kind].duration; // 时间叠加
		pCity->ofquick[type] = 0;
	}
	else if ( type == 1 )
	{
		if ( kind <= 0 || kind >= g_official_gov_maxnum )
			return -1;
		Building *pBuilding = building_getptr_kind( g_actors[actor_index].city_index, BUILDING_Main );
		if ( !pBuilding )
			return -1;
		if ( pBuilding->level < g_official_gov[kind].buildinglevel )
			return -1;

		if ( g_official_gov[kind].free == 0 || (pCity->offree[type] & (1 << kind)) )
		{ // 无免费
			if ( g_official_gov[kind].silver > 0 )
			{
				if ( pCity->silver < g_official_gov[kind].silver )
					return -1;
				city_changesilver( g_actors[actor_index].city_index, -g_official_gov[kind].silver, PATH_HIRE_GOV );
			}
			else if ( g_official_gov[kind].token > 0 )
			{
				if ( actor_change_token( actor_index, -g_official_tech[kind].token, PATH_HIRE_GOV, 0 ) < 0 )
					return -1;
			}
		}
		else
		{
			pCity->offree[type] |= (1 << kind);
		}

		
		pCity->ofkind[type] = kind;
		pCity->ofsec[type] = g_official_gov[kind].duration;
		pCity->ofquick[type] = 0;
	}
	else if ( type == 2 )
	{
		if ( kind <= 0 || kind >= g_official_tech_maxnum )
			return -1; 
		Building *pBuilding = building_getptr_kind( g_actors[actor_index].city_index, BUILDING_Tech );
		if ( !pBuilding )
			return -1;
		if ( pBuilding->level < g_official_tech[kind].buildinglevel )
			return -1;

		if ( g_official_tech[kind].free == 0 || (pCity->offree[type] & (1 << kind)) )
		{ // 无免费
			if ( g_official_tech[kind].silver > 0 )
			{
				if ( pCity->silver < g_official_tech[kind].silver )
					return -1;
				city_changesilver( g_actors[actor_index].city_index, -g_official_tech[kind].silver, PATH_HIRE_TECH );
			}
			else if( g_official_tech[kind].token > 0 )
			{
				if ( actor_change_token( actor_index, -g_official_tech[kind].token, PATH_HIRE_TECH, 0 ) < 0 )
					return -1;
			}
		}
		else
		{
			pCity->offree[type] |= (1 << kind);
		}

		int oldkind = pCity->ofkind[type];
		pCity->ofkind[type] = kind;
		if ( oldkind > 0 && oldkind < g_official_tech_maxnum )
		{
			if ( g_official_tech[oldkind].token > 0 )
				pCity->ofsec[type] += g_official_tech[kind].duration;
			else
				pCity->ofsec[type] = g_official_tech[kind].duration;

			pCity->ofquick[type] = g_official_tech[kind].quick - g_official_tech[oldkind].quick;
		}
		else
		{
			pCity->ofsec[type] = g_official_tech[kind].duration;
			pCity->ofquick[type] = 0;
		}

	}
	city_officialhire_sendinfo( pCity, type );
	return 0;
}

int city_officialhire_sendinfo( City *pCity, int type )
{
	if ( !pCity )
		return -1;
	if ( type < 0 || type > 2 )
		return -1;
	SLK_NetS_OfficialHireChange pValue = { 0 };
	pValue.m_type = type;
	pValue.m_info.m_ofkind = pCity->ofkind[type];
	pValue.m_info.m_ofsec = pCity->ofsec[type];
	pValue.m_info.m_offree = pCity->offree[type];
	pValue.m_info.m_ofquick = pCity->ofquick[type];
	netsend_officialhirechange_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 获取士兵数量
int city_soldiers( int city_index, short corps )
{
	CITY_CHECK_INDEX( city_index );
	int kind = 0;
	if ( corps == 0 )
		kind = BUILDING_Infantry;
	else if ( corps == 1 )
		kind = BUILDING_Cavalry;
	else
		kind = BUILDING_Archer;

	int total = building_soldiers_total( city_index, kind );
	return total;
}