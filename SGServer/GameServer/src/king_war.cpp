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
#include "world_quest.h"
#include "king_war.h"
#include "girl.h"
#include "auto_data_kingwar_towninfo.h"
#include "auto_data_map_towninfo.h"
#include "auto_data_kingwar.h"

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
extern int g_army_maxindex;
extern Fight g_fight;

extern KingwarTownInfo *g_kingwar_towninfo;
extern int g_kingwar_towninfo_maxnum;

extern MapTownInfo *g_towninfo;
extern int g_towninfo_maxnum;

extern MapTown *g_map_town;
extern int g_map_town_maxcount;

extern char g_open_zone_sili;
extern char g_open_zone_luoyang;

extern KingWarConfig *g_kingwar_config;
extern int g_kingwar_config_maxnum;

extern char g_test_mod;

KingwarTown *g_kingwar_town = NULL;
int g_kingwar_town_maxcount = 0;

int g_kingwar_lost_totalhp = 0; // 总损失兵力
char g_kingwar_level = 1;
int g_kingwar_activity_openweek = 0; // 活动首次启动周
int g_kingwar_activity_beginstamp = 0; // 活动开始时间戳
int g_kingwar_activity_endstamp = 0; // 活动结束时间戳
int g_kingwar_activity_duration = 0; // 活动持续时间
char g_kingwar_activity_open = 0; // 活动是否开启中
char g_kingwar_treasure_open = 0; // 挖宝活动开启中
int g_kingwar_treasure_endstamp = 0; // 挖宝结束时间戳
int g_kingwar_nextstamp = 0; // 下次开启的时间戳，固定7天以后
char g_kingwar_town_update = 0;
KingWarRank g_kingwar_rank[KINGWAR_RANK_MAX] = { 0 }; // 排行榜
short g_treasure_nationitem[3][KINGWAR_TREASURE_MAX] = { 0 }; // 宝图道具
char g_treasure_nationitem_count[3] = { 0 }; // 剩余数量
SLK_NetS_TreasureActor g_treasure_actor[3][KINGWAR_TREASURE_MAX] = { 0 };

// 读档完毕的回调
int kingwar_town_loadcb( int id )
{
	if ( id <= 0 || id >= g_kingwar_town_maxcount )
		return -1;
	return 0;
}

int kingwar_town_load()
{
	kingwar_activity_load();
	g_kingwar_town_maxcount = g_kingwar_towninfo_maxnum;
	g_kingwar_town = (KingwarTown*)malloc( sizeof( KingwarTown ) * g_kingwar_town_maxcount );
	memset( g_kingwar_town, 0, sizeof( KingwarTown ) * g_kingwar_town_maxcount );
	printf_msg( "KingWarTown  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_kingwar_town_maxcount, (sizeof( KingwarTown )*g_kingwar_town_maxcount) / 1024.0 / 1024.0, sizeof( KingwarTown ) / 1024.0 );
	for ( int id = 1; id < g_kingwar_town_maxcount; id++ )
	{
		if ( g_kingwar_towninfo[id].id <= 0 )
			continue;
		g_kingwar_town[id].id = id;
		g_kingwar_town[id].nation = (char)g_kingwar_towninfo[id].base_nation;
		for ( int tmpi = 0; tmpi < KINGWAR_TOWN_QUEUE_MAX; tmpi++ )
		{
			g_kingwar_town[id].attack_queue[tmpi] = -1;
			g_kingwar_town[id].defense_queue[tmpi] = -1;
		}
	}
	kingwar_town_load_auto( kingwar_town_getptr, kingwar_town_loadcb, "kingwar_town" );
	return 0;
}

KingwarTown *kingwar_town_getptr( int id )
{
	if ( id <= 0 || id >= g_kingwar_town_maxcount )
		return NULL;
	return &g_kingwar_town[id];
}

KingwarTownInfo *kingwar_town_getconfig( int id )
{
	if ( id <= 0 || id >= g_kingwar_town_maxcount )
		return NULL;
	return &g_kingwar_towninfo[id];
}

int kingwar_town_save( FILE *fp )
{
	kingwar_town_batch_save_auto( g_kingwar_town, g_kingwar_town_maxcount, "kingwar_town", fp );
	kingwar_activity_save( fp );
	return 0;
}

char kingwar_town_attack_or_defense( int id, int army_index )
{
	if ( id <= 0 || id >= g_kingwar_town_maxcount )
		return -1;
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	if ( g_kingwar_town[id].nation == army_getnation( army_index ) )
	{
		return 2;
	}
	return 1;
}

int kingwar_town_queueadd( int id, char attack, int army_index )
{
	if ( id <= 0 || id >= g_kingwar_town_maxcount )
		return -1;
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	if ( attack == KINGWAR_TOWN_ATTACK )
	{ // 添加到攻击方
		for ( int tmpi = 0; tmpi < KINGWAR_TOWN_QUEUE_MAX; tmpi++ )
		{
			if ( g_kingwar_town[id].attack_queue[tmpi] < 0 )
			{
				g_kingwar_town[id].attack_queue[tmpi] = army_index;
				g_kingwar_town[id].attack_total += g_army[army_index].totals;
				g_army[army_index].to_id = id;
				g_army[army_index].to_index = id;
				g_army[army_index].state = ARMY_STATE_KINGWAR_FIGHT;
				g_army[army_index].move_total_distance = tmpi;
				break;
			}
		}
	}
	else if ( attack == KINGWAR_TOWN_DEFENSE )
	{ // 添加到防御方
		for ( int tmpi = 0; tmpi < KINGWAR_TOWN_QUEUE_MAX; tmpi++ )
		{
			if ( g_kingwar_town[id].defense_queue[tmpi] < 0 )
			{
				g_kingwar_town[id].defense_queue[tmpi] = army_index;
				g_kingwar_town[id].defense_total += g_army[army_index].totals;
				g_army[army_index].to_id = id;
				g_army[army_index].to_index = id;
				g_army[army_index].state = ARMY_STATE_KINGWAR_FIGHT;
				g_army[army_index].move_total_distance = tmpi;
				break;
			}
		}
	}
	return 0;
}

// 删除一个待战部队
int kingwar_town_queuedel( int id, char attack, int army_index )
{
	if ( id <= 0 || id >= g_kingwar_town_maxcount )
		return -1;
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	char isdelete = 0;
	if ( attack == KINGWAR_TOWN_ATTACK )
	{
		for ( int tmpi = 0; tmpi < KINGWAR_TOWN_QUEUE_MAX; tmpi++ )
		{
			if ( g_kingwar_town[id].attack_queue[tmpi] == army_index )
			{
				g_kingwar_town[id].attack_queue[tmpi] = -1;
				if ( tmpi < KINGWAR_TOWN_QUEUE_MAX - 1 )
				{
					memmove( &g_kingwar_town[id].attack_queue[tmpi], &g_kingwar_town[id].attack_queue[tmpi + 1], sizeof(int) *(KINGWAR_TOWN_QUEUE_MAX - 1 - tmpi) );
					g_kingwar_town[id].attack_queue[KINGWAR_TOWN_QUEUE_MAX - 1] = -1;
				}
				isdelete = 1;
				break;
			}
		}
	}
	else if ( attack == KINGWAR_TOWN_DEFENSE )
	{
		for ( int tmpi = 0; tmpi < KINGWAR_TOWN_QUEUE_MAX; tmpi++ )
		{
			if ( g_kingwar_town[id].defense_queue[tmpi] == army_index )
			{
				g_kingwar_town[id].defense_queue[tmpi] = -1;
				if ( tmpi < KINGWAR_TOWN_QUEUE_MAX - 1 )
				{
					memmove( &g_kingwar_town[id].defense_queue[tmpi], &g_kingwar_town[id].defense_queue[tmpi + 1], sizeof( int ) *(KINGWAR_TOWN_QUEUE_MAX - 1 - tmpi) );
					g_kingwar_town[id].defense_queue[KINGWAR_TOWN_QUEUE_MAX - 1] = -1;
				}
				isdelete = 1;
				break;
			}
		}
	}
	// 安全处理 
	if ( isdelete == 0 )
	{
		if ( attack == KINGWAR_TOWN_ATTACK )
		{
			for ( int tmpi = 0; tmpi < KINGWAR_TOWN_QUEUE_MAX; tmpi++ )
			{
				if ( g_kingwar_town[id].defense_queue[tmpi] == army_index )
				{
					g_kingwar_town[id].defense_queue[tmpi] = -1;
					if ( tmpi < KINGWAR_TOWN_QUEUE_MAX - 1 )
					{
						memmove( &g_kingwar_town[id].defense_queue[tmpi], &g_kingwar_town[id].defense_queue[tmpi + 1], sizeof( int ) *(KINGWAR_TOWN_QUEUE_MAX - 1 - tmpi) );
						g_kingwar_town[id].defense_queue[KINGWAR_TOWN_QUEUE_MAX - 1] = -1;
					}
					break;
				}
			}
		}
		else if( attack == KINGWAR_TOWN_DEFENSE )
		{
			for ( int tmpi = 0; tmpi < KINGWAR_TOWN_QUEUE_MAX; tmpi++ )
			{
				if ( g_kingwar_town[id].attack_queue[tmpi] == army_index )
				{
					g_kingwar_town[id].attack_queue[tmpi] = -1;
					if ( tmpi < KINGWAR_TOWN_QUEUE_MAX - 1 )
					{
						memmove( &g_kingwar_town[id].attack_queue[tmpi], &g_kingwar_town[id].attack_queue[tmpi + 1], sizeof( int ) *(KINGWAR_TOWN_QUEUE_MAX - 1 - tmpi) );
						g_kingwar_town[id].attack_queue[KINGWAR_TOWN_QUEUE_MAX - 1] = -1;
					}
					break;
				}
			}
		}
	}
	kingwar_town_totalcalc( id, attack );
	return 0;
}

// 获取队列头内容
int kingwar_town_queueget( int id, char attack )
{
	if ( id <= 0 || id >= g_kingwar_town_maxcount )
		return -1;
	int army_index = -1;
	if ( attack == KINGWAR_TOWN_ATTACK )
	{
		army_index = g_kingwar_town[id].attack_queue[0];
	}
	else if ( attack == KINGWAR_TOWN_DEFENSE )
	{
		// 从队列中取出一项
		army_index = g_kingwar_town[id].defense_queue[0];
	}
	return army_index;
}

// 从新计算兵力
int kingwar_town_totalcalc( int id, char attack )
{
	if ( id <= 0 || id >= g_kingwar_town_maxcount )
		return -1;
	if ( attack == KINGWAR_TOWN_ATTACK )
	{ // 攻击方
		g_kingwar_town[id].attack_total = 0;
		for ( int tmpi = 0; tmpi < KINGWAR_TOWN_QUEUE_MAX; tmpi++ )
		{
			int army_index = g_kingwar_town[id].attack_queue[tmpi];
			if ( army_index < 0 || army_index >= g_army_maxcount )
				break;
			g_kingwar_town[id].attack_total += g_army[army_index].totals;
		}
	}
	else if ( attack == KINGWAR_TOWN_DEFENSE )
	{ // 防御方
		g_kingwar_town[id].defense_total = 0;
		for ( int tmpi = 0; tmpi < KINGWAR_TOWN_QUEUE_MAX; tmpi++ )
		{
			int army_index = g_kingwar_town[id].defense_queue[tmpi];
			if ( army_index < 0 || army_index >= g_army_maxcount )
				break;
			g_kingwar_town[id].defense_total += g_army[army_index].totals;
		}
	}
	return 0;
}

// 改变国家
int kingwar_town_change_nation( int id, int nation, int attack_army_index )
{
	if ( id <= 0 || id >= g_kingwar_town_maxcount )
		return -1;
	if ( g_kingwar_town[id].nation == nation )
		return -1;
	if ( g_kingwar_town[id].defense_queue[0] >= 0 )
		return -1;
	char oldnation = g_kingwar_town[id].nation;
	g_kingwar_town[id].nation = nation;
	g_kingwar_town[id].attack_total = 0;
	g_kingwar_town[id].defense_total = 0;

	int temp[KINGWAR_TOWN_QUEUE_MAX];
	int temp_total = 0;
	int temp_index = 0;
	for ( int tmpi = 0; tmpi < KINGWAR_TOWN_QUEUE_MAX; tmpi++ )
	{
		temp[tmpi] = -1;
	}

	// 把攻击队列中属于这个国家的挑出来,放到防御队列里
	int index = 0;
	for ( int tmpi = 0; tmpi < KINGWAR_TOWN_QUEUE_MAX; tmpi++ )
	{
		int army_index = g_kingwar_town[id].attack_queue[tmpi];
		if ( army_index < 0 || army_index >= g_army_maxcount )
			continue;
		if ( army_getnation( army_index ) == nation )
		{
			g_kingwar_town[id].defense_queue[index] = army_index;
			g_kingwar_town[id].defense_total += g_army[army_index].totals;
			index += 1;
		}
		else
		{
			temp[temp_index] = army_index;
			temp_total += g_army[army_index].totals;
			temp_index += 1;
		}
		g_kingwar_town[id].attack_queue[tmpi] = -1;
	}


	memcpy( g_kingwar_town[id].attack_queue, temp, sizeof( int )*KINGWAR_TOWN_QUEUE_MAX );
	g_kingwar_town[id].attack_total = temp_total;

	// 据点发生变更，那么失败方的救援武将需要移动到集结点等待救援
	for ( int army_index = 0; army_index < g_army_maxindex/*注意：使用索引位置，为了效率*/; army_index++ )
	{
		if ( g_army[army_index].state == ARMY_STATE_KINGWAR_WAITSOS && g_army[army_index].to_id == id )
		{
			char army_nation = army_getnation( army_index );
			if ( army_nation != nation )
			{
				g_army[army_index].to_id = army_nation;
				g_army[army_index].to_index = army_nation;
				city_battlequeue_sendupdate( army_index );
			}
		}
	}

	if ( id == 7 )
	{ // 洛阳攻防交替，增加时间
		if ( g_kingwar_activity_duration < global.kingwar_activity_duration_max )
		{
			g_kingwar_activity_duration += global.kingwar_activity_duration;
			g_kingwar_activity_endstamp += global.kingwar_activity_duration;
		}
	}
	// {0}在皇城血战中奋勇当先，身先士卒，终不负众望攻破{1}{2}据点
	if ( attack_army_index >= 0 && attack_army_index < g_army_maxcount )
	{
		char v1[32] = { 0 };
		char v2[32] = { 0 };
		char v3[32] = { 0 };
		City *pCity = army_getcityptr( attack_army_index );
		if ( pCity )
			sprintf( v1, "%s", pCity->name );
		sprintf( v2, "%s%d", TAG_NATION, oldnation );
		sprintf( v3, "%s%d", TAG_KINGWAR, id );
		system_talkjson_world( 6010, v1, v2, v3, NULL, NULL, NULL, 1 );
	}
	g_kingwar_town_update = 1;
	return 0;
}

void kingwar_town_fight( int id )
{
	if ( g_test_mod == 1 )
	{
		return;
	}
	if ( id <= 0 || id >= g_kingwar_town_maxcount )
		return;
	int attack_army_index = -1;
	int defense_army_index = -1;

	// 获取攻击方
	attack_army_index = kingwar_town_queueget( id, KINGWAR_TOWN_ATTACK );
	if ( attack_army_index < 0 )
		return;

	// 获取防御方
	defense_army_index = kingwar_town_queueget( id, KINGWAR_TOWN_DEFENSE );
	if ( defense_army_index < 0 )
	{
		// 防御方没人了，变成攻击方的
		kingwar_town_change_nation( id, army_getnation( attack_army_index ), attack_army_index );
		return;
	}

	// 双方都有人，战斗
	if ( fight_start( attack_army_index, MAPUNIT_TYPE_KINGWAR_TOWN, defense_army_index ) < 0 )
	{
		return;
	}

	// 战斗结算
	fight_lost_calc();

	// 总损失兵力
	g_kingwar_lost_totalhp += g_fight.attack_total_damage;
	g_kingwar_lost_totalhp += g_fight.defense_total_damage;

	if ( g_fight.result == FIGHT_WIN )
	{ // 进攻方胜利
		kingwar_town_totalcalc( id, KINGWAR_TOWN_ATTACK );
		// 删除一个防守方队列
		kingwar_town_queuedel( id, KINGWAR_TOWN_DEFENSE, defense_army_index );
		g_army[defense_army_index].state = ARMY_STATE_KINGWAR_WAITSOS;
		g_army[defense_army_index].statetime = 0;
		g_army[defense_army_index].stateduration = global.kingwar_dead_cd;
	}
	else
	{ // 防御方胜利
		kingwar_town_totalcalc( id, KINGWAR_TOWN_DEFENSE );
		// 删除一个攻击方队列
		kingwar_town_queuedel( id, KINGWAR_TOWN_ATTACK, attack_army_index );
		g_army[attack_army_index].state = ARMY_STATE_KINGWAR_WAITSOS;
		g_army[attack_army_index].statetime = 0;
		g_army[attack_army_index].stateduration = global.kingwar_dead_cd;
	}
	city_battlequeue_sendupdate( attack_army_index );
	city_battlequeue_sendupdate( defense_army_index );
	g_kingwar_town_update = 1;

	// 战斗通知
	SLK_NetS_KingWarNotify pValue = { 0 }; 
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
	netsend_kingwarnotify_S( SUBSCRIBE_CMD_KINGWARDLG, SENDTYPE_SUBSCRIBE_NATION + pValue.m_d_nation, &pValue );
}

void kingwar_town_logic()
{
	for ( int id = 4; id < g_kingwar_town_maxcount; id++ )
	{
		kingwar_town_fight( id );
	}
}

// 活动信息
int kingwar_activity_load()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];

	// 读取存档
	sprintf( szSQL, "select openweek,open,beginstamp,endstamp,duration,lost_totalhp,level,treasure_open,treasure_endstamp,treasure_nation1,treasure_nation2,treasure_nation3,nextstamp from kingwar_activity;" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( (row = mysql_fetch_row( res )) )
	{
		g_kingwar_activity_openweek = atoi( row[0] );
		g_kingwar_activity_open = atoi( row[1] );
		g_kingwar_activity_beginstamp = atoi( row[2] );
		g_kingwar_activity_endstamp = atoi( row[3] );
		g_kingwar_activity_duration = atoi( row[4] );
		g_kingwar_lost_totalhp = atoi( row[5] );
		g_kingwar_level = atoi( row[6] );
		g_kingwar_treasure_open = atoi( row[7] );
		g_kingwar_treasure_endstamp = atoi( row[8] );
		memcpy( g_treasure_nationitem[0], row[9], sizeof( short ) * KINGWAR_TREASURE_MAX );
		memcpy( g_treasure_nationitem[1], row[10], sizeof( short ) * KINGWAR_TREASURE_MAX );
		memcpy( g_treasure_nationitem[2], row[11], sizeof( short ) * KINGWAR_TREASURE_MAX );
		for ( int nation = 0; nation < 3; nation++ )
		{
			for ( int tmpi = 0; tmpi < KINGWAR_TREASURE_MAX; tmpi++ )
			{
				if ( g_treasure_nationitem[nation][tmpi] <= 0 )
					break;
				g_treasure_nationitem_count[nation] += 1;
			}
		}
		g_kingwar_nextstamp = atoi( row[12] );

		if ( g_kingwar_nextstamp > 0 )
		{
			g_kingwar_activity_openweek = 1;
		}
	}
	mysql_free_result( res );

	// 读取排行榜存档
	sprintf( szSQL, "select `rank`, `actorid`, `herokind`, `kill` from kingwar_rank;" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( (row = mysql_fetch_row( res )) )
	{
		int rank = atoi( row[0] ) - 1;
		if ( rank < 0 || rank >= KINGWAR_RANK_MAX )
			continue;
		g_kingwar_rank[rank].actorid = atoi( row[1] );
		g_kingwar_rank[rank].info.m_herokind = atoi( row[2] );
		g_kingwar_rank[rank].info.m_kill = atoi( row[3] );

		City *pCity = city_getptr_withactorid( g_kingwar_rank[rank].actorid );
		if ( pCity )
		{
			g_kingwar_rank[rank].city_index = pCity->index;
			g_kingwar_rank[rank].info.m_herocolor = hero_getcolor( pCity, g_kingwar_rank[rank].info.m_herokind );
			g_kingwar_rank[rank].info.m_nation = pCity->nation;
			strncpy( g_kingwar_rank[rank].info.m_name, pCity->name, NAME_SIZE );
			g_kingwar_rank[rank].info.m_name_len = strlen( g_kingwar_rank[rank].info.m_name );
		}

	}
	mysql_free_result( res );

	// 读取挖取红图玩家
	sprintf( szSQL, "select `nation`, `offset`, `itemkind`, `name` from kingwar_treasure;" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( (row = mysql_fetch_row( res )) )
	{
		char nation = atoi( row[0] );
		if ( nation < 0 || nation > 2 )
			continue;
		char offset = atoi( row[1] );
		if ( offset < 0 || offset >= KINGWAR_TREASURE_MAX )
			continue;
		g_treasure_actor[nation][offset].m_itemkind = atoi( row[2] );
		strncpy( g_treasure_actor[nation][offset].m_name, row[3], NAME_SIZE );
		g_treasure_actor[nation][offset].m_name_len = strlen( g_treasure_actor[nation][offset].m_name );
	}
	mysql_free_result( res );
	return 0;
}
int kingwar_activity_save( FILE *fp )
{
	char szSQL[1024];
	char nationitem1[sizeof( short ) * KINGWAR_TREASURE_MAX * 2 + 1] = { 0 };
	char nationitem2[sizeof( short ) * KINGWAR_TREASURE_MAX * 2 + 1] = { 0 };
	char nationitem3[sizeof( short ) * KINGWAR_TREASURE_MAX * 2 + 1] = { 0 };

	// 保存活动估计出数据
	sprintf( szSQL, "replace into kingwar_activity ( id,openweek,open,beginstamp,endstamp,duration,lost_totalhp,level,treasure_open,treasure_endstamp,treasure_nation1,treasure_nation2,treasure_nation3,nextstamp ) values('1','%d','%d','%d','%d','%d','%d','%d','%d','%d','%s','%s','%s','%d');",
		g_kingwar_activity_openweek,g_kingwar_activity_open, g_kingwar_activity_beginstamp, g_kingwar_activity_endstamp, g_kingwar_activity_duration, g_kingwar_lost_totalhp, g_kingwar_level, g_kingwar_treasure_open, g_kingwar_treasure_endstamp,
		db_escape( (const char *)g_treasure_nationitem[0], nationitem1, sizeof( short ) * KINGWAR_TREASURE_MAX ),
		db_escape( (const char *)g_treasure_nationitem[1], nationitem2, sizeof( short ) * KINGWAR_TREASURE_MAX ),
		db_escape( (const char *)g_treasure_nationitem[2], nationitem3, sizeof( short ) * KINGWAR_TREASURE_MAX ),
		g_kingwar_nextstamp );
	if ( fp )
	{
		fprintf( fp, "%s\n", szSQL );
	}
	else if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}

	// 保存排行榜数据
	for ( int tmpi = 0; tmpi < KINGWAR_RANK_MAX; tmpi++ )
	{
		sprintf( szSQL, "replace into kingwar_rank ( `rank`,`actorid`,`herokind`,`kill` ) values('%d','%d','%d','%d');",
			tmpi + 1, g_kingwar_rank[tmpi].actorid, g_kingwar_rank[tmpi].info.m_herokind, g_kingwar_rank[tmpi].info.m_kill );
		if ( fp )
		{
			fprintf( fp, "%s\n", szSQL );
		}
		else if ( mysql_query( myGame, szSQL ) )
		{
			printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
			write_gamelog( "%s", szSQL );
			if ( mysql_ping( myGame ) != 0 )
				db_reconnect_game();
			continue;
		}
	}

	// 保存挖红图数据
	char szText_newname[NAME_SIZE * 2 + 1] = { 0 };
	for ( int nation = 0; nation < 3; nation++ )
	{
		for ( int offset = 0; offset < KINGWAR_TREASURE_MAX; offset++ )
		{
			if ( g_treasure_actor[nation][offset].m_itemkind <= 0 )
				continue;
			szText_newname[0] = 0;
			db_escape( (const char *)g_treasure_actor[nation][offset].m_name, szText_newname, 0 );
			sprintf( szSQL, "replace into kingwar_treasure ( `nation`, `offset`, `itemkind`, `name` ) values('%d','%d','%d','%s');",
				nation, offset, g_treasure_actor[nation][offset].m_itemkind, szText_newname );
			if ( fp )
			{
				fprintf( fp, "%s\n", szSQL );
			}
			else if ( mysql_query( myGame, szSQL ) )
			{
				printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
				write_gamelog( "%s", szSQL );
				if ( mysql_ping( myGame ) != 0 )
					db_reconnect_game();
				continue;
			}
		}
	}
	return 0;
}

// 活动逻辑，每秒
void kingwar_activity_logic()
{
return;
	if ( g_open_zone_luoyang == 0 )
	{ // 世界任务董卓
		return;
	}

	int nowstamp = (int)time( NULL );
	time_t t;
	time( &t );
	struct tm *nowtime = localtime( &t );
	if ( nowtime->tm_wday == global.kingwar_activity_week )
	{
		int weeknum = system_getfweek();
		if ( weeknum >= g_kingwar_activity_openweek )
		{
			// 活动开始
			if ( nowtime->tm_hour == global.kingwar_activity_hour && nowtime->tm_min == global.kingwar_activity_minute && nowtime->tm_sec == 0 )
			{
				kingwar_activity_onopen();
				return;
			}

			// 活动结束
			if ( g_kingwar_activity_open == 1 && nowstamp >= g_kingwar_activity_endstamp )
			{
				kingwar_activity_onclose();
				return;
			}

			// 在活动中
			if ( nowstamp >= g_kingwar_activity_beginstamp && nowstamp <= g_kingwar_activity_endstamp )
			{
				// 城镇逻辑
				kingwar_town_logic();
				// 每秒发送一次
				if ( g_kingwar_town_update == 1 )
				{
					kingwar_town_sendall();
					g_kingwar_town_update = 0;
				}
			}
		}
	}

	// 检查挖宝活动
	if ( g_kingwar_treasure_open == 1 && nowstamp >= g_kingwar_treasure_endstamp )
	{
		kingwar_treasure_onclose();
	}
}

// 活动启动
int kingwar_activity_open()
{
	//int week = system_getweek();
	//if ( week == 6 && week == 0 )
	//{ // 星期六星期日不用隔周了
	//	g_kingwar_activity_openweek = system_getfweek();
	//}
	//else
	//{
	//	//g_kingwar_activity_openweek = system_getfweek() + 1;
	//	g_kingwar_activity_openweek = system_getfweek();
	//}
	return 0;
}

// 活动开启
int kingwar_activity_onopen()
{
	g_kingwar_activity_open = 1;
	g_kingwar_activity_beginstamp = (int)time( NULL ); // 活动刚开始时间戳
	g_kingwar_activity_endstamp = g_kingwar_activity_beginstamp + global.kingwar_activity_duration; // 活动结束时间戳
	g_kingwar_activity_duration = global.kingwar_activity_duration; // 活动持续时间
	g_kingwar_lost_totalhp = 0; // 总损失兵力
	g_kingwar_treasure_open = 0; // 挖宝活动
	g_kingwar_nextstamp = g_kingwar_activity_beginstamp + 86400 * 7;

	// 据点初始化
	for ( int id = 1; id < g_kingwar_town_maxcount; id++ )
	{
		if ( g_kingwar_towninfo[id].id <= 0 )
			continue;
		if ( id == 7 )
		{
			g_kingwar_town[id].nation = (char)map_town_getnation( MAPUNIT_KING_TOWNID );
		}
		else
		{
			g_kingwar_town[id].nation = (char)g_kingwar_towninfo[id].base_nation;
		}
		for ( int tmpi = 0; tmpi < KINGWAR_TOWN_QUEUE_MAX; tmpi++ )
		{
			g_kingwar_town[id].attack_queue[tmpi] = -1;
			g_kingwar_town[id].defense_queue[tmpi] = -1;
		}
	}
	// 玩家初始化
	for ( int city_index = 0; city_index < g_city_maxindex/*注意：使用索引位置，为了效率*/; city_index++ )
	{
		if ( g_city[city_index].actorid <= 0 )
			continue;
		g_city[city_index].kw_totalkill = 0;
		g_city[city_index].kw_has = 0;
		g_city[city_index].kw_px = 0;
		g_city[city_index].kw_py = 0;
		g_city[city_index].kw_tn = 0;
		g_city[city_index].kw_co = 0;
	}
	// 排行榜初始化
	memset( g_kingwar_rank, 0, sizeof( KingWarRank )*KINGWAR_RANK_MAX );
	for ( int tmpi = 0; tmpi < KINGWAR_RANK_MAX; tmpi++ )
	{
		g_kingwar_rank[tmpi].city_index = -1;
	}
	// 图纸初始化
	for ( int nation = 0; nation < 3; nation++ )
	{
		for ( int tmpi = 0; tmpi < KINGWAR_TREASURE_MAX; tmpi++ )
		{
			g_treasure_nationitem[nation][tmpi] = 0;
		}
		g_treasure_nationitem_count[nation] = 0;
		memset( g_treasure_actor[nation], 0, sizeof( SLK_NetS_TreasureActor ) * KINGWAR_TREASURE_MAX );
	}
	kingwar_activity_sendinfo( -1 );
	system_talkjson_world( 6008, NULL, NULL, NULL, NULL, NULL, NULL, 1 );
	return 0;
}

// 活动结束
int kingwar_activity_onclose()
{
	g_kingwar_activity_open = 0;
	kingwar_activity_sendinfo( -1 );

	// 所有活动部队解散
	for ( int army_index = 0; army_index < g_army_maxindex/*注意：使用索引位置，为了效率*/; army_index++ )
	{
		if ( g_army[army_index].action == ARMY_ACTION_KINGWAR )
		{
			g_army[army_index].state = ARMY_STATE_KINGWAR_READY;
			army_delete( army_index );
		}
	}

	// 根据损失兵力计算血战等级
	int level = 1;
	for ( level = 1; level < g_kingwar_config_maxnum; level++ )
	{
		if ( g_kingwar_lost_totalhp < g_kingwar_config[level].exp )
		{
			break;
		}
	}
	g_kingwar_level = level;

	// 挖宝图纸列表
	for ( int nation = 0; nation < 3; nation++ )
	{
		g_treasure_nationitem[nation][0] = g_kingwar_config[1].exchange_item;
		g_treasure_nationitem[nation][1] = g_kingwar_config[2].exchange_item;
		g_treasure_nationitem[nation][2] = g_kingwar_config[3].exchange_item;
		g_treasure_nationitem[nation][3] = g_kingwar_config[4].exchange_item;
		g_treasure_nationitem[nation][4] = g_kingwar_config[5].exchange_item;
		g_treasure_nationitem[nation][5] = g_kingwar_config[6].exchange_item;
		if ( level == 1 )
		{
			ruffle_short( &g_treasure_nationitem[nation][0], 6 );
			g_treasure_nationitem_count[nation] = 6;
		}
		if ( level == 2 )
		{
			g_treasure_nationitem[nation][6] = g_kingwar_config[1].drawing_other;
			g_treasure_nationitem[nation][7] = g_kingwar_config[2].drawing_other;
			g_treasure_nationitem[nation][8] = g_kingwar_config[3].drawing_other;
			ruffle_short( &g_treasure_nationitem[nation][0], 9 );
			g_treasure_nationitem_count[nation] = 9;
		}
		else if( level > 2 )
		{
			g_treasure_nationitem[nation][6] = g_kingwar_config[1].exchange_item;
			g_treasure_nationitem[nation][7] = g_kingwar_config[2].exchange_item;
			g_treasure_nationitem[nation][8] = g_kingwar_config[3].exchange_item;
			g_treasure_nationitem[nation][9] = g_kingwar_config[4].exchange_item;
			g_treasure_nationitem[nation][10] = g_kingwar_config[5].exchange_item;
			g_treasure_nationitem[nation][11] = g_kingwar_config[6].exchange_item;
			ruffle_short( &g_treasure_nationitem[nation][0], 12 );
			g_treasure_nationitem_count[nation] = 12;
		}
	}

	g_map_town[MAPUNIT_KING_TOWNID].nation = g_kingwar_town[7].nation;
	// 城镇辐射圈归属
	map_tile_setnation( g_towninfo[MAPUNIT_KING_TOWNID].posx, g_towninfo[MAPUNIT_KING_TOWNID].posy, g_towninfo[MAPUNIT_KING_TOWNID].range, MAPUNIT_KING_TOWNID, g_map_town[MAPUNIT_KING_TOWNID].nation );
	// 核心建筑决定地区归属
	map_zone_setnation( map_zone_getid( g_towninfo[MAPUNIT_KING_TOWNID].posx, g_towninfo[MAPUNIT_KING_TOWNID].posy ), g_map_town[MAPUNIT_KING_TOWNID].nation );
	// 地区皇城区域都城和名城更新
	map_zone_center_townchange( MAPUNIT_KING_TOWNID );
	// 城镇更新
	mapunit_update( MAPUNIT_TYPE_TOWN, MAPUNIT_KING_TOWNID, g_map_town[MAPUNIT_KING_TOWNID].unit_index );
	// {0}在血战皇城中获胜，成功占领了洛阳城
	char v1[32] = { 0 };
	sprintf( v1, "%s%d", TAG_NATION, g_map_town[MAPUNIT_KING_TOWNID].nation );
	system_talkjson_world( 6009, v1, NULL, NULL, NULL, NULL, NULL, 1 );

	if ( g_map_town[MAPUNIT_KING_TOWNID].nation > 0 )
	{
		// 邮件
		// 5530	本届Lv.{0}皇城血战圆满结束，[{1}]登顶皇城。战胜国战神{2}携带金色图纸消息而来
		char v1[32] = { 0 };
		char v2[32] = { 0 };
		char v3[32] = { 0 };
		sprintf( v1, "%d", g_kingwar_level );
		sprintf( v2, "%s%d", TAG_NATION, g_map_town[MAPUNIT_KING_TOWNID].nation );
		if ( g_map_town[MAPUNIT_KING_TOWNID].nation == 1 )
			sprintf( v3, "%s%d", TAG_HERO, 92 );
		else if ( g_map_town[MAPUNIT_KING_TOWNID].nation == 2 )
			sprintf( v3, "%s%d", TAG_HERO, 118 );
		else
			sprintf( v3, "%s%d", TAG_HERO, 119 );
		mail_sendall( 5034, 5530, v1, v2, v3, "" );

		// 排行榜积分
		for ( int tmpi = 0; tmpi < KINGWAR_RANK_MAX; tmpi++ )
		{
			if ( g_kingwar_rank[tmpi].actorid <= 0 )
				continue;
			city_kingwarpoint( g_kingwar_rank[tmpi].city_index, g_kingwar_config[tmpi + 1].rank_award_point, PATH_KINGWAR_RANK );
		}

		// 累计杀敌积分
		for ( int city_index = 0; city_index < g_city_maxindex/*注意：使用索引位置，为了效率*/; city_index++ )
		{
			if ( g_city[city_index].actorid <= 0 )
				continue;
			for ( int i = g_kingwar_config_maxnum - 1; i >= 0; i-- )
			{
				if ( g_city[city_index].kw_totalkill >= g_kingwar_config[i].totalkill )
				{
					if ( g_city[city_index].kw_totalkill > 0 && g_kingwar_config[i].totalkill_award_point == 0 )
						city_kingwarpoint( city_index, 200, PATH_KINGWAR_KILL );
					else
						city_kingwarpoint( city_index, g_kingwar_config[i].totalkill_award_point, PATH_KINGWAR_KILL );
					break;
				}
			}
		}

		// 开启挖宝活动
		kingwar_treasure_onopen();
	}
	return 0;
}

// 在活动中
int kingwar_activity_inttime()
{
	time_t t;
	time( &t );
	struct tm *nowtime = localtime( &t );
	if ( nowtime->tm_wday == global.kingwar_activity_week )
	{
		int nowstamp = (int)time( NULL );
		if ( nowstamp >= g_kingwar_activity_beginstamp && nowstamp <= g_kingwar_activity_endstamp )
		{
			return 1;
		}
	}
	return 0;
}

// 活动信息
int kingwar_activity_sendinfo( int actor_index )
{
	// 世界任务董卓
	if ( g_open_zone_luoyang == 0 )
		return -1;

	time_t t;
	time( &t );
	struct tm *nowtime = localtime( &t );
	if ( nowtime->tm_wday != global.kingwar_activity_week )
		return -1;

	SLK_NetS_KingWarActivity pValue = { 0 };
	time_t activity_t;
	time( &activity_t );
	struct tm *activitytime = localtime( &activity_t );
	// 重新活动开始时间戳
	struct tm BeginTm = { 0 };
	BeginTm.tm_year = nowtime->tm_year;
	BeginTm.tm_mon = nowtime->tm_mon;
	BeginTm.tm_mday = nowtime->tm_mday;
	BeginTm.tm_hour = global.kingwar_activity_hour;
	BeginTm.tm_min = global.kingwar_activity_minute;
	BeginTm.tm_sec = 0;
	int beginstamp = (int)mktime( &BeginTm );
	int nowstamp = (int)time( NULL );
	// 活动未开始
	if ( nowstamp < beginstamp )
	{
		pValue.m_state = 0;
		pValue.m_beginstamp = beginstamp;
		pValue.m_endstamp = 0;
	}
	// 活动中
	else if ( nowstamp >= g_kingwar_activity_beginstamp && nowstamp < g_kingwar_activity_endstamp )
	{
		pValue.m_state = 1;
		pValue.m_beginstamp = g_kingwar_activity_beginstamp;
		pValue.m_endstamp = g_kingwar_activity_endstamp;
	}
	else
	{ // 活动结束
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

int kingwar_town_sendinfo( int id )
{
	if ( id <= 0 || id >= g_kingwar_town_maxcount )
		return -1;
	SLK_NetS_KingwarTown pValue = { 0 };
	pValue.m_id = id;
	pValue.m_nation = g_kingwar_town[id].nation;
	pValue.m_attack_total = g_kingwar_town[id].attack_total;
	pValue.m_defense_total = g_kingwar_town[id].defense_total;
	netsend_kingwartown_S( SUBSCRIBE_CMD_KINGWARDLG, SENDTYPE_SUBSCRIBE, &pValue );
	return 0;
}

int kingwar_town_sendall()
{
	SLK_NetS_KingwarTownList pValue = { 0 };
	for ( int id = 4; id < g_kingwar_town_maxcount; id++ )
	{
		pValue.m_list[pValue.m_count].m_id = id;
		pValue.m_list[pValue.m_count].m_nation = g_kingwar_town[id].nation;
		pValue.m_list[pValue.m_count].m_attack_total = g_kingwar_town[id].attack_total;
		pValue.m_list[pValue.m_count].m_defense_total = g_kingwar_town[id].defense_total;
		pValue.m_count += 1;
	}
	pValue.m_leftstamp = g_kingwar_activity_endstamp - (int)time( NULL );
	pValue.m_losthp = g_kingwar_lost_totalhp;
	netsend_kingwartownlist_S( SUBSCRIBE_CMD_KINGWARDLG, SENDTYPE_SUBSCRIBE, &pValue );
	return 0;
}

int kingwar_town_sendlist( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	SLK_NetS_KingwarTownList pValue = { 0 };
	for ( int id = 4; id < g_kingwar_town_maxcount; id++ )
	{
		pValue.m_list[pValue.m_count].m_id = id;
		pValue.m_list[pValue.m_count].m_nation = g_kingwar_town[id].nation;
		pValue.m_list[pValue.m_count].m_attack_total = g_kingwar_town[id].attack_total;
		pValue.m_list[pValue.m_count].m_defense_total = g_kingwar_town[id].defense_total;
		pValue.m_count += 1;
	}
	pValue.m_leftstamp = g_kingwar_activity_endstamp - (int)time( NULL );
	pValue.m_losthp = g_kingwar_lost_totalhp;
	netsend_kingwartownlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	g_actors[actor_index].subscribe_cmd[SUBSCRIBE_CMD_KINGWARDLG] = 1;
	return 0;
}

// 取消订阅
int kingwar_subscribe_cancel( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	g_actors[actor_index].subscribe_cmd[SUBSCRIBE_CMD_KINGWARDLG] = 0;
	return 0;
}

// 血战排行榜
int kingwar_ranklist( int actor_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_KingWarRankList pValue = { 0 };
	pValue.m_mypoint = pCity->kw_point;
	pValue.m_totalkill = pCity->kw_totalkill;
	for ( int tmpi = 0; tmpi < KINGWAR_RANK_MAX; tmpi++ )
	{
		if ( g_kingwar_rank[tmpi].actorid <= 0 )
			break;
		if ( g_kingwar_rank[tmpi].actorid == pCity->actorid )
		{
			pValue.m_myrank = tmpi + 1;
		}
		memcpy( &pValue.m_list[tmpi], &g_kingwar_rank[tmpi].info, sizeof( SLK_NetS_KingWarRank ) );
		pValue.m_list[tmpi].m_rank = tmpi + 1;
		pValue.m_count += 1;
	}
	netsend_kingwarranklist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 排行榜计算
int kingwar_rankcalc( int army_index )
{
	City *pArmyCity = army_getcityptr( army_index );
	if ( !pArmyCity )
		return -1;

	// 检查排行榜里是否有我的武将，如果有那么和这个存在武将比较，如果比这个存在武将多，那删除，然后进行插入排序
	for ( int tmpi = 0; tmpi < KINGWAR_RANK_MAX; tmpi++ )
	{
		if ( g_kingwar_rank[tmpi].actorid == pArmyCity->actorid )
		{
			if ( g_army[army_index].damage > g_kingwar_rank[tmpi].info.m_kill )
			{
				if ( tmpi < KINGWAR_RANK_MAX - 1 )
				{
					memmove( &g_kingwar_rank[tmpi], &g_kingwar_rank[tmpi + 1], sizeof( KingWarRank ) * (KINGWAR_RANK_MAX - 1 - tmpi) );
				}
				memset( &g_kingwar_rank[KINGWAR_RANK_MAX - 1], 0, sizeof( KingWarRank ) );
			}
		}
	}

	for ( int tmpi = 0; tmpi < KINGWAR_RANK_MAX; tmpi++ )
	{
		if ( g_army[army_index].damage > g_kingwar_rank[tmpi].info.m_kill )
		{
			if ( tmpi < KINGWAR_RANK_MAX - 1 )
			{
				memmove( &g_kingwar_rank[tmpi+1], &g_kingwar_rank[tmpi], sizeof( KingWarRank ) * (KINGWAR_RANK_MAX - 1 - tmpi) );
			}

			g_kingwar_rank[tmpi].actorid = pArmyCity->actorid;
			g_kingwar_rank[tmpi].city_index = pArmyCity->index;
			g_kingwar_rank[tmpi].info.m_kill = g_army[army_index].damage;
			g_kingwar_rank[tmpi].info.m_herokind = g_army[army_index].herokind[0];
			g_kingwar_rank[tmpi].info.m_herocolor = hero_getcolor( pArmyCity, g_army[army_index].herokind[0] );
			g_kingwar_rank[tmpi].info.m_nation = pArmyCity->nation;
			strncpy( g_kingwar_rank[tmpi].info.m_name, pArmyCity->name, NAME_SIZE );
			g_kingwar_rank[tmpi].info.m_name_len = strlen( g_kingwar_rank[tmpi].info.m_name );
			break;
		}
	}
	return 0;
}

// 前往
int kingwar_army_goto( int actor_index, int army_index, int id )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( army_myself( actor_index, army_index ) < 0 )
		return -1;
	if ( id <= 0 || id >= g_kingwar_town_maxcount )
		return -1;
	if ( g_army[army_index].state == ARMY_STATE_KINGWAR_WAITSOS || g_army[army_index].state == ARMY_STATE_KINGWAR_DEAD )
	{ // 此武将已经败退
		actor_notify_alert( actor_index, 1418 );
		return -1;
	}
	int oldid = g_army[army_index].to_id;
	if ( oldid >= 4 && oldid < g_kingwar_town_maxcount )
	{ // 如果英雄在据点上，并且当前战斗状态，那么不允许使用前往，只能使用回防
		if ( g_kingwar_town[oldid].attack_queue[0] >= 0 && g_kingwar_town[oldid].defense_queue[0] >= 0 )
		{
			char v1[32] = { 0 };
			sprintf( v1, "%d", global.kingwar_token_def );
			actor_notify_msgbox_v( actor_index, MSGBOX_CALLBACK_KINGWAR_DEFENSE, army_index, id, 1410, v1, NULL );
			return -1;
		}
	}
	else
	{ // 英雄在集结点上，只能前往对应路线的据点
		if ( pCity->nation == 1 )
		{
			if ( id != 4 )
			{
				return -1;
			}
		}
		else if ( pCity->nation == 2 )
		{
			if ( id != 5 )
			{
				return -1;
			}
		}
		else if ( pCity->nation == 3 )
		{
			if ( id != 6 )
			{
				return -1;
			}
		}
	}

	if ( g_kingwar_town[id].nation == pCity->nation )
	{ // 被我国占领，加入防御方
		kingwar_town_queuedel( oldid, kingwar_town_attack_or_defense( oldid, army_index ), army_index );
		if ( kingwar_town_queueadd( id, KINGWAR_TOWN_DEFENSE, army_index ) < 0 )
			return -1;
	}
	else if ( g_kingwar_town[id].nation == 0 && g_kingwar_town[id].defense_queue[0] < 0 )
	{ // 被群雄占领，并且没人防守，此为初始状态，直接加入防御方
		kingwar_town_queuedel( oldid, kingwar_town_attack_or_defense( oldid, army_index ), army_index );
		if ( kingwar_town_queueadd( id, KINGWAR_TOWN_DEFENSE, army_index ) < 0 )
			return -1;
		g_kingwar_town[id].nation = pCity->nation;
	}
	else
	{ // 加入攻击方
		kingwar_town_queuedel( oldid, kingwar_town_attack_or_defense( oldid, army_index ), army_index );
		if ( kingwar_town_queueadd( id, KINGWAR_TOWN_ATTACK, army_index ) < 0 )
			return -1;
	}
	city_battlequeue_sendupdate( army_index );
	g_kingwar_town_update = 1;
	return 0;
}

// 进攻
int kingwar_army_attack( int actor_index, int army_index, int id )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( army_myself( actor_index, army_index ) < 0 )
		return -1;
	if ( id <= 0 || id >= g_kingwar_town_maxcount )
		return -1;
	if ( g_army[army_index].state == ARMY_STATE_KINGWAR_WAITSOS || g_army[army_index].state == ARMY_STATE_KINGWAR_DEAD )
	{ // 此武将已经败退
		actor_notify_alert( actor_index, 1418 );
		return -1;
	}

	int oldid = g_army[army_index].to_id;
	if ( oldid >= 4 && oldid < g_kingwar_town_maxcount )
	{ // 如果英雄在据点上，并且当前战斗状态，那么不允许使用进攻，只能使用偷袭
		if ( g_kingwar_town[oldid].attack_queue[0] >= 0 && g_kingwar_town[oldid].defense_queue[0] >= 0 )
		{
			char v1[32] = { 0 };
			sprintf( v1, "%d", global.kingwar_token_sneak );
			actor_notify_msgbox_v( actor_index, MSGBOX_CALLBACK_KINGWAR_SNEAK, army_index, id, 1411, v1, NULL );
			return -1;
		}
	}

	if ( g_kingwar_town[id].nation == pCity->nation )
	{ // 被我国占领，加入防御方
		kingwar_town_queuedel( oldid, kingwar_town_attack_or_defense( oldid, army_index ), army_index );
		if ( kingwar_town_queueadd( id, KINGWAR_TOWN_DEFENSE, army_index ) < 0 )
			return -1;
	}
	else if ( g_kingwar_town[id].nation == 0 && g_kingwar_town[id].defense_queue[0] < 0 )
	{ // 被群雄占领，并且没人防守，此为初始状态，直接加入防御方
		kingwar_town_queuedel( oldid, kingwar_town_attack_or_defense( oldid, army_index ), army_index );
		if ( kingwar_town_queueadd( id, KINGWAR_TOWN_DEFENSE, army_index ) < 0 )
			return -1;
		g_kingwar_town[id].nation = pCity->nation;
	}
	else
	{ // 加入攻击方
		kingwar_town_queuedel( oldid, kingwar_town_attack_or_defense( oldid, army_index ), army_index );
		if ( kingwar_town_queueadd( id, KINGWAR_TOWN_ATTACK, army_index ) < 0 )
			return -1;
	}
	city_battlequeue_sendupdate( army_index );
	g_kingwar_town_update = 1;
	return 0;
}

// 偷袭
int kingwar_army_sneak( int actor_index, int army_index, int id )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( army_myself( actor_index, army_index ) < 0 )
		return -1;
	if ( id <= 0 || id >= g_kingwar_town_maxcount )
		return -1;
	if ( g_army[army_index].state == ARMY_STATE_KINGWAR_WAITSOS || g_army[army_index].state == ARMY_STATE_KINGWAR_DEAD )
	{ // 此武将已经败退
		actor_notify_alert( actor_index, 1418 );
		return -1;
	}
	if ( g_army[army_index].state != ARMY_STATE_KINGWAR_FIGHT )
		return -1;
	int oldid = g_army[army_index].to_id;
	if ( army_index == g_kingwar_town[oldid].attack_queue[0] || army_index == g_kingwar_town[oldid].defense_queue[0] )
	{ // 该武将已经上阵，无法进行偷袭
		actor_notify_alert( actor_index, 1404 );
		return -1;
	}
	if ( oldid >= 4 && oldid < g_kingwar_town_maxcount )
	{ // 如果英雄在据点上，并且当前非战斗状态了，直接转变为进攻，不消耗钻石
		if ( g_kingwar_town[oldid].attack_queue[0] < 0 )
		{
			kingwar_army_attack( actor_index, army_index, id );
			return -1;
		}
	}

	if ( g_kingwar_town[id].nation == pCity->nation )
	{ // 被本国贡献，偷袭等于回防
		kingwar_army_defense( actor_index, army_index, id );
		return -1;
	}

	if ( g_actors[actor_index].token < global.kingwar_token_sneak )
	{
		actor_notify_alert( actor_index, 645 );
		return -1;
	}
	
	// 删除之前队列
	kingwar_town_queuedel( oldid, kingwar_town_attack_or_defense( oldid, army_index ), army_index );
	if ( kingwar_town_queueadd( id, KINGWAR_TOWN_ATTACK, army_index ) < 0 )
		return -1;
	actor_change_token( actor_index, -global.kingwar_token_sneak, PATH_KINGWAR_SNEAK, 0 );
	city_battlequeue_sendupdate( army_index );
	g_kingwar_town_update = 1;
	return 0;
}

// 回防
int kingwar_army_defense( int actor_index, int army_index, int id )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( army_myself( actor_index, army_index ) < 0 )
		return -1;
	if ( id <= 0 || id >= g_kingwar_town_maxcount )
		return -1;
	if ( g_army[army_index].state == ARMY_STATE_KINGWAR_WAITSOS || g_army[army_index].state == ARMY_STATE_KINGWAR_DEAD )
	{ // 此武将已经败退
		actor_notify_alert( actor_index, 1418 );
		return -1;
	}
	if ( g_army[army_index].state != ARMY_STATE_KINGWAR_FIGHT )
		return -1;

	int oldid = g_army[army_index].to_id;
	if ( army_index == g_kingwar_town[oldid].attack_queue[0] || army_index == g_kingwar_town[oldid].defense_queue[0] )
	{ // 该武将已经上阵，无法进行回防
		actor_notify_alert( actor_index, 1405 );
		return -1;
	}
	if ( oldid >= 4 && oldid < g_kingwar_town_maxcount )
	{ // 如果英雄在据点上，并且当前非战斗状态了，直接转变为前往，不消耗钻石
		if ( g_kingwar_town[oldid].attack_queue[0] < 0 )
		{
			kingwar_army_goto( actor_index, army_index, id );
			return -1;
		}
	}

	if ( g_kingwar_town[id].nation != pCity->nation )
	{ // 该据点已经沦陷，无法回防
		actor_notify_alert( actor_index, 1409 );
		return -1;
	}

	if ( g_actors[actor_index].token < global.kingwar_token_def )
	{
		actor_notify_alert( actor_index, 645 );
		return -1;
	}

	// 删除之前队列
	kingwar_town_queuedel( oldid, kingwar_town_attack_or_defense( oldid, army_index ), army_index );
	if ( kingwar_town_queueadd( id, KINGWAR_TOWN_DEFENSE, army_index ) < 0 )
		return -1;
	actor_change_token( actor_index, -global.kingwar_token_def, PATH_KINGWAR_DEFENSE, 0 );
	city_battlequeue_sendupdate( army_index );
	g_kingwar_town_update = 1;
	return 0;
}

// 单挑
int kingwar_army_pk( int actor_index, int army_index, int id )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( army_myself( actor_index, army_index ) < 0 )
		return -1;
	if ( id <= 0 || id >= g_kingwar_town_maxcount )
		return -1;
	if ( g_army[army_index].state == ARMY_STATE_KINGWAR_WAITSOS || g_army[army_index].state == ARMY_STATE_KINGWAR_DEAD )
	{ // 此武将已经败退
		actor_notify_alert( actor_index, 1418 );
		return -1;
	}
	if ( g_army[army_index].state != ARMY_STATE_KINGWAR_FIGHT )
		return -1;

	if ( army_index == g_kingwar_town[id].attack_queue[0] || army_index == g_kingwar_town[id].defense_queue[0] )
	{ // 该武将已经上阵，无法进行单挑
		actor_notify_alert( actor_index, 1406 );
		return -1;
	}

	// 我是攻击方还是防御方
	char attack = kingwar_town_attack_or_defense( id, army_index );
	char target;
	if ( attack == KINGWAR_TOWN_ATTACK )
		target = KINGWAR_TOWN_DEFENSE;
	else
		target = KINGWAR_TOWN_ATTACK;

	if ( kingwar_town_queueget( id, target ) < 0 )
	{ // 对方无可单挑队伍
		actor_notify_alert( actor_index, 1407 );
		return -1;
	}

	SLK_NetS_KingWarPK pkValue = { 0 };
	pkValue.m_herokind = g_army[army_index].herokind[0];
	pkValue.m_id = id;
	if ( attack == KINGWAR_TOWN_ATTACK )
	{
		pkValue.m_hp = g_kingwar_town[id].defense_total;
	}
	else
	{
		pkValue.m_hp = g_kingwar_town[id].attack_total;
	}
	netsend_kingwarpk_S( actor_index, SENDTYPE_ACTOR, &pkValue );

	// 战斗通知
	SLK_NetS_KingWarNotifyList pValue = { 0 };
	for ( int tmpi = 0; tmpi < KINGWAR_TOWN_QUEUE_MAX; tmpi++ )
	{
		int target_army_index = kingwar_town_queueget( id, target );
		if ( target_army_index < 0 || target_army_index >= g_army_maxcount )
		{ // 没有了对手了
			break;
		}

		// 双方都有人，战斗
		if ( fight_start( army_index, MAPUNIT_TYPE_KINGWAR_TOWN, target_army_index ) < 0 )
		{
			break;
		}

		// 战斗结算
		fight_lost_calc();

		// 总损失兵力
		g_kingwar_lost_totalhp += g_fight.attack_total_damage;
		g_kingwar_lost_totalhp += g_fight.defense_total_damage;

		pValue.m_list[pValue.m_count].m_id = id;
		pValue.m_list[pValue.m_count].m_result = g_fight.result;

		pValue.m_list[pValue.m_count].m_a_losthp = g_fight.defense_total_damage;
		pValue.m_list[pValue.m_count].m_d_losthp = g_fight.attack_total_damage;

		pValue.m_list[pValue.m_count].m_a_nation = army_getnation( army_index );
		pValue.m_list[pValue.m_count].m_d_nation = army_getnation( target_army_index );

		pValue.m_list[pValue.m_count].m_a_heroid = g_army[army_index].herokind[0];
		pValue.m_list[pValue.m_count].m_d_heroid = g_army[target_army_index].herokind[0];

		City *pACity = army_getcityptr( army_index );
		if ( pACity )
		{
			pACity->kw_totalkill += g_fight.attack_total_damage;
			pValue.m_list[pValue.m_count].m_a_color = hero_getcolor( pACity, pValue.m_list[pValue.m_count].m_a_heroid );
			strncpy( pValue.m_list[pValue.m_count].m_a_name, pACity->name, NAME_SIZE );
			pValue.m_list[pValue.m_count].m_a_name_len = strlen( pValue.m_list[pValue.m_count].m_a_name );
		}

		City *pDCity = army_getcityptr( target_army_index );
		if ( pDCity )
		{
			pDCity->kw_totalkill += g_fight.defense_total_damage;
			pValue.m_list[pValue.m_count].m_d_color = hero_getcolor( pDCity, pValue.m_list[pValue.m_count].m_d_heroid );
			strncpy( pValue.m_list[pValue.m_count].m_d_name, pDCity->name, NAME_SIZE );
			pValue.m_list[pValue.m_count].m_d_name_len = strlen( pValue.m_list[pValue.m_count].m_d_name );
		}
		pValue.m_count += 1;

		if ( pValue.m_count >= 10 )
		{
			netsend_kingwarnotifylist_S( actor_index, SENDTYPE_ACTOR, &pValue );
			pValue.m_count = 0;
		}

		if ( g_fight.result == FIGHT_WIN )
		{ // 单挑一次胜利
			kingwar_town_totalcalc( id, attack );
			// 删除一个防守方队列
			kingwar_town_queuedel( id, target, target_army_index );
			g_army[target_army_index].state = ARMY_STATE_KINGWAR_WAITSOS;
			g_army[target_army_index].statetime = 0;
			g_army[target_army_index].stateduration = global.kingwar_dead_cd;
			city_battlequeue_sendupdate( target_army_index );
		}
		else
		{ // 失败
			kingwar_town_totalcalc( id, target );
			// 删除一个攻击方队列
			kingwar_town_queuedel( id, attack, army_index );
			g_army[army_index].state = ARMY_STATE_KINGWAR_WAITSOS;
			g_army[army_index].statetime = 0;
			g_army[army_index].stateduration = global.kingwar_dead_cd;
			city_battlequeue_sendupdate( army_index );
			break;
		}
	}
	if ( pValue.m_count > 0 )
	{
		netsend_kingwarnotifylist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}

	city_battlequeue_sendupdate( army_index );

	//// 检查目标是否有人
	//if ( kingwar_town_queueget( id, target ) < 0 )
	//{ // 目标没人了
	//	if ( target == KINGWAR_TOWN_DEFENSE )
	//	{ // 转换
	//		kingwar_town_change_nation( id, army_getnation( army_index ), army_index );
	//	}
	//}

	// 防御方没人了，变成攻击方的
	if ( kingwar_town_queueget( id, KINGWAR_TOWN_DEFENSE ) < 0 )
	{
		kingwar_town_change_nation( id, army_getnation( army_index ), army_index );
	}

	g_kingwar_town_update = 1;
	return 0;
}

// 救援
int kingwar_army_rebirth( int actor_index, int army_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( g_army[army_index].state == 0 || g_army[army_index].state == ARMY_STATE_KINGWAR_DEAD )
	{ // 此武将已经败退
		actor_notify_alert( actor_index, 1418 );
		return -1;
	}
	if ( army_myself( actor_index, army_index ) < 0 )
	{ // 此武将已经败退
		actor_notify_alert( actor_index, 1418 );
		return -1;
	}
	if ( g_army[army_index].state != ARMY_STATE_KINGWAR_WAITSOS )
		return -1;

	int costtoken = 0;
	if ( g_army[army_index].appdata == 0 )
		costtoken = 10;
	else if ( g_army[army_index].appdata == 1 )
		costtoken = 100;
	else if ( g_army[army_index].appdata == 2 )
		costtoken = 200;
	else
		costtoken = 400;

	if ( g_actors[actor_index].token < costtoken )
		return -1;
	
	// 补充士兵
	int soldiers = hero_addsoldiers( actor_index, g_army[army_index].herokind[0], PATH_KINGWAR_REBIRTH );
	if ( soldiers <= 0 )
	{ // 兵营库存不足，救援失败
		actor_notify_alert( actor_index, 1408 );
		return -1;
	}

	g_army[army_index].statetime = 0;
	g_army[army_index].stateduration = 0;
	g_army[army_index].appdata += 1;
	g_army[army_index].totals = soldiers;

	if ( g_army[army_index].to_id >= 1 && g_army[army_index].to_id <= 3 )
	{ // 在集结点救援
		g_army[army_index].state = ARMY_STATE_KINGWAR_READY;
	}
	else
	{
		g_army[army_index].state = ARMY_STATE_KINGWAR_FIGHT;
		if ( kingwar_town_queueadd( g_army[army_index].to_id, kingwar_town_attack_or_defense( g_army[army_index].to_id, army_index ), army_index ) < 0 )
			return -1;
	}

	actor_change_token( actor_index, -costtoken, PATH_KINGWAR_REBIRTH, 0 );
	city_battlequeue_sendupdate( army_index );
	g_kingwar_town_update = 1;

	// 女将亲昵度
	Hero *pHero = city_hero_getptr_withkind( pCity->index, g_army[army_index].herokind[0] );
	if ( pHero && pHero->girlkind > 0 )
	{
		girl_addloveexp_kingwar( pCity, pHero->kind, pHero->girlkind );
	}
	return 0;
}

// 发送血战积分
int kingwar_sendpoint( int actor_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_KingWarPoint pValue;
	pValue.m_point = pCity->kw_point;
	netsend_kingwarpoint_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 皇城密库兑换
int kingwar_changeitem( int actor_index, int index, int itemkind )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( index < 0 || index >= g_kingwar_config_maxnum )
		return -1;
	if ( g_kingwar_config[index].exchange_item != itemkind )
		return -1;
	if ( pCity->kw_point < g_kingwar_config[index].exchange_point )
		return -1;
	item_getitem( actor_index, itemkind, 1, -1, PATH_KINGWAR_CHANGE );
	city_kingwarpoint( pCity->index, -g_kingwar_config[index].exchange_point, PATH_KINGWAR_CHANGE );
	kingwar_sendpoint( actor_index );
	return 0;
}

// 挖宝开启
int kingwar_treasure_onopen()
{
	g_kingwar_treasure_open = 1;

	// 距离今天零点剩余多少秒
	time_t t;
	time( &t );
	struct tm *nowtime = localtime( &t );

	time_t zero_t;
	time( &zero_t );
	struct tm *activitytime = localtime( &zero_t );
	struct tm ZeroTm = { 0 };
	ZeroTm.tm_year = nowtime->tm_year;
	ZeroTm.tm_mon = nowtime->tm_mon;
	ZeroTm.tm_mday = nowtime->tm_mday;
	ZeroTm.tm_hour = 23;
	ZeroTm.tm_min = 59;
	ZeroTm.tm_sec = 59;
	int endstamp = (int)mktime( &ZeroTm );
	g_kingwar_treasure_endstamp = endstamp + 24 * 3600;
	kingwar_treasure_sendinfo( -1 );
	return 0;
}

// 挖宝结束
int kingwar_treasure_onclose()
{
	kingwar_treasure_sendinfo( -1 );
	g_kingwar_treasure_open = 0;

	// 5531	下一届的皇城血战开启时间{ 0 }，请各国在此期间积极备战，争夺霸主之位。
	char v1[32] = { 0 };
	sprintf( v1, "%s%d", TAG_DATE, g_kingwar_nextstamp );
	mail_sendall( 5035, 5531, v1, NULL, NULL, "" );
	return 0;
}

// 挖宝活动发送
int kingwar_treasure_sendinfo( int actor_index )
{
	if ( g_kingwar_treasure_open == 0 )
		return -1;

	SLK_NetS_TreasureActivity pValue = { 0 };
	int nowstamp = (int)time( NULL );
	if ( nowstamp < g_kingwar_treasure_endstamp )
	{
		pValue.m_state = 1;
		pValue.m_endstamp = g_kingwar_treasure_endstamp;
	}
	else
	{ // 活动结束
		pValue.m_state = 2;
	}
	pValue.m_nation = g_map_town[MAPUNIT_KING_TOWNID].nation;
	if ( g_kingwar_level > 0 && g_kingwar_level < g_kingwar_config_maxnum )
	{
		pValue.m_treasure_num_max = g_kingwar_config[g_kingwar_level].drawing_num;
		for ( int tmpi = 0; tmpi < 3; tmpi++ )
		{
			pValue.m_treasure_num[tmpi] = g_treasure_nationitem_count[tmpi];
		}
	}

	if ( actor_index >= 0 && actor_index < g_maxactornum )
	{
		netsend_treasureactivity_S( actor_index, SENDTYPE_ACTOR, &pValue );
		kingwar_treasure_cityinfo( actor_index );
	}
	else
	{
		netsend_treasureactivity_S( 0, SENDTYPE_WORLDMAP, &pValue );
	}
	return 0;
}

int kingwar_treasure_cityinfo( int actor_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_TreasureHas pValue;
	pValue.m_has = pCity->kw_has;
	pValue.m_px = pCity->kw_px;
	pValue.m_py = pCity->kw_py;
	pValue.m_tn = pCity->kw_tn;
	netsend_treasurehas_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 买酒获得坐标
int kingwar_treasure_buypos( int actor_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	char nation = pCity->nation - 1;
	if ( nation < 0 || nation > 2 )
		return -1;

	if ( g_treasure_nationitem_count[nation] <= 0 )
	{
		kingwar_treasure_sendinfo( actor_index );
		return -1;
	}
	if ( pCity->kw_has > 0 )
	{
		return -1;
	}

	if ( pCity->kw_tn >= 10 )
		pCity->kw_tn = 9;

	if ( actor_change_token( actor_index, -g_kingwar_config[pCity->kw_tn + 1].treasure_costtoken, PATH_KINGWAR_TREASURE, 0 ) < 0 )
		return -1;

	short posx = -1, posy = -1;
	map_zone_randpos( MAPZONE_CENTERID, &posx, &posy, 4096 );
	pCity->kw_px = posx;
	pCity->kw_py = posy;
	pCity->kw_tn += 1;
	kingwar_treasure_cityinfo( actor_index );
	return 0;
}

// 挖宝
int kingwar_treasure_do( int actor_index, short posx, short posy )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	char nation = pCity->nation - 1;
	if ( nation < 0 || nation > 2 )
		return -1;

	if ( posx == pCity->kw_px && posy == pCity->kw_py )
	{ // 是挖宝点
		// 检查积分
		if ( pCity->kw_tn > 10 )
			pCity->kw_tn = 10;
		if ( pCity->kw_point < g_kingwar_config[pCity->kw_tn].treasure_costpoint )
		{
			actor_notify_alert( actor_index, 1918 );
			return -1;
		}
		city_kingwarpoint( pCity->index, -g_kingwar_config[pCity->kw_tn].treasure_costpoint, PATH_KINGWAR_TREASURE );

		int odds = rand() % 1000;
		if ( odds <= g_kingwar_config[pCity->kw_tn].treasure_equipodds && g_treasure_nationitem_count[nation] > 0 )
		{ // 掉落装备图纸
			short itemkind = g_treasure_nationitem[nation][0];
			if ( itemkind > 0 )
			{
				memmove( &g_treasure_nationitem[nation][0], &g_treasure_nationitem[nation][1], sizeof( short )*(KINGWAR_TREASURE_MAX - 1) );
				g_treasure_nationitem[nation][KINGWAR_TREASURE_MAX - 1] = 0;
				g_treasure_nationitem_count[nation] -= 1;
				kingwar_congratulate_add( pCity, itemkind );
				item_getitem( actor_index, itemkind, 1, -1, PATH_KINGWAR_TREASURE );
				pCity->kw_has = 1;
				kingwar_treasure_sendinfo( -1 );

				// 6011	{0}在神将{1}的指引下，扛着洛阳铲前去挖宝，挖出了<color={2}>{3}</color>
				char v1[32] = { 0 };
				char v2[32] = { 0 };
				char v3[32] = { 0 };
				char v4[32] = { 0 };
				sprintf( v1, "%s", pCity->name );
				if ( pCity->nation == 1 )
					sprintf( v2, "%s%d", TAG_HERO, 92 );
				else if ( pCity->nation == 2 )
					sprintf( v2, "%s%d", TAG_HERO, 118 );
				else
					sprintf( v2, "%s%d", TAG_HERO, 119 );
				sprintf( v3, "%s%d", TAG_COLOR, item_getcolorlevel( itemkind ) );
				sprintf( v4, "%s%d", TAG_ITEMKIND, itemkind );
				system_talkjson_world( 6011, v1, v2, v3, v4, NULL, NULL, 1 );

				// 1998	地表冒着灿烂的金光，您掘地三尺，终于挖到了<color=#{0}>{1}</color>
				actor_notify_alert_v( actor_index, 1998, v3, v4 );

				// 6012	{0}的金色色图纸已全部集齐，国宴已顺利开启，主公前往恭贺会获得一份大礼
				if ( g_treasure_nationitem_count[nation] <= 0 )
				{
					sprintf( v1, "%s%d", TAG_NATION, pCity->nation );
					system_talkjson( 0, SENDTYPE_NATION + pCity->nation, 6012, v1, NULL, NULL, NULL, NULL, NULL, 1 );
				}
			}
		}
		else
		{ // 给与安慰奖
			int awardgroup = g_kingwar_config[pCity->kw_tn].treasure_award;
			AwardGetInfo getinfo = { 0 };
			awardgroup_withindex( actor_index, awardgroup, -1, PATH_KINGWAR_TREASURE, &getinfo );
			// 1997	地表似乎有些新的泥土，您掘地三尺，只挖到了{1}, 再去问问新的坐标吧
			char v1[64] = { 0 };
			char award_content[256] = { 0 };
			awardgroup_makestr( &getinfo, award_content );
			snprintf( v1, 64, "%s%s", TAG_AWARD, award_content );
			actor_notify_alert_v( actor_index, 1997, v1, NULL );
		}

		pCity->kw_px = 0;
		pCity->kw_py = 0;
		kingwar_treasure_cityinfo( actor_index );
	}
	else
	{ // 非买酒挖宝点
		if ( pCity->kw_point < g_kingwar_config[1].treasure_normalpoint )
		{
			actor_notify_alert( actor_index, 1918 );
			return -1;
		}
		city_kingwarpoint( pCity->index, -g_kingwar_config[1].treasure_normalpoint, PATH_KINGWAR_TREASURE );

		int awardgroup = g_kingwar_config[1].treasure_normalaward;
		AwardGetInfo getinfo = { 0 };
		awardgroup_withindex( actor_index, awardgroup, -1, PATH_KINGWAR_TREASURE, &getinfo );
		// 1923	地表似乎有些新的泥土，您掘地三尺，只挖到了{1}, 再去问问新的坐标吧
		char v1[64] = { 0 };
		char award_content[256] = { 0 };
		awardgroup_makestr( &getinfo, award_content );
		snprintf( v1, 64, "%s%s", TAG_AWARD, award_content );
		actor_notify_alert_v( actor_index, 1997, v1, NULL );
	}
	return 0;
}
// 前往
int kingwar_treasure_goto( int actor_index, short posx, short posy )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	map_goto_withpos( actor_index, 1, posx, posy );
	return 0;
}

// 添加恭贺列表
int kingwar_congratulate_add( City *pCity, int itemkind )
{
	if ( !pCity )
		return -1;
	char nation = pCity->nation - 1;
	if ( nation < 0 || nation > 2 )
		return -1;

	for ( int tmpi = 0; tmpi < KINGWAR_TREASURE_MAX; tmpi++ )
	{
		if ( g_treasure_actor[nation][tmpi].m_itemkind <= 0 )
		{
			g_treasure_actor[nation][tmpi].m_itemkind = itemkind;
			strncpy( g_treasure_actor[nation][tmpi].m_name, pCity->name, NAME_SIZE );
			g_treasure_actor[nation][tmpi].m_name_len = strlen( g_treasure_actor[nation][tmpi].m_name );
			break;
		}
	}
	return 0;
}

// 恭贺列表
int kingwar_congratulate_sendlist( int actor_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	char nation = pCity->nation - 1;
	if ( nation < 0 || nation > 2 )
		return -1;
	SLK_NetS_TreasureActorList pValue = { 0 };
	pValue.m_co = pCity->kw_co;
	for ( int tmpi = 0; tmpi < KINGWAR_TREASURE_MAX; tmpi++ )
	{
		if ( g_treasure_actor[nation][tmpi].m_itemkind <= 0 )
			continue;
		memcpy( &pValue.m_list[pValue.m_count], &g_treasure_actor[nation][tmpi], sizeof( SLK_NetS_TreasureActor ) );
		pValue.m_count += 1;
	}
	netsend_treasureactorlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 恭贺
int kingwar_congratulate( int actor_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	char nation = pCity->nation - 1;
	if ( nation < 0 || nation > 2 )
		return -1;
	if ( pCity->kw_co > 0 )
		return -1;
	pCity->kw_co = 1;

	int awardgroup = g_kingwar_config[1].co_award;
	AwardGetInfo getinfo = { 0 };
	awardgroup_withindex( actor_index, awardgroup, -1, PATH_KINGWAR_TREASURE_CO, &getinfo );

	// 1999	恭贺获得{0}
	char v1[64] = { 0 };
	char award_content[256] = { 0 };
	awardgroup_makestr( &getinfo, award_content );
	snprintf( v1, 64, "%s%s", TAG_AWARD, award_content );
	actor_notify_alert_v( actor_index, 1999, v1, NULL );
	return 0;
}
