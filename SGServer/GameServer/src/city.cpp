#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <math.h>
#include <limits.h>
#include "db.h"
#include "dserver.h"
#include "global.h"
#include "wqueue.h"
#include "actor.h"
#include "actor_times.h"
#include "city.h"
#include "city_attr.h"
#include "city_tech.h"
#include "building.h"
#include "hero.h"
#include "equip.h"
#include "mapunit.h"
#include "zoneunit.h"
#include "map.h"
#include "map_zone.h"
#include "server_netsend_auto.h"
#include "system.h"
#include "item.h"
#include "vip.h"
#include "actor_notify.h"
#include "army.h"
#include "army_group.h"
#include "map_enemy.h"
#include "map_res.h"
#include "map_event.h"
#include "map_call.h"
#include "mail.h"
#include "nation.h"
#include "world_quest.h"
#include "quest.h"
#include "pay.h"
#include "nation_equip.h"
#include "nation_hero.h"
#include "actor_friend.h"
#include "activity_04.h"

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

extern MaterialMakeInfo *g_material_make;
extern int g_material_make_maxnum;

extern MapZoneInfo *g_zoneinfo;
extern int g_zoneinfo_maxnum;

extern Army *g_army;
extern int g_army_maxcount;

extern BuildingUpgrade *g_building_upgrade;
extern int g_building_upgrade_maxnum;

extern ArmyGroup *g_armygroup;
extern int g_armygroup_maxcount;

extern char g_gm_outresult[MAX_OUTRESULT_LEN];

extern int g_city_maxindex;
City *g_city = NULL;
int g_city_maxcount = 0;
char g_city_allinited = 0;
char g_szSpyMailjson[8192] = { 0 };
char g_szSpyMailjsonBak[8192] = { 0 };

// 城池数据重置
int city_reset()
{
	memset( g_city, 0, sizeof(City)*g_city_maxcount );
	for ( int tmpi = 0; tmpi < g_city_maxcount; tmpi++ )
	{
		g_city[tmpi].index = -1;
		g_city[tmpi].unit_index = -1;
		g_city[tmpi].actor_index = -1;
		for ( int index = 0; index < CITY_BATTLEQUEUE_MAX; index++ )
		{
			g_city[tmpi].battle_armyindex[index] = -1;
		}
		for ( int index = 0; index < CITY_UNDERFIRE_MAX; index++ )
		{
			g_city[tmpi].underfire_armyindex[index] = -1;
		}
		for ( int index = 0; index < CITY_HELPDEFENSE_MAX; index++ )
		{
			g_city[tmpi].help_armyindex[index] = -1;
		}
		for ( int index = 0; index < CITY_MAPEVENT_MAX; index++ )
		{
			g_city[tmpi].mapevent_index[index] = -1;
		}
		for ( int index = 0; index < CITY_UNDERFIRE_GROUP_MAX; index++ )
		{
			g_city[tmpi].underfire_groupindex[index] = -1;
		}
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

	// 读取国家名将信息
	city_nation_hero_load_auto( g_city[city_index].actorid, city_index, city_nation_hero_getptr, "city_nation_hero" );

	// 读取城墙守卫
	city_guard_load_auto( g_city[city_index].actorid, city_index, city_guard_getptr, "city_guard" );

	// 城池事件
	city_event_load( &g_city[city_index] );
	// 计算临时属性
	city_attr_reset( &g_city[city_index] );

	// 计算所有上阵英雄属性
	for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
	{
		if ( g_city[city_index].hero[tmpi].kind <= 0 )
			continue;
		hero_attr_calc( &g_city[city_index], &g_city[city_index].hero[tmpi] );
	}

	// 城墙守卫数量
	for ( int tmpi = 0; tmpi < CITY_GUARD_MAX; tmpi++ )
	{
		if ( g_city[city_index].guard[tmpi].monsterid > 0 )
			g_city[city_index].guardnum += 1;
	}

	// 最后计算战力
	city_battlepower_reset( &g_city[city_index] );

	// 重新随机位置
	if ( g_city[city_index].posx == -1 || g_city[city_index].posy == -1 )
	{
		map_getrandcitypos( &g_city[city_index].posx, &g_city[city_index].posy );
	}
	// 添加到地图显示单元
	g_city[city_index].unit_index = mapunit_add( MAPUNIT_TYPE_CITY, city_index );
	g_city[city_index].zoneunit_index = zoneunit_add( MAPUNIT_TYPE_CITY, city_index );

	// 占地块信息添加到世界地图
	map_addobject( MAPUNIT_TYPE_CITY, g_city[city_index].unit_index, g_city[city_index].posx, g_city[city_index].posy );
	g_city[city_index].zone = map_zone_getid( g_city[city_index].posx, g_city[city_index].posy );
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

	// 国家名将
	city_nation_hero_batch_save_auto( pCity->actorid, pCity->nation_hero, NATIONHERO_MAX, "city_nation_hero", fp );

	// 城墙守卫
	db_delete( pCity->actorid, "city_guard", fp );
	city_guard_batch_save_auto( pCity->actorid, pCity->guard, CITY_GUARD_MAX, "city_guard", fp );

	// 城池事件
	city_event_save( pCity, fp );
	return 0;
}

// 获取这个玩家的城池属性
inline City *city_indexptr( int city_index )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	return &g_city[city_index];
}

// 获取这个玩家的城池属性
inline City *city_getptr( int actor_index )
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

// 根据玩家名称找到城池索引
int city_getindex_withactorname( const char *actorname )
{
	if ( !actorname )
		return -1;
	int city_index = -1;
	for ( int tmpi = 0; tmpi < g_city_maxindex/*注意：使用索引位置，为了效率*/; tmpi++ )
	{
		if ( g_city[tmpi].actorid < MINACTORID )
			continue;
		if ( strcmp( g_city[tmpi].name, actorname ) == 0 )
		{
			city_index = tmpi;
			break;
		}
	}
	return city_index;
}

// 获取国家
char city_getnation( int city_index )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return 0;
	return g_city[city_index].nation;
}

// 获取国家名称
char* city_getname( int city_index )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return "";
	return g_city[city_index].name;
}

// 显示单元属性
void city_makeunit( int city_index, SLK_NetS_AddMapUnit *pAttr )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return;
	City *pCity = &g_city[city_index];
	if ( !pCity )
		return;
	if ( !pAttr )
		return;
	pAttr->m_state = pCity->state;
	pAttr->m_posx = pCity->posx;
	pAttr->m_posy = pCity->posy;
	strncpy( pAttr->m_name, pCity->name, sizeof( char )*NAME_SIZE );
	pAttr->m_namelen = strlen( pAttr->m_name );

	// 主城等级
	pAttr->m_char_value[0] = pCity->building[0].level;
	// 国家
	pAttr->m_char_value[1] = pCity->nation;
	// 城市保护
	if ( pCity->ptsec > 0 )
	{
		pAttr->m_char_value[2] = 1;
	}
	// 地图召唤
	pAttr->m_char_value[3] = pCity->mapcall;
	pAttr->m_char_value_count = 4;
	if ( pCity->mapcall > 0 )
	{
		MapCall * pMapCall = map_call_getptr( pCity->mapcall );
		if ( pMapCall )
		{
			pAttr->m_short_value[0] = pMapCall->endtimestamp - (int)time(NULL);
			pAttr->m_short_value[1] = pMapCall->num;
			pAttr->m_short_value[2] = pMapCall->maxnum;
			pAttr->m_short_value[3] = pMapCall->limitlevel;
			pAttr->m_short_value_count = 4;
		}
	}
}

void city_makezoneunit( int city_index, SLK_NetS_ZoneUnit *pAttr )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return;
	City *pCity = &g_city[city_index];
	if ( !pCity )
		return;
	if ( !pAttr )
		return;
	pAttr->m_posx = pCity->posx;
	pAttr->m_posy = pCity->posy;
	pAttr->m_level = (char)pCity->level;
	pAttr->m_nation = pCity->nation;
}

// 获取位置
void city_getpos( int city_index, short *posx, short *posy )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return;
	*posx = g_city[city_index].posx;
	*posy = g_city[city_index].posy;
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
			for ( int index = 0; index < CITY_BATTLEQUEUE_MAX; index++ )
			{
				g_city[city_index].battle_armyindex[index] = -1;
			}
			for ( int index = 0; index < CITY_UNDERFIRE_MAX; index++ )
			{
				g_city[city_index].underfire_armyindex[index] = -1;
			}
			for ( int index = 0; index < CITY_HELPDEFENSE_MAX; index++ )
			{
				g_city[city_index].help_armyindex[index] = -1;
			}
			for ( int tmpi = 0; tmpi < CITY_MAPEVENT_MAX; tmpi++ )
			{
				g_city[city_index].mapevent_index[tmpi] = -1;
			}
			for ( int tmpi = 0; tmpi < CITY_UNDERFIRE_GROUP_MAX; tmpi++ )
			{
				g_city[city_index].underfire_groupindex[tmpi] = -1;
			}
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

	// 国家任务
	g_city[city_index].nation_qv[0] = 0;
	g_city[city_index].nation_qv[1] = 0;
	g_city[city_index].nation_qv[2] = 0;

	// 活动礼包
	activity_paybag_citynew( &g_city[city_index] );

	// 存档
	city_save_auto( &g_city[city_index], "city", NULL );
	return city_index;
}

// 所有城市每秒的逻辑 , 一般是检查建筑的建造升级拆除，造兵等
void city_logic_sec( int begin, int end )
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
		
		// 商用建造队列到期时间
		if ( g_city[city_index].worker_expire_ex > 0 )
		{
			g_city[city_index].worker_expire_ex -= 1;
			if ( g_city[city_index].worker_expire_ex <= 0 )
			{
				if ( g_city[city_index].worker_sec_ex <= 0 )
				{
					building_sendworker( g_city[city_index].actor_index );
				}
			}
		}

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
				g_city[city_index].wnquick = 0;
				building_sendworker( g_city[city_index].actor_index );

				// 自动建造
				building_upgrade_autocheck( city_index );
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
				g_city[city_index].wnquick_ex = 0;
				building_sendworker( g_city[city_index].actor_index );

				// 自动建造
				building_upgrade_autocheck( city_index );
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
			case BUILDING_Main:
				if ( g_city[city_index].building[tmpi].level >= 10 )
				{
					BuildingUpgradeConfig *config = building_getconfig( kind, g_city[city_index].building[tmpi].level );
					if ( !config )
						break;

					// 人口
					int min_people = config->value[0]; // 城池人口下限
					int max_people = config->value[1]; // 城池人口上限
					int add = config->value[2]; // 每小时增长基数数

					g_city[city_index].peoplesec -= 1;
					if ( g_city[city_index].peoplesec <= 0 )
					{
						g_city[city_index].peoplesec = global.people_sec;

						if ( g_city[city_index].people < min_people )
						{ // 低于下限，增长
							g_city[city_index].people += (add + min( global.people_add_v1, (int)(min_people / (float)g_city[city_index].people * global.people_add_v2) ));
						}
						else if ( g_city[city_index].people > max_people )
						{ // 高于上限，减少
							g_city[city_index].people -= min( global.people_sub_v1, (int)(g_city[city_index].people / (float)max_people * global.people_sub_v2) );
							if ( g_city[city_index].people < 0 )
								g_city[city_index].people = 0;
						}
					}		
				}
				break;
			case BUILDING_Cabinet:
				if ( g_city[city_index].building[tmpi].level >= 2 )
				{
					if ( g_city[city_index].building[tmpi].value <= 0 )
					{
						hero_guard_soldiers_auto( &g_city[city_index] );
						g_city[city_index].building[tmpi].value = 60;
					}
					else
					{
						g_city[city_index].building[tmpi].value -= 1;
					}
				}
				break;
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
					for ( int tmpi = 0; tmpi < g_city[city_index].matquenum+2; tmpi++ )
					{
						if ( g_city[city_index].matkind[tmpi] <= 0 )
							continue;
						int needsec = city_material_needsec( &g_city[city_index], g_city[city_index].matkind[tmpi] );
						g_city[city_index].matsec[tmpi] += 1;
						if ( g_city[city_index].matsec[tmpi] >= needsec )
						{
							city_material_finish( &g_city[city_index], tmpi );
						}
					}
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
			if ( g_city[city_index].guardsec == 0 )
			{
				if ( g_city[city_index].atgu_op == 1 && g_city[city_index].atgu > 0 )
				{
					if ( city_guard_call( city_index ) >= 0 )
					{
						city_change_autoguard( city_index, -1, PATH_SYSTEM );
					}
				}
				else
				{
					city_guard_sendsec( g_city[city_index].actor_index );
				}
			}
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
		for ( int i = 0; i < 6; i++ )
		{
			if ( g_city[city_index].neq_sec[i] > 0 )
			{
				g_city[city_index].neq_sec[i] -= 1;
				if ( g_city[city_index].neq_sec[i] <= 0 )
				{
					// 通知国器完毕
					nation_equip_over( &g_city[city_index], i );
				}
			}
		}

		// 良将免费寻访
		if ( g_city[city_index].hv_fcd > 0 )
		{
			g_city[city_index].hv_fcd -= 1;
			if ( g_city[city_index].hv_fcd <= 0 )
			{ // 可以免费良将寻访了
				hero_visit_snedflag( g_city[city_index].actor_index );
			}
		}

		// 神将寻访持续时间
		if ( g_city[city_index].hv_hsec > 0 )
		{
			g_city[city_index].hv_hsec -= 1;
			if ( g_city[city_index].hv_hsec <= 0 )
			{
				hero_visit_snedflag( g_city[city_index].actor_index );
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

		// buff
		for ( int i = 0; i < CITY_BUFF_MAX; i++ )
		{
			if ( g_city[city_index].buffsec[i] > 0 )
			{
				g_city[city_index].buffsec[i] -= 1;
				if ( g_city[city_index].buffsec[i] == 0 )
				{
					city_change_buff( city_index, i, 0, PATH_SYSTEM );
				}
			}
		}

		map_event_citylogic( city_index );
		// 事件倒计时
		if ( g_city[city_index].eventsec >= 0 )
		{
			g_city[city_index].eventsec -= 1;
			if ( g_city[city_index].eventsec <= 0 )
			{
				g_city[city_index].eventsec = global.mapevent_sec;
				exec_queue_add( EXEC_QUEUE_TYPE_MAPEVENT_CREATE, city_index, g_city[city_index].zone );
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

// 设置城市状态
void city_setstate( City *pCity, char state )
{
	if ( pCity == NULL )
		return;
	pCity->state |= state;
	mapunit_update( MAPUNIT_TYPE_CITY, -1, pCity->unit_index );

	SLK_NetS_CityState pValue = { 0 };
	pValue.m_state = pCity->state;
	pValue.m_change = state;
	netsend_citystate_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
}

// 移除城市状态
void city_delstate( City *pCity, char state )
{
	if ( pCity == NULL )
		return;
	pCity->state &= ~state;
	mapunit_update( MAPUNIT_TYPE_CITY, -1, pCity->unit_index );

	SLK_NetS_CityState pValue = { 0 };
	pValue.m_state = pCity->state;
	pValue.m_change = state;
	netsend_citystate_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
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

// 功能关闭
void city_function_close( City *pCity, int offset )
{
	if ( pCity == NULL )
		return;
	pCity->function &= ~(1 << offset);
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
	g_city[city_index].battle_event[0].m_mailid = mailid;
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

	if ( isup )
	{
		// 自动建造
		building_upgrade_autocheck( city_index );

		if ( g_city[city_index].level == global.hero_visit_actorlevel )
		{
			hero_visit_snedflag( g_city[city_index].actor_index );
		}
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
	// 师徒升级奖励
	actor_studentlevelup( g_city[city_index].teacherid, g_city[city_index].level );
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
	if ( g_city[city_index].silver < 0 )
		g_city[city_index].silver = 0;
	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_Silver pValue = { 0 };
	pValue.m_add = value;
	pValue.m_total = g_city[city_index].silver;
	pValue.m_path = path;
	netsend_changesilver_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );

	if ( value > 0 )
	{
		// 自动建造
		building_upgrade_autocheck( city_index );
	}
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
	if ( g_city[city_index].wood < 0 )
		g_city[city_index].wood = 0;
	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_Wood pValue = { 0 };
	pValue.m_add = value;
	pValue.m_total = g_city[city_index].wood;
	pValue.m_path = path;
	netsend_changewood_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );

	if ( value > 0 )
	{
		// 自动建造
		building_upgrade_autocheck( city_index );
	}
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
	if ( g_city[city_index].food < 0 )
		g_city[city_index].food = 0;
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
	if ( g_city[city_index].iron < 0 )
		g_city[city_index].iron = 0;
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
	if ( g_city[city_index].people < 0 )
		g_city[city_index].people = 0;
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
	if ( g_city[city_index].prestige < 0 )
		g_city[city_index].prestige = 0;
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
	if ( g_city[city_index].friendship < 0 )
		g_city[city_index].friendship = 0;
	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_Friendship pValue = { 0 };
	pValue.m_add = value;
	pValue.m_total = g_city[city_index].friendship;
	pValue.m_path = path;
	netsend_changefriendship_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
// 血战积分
int city_kingwarpoint( int city_index, int value, short path )
{
	CITY_CHECK_INDEX( city_index );
	if ( value > 0 && g_city[city_index].kw_point > INT_MAX - value )
		g_city[city_index].kw_point = INT_MAX;
	else
		g_city[city_index].kw_point += value;
	if ( g_city[city_index].kw_point < 0 )
		g_city[city_index].kw_point = 0;
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
	if ( corps < 0 || corps > 2 )
		return -1;

	if ( value > 0 && g_city[city_index].soldiers[corps] > INT_MAX - value )
		g_city[city_index].soldiers[corps] = INT_MAX;
	else
		g_city[city_index].soldiers[corps] += value;
	if ( g_city[city_index].soldiers[corps] < 0 )
		g_city[city_index].soldiers[corps] = 0;

	SLK_NetS_Soldiers pValue = { 0 };
	pValue.m_corps = (char)corps;
	pValue.m_add = value;
	pValue.m_soldiers = g_city[city_index].soldiers[corps];
	pValue.m_path = path;
	netsend_soldiers_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

int city_autobuild_open( int city_index )
{
	CITY_CHECK_INDEX( city_index );
	if ( g_city[city_index].autobuildopen == 1 )
		g_city[city_index].autobuildopen = 0;
	else
	{
		if ( g_city[city_index].autobuild <= 0 )
			g_city[city_index].autobuildopen = 0;
		else
			g_city[city_index].autobuildopen = 1;
	}

	// 自动建造
	if ( building_upgrade_autocheck( city_index ) < 0 )
		actor_notify_pop( g_city[city_index].actor_index, 2432 );

	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_ChangeAutoBuild pValue = { 0 };
	pValue.m_autobuild = g_city[city_index].autobuild;
	pValue.m_autobuildopen = g_city[city_index].autobuildopen;
	netsend_changeautobuild_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 自动建造次数
int city_change_autobuild( int city_index, int value, short path )
{
	CITY_CHECK_INDEX( city_index );
	if ( value > 0 && g_city[city_index].autobuild > global.autobuild_max - value )
		g_city[city_index].autobuild = global.autobuild_max;
	else
		g_city[city_index].autobuild += value;

	if ( g_city[city_index].autobuild <= 0 )
	{
		g_city[city_index].autobuild = 0;
		g_city[city_index].autobuildopen = 0;
	}
	if ( path == PATH_QUEST && g_city[city_index].autobuild > 0 )
	{
		g_city[city_index].autobuildopen = 0;
	}
	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_ChangeAutoBuild pValue = { 0 };
	pValue.m_autobuild = g_city[city_index].autobuild;
	pValue.m_autobuildopen = g_city[city_index].autobuildopen;
	pValue.m_path = path;
	netsend_changeautobuild_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

int city_autoguard_open( int city_index )
{
	CITY_CHECK_INDEX( city_index );
	if ( g_city[city_index].atgu_op == 1 )
		g_city[city_index].atgu_op = 0;
	else
	{
		if ( g_city[city_index].atgu <= 0 )
			g_city[city_index].atgu_op = 0;
		else
			g_city[city_index].atgu_op = 1;
	}

	// 自动补充
	if ( g_city[city_index].atgu_op == 1 )
	{
		city_guard_call( city_index );
	}

	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_ChangeAutoGuard pValue = { 0 };
	pValue.m_autoguard = g_city[city_index].atgu;
	pValue.m_autoguardopen = g_city[city_index].atgu_op;
	netsend_changeautoguard_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 城防补充次数
int city_change_autoguard( int city_index, int value, short path )
{
	CITY_CHECK_INDEX( city_index );
	if ( value > 0 && g_city[city_index].atgu > global.autoguard_max - value )
		g_city[city_index].atgu = (char)global.autoguard_max;
	else
		g_city[city_index].atgu += value;

	if ( g_city[city_index].atgu <= 0 )
	{
		g_city[city_index].atgu = 0;
		g_city[city_index].atgu_op = 0;
	}
	if ( path == PATH_QUEST && g_city[city_index].atgu > 0 )
	{
		g_city[city_index].atgu_op = 0;
	}
	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_ChangeAutoGuard pValue = { 0 };
	pValue.m_autoguard = g_city[city_index].atgu;
	pValue.m_autoguardopen = g_city[city_index].atgu_op;
	pValue.m_path = path;
	netsend_changeautoguard_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
// 洗髓次数
int city_change_herowash( int city_index, int value, short path )
{
	CITY_CHECK_INDEX( city_index );
	if ( value > 0 && g_city[city_index].hero_washnum > global.hero_wash_max - value )
	{
		g_city[city_index].hero_washnum = global.hero_wash_max;
		g_city[city_index].hero_washsec = global.hero_wash_sec;
	}
	else
		g_city[city_index].hero_washnum += value;

	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );

	int pValue[4] = { 0 };
	pValue[0] = 1;
	pValue[1] = value;
	pValue[2] = g_city[city_index].hero_washnum;
	pValue[3] = path;
	actor_notify_value( g_city[city_index].actor_index, NOTIFY_VALUECHANGE, 4, pValue, NULL );
	return 0;
}
// 洗练次数
int city_change_equipwash( int city_index, int value, short path )
{
	CITY_CHECK_INDEX( city_index );
	if ( value > 0 && g_city[city_index].equip_washnum > global.equip_wash_max - value )
	{
		g_city[city_index].equip_washnum = global.equip_wash_max;
		g_city[city_index].equip_washsec = global.equip_wash_sec;
	}
	else
		g_city[city_index].equip_washnum += value;

	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	int pValue[4] = { 0 };
	pValue[0] = 2;
	pValue[1] = value;
	pValue[2] = g_city[city_index].equip_washnum;
	pValue[3] = path;
	actor_notify_value( g_city[city_index].actor_index, NOTIFY_VALUECHANGE, 4, pValue, NULL );
	return 0;
}

// 选票
int city_changevote( int city_index, int value, short path )
{
	CITY_CHECK_INDEX( city_index );
	if ( value > 0 && g_city[city_index].vote > SHRT_MAX - value )
		g_city[city_index].vote = SHRT_MAX;
	else
		g_city[city_index].vote += value;
	if ( g_city[city_index].vote < 0 )
		g_city[city_index].vote = 0;
	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
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
		actor_notify_pop( g_city[city_index].actor_index, 795 );
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
	g_city[city_index].guardnum = count;
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
	char shape = random( 1, 15 );
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
	g_city[city_index].guardnum += 1;
	// 着火
	if ( g_city[city_index].state | CITY_STATE_FIRE )
	{
		city_delstate( &g_city[city_index], CITY_STATE_FIRE );
	}
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
	unsigned char level = g_city[city_index].guard[offset].level;
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

// 城墙守卫血量变化
int city_guard_subsoldiers( int city_index, int offset, int sub )
{
	if ( sub < 0 )
		return -1;
	if ( offset < 0 || offset >= CITY_GUARD_MAX )
		return -1;

	g_city[city_index].guard[offset].soldiers -= sub;
	if ( g_city[city_index].guard[offset].soldiers <= 0 )
	{ // 死了
		memset( &g_city[city_index].guard[offset], 0, sizeof( CityGuard ) );
		if ( offset < CITY_GUARD_MAX - 1 )
		{
			memmove( &g_city[city_index].guard[offset], &g_city[city_index].guard[offset + 1], sizeof( CityGuard )*(CITY_GUARD_MAX - 1 - offset) );
			memset( &g_city[city_index].guard[CITY_GUARD_MAX-1], 0, sizeof( CityGuard ) );
			g_city[city_index].guardnum -= 1;
		}
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

	if ( g_city[city_index].atgu_op == 1 && g_city[city_index].atgu > 0 )
	{
		if ( city_guard_call( city_index ) >= 0 )
		{
			city_change_autoguard( city_index, -1, PATH_SYSTEM );
		}
	}
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
	pValue.m_guardsec = pCity->guardsec;
	for ( int tmpi = 0; tmpi < CITY_GUARD_MAX; tmpi++ )
	{
		if ( pCity->guard[tmpi].monsterid <= 0 )
			continue;
		city_guard_makestruct( pCity, &pValue.m_list[pValue.m_count], tmpi );
		pValue.m_count += 1;
	}
	pCity->guardnum = (char)pValue.m_count;
	netsend_cityguardlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

int city_guard_sendnum( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int value = pCity->guardnum;
	actor_notify_value( pCity->actor_index, NOTIFY_CITYGUARDNUM, 1, &value, NULL );
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

	int weather_yield = 0;
	int ability = weather_attr_ability();
	int value = weather_attr_value();

	switch ( kind )
	{
	case BUILDING_Silver:
		if ( ability == 1 )
		{
			weather_yield = (int)ceil( base_yield * (value / 100.0f) );
		}
		break;
	case BUILDING_Wood:
		if ( ability == 2 )
		{
			weather_yield = (int)ceil( base_yield * (value / 100.0f) );
		}
		break;
	case BUILDING_Food:
		if ( ability == 3 )
		{
			weather_yield = (int)ceil( base_yield * (value / 100.0f) );
		}
		break;
	case BUILDING_Iron:
		if ( ability == 4 )
		{
			weather_yield = (int)ceil( base_yield * (value / 100.0f) );
		}
		break;
	default:
		break;
	}

	int yield = base_yield + tech_yield + official_yield + weather_yield;
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

	// 任务
	quest_main_addvalue( pCity, QUEST_DATATYPE_LEVYNUM, 0, 0, 1 );
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

	int ability = weather_attr_ability();
	int value = weather_attr_value();
	if ( ability == 1 )
		pValue.m_weather[0] = (int)ceil( pValue.m_base[0] * (value / 100.0f) );
	else if ( ability == 2 )
		pValue.m_weather[1] = (int)ceil( pValue.m_base[1] * (value / 100.0f) );
	else if ( ability == 3 )
		pValue.m_weather[2] = (int)ceil( pValue.m_base[2] * (value / 100.0f) );
	else if ( ability == 4 )
		pValue.m_weather[3] = (int)ceil( pValue.m_base[3] * (value / 100.0f) );

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
	int tech1 = (int)ceil(config->value[0] * pCity->attr.train_per[corps]);
	int tech2 = (int)ceil( config->value[0] * pCity->attr.trainspeed_per );
	int buff = 0;
	if ( pCity->buffsec[CITY_BUFF_TRAIN] > 0 )
	{
		buff = (int)ceil( config->value[0] * (float)(pCity->bufftrain / 100.0f) );
	}

	int weather = 0;
	int ability = weather_attr_ability();
	int value = weather_attr_value();
	if ( ability == 5 )
	{
		weather = (int)ceil( config->value[0] * (float)(value / 100.0f) );
	}
	int total = base + tech1 + tech2 + buff + weather;
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
	
	// 容量要相加
	BuildingBarracks *pOther = NULL;
	if ( barracks->kind == BUILDING_Infantry )
	{
		pOther = buildingbarracks_getptr_kind( pCity->index, BUILDING_Militiaman_Infantry );
	}
	else if ( barracks->kind == BUILDING_Cavalry )
	{
		pOther = buildingbarracks_getptr_kind( pCity->index, BUILDING_Militiaman_Cavalry );
	}
	else if ( barracks->kind == BUILDING_Archer )
	{
		pOther = buildingbarracks_getptr_kind( pCity->index, BUILDING_Militiaman_Archer );
	}
	else if ( barracks->kind == BUILDING_Militiaman_Infantry )
	{
		pOther = buildingbarracks_getptr_kind( pCity->index, BUILDING_Infantry );
	}
	else if ( barracks->kind == BUILDING_Militiaman_Cavalry )
	{
		pOther = buildingbarracks_getptr_kind( pCity->index, BUILDING_Cavalry );
	}
	else if ( barracks->kind == BUILDING_Militiaman_Archer )
	{
		pOther = buildingbarracks_getptr_kind( pCity->index, BUILDING_Archer );
	}

	int other_maxnum = 0;
	if ( pOther )
	{
		BuildingUpgradeConfig *pOtherConfig = building_getconfig( pOther->kind, pOther->level );
		if ( config )
		{
			other_maxnum = pOtherConfig->value[1] + pOther->queue * 3000;
		}
	}

	return maxnum + other_maxnum;
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

	int soldiers = city_soldiers_withkind( pCity->index, kind );
	if ( soldiers >= city_trainsoldiersmax( pCity, barracks ) )
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

	// 任务
	char corps = 0;
	if ( kind == BUILDING_Infantry || kind == BUILDING_Militiaman_Infantry )
		corps = 1;
	else if ( kind == BUILDING_Cavalry || kind == BUILDING_Militiaman_Cavalry )
		corps = 2;
	else if ( kind == BUILDING_Archer || kind == BUILDING_Militiaman_Archer )
		corps = 3;
	quest_addvalue( pCity, QUEST_DATATYPE_TRAIN_OP, corps, 0, trainnum );
	quest_addvalue( pCity, QUEST_DATATYPE_TRAINCOUNT_OP, corps, 0, trainnum );
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
		barracks->queuesec[CITY_TRAINQUEUE_MAX - 1] = 0;
		barracks->queuenum[CITY_TRAINQUEUE_MAX - 1] = 0;
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
				memmove( &barracks->queuesec[tmpi], &barracks->queuesec[tmpi + 1], sizeof( int )*(CITY_TRAINQUEUE_MAX - tmpi - 1) );
				memmove( &barracks->queuenum[tmpi], &barracks->queuenum[tmpi + 1], sizeof( int )*(CITY_TRAINQUEUE_MAX - tmpi - 1) );
				barracks->queuesec[CITY_TRAINQUEUE_MAX - 1] = 0;
				barracks->queuenum[CITY_TRAINQUEUE_MAX - 1] = 0;
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
	char corps = 0;
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
	else
		return -1;

	int overnum = barracks->overnum;
	pCity->soldiers[corps] += barracks->overnum;
	barracks->overnum = 0;
	// 事件
	city_event_add( pCity->index, CITY_EVENT_TRAIN, corps, overnum );
	building_sendinfo_barracks( actor_index, kind );

	SLK_NetS_Soldiers pValue = { 0 };
	pValue.m_corps = corps;
	pValue.m_add = overnum;
	pValue.m_soldiers = pCity->soldiers[corps];
	netsend_soldiers_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );

	wlog( 0, LOGOP_BARRACKS, PATH_TRAIN_GET, kind, overnum, 0, pCity->actorid, city_mainlevel( pCity->index ) );

	// 自动补兵
	hero_addsoldiers_audo( pCity );

	// 任务
	quest_addvalue( pCity, QUEST_DATATYPE_TRAIN, corps + 1, 0, overnum );
	quest_addvalue( pCity, QUEST_DATATYPE_TRAINCOUNT, corps + 1, 0, overnum );
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
	pValue.m_soldiers = city_soldiers_withkind( pCity->index, kind );
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

// 给予招募士兵建筑一个加速
int city_train_awardquick( int city_index, int sec )
{
	CITY_CHECK_INDEX( city_index );
	for ( int kind = BUILDING_Infantry; kind <= BUILDING_Militiaman_Archer; kind++ )
	{
		BuildingBarracks *barracks = buildingbarracks_getptr_kind( city_index, kind );
		if ( !barracks )
			continue;
		if ( barracks->level <= 0 )
			continue;
		if ( barracks->trainsec <= 0 )
			continue;
		if ( barracks->quicksec > 0 )
			continue;
		barracks->quicksec += sec;
		building_sendinfo_barracks( g_city[city_index].actor_index, kind );
		break;
	}
	return 0;
}

// 奖励的加速领取
int city_train_awardquick_get( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( kind >= BUILDING_Infantry && kind <= BUILDING_Militiaman_Archer )
	{ // 募兵加速
		BuildingBarracks *barracks = buildingbarracks_getptr_kind( pCity->index, kind );
		if ( !barracks )
			return -1;
		if ( barracks->level <= 0 )
			return -1;
		if ( barracks->trainsec <= 0 )
			return -1;
		if ( barracks->quicksec <= 0 )
			return -1;
		barracks->trainsec -= barracks->quicksec;
		barracks->quicksec = 0;
		if ( barracks->trainsec <= 0 )
		{
			city_train_finish( pCity, barracks );
		}
		else
		{
			building_sendinfo_barracks( actor_index, kind );
			city_train_sendinfo( actor_index, kind );
		}
	}
	return 0;
}

// 材料制造时间
int city_material_needsec( City *pCity, int itemkind )
{
	if ( !pCity )
		return 0;
	int itemcolor = item_getcolorlevel( itemkind );
	int id = itemcolor - 1;
	if ( id <= 0 || id >= g_material_make_maxnum )
		id = 3;
	int count = 0;
	for ( int tmpi = 0; tmpi < CITY_MATERIALMAKE_MAX; tmpi++ )
	{
		if ( pCity->matkind[tmpi] > 0 )
			count += 1;
	}
	if ( count <= 0 )
		count = 1;
	int people = (pCity->people+nation_people_get(pCity->nation)) / count;
	int needsec = (int)ceil( max( g_material_make[id].sec * global.material_make_value1, g_material_make[id].sec - global.material_make_value2*people ) );
	needsec = (int)ceil( needsec * ( 1.0f - pCity->attr.materialsec_per[0] )*(1.0f - pCity->attr.materialsec_per[1]) );
	return needsec;
}

// 材料生产信息
int city_material_sendinfo( int actor_index, char sendchange )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_MaterialList pValue = { 0 };
	pValue.m_matquenum = pCity->matquenum;
	pValue.m_city_people = pCity->people;
	pValue.m_nation_people = nation_people_get( pCity->nation );
	if ( sendchange == 1 )
	{
		pValue.m_change_city_people = pCity->people - g_actors[actor_index].lastpeople_c;
		pValue.m_change_nation_people = pValue.m_nation_people - g_actors[actor_index].lastpeople_n;

		g_actors[actor_index].lastpeople_c = pCity->people;
		g_actors[actor_index].lastpeople_n = pValue.m_nation_people;
	}
	for ( int tmpi = 0; tmpi < CITY_MATERIALMAKE_MAX; tmpi++ )
	{
		if ( pCity->matkind[tmpi] <= 0 )
			continue;
		pValue.m_list[pValue.m_count].m_matkind = pCity->matkind[tmpi];
		pValue.m_list[pValue.m_count].m_matnum = pCity->matnum[tmpi];
		pValue.m_list[pValue.m_count].m_matsec = pCity->matsec[tmpi];
		pValue.m_list[pValue.m_count].m_matsec_need = city_material_needsec( pCity, pCity->matkind[tmpi] );
		pValue.m_count += 1;
	}
	netsend_materiallist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

int city_material_will_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	int maxqueue = 2 + pCity->matquenum;
	if ( maxqueue > CITY_MATERIALMAKE_MAX )
		maxqueue = CITY_MATERIALMAKE_MAX;

	SLK_NetS_MaterialWillList pValue = { 0 };
	for ( int tmpi = 0; tmpi < maxqueue; tmpi++ )
	{
		pValue.m_list[pValue.m_count].m_matkind = pCity->matkind[tmpi];
		pValue.m_list[pValue.m_count].m_matsec = city_material_needsec( pCity, pCity->matkind[tmpi] ) - pCity->matsec[tmpi];
		pValue.m_list[pValue.m_count].m_matkind_will = pCity->matkind_will[tmpi];
		pValue.m_list[pValue.m_count].m_matnum_will = pCity->matnum_will[tmpi];
		pValue.m_count += 1;
	}
	netsend_materialwilllist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 更新建筑显示信息
int city_material_updatebuilding( City *pCity )
{
	if ( !pCity )
		return -1;

	Building *pBuilding = building_getptr_kind( pCity->index, BUILDING_Craftsman );
	if ( !pBuilding )
		return -1;
	
	pBuilding->value = 0;
	pBuilding->overvalue = 0;
	pBuilding->sec = 0;
	pBuilding->needsec = 0;

	// 先检查是否有完成，有就优先显示完成的
	for ( int tmpi = 0; tmpi < 8; tmpi++ )
	{
		if ( pCity->matkind_over[tmpi] > 0 )
		{
			pBuilding->value = 0;
			pBuilding->overvalue = pCity->matkind_over[tmpi];
			pBuilding->sec = 0;
			pBuilding->needsec = 0;
			return 0;
		}
	}
	
	// 没有完成的，显示制造时间最小的
	int index = -1;
	int minkind = -1;
	int minsec = INT_MAX;
	for ( int tmpi = 0; tmpi < CITY_MATERIALMAKE_MAX; tmpi++ )
	{
		if ( pCity->matkind[tmpi] <= 0 )
			continue;
		int needsec = city_material_needsec( pCity, pCity->matkind[tmpi] );
		int leftsec = needsec - pCity->matsec[tmpi];
		if ( leftsec < minsec )
		{
			minsec = leftsec;
			index = tmpi;
		}
	}

	if ( index >= 0 )
	{
		int needsec = city_material_needsec( pCity, pCity->matkind[index] );
		pBuilding->value = pCity->matkind[index];
		pBuilding->overvalue = 0;
		pBuilding->sec = needsec - pCity->matsec[index];
		pBuilding->needsec = needsec;
	}
	return 0;
}

// 开始生产
int city_material_make( int actor_index, int id, int itemkind, int type )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	if ( id <= 0 || id >= g_material_make_maxnum )
		return-1;

	// 检查材料
	// 检查选择的图纸是否在图纸列表里
	char has = 0;
	for ( int tmpi = 0; tmpi < 6; tmpi++ )
	{
		if ( g_material_make[id].itemkind[tmpi] == itemkind )
		{
			has = 1;
			break;
		}
	}
	if ( has == 0 )
		return -1;

	if ( item_getitemnum( actor_index, itemkind ) <= 0 )
		return -1;
	if ( pCity->silver < g_material_make[id].silver )
		return -1;
	if ( pCity->wood < g_material_make[id].wood )
		return -1;

	int maxqueue = 2 + pCity->matquenum;
	if ( maxqueue > CITY_MATERIALMAKE_MAX )
		maxqueue = CITY_MATERIALMAKE_MAX;

	// 生产
	if ( type == 0 )
	{
		// 找到一个空闲位置
		int index = -1;
		for ( int tmpi = 0; tmpi < maxqueue; tmpi++ )
		{
			if ( pCity->matkind[tmpi] <= 0 )
			{
				index = tmpi;
				break;
			}
		}
		if ( index < 0 )
		{
			return -1;
		}

		// 扣除材料
		item_lost( actor_index, itemkind, 1, PATH_MATERIALMAKE );
		city_changesilver( pCity->index, -g_material_make[id].silver, PATH_MATERIALMAKE );
		city_changewood( pCity->index, -g_material_make[id].wood, PATH_MATERIALMAKE );

		int randtmpi = rand() % 4;
		pCity->matkind[index] = (char)g_material_make[id].materialkind[randtmpi];
		pCity->matnum[index] = random( 1, 5 );
		pCity->matsec[index] = 0; // 此处与别处不用，这是已经生产时间，不是倒计时
		city_material_sendinfo( actor_index, 0 );

		// 更新建筑信息
		city_material_updatebuilding( pCity );
		building_sendinfo( pCity->actor_index, BUILDING_Craftsman );

		// 任务
		quest_addvalue( pCity, QUEST_DATATYPE_MATERIAL_MAKE, 0, 0, 1 );
	}
	// 预定生产
	else
	{
		if ( actor_get_sflag( actor_index, ACTOR_SFLAG_MATERIAL_MAKEWILL ) == 0 )
			return -1;
		// 找到一个空闲位置
		int index = -1;
		for ( int tmpi = 0; tmpi < maxqueue; tmpi++ )
		{
			if ( pCity->matkind_will[tmpi] <= 0 )
			{
				index = tmpi;
				break;
			}
		}
		if ( index < 0 )
		{
			return -1;
		}

		// 扣除材料
		item_lost( actor_index, itemkind, 1, PATH_MATERIALMAKE );
		city_changesilver( pCity->index, -g_material_make[id].silver, PATH_MATERIALMAKE );
		city_changewood( pCity->index, -g_material_make[id].wood, PATH_MATERIALMAKE );

		int randtmpi = rand() % 4;
		pCity->matkind_will[index] = (char)g_material_make[id].materialkind[randtmpi];
		pCity->matnum_will[index] = random( 1, 5 );
		city_material_will_sendinfo( actor_index );
	}
	return 0;
}

// 完成
int city_material_finish( City *pCity, int index )
{
	if ( !pCity )
		return -1;
	if ( index < 0 || index >= CITY_MATERIALMAKE_MAX )
		return -1;
	int kind = pCity->matkind[index];
	pCity->matsec[index] = 0;

	// 找到相同的已经完成的道具，加上数量
	for ( int tmpi = 0; tmpi < 8; tmpi++ )
	{
		if ( pCity->matkind_over[tmpi] == pCity->matkind[index] )
		{
			pCity->matnum_over[tmpi] += pCity->matnum[index];
			pCity->matkind[index] = 0;
			pCity->matnum[index] = 0;
			break;
		}
	}
	
	// 没有相同道具，找空位置
	if ( pCity->matkind[index] > 0 )
	{
		for ( int tmpi = 0; tmpi < 8; tmpi++ )
		{
			if ( pCity->matkind_over[tmpi] <= 0 )
			{
				pCity->matkind_over[tmpi] = pCity->matkind[index];
				pCity->matnum_over[tmpi] = pCity->matnum[index];
				pCity->matkind[index] = 0;
				pCity->matnum[index] = 0;
				break;
			}
		}
	}

	// 预定生产队列
	if ( pCity->matkind_will[index] > 0 )
	{
		pCity->matkind[index] = pCity->matkind_will[index];
		pCity->matnum[index] = pCity->matnum_will[index];
		pCity->matkind_will[index] = 0;
		pCity->matnum_will[index] = 0;
	}
	city_material_sendinfo( pCity->actor_index, 0 );

	// 更新建筑信息
	city_material_updatebuilding( pCity );
	building_sendinfo( pCity->actor_index, BUILDING_Craftsman );
	return 0;
}

// 领取
int city_material_get( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	for ( int tmpi = 0; tmpi < 8; tmpi++ )
	{
		if ( pCity->matkind_over[tmpi] > 0 )
		{
			item_getitem( actor_index, pCity->matkind_over[tmpi], pCity->matnum_over[tmpi], -1, PATH_MATERIALMAKE );
			pCity->matkind_over[tmpi] = 0;
			pCity->matnum_over[tmpi] = 0;
			break; // (一个一个领取)
		}
	}
	// 更新建筑信息
	city_material_updatebuilding( pCity );
	building_sendinfo( actor_index, BUILDING_Craftsman );
	return 0;
}

// 购买队列
static int s_material_queue_token[] = { 500, 2000, 10000, 10000, 0, 0, 0, 0, 0 };
int city_material_buyqueue( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->matquenum < 0 || pCity->matquenum >= CITY_MATERIALMAKE_MAX - 2 )
		return -1;
	if ( actor_change_token( actor_index, -s_material_queue_token[pCity->matquenum], PATH_MATERIALMAKE_QUEUE, 0 ) < 0 )
		return -1;
	pCity->matquenum += 1;
	city_material_sendinfo( actor_index, 0 );
	return 0;
}

int city_material_gm( City *pCity, int sec )
{
	if ( !pCity )
		return -1;
	for ( int tmpi = 0; tmpi < CITY_MATERIALMAKE_MAX; tmpi++ )
	{
		if ( pCity->matkind[tmpi] <= 0 )
			continue;
		int needsec = city_material_needsec( pCity, pCity->matkind[tmpi] );
		pCity->matsec[tmpi] += sec;
		if ( pCity->matsec[tmpi] >= needsec )
			pCity->matsec[tmpi] = needsec-5;
	}
	city_material_sendinfo( pCity->actor_index, 0 );

	// 更新建筑信息
	city_material_updatebuilding( pCity );
	building_sendinfo( pCity->actor_index, BUILDING_Craftsman );
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
		pCity->ofsec[type] += g_official_forging[kind].duration; // 时间叠加
		pCity->ofquick[type] = 0;

		// 任务
		quest_addvalue( pCity, QUEST_DATATYPE_SMITHYOFFHIRE, 0, 0, kind );
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
				if ( actor_change_token( actor_index, -g_official_gov[kind].token, PATH_HIRE_GOV, 0 ) < 0 )
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

		// 七日狂欢
		if ( g_official_gov[kind].token > 0 )
		{
			activity_04_addvalue( pCity->index, ACTIVITY_SEVENDAY_TYPE4, 1 );
		}
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
		if ( g_official_tech[kind].vip > 0 )
		{
			if ( actor_get_sflag( actor_index, ACTOR_SFLAG_OFFICIAL_TECH ) == 0 )
			{
				actor_notify_alert( actor_index, 2321 );
				return -1;
			}
		}
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

		// 七日狂欢
		if ( g_official_tech[kind].token > 0 )
		{
			activity_04_addvalue( pCity->index, ACTIVITY_SEVENDAY_TYPE3, 1 );
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
	int total = 0;
	if ( corps == 0 )
		total = g_city[city_index].soldiers[0];
	else if ( corps == 1 )
		total = g_city[city_index].soldiers[1];
	else if ( corps == 2 )
		total = g_city[city_index].soldiers[2];
	return total;
}
int city_soldiers_withkind( int city_index, short kind )
{
	CITY_CHECK_INDEX( city_index );
	int total = 0;
	if ( kind == BUILDING_Infantry || kind == BUILDING_Militiaman_Infantry )
		total = g_city[city_index].soldiers[0];
	else if ( kind == BUILDING_Cavalry || kind == BUILDING_Militiaman_Cavalry )
		total = g_city[city_index].soldiers[1];
	else if ( kind == BUILDING_Archer || kind == BUILDING_Militiaman_Archer )
		total = g_city[city_index].soldiers[2];
	return total;
}

// 加入出征队列
int city_battlequeue_add( City *pCity, int army_index )
{
	if ( pCity == NULL )
		return -1;
	for ( int tmpi = 0; tmpi < CITY_BATTLEQUEUE_MAX; tmpi++ )
	{
		if ( pCity->battle_armyindex[tmpi] < 0 )
		{
			pCity->battle_armyindex[tmpi] = army_index;
			break;
		}
	}

	// 是否跳转过去
	int unit_index = -1;
	/*if ( g_army[army_index].target_type == MAPUNIT_TYPE_CITY || g_army[army_index].target_type == MAPUNIT_TYPE_ARMY )
	{
	unit_index = g_army[army_index].unit_index;
	}*/
	city_battlequeue_sendlist( pCity->actor_index, unit_index );
	return 0;
}

// 删除出征队列
int city_battlequeue_del( City *pCity, int army_index )
{
	if ( pCity == NULL )
		return -1;
	for ( int tmpi = 0; tmpi < CITY_BATTLEQUEUE_MAX; tmpi++ )
	{
		if ( pCity->battle_armyindex[tmpi] == army_index )
		{
			pCity->battle_armyindex[tmpi] = -1;
			break;
		}
	}
	city_battlequeue_sendlist( pCity->actor_index, -1 );
	return 0;
}

void city_battlequeue_makestruct( SLK_NetS_BattleInfo *pValue, int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	pValue->m_army_index = army_index;
	pValue->m_action = g_army[army_index].action;
	pValue->m_state = g_army[army_index].state;
	pValue->m_statetime = g_army[army_index].statetime;
	pValue->m_stateduration = g_army[army_index].stateduration;
	pValue->m_to_posx = g_army[army_index].to_posx;
	pValue->m_to_posy = g_army[army_index].to_posy;
	pValue->m_to_type = g_army[army_index].to_type;
	pValue->m_unit_index = g_army[army_index].unit_index;
	for ( int i = 0; i < ARMY_HERO_MAX; i++ )
	{
		pValue->m_herokind[i] = g_army[army_index].herokind[i];
	}
	if ( g_army[army_index].state == ARMY_STATE_GATHER && g_army[army_index].to_type == MAPUNIT_TYPE_RES )
	{ // 采集中，使用资源的unit_index
		pValue->m_unit_index = map_res_getunit( g_army[army_index].to_index );
	}
	else if ( g_army[army_index].state == ARMY_STATE_HELP && g_army[army_index].to_type == MAPUNIT_TYPE_CITY )
	{ // 援助中，使用盟友城池的unit_index
		City *pTargetCity = army_getcityptr_target( army_index );
		if ( !pTargetCity )
			pValue->m_unit_index = -1;
		else
			pValue->m_unit_index = pTargetCity->unit_index;
	}
	else if ( g_army[army_index].state == ARMY_STATE_GROUP_END )
	{ // 集结完毕
		pValue->m_statetime = armygroup_statetime( g_army[army_index].group_index );
		pValue->m_stateduration = armygroup_stateduration( g_army[army_index].group_index );
	}
	else if ( g_army[army_index].state >= ARMY_STATE_KINGWAR_READY && g_army[army_index].state <= ARMY_STATE_KINGWAR_DEAD )
	{ // 血战等待战斗
		pValue->m_to_posx = g_army[army_index].to_id;
		pValue->m_to_posy = g_army[army_index].move_total_distance;
		pValue->m_to_type = g_army[army_index].appdata;
		pValue->m_unit_index = g_army[army_index].totals;
	}
	else
	{
		pValue->m_unit_index = g_army[army_index].unit_index;
	}
}

// 出征队列
void city_battlequeue_sendlist( int actor_index, int unit_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return;
	City *pCity = city_getptr( actor_index );
	if ( pCity == NULL )
		return;
	SLK_NetS_BattleList info = { 0 };
	info.m_unit_index = unit_index;
	for ( int tmpi = 0; tmpi < CITY_BATTLEQUEUE_MAX; tmpi++ )
	{
		int army_index = pCity->battle_armyindex[tmpi];
		if ( army_index < 0 || army_index >= g_army_maxcount )
			continue;
		city_battlequeue_makestruct( &info.m_list[info.m_count], army_index );
		info.m_count += 1;
	}
	netsend_battlelist_S( actor_index, SENDTYPE_ACTOR, &info );
}

// 城市出征队列
void city_battlequeue_sendupdate( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;
	City *pCity = army_getcityptr( army_index );
	if ( !pCity )
		return;
	if ( pCity->actor_index < 0 )
		return;
	SLK_NetS_BattleInfo pValue = {};
	city_battlequeue_makestruct( &pValue, army_index );
	netsend_battleinfo_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
}

// 空闲的被攻击列表索引
static int underfire_freeindex( City *pCity )
{
	if ( pCity == NULL )
		return -1;
	for ( int tmpi = 0; tmpi < CITY_UNDERFIRE_MAX; tmpi++ )
	{
		if ( pCity->underfire_armyindex[tmpi] < 0 )
			return tmpi;
	}
	return -1;
}

// 被攻击信息添加
int city_underfire_add( City *pCity, int army_index )
{
	if ( pCity == NULL )
		return -1;
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	if ( g_army[army_index].state == ARMY_STATE_FIGHT )
		return -1;
	int index = underfire_freeindex( pCity );
	if ( index < 0 )
		return -1;
	pCity->underfire_armyindex[index] = army_index;
	// 通知该玩家
	if ( pCity->actor_index >= 0 && pCity->actor_index < g_maxactornum )
	{
		// 军情数量
		//city_alarm_sendnum( pCity );

		// 通知自己的界面更新
		//actor_dialogupdate( pCity->actor_index, 4 );
	}
	else
	{
		// 推送
		//char ext[64] = { 0 };
		//city_getprefixname( army_getcityptr( army_index ), ext );
		//pushwork_addpush( PUSHACTION_MARCH, pCity, ext );
	}
	return 0;
}

// 被攻击信息移除
int city_underfire_del( City *pCity, int army_index )
{
	if ( pCity == NULL )
		return -1;
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	int tmpi = 0;
	for ( tmpi = 0; tmpi < CITY_UNDERFIRE_MAX; tmpi++ )
	{
		if ( pCity->underfire_armyindex[tmpi] == army_index )
		{
			pCity->underfire_armyindex[tmpi] = -1;
			break;
		}
	}

	if ( tmpi >= CITY_UNDERFIRE_MAX )
		return -1;

	// 通知该玩家
	if ( pCity->actor_index >= 0 && pCity->actor_index < g_maxactornum )
	{
		// 军情数量
		//city_alarm_sendnum( pCity );

		// 通知自己的界面更新
		//actor_dialogupdate( pCity->actor_index, 4 );
	}
	return 0;
}

// 被攻击信息移除
int city_underfire_del_equal( City *pCity, int equal_army_index )
{
	if ( pCity == NULL )
		return -1;
	if ( equal_army_index < 0 || equal_army_index >= g_army_maxcount )
		return -1;
	int change = 0;
	int tmpi = 0;
	for ( tmpi = 0; tmpi < CITY_UNDERFIRE_MAX; tmpi++ )
	{
		int army_index = pCity->underfire_armyindex[tmpi];
		if ( army_index < 0 || army_index >= g_army_maxcount )
			continue;
		if ( g_army[army_index].to_type == MAPUNIT_TYPE_ARMY )
		{
			if ( g_army[army_index].to_index == equal_army_index )
			{
				pCity->underfire_armyindex[tmpi] = -1;
				change = 1;
			}
		}
		else if ( g_army[army_index].to_type == MAPUNIT_TYPE_RES )
		{
			if ( map_res_getarmy( g_army[army_index].to_index ) == equal_army_index )
			{
				pCity->underfire_armyindex[tmpi] = -1;
				change = 1;
			}
		}
	}

	if ( change == 0 )
		return -1;

	// 通知该国王
	if ( pCity->actor_index >= 0 && pCity->actor_index < g_maxactornum )
	{
		// 军情数量
		//city_alarm_sendnum( pCity );

		// 通知自己的界面更新
		//actor_dialogupdate( pCity->actor_index, 4 );
	}
	return 0;
}

// 被攻击信息数量
int city_underfire_getnum( City *pCity )
{
	if ( pCity == NULL )
		return 0;
	int num = 0;
	for ( int tmpi = 0; tmpi < CITY_UNDERFIRE_MAX; tmpi++ )
	{
		if ( pCity->underfire_armyindex[tmpi] >= 0 )
		{
			num += 1;
		}
	}
	return num;
}

// 空闲的被攻击列表索引
static int underfire_group_freeindex( City *pCity )
{
	if ( pCity == NULL )
		return -1;
	for ( int tmpi = 0; tmpi < CITY_UNDERFIRE_GROUP_MAX; tmpi++ )
	{
		if ( pCity->underfire_groupindex[tmpi] < 0 )
			return tmpi;
	}
	return -1;
}

// 被攻击信息添加
int city_underfire_groupadd( City *pCity, int group_index )
{
	if ( pCity == NULL )
		return -1;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	int index = underfire_group_freeindex( pCity );
	if ( index < 0 )
		return -1;
	pCity->underfire_groupindex[index] = group_index;

	// 通知该玩家
	if ( pCity->actor_index >= 0 && pCity->actor_index < g_maxactornum )
	{
		SLK_NetS_CityWarInfo pValue = { 0 };
		city_underfire_makestruct( group_index, &pValue );
		netsend_citywarinfo_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
	}
	else
	{ // 推送
	}
	return 0;
}

// 被攻击信息移除
int city_underfire_groupdel( City *pCity, int group_index )
{
	if ( pCity == NULL )
		return -1;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	int tmpi = 0;
	for ( tmpi = 0; tmpi < CITY_UNDERFIRE_GROUP_MAX; tmpi++ )
	{
		if ( pCity->underfire_groupindex[tmpi] == group_index )
		{
			pCity->underfire_groupindex[tmpi] = -1;
			break;
		}
	}

	// 城战状态
	char has = 0;
	for ( int i = 0; i < CITY_UNDERFIRE_GROUP_MAX; i++ )
	{
		if ( pCity->underfire_groupindex[i] >= 0 )
		{
			has = 1;
			break;
		}
	}
	if ( has == 0 )
	{
		city_delstate( pCity, CITY_STATE_ARMYGROUP );
	}

	if ( tmpi >= CITY_UNDERFIRE_GROUP_MAX )
		return -1;

	// 通知该玩家
	if ( pCity->actor_index >= 0 && pCity->actor_index < g_maxactornum )
	{
		SLK_NetS_CityWarDel pValue = { 0 };
		pValue.m_group_index = group_index;
		netsend_citywardel_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
	}
	return 0;
}

// 军情列表
void city_underfire_makestruct( int group_index, SLK_NetS_CityWarInfo *pValue )
{
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return;
	pValue->m_group_index = group_index;
	pValue->m_group_id = g_armygroup[group_index].id;
	pValue->m_statetime = g_armygroup[group_index].statetime;
	pValue->m_stateduration = g_armygroup[group_index].stateduration;
	pValue->m_from_nation = g_armygroup[group_index].from_nation;
	pValue->m_from_posx = g_armygroup[group_index].from_posx;
	pValue->m_from_posy = g_armygroup[group_index].from_posy;
	if ( g_armygroup[group_index].from_type == MAPUNIT_TYPE_CITY )
	{
		City *pTmpCity = city_indexptr( g_armygroup[group_index].from_index );
		if ( pTmpCity )
		{
			strncpy( pValue->m_name, pTmpCity->name, NAME_SIZE );
			pValue->m_namelen = strlen( pValue->m_name );
		}
	}
}
int city_underfire_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_CityWarList pValue = { 0 };
	for ( int tmpi = 0; tmpi < CITY_UNDERFIRE_GROUP_MAX; tmpi++ )
	{
		int group_index = pCity->underfire_groupindex[tmpi];
		if ( group_index < 0 || group_index >= g_armygroup_maxcount )
			continue;
		city_underfire_makestruct( group_index, &pValue.m_list[pValue.m_count] );
		pValue.m_count += 1;
	}
	netsend_citywarlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 添加驻防部队
int city_helparmy_add( City *pCity, int army_index )
{
	if ( pCity == NULL )
		return -1;
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	for ( int index = 0; index < CITY_HELPDEFENSE_MAX; index++ )
	{
		if ( pCity->help_armyindex[index] < 0 )
		{
			pCity->help_armyindex[index] = army_index;

			// 给集结 发送援助部队信息
			//armygroup_send_addinfo( army_index, 1 );
			// 通知自己的界面更新
			//actor_dialogupdate( pCity->actor_index, 3 );
			break;
		}
	}
	return 0;
}

// 删除驻防部队
int city_helparmy_del( City *pCity, int army_index )
{
	if ( pCity == NULL )
		return -1;
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	for ( int index = 0; index < CITY_HELPDEFENSE_MAX; index++ )
	{
		if ( pCity->help_armyindex[index] == army_index )
		{
			pCity->help_armyindex[index] = -1;

			// 数据前移
			if ( index < CITY_HELPDEFENSE_MAX - 1 )
			{
				memmove( &pCity->help_armyindex[index], &pCity->help_armyindex[index + 1], sizeof( int )*(CITY_HELPDEFENSE_MAX - 1 - index) );
				pCity->help_armyindex[CITY_HELPDEFENSE_MAX - 1] = -1;
			}
			// 给集结 发送援助删除信息
			//armygroup_send_delinfo( army_index, 1 );
			// 通知自己的界面更新
			//actor_dialogupdate( pCity->actor_index, 3 );
			break;
		}
	}
	return 0;
}

// 获取驻防部队数量
int city_helparmy_getnum( City *pCity )
{
	if ( !pCity )
		return 0;
	int num = 0;
	for ( int index = 0; index < CITY_HELPDEFENSE_MAX; index++ )
	{
		if ( pCity->help_armyindex[index] >= 0 )
		{
			num += 1;
		}
	}
	return num;
}

// 获取驻防部队数量上限
int city_helparmy_maxnum( City *pCity )
{
	if ( !pCity )
		return 0;
	Building *pBuilding = building_getptr_kind( pCity->index, BUILDING_Wall );
	if ( !pBuilding )
		return 0;
	return pBuilding->level;
}

void city_helparmy_makestruct( int army_index, SLK_NetS_CItyHelp *pValue )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return;

	City *pCity = army_getcityptr( army_index );
	if ( !pCity )
		return;
	pValue->m_army_index = army_index;
	pValue->m_actorid = pCity->actorid;
	pValue->m_level = pCity->level;
	strncpy( pValue->m_actorname, pCity->name, NAME_SIZE );
	pValue->m_actorname_length = strlen( pValue->m_actorname );
	pValue->m_herokind = g_army[army_index].herokind[0];
	int hero_index = city_hero_getindex( pCity->index, g_army[army_index].herokind[0] );
	if ( hero_index >= 0 && hero_index < HERO_CITY_MAX )
	{
		pValue->m_soldiers = pCity->hero[hero_index].soldiers;
	}
}

// 获取驻防部信息
int city_helparmy_sendlist( int actor_index, int unit_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
	{// 发送自己城墙的
		Building *pBuilding = building_getptr_kind( pCity->index, BUILDING_Wall );
		if ( !pBuilding )
			return -1;
		SLK_NetS_CItyHelpList pValue = { 0 };
		pValue.m_walllevel = pBuilding->level;
		for ( int index = 0; index < CITY_HELPDEFENSE_MAX; index++ )
		{
			if ( pCity->help_armyindex[index] >= 0 )
			{
				city_helparmy_makestruct( pCity->help_armyindex[index], &pValue.m_list[pValue.m_count] );
				pValue.m_count += 1;
				if ( pValue.m_count >= 30 )
				{
					netsend_cityhelplist_S( actor_index, SENDTYPE_ACTOR, &pValue );
					pValue.m_count = 0;
				}
			}
		}
		netsend_cityhelplist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}
	else
	{ // 别人的
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
		Building *pBuilding = building_getptr_kind( pTargetCity->index, BUILDING_Wall );
		if ( !pBuilding )
			return -1;
		SLK_NetS_MapCItyHelpList pValue = { 0 };
		pValue.m_walllevel = pBuilding->level;
		for ( int index = 0; index < CITY_HELPDEFENSE_MAX; index++ )
		{
			if ( pTargetCity->help_armyindex[index] >= 0 )
			{
				city_helparmy_makestruct( pTargetCity->help_armyindex[index], &pValue.m_list[pValue.m_count] );
				pValue.m_count += 1;
				if ( pValue.m_count >= 30 )
				{
					netsend_mapcityhelplist_S( actor_index, SENDTYPE_ACTOR, &pValue );
					pValue.m_count = 0;
				}
			}
		}
		netsend_mapcityhelplist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}
	return 0;
}

// 迁城
int city_move_actor( int actor_index, short posx, short posy, int itemkind )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	// 武将有出征的
	for ( int tmpi = 0; tmpi < HERO_CITY_MAX; tmpi++ )
	{
		if ( pCity->hero[tmpi].state > 0 )
		{
			actor_notify_pop( pCity->actor_index, 1001 ); // 召回在外征战的武将
			return -1;
		}
	}
	// 当前的地区
	char cur_zoneid = map_zone_getid( pCity->posx, pCity->posy );
	char boss1_complete = 0;
	char boss2_complete = 0;
	if ( worldquest_check( actor_index, WORLDQUEST_WORLDBOSS1, NULL ) == 1 )
	{ // 如果已经完成了世界任务张角
		boss1_complete = 1;
	}
	if ( worldquest_check( actor_index, WORLDQUEST_WORLDBOSS2, NULL ) == 1 )
	{ // 如果已经完成了世界任务董卓
		boss2_complete = 1;
	}

	short move_posx = -1;
	short move_posy = -1;
	if ( itemkind == 131 )
	{ // 已经解锁的地图纯随机
		short zoneidlist[32] = { 0 };
		short zoneidcount = 0;
		if ( boss2_complete == 1 )
		{ // 全部地区已经解锁
			for ( short tmp_zoneid = 1; tmp_zoneid < g_zoneinfo_maxnum; tmp_zoneid++ )
			{
				if ( g_zoneinfo[tmp_zoneid].open == 0 )
					continue;
				if ( pCity->level < g_zoneinfo[tmp_zoneid].actorlevel )
					continue;
				if ( pCity->mokilllv < g_zoneinfo[tmp_zoneid].killenemy )
					continue;
				zoneidlist[zoneidcount] = tmp_zoneid;
				zoneidcount += 1;
			}
		}
		else if ( boss1_complete == 1 )
		{ // 部分指定地图已经解锁
			for ( short tmpi = 0; tmpi < 2; tmpi++ )
			{
				int tmp_zoneid = g_zoneinfo[cur_zoneid].move_zoneid[tmpi];
				if ( tmp_zoneid <= 0 || tmp_zoneid >= g_zoneinfo_maxnum )
					continue;
				if ( pCity->level < g_zoneinfo[tmp_zoneid].actorlevel )
					continue;
				if ( pCity->mokilllv < g_zoneinfo[tmp_zoneid].killenemy )
					continue;
				zoneidlist[zoneidcount] = tmp_zoneid;
				zoneidcount += 1;
			}
		}
		if ( zoneidcount <= 0 )
		{
			return -1;
		}
		short index = rand() % zoneidcount;
		short zoneid = zoneidlist[index];
		// 如果玩家在皇城区域，有85%的概率会随机迁移到皇城区域的该国的领土内。5%几率迁移到其他国家的领土（包括群雄）。10%的几率飞到皇城以外的区域
		if ( cur_zoneid == MAPZONE_CENTERID )
		{
			int odds = rand() % 100;
			if ( odds < 85 )
			{
				map_zone_nation_randpos( pCity->nation, &move_posx, &move_posy, 4096 );
			}
			else if ( odds < 90 )
			{
				map_zone_randpos( MAPZONE_CENTERID, &move_posx, &move_posy, 4096 );
			}
			else
			{
				map_zone_randpos( zoneid, &move_posx, &move_posy, 4096 );
			}
		}
		else
		{ // 不在皇城，暂时随机
			map_zone_randpos( zoneid, &move_posx, &move_posy, 4096 );
		}
		if ( map_canmove( move_posx, move_posy ) == 0 )
			return -1;
	}
	else if ( itemkind == 132 || itemkind == 133 )
	{ // 选择指定解锁地图随机 || 选择指定解锁地图指定坐标点
		char zoneid = map_zone_getid( posx, posy );
		if ( zoneid <= 0 || zoneid >= g_zoneinfo_maxnum )
			return -1;
		if ( g_zoneinfo[zoneid].open == 0 )
		{
			actor_notify_alert( actor_index, 1365 );
			return -1;
		}
		if ( boss2_complete == 1 )
		{ // 全部地区已经解锁
			
		}
		else if ( boss1_complete == 1 )
		{ // 部分地区已经解锁
			if ( g_zoneinfo[cur_zoneid].move_zoneid[0] != zoneid && g_zoneinfo[cur_zoneid].move_zoneid[1] != zoneid )
			{ // 需要击败世界boss董卓后才可前往该地图
				actor_notify_alert( actor_index, 1368 );
				return -1;
			}
		}
		else
		{
			if ( zoneid == cur_zoneid )
			{

			}
			else if ( g_zoneinfo[cur_zoneid].move_zoneid[0] != zoneid && g_zoneinfo[cur_zoneid].move_zoneid[1] != zoneid )
			{ // 需要击败世界boss董卓后才可前往该地图
				actor_notify_alert( actor_index, 1368 );
				return -1;
			}
			else
			{ // 需要击败世界boss张角后才可前往该地图
				actor_notify_alert( actor_index, 1378 );
				return -1;
			}
		}

		if ( pCity->level < g_zoneinfo[zoneid].actorlevel )
		{ // 需要玩家等级{0}才可迁移到该地图
			char v1[32] = { 0 };
			sprintf( v1, "%d", g_zoneinfo[zoneid].actorlevel );
			actor_notify_alert_v( actor_index, 1366, v1, NULL );
			return -1;
		}
		if ( pCity->mokilllv < g_zoneinfo[zoneid].killenemy )
		{ // 需要击败{ 0 }级流寇才可迁移到该地图
			char v1[32] = { 0 };
			sprintf( v1, "%d", g_zoneinfo[zoneid].killenemy );
			actor_notify_alert_v( actor_index, 1367, v1, NULL );
			return -1;
		}

		if ( itemkind == 132 )
		{ // 选择指定解锁地图随机
			if ( zoneid == MAPZONE_CENTERID )
			{
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
				if ( map_canmove( move_posx, move_posy ) == 0 )
					return -1;
				return -1;
			}
		}
		else
		{ // 选择指定解锁地图指定坐标点
			if ( posx < 0 || posy < 0 || posx >= g_map.m_nMaxWidth || posy >= g_map.m_nMaxWidth )
				return 1;
			if ( g_map.m_aTileData[posx][posy].unit_type == MAPUNIT_TYPE_EVENT )
			{ // 如果坐标点是别人的随机事件，对方随机事件改变位置
				if ( map_event_changepos_rand( g_map.m_aTileData[posx][posy].unit_index ) < 0 )
					return -1;
			}
			else if ( g_map.m_aTileData[posx][posy].unit_type > 0 )
			{
				return -1;
			}
			move_posx = posx;
			move_posy = posy;
		}
	}
	else
		return -1;

	if ( item_lost( actor_index, itemkind, 1, PATH_ITEMUSE ) < 0 )
	{	// 没有道具，那么使用钻石
		int price = item_gettoken( itemkind );
		if ( actor_change_token( actor_index, -price, PATH_ITEMUSE, itemkind ) == -2 )
		{
			return -1;
		}
	}

	return city_move( pCity, move_posx, move_posy );
}
int city_move( City *pCity, short posx, short posy )
{
	if ( !pCity )
		return -1;
	if ( map_canmove( posx, posy ) == 0 )
		return -1;
	short lastposx = pCity->posx;
	short lastposy = pCity->posy;
	map_delobject( MAPUNIT_TYPE_CITY, pCity->index, lastposx, lastposy );
	zoneunit_del( MAPUNIT_TYPE_CITY, pCity->index, pCity->zoneunit_index );
	pCity->zoneunit_index = -1;
	pCity->posx = posx;
	pCity->posy = posy;

	// 通知区域
	if ( pCity->unit_index < 0 )
	{
		pCity->unit_index = mapunit_add( MAPUNIT_TYPE_CITY, pCity->index );
	}
	
	map_addobject( MAPUNIT_TYPE_CITY, pCity->index, pCity->posx, pCity->posy );
	pCity->zone = map_zone_getid( pCity->posx, pCity->posy );

	mapunit_area_change( pCity->unit_index, pCity->posx, pCity->posy, 1 );
	city_attr_reset( pCity );
	
	// 通知地区
	if ( pCity->zoneunit_index < 0 )
	{
		pCity->zoneunit_index = zoneunit_add( MAPUNIT_TYPE_CITY, pCity->index );
	}

	// 我的随机事件也要更新位置
	for ( int tmpi = 0; tmpi < CITY_MAPEVENT_MAX; tmpi++ )
	{
		if ( pCity->mapevent_index[tmpi] < 0 )
			continue;
		map_event_changepos_randhaspos( pCity->mapevent_index[tmpi], pCity->posx, pCity->posy );
	}

	if ( pCity->actor_index >= 0 && pCity->actor_index < g_maxactornum )
	{
		if ( map_zone_gettype( pCity->zone ) == MAPZONE_TYPE1 )
		{
			if ( actor_get_sflag( pCity->actor_index, ACTOR_SFLAG_MAPZONE_GO_ZC ) == 0 )
			{
				actor_set_sflag( pCity->actor_index, ACTOR_SFLAG_MAPZONE_GO_ZC, 1 );
				// 隐藏前往州城按钮
				int value = 0;
				actor_notify_value( pCity->actor_index, NOTIFY_MAPZONEGOZC, 1, &value, NULL );
			}
		}

		// 通知客户端更新缓存
		int value[7] = { 0 };
		value[0] = 1;
		value[1] = pCity->unit_index;
		value[2] = pCity->posx;
		value[3] = pCity->posy;
		value[4] = lastposx;
		value[5] = lastposy;
		value[6] = pCity->zone;
		actor_notify_value( pCity->actor_index, NOTIFY_WORLDMAP, 7, value, NULL );
	}
	return 0;
}

// 侦察
int city_spy( int actor_index, int unit_index, int type )
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

	// 侦察科技等级
	int techlevel = pCity->techlevel[19];
	if ( techlevel <= 0 )
		return -1;

	// 侦察方主城等级
	int mainlevel = city_mainlevel( pCity->index );
	if ( mainlevel <= 0 || mainlevel >= g_building_upgrade[BUILDING_Main].maxnum )
		return -1;
	// 被侦察方主城等级
	int target_mainlevel = city_mainlevel( pTargetCity->index );
	if ( target_mainlevel <= 0 || target_mainlevel >= g_building_upgrade[BUILDING_Main].maxnum )
		return -1;

	// 此处注意：侦察类型决定，到底是增加科技等级还是概率，案子上和表现上不同
	int typeodds = 0;
	if ( type == 1 )
	{ // 初级侦察
		int silver = g_building_upgrade[BUILDING_Main].config[mainlevel].value[3];
		if ( pCity->silver < silver )
			return -1;
		city_changesilver( pCity->index, -silver, PATH_SPY );
		//techlevel += global.spy_add_techlevel1;
	}
	else if ( type == 2 )
	{ // 中级侦察
		int silver = g_building_upgrade[BUILDING_Main].config[mainlevel].value[4];
		if ( pCity->silver < silver )
			return -1;
		city_changesilver( pCity->index, -silver, PATH_SPY );
		//techlevel += global.spy_add_techlevel2;
		typeodds = 20;
	}
	else if ( type == 3 )
	{ // 高级侦察
		int token = global.spy_token;
		if ( actor_change_token( actor_index, -token, PATH_SPY, 0 ) < 0 )
			return -1;
		//techlevel += global.spy_add_techlevel3;
		typeodds = 50;
	}
	else
	{
		return -1;
	}

	if ( techlevel >= g_techinfo[19].maxnum )
	{
		techlevel = g_techinfo[19].maxnum - 1;
	}

	char title[MAIL_TITLE_MAXSIZE] = { 0 };
	char content[MAIL_CONTENT_MAXSIZE] = { 0 };

	char be_title[MAIL_TITLE_MAXSIZE] = { 0 };
	char be_content[MAIL_CONTENT_MAXSIZE] = { 0 };

	char res_success = 0;
	char wall_success = 0;
	char hero_success = 0;

	// 最终侦查成功概率 = 初始侦查成功概率 + 侦查方式增加概率 + 科技增加概率 + （被侦查方主城等级 - 侦查方主城等级） * 5% 
	int baseodds = typeodds + g_techinfo[19].config[techlevel].value + (target_mainlevel - mainlevel) * 5;
	if ( baseodds < 0 )
		baseodds = 0;
	// 1侦察资源情况
	int odds = global.spy_res_odds + baseodds;
	if ( rand() % 100 <= odds )
	{ 
		res_success = 1;
	}

	// 2侦察城墙兵力	
	if ( res_success == 1 )
	{
		odds = global.spy_wall_odds + baseodds;
		if ( rand() % 100 <= odds )
		{
			wall_success = 1;
		}
	}

	// 3侦察驻守武将
	if ( wall_success == 1 )
	{
		odds = global.spy_hero_odds + baseodds;
		if ( rand() % 100 <= odds )
		{
			hero_success = 1;
		}
	}


	if ( res_success == 1 )
	{// 侦察成功
		sprintf( title, "%s%d", TAG_TEXTID, 5011 );
		snprintf( content, MAIL_CONTENT_MAXSIZE, "{\"flag\":1,\"n\":%d,\"lv\":%d,\"na\":\"%s\",\"pos\":\"%d,%d\",\"pp\":%d,\"silver\":%d,\"wood\":%d,\"food\":%d,\"iron\":%d",
			pTargetCity->nation, pTargetCity->level, pTargetCity->name, pTargetCity->posx, pTargetCity->posy, pTargetCity->people, pTargetCity->silver, pTargetCity->wood, pTargetCity->food, pTargetCity->iron );

		if ( wall_success == 1 )
		{ // 侦察到城池信息
			char szTmp[256] = { 0 };
			// 城墙等级
			Building *pWall = building_getptr_kind( pTargetCity->index, BUILDING_Wall );
			int walllv = 0;
			if ( pWall )
				walllv = pWall->level;
			// 兵营库存
			int corps[3] = { 0 };
			corps[0] = pTargetCity->soldiers[0];
			corps[1] = pTargetCity->soldiers[1];
			corps[2] = pTargetCity->soldiers[2];
			snprintf( szTmp, 255, ",\"walllv\":%d,\"bp\":%d,\"cp1\":%d,\"cp2\":%d,\"cp3\":%d,\"hsu\":%d}", walllv, pTargetCity->battlepower, corps[0], corps[1], corps[2], hero_success );
			strcat( content, szTmp );
		}
		else
		{
			strcat( content, "}" );
		}

		// 发送给侦察人
		i64 mailid = mail( pCity->actor_index, pCity->actorid, MAIL_TYPE_CITY_SPY, title, content, "", 0, 0 );
		if ( hero_success == 1 )
		{ // 侦察到武将信息
			g_szSpyMailjson[0] = 0;
			sprintf( g_szSpyMailjson, "{\"heros\":[" );
			
			// 武将
			for ( int tmpi = 0; tmpi < 4; tmpi++ )
			{
				if ( pTargetCity->hero[tmpi].kind <= 0 )
					continue;
				Hero *pHero = &pTargetCity->hero[tmpi];
				HeroInfoConfig *config = hero_getconfig( pHero->kind, pHero->color );
				if ( !config )
					continue;
				char szTmp[512] = { 0 };
				char sflag = ',';
				if ( tmpi == 0 )
					sflag = ' ';

				Army *pArmy = army_getptr_cityhero( pTargetCity, pHero->kind );
				if ( pHero->state == 0 || !pArmy )
				{ // 未出征状态
					sprintf( szTmp, "%c{\"kd\":%d,\"lv\":%d,\"cr\":%d,\"cs\":%d,\"hp\":%d,\"state\":0}",
						sflag, pHero->kind, pHero->level, config->corps, pHero->color, pHero->soldiers );
				}
				else
				{ // 外出要显示更详细内容
					char szInfo[256] = { 0 };
					if ( pArmy->state == ARMY_STATE_MARCH )
					{ // 行军中，要显示，多久到，和行军目标
						if ( pArmy->to_type == MAPUNIT_TYPE_CITY )
						{
							sprintf( szInfo, "\"armystate\":%d,\"armytime\":%d,\"totype\":%d,\"toname\":\"%s\"", pArmy->state, pArmy->statetime, pArmy->to_type, army_getname_target( pArmy->index ) );
						}
						else if ( pArmy->to_type == MAPUNIT_TYPE_TOWN )
						{
							sprintf( szInfo, "\"armystate\":%d,\"armytime\":%d,\"totype\":%d,\"tokind\":%d", pArmy->state, pArmy->statetime, pArmy->to_type, pArmy->to_id );
						}
						else if ( pArmy->to_type == MAPUNIT_TYPE_ENEMY )
						{
							int kind = 0;
							MapEnemy *enemy = map_enemy_getptr( pArmy->to_index );
							if ( enemy )
							{
								kind = enemy->kind;
							}
							sprintf( szInfo, "\"armystate\":%d,\"armytime\":%d,\"totype\":%d,\"tokind\":%d", pArmy->state, pArmy->statetime, pArmy->to_type, kind );
						}
						else if ( pArmy->to_type == MAPUNIT_TYPE_RES )
						{
							int kind = 0;
							MapRes *res = map_res_getptr( pArmy->to_index );
							if ( res )
							{
								kind = res->kind;
							}
							sprintf( szInfo, "\"armystate\":%d,\"armytime\":%d,\"totype\":%d,\"tokind\":%d", pArmy->state, pArmy->statetime, pArmy->to_type, kind );
						}
						else if ( pArmy->to_type == MAPUNIT_TYPE_NATIONHERO )
						{
							// 未知
							sprintf( szInfo, "\"armystate\":%d", pArmy->state );
						}
						else
						{
							sprintf( szInfo, "\"armystate\":%d,\"armytime\":%d,\"totype\":%d", pArmy->state, pArmy->statetime, pArmy->to_type );
						}
						
					}
					else if ( pArmy->state == ARMY_STATE_REBACK )
					{ // 往返程中，只显示还有多久回城
						sprintf( szInfo, "\"armystate\":%d,\"armytime\":%d", pArmy->state, pArmy->statetime );
					}
					else if( pArmy->state == ARMY_STATE_GATHER )
					{ // 采集中，显示采集的是多少级的什么资源点
						int reskind = 0;
						MapRes *res = map_res_getptr( pArmy->to_index );
						if ( res )
						{
							reskind = res->kind;
						}
						sprintf( szInfo, "\"armystate\":%d,\"tokind\":%d", pArmy->state, reskind );
					}
					else
					{ // 未知
						sprintf( szInfo, "\"armystate\":%d", pArmy->state );
					}

					sprintf( szTmp, "%c{\"kd\":%d,\"lv\":%d,\"cr\":%d,\"cs\":%d,\"hp\":%d,\"state\":1,%s}",
						sflag, pHero->kind, pHero->level, pHero->color, config->corps, pHero->soldiers, szInfo );
				}	
				strcat( g_szSpyMailjson, szTmp );
			}
			strcat( g_szSpyMailjson, "]," );

			// 守卫
			strcat( g_szSpyMailjson, "\"guards\":[" );
			for ( int tmpi = 0; tmpi < CITY_GUARD_MAX; tmpi++ )
			{
				if ( pTargetCity->guard[tmpi].monsterid <= 0 )
					continue;
				char szTmp[256] = { 0 };
				char sflag = ',';
				if ( tmpi == 0 )
					sflag = ' ';

				sprintf( szTmp, "%c{\"sp\":%d,\"lv\":%d,\"cr\":%d,\"cs\":%d,\"hp\":%d}",
					sflag, pTargetCity->guard[tmpi].shape, pTargetCity->guard[tmpi].level, pTargetCity->guard[tmpi].color, pTargetCity->guard[tmpi].corps, pTargetCity->guard[tmpi].soldiers );

				strcat( g_szSpyMailjson, szTmp );
			}
			strcat( g_szSpyMailjson, "]}" );

			mail_fight( mailid, pCity->actorid, g_szSpyMailjson );
		}
		
		// 通知侦察人显示界面
		SLK_NetS_MailOpResult info = { 0 };
		info.m_op = 1;
		info.m_mailid = mailid;
		netsend_mailopresult_S( actor_index, SENDTYPE_ACTOR, &info );

		// 发给被侦察人
		sprintf( be_title, "%s%d", TAG_TEXTID, 5013 );
		snprintf( be_content, MAIL_CONTENT_MAXSIZE, "{\"flag\":0,\"n\":%d,\"lv\":%d,\"na\":\"%s\",\"pos\":\"%d,%d\"}", pCity->nation, pCity->level, pCity->name, pCity->posx, pCity->posy );
		mail( pTargetCity->actor_index, pTargetCity->actorid, MAIL_TYPE_CITY_BESPY, be_title, be_content, "", 0, 0 );

		// 事件
		city_battle_event_add( pCity->index, CITY_BATTLE_EVENT_SPY, pTargetCity->name, 1, mailid );
		city_battle_event_add( pTargetCity->index, CITY_BATTLE_EVENT_BESPY, pCity->name, 1, mailid );

		actor_notify_pop( actor_index, 2429 );
	}
	else
	{ // 侦察失败
		sprintf( title, "%s%d", TAG_TEXTID, 5012 );
		snprintf( content, MAIL_CONTENT_MAXSIZE, "{\"flag\":0,\"n\":%d,\"lv\":%d,\"na\":\"%s\",\"pos\":\"%d,%d\"}", pTargetCity->nation, pTargetCity->level, pTargetCity->name, pTargetCity->posx, pTargetCity->posy );
		i64 mailid = mail( pCity->actor_index, pCity->actorid, MAIL_TYPE_CITY_SPY, title, content, "", 0, 0 );

		// 事件
		city_battle_event_add( pCity->index, CITY_BATTLE_EVENT_SPY, pTargetCity->name, 0, mailid );

		//sprintf( be_title, "%s%d", TAG_TEXTID, 5014 );
		//snprintf( be_content, MAIL_CONTENT_MAXSIZE, "{\"fromid\":%d,\"msg\":\"%s\",\"reply\":\"%s\",\"t\":%d,\"n\":%d,\"na\":\"%s\"}", pCity->actorid, pValue->m_content, pValue->m_reply, pValue->m_reply_recvtime, pTargetCity->nation, pTargetCity->name );
		//mail( pTargetCity->actor_index, pTargetCity->actorid, MAIL_TYPE_CITY_BESPY, be_title, be_content, "", 0 );
		actor_notify_pop( actor_index, 2430 );
	}

	return 0;
}

// 家园重建
int city_lost_rebuild( City *pCity )
{
	if ( !pCity )
		return -1;
	if ( pCity->actor_index < 0 )
		return -1;
	if ( pCity->rb_silver <= 0 )
		return -1;
	SLK_NetS_LostRebuild pValue = {0};
	pValue.m_rb_silver = pCity->rb_silver;
	pValue.m_rb_wood = pCity->rb_wood;
	pValue.m_rb_food = pCity->rb_food;
	pValue.m_rb_df = pCity->rb_df;
	netsend_lostrebuild_S( pCity->actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

int city_lost_rebuild_get( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->rb_silver > 0 )
	{
		city_changesilver( pCity->index, pCity->rb_silver, PATH_LOSTREBUILD );
		pCity->rb_silver = 0;
	}
	if ( pCity->rb_wood > 0 )
	{
		city_changewood( pCity->index, pCity->rb_wood, PATH_LOSTREBUILD );
		pCity->rb_wood = 0;
	}
	if ( pCity->rb_food > 0 )
	{
		city_changefood( pCity->index, pCity->rb_food, PATH_LOSTREBUILD );
		pCity->rb_food = 0;
	}
	if ( pCity->rb_df > 0 )
	{
		pCity->rb_df = 0;
	}
	return 0;
}

int city_lost_rebuild_num( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int value[2] = { 0 };
	value[0] = 0;
	value[1] = pCity->rb_num;
	actor_notify_value( actor_index, NOTIFY_LOSTREBUILD, 2, value, NULL );
	return 0;
}

// GM获取城池信息
int city_gm_getinfo( City *pCity )
{
	if ( !pCity )
		return -1;
	g_gm_outresult[0] = '\0';
	char json[512] = { 0 };

	sprintf( json, "{\"serverid\":%d,\"posx\":%d,\"posy\":%d,\"state\":%d,\"sflag\":%d,\"level\":%d,\"exp\":%d,\"viplevel\":%d,\"vipexp\":%d,\"official\":%d,\"place\":%d,\"zone\":%d,", g_Config.server_code, pCity->posx, pCity->posy, pCity->state, pCity->sflag, pCity->level, pCity->exp, pCity->viplevel, pCity->vipexp, pCity->official, pCity->place, pCity->zone );
	strcat( g_gm_outresult, json );

	sprintf( json, "\"bp\":%d,\"bp_hero\":%d,\"bp_equip\":%d,\"bp_tech\":%d,\"bp_nequip\":%d,\"bp_place\":%d,\"bp_girl\":%d,", pCity->battlepower, pCity->battlepower_hero, pCity->battlepower_equip, pCity->battlepower_tech, pCity->battlepower_nequip, pCity->battlepower_place, pCity->battlepower_girl );
	strcat( g_gm_outresult, json );

	sprintf( json, "\"mokilllv\":%d,\"body\":%d,\"silver\":%d,\"wood\":%d,\"food\":%d,\"iron\":%d,\"levynum\":%d,\"people\":%d,\"prestige\":%d,", pCity->mokilllv, pCity->body, pCity->silver, pCity->wood, pCity->food, pCity->iron, pCity->levynum, pCity->people, pCity->prestige );
	strcat( g_gm_outresult, json );

	sprintf( json, "\"soldiers0\":%d,\"soldiers1\":%d,\"soldiers2\":%d,\"mainquestid\":%d,", pCity->soldiers[0], pCity->soldiers[1], pCity->soldiers[2], pCity->questid[0] );
	strcat( g_gm_outresult, json );

	sprintf( json, "\"worker_op\":%d,\"worker_kind\":%d,\"worker_sec\":%d,\"worker_op_ex\":%d,\"worker_kind_ex\":%d,\"worker_sec_ex\":%d,\"worker_expire_ex\":%d,", pCity->worker_op, pCity->worker_kind, pCity->worker_sec, pCity->worker_op_ex, pCity->worker_kind_ex, pCity->worker_sec_ex, pCity->worker_expire_ex );
	strcat( g_gm_outresult, json );

	// 科技列表
	sprintf( json, "\"techlist\":[" );
	strcat( g_gm_outresult, json );
	int count = 0;
	for ( int kind = 0; kind < CITY_TECH_MAX; kind++ )
	{
		if ( pCity->techlevel[kind] <= 0 )
			continue;
		if ( count > 0 )
		{
			strcat( g_gm_outresult, "," );
		}
		sprintf( json, "{\"kind\":%d,\"lv\":%d,\"pro\":%d}", kind, pCity->techlevel[kind], pCity->techprogress[kind] );
		strcat( g_gm_outresult, json );
		count += 1;
	}
	sprintf( json, "]," );
	strcat( g_gm_outresult, json );

	// 建筑列表
	sprintf( json, "\"building\":[" );
	strcat( g_gm_outresult, json );
	count = 0;
	for ( int tmpi = 0; tmpi < BUILDING_MAXNUM; tmpi++ )
	{
		if ( pCity->building[tmpi].kind <= 0 )
			continue;
		if ( count > 0 )
		{
			strcat( g_gm_outresult, "," );
		}
		sprintf( json, "{\"kind\":%d,\"level\":%d}", pCity->building[tmpi].kind, pCity->building[tmpi].level );
		strcat( g_gm_outresult, json );
		count += 1;
	}
	sprintf( json, "]," );
	strcat( g_gm_outresult, json );

	// 兵营建筑列表
	sprintf( json, "\"building_barracks\":[" );
	strcat( g_gm_outresult, json );
	count = 0;
	for ( int tmpi = 0; tmpi < BUILDING_BARRACKS_MAXNUM; tmpi++ )
	{
		if ( pCity->building_barracks[tmpi].kind <= 0 )
			continue;
		if ( count > 0 )
		{
			strcat( g_gm_outresult, "," );
		}
		sprintf( json, "{\"kind\":%d,\"level\":%d,\"queue\":%d,\"trainlong\":%d}", pCity->building_barracks[tmpi].kind, pCity->building_barracks[tmpi].level, pCity->building_barracks[tmpi].queue, pCity->building_barracks[tmpi].trainlong );
		strcat( g_gm_outresult, json );
		count += 1;
	}
	sprintf( json, "]," );
	strcat( g_gm_outresult, json );

	// 资源建筑列表
	sprintf( json, "\"building_res\":[" );
	strcat( g_gm_outresult, json );
	count = 0;
	for ( int tmpi = 0; tmpi < BUILDING_RES_MAXNUM; tmpi++ )
	{
		if ( pCity->building_res[tmpi].kind <= 0 || pCity->building_res[tmpi].level <= 0 )
			continue;
		if ( count > 0 )
		{
			strcat( g_gm_outresult, "," );
		}
		sprintf( json, "{\"kind\":%d,\"level\":%d}", pCity->building_res[tmpi].kind, pCity->building_res[tmpi].level );
		strcat( g_gm_outresult, json );
		count += 1;
	}
	sprintf( json, "]," );
	strcat( g_gm_outresult, json );

	// 出征列表
	sprintf( json, "\"battlelist\":[" );
	strcat( g_gm_outresult, json );
	count = 0;
	for ( int tmpi = 0; tmpi < CITY_BATTLEQUEUE_MAX; tmpi++ )
	{
		int army_index = pCity->battle_armyindex[tmpi];
		if ( army_index < 0 || army_index >= g_army_maxcount )
			continue;
		if ( count > 0 )
		{
			strcat( g_gm_outresult, "," );
		}
		sprintf( json, "{\"ttype\":%d,\"tposx\":%d,\"tposy\":%d,\"tname\":\"%s\",\"state\":%d,\"action\":%d,\"index\":%d}", g_army[army_index].to_type, g_army[army_index].to_posx, g_army[army_index].to_posy, army_getname_target( army_index ), g_army[army_index].state, g_army[army_index].action, army_index );
		strcat( g_gm_outresult, json );
		count += 1;
	}
	sprintf( json, "]," );
	strcat( g_gm_outresult, json );

	// 挨打列表
	sprintf( json, "\"underfirelist\":[" );
	strcat( g_gm_outresult, json );
	count = 0;
	for ( int tmpi = 0; tmpi < CITY_UNDERFIRE_MAX; tmpi++ )
	{
		int army_index = pCity->underfire_armyindex[tmpi];
		if ( army_index < 0 || army_index >= g_army_maxcount )
			continue;
		if ( count > 0 )
		{
			strcat( g_gm_outresult, "," );
		}
		sprintf( json, "{\"ftype\":%d,\"fposx\":%d,\"fposy\":%d,\"fname\":\"%s\",\"state\":%d,\"action\":%d,\"index\":%d}", g_army[army_index].from_type, g_army[army_index].from_posx, g_army[army_index].from_posy, army_getname( army_index ), g_army[army_index].state, g_army[army_index].action, army_index );
		strcat( g_gm_outresult, json );
		count += 1;
	}
	sprintf( json, "]," );
	strcat( g_gm_outresult, json );

	// 帮助列表
	sprintf( json, "\"helplist\":[" );
	strcat( g_gm_outresult, json );
	count = 0;
	for ( int tmpi = 0; tmpi < CITY_HELPDEFENSE_MAX; tmpi++ )
	{
		int army_index = pCity->help_armyindex[tmpi];
		if ( army_index < 0 || army_index >= g_army_maxcount )
			continue;
		if ( count > 0 )
		{
			strcat( g_gm_outresult, "," );
		}
		sprintf( json, "{\"ftype\":%d,\"fposx\":%d,\"fposy\":%d,\"fname\":\"%s\",\"state\":%d,\"index\":%d}", g_army[army_index].from_type, g_army[army_index].from_posx, g_army[army_index].from_posy, army_getname( army_index ), g_army[army_index].state, army_index );
		strcat( g_gm_outresult, json );
		count += 1;
	}
	sprintf( json, "]" );


	strcat( g_gm_outresult, json );
	strcat( g_gm_outresult, "}" );
	return 0;
}