#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "actor.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "script.h"
#include "db.h"
#include "script_auto.h"
#include "server_netsend_auto.h"
#include "activity.h"
#include "wqueue.h"
#include "timegmcmd.h"
#include "gmcmd.h"
#include "system.h"
#include "global.h"
#include "pay.h"
#include "actor_times.h"
#include "city.h"
#include "chat.h"
#include "quest.h"
#include "hero.h"
#include "nation.h"
#include "mail.h"
#include "activity_04.h"
#include "activity_22.h"
#include "equip.h"
#include "map.h"
#include "map_activity.h"
#include "map_zone.h"
#include "mapunit.h"
#include "army.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern Actor *g_actors;
extern int g_maxactornum;

extern Global global;
extern CommandEntry *g_command_entry;
extern int g_command_count;

extern City * g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern Army *g_army;
extern int g_army_maxcount;

ActivityItem *g_activity_item;
int g_activity_count = MAX_ACTIVITY_COUNT;

extern ActivityInfo02 *g_activity_02;
extern int g_activity_02_maxnum;

extern ActivityInfo03 *g_activity_03;
extern int g_activity_03_maxnum;

extern ActivityInfo04 *g_activity_04;
extern int g_activity_04_maxnum;

extern ActivityInfo05 *g_activity_05;
extern int g_activity_05_maxnum;

extern ActivityInfo06 *g_activity_06;
extern int g_activity_06_maxnum;

extern ActivityInfo08 *g_activity_08;
extern int g_activity_08_maxnum;

extern ActivityInfo10 *g_activity_10;
extern int g_activity_10_maxnum;

extern ActivityInfo12 *g_activity_12;
extern int g_activity_12_maxnum;

extern ActivityInfo17 *g_activity_17;
extern int g_activity_17_maxnum;

extern ActivityInfo33 *g_activity_33;
extern int g_activity_33_maxnum;

extern PayBag g_paybag[CITY_BAG_MAX];
extern Nation g_nation[NATION_MAX];

extern MapZoneInfo *g_zoneinfo;
extern int g_zoneinfo_maxnum;

extern Pos g_mapzone_emptypos[MAPZONE_POSCOUNT];
extern int g_mapzone_emptypos_count;

extern MapActivity *g_map_activity;
extern int g_map_activity_maxcount;

// 系统初始化
int activity_init()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[128];
	int activityid;

	g_activity_count = MAX_ACTIVITY_COUNT;

	// 分配空间
	g_activity_item = (ActivityItem *)malloc( sizeof(ActivityItem)*g_activity_count );
	memset( g_activity_item, 0, sizeof(ActivityItem)*g_activity_count );
	sprintf( szSQL, "select activityid,warningtime,starttime,endtime,closetime,openstat,endstat from activity" );
	if( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error(myGame), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myGame );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		activityid = atoi( row[0] );
		if( activityid < 0 )
		{
			mysql_free_result( res );
			return -1;
		}
		else if( activityid >= g_activity_count )
			continue;
		g_activity_item[activityid].m_activityid = activityid;
		g_activity_item[activityid].m_warningtime = atoi( row[1] );
		g_activity_item[activityid].m_starttime = atoi( row[2] );
		g_activity_item[activityid].m_endtime = atoi( row[3] );
		g_activity_item[activityid].m_closetime = atoi( row[4] );
		g_activity_item[activityid].m_openstat = atoi( row[5] );
		g_activity_item[activityid].m_endstat = atoi( row[6] );
	}
	mysql_free_result( res );
	return 0;
}

// 设置活动
int activity_set( int activityid, int warningtime, int starttime, int endtime, int closetime )
{
	if ( activityid <= 0 || activityid >= g_activity_count )
		return -1;
	if ( activity_inclose( activityid ) )
	{// 活动没有完全关闭，不允许设置
		return -1;
	}
	g_activity_item[activityid].m_activityid = activityid;
	g_activity_item[activityid].m_warningtime = warningtime;
	g_activity_item[activityid].m_starttime = starttime;
	g_activity_item[activityid].m_endtime = endtime;
	g_activity_item[activityid].m_closetime = closetime;
	g_activity_item[activityid].m_openstat = 0;
	g_activity_item[activityid].m_endstat = 0;

	char szSQL[1024];
	sprintf( szSQL, "replace into activity ( activityid,warningtime,starttime,endtime,closetime,openstat,endstat ) values('%d','%d','%d','%d','%d','%d','%d')", 
		activityid, g_activity_item[activityid].m_warningtime, g_activity_item[activityid].m_starttime, g_activity_item[activityid].m_endtime, g_activity_item[activityid].m_closetime, 0, 0 );
	if( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error(myGame) );
		write_gamelog( "%s", szSQL );
		if( mysql_ping(myGame) != 0 )
			db_reconnect_game();
		return -1;
	}
	return 0;
}

// 临时改变结束时间
int activity_setendtime( int activityid, int endtime, int closetime )
{
	if ( activityid <= 0 || activityid >= g_activity_count )
		return -1;
	if ( g_activity_item[activityid].m_endstat == 1 )
		return -1; // 已经调用过活动结束了，不允许临时改变了
	g_activity_item[activityid].m_endtime = endtime;
	g_activity_item[activityid].m_closetime = closetime;
	char szSQL[256] = { 0 };
	sprintf( szSQL, "UPDATE `activity` SET `endtime`='%d',`closetime`='%d' WHERE activityid='%d'", g_activity_item[activityid].m_endtime, g_activity_item[activityid].m_closetime, activityid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	return 0;
}

// 活动强制结束
int activity_force_end( int activityid )
{
	if ( activityid <= 0 || activityid >= g_activity_count )
		return -1;
	int nowstamp = (int)time( NULL );
	int left = g_activity_item[activityid].m_closetime - g_activity_item[activityid].m_endtime;
	activity_setendtime( activityid, nowstamp, nowstamp + left );
	activity_onend( activityid );
	if ( nowstamp >= g_activity_item[activityid].m_closetime && g_activity_item[activityid].m_endstat == 1 )
	{
		activity_onclose( activityid );
	}
	return 0;
}
// 活动强制关闭
int activity_force_close( int activityid )
{
	if ( activityid <= 0 || activityid >= g_activity_count )
		return -1;
	activity_onclose( activityid );
	return 0;
}

// 设置调用过开启
int activity_setopenstat( int activityid, int stat )
{
	if ( activityid >= g_activity_count )
		return -1;
	g_activity_item[activityid].m_openstat = stat;
	char szSQL[256] = { 0 };
	sprintf( szSQL, "UPDATE `activity` SET `openstat`='%d' WHERE activityid='%d'", stat, activityid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
	}
	return 0;
}

// 设置调用过关闭
int activity_setendstat( int activityid, int stat )
{
	if ( activityid >= g_activity_count )
		return -1;
	g_activity_item[activityid].m_endstat = stat;
	char szSQL[256] = { 0 };
	sprintf( szSQL, "UPDATE `activity` SET `endstat`='%d' WHERE activityid='%d'", stat, activityid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
	}
	return 0;
}

// 是否结束
int activity_intime( int activityid )
{
	if ( activityid >= g_activity_count )
		return 0;
	if ( g_activity_item[activityid].m_starttime <= 0 )
		return 0;
	int timestamp = (int)time( NULL );
	if ( timestamp < g_activity_item[activityid].m_starttime || timestamp >= g_activity_item[activityid].m_endtime )
		return 0;
	return 1;
}

// 是否关闭
int activity_inclose( int activityid )
{
	if ( activityid >= g_activity_count )
		return 0;
	if ( g_activity_item[activityid].m_starttime <= 0 )
		return 0;
	int timestamp = (int)time( NULL );
	if ( timestamp < g_activity_item[activityid].m_starttime || timestamp >= g_activity_item[activityid].m_closetime )
		return 0;
	return 1;
}

// 获得活动剩余时间
int activity_lefttime( int activityid )
{
	if ( activityid >= g_activity_count )
		return 0;
	if ( g_activity_item[activityid].m_starttime <= 0 )
		return 0;

	int lefttime = g_activity_item[activityid].m_endtime - (int)time( NULL );
	if ( lefttime < 0 )
		lefttime = 0;

	return lefttime;
}

// 获得活动应该持续时间
int activity_totaltime( int activityid )
{
	if ( activityid >= g_activity_count )
		return 0;
	if ( g_activity_item[activityid].m_starttime <= 0 )
		return 0;
	if ( g_activity_item[activityid].m_starttime <= 1 )
	{
		return g_activity_item[activityid].m_endtime - (int)time( NULL );
	}
	return g_activity_item[activityid].m_endtime - g_activity_item[activityid].m_starttime;
}

 // 活动逻辑分钟
int activity_logic()
{
	int activityid;
	int timestamp = (int)time( NULL );
	for ( activityid = 1; activityid < g_activity_count; activityid++ )
	{
		if ( g_activity_item[activityid].m_starttime <= 0 )
			continue;
		if ( timestamp < g_activity_item[activityid].m_warningtime )
		{// 未到预热时间
			continue;
		}

		// 已到预热时间，未到活动开启时间
		if ( timestamp < g_activity_item[activityid].m_starttime )
		{
			activity_onwarning( activityid, g_activity_item[activityid].m_starttime - timestamp );
			continue;
		}

		// 已经到开启时间，只会调用一次
		if ( g_activity_item[activityid].m_openstat == 0 )
		{
			activity_onopen( activityid );
		}

		// 结束
		if ( timestamp >= g_activity_item[activityid].m_endtime && g_activity_item[activityid].m_endstat == 0 )
		{
			activity_onend( activityid );
		}

		// 关闭
		if ( timestamp >= g_activity_item[activityid].m_closetime && g_activity_item[activityid].m_endstat == 1 )
		{
			activity_onclose( activityid );
			continue;
		}
		if ( timestamp < g_activity_item[activityid].m_endtime )
		{
			activity_onlogic( activityid );
		}
	}
	return 0;
}

// 活动结束后删除活动
int activity_delete( int activityid )
{
	if ( activityid >= g_activity_count )
		return 0;
	memset( &g_activity_item[activityid], 0, sizeof( ActivityItem ) );
	char szSQL[1024];
	sprintf( szSQL, "DELETE FROM `activity` WHERE `activityid`=%d;", activityid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	return 0;
}

// 调用预热
int activity_onwarning( int activityid, int lefttime )
{
	if ( activityid >= g_activity_count )
		return 0;
	switch ( activityid )
	{
	case ACTIVITY_NORMAL:
		break;
	case ACTIVITY_12:
		activity_12_onwarning( lefttime );
		break;
	case ACTIVITY_22:
		activity_22_onwarning( lefttime );
		break;
	case ACTIVITY_27:
		activity_27_onwarning( lefttime );
		break;
	default:
		break;
	}
	return 0;
}

// 调用开启
int activity_onopen( int activityid )
{
	if( activityid >= g_activity_count )
		return 0;

	switch( activityid )
	{
	case ACTIVITY_6:
		activity_06_onopen();
		break;
	case ACTIVITY_12:
		activity_12_onopen();
		break;
	case ACTIVITY_17:
		activity_17_onopen();
		break;
	case ACTIVITY_22:
		activity_22_onopen();
		break;
	case ACTIVITY_27:
		activity_27_onopen();
		break;
	case ACTIVITY_33:
		activity_33_onopen();
		break;
	default:
		sc_ActivityOnOpen( activityid );
		break;
	}
	activity_setopenstat( activityid, 1 );
	return 0;
}

// 调用结束
int activity_onend( int activityid )
{
	if ( activityid >= g_activity_count )
		return 0;
	switch ( activityid )
	{
	case ACTIVITY_6:
		activity_06_onend();
		break;
	case ACTIVITY_12:
		activity_12_onend();
		break;
	case ACTIVITY_17:
		activity_17_onend();
		break;
	case ACTIVITY_22:
		activity_22_onend();
		break;
	case ACTIVITY_27:
		activity_27_onend();
		break;
	case ACTIVITY_33:
		activity_33_onend();
		break;
	default:
		sc_ActivityOnEnd( activityid );
		break;
	}
	activity_setendstat( activityid, 1 );
	return 0;
}

// 调用关闭
int activity_onclose( int activityid )
{
	if( activityid >= g_activity_count )
		return 0;
	switch( activityid )
	{
	case ACTIVITY_6:
		activity_06_onclose();
		break;
	case ACTIVITY_17:
		activity_17_onclose();
		break;
	case  ACTIVITY_12:
		activity_12_onclose();
		break;
	case  ACTIVITY_22:
		activity_22_onclose();
		break;
	case ACTIVITY_27:
		activity_27_onclose();
		break;
	case ACTIVITY_33:
		activity_33_onclose();
		break;
	default:
		sc_ActivityOnClose( activityid );
		break;
	}
	activity_delete( activityid );
	return 0;
}

int activity_onlogic( int activityid )
{
	if( activityid >= g_activity_count )
		return 0;
	switch( activityid )
	{
	case 0:
		break;
	case ACTIVITY_27:
		activity_27_onlogic();
		break;
	case ACTIVITY_22:
		activity_22_onlogic();
		break;
	default:
		sc_ActivityOnLogic( activityid );
		break;
	}
	return 0;
}

// 活动列表
int activity_sendlist( int actor_index )
{
	int endtime = 0;
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	time_t t;
	time( &t );
	struct tm *nowtime = localtime( &t );

	SLK_NetS_ActivityList pValue = { 0 };
	for ( int activityid = 1; activityid < MAX_ACTIVITY_COUNT; activityid++ )
	{
		int starttime = g_activity_item[activityid].m_starttime;
		if ( starttime <= 0 )
			continue;
		if ( g_activity_item[activityid].m_endstat == 1 )
			continue;

		pValue.m_list[pValue.m_count].m_activityid = activityid;
		pValue.m_list[pValue.m_count].m_starttime = starttime;
		pValue.m_list[pValue.m_count].m_endtime = g_activity_item[activityid].m_endtime;
		pValue.m_list[pValue.m_count].m_closetime = g_activity_item[activityid].m_closetime;
		if ( activityid == ACTIVITY_6 )
		{ // 全服返利
			int serv_paytoken = world_data_getcache( WORLD_DATA_ACTIVITY06_PAYTOKEN );
			for ( int id = 1; id < g_activity_06_maxnum; id++ )
			{
				if ( (pCity->act06_state & (1 << id)) == 0 && serv_paytoken >= g_activity_06[id].token )
				{
					pValue.m_list[pValue.m_count].m_red = 1;
					break;
				}
			}
		}
		else if ( activityid == ACTIVITY_17 )
		{ // 充值豪礼
			for ( int id = 1; id < g_activity_17_maxnum; id++ )
			{
				if ( (g_actors[actor_index].act17_state & (1 << id)) == 0 && g_actors[actor_index].charge_point >= g_activity_17[id].pay )
				{
					pValue.m_list[pValue.m_count].m_red = 1;
					break;
				}
			}
		}
		pValue.m_count += 1;
	}

	// 首日免费
	if ( g_Config.servplat == 0 )
	{
		endtime = g_actors[actor_index].createtime + 86400;
		if ( (int)time( NULL ) < endtime || (g_actors[actor_index].act25_point > 0 && g_actors[actor_index].act25_isget == 0) )
		{
			pValue.m_list[pValue.m_count].m_activityid = ACTIVITY_25;
			pValue.m_list[pValue.m_count].m_starttime = g_actors[actor_index].createtime;
			pValue.m_list[pValue.m_count].m_endtime = endtime;
			pValue.m_list[pValue.m_count].m_closetime = endtime;
			if ( (int)time( NULL ) >= endtime )
			{
				pValue.m_list[pValue.m_count].m_red = 1;
			}
			pValue.m_count += 1;
		}
	}

	// 首充礼包
	int fristpay = city_get_sflag( pCity, CITY_SFLAG_FRISTPAY );
	int fristpay_awardget = actor_get_sflag( actor_index, ACTOR_SFLAG_FRISTPAY_AWARDGET );
	if ( fristpay == 0 || fristpay == 1 && fristpay_awardget == 0 )
	{
		pValue.m_list[pValue.m_count].m_activityid = ACTIVITY_1;
		pValue.m_list[pValue.m_count].m_starttime = g_actors[actor_index].createtime;
		pValue.m_list[pValue.m_count].m_endtime = 0;
		pValue.m_list[pValue.m_count].m_closetime = 0;
		if ( fristpay == 1 && fristpay_awardget == 0 )
		{
			pValue.m_list[pValue.m_count].m_red = 1;
		}
		pValue.m_count += 1;
	}

	// 主城等级
	char activity02_over = 1;
	for ( int id = 1; id < g_activity_02_maxnum; id++ )
	{
		if ( (g_actors[actor_index].act02_state & (1 << id)) == 0 )
		{
			activity02_over = 0;
			break;
		}
	}
	if ( activity02_over == 0 )
	{
		pValue.m_list[pValue.m_count].m_activityid = ACTIVITY_2;
		pValue.m_list[pValue.m_count].m_starttime = 0;
		pValue.m_list[pValue.m_count].m_endtime = 0;
		pValue.m_list[pValue.m_count].m_closetime = 0;
		for ( int id = 1; id < g_activity_02_maxnum; id++ )
		{
			if ( (g_actors[actor_index].act02_state & (1 << id)) == 0 && pCity->building[0].level >= g_activity_02[id].level )
			{
				pValue.m_list[pValue.m_count].m_red = 1;
				break;
			}
		}
		pValue.m_count += 1;
	}

	// 攻城略地
	char activity03_over = 1;
	for ( int id = 1; id < g_activity_03_maxnum; id++ )
	{
		if ( (g_actors[actor_index].act03_state & (1 << id)) == 0 )
		{
			activity03_over = 0;
			break;
		}
	}
	if ( activity03_over == 0 )
	{
		pValue.m_list[pValue.m_count].m_activityid = ACTIVITY_3;
		pValue.m_list[pValue.m_count].m_starttime = 0;
		pValue.m_list[pValue.m_count].m_endtime = 0;
		pValue.m_list[pValue.m_count].m_closetime = 0;
		for ( int id = 1; id < g_activity_03_maxnum; id++ )
		{
			if ( (g_actors[actor_index].act03_state & (1 << id)) == 0 && data_record_getvalue( pCity, g_activity_03[id].record_offset ) >= g_activity_03[id].needvalue )
			{
				pValue.m_list[pValue.m_count].m_red = 1;
				break;
			}
		}
		pValue.m_count += 1;
	}

	// 七日狂欢
	endtime = g_actors[actor_index].createtime + ACTIVITY_SEVENDAY_TIME;
	if ( (int)time( NULL ) < endtime )
	{
		pValue.m_list[pValue.m_count].m_activityid = ACTIVITY_4;
		pValue.m_list[pValue.m_count].m_starttime = g_actors[actor_index].createtime;
		pValue.m_list[pValue.m_count].m_endtime = endtime;
		pValue.m_list[pValue.m_count].m_closetime = endtime; 
		int myday = system_getfday() - system_getfday_withstamp( g_actors[actor_index].createtime ) + 1;
		for ( int id = 1; id < g_activity_04_maxnum; id++ )
		{
			if ( myday >= g_activity_04[id].day && g_actors[actor_index].act04_state[id] == 0 )
			{
				int value = activity_04_getvalue( actor_index, g_activity_04[id].type, id );
				if ( g_activity_04[id].type == ACTIVITY_SEVENDAY_TYPE2 )
				{
					if ( value > 0 )
					{
						pValue.m_list[pValue.m_count].m_red = 1;
						break;
					}
				}
				else
				{
					if ( value >= g_activity_04[id].value )
					{
						pValue.m_list[pValue.m_count].m_red = 1;
						break;
					}
				}
			}
		}
		pValue.m_count += 1;
	}

	// 七星拜将
	endtime = g_actors[actor_index].createtime + 5 * 86400;
	int activity05_call = actor_get_sflag( actor_index, ACTOR_SFLAG_ACTIVITY05_CALL );
	if ( (int)time( NULL ) < endtime || (activity05_call == 0 && g_actors[actor_index].act05_xw >= 7) )
	{
		pValue.m_list[pValue.m_count].m_activityid = ACTIVITY_5;
		pValue.m_list[pValue.m_count].m_starttime = g_actors[actor_index].createtime;
		pValue.m_list[pValue.m_count].m_endtime = endtime;
		pValue.m_list[pValue.m_count].m_closetime = endtime;
		if ( activity05_call == 0 && g_actors[actor_index].act05_xw >= 7 )
		{
			pValue.m_list[pValue.m_count].m_red = 1;
		}
		pValue.m_count += 1;
	}
	
	// 成长计划
	char activity08_over = 1;
	for ( int id = 1; id < g_activity_08_maxnum; id++ )
	{
		if ( (g_actors[actor_index].act08_state & (1 << id)) == 0 )
		{
			activity08_over = 0;
			break;
		}
	}
	if ( activity08_over == 0 )
	{
		pValue.m_list[pValue.m_count].m_activityid = ACTIVITY_8;
		pValue.m_list[pValue.m_count].m_starttime = 0;
		pValue.m_list[pValue.m_count].m_endtime = 0;
		pValue.m_list[pValue.m_count].m_closetime = 0;
		for ( int id = 1; id < g_activity_08_maxnum; id++ )
		{
			if ( (g_actors[actor_index].act08_state & (1 << id)) == 0 && g_actors[actor_index].level >= g_activity_08[id].level )
			{
				pValue.m_list[pValue.m_count].m_red = 1;
				break;
			}
		}
		pValue.m_count += 1;
	}
	
	// 特价礼包
	char activity10_hasbag = 0;
	for ( int tmpi = 0; tmpi < CITY_BAG_MAX; tmpi++ )
	{
		if ( pCity->bag_gid[tmpi] > 0 )
		{
			if ( (int)time( NULL ) >= pCity->bag_time[tmpi] )
			{
				pCity->bag_gid[tmpi] = 0;
				pCity->bag_time[tmpi] = 0;
				pCity->bag_num[tmpi] = 0;
			}
			else
			{
				activity10_hasbag = 1;
				break;
			}
		}
	}
	if ( activity10_hasbag == 1 )
	{
		pValue.m_list[pValue.m_count].m_activityid = ACTIVITY_10;
		pValue.m_list[pValue.m_count].m_starttime = 0;
		pValue.m_list[pValue.m_count].m_endtime = 0;
		pValue.m_list[pValue.m_count].m_closetime = 0;
		pValue.m_list[pValue.m_count].m_red = 1;
		pValue.m_count += 1;
	}

	// 出师大宴
	pValue.m_list[pValue.m_count].m_activityid = ACTIVITY_11;
	pValue.m_list[pValue.m_count].m_starttime = 0;
	pValue.m_list[pValue.m_count].m_endtime = 0;
	pValue.m_list[pValue.m_count].m_closetime = 0;
	if ( nowtime->tm_hour >= 12 && nowtime->tm_hour <= 14 && actor_get_today_char_times( actor_index, TODAY_CHAR_ACTIVITY_BODYGET1 ) == 0 )
	{ // 午宴
		pValue.m_list[pValue.m_count].m_red = 1;
	}
	else if ( nowtime->tm_hour >= 18 && nowtime->tm_hour <= 20 && actor_get_today_char_times( actor_index, TODAY_CHAR_ACTIVITY_BODYGET2 ) == 0 )
	{ // 晚宴
		pValue.m_list[pValue.m_count].m_red = 1;
	}
	pValue.m_count += 1;

	netsend_activitylist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

char activity_checkred( int actor_index )
{
	int endtime = 0;
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	time_t t;
	time( &t );
	struct tm *nowtime = localtime( &t );

	for ( int activityid = 1; activityid < MAX_ACTIVITY_COUNT; activityid++ )
	{
		int starttime = g_activity_item[activityid].m_starttime;
		if ( starttime <= 0 )
			continue;

		if ( activityid == ACTIVITY_6 )
		{ // 全服返利
			int serv_paytoken = world_data_getcache( WORLD_DATA_ACTIVITY06_PAYTOKEN );
			for ( int id = 1; id < g_activity_06_maxnum; id++ )
			{
				if ( (pCity->act06_state & (1 << id)) == 0 && serv_paytoken >= g_activity_06[id].token )
				{
					return 1;
				}
			}
		}
		//else if ( activityid == ACTIVITY_17 )
		//{ // 充值豪礼
		//	for ( int id = 1; id < g_activity_17_maxnum; id++ )
		//	{
		//		if ( (g_actors[actor_index].act17_state & (1 << id)) == 0 && g_actors[actor_index].charge_point >= g_activity_17[id].pay )
		//		{
		//			return 1;
		//		}
		//	}
		//}
		else if ( activityid == ACTIVITY_27 )
		{ // 西凉暴乱
			
		}
	}
	
	if ( nowtime->tm_hour >= 12 && nowtime->tm_hour <= 14 && actor_get_today_char_times( actor_index, TODAY_CHAR_ACTIVITY_BODYGET1 ) == 0 )
	{ // 午宴
		return 1;
	}
	else if ( nowtime->tm_hour >= 18 && nowtime->tm_hour <= 20 && actor_get_today_char_times( actor_index, TODAY_CHAR_ACTIVITY_BODYGET2 ) == 0 )
	{ // 晚宴
		return 1;
	}

	// 首充礼包
	int fristpay = city_get_sflag( pCity, CITY_SFLAG_FRISTPAY );
	int fristpay_awardget = actor_get_sflag( actor_index, ACTOR_SFLAG_FRISTPAY_AWARDGET );
	if ( fristpay == 0 || fristpay == 1 && fristpay_awardget == 0 )
	{
		if ( fristpay == 1 && fristpay_awardget == 0 )
		{
			return 1;
		}
	}

	// 特价礼包
	char activity10_hasbag = 0;
	for ( int tmpi = 0; tmpi < CITY_BAG_MAX; tmpi++ )
	{
		if ( pCity->bag_gid[tmpi] > 0 )
		{
			if ( (int)time( NULL ) >= pCity->bag_time[tmpi] )
			{
				pCity->bag_gid[tmpi] = 0;
				pCity->bag_time[tmpi] = 0;
				pCity->bag_num[tmpi] = 0;
			}
			else
			{
				return 1;
			}
		}
	}

	// 主城等级
	char activity02_over = 1;
	for ( int id = 1; id < g_activity_02_maxnum; id++ )
	{
		if ( (g_actors[actor_index].act02_state & (1 << id)) == 0 && pCity->building[0].level >= g_activity_02[id].level )
		{
			return 1;
		}
	}

	// 攻城略地
	char activity03_over = 1;
	for ( int id = 1; id < g_activity_03_maxnum; id++ )
	{
		if ( (g_actors[actor_index].act03_state & (1 << id)) == 0 && data_record_getvalue( pCity, g_activity_03[id].record_offset ) >= g_activity_03[id].needvalue )
		{
			return 1;
		}
	}
	
	// 七日狂欢
	endtime = g_actors[actor_index].createtime + ACTIVITY_SEVENDAY_TIME;
	if ( (int)time( NULL ) < endtime )
	{
		int myday = system_getfday() - system_getfday_withstamp( g_actors[actor_index].createtime ) + 1;
		for ( int id = 1; id < g_activity_04_maxnum; id++ )
		{
			if ( myday >= g_activity_04[id].day && g_actors[actor_index].act04_state[id] == 0 )
			{
				int value = activity_04_getvalue( actor_index, g_activity_04[id].type, id );
				if ( g_activity_04[id].type == ACTIVITY_SEVENDAY_TYPE2 )
				{
					if ( value > 0 )
					{
						return 1;
					}
				}
				else
				{
					if ( value >= g_activity_04[id].value )
					{
						return 1;
					}
				}
			}
		}
	}

	// 七星拜将
	endtime = g_actors[actor_index].createtime + 5 * 86400;
	int activity05_call = actor_get_sflag( actor_index, ACTOR_SFLAG_ACTIVITY05_CALL );
	if ( (int)time( NULL ) < endtime || (activity05_call == 0 && g_actors[actor_index].act05_xw >= 7) )
	{
		if ( activity05_call == 0 && g_actors[actor_index].act05_xw >= 7 )
		{
			return 1;
		}
	}

	// 成长计划
	char activity08_over = 1;
	for ( int id = 1; id < g_activity_08_maxnum; id++ )
	{
		if ( (g_actors[actor_index].act08_state & (1 << id)) == 0 && g_actors[actor_index].level >= g_activity_08[id].level )
		{
			return 1;
		}
	}
	
	return 0;
}

// 首充礼包活动
int activity_01_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_Activity01List pValue = { 0 };
	pValue.m_fristpay = city_get_sflag( pCity, CITY_SFLAG_FRISTPAY );
	pValue.m_fristpay_award = actor_get_sflag( actor_index, ACTOR_SFLAG_FRISTPAY_AWARDGET );

	AwardGetInfo getinfo = { 0 };
	awardgroup_random( 164, -1, &getinfo );
	for ( int tmpi = 0; tmpi < getinfo.count; tmpi++ )
	{
		pValue.m_list[pValue.m_count].m_kind = getinfo.kind[tmpi];
		pValue.m_list[pValue.m_count].m_num = getinfo.num[tmpi];
		pValue.m_count++;
		if ( pValue.m_count >= 8 )
		{
			break;
		}
	}
	netsend_activity01list_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
int activity_01_get( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( actor_get_sflag( actor_index, ACTOR_SFLAG_FRISTPAY_AWARDGET ) == 1 )
		return -1;
	awardgroup_withindex( actor_index, 164, -1, PATH_ACTIVITY, NULL );
	actor_set_sflag( actor_index, ACTOR_SFLAG_FRISTPAY_AWARDGET, 1 );
	activity_01_sendinfo( actor_index );
	actor_redinfo( actor_index, 2 );
	return 0;
}

// 主城等级活动
int activity_02_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int value[3] = { 0 };
	value[0] = ACTIVITY_2;
	value[1] = 0;
	value[2] = g_actors[actor_index].act02_state;
	actor_notify_value( actor_index, NOTIFY_ACTIVITY, 3, value, NULL );
	return 0;
}
int activity_02_get( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( id <= 0 || id >= g_activity_02_maxnum )
		return -1;
	if ( g_actors[actor_index].act02_state & (1 << id) )
		return -1;
	if ( pCity->building[0].level < g_activity_02[id].level )
		return -1;
	for ( int tmpi = 0; tmpi < 3; tmpi++ )
	{
		if ( g_activity_02[id].awardkind[tmpi] <= 0 )
			continue;
		award_getaward( actor_index, g_activity_02[id].awardkind[tmpi], g_activity_02[id].awardnum[tmpi], -1, PATH_ACTIVITY, NULL );
	}
	g_actors[actor_index].act02_state |= (1 << id);
	activity_02_sendinfo( actor_index );
	return 0;
}

// 攻城略地活动
int activity_03_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_Activity03List pValue = { 0 };
	for ( int id = 1; id < g_activity_03_maxnum; id++ )
	{
		pValue.m_list[pValue.m_count].m_value = data_record_getvalue( pCity, g_activity_03[id].record_offset );
		if ( g_actors[actor_index].act03_state & (1 << id))
			pValue.m_list[pValue.m_count].m_state = 1;
		else
			pValue.m_list[pValue.m_count].m_state = 0;
		pValue.m_count += 1;
	}
	netsend_activity03list_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

int activity_03_get( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( id <= 0 || id >= g_activity_03_maxnum )
		return -1;
	if ( data_record_getvalue( pCity, g_activity_03[id].record_offset ) < g_activity_03[id].needvalue )
		return -1;
	if ( g_actors[actor_index].act03_state & (1 << id) )
		return -1;
	for ( int tmpi = 0; tmpi < 5; tmpi++ )
	{
		if ( g_activity_03[id].awardkind[tmpi] <= 0 )
			continue;
		award_getaward( actor_index, g_activity_03[id].awardkind[tmpi], g_activity_03[id].awardnum[tmpi], -1, PATH_ACTIVITY, NULL );
	}
	g_actors[actor_index].act03_state |= (1 << id);
	activity_03_sendinfo( actor_index );
	return 0;
}

// 七星拜将活动
int activity_05_item_random( int *kindlist, int kindnum, int *totalvalue )
{
	int outid = 0;
	int allvalue = *totalvalue;
	int curvalue = 0;
	int odds = allvalue > 0 ? rand() % allvalue : 0;
	for ( int tmpi = 0; tmpi < kindnum; tmpi++ )
	{
		// 按照评价值方式随机
		int id = kindlist[tmpi];
		if ( id <= 0 || id >= g_activity_05_maxnum )
			continue;
		curvalue = g_activity_05[id].weight;
		if ( curvalue > 0 && curvalue > odds )
		{
			outid = id;
			kindlist[tmpi] = 0;
			*totalvalue -= curvalue;
			break;
		}
		odds -= curvalue;
	}
	return outid;
}
int activity_05_item_update( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );

	int kindlist[64] = { 0 };
	int kindnum = 0;
	int allvalue = 0;
	for ( int id = 1; id < g_activity_05_maxnum; id++ )
	{
		int awardkind = g_activity_05[id].awardkind;
		if ( awardkind <= 0 )
			continue;
		kindlist[kindnum] = id;
		kindnum++;
		allvalue += g_activity_05[id].weight;
	}

	g_actors[actor_index].act05_item[0] = activity_05_item_random( kindlist, kindnum, &allvalue );
	g_actors[actor_index].act05_item[1] = activity_05_item_random( kindlist, kindnum, &allvalue );
	g_actors[actor_index].act05_item[2] = activity_05_item_random( kindlist, kindnum, &allvalue );
	g_actors[actor_index].act05_item[3] = activity_05_item_random( kindlist, kindnum, &allvalue );
	g_actors[actor_index].act05_item[4] = activity_05_item_random( kindlist, kindnum, &allvalue );
	if ( actor_get_sflag( actor_index, ACTOR_SFLAG_ACTIVITY05_CALL ) == 1 || g_actors[actor_index].act05_xw >= 7 )
	{
		g_actors[actor_index].act05_item[5] = activity_05_item_random( kindlist, kindnum, &allvalue );
	}
	else
		g_actors[actor_index].act05_item[5] = 1;
	g_actors[actor_index].act05_buynum = 0;
	g_actors[actor_index].act05_isbuy = 0;
	return 0;
}
int activity_05_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );

	// 第一次刷新
	if ( g_actors[actor_index].act05_item[0] == 0 )
	{
		activity_05_item_update( actor_index );
		g_actors[actor_index].act05_upnum = 4;
		g_actors[actor_index].act05_upstamp = 0;
	}

	if ( g_actors[actor_index].act05_upstamp > 0 )
	{
		if ( (int)time( NULL ) > g_actors[actor_index].act05_upstamp )
		{ // 倒计时
			int add = 0;
			int spacetime = (int)time( NULL ) - g_actors[actor_index].act05_upstamp;
			if ( spacetime >= global.activity05_update_sec * 3 )
				add = 4;
			else if ( spacetime >= global.activity05_update_sec * 2 )
				add = 3;
			else if ( spacetime >= global.activity05_update_sec * 1 )
				add = 2;
			else
				add = 1;

			g_actors[actor_index].act05_upnum += add;
			g_actors[actor_index].act05_upstamp += spacetime + global.activity05_update_sec;
			if ( g_actors[actor_index].act05_upnum >= 4 )
			{
				g_actors[actor_index].act05_upnum = 4;
				g_actors[actor_index].act05_upstamp = 0;
			}
		}
	}

	SLK_NetS_Activity05List pValue = { 0 };
	for ( int tmpi = 0; tmpi < 6; tmpi++ )
	{
		int id = g_actors[actor_index].act05_item[tmpi];
		if ( id > 0 && id < g_activity_05_maxnum )
		{
			pValue.m_list[pValue.m_count].m_awardkind = g_activity_05[id].awardkind;
			pValue.m_list[pValue.m_count].m_awardnum = g_activity_05[id].awardnum;
			int token = g_activity_05[id].token;
			if ( g_actors[actor_index].act05_buynum == 1 )
				token = (int)(g_activity_05[id].token*0.9f);
			else if ( g_actors[actor_index].act05_buynum == 2 )
				token = (int)(g_activity_05[id].token*0.8f);
			else if ( g_actors[actor_index].act05_buynum == 3 )
				token = (int)(g_activity_05[id].token*0.7f);
			else if ( g_actors[actor_index].act05_buynum == 4 )
				token = (int)(g_activity_05[id].token*0.6f);
			else if ( g_actors[actor_index].act05_buynum >= 5 )
				token = (int)(g_activity_05[id].token*0.5f);
			pValue.m_list[pValue.m_count].m_token = token;
			if ( g_actors[actor_index].act05_isbuy & (1 << tmpi) )
				pValue.m_list[pValue.m_count].m_isbuy = 1;
			else
				pValue.m_list[pValue.m_count].m_isbuy = 0;
		}
		pValue.m_count += 1;
	}
	pValue.m_buynum = g_actors[actor_index].act05_buynum;
	pValue.m_updatenum = g_actors[actor_index].act05_upnum;
	pValue.m_updatestamp = g_actors[actor_index].act05_upstamp - (int)time(NULL);
	pValue.m_myxw = g_actors[actor_index].act05_xw;
	netsend_activity05list_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
int activity_05_buy( int actor_index, int index )
{
	ACTOR_CHECK_INDEX( actor_index );
	int endtime = g_actors[actor_index].createtime + 3 * 86400;
	if ( (int)time( NULL ) > endtime )
	{
		actor_notify_alert( actor_index, 2481 );//活动已经结束，请尽快领取奖励！
		return -1;
	}
	if ( index < 0 || index >= 6 )
		return -1;
	int id = g_actors[actor_index].act05_item[index];
	if ( id <= 0 || id >= g_activity_05_maxnum )
		return -1;
	if ( g_actors[actor_index].act05_isbuy & (1 << index) )
		return -1;

	int token = g_activity_05[id].token;
	if ( g_actors[actor_index].act05_buynum == 1 )
		token = (int)(g_activity_05[id].token*0.9f);
	else if ( g_actors[actor_index].act05_buynum == 2 )
		token = (int)(g_activity_05[id].token*0.8f);
	else if ( g_actors[actor_index].act05_buynum == 3 )
		token = (int)(g_activity_05[id].token*0.7f);
	else if ( g_actors[actor_index].act05_buynum == 4 )
		token = (int)(g_activity_05[id].token*0.6f);
	else if ( g_actors[actor_index].act05_buynum >= 5 )
		token = (int)(g_activity_05[id].token*0.5f);

	if ( actor_change_token( actor_index, -token, PATH_ACTIVITY, 0 ) < 0 )
		return -1;

	award_getaward( actor_index, g_activity_05[id].awardkind, g_activity_05[id].awardnum, -1, PATH_ACTIVITY, NULL );
	g_actors[actor_index].act05_isbuy |= (1 << index);
	g_actors[actor_index].act05_buynum += 1;
	activity_05_sendinfo( actor_index );
	return 0;
}
int activity_05_update( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	int endtime = g_actors[actor_index].createtime + 3 * 86400;
	if ( (int)time( NULL ) > endtime )
	{
		actor_notify_alert( actor_index, 2481 );//活动已经结束，请尽快领取奖励！
		return -1;
	}
	if ( g_actors[actor_index].act05_upnum <= 0 )
	{
		if ( actor_change_token( actor_index, -global.activity05_update_token, PATH_ACTIVITY, 0 ) < 0 )
			return -1;
		activity_05_item_update( actor_index );
		activity_05_sendinfo( actor_index );
		return 0;
	}
	activity_05_item_update( actor_index );
	g_actors[actor_index].act05_upnum -= 1;
	if ( g_actors[actor_index].act05_upnum < 4 && g_actors[actor_index].act05_upstamp == 0 )
	{
		g_actors[actor_index].act05_upstamp = (int)time(NULL) + global.activity05_update_sec;
	}
	activity_05_sendinfo( actor_index );
	return 0;
}
int activity_05_callhero( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( g_actors[actor_index].act05_xw < 7 )
	{
		return -1;
	}
	if ( actor_get_sflag( actor_index, ACTOR_SFLAG_ACTIVITY05_CALL ) == 1 )
	{
		return -1;
	}
	g_actors[actor_index].act05_xw -= 7;
	hero_gethero( actor_index, 49, PATH_ACTIVITY );
	actor_set_sflag( actor_index, ACTOR_SFLAG_ACTIVITY05_CALL, 1 );
	activity_05_sendinfo( actor_index );
	return 0;
}

// 全服返利活动
void activity_06_onopen()
{
	g_nation[1].paytoken = 0;
	g_nation[2].paytoken = 0;
	g_nation[3].paytoken = 0;
	world_data_set( WORLD_DATA_ACTIVITY06_PAYTOKEN, 0, NULL, NULL );
}
void activity_06_onend()
{
	int max_nation = 1;
	if ( g_nation[2].paytoken > g_nation[1].paytoken )
	{
		max_nation = 2;
	}
	if ( g_nation[3].paytoken > g_nation[max_nation].paytoken )
	{
		max_nation = 3;
	}
	max_nation = max_nation - 1;

	char attach[256] = { 0 };
	awardgroup_mail( 169, 0, attach );

	for ( int tmpi = 0; tmpi < g_city_maxcount; tmpi++ )
	{
		if ( g_city[tmpi].actorid < MINACTORID )
			continue;
		if ( g_city[tmpi].nation == max_nation )
		{
			mail_system( MAIL_ACTORID, g_city[tmpi].actorid, 5047, 5542, NULL, NULL, NULL, attach, 1 );
		}
	}
}
void activity_06_onclose()
{

}
int activity_06_sendinfo( int actor_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int value[7] = { 0 };
	value[0] = ACTIVITY_6;
	value[1] = 0;
	value[2] = world_data_getcache( WORLD_DATA_ACTIVITY06_PAYTOKEN );
	value[3] = nation_paytoken_get( 1 );
	value[4] = nation_paytoken_get( 2 );
	value[5] = nation_paytoken_get( 3 );
	value[6] = pCity->act06_state;
	actor_notify_value( actor_index, NOTIFY_ACTIVITY, 7, value, NULL );
	return 0;
}
int activity_06_get( int actor_index, int id )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( id <= 0 || id >= g_activity_06_maxnum )
		return -1;
	if ( pCity->act06_state & (1 << id) )
		return -1;
	int serv_paytoken = world_data_getcache( WORLD_DATA_ACTIVITY06_PAYTOKEN );
	if ( serv_paytoken < g_activity_06[id].token )
		return -1;
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		if ( g_activity_06[id].awardkind[tmpi] <= 0 )
			continue;
		award_getaward( actor_index, g_activity_06[id].awardkind[tmpi], g_activity_06[id].awardnum[tmpi], -1, PATH_ACTIVITY, NULL );
	}
	pCity->act06_state |= (1 << id);
	activity_06_sendinfo( actor_index );
	return 0;
}

// 成长计划活动
int activity_08_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	int value[4] = { 0 };
	value[0] = ACTIVITY_8;
	value[1] = 0;
	value[2] = actor_get_sflag( actor_index, ACTOR_SFLAG_ACTIVITY08_OPEN );
	value[3] = g_actors[actor_index].act08_state;
	actor_notify_value( actor_index, NOTIFY_ACTIVITY, 4, value, NULL );
	return 0;
}
int activity_08_open( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( actor_get_sflag( actor_index, ACTOR_SFLAG_ACTIVITY08_OPEN ) == 1 )
		return -1;
	if ( pCity->viplevel < global.activity08_viplevel )
		return -1;
	if ( actor_change_token( actor_index, -global.activity08_token, PATH_ACTIVITY, 0 ) < 0 )
		return -1;
	actor_set_sflag( actor_index, ACTOR_SFLAG_ACTIVITY08_OPEN, 1 );
	activity_08_sendinfo( actor_index );
	return 0;
}
int activity_08_get( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( id <= 0 || id >= g_activity_08_maxnum )
		return -1;
	if ( actor_get_sflag( actor_index, ACTOR_SFLAG_ACTIVITY08_OPEN ) == 0 )
		return -1;
	if ( g_actors[actor_index].act08_state & (1 << id) )
		return -1;
	if ( pCity->level < g_activity_08[id].level )
		return -1;
	for ( int tmpi = 0; tmpi < 5; tmpi++ )
	{
		if ( g_activity_08[id].awardkind[tmpi] <= 0 )
			continue;
		award_getaward( actor_index, g_activity_08[id].awardkind[tmpi], g_activity_08[id].awardnum[tmpi], -1, PATH_ACTIVITY, NULL );
	}
	g_actors[actor_index].act08_state |= (1 << id);
	activity_08_sendinfo( actor_index );
	return 0;
}

// 特价礼包
int activity_10_init()
{
	for ( int id = 1; id < g_activity_10_maxnum; id++ )
	{
		if ( g_activity_10[id].goodsid > 0 )
		{
			paybag_insert( g_activity_10[id].goodsid, (int)time( NULL ), (int)time( NULL ) + g_activity_10[id].duration, g_activity_10[id].buynum );
		}
	}
	return 0;
}

// 出师大宴活动
int activity_11_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	int value[4] = { 0 };
	value[0] = ACTIVITY_11;
	value[1] = 0;
	value[2] = actor_get_today_char_times( actor_index, TODAY_CHAR_ACTIVITY_BODYGET1 );
	value[3] = actor_get_today_char_times( actor_index, TODAY_CHAR_ACTIVITY_BODYGET2 );
	actor_notify_value( actor_index, NOTIFY_ACTIVITY, 4, value, NULL );
	return 0;
}

int activity_11_get( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	time_t t;
	time( &t );
	struct tm *nowtime = localtime( &t );

	if ( nowtime->tm_hour >= 12 && nowtime->tm_hour <= 14 )
	{ // 午宴
		if ( actor_get_today_char_times( actor_index, TODAY_CHAR_ACTIVITY_BODYGET1 ) > 0 )
		{
			return -1;
		}
		awardgroup_withindex( actor_index, 163, -1, PATH_ACTIVITY, NULL );
		actor_add_today_char_times( actor_index, TODAY_CHAR_ACTIVITY_BODYGET1 );
	}
	else if ( nowtime->tm_hour >= 18 && nowtime->tm_hour <= 20 )
	{ // 晚宴
		if ( actor_get_today_char_times( actor_index, TODAY_CHAR_ACTIVITY_BODYGET2 ) > 0 )
		{
			return -1;
		}
		awardgroup_withindex( actor_index, 163, -1, PATH_ACTIVITY, NULL );
		actor_add_today_char_times( actor_index, TODAY_CHAR_ACTIVITY_BODYGET2 );
	}
	else
		return -1;
	activity_11_sendinfo( actor_index );
	return 0;
}

// 南蛮入侵
void activity_12_onwarning( int lefttime )
{
	if ( lefttime < 0 )
		return;
	int countdown = lefttime / 60; //倒计时（分钟）

	// 6036	南蛮入侵活动将在{0}分钟后开启，请参加守城的玩家前往活动信息页面开启活动！
	if ( countdown == 10 || countdown == 5 || countdown == 1 )
	{
		char v1[64] = { 0 };
		sprintf( v1, "%d", countdown );
		system_talkjson_world( 6036, v1, NULL, NULL, NULL, NULL, NULL, 2 );
	}
}
void activity_12_onopen()
{
	for ( int tmpi = 0; tmpi < g_city_maxcount; tmpi++ )
	{
		if ( g_city[tmpi].actorid < MINACTORID )
			continue;
		g_city[tmpi].act12_state = 0;
		g_city[tmpi].act12_turn = 0;
		g_city[tmpi].act12_idx = -1;
	}
	SLK_NetS_RedInfo pValue = { 0 };
	pValue.m_path = 6;
	pValue.m_has = 1;
	netsend_redinfo_S( 0, SENDTYPE_WORLD, &pValue );
}
void activity_12_onend()
{
	SLK_NetS_RedInfo pValue = { 0 };
	pValue.m_path = 6;
	pValue.m_has = 0;
	netsend_redinfo_S( 0, SENDTYPE_WORLD, &pValue );
}
void activity_12_onclose()
{
	for ( int tmpi = 0; tmpi < g_city_maxcount; tmpi++ )
	{
		if ( g_city[tmpi].actorid < MINACTORID )
			continue;
		g_city[tmpi].act12_state = 0;
		g_city[tmpi].act12_turn = 0;
		g_city[tmpi].act12_idx = -1;
	}
	map_activity_delete_withactivityid( ACTIVITY_12 );
}
void activity_12_onlogic()
{
}
int activity_12_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	int value[5] = { 0 };
	value[0] = ACTIVITY_12;
	value[1] = pCity->act12_state;
	value[2] = pCity->act12_turn;
	if ( pCity->act12_idx >= 0 && pCity->act12_idx < g_map_activity_maxcount )
	{
		int army_index = g_map_activity[pCity->act12_idx].army_index;
		if ( army_index >= 0 && army_index < g_army_maxcount )
		{
			value[3] = g_army[army_index].statetime;
			value[4] = g_army[army_index].stateduration;
		}
		else
		{
			pCity->act12_state = 2;
			value[1] = pCity->act12_state;
		}
	}
	actor_notify_value( actor_index, NOTIFY_ACTIVITY, 5, value, NULL );

	// 发奖励
	if ( pCity->act12_state == 1 && pCity->act12_turn > 0 && pCity->act12_turn < g_activity_12_maxnum )
	{
		SLK_NetS_AwardInfoList pValue = { 0 };
		pValue.m_callback_code = 7;
		pValue.m_value = pCity->act12_turn;
		pValue.m_count = 0;
		
		for ( int i = 0; i < 4; i++ )
		{
			if ( g_activity_12[pCity->act12_turn].awardkind[i] > 0 )
			{
				pValue.m_list[pValue.m_count].m_kind = g_activity_12[pCity->act12_turn].awardkind[i];
				pValue.m_list[pValue.m_count].m_num = g_activity_12[pCity->act12_turn].awardnum[i];
				pValue.m_count += 1;
			}
		}

		netsend_awardinfolist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}
	return 0;
}
int activity_12_manualopen( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( activity_intime( ACTIVITY_12 ) == 0 )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->act06_state == 1 )
		return -1;
	pCity->act12_state = 1;
	pCity->act12_turn = 1;

	int unit_index = map_activity_range_brush( 2, pCity->posx, pCity->posy, 4, 0, 0, &pCity->act12_idx );
	if ( unit_index < 0 )
	{
		return -1;
	}
	activity_12_createarmy( pCity );
	activity_12_sendinfo( actor_index );
	return 0;
}

int activity_12_createarmy( City *pCity )
{
	if ( !pCity )
		return -1;
	if ( pCity->act12_idx < 0 || pCity->act12_idx >= g_map_activity_maxcount )
		return -1;
	SLK_NetC_MapBattle info = { 0 };
	info.m_to_unit_type = MAPUNIT_TYPE_CITY;
	info.m_to_unit_index = pCity->unit_index;
	info.m_id = pCity->actorid;
	info.m_to_posx = pCity->posx;
	info.m_to_posy = pCity->posy;
	info.m_appdata = pCity->act12_turn;
	info.m_action = ARMY_ACTION_FIGHT;
	info.m_group_index = -1;
	info.m_herokind[0] = 1002;
	int army_index = army_create( MAPUNIT_TYPE_ACTIVITY, pCity->act12_idx, MAPUNIT_TYPE_CITY, pCity->actorid, ARMY_STATE_MARCH, &info );
	if ( army_index < 0 )
		return -1;
	g_army[army_index].totals = 0;
	if ( pCity->act12_idx >= 0 && pCity->act12_idx < g_map_activity_maxcount )
	{
		map_activity_setarmy( pCity->act12_idx, army_index );
	}
	// 目标是玩家
	City *pTargetCity = army_getcityptr_target( army_index );
	if ( pTargetCity )
	{
		if ( info.m_action == ARMY_ACTION_FIGHT )
		{// 被攻击列表
			city_underfire_add( pTargetCity, army_index );
		}
	}
	return 0;
}

// 计算行军时间
int activity_12_army_marchtime( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	int index = g_army[army_index].appdata;
	if ( index > 0 && index < g_activity_12_maxnum )
	{
		g_army[army_index].stateduration = g_activity_12[index].marchtime;
	}
	else
	{
		g_army[army_index].stateduration = 120;
	}
	return 0;
}

// 充值豪礼
void activity_17_onopen()
{
	
}
void activity_17_onend()
{
	
}
void activity_17_onclose()
{

}
int activity_17_sendinfo( int actor_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_Activity17 pValue = { 0 };
	for ( int tmpi = 0; tmpi < g_activity_17_maxnum; tmpi++ )
	{
		if ( g_activity_17[tmpi].id <= 0 )
			continue;
		pValue.m_list[pValue.m_count].m_id = g_activity_17[tmpi].id;
		pValue.m_list[pValue.m_count].m_pay = g_activity_17[tmpi].pay;
		for ( int i = 0; i < 16; i++ )
		{
			pValue.m_list[pValue.m_count].m_awardkind[i] = g_activity_17[tmpi].awardkind[i];
			pValue.m_list[pValue.m_count].m_awardnum[i] = g_activity_17[tmpi].awardnum[i];
		}
		if ( g_actors[actor_index].act17_state & (1 << g_activity_17[tmpi].id) )
		{
			pValue.m_list[pValue.m_count].m_isget = 1;
		}
		else
		{
			pValue.m_list[pValue.m_count].m_isget = 0;
		}
		pValue.m_count += 1;
	}
	pValue.m_mypay = g_actors[actor_index].charge_point;
	netsend_activity17_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
int activity_17_get( int actor_index, int id )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( id <= 0 || id >= g_activity_17_maxnum )
		return -1;
	if ( g_actors[actor_index].act17_state & (1 << id) )
		return -1;

	int equipcount = 0;
	for ( int tmpi = 0; tmpi < 16; tmpi++ )
	{
		if ( g_activity_17[id].awardkind[tmpi] > AWARDKIND_EQUIPBASE && g_activity_17[id].awardkind[tmpi] < AWARDKIND_HEROBASE )
		{
			equipcount += g_activity_17[id].awardnum[tmpi];
		}
	}

	if ( equip_getempty( actor_index ) < equipcount )
	{
		char v1[32] = { 0 };
		sprintf( v1, "%d", equipcount );
		actor_notify_pop_v( actor_index, 526, v1, NULL );
		return -1;
	}
	for ( int tmpi = 0; tmpi < 16; tmpi++ )
	{
		if ( g_activity_17[id].awardkind[tmpi] <= 0 )
			continue;
		award_getaward( actor_index, g_activity_17[id].awardkind[tmpi], g_activity_17[id].awardnum[tmpi], -1, PATH_ACTIVITY, NULL );
	}
	g_actors[actor_index].act17_state |= (1 << id);

	int value = 0;
	actor_notify_value( actor_index, NOTIFY_DELAYQUEUEPLAY, 1, &value, NULL );

	activity_17_sendinfo( actor_index );
	actor_redinfo( actor_index, 5 );
	return 0;
}

// 首日免费
int activity_25_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	int value[5] = { 0 };
	value[0] = ACTIVITY_25;
	value[1] = g_actors[actor_index].createtime + 86400 - (int)time(NULL);
	value[2] = g_actors[actor_index].act25_point;
	value[3] = g_actors[actor_index].act25_point * 10;
	value[4] = g_actors[actor_index].act25_isget;
	actor_notify_value( actor_index, NOTIFY_ACTIVITY, 5, value, NULL );
	return 0;
}

int activity_25_get( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( g_actors[actor_index].createtime + 86400 - (int)time( NULL ) > 0 )
		return -1;
	if ( g_actors[actor_index].act25_isget == 1 )
		return -1;
	if ( g_actors[actor_index].act25_point <= 0 )
		return -1;
	actor_change_token( actor_index, g_actors[actor_index].act25_point * 10, PATH_ACTIVITY25, 0 );
	g_actors[actor_index].act25_isget = 1;
	activity_25_sendinfo( actor_index );
	actor_redinfo( actor_index, 4 );
	return 0;
}

// 西凉暴乱
int g_activity_27_brushtime = 0;
void activity_27_onwarning( int lefttime )
{
	if ( lefttime < 0 )
		return;
	int countdown = lefttime / 60; //倒计时（分钟）
	
	// 6033	据前哨来报，西凉暴乱，{0}分钟后将出现大量西凉铁骑，各位英雄速速前往平叛！
	if ( countdown == 10 || countdown == 5 || countdown == 1 )
	{
		char v1[64] = { 0 };
		sprintf( v1, "%d", countdown );
		system_talkjson_world( 6033, v1, NULL, NULL, NULL, NULL, NULL, 2 );
	}
}
void activity_27_onopen()
{
	g_activity_27_brushtime = 0;
	activity_27_brush();

	SLK_NetS_RedInfo pValue = { 0 };
	pValue.m_path = 7;
	pValue.m_has = 1;
	netsend_redinfo_S( 0, SENDTYPE_WORLD, &pValue );
}
void activity_27_onend()
{
	// 6035	西凉叛军已被剿灭，我们大获全胜，还望各位英雄加固城防，积蓄粮草，训练勇士，以防叛军再犯！
	system_talkjson_world( 6035, NULL, NULL, NULL, NULL, NULL, NULL, 2 );
	map_activity_delete_withactivityid( ACTIVITY_27 );

	SLK_NetS_RedInfo pValue = { 0 };
	pValue.m_path = 7;
	pValue.m_has = 0;
	netsend_redinfo_S( 0, SENDTYPE_WORLD, &pValue );
}
void activity_27_onclose()
{
	
}
void activity_27_onlogic()
{
	g_activity_27_brushtime += 1;
	if ( g_activity_27_brushtime >= global.activity27_brushmin )
	{
		g_activity_27_brushtime = 0;
		activity_27_brush();

		// 6034	西凉铁骑增援部队出现！
		system_talkjson_world( 6034, NULL, NULL, NULL, NULL, NULL, NULL, 2 );
	}
}
int activity_27_brush()
{
	// 按地区刷新
	for ( char zoneid = 1; zoneid < g_zoneinfo_maxnum; zoneid++ )
	{
		brush_enemy_queue_add( BRUSH_ENEMY_QUEUE_ACTIVITY27, zoneid );
	}	
	return 0;
}
int activity_27_brush_withzoneid( int zoneid )
{
	// 获得已经存在的
	int hasnum = map_activity_num_withactivityid( zoneid, ACTIVITY_27 );
	int brushnum = global.activity27_brushcount - hasnum;
	if ( brushnum > 0 )
	{
		// 随机所有的空余位置
		map_zone_getemptypos( zoneid );
		for ( int tmpi = 0; tmpi < brushnum; tmpi++ )
		{
			map_activity_create( 1, g_mapzone_emptypos[tmpi].x, g_mapzone_emptypos[tmpi].y, 0, 0, NULL );
		}
	}
	return 0;
}
int activity_27_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	return 0;
}

// 充值排名活动
Activity33Rank g_activity33_rank[ACTIVITY33_MEMBERMAX];
int activity_33_load()
{
	for ( int tmpi = 0; tmpi < ACTIVITY33_MEMBERMAX; tmpi++ )
	{
		g_activity33_rank[tmpi].actorid = 0;
		g_activity33_rank[tmpi].city_index = -1;
	}
	if ( activity_intime( ACTIVITY_33 ) == 0 )
		return 0;
	for ( int tmpi = 0; tmpi < g_city_maxcount; tmpi++ )
	{
		if ( g_city[tmpi].actorid < MINACTORID )
			continue;
		activity_33_calc_rank( &g_city[tmpi] );
	}
	return 0;
}

void activity_33_onopen()
{
	for ( int tmpi = 0; tmpi < g_city_maxcount; tmpi++ )
	{ // 重置数据
		if ( g_city[tmpi].actorid < MINACTORID )
			continue;
		g_city[tmpi].act33_pay = 0;
	}
	for ( int tmpi = 0; tmpi < ACTIVITY33_MEMBERMAX; tmpi++ )
	{
		g_activity33_rank[tmpi].actorid = 0;
		g_activity33_rank[tmpi].city_index = -1;
	}
}

void activity_33_onend()
{
	// 发奖励
	for ( int tmpi = 0; tmpi < ACTIVITY33_MEMBERMAX; tmpi++ )
	{
		if ( g_activity33_rank[tmpi].actorid <= 0 )
			continue;
		int id = 0;
		if ( tmpi == 0 )
		{
			id = 1;
		}
		else if ( tmpi == 1 )
		{
			id = 2;
		}
		else if ( tmpi == 2 )
		{
			id = 3;
		}
		else if ( tmpi >= 3 && tmpi <= 8 )
		{
			id = 4;
		}
		else if ( tmpi >= 9 && tmpi <= 38 )
		{
			id = 5;
		}
		else if ( tmpi >= 39 && tmpi <= 68 )
		{
			id = 6;
		}
		else if ( tmpi >= 69 && tmpi <= 99 )
		{
			id = 7;
		}

		char attach[256] = { 0 };
		for ( int i = 0; i < 5; i++ )
		{
			if ( g_activity_33[id].awardkind[i] <= 0 )
				continue;
			char tempitem[64] = { 0 };
			sprintf( tempitem, "%d,%d@", g_activity_33[id].awardkind[i], g_activity_33[id].awardnum[i] );
			strcat( attach, tempitem );
		}
		char v1[32] = { 0 };
		sprintf( v1, "%d", (tmpi+1) );
		mail_system( MAIL_ACTORID, g_activity33_rank[tmpi].actorid, 5050, 5545, v1, NULL, NULL, attach, 1 );
	}
}

void activity_33_onclose()
{
	for ( int tmpi = 0; tmpi < g_city_maxcount; tmpi++ )
	{ // 重置数据
		if ( g_city[tmpi].actorid < MINACTORID )
			continue;
		g_city[tmpi].act33_pay = 0;
	}
}

void activity_33_calc_rank( City *pCity )
{
	if ( !pCity || pCity ->act33_pay <= 0 )
		return;

	// 变化之前的第一名
	int one_city_index = g_activity33_rank[0].city_index;

	// 找到我的排名，把我踢出
	int my_tmpi = -1;
	for ( int tmpi = 0; tmpi < ACTIVITY33_MEMBERMAX; tmpi++ )
	{
		if ( g_activity33_rank[tmpi].actorid == pCity->actorid )
		{
			my_tmpi = tmpi;
			break;
		}
	}
	if ( my_tmpi >= 0 && my_tmpi < ACTIVITY33_MEMBERMAX - 1 )
	{
		// 往前移
		memmove( &g_activity33_rank[my_tmpi], &g_activity33_rank[my_tmpi + 1], sizeof( Activity33Rank )*(ACTIVITY33_MEMBERMAX - my_tmpi - 1) );
		g_activity33_rank[ACTIVITY33_MEMBERMAX - 1].actorid = 0;
		g_activity33_rank[ACTIVITY33_MEMBERMAX - 1].city_index = -1;
	}
	else if ( my_tmpi == ACTIVITY33_MEMBERMAX - 1 )
	{
		g_activity33_rank[my_tmpi].actorid = 0;
		g_activity33_rank[my_tmpi].city_index = -1;
	}

	// 跟前100名逐一比较
	int replace_tmpi = -1;
	for ( int tmpi = 0; tmpi < ACTIVITY33_MEMBERMAX; tmpi++ )
	{
		int value = 0;
		int city_index = g_activity33_rank[tmpi].city_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
		{
			value = g_city[city_index].act33_pay;
		}

		if ( pCity->act33_pay > value )
		{
			replace_tmpi = tmpi;
			break;
		}
	}

	if ( replace_tmpi >= 0 && replace_tmpi < ACTIVITY33_MEMBERMAX - 1 )
	{
		// 往后移
		memmove( &g_activity33_rank[replace_tmpi + 1], &g_activity33_rank[replace_tmpi], sizeof( Activity33Rank )*(ACTIVITY33_MEMBERMAX - replace_tmpi - 1) );
		g_activity33_rank[replace_tmpi].actorid = pCity->actorid;
		g_activity33_rank[replace_tmpi].city_index = pCity->index;
	}
	else if ( replace_tmpi == ACTIVITY33_MEMBERMAX - 1 )
	{
		g_activity33_rank[replace_tmpi].actorid = pCity->actorid;
		g_activity33_rank[replace_tmpi].city_index = pCity->index;
	}

	if ( my_tmpi > 0 && replace_tmpi == 0 )
	{//  6032	恭喜<color=03DE27FF>[{0}]{1}</color>在充值排行活动中暂时位居第一
		char v1[64] = { 0 };
		char v2[64] = { 0 };
		sprintf( v1, "%s%d", TAG_NATION, pCity->nation );
		sprintf( v2, "%s", pCity->name );
		system_talkjson_world( 6032, v1, v2, NULL, NULL, NULL, NULL, 2 );
	}
}

int activity_33_addvalue( int actor_index, int value )
{
	if ( activity_intime( ACTIVITY_33 ) == 0 )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	pCity->act33_pay += value;
	activity_33_calc_rank( pCity );
	return 0;
}

int activity_33_sendinfo( int actor_index )
{
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_Activity33 pValue = {0};
	// 发送前10名
	for ( int tmpi = 0; tmpi < 10; tmpi++ )
	{
		int value = 0;
		int city_index = g_activity33_rank[tmpi].city_index;
		if ( city_index >= 0 && city_index < g_city_maxcount )
		{
			strncpy( pValue.m_list[pValue.m_count].m_name, g_city[city_index].name, NAME_SIZE );
			pValue.m_list[pValue.m_count].m_namelen = strlen( pValue.m_list[pValue.m_count].m_name );
			pValue.m_list[pValue.m_count].m_nation = g_city[city_index].nation;
			pValue.m_list[pValue.m_count].m_pay = g_city[city_index].act33_pay;
			pValue.m_count += 1;
		}
	}
	// 我的排名
	int my_tmpi = -1;
	for ( int tmpi = 0; tmpi < ACTIVITY33_MEMBERMAX; tmpi++ )
	{
		if ( g_activity33_rank[tmpi].actorid == pCity->actorid )
		{
			my_tmpi = tmpi;
			break;
		}
	}
	pValue.m_myrank = my_tmpi;
	pValue.m_mypay = pCity->act33_pay;

	// 发送奖励
	for ( int tmpi = 1; tmpi < g_activity_33_maxnum; tmpi++ )
	{
		for ( int i = 0; i < 5; i++ )
		{
			pValue.m_awardlist[pValue.m_awardcount].m_awardkind[i] = g_activity_33[tmpi].awardkind[i];
			pValue.m_awardlist[pValue.m_awardcount].m_awardnum[i] = g_activity_33[tmpi].awardnum[i];
		}
		pValue.m_awardcount += 1;
	}
	netsend_activity33_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}
