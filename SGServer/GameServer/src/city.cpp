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
#include "building.h"
#include "hero.h"
#include "equip.h"
#include "mapunit.h"
#include "map.h"
#include "server_netsend_auto.h"
#include "system.h"
#include "item.h"

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
							g_city[city_index].building[tmpi].sec = 0;
							g_city[city_index].building[tmpi].needsec = 0;
							g_city[city_index].building[tmpi].overvalue = g_city[city_index].building[tmpi].value;
							g_city[city_index].building[tmpi].value = 0;
							building_sendinfo( g_city[city_index].actor_index, kind );
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
			if ( g_city[city_index].building_barracks[tmpi].kind <= 0 )
				continue;
			if ( g_city[city_index].building_barracks[tmpi].trainsec > 0 )
			{
				g_city[city_index].building_barracks[tmpi].trainsec -= 1;
				if ( g_city[city_index].building_barracks[tmpi].trainsec <= 0 )
				{
					// 招募完毕
					g_city[city_index].building_barracks[tmpi].overnum += g_city[city_index].building_barracks[tmpi].trainnum;
					g_city[city_index].building_barracks[tmpi].trainsec = 0;
					g_city[city_index].building_barracks[tmpi].trainsec_need = 0;
					g_city[city_index].building_barracks[tmpi].trainnum = 0;
					if ( g_city[city_index].building_barracks[tmpi].queue )
					{
					}
				}
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
		Value.m_level = g_city[city_index].level;
		Value.m_isup = isup;
		Value.m_path = path;
		netsend_experience_S( g_city[city_index].actor_index, SENDTYPE_ACTOR, &Value );
	}
	return isup;
}

// 主角升级
int city_actorupgrade( int city_index, char path, AwardGetInfo *getinfo )
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
	if ( value > 0 && g_city[city_index].body > SHRT_MAX - value )
		g_city[city_index].body = SHRT_MAX;
	else
		g_city[city_index].body += value;
	
	ACTOR_CHECK_INDEX( g_city[city_index].actor_index );
	SLK_NetS_Body pValue = { 0 };
	pValue.m_add = value;
	pValue.m_total = g_city[city_index].body;
	pValue.m_path = path;
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
		return -1;
	int count = 0;
	for ( int tmpi = 0; tmpi < CITY_GUARD_MAX; tmpi++ )
	{
		if ( g_city[city_index].guard[tmpi].monsterid > 0 )
			count += 1;
	}
	if ( count >= building_getlevel( city_index, BUILDING_Wall, -1 ) )
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
	
	char monsterid = random( 1, g_cityguardinfo_maxnum-1 );
	char corps = random( 0, 2 );
	char color = random( 0, 2 );
	char shape = random( 0, 7 );
	unsigned char minlevel = g_city[city_index].level - 10;
	unsigned char maxlevel = g_city[city_index].level + 10;
	minlevel = minlevel <= 0 ? 1 : minlevel;
	maxlevel = maxlevel > global.actorlevel_max ? global.actorlevel_max : maxlevel;
	char level = random( minlevel, maxlevel );

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
	char monsterid = g_city[city_index].guard[offset].monsterid;
	char color = g_city[city_index].guard[offset].color;
	char level = g_city[city_index].guard[offset].level;
	if ( color < ITEM_COLOR_LEVEL_GREEN )
	{
		if ( rand()%100 < 50  )
		{
			color += 1;
		}	
	}
	
	if ( level < g_city[city_index].level + 10 )
	{
		level += 1;
	}
	
	CityGuardInfoConfig *config = city_guard_config( monsterid, color );
	if ( config )
	{
		if ( actor_change_token( g_city[city_index].actor_index, -global.city_guard_up_token, PATH_GUARD_UPGRADE, 0 ) < 0 )
		{
			return -1;
		}	
		g_city[city_index].guard[offset].color = color;
		g_city[city_index].guard[offset].level = level;
		g_city[city_index].guard[offset].soldiers = TROOPS( level, config->troops, config->troops_growth );
		actor_change_token( g_city[city_index].actor_index, global.city_guard_up_token, PATH_GUARD_UPGRADE, 0 );
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

int city_yield_total( City *pCity, int kind )
{
	if ( !pCity )
		return 0;
	int yield = 0;
	yield = city_yield_base( pCity, kind );
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

		//pValue.m_tech[4]
		//pValue.m_weather[4]
		//pValue.m_activity[4]
		//pValue.m_offical[4]
	netsend_levyinfo_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 招募每五分钟数
int city_trainnum( City *pCity, int base )
{
	if ( !pCity )
	{
		return base;
	}
	int total = base;
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
	if ( barracks->soldiers >= config->value[1] )
		return -1;

	int v = trainsec / 300;
	int trainnum = v * city_trainnum( pCity, config->value[0] );
	int trainfood = city_trainfood( pCity )*trainnum;
	if ( pCity->food < trainfood )
		return -1;
	city_changefood( pCity->index, -trainfood, PATH_TRAIN );
	barracks->trainsec = trainsec;
	barracks->trainsec_need = trainsec;
	barracks->trainnum = trainnum;
	city_train_sendinfo( actor_index, kind );
	building_sendinfo_barracks( actor_index, kind );
	return 0;
}

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

	city_train_sendinfo( actor_index, kind );
	return 0;
}

int city_train_quick( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	BuildingBarracks *barracks = buildingbarracks_getptr_kind( pCity->index, kind );
	if ( !barracks )
		return -1;

	city_train_sendinfo( actor_index, kind );
	building_sendinfo_barracks( actor_index, kind );
	return 0;
}

int city_train_get( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	BuildingBarracks *barracks = buildingbarracks_getptr_kind( pCity->index, kind );
	if ( !barracks )
		return -1;

	building_sendinfo_barracks( actor_index, kind );
	return 0;
}

int city_train_buyqueue( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	BuildingBarracks *barracks = buildingbarracks_getptr_kind( pCity->index, kind );
	if ( !barracks )
		return -1;

	city_train_sendinfo( actor_index, kind );
	return 0;
}

int city_train_buylong( int actor_index, int kind )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	BuildingBarracks *barracks = buildingbarracks_getptr_kind( pCity->index, kind );
	if ( !barracks )
		return -1;

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
	pValue.m_soldiers_max = config->value[1];
	pValue.m_trainnum = barracks->trainnum;
	pValue.m_trainsec = barracks->trainsec;
	pValue.m_trainsec_need = barracks->trainsec_need;
	pValue.m_queue = barracks->queue;
	pValue.m_trainlong = barracks->trainlong;
	pValue.m_train_confnum = city_trainnum( pCity, config->value[0] );
	pValue.m_train_confsec = g_trainlong[barracks->trainlong].timelong*60;
	pValue.m_train_conffood = city_trainfood( pCity );
	pValue.m_queuenum[16];	//队列
	int count = 0;
	for ( int tmpi = 0; tmpi < 8; tmpi++ )
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
