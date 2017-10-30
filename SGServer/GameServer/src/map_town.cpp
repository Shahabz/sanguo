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
#include "map.h"
#include "map_zone.h"
#include "map_town.h"
#include "mail.h"
#include "city.h"
#include "nation.h"
#include "chat.h"
#include "army_group.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern City * g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern Army *g_army;
extern int g_army_maxcount;

extern MonsterInfo *g_monster;
extern int g_monster_maxnum;

extern ArmyGroup *g_armygroup;
extern int g_armygroup_maxcount;

extern MapTownInfo *g_towninfo;
extern int g_towninfo_maxnum;

MapTown *g_map_town = NULL;
int g_map_town_maxcount = 0;
extern int g_map_town_maxindex;
int g_map_town_attackmonster[3][8] = { { 1884, 1885, 1886, 1887, 0, 0, 0, 0 }, { 1884, 1885, 1886, 1887, 0, 0, 0, 0 }, { 1884, 1885, 1886, 1887, 0, 0, 0, 0 } };

// 读档完毕的回调
int map_town_loadcb( int townid )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;

	// 重置守军
	map_town_monster_reset( townid, 0 );

	// 城主
	if ( g_map_town[townid].own_actorid > 0 )
	{
		g_map_town[townid].own_city_index = city_getindex_withactorid( g_map_town[townid].own_actorid );
		if ( g_map_town[townid].own_city_index >= 0 && g_map_town[townid].own_city_index < g_city_maxcount )
		{
			g_city[g_map_town[townid].own_city_index].own_townid = townid;
		}
		else
		{
			g_map_town[townid].own_actorid = 0;
			g_map_town[townid].own_city_index = -1;
			g_map_town[townid].own_sec = 0;
		}
	}
	else
	{ // 竞选者
		for ( int tmpi = 0; tmpi < MAP_TOWN_JOIN_MAX; tmpi++ )
		{
			if ( g_map_town[townid].ask_actorid[tmpi] <= 0 )
				continue;
			g_map_town[townid].ask_city_index[tmpi] = city_getindex_withactorid( g_map_town[townid].ask_actorid[tmpi] );
		}
	}

	// 城镇辐射圈归属
	map_tile_setnation( g_towninfo[townid].posx, g_towninfo[townid].posy, g_towninfo[townid].range, townid, g_map_town[townid].nation );

	// 核心建筑决定地区归属
	if ( g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_TYPE3 || g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_TYPE6 || g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_TYPE9 )
	{
		map_zone_setnation( g_map_town[townid].zoneid, g_map_town[townid].nation );
	}
	return 0;
}

int map_town_load()
{
	g_map_town_maxcount = g_towninfo_maxnum;
	g_map_town = (MapTown*)malloc( sizeof( MapTown ) * g_map_town_maxcount );
	memset( g_map_town, 0, sizeof( MapTown ) * g_map_town_maxcount );
	printf_msg( "MapTown  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_map_town_maxcount, (sizeof( MapTown )*g_map_town_maxcount) / 1024.0 / 1024.0, sizeof( MapTown ) / 1024.0 );
	for ( int townid = 1; townid < g_map_town_maxcount; townid++ )
	{
		if ( g_towninfo[townid].id <= 0 )
			continue;
		g_map_town[townid].townid = townid;
		g_map_town[townid].own_city_index = -1;
		for ( int tmpi = 0; tmpi < MAP_TOWN_UNDERFIRE_GROUP_MAX; tmpi++ )
		{
			g_map_town[townid].underfire_groupindex[tmpi] = -1;
		}

		for ( int tmpi = 0; tmpi < MAP_TOWN_JOIN_MAX; tmpi++ )
		{
			g_map_town[townid].ask_city_index[tmpi] = -1;
		}

		// 上一级townid列表
		char **pptable = NULL;
		int groupcount = 0;
		pptable = u_strcut_ex( g_towninfo[townid].preid, ',', &groupcount );
		for ( int tmpi = 0; tmpi < groupcount; tmpi++ )
		{
			if ( groupcount >= 8 )
				break;
			g_map_town[townid].pre_townid[tmpi] = atoi( pptable[tmpi] );
		}
		u_free_vec( pptable );

		// 添加到地图显示单元
		g_map_town[townid].unit_index = mapunit_add( MAPUNIT_TYPE_TOWN, townid );

		// 所属地区
		g_map_town[townid].zoneid = map_zone_getid( g_towninfo[townid].posx, g_towninfo[townid].posy );

		// 占地块信息添加到世界地图
		map_addobject( MAPUNIT_TYPE_TOWN, townid, g_towninfo[townid].posx, g_towninfo[townid].posy );

		// 城镇辐射圈归属
		map_tile_setnation( g_towninfo[townid].posx, g_towninfo[townid].posy, g_towninfo[townid].range, townid, g_map_town[townid].nation );

		// 核心建筑决定地区归属
		if ( g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_TYPE3 || g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_TYPE6 || g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_TYPE9 )
		{
			map_zone_setnation( g_map_town[townid].zoneid, g_map_town[townid].nation );
		}

		// 重置守军
		map_town_monster_reset( townid, 0 );
	}
	map_town_load_auto( map_town_getptr, map_town_loadcb, "map_town" );
	return 0;
}

MapTown *map_town_getptr( int townid )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return NULL;
	return &g_map_town[townid];
}

MapTownInfo *map_town_getconfig( int townid )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return NULL;
	return &g_towninfo[townid];
}

int map_town_save( FILE *fp )
{
	map_town_batch_save_auto( g_map_town, g_map_town_maxcount, "map_town", fp );
	return 0;
}

// 显示单元属性
void map_town_makeunit( int index, SLK_NetS_AddMapUnit *pAttr )
{
	if ( index <= 0 || index >= g_map_town_maxcount )
		return;
	if ( g_map_town[index].name[0] != 0 )
	{
		strncpy( pAttr->m_name, g_map_town[index].name, NAME_SIZE );
		pAttr->m_namelen = strlen( pAttr->m_name );
	}
	pAttr->m_posx = g_towninfo[index].posx;
	pAttr->m_posy = g_towninfo[index].posy;
	pAttr->m_char_value[0] = g_map_town[index].nation;
	pAttr->m_char_value[1] = g_map_town[index].dev_level;
	pAttr->m_char_value[2] = -1; // 国战攻击方国家
	pAttr->m_char_value[3] = -1; // 国战攻击方国家
	pAttr->m_char_value[4] = -1; // 国战攻击方国家
	pAttr->m_char_value[5] = -1; // 国战攻击方国家
	pAttr->m_char_value_count = 6;

	pAttr->m_short_value[0] = (short)g_towninfo[index].id;
	pAttr->m_short_value[1] = g_map_town[index].produce_num;
	pAttr->m_short_value_count = 2;

	pAttr->m_int_value[0] = g_map_town[index].protect_sec;
	pAttr->m_int_value[1] = g_map_town[index].produce_sec;
	pAttr->m_int_value_count = 2;

	// 国战信息
	int count = 2;
	for ( int tmpi = 0; tmpi < MAP_TOWN_UNDERFIRE_GROUP_MAX; tmpi++ )
	{
		int group_index = g_map_town[index].underfire_groupindex[tmpi];
		if ( group_index < 0 || group_index >= g_armygroup_maxcount )
			continue;
		pAttr->m_char_value[count++] = g_armygroup[group_index].from_nation;
	}
}

// 位置
void map_town_getpos( int index, short *posx, short *posy )
{
	MapTownInfo *config = map_town_getconfig( index );
	if ( config )
	{
		*posx = config->posx;
		*posy = config->posy;
	}
}

// 获取国家
char map_town_getnation( int townid )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return 0;
	return g_map_town[townid].nation;
}

// 重置守军
void map_town_monster_reset( int townid, char reset )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return;
	char **pptable = NULL;
	int groupcount = 0;

	// 先重置
	for ( int tmpi = 0; tmpi < MAP_TOWN_MONSTER_MAX; tmpi++ )
	{
		g_map_town[townid].monster[tmpi] = 0;
	}

	// 将字符串形式的组合数据拆解成整数数组
	if ( g_map_town[townid].nation == 0 )
	{
		pptable = u_strcut_ex( g_towninfo[townid].monster, ',', &groupcount );
	}
	else
	{
		pptable = u_strcut_ex( g_towninfo[townid].monster_guard, ',', &groupcount );
	}
	for ( int tmpi = 0; tmpi < groupcount; tmpi++ )
	{
		if ( groupcount >= MAP_TOWN_MONSTER_MAX )
			break;
		g_map_town[townid].monster[tmpi] = atoi( pptable[tmpi] );

		int monsterid = g_map_town[townid].monster[tmpi];
		if ( monsterid <= 0 || monsterid >= g_monster_maxnum )
			continue;

		// 群雄守卫，血量重置
		if ( g_map_town[townid].nation == 0 || reset )
		{
			g_map_town[townid].soldier[tmpi] = g_monster[monsterid].troops;
		}
	}

	// 容错
	for ( int tmpi = 0; tmpi < MAP_TOWN_MONSTER_MAX; tmpi++ )
	{
		int monsterid = g_map_town[townid].monster[tmpi];
		if ( monsterid <= 0 || monsterid >= g_monster_maxnum )
		{
			g_map_town[townid].soldier[tmpi] = 0;
			continue;
		}

		if ( g_map_town[townid].soldier[tmpi] > g_monster[monsterid].troops )
		{
			g_map_town[townid].soldier[tmpi] = g_monster[monsterid].troops;
		}
	}

	// 释放内存空间
	u_free_vec( pptable );
}

// 单个城镇每秒逻辑
void map_town_logic( int townid )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return;

	if ( g_map_town[townid].protect_sec > 0 )
	{ // 保护时间内
		g_map_town[townid].protect_sec -= 1;
		if ( g_map_town[townid].protect_sec <= 0 )
		{ // 保护时间到，执行选城主逻辑
			map_town_alloc_owner( townid );
			mapunit_update( MAPUNIT_TYPE_TOWN, townid, g_map_town[townid].unit_index );
		}
	}
	else if ( g_map_town[townid].own_actorid > 0 )
	{ // 有城主，减少任期
		g_map_town[townid].own_sec -= 1;
		if ( g_map_town[townid].own_sec <= 0 )
		{ // 任期到
			// 发送邮件
			char v1[64] = { 0 };
			char v2[64] = { 0 };
			sprintf( v1, "%s%d", TAG_TOWNID, townid );
			sprintf( v2, "%d,%d", g_towninfo[townid].posx, g_towninfo[townid].posy );
			mail_system( -1, g_map_town[townid].own_actorid, 5024, 5522, v1, v2, NULL, "" );

			// 重置
			if ( g_map_town[townid].own_city_index >= 0 && g_map_town[townid].own_city_index < g_city_maxcount )
			{
				g_city[g_map_town[townid].own_city_index].own_townid = 0;
			}
			g_map_town[townid].own_actorid = 0;
			g_map_town[townid].own_city_index = -1;
			mapunit_update( MAPUNIT_TYPE_TOWN, townid, g_map_town[townid].unit_index );
		}
	}

	// 征收次数
	if ( g_map_town[townid].nation > 0 && g_map_town[townid].produce_num < g_towninfo[townid].produce_maxnum )
	{
		if ( g_map_town[townid].produce_sec > 0 )
		{
			g_map_town[townid].produce_sec -= 1;
			if ( g_map_town[townid].produce_sec <= 0 )
			{
				g_map_town[townid].produce_num += 1;
				g_map_town[townid].produce_sec = g_towninfo[townid].produce_maxsec;
				mapunit_update( MAPUNIT_TYPE_TOWN, townid, g_map_town[townid].unit_index );
			}
		}
	}
	
	// 主动出击
	if ( g_towninfo[townid].type == MAPUNIT_TYPE_TOWN_TYPE8 && g_map_town[townid].attackcd > 0 )
	{
		g_map_town[townid].attackcd -= 1;
		if ( g_map_town[townid].attackcd <= 0 )
		{
			map_town_attack( townid );
			g_map_town[townid].attackcd = global.town_attackcd;
		}
	}
}

// 所有城镇每秒逻辑
void map_town_alllogic()
{
	for ( int townid = 1; townid < g_map_town_maxcount; townid++ )
	{
		if ( g_towninfo[townid].id <= 0 )
			continue;
		map_town_logic( townid );
	}
}

// 检查是否是参战人员
int map_town_check_join( int townid, int actorid )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;

	// 检查是否是参战人员
	int tmpi = 0;
	for ( tmpi = 0; tmpi < MAP_TOWN_JOIN_MAX; tmpi++ )
	{
		if ( g_map_town[townid].join_actorid[tmpi] == actorid )
		{
			break;
		}
	}
	if ( tmpi >= MAP_TOWN_JOIN_MAX )
	{ // 不是参战人员
		return -1;
	}

	return 0;
}

// 检查是否是已经申请人员
int map_town_check_ask( int townid, int actorid )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;

	// 检查是否是参战人员
	int tmpi = 0;
	for ( tmpi = 0; tmpi < MAP_TOWN_JOIN_MAX; tmpi++ )
	{
		if ( g_map_town[townid].ask_actorid[tmpi] == actorid )
		{
			break;
		}
	}
	if ( tmpi >= MAP_TOWN_JOIN_MAX )
	{ // 没有申请
		return -1;
	}

	return 0;
}

// 申请城主
int map_town_ask_owner( int actor_index, int townid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->own_townid > 0 )
	{
		actor_notify_alert( actor_index, 1353 );
		return -1;
	}
	int type = 0;
	int free_index = -1;
	// 如果已经过了保护期
	if ( g_map_town[townid].protect_sec <= 0 )
	{
		if ( g_map_town[townid].own_actorid > 0 )
		{ // 已经有城主了
			return -1;
		}
		// 不能跨区域申请城主
		if ( map_zone_checksame( g_towninfo[townid].posx, g_towninfo[townid].posy, pCity->posx, pCity->posy ) == 0 )
		{
			actor_notify_alert( actor_index, 1271 );
			return -1;
		}
		type = 0;
	}
	else
	{
		type = 1;
		// 检查是否是参战人员
		if ( map_town_check_join( townid, g_actors[actor_index].actorid ) < 0 )
		{
			actor_notify_alert( actor_index, 1311 );
			return -1;
		}

		// 检查是否已经申请过
		if ( map_town_check_ask( townid, g_actors[actor_index].actorid ) == 0 )
			return -1;

		// 找到一个位置
		for ( int tmpi = 0; tmpi < MAP_TOWN_JOIN_MAX; tmpi++ )
		{
			if ( g_map_town[townid].ask_actorid[tmpi] <= 0 )
			{
				free_index = tmpi;
				break;
			}

			City *pAsk = city_indexptr( g_map_town[townid].ask_city_index[tmpi] );
			if ( pAsk )
			{
				if ( pCity->place > pAsk->place )
				{
					free_index = tmpi;
					break;
				}
				else if ( pCity->place == pAsk->place && pCity->battlepower > pAsk->battlepower )
				{
					free_index = tmpi;
					break;
				}
			}
		}
	}


	// 检查申请资源
	if ( g_towninfo[townid].ask_silver > 0 )
	{
		if ( pCity->silver < g_towninfo[townid].ask_silver )
		{
			return -1;
		}
	}
	if ( g_towninfo[townid].ask_wood > 0 )
	{
		if ( pCity->wood < g_towninfo[townid].ask_wood )
		{
			return -1;
		}
	}
	if ( g_towninfo[townid].ask_food > 0 )
	{
		if ( pCity->food < g_towninfo[townid].ask_food )
		{
			return -1;
		}
	}
	if ( g_towninfo[townid].ask_iron > 0 )
	{
		if ( pCity->iron < g_towninfo[townid].ask_iron )
		{
			return -1;
		}
	}

	// 扣除申请资源
	if ( g_towninfo[townid].ask_silver > 0 )
	{
		city_changesilver( pCity->index, -g_towninfo[townid].ask_silver, PATH_TOWNASK );
	}
	if ( g_towninfo[townid].ask_wood > 0 )
	{
		city_changewood( pCity->index, -g_towninfo[townid].ask_wood, PATH_TOWNASK );
	}
	if ( g_towninfo[townid].ask_food > 0 )
	{
		city_changefood( pCity->index, -g_towninfo[townid].ask_food, PATH_TOWNASK );
	}
	if ( g_towninfo[townid].ask_iron > 0 )
	{
		city_changeiron( pCity->index, -g_towninfo[townid].ask_iron, PATH_TOWNASK );
	}


	if ( type == 0 )
	{ // 直接成为城主
		g_map_town[townid].own_actorid = pCity->actorid;
		g_map_town[townid].own_city_index = pCity->index;
		g_map_town[townid].own_sec = g_towninfo[townid].own_maxsec;
		pCity->own_townid = townid;
		// 您成为了{ 0 }<url = { 1 }>[{1}]< / url>的城主。任期{ 2 }天
		char v1[64] = { 0 };
		char v2[64] = { 0 };
		char v3[64] = { 0 };
		char v4[64] = { 0 };
		sprintf( v1, "%s%d", TAG_TOWNID, townid );
		sprintf( v2, "%d,%d", g_towninfo[townid].posx, g_towninfo[townid].posy );
		sprintf( v3, "%s%d", TAG_TIMEDAY, g_towninfo[townid].own_maxsec );
		mail_system( pCity->actor_index, pCity->actorid, 5025, 5524, v1, v2, v3, "" );
		actor_notify_alert_v( actor_index, 1307, v1, NULL );

		// 6003	玩家{3}重建了{0}的{1}<color=d95df4ff><url={2}>[{2}]</url></color>
		sprintf( v1, "%s%d", TAG_ZONEID, g_towninfo[townid].zoneid );
		sprintf( v2, "%s%d", TAG_TOWNID, townid );
		sprintf( v3, "%d,%d", g_towninfo[townid].posx, g_towninfo[townid].posy );
		sprintf( v4, "%s", pCity->name );
		system_talkjson( 0, pCity->nation, 6003, v1, v2, v3, v4, NULL, NULL, 1 );
	}
	else
	{
		if ( free_index < 0 || free_index >= MAP_TOWN_JOIN_MAX )
		{ // 没有申请位置了
			return -1;
		}
		// 位置后移，我插入
		if ( free_index < MAP_TOWN_JOIN_MAX - 1 )
		{
			memmove( &g_map_town[townid].ask_actorid[free_index+1], &g_map_town[townid].ask_actorid[free_index], sizeof( int )*(MAP_TOWN_JOIN_MAX - 1 - free_index) );
			memmove( &g_map_town[townid].ask_city_index[free_index+1], &g_map_town[townid].ask_city_index[free_index], sizeof( int )*(MAP_TOWN_JOIN_MAX - 1 - free_index) );
		}
		g_map_town[townid].ask_actorid[free_index] = pCity->actorid;
		g_map_town[townid].ask_city_index[free_index] = pCity->index;
		map_town_ask_owner_sendlist( actor_index, townid );
	}

	return 0;
}

// 分配城主
int map_town_alloc_owner( int townid )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;

	City *pCity = NULL;
	int place = 0;
	int battlepower = 0;
	for ( int tmpi = 0; tmpi < MAP_TOWN_JOIN_MAX; tmpi++ )
	{
		if ( g_map_town[townid].ask_actorid[tmpi] <= 0 )
			continue;
		City *pAsk = city_indexptr( g_map_town[townid].ask_city_index[tmpi] );
		if ( !pAsk )
			continue;
		if ( pAsk->own_townid > 0 )
			continue;
		if ( (!pCity) || (pAsk->place > place) || (pAsk->place == place && pAsk->battlepower > battlepower) )
		{
			pCity = pAsk;
			place = pAsk->place;
			battlepower = pAsk->battlepower;
		}
	}

	if ( !pCity )
		return -1;

	// 成为城主
	g_map_town[townid].own_actorid = pCity->actorid;
	g_map_town[townid].own_city_index = pCity->index;
	g_map_town[townid].own_sec = g_towninfo[townid].own_maxsec;
	pCity->own_townid = townid;

	// 您成为了{ 0 }<url = { 1 }>[{1}]< / url>的城主。任期{ 2 }天
	char v1[64] = { 0 };
	char v2[64] = { 0 };
	char v3[64] = { 0 };
	char v4[64] = { 0 };
	sprintf( v1, "%s%d", TAG_TOWNID, townid );
	sprintf( v2, "%d,%d", g_towninfo[townid].posx, g_towninfo[townid].posy );
	sprintf( v3, "%s%d", TAG_TIMEDAY, g_towninfo[townid].own_maxsec );
	mail_system( pCity->index, pCity->actorid, 5025, 5524, v1, v2, v3, "" );

	// 给其它竞选者发送失败邮件, 并返还资源
	// 很遗憾，{0}在众多候选人中爵位最高，成为了{1}<url={2}>[{2}]</url>的城主
	sprintf( v1, "%s", pCity->name );
	sprintf( v2, "%s%d", TAG_TOWNID, townid );
	sprintf( v3, "%d,%d", g_towninfo[townid].posx, g_towninfo[townid].posy );
	char attach[256] = { 0 };
	char tmpStr[64] = { 0 };
	if ( g_towninfo[townid].ask_silver > 0 )
	{
		sprintf( tmpStr, "%d,%d@", AWARDKIND_SILVER, g_towninfo[townid].ask_silver );
		strcat( attach, tmpStr );
	}
	if ( g_towninfo[townid].ask_wood > 0 )
	{
		sprintf( tmpStr, "%d,%d@", AWARDKIND_WOOD, g_towninfo[townid].ask_wood );
		strcat( attach, tmpStr );
	}
	if ( g_towninfo[townid].ask_food > 0 )
	{
		sprintf( tmpStr, "%d,%d@", AWARDKIND_FOOD, g_towninfo[townid].ask_food );
		strcat( attach, tmpStr );
	}
	if ( g_towninfo[townid].ask_iron > 0 )
	{
		sprintf( tmpStr, "%d,%d@", AWARDKIND_IRON, g_towninfo[townid].ask_iron );
		strcat( attach, tmpStr );
	}
	
	for ( int tmpi = 0; tmpi < MAP_TOWN_JOIN_MAX; tmpi++ )
	{
		if ( g_map_town[townid].ask_actorid[tmpi] <= 0 )
			continue;
		if ( g_map_town[townid].ask_actorid[tmpi] == pCity->actorid )
			continue;
		City *pAsk = city_indexptr( g_map_town[townid].ask_city_index[tmpi] );
		if ( !pAsk )
			continue;
		mail_system( pAsk->index, pAsk->actorid, 5026, 5525, v1, v2, v3, attach );
		g_map_town[townid].ask_actorid[tmpi] = 0;
		g_map_town[townid].ask_city_index[tmpi] = -1;
	}

	// 6003	玩家{3}重建了{0}的{1}<color=d95df4ff><url={2}>[{2}]</url></color>
	sprintf( v1, "%s%d", TAG_ZONEID, g_towninfo[townid].zoneid );
	sprintf( v2, "%s%d", TAG_TOWNID, townid );
	sprintf( v3, "%d,%d", g_towninfo[townid].posx, g_towninfo[townid].posy );
	sprintf( v4, "%s", pCity->name );
	system_talkjson( 0, pCity->nation, 6003, v1, v2, v3, v4, NULL, NULL, 1 );
	return 0;
}

// 放弃城主
int map_town_owner_leave( int actor_index, int townid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( g_map_town[townid].own_actorid != pCity->actorid )
		return -1;

	if ( g_map_town[townid].own_city_index >= 0 && g_map_town[townid].own_city_index < g_city_maxcount )
	{
		g_city[g_map_town[townid].own_city_index].own_townid = 0;
	}

	g_map_town[townid].own_actorid = 0;
	g_map_town[townid].own_city_index = -1;
	g_map_town[townid].own_sec = 0;

	char v1[64] = { 0 };
	char v2[64] = { 0 };
	char v3[64] = { 0 };
	char v4[64] = { 0 };

	// 6004	玩家{3}深明大义，让出了{0}的{1}<color=d95df4ff><url={2}>[{2}]</url></color>
	sprintf( v1, "%s%d", TAG_ZONEID, g_towninfo[townid].zoneid );
	sprintf( v2, "%s%d", TAG_TOWNID, townid );
	sprintf( v3, "%d,%d", g_towninfo[townid].posx, g_towninfo[townid].posy );
	sprintf( v4, "%s", pCity->name );
	system_talkjson( 0, pCity->nation, 6004, v1, v2, v3, v4, NULL, NULL, 1 );
	return 0;
}

// 补兵
int map_town_soldiers_repair( int actor_index, int townid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( g_map_town[townid].own_actorid != pCity->actorid )
		return -1;

	int maxhp = 0;
	int hp = 0;
	for ( int tmpi = 0; tmpi < MAP_TOWN_MONSTER_MAX; tmpi++ )
	{
		int monsterid = g_map_town[townid].monster[tmpi];
		if ( monsterid <= 0 || monsterid >= g_monster_maxnum )
			continue;
		maxhp += g_monster[monsterid].troops;
		hp += g_map_town[townid].soldier[tmpi];
	}

	int repairhp = min( maxhp - hp, maxhp / 20 );
	int cost_silver = (int)ceil( repairhp / (float)maxhp * g_towninfo[townid].ask_silver );
	int cost_wood = (int)ceil( repairhp / (float)maxhp * g_towninfo[townid].ask_wood );

	if ( pCity->silver < cost_silver )
		return -1;
	if ( pCity->wood < cost_wood )
		return -1;
	city_changesilver( pCity->index, -cost_silver, PATH_TOWNREPAIR );
	city_changesilver( pCity->index, -cost_wood, PATH_TOWNREPAIR );

	// 一个一个补
	for ( int tmpi = 0; tmpi < MAP_TOWN_MONSTER_MAX; tmpi++ )
	{
		if ( repairhp <= 0 )
			break;

		int monsterid = g_map_town[townid].monster[tmpi];
		if ( monsterid <= 0 || monsterid >= g_monster_maxnum )
			continue;

		int add = g_monster[monsterid].troops - g_map_town[townid].soldier[tmpi];
		if ( add <= 0 )
			continue;

		if ( add >= repairhp )
		{
			add = repairhp;
			repairhp = 0;
		}
		else
		{
			repairhp -= add;
		}

		g_map_town[townid].soldier[tmpi] += add;
		if ( g_map_town[townid].soldier[tmpi] > g_monster[monsterid].troops )
			g_map_town[townid].soldier[tmpi] = g_monster[monsterid].troops;
	}

	map_town_sendinfo( actor_index, townid );
	return 0;
}

// 城镇征收
int map_town_levy( int actor_index, int townid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( g_map_town[townid].nation != pCity->nation )
		return -1;
	if ( g_map_town[townid].produce_num <= 0 )
		return -1;
	// 主公不可跨区域收取图纸
	if ( map_zone_checksame( g_towninfo[townid].posx, g_towninfo[townid].posy, pCity->posx, pCity->posy ) == 0 )
	{
		actor_notify_alert( actor_index, 1337 );
		return -1;
	}
	// 消耗威望
	int dd = 1;
	int prestige = g_towninfo[townid].levy_prestige;
	if ( item_getitemnum( actor_index, g_towninfo[townid].levy_item ) > 0 )
	{
		prestige *= 2;
		dd = 2;
	}

	if ( pCity->prestige < prestige )
	{
		return -1;
	}

	// 扣威望
	city_changeprestige( pCity->index, -prestige, PATH_TOWNLEVY );
	// 扣强征令
	if ( dd == 1 )
		item_lost( actor_index, g_towninfo[townid].levy_item, 1, PATH_TOWNLEVY );

	// 总征收次数-1
	g_map_town[townid].produce_num -= 1;
	if ( g_map_town[townid].produce_num == 0 )
	{
		mapunit_update( MAPUNIT_TYPE_TOWN, townid, g_map_town[townid].unit_index );
	}

	// 检查上一级营地是否被攻破
	char occupy = 0;
	for ( int tmpi = 0; tmpi < 8; tmpi++ )
	{
		int pre_townid = g_map_town[townid].pre_townid[tmpi];
		if ( pre_townid <= 0 || pre_townid >= g_map_town_maxcount )
			continue;
		if ( g_map_town[pre_townid].nation > 0 )
		{
			occupy = 1;
			break;
		}
	}

	AwardGetInfo getinfo = { 0 };
	for ( int tmpi = 0; tmpi < dd; tmpi++ )
	{
		if ( occupy == 0 )
		{
			awardgroup_withindex( actor_index, g_towninfo[townid].base_award, 0, PATH_TOWNLEVY, &getinfo );
		}
		else
		{
			awardgroup_withindex( actor_index, g_towninfo[townid].other_award, 0, PATH_TOWNLEVY, &getinfo );
		}
	}
	map_town_sendinfo( actor_index, townid );

	char v1[64] = { 0 };
	char v2[64] = { 0 };
	char v3[64] = { 0 };
	char v4[64] = { 0 };
	char v5[64] = { 0 };
	char award_content[256] = { 0 };
	// 6005	玩家{3}在{0}的{1}<color=d95df4ff><url={2}>[{2}]</url></color>征收了{4}到自己的主公府中
	sprintf( v1, "%s%d", TAG_ZONEID, g_towninfo[townid].zoneid );
	sprintf( v2, "%s%d", TAG_TOWNID, townid );
	sprintf( v3, "%d,%d", g_towninfo[townid].posx, g_towninfo[townid].posy );
	sprintf( v4, "%s", pCity->name );
	awardgroup_makestr( &getinfo, award_content );
	sprintf( v5, "%s%s", TAG_AWARD, award_content );
	system_talkjson( 0, pCity->nation, 6005, v1, v2, v3, v4, v5, NULL, 1 );
	return 0;
}

// 城镇领主奖励
int map_town_owner_award_actor( int townid )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	// 检查上一级营地是否被攻破
	char occupy = 0;
	for ( int tmpi = 0; tmpi < 8; tmpi++ )
	{
		int pre_townid = g_map_town[townid].pre_townid[tmpi];
		if ( pre_townid <= 0 || pre_townid >= g_map_town_maxcount )
			continue;
		if ( g_map_town[pre_townid].nation > 0 )
		{
			occupy = 1;
			break;
		}
	}
	int awardgroup = 0;
	if ( occupy == 0 )
	{
		awardgroup = g_towninfo[townid].base_award;
	}
	else
	{
		awardgroup = g_towninfo[townid].other_award;
	}

	// 奖励，2个
	char attach[256] = { 0 };
	awardgroup_mail( awardgroup, 0, attach );
	awardgroup_mail( awardgroup, 0, attach );

	char v1[32] = { 0 };
	sprintf( v1, "%s%d", TAG_TOWNID, townid );

	char v2[32] = { 0 };
	sprintf( v2, "%d", global.town_owner_award );

	char v3[32] = { 0 };
	sprintf( v3, "%d", g_map_town[townid].own_sec/86400 );

	mail_system( MAIL_ACTORID, g_map_town[townid].own_actorid, 5031, 5527, v1, v2, v3, attach );
	return 0;
}
int map_town_owner_award()
{
	for ( int townid = 1; townid < g_map_town_maxcount; townid++ )
	{
		if ( g_towninfo[townid].id <= 0 )
			continue;
		if ( g_map_town[townid].own_actorid <= 0 )
			continue;
		map_town_owner_award_actor( townid );
	}
	return 0;
}

// 都城出动禁卫军功能启动
int map_town_attack_checkstart()
{
	int num = 0;
	for ( int townid = 301; townid <= 304; townid++ )
	{
		if ( g_map_town[townid].nation > 0 )
		{
			num += 1;
		}
	}
	if ( num >= 3 )
	{ // 满足3个都城都被占领
		char start = 0;
		for ( int townid = 301; townid <= 304; townid++ )
		{
			if ( g_map_town[townid].attackcd > 0 )
			{
				start = 1;
				break;
			}
		}
		if ( start == 1 )
		{
			return -1;
		}
		// 当前拥有名城最少的国家先发兵
		int xlist[4] = { 0 };
		int ylist[4] = { 0, 1, 2, 3 };
		for ( int nation = 0; nation < 3; nation++ )
		{
			xlist[nation] = nation_town_num( nation, MAPUNIT_TYPE_TOWN_TYPE7 );
		}
		for ( int tmpi = 0; tmpi < 4; tmpi++ )
		{
			for ( int tmpj = tmpi+1; tmpj < 4; tmpj++ )
			{
				if ( xlist[tmpj] < xlist[tmpi] )
				{
					int xtmp = xlist[tmpj];
					xlist[tmpj] = xlist[tmpi];
					xlist[tmpi] = xtmp;

					int ytmp = ylist[tmpj];
					ylist[tmpj] = ylist[tmpi];
					ylist[tmpi] = ytmp;
				}
			}
		}

		for ( int tmpi = 0; tmpi < 4; tmpi++ )
		{
			char nation = ylist[tmpi];
			int capital_townid = nation_capital_townid_get( nation );
			if ( capital_townid <= 0 || capital_townid >= g_map_town_maxcount )
				continue;
			if ( g_towninfo[capital_townid].type != MAPUNIT_TYPE_TOWN_TYPE8 )
				continue;
			g_map_town[capital_townid].attackcd = global.town_attackcd + (tmpi * 3600);
		}
	}
	return 0;
}

// 都城出动禁卫军
int map_town_attack( int townid )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	if ( g_towninfo[townid].type != MAPUNIT_TYPE_TOWN_TYPE8 )
		return -1;
	int to_townid = 0;

	// 优先选取
	int townlist[128] = { 0 };
	int towncount = 0;
	for ( int tmpi = 0; tmpi < 8; tmpi++ )
	{
		int id = g_map_town[townid].pre_townid[tmpi];
		if ( id <= 0 || id >= g_map_town_maxcount )
			continue;
		if ( g_map_town[id].protect_sec > 0 )
			continue;
		if ( g_map_town[townid].nation != g_map_town[id].nation )
		{
			townlist[towncount] = id;
			towncount += 1;
		}
	}
	if ( towncount > 0 )
	{
		int randint = rand() % towncount;
		to_townid = townlist[randint];
	}

	// 优先列表没有，那么随机选取
	if ( to_townid <= 0 )
	{
		for ( int id = 1; id < g_map_town_maxcount; id++ )
		{
			if ( g_towninfo[id].id <= 0 )
				continue;
			if ( g_towninfo[id].type != MAPUNIT_TYPE_TOWN_TYPE7 )
				continue;
			if ( g_map_town[id].nation == g_map_town[townid].nation )
				continue;
			if ( g_map_town[id].protect_sec > 0 )
				continue;
			townlist[towncount] = id;
			towncount += 1;
		}
	}
	if ( towncount > 0 )
	{
		int randint = rand() % towncount;
		to_townid = townlist[randint];
	}
	if ( to_townid <= 0 )
	{
		return -1;
	}

	// 创建国战
	int group_index = armygroup_nation_askcreate_ai( townid, to_townid );
	if ( group_index < 0 )
		return -1; 

	int level = g_map_town[townid].dev_level;
	if ( level < 0 )
		level = 0;
	else if ( level > 2 )
		level = 2;

	int totalsoldiers = 0;
	for ( int tmpi = 0; tmpi < 8; tmpi++ )
	{
		int monsterid = g_map_town_attackmonster[level][tmpi];
		if ( monsterid <= 0 || monsterid >= g_monster_maxnum )
			continue;
		MonsterInfo *pMonster = &g_monster[monsterid];
		if ( !pMonster )
			continue;
		totalsoldiers += pMonster->troops;
	}

	// 出征信息
	SLK_NetC_MapBattle info = { 0 };
	info.m_to_unit_type = MAPUNIT_TYPE_TOWN;
	info.m_to_unit_index = -1;
	info.m_id = g_armygroup[group_index].id;
	info.m_herokind[0] = 1000;
	info.m_to_posx = g_armygroup[group_index].to_posx;
	info.m_to_posy = g_armygroup[group_index].to_posy;
	//info.m_appdata
	info.m_action = ARMY_ACTION_NATION_ATTACK;
	info.m_group_index = group_index;

	// 创建部队
	int army_index = army_create( MAPUNIT_TYPE_TOWN, townid, MAPUNIT_TYPE_TOWN, to_townid, ARMY_STATE_MARCH, &info );
	if ( army_index < 0 )
		return -1;

	// 总兵力
	g_army[army_index].totals = totalsoldiers;

	// 发起集结,设置队长
	if ( group_index >= 0 && group_index < g_armygroup_maxcount )
	{
		g_army[army_index].group_index = group_index;
		g_army[army_index].group_id = g_armygroup[group_index].id;
		// 加入集结列表
		armygroup_addarmy( army_index );
	}
	return 0;
}

// 开发所需经验
int map_town_dev_expmax( int townid )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return 0;
	int devlevel = 0;
	for ( int lv = 2; lv >= 0; lv-- )
	{
		if ( g_map_town[townid].dev_exp >= global.town_dev_expmax[lv] )
		{
			devlevel = lv;
			break;
		}
	}

	int maxexp = 0;
	if ( devlevel >= 2 )
		maxexp = global.town_dev_expmax[2];
	else
		maxexp = global.town_dev_expmax[devlevel+1];

	return maxexp;
}

// 添加都城开发经验
int map_town_dev_addexp( int townid, int exp )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	g_map_town[townid].dev_exp += exp;
	
	int devlevel = 0;
	for ( int lv = 2; lv >= 0; lv-- )
	{
		if ( g_map_town[townid].dev_exp >= global.town_dev_expmax[lv] )
		{
			devlevel = lv;
			break;
		}
	}

	int maxexp = 0;
	if ( devlevel >= 2 )
		maxexp = global.town_dev_expmax[2];
	else
		maxexp = global.town_dev_expmax[devlevel + 1];

	if ( g_map_town[townid].dev_exp > maxexp )
	{
		g_map_town[townid].dev_exp = maxexp;
	}
	if ( g_map_town[townid].dev_level != devlevel )
	{
		g_map_town[townid].dev_level = devlevel;
		mapunit_update( MAPUNIT_TYPE_TOWN, townid, g_map_town[townid].unit_index );
	}
	nation_people_capital_set( g_map_town[townid].nation, g_map_town[townid].dev_exp );
 	return 0;
}

// 都城开发
int map_town_dev( int actor_index, int townid )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	if ( actor_check_uselimit_cd( actor_index, USELIMIT_CD_TOWN_DEV ) > 0 )
		return -1;
	if ( pCity->silver < global.town_dev_silver )
		return -1;
	if ( pCity->wood < global.town_dev_wood )
		return -1;
	city_changesilver( pCity->index, -global.town_dev_silver, PATH_TOWNDEV_CONTRIBUTE );
	city_changewood( pCity->index, -global.town_dev_wood, PATH_TOWNDEV_CONTRIBUTE );
	map_town_dev_addexp( townid, global.town_dev_contribute );
	actor_set_uselimit_cd( actor_index, USELIMIT_CD_TOWN_DEV, global.town_devcd );
	map_town_ex_sendinfo( actor_index, townid );
	return 0;
}

// 都城开发消除cd
int map_town_dev_delcd( int actor_index, int townid )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	int sec = actor_check_uselimit_cd( actor_index, USELIMIT_CD_TOWN_DEV );
	if ( sec <= 0 )
	{
		map_town_ex_sendinfo( actor_index, townid );
		return -1;
	}

	int token = (int)ceil( (sec / 60 + 1) * global.upgradequick_token );
	if ( actor_change_token( actor_index, -token, PATH_TOWNDEV_CONTRIBUTE, 0 ) < 0 )
		return -1;

	actor_clear_uselimit_cd( actor_index, USELIMIT_CD_TOWN_DEV );
	map_town_ex_sendinfo( actor_index, townid );
	return 0;
}

// 修改名称
int map_town_changename( int actor_index, int townid, char *pname )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	if ( !pname )
		return -1;
	
	// 权限不足


	int namelen = (int)strlen( pname );
	if ( namelen <= 0 || namelen >= NAME_SIZE )
		return -1;

	int costtype = 0;// 使用道具模式
	int itemkind = 172;
	int itemnum = item_getitemnum( actor_index, itemkind );
	int token = item_gettoken( itemkind );

	// 有道具使用道具，无道具使用钻石
	if ( itemnum == 0 )
	{
		if ( g_actors[actor_index].token < token * 2 )
		{
			return -1;
		}
		costtype = 1; // 使用钻石模式
	}
	else if ( itemnum == 1 )
	{
		if ( g_actors[actor_index].token < token )
		{
			return -1;
		}
		costtype = 2; // 使用钻石+1个道具模式
	}
	else
	{
		costtype = 3; // 使用道具模式
	}

	// 改名
	strncpy( g_map_town[townid].name, pname, NAME_SIZE );
	g_map_town[townid].name[NAME_SIZE - 1] = 0;

	if ( costtype == 1 )
	{
		actor_change_token( actor_index, -token * 2, PATH_CHANGENAME, 0 );
	}
	if ( costtype == 2 )
	{
		item_lost( actor_index, itemkind, 1, PATH_CHANGENAME );
		actor_change_token( actor_index, -token, PATH_CHANGENAME, 0 );
	}
	else
	{
		item_lost( actor_index, itemkind, 2, PATH_CHANGENAME );
	}

	// 通知到城外
	mapunit_update( MAPUNIT_TYPE_TOWN, townid, g_map_town[townid].unit_index );
	return 0;
}

// 获取野怪奖励
int map_town_sendaward( int actor_index, int townid )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	// 检查上一级营地是否被攻破
	char occupy = 0;
	for ( int tmpi = 0; tmpi < 8; tmpi++ )
	{
		int pre_townid = g_map_town[townid].pre_townid[tmpi];
		if ( pre_townid <= 0 || pre_townid >= g_map_town_maxcount )
			continue;
		if ( g_map_town[pre_townid].nation > 0 )
		{
			occupy = 1;
			break;
		}
	}

	if ( occupy == 0 )
	{
		awardgroup_sendinfo( actor_index, g_towninfo[townid].base_award, 2, townid, 16 );
	}
	else
	{
		awardgroup_sendinfo( actor_index, g_towninfo[townid].other_award, 2, townid, 16 );
	}
	return 0;
}

// 获取城镇信息
int map_town_sendinfo( int actor_index, int townid )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	SLK_NetS_MapTownInfo pValue = { 0 };
	pValue.m_protect_sec = g_map_town[townid].protect_sec;
	pValue.m_produce_sec = g_map_town[townid].produce_sec;
	pValue.m_produce_num = g_map_town[townid].produce_num;
	pValue.m_own_actorid = g_map_town[townid].own_actorid;
	if ( g_map_town[townid].own_actorid > 0 )
	{ // 已经有城主
		int city_index = g_map_town[townid].own_city_index;
		if ( city_index < 0 || city_index >= g_city_maxcount )
		{
			city_index = city_getindex_withactorid( g_map_town[townid].own_actorid );
			g_map_town[townid].own_city_index = city_index;
		}
		if ( city_index >= 0 && city_index < g_city_maxcount )
		{
			strncpy( pValue.m_own_name, g_city[city_index].name, NAME_SIZE );
			pValue.m_own_namelen = strlen( pValue.m_own_name );
			pValue.m_own_sec = g_map_town[townid].own_sec;
		}

		for ( int tmpi = 0; tmpi < MAP_TOWN_MONSTER_MAX; tmpi++ )
		{
			int monsterid = g_map_town[townid].monster[tmpi];
			if ( monsterid <= 0 || monsterid >= g_monster_maxnum )
				continue;
			pValue.m_maxhp += g_monster[monsterid].troops;
			pValue.m_hp += g_map_town[townid].soldier[tmpi];
		}
	}
	else
	{ // 没城主
		if ( g_map_town[townid].protect_sec > 0 )
		{ // 检查我是否在申请列表里
			for ( int tmpi = 0; tmpi < MAP_TOWN_JOIN_MAX; tmpi++ )
			{
				if ( g_map_town[townid].ask_actorid[tmpi] <= 0 )
					continue;
				if ( g_map_town[townid].ask_actorid[tmpi] == pCity->actorid )
				{
					pValue.m_myask = 1;
					break;
				}
			}
		}
	}

	netsend_maptowninfo_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 获取都城信息
int map_town_ex_sendinfo( int actor_index, int townid )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	SLK_NetS_MapTownExInfo pValue = { 0 };
	pValue.m_mytownid = nation_capital_townid_get( pCity->nation );
	pValue.m_dev_level = g_map_town[townid].dev_level;
	pValue.m_dev_exp = g_map_town[townid].dev_exp;
	pValue.m_dev_expmax = map_town_dev_expmax( townid );
	pValue.m_dev_cd = actor_check_uselimit_cd( actor_index, USELIMIT_CD_TOWN_DEV );
	pValue.m_attackcd = g_map_town[townid].attackcd;
	netsend_maptownexinfo_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 申请列表
int map_town_ask_owner_sendlist( int actor_index, int townid )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	SLK_NetS_TownOwnerAskList pValue = { 0 };
	pValue.m_sec = g_map_town[townid].protect_sec;

	for ( int tmpi = 0; tmpi < MAP_TOWN_JOIN_MAX; tmpi++ )
	{
		int city_index = g_map_town[townid].ask_city_index[tmpi];
		if ( city_index < 0 || city_index >= g_city_maxcount )
			continue;
		strncpy( pValue.m_list[pValue.m_count].m_name, g_city[city_index].name, NAME_SIZE );
		pValue.m_list[pValue.m_count].m_name_len = strlen( pValue.m_list[pValue.m_count].m_name );
		pValue.m_list[pValue.m_count].m_place = g_city[city_index].place;
		pValue.m_count += 1;
		if ( pValue.m_count >= 5 )
		{
			break;
		}
	}

	netsend_townowneraskList_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 城镇被攻击信息添加
int map_town_underfire_groupadd( int townid, int group_index )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	int index = -1;
	for ( int tmpi = 0; tmpi < MAP_TOWN_UNDERFIRE_GROUP_MAX; tmpi++ )
	{
		if ( g_map_town[townid].underfire_groupindex[tmpi] < 0 )
		{
			index = tmpi;
			break;
		}
	}
	if ( index < 0 )
		return -1;
	g_map_town[townid].underfire_groupindex[index] = group_index;
	g_map_town[townid].fightstate = 1;
	mapunit_update( MAPUNIT_TYPE_TOWN, townid, g_map_town[townid].unit_index );
	return 0;
}

// 城镇被攻击信息移除
int map_town_underfire_groupdel( int townid, int group_index )
{
	if ( townid <= 0 || townid >= g_map_town_maxcount )
		return -1;
	if ( group_index < 0 || group_index >= g_armygroup_maxcount )
		return -1;
	int tmpi = 0;
	for ( tmpi = 0; tmpi < MAP_TOWN_UNDERFIRE_GROUP_MAX; tmpi++ )
	{
		if ( g_map_town[townid].underfire_groupindex[tmpi] == group_index )
		{
			g_map_town[townid].underfire_groupindex[tmpi] = -1;
			break;
		}
	}

	// 国战状态
	char has = 0;
	for ( int i = 0; i < CITY_UNDERFIRE_GROUP_MAX; i++ )
	{
		if ( g_map_town[townid].underfire_groupindex[tmpi] >= 0 )
		{
			has = 1;
			break;
		}
	}
	if ( has == 0 )
	{
		g_map_town[townid].fightstate = 0;
	}

	mapunit_update( MAPUNIT_TYPE_TOWN, townid, g_map_town[townid].unit_index );
	return 0;
}
