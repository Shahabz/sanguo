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

extern MYSQL *myGame;
extern Actor *g_actors;
extern int g_maxactornum;

extern Global global;
extern CommandEntry *g_command_entry;
extern int g_command_count;

ActivityItem *g_activity_item;
int g_activity_count = MAX_ACTIVITY_COUNT;

extern ActivityInfo03 *g_activity_03;
extern int g_activity_03_maxnum;

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

	// 攻城略地
	char activity03_over = 1;
	for ( int tmpi = 1; tmpi < g_activity_03_maxnum; tmpi++ )
	{
		if ( data_record_getvalue( pCity, g_activity_03[tmpi].record_offset ) < g_activity_03[tmpi].needvalue )
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
	
	
	// 出师大宴
	pValue.m_list[pValue.m_count].m_activityid = ACTIVITY_11;
	pValue.m_list[pValue.m_count].m_starttime = 0;
	pValue.m_list[pValue.m_count].m_endtime = 0;
	pValue.m_list[pValue.m_count].m_closetime = 0;
	pValue.m_count += 1;

	netsend_activitylist_S( actor_index, SENDTYPE_ACTOR, &pValue );
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
		pValue.m_list[pValue.m_count].m_state = (g_actors[actor_index].act03_state & (1 << id));
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
