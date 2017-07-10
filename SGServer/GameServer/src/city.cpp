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
	for ( city_index = 1; city_index < g_city_maxcount; city_index++ )
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
			}
		}

		// 普通建筑逻辑
		for ( int tmpi = 0; tmpi < BUILDING_MAXNUM; tmpi++ )
		{
			if ( g_city[city_index].building[tmpi].kind <= 0 )
				continue;
			//g_city[city_index].building[tmpi].sec -= 1;
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
					g_city[city_index].building_barracks[tmpi].soldiers += g_city[city_index].building_barracks[tmpi].trainnum;
					g_city[city_index].building_barracks[tmpi].trainsec = 0;
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
			g_city[city_index].exp -= g_upgradeinfo[g_city[city_index].level].exp;
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
	if ( g_city[city_index].body > SHRT_MAX - value )
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
	if ( g_city[city_index].levynum > CHAR_MAX - value )
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
	if ( g_city[city_index].silver > INT_MAX - value )
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
	if ( g_city[city_index].wood > INT_MAX - value )
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
	if ( g_city[city_index].food > INT_MAX - value )
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
	if ( g_city[city_index].iron > INT_MAX - value )
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
	if ( g_city[city_index].people > INT_MAX - value )
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
	if ( g_city[city_index].prestige > INT_MAX - value )
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
	if ( g_city[city_index].friendship > INT_MAX - value )
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
