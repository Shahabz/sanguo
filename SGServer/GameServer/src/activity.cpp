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
#include "timegmcmd.h"
#include "gmcmd.h"
#include "system.h"
#include "global.h"
#include "pay.h"
#include "actor_times.h"
#include "city.h"
#include "quest.h"
#include "hero.h"

extern MYSQL *myGame;
extern Actor *g_actors;
extern int g_maxactornum;

extern Global global;
extern CommandEntry *g_command_entry;
extern int g_command_count;

ActivityItem *g_activity_item;
int g_activity_count = MAX_ACTIVITY_COUNT;

extern ActivityInfo02 *g_activity_02;
extern int g_activity_02_maxnum;

extern ActivityInfo03 *g_activity_03;
extern int g_activity_03_maxnum;

extern ActivityInfo05 *g_activity_05;
extern int g_activity_05_maxnum;

extern ActivityInfo08 *g_activity_08;
extern int g_activity_08_maxnum;

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

	sprintf( szSQL, "select activityid,starttime,endtime,closetime,value0,value1,value2,value3,strvalue,openstat,endstat from activity" );
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
		g_activity_item[activityid].m_starttime = atoi( row[1] );
		g_activity_item[activityid].m_endtime = atoi( row[2] );
		g_activity_item[activityid].m_closetime = atoi( row[3] );
		g_activity_item[activityid].m_value[0] = atoi( row[4] );
		g_activity_item[activityid].m_value[1] = atoi( row[5] );
		g_activity_item[activityid].m_value[2] = atoi( row[6] );
		g_activity_item[activityid].m_value[3] = atoi( row[7] );
		strcpy( g_activity_item[activityid].m_strvalue, row[8] );
		g_activity_item[activityid].m_openstat = atoi( row[9] );
		g_activity_item[activityid].m_endstat = atoi( row[10] );
	}
	mysql_free_result( res );
	return 0;
}

// 打开活动
int activity_settime( int activityid, int begin_min, int len_min, int value0, int value1, int value2, int value3, char *pstr )
{
	if ( activityid <= 0 || activityid >= g_activity_count )
		return -1;

	int tmpi = 0;
	int begintime;
	int endtime;
	int values[4] = {0};

	values[0] = value0;
	values[1] = value1;
	values[2] = value2;
	values[3] = value3;

	if( begin_min < 0 || len_min <= 0 )
	{
		begintime = 0;
		endtime = 0;
	}
	else
	{
		begintime = (int)time(NULL) + begin_min*60;
		endtime = (int)time(NULL) + (begin_min+len_min)*60;
	}
	g_activity_item[activityid].m_activityid = activityid;
	g_activity_item[activityid].m_starttime = begintime;
	g_activity_item[activityid].m_endtime = endtime;
	if ( endtime > 0 && activityid == ACTIVITY_NORMAL )
	{
		g_activity_item[activityid].m_closetime = endtime + 12 * 3600;
	}
	else
	{
		g_activity_item[activityid].m_closetime = 0;
	}
	g_activity_item[activityid].m_openstat = 0;
	g_activity_item[activityid].m_endstat = 0;
	for( tmpi = 0; tmpi < 4; tmpi++ )
	{
		if( values[tmpi] > 0 )
			g_activity_item[activityid].m_value[tmpi] = values[tmpi];
		else if( values[tmpi] == -1 )
			g_activity_item[activityid].m_value[tmpi] = 0;
	}

	if( pstr && strlen( pstr ) > 0 )
	{
		strcpy( g_activity_item[activityid].m_strvalue, pstr );
	}

	char szSQL[1024];
	sprintf( szSQL, "replace into activity ( activityid,starttime,endtime,closetime,value0,value1,value2,value3,strvalue,openstat,endstat ) values('%d','%d','%d','%d','%d','%d','%d','%d','%s','%d','%d')", activityid, begintime, endtime, g_activity_item[activityid].m_closetime, value0, value1, value2, value3, pstr, 0, 0 );
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
int activity_setendtime( int activityid, int endtime )
{
	if ( activityid >= g_activity_count )
		return -1;
	g_activity_item[activityid].m_endtime = endtime;
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

// 获取活动数据
int activity_getdata( int activityid, int *value, char *pstr )
{
	if( activityid >= g_activity_count )
		return -1;
	if( value == NULL || pstr == NULL )
		return -1;
	memcpy( value, g_activity_item[activityid].m_value, sizeof(int)*4 );
	strcpy( pstr, g_activity_item[activityid].m_strvalue );
	return 0;
}

// 获取活动值
int activity_getvalue( int activityid, int index )
{
	if( activityid >= g_activity_count )
		return 0;
	if( index < 0 || index >= 4 )
		return 0;
	return g_activity_item[activityid].m_value[index];
}

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

int activity_intime( int activityid )
{
	if( activityid >= g_activity_count )
		return 0;
	if( g_activity_item[activityid].m_starttime <= 0 )
		return 0;

	int timestamp = (int)time(NULL);
	if( timestamp < g_activity_item[activityid].m_starttime || timestamp >= g_activity_item[activityid].m_endtime )
		return 0;
	return 1;
}

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

// 获得活动启动时间秒
int activity_starttime( int activityid )
{
	if ( activityid >= g_activity_count )
		return 0;
	if ( g_activity_item[activityid].m_starttime <= 0 )
		return 0;
	return g_activity_item[activityid].m_starttime;
}

// 活动结束时间
int activity_endtime( int activityid )
{
	if ( activityid >= g_activity_count )
		return 0;
	if ( g_activity_item[activityid].m_endtime <= 0 )
		return 0;
	return g_activity_item[activityid].m_endtime;
}

// 活动关闭时间
int activity_closetime( int activityid )
{
	if ( activityid >= g_activity_count )
		return 0;
	if ( g_activity_item[activityid].m_closetime <= 0 )
		return 0;
	return g_activity_item[activityid].m_closetime;
}

// 获得活动剩余时间 秒
int activity_lefttime( int activityid )
{
	if( activityid >= g_activity_count )
		return 0;
	if( g_activity_item[activityid].m_starttime <= 0 )
		return 0;

	int lefttime = g_activity_item[activityid].m_endtime - (int)time( NULL );
	if( lefttime < 0 )
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


int activity_delete( int activityid )
{
	if ( activityid >= g_activity_count )
		return 0;
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

int activity_onwarning( int activityid, int surplus )
{
	if ( activityid >= g_activity_count )
		return 0;
	switch ( activityid )
	{
	case ACTIVITY_NORMAL:
		break;
	default:
		break;
	}
	return 0;
}

int activity_onopen( int activityid )
{
	if( activityid >= g_activity_count )
		return 0;

	switch( activityid )
	{
	case ACTIVITY_NORMAL:
		break;
	case ACTIVITY_PAYBAG:
		break;
	default:
		sc_ActivityOnOpen( activityid );
		break;
	}
	return 0;
}

int activity_onend( int activityid )
{
	if ( activityid >= g_activity_count )
		return 0;
	switch ( activityid )
	{
	case ACTIVITY_PAYBAG:
		break;
	default:
		sc_ActivityOnEnd( activityid );
		break;
	}
	return 0;
}

int activity_onclose( int activityid )
{
	if( activityid >= g_activity_count )
		return 0;
	switch( activityid )
	{
	case 0:
		break;
	default:
		sc_ActivityOnClose( activityid );
		break;
	}
	activity_settime( activityid, -1, -1, 0, 0, 0, 0, "" );
	activity_setopenstat( activityid, 0 );
	activity_setendstat( activityid, 0 );
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
	default:
		sc_ActivityOnLogic( activityid );
		break;
	}
	return 0;
}

int activity_logic()
{
	int activityid;
	int timestamp = (int)time(NULL);
	for( activityid = 1; activityid < g_activity_count; activityid++ )
	{
		if ( g_activity_item[activityid].m_starttime <= 0 )
		{
			g_activity_item[activityid].m_nexttime = time_gmcmd_getnexttime( GMC_ACTIVITY, activityid );
			activity_onwarning( activityid, g_activity_item[activityid].m_nexttime - timestamp );
			continue;
		}

		// 活动尚未开始
		if ( timestamp < g_activity_item[activityid].m_starttime )
		{
			activity_onwarning( activityid, g_activity_item[activityid].m_starttime - timestamp );
			continue;
		}

		// 开启，只会调用一次
		if ( g_activity_item[activityid].m_openstat == 0 )
		{
			activity_onopen( activityid );
			activity_setopenstat( activityid, 1 );
		}

		// 结束
		if ( timestamp >= g_activity_item[activityid].m_endtime && g_activity_item[activityid].m_endstat == 0 )
		{
			activity_onend( activityid );
			activity_setendstat( activityid, 1 );
		}

		// 关闭
		if( timestamp >= g_activity_item[activityid].m_closetime && g_activity_item[activityid].m_endstat == 1 )
		{
			activity_onclose( activityid );
			continue;
		}
		activity_onlogic( activityid );
	}
	return 0;
}

// 设置活动参数
int activity_setdata( int activityid, int value0, int value1, int value2, int value3, char* pstr )
{
	char szSQL[1024];

	if( activityid < 0 || activityid >= g_activity_count )
		return -1;

	g_activity_item[activityid].m_value[0] = value0;
	g_activity_item[activityid].m_value[1] = value1;
	g_activity_item[activityid].m_value[2] = value2;
	g_activity_item[activityid].m_value[3] = value3;
	if( pstr )
		strcpy( g_activity_item[activityid].m_strvalue, pstr );
	else
		g_activity_item[activityid].m_strvalue[0] = 0;

	sprintf( szSQL, "replace into activity ( activityid,starttime,endtime,closetime,value0,value1,value2,value3,strvalue,openstat,endstat ) values('%d','%d','%d','%d','%d','%d','%d','%d','%s','%d','%d')", 
		activityid, g_activity_item[activityid].m_starttime, g_activity_item[activityid].m_endtime, g_activity_item[activityid].m_closetime, value0, value1, value2, value3, pstr, g_activity_item[activityid].m_openstat, g_activity_item[activityid].m_endstat );
	if( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}

	return 0;
}

// 活动倒计时
int activity_countdown( int activityid )
{
	if ( activityid < 1 || activityid >= g_activity_count )
		return -1;
	int timestamp = (int)time( NULL );
	int statetime = 0;
	if ( g_activity_item[activityid].m_starttime <= 0 || activity_intime( activityid ) == 0 )
	{
		statetime = time_gmcmd_getnexttime( GMC_ACTIVITY, activityid ) - timestamp;
	}
	else if ( timestamp < g_activity_item[activityid].m_starttime )
	{
		statetime = g_activity_item[activityid].m_starttime - timestamp;
	}
	return statetime;
}

// 活动列表
int activity_sendlist( int actor_index )
{
	int endtime = 0;
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_ActivityList pValue = { 0 };
	for ( int activityid = 1; activityid < MAX_ACTIVITY_COUNT; activityid++ )
	{
		int starttime = g_activity_item[activityid].m_starttime;
		if ( starttime <= 0 )
		{
			starttime = g_activity_item[activityid].m_nexttime;
		}
		if ( starttime <= 0 )
		{
			continue;
		}
		pValue.m_list[pValue.m_count].m_activityid = activityid;
		pValue.m_list[pValue.m_count].m_starttime = starttime;
		pValue.m_list[pValue.m_count].m_endtime = g_activity_item[activityid].m_endtime;
		pValue.m_list[pValue.m_count].m_closetime = g_activity_item[activityid].m_closetime;
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
		pValue.m_count += 1;
	}

	// 七星拜将
	endtime = g_actors[actor_index].createtime + 3 * 86400;
	if ( (int)time( NULL ) < endtime || (actor_get_sflag( actor_index, ACTOR_SFLAG_ACTIVITY05_CALL ) == 0 && g_actors[actor_index].act05_xw >=7 ) )
	{
		pValue.m_list[pValue.m_count].m_activityid = ACTIVITY_5;
		pValue.m_list[pValue.m_count].m_starttime = g_actors[actor_index].createtime;
		pValue.m_list[pValue.m_count].m_endtime = endtime;
		pValue.m_list[pValue.m_count].m_closetime = endtime;
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
		pValue.m_count += 1;
	}
	
	// 出师大宴
	pValue.m_list[pValue.m_count].m_activityid = ACTIVITY_11;
	pValue.m_list[pValue.m_count].m_starttime = 0;
	pValue.m_list[pValue.m_count].m_endtime = 0;
	pValue.m_list[pValue.m_count].m_closetime = 0;
	pValue.m_count += 1;

	netsend_activitylist_S( actor_index, SENDTYPE_ACTOR, &pValue );
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
			pValue.m_list[pValue.m_count].m_token = g_activity_05[id].token;
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

// 出师大宴活动
int activity_body_sendinfo( int actor_index )
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

int activity_body_get( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	time_t t;
	time( &t );
	struct tm *nowtime = localtime( &t );

	if ( nowtime->tm_hour >= 12 && nowtime->tm_hour <= 14 )
	{ // 午宴
		awardgroup_withindex( actor_index, 163, -1, PATH_ACTIVITY, NULL );
		actor_add_today_char_times( actor_index, TODAY_CHAR_ACTIVITY_BODYGET1 );
	}
	else if ( nowtime->tm_hour >= 18 && nowtime->tm_hour <= 20 )
	{ // 晚宴
		awardgroup_withindex( actor_index, 163, -1, PATH_ACTIVITY, NULL );
		actor_add_today_char_times( actor_index, TODAY_CHAR_ACTIVITY_BODYGET2 );
	}
	else
		return -1;
	activity_body_sendinfo( actor_index );
	return 0;
}
