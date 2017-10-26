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
#include "global.h"
#include "city.h"
#include "building.h"
#include "actor.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "award.h"
#include "server_netsend_auto.h"
#include "quest.h"
#include "world_quest.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

extern City *g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern WorldQuestInfo *g_worldquestinfo;
extern int g_worldquestinfo_maxnum;

int *g_world_questlist = NULL;
int g_world_questid = 0;

int worldquest_init()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];

	// 任务列表
	int questlist_index = 0;
	g_world_questlist = (int *)malloc( sizeof( int )*g_worldquestinfo_maxnum );
	memset( g_world_questlist, 0, sizeof( int )*g_worldquestinfo_maxnum );

	// 按照顺序读取任务列表
	sprintf( szSQL, "select questid from world_questinfo order by sort asc;" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while ( (row = mysql_fetch_row( res )) )
	{
		int questid = atoi( row[0] );
		if ( questlist_index >= 0 && questlist_index <= g_worldquestinfo_maxnum )
		{
			g_world_questlist[questlist_index] = questid;
			questlist_index += 1;
		}
	}
	mysql_free_result( res );
	return 0;
}

// 重读
int worldquest_reload()
{
	if ( g_world_questlist )
	{
		free( g_world_questlist );
		g_world_questlist = NULL;
	}
	worldquest_init();
	return 0;
}

// 获取当前的世界任务
int worldquest_give( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	for ( int tmpi = 0; tmpi < g_worldquestinfo_maxnum; tmpi++ )
	{
		int questid = g_world_questlist[tmpi];
		if ( questid <= 0 || questid >= g_worldquestinfo_maxnum )
			continue;
	
		// 任务已经完成（已经领取奖励）
		if ( worldquest_getcomplete( actor_index, questid, NULL ) == 1 )
			continue;

		// 前置任务没完成
		int preid = g_worldquestinfo[questid].preid;
		if ( preid > 0 && worldquest_getcomplete( actor_index, preid, NULL ) == 0 )
			continue;

		if ( g_actors[actor_index].worldquestid != questid )
		{
			g_actors[actor_index].worldquestid = questid;
			break;
		}
	}
	return 0;
}

// 世界任务添加数值
int worldquest_addvalue( int questid, int value )
{
	if ( questid <= 0 || questid >= g_worldquestinfo_maxnum )
		return 0;
	if ( g_worldquestinfo[questid].type != 1 )
		return 0;
	int saveindex = g_worldquestinfo[questid].saveindex;
	if ( saveindex < 0 || saveindex >= 16 )
		return 0;
	int datavalue = world_data_getcache( WORLD_DATA_WORLDQUEST_BASE + saveindex );
	datavalue += value;
	world_data_set( WORLD_DATA_WORLDQUEST_BASE + saveindex, datavalue, NULL, NULL );
	return 0;
}

// 世界任务设置数值
int worldquest_setvalue( int questid, int value )
{
	if ( questid <= 0 || questid >= g_worldquestinfo_maxnum )
		return 0;
	if ( g_worldquestinfo[questid].type != 1 )
		return 0;
	int saveindex = g_worldquestinfo[questid].saveindex;
	if ( saveindex < 0 || saveindex >= 16 )
		return 0;
	int datavalue = world_data_getcache( WORLD_DATA_WORLDQUEST_BASE + saveindex );
	if ( datavalue > 0 )
	{
		return 0;
	}
	world_data_set( WORLD_DATA_WORLDQUEST_BASE + saveindex, value, NULL, NULL );

	if ( questid == WORLDQUEST_WORLDBOSS1 || questid == WORLDQUEST_WORLDBOSS2 )
	{
		// 通知全服在线玩家
		for ( int tmpi = 0; tmpi < g_maxactornum; tmpi++ )
		{
			if ( g_actors[tmpi].actorid > 0 )
			{
				worldquest_give( tmpi );
				worldquest_sendinfo( tmpi );
			}
		}
	}
	else
	{
		// 通知全服在线玩家
		for ( int tmpi = 0; tmpi < g_maxactornum; tmpi++ )
		{
			if ( g_actors[tmpi].actorid > 0 )
			{
				worldquest_checkcomplete( tmpi, 1 );
			}
		}
	}
	return 0;
}

// 设置任务完毕
int worldquest_setcomplete( int actor_index, int questid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return 0;
	g_actors[actor_index].worldquest_complete |= (1 << questid);
	return 0;
}

// 获取任务是否完毕
int worldquest_getcomplete( int actor_index, int questid, int *value )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return 0;
	int complete = g_actors[actor_index].worldquest_complete & (1 << questid);
	if ( complete > 0 )
	{
		return 1;
	}
	else if ( questid == WORLDQUEST_WORLDBOSS1 || questid == WORLDQUEST_WORLDBOSS2 )
	{
		if ( worldquest_check( actor_index, questid, value ) == 1 )
		{
			return 1;
		}
	}
	return 0;
}

// 任务检查
int worldquest_check( int actor_index, int questid, int *value )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return 0;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return 0;
	if ( questid <= 0 || questid >= g_worldquestinfo_maxnum )
		return 0;
	if ( g_worldquestinfo[questid].type == 0 )
	{// 个人
		int saveindex = g_worldquestinfo[questid].saveindex;
		if ( saveindex < 0 || saveindex >= 16 )
			return 0;
		int datav = data_record_getvalue( pCity, saveindex );
		if ( value )
			*value = datav;
		if ( datav >= g_worldquestinfo[questid].value )
		{
			return 1;
		}
	}
	else if ( g_worldquestinfo[questid].type == 1 )
	{ // 全服
		int saveindex = g_worldquestinfo[questid].saveindex;
		if ( saveindex < 0 || saveindex >= 16 )
			return 0;
		int datav = world_data_getcache( WORLD_DATA_WORLDQUEST_BASE + saveindex );
		if ( value )
			*value = datav;
		if ( datav > 0 )
		{
			return 1;
		}
	}
	return 0;
}

// 任务检查完成情况
int worldquest_checkcomplete( int actor_index, char type )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return 0;
	int questid = g_actors[actor_index].worldquestid;
	if ( questid <= 0 || questid >= g_worldquestinfo_maxnum )
		return 0;
	if ( g_worldquestinfo[questid].type != type )
		return 0;
	if ( g_actors[actor_index].view_areaindex < 0 )
		return 0;
	worldquest_sendinfo( actor_index );
	return 0;
}

// 发送任务信息
int worldquest_sendinfo( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_WorldQuest pValue = { 0 };
	int questid = g_actors[actor_index].worldquestid;
	if ( questid > 0 && questid < g_worldquestinfo_maxnum-1 )// -1,不发皇城血战
	{
		int datav = 0;
		int complete = worldquest_check( actor_index, questid, &datav );
		pValue.m_questid = questid;
		pValue.m_complete = complete;
		pValue.m_value = datav;
		pValue.m_maxvalue = g_worldquestinfo[questid].value;
		int saveindex = g_worldquestinfo[questid].saveindex;
		if ( saveindex >= 0 && saveindex < 16 )
		{
			int datavalue = world_data_getcache( WORLD_DATA_WORLDQUEST_BASE + saveindex );
			if ( datavalue == pCity->nation )
			{
				pValue.m_dd = 1; // 双倍
			}
		}
	}
	else
	{
		pValue.m_questid = 0;
	}
	netsend_worldquest_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 获取任务奖励
int worldquest_sendaward( int actor_index, int questid )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( questid <= 0 || questid >= g_worldquestinfo_maxnum )
		return 0;
	awardgroup_sendinfo( actor_index, g_worldquestinfo[questid].awardgroup, 3, questid, 4 );
	return 0;
}

// 领取奖励
int worldquest_getaward( int actor_index, int questid )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( questid <= 0 || questid >= g_worldquestinfo_maxnum )
		return 0;
	if ( worldquest_getcomplete( actor_index, questid, NULL ) == 1 )
		return -1;
	if ( worldquest_check( actor_index, questid, NULL ) <= 0 )
		return -1;

	char dd = 1;
	if ( questid == WORLDQUEST_ID4 || questid == WORLDQUEST_ID5 || questid == WORLDQUEST_ID6 || questid == WORLDQUEST_ID8 || questid == WORLDQUEST_ID9 )
	{
		int saveindex = g_worldquestinfo[questid].saveindex;
		if ( saveindex >= 0 && saveindex < 16 )
		{
			int datavalue = world_data_getcache( WORLD_DATA_WORLDQUEST_BASE + saveindex );
			if ( datavalue == pCity->nation )
			{
				dd = 2;
			}
		}
	}

	AwardGetInfo getinfo = { 0 };
	for ( int tmpi = 0; tmpi < dd; tmpi++ )
	{
		awardgroup_withindex( actor_index, g_worldquestinfo[questid].awardgroup, -1, PATH_QUEST, &getinfo );
	}
	
	worldquest_setcomplete( actor_index, questid );
	worldquest_give( actor_index );
	worldquest_sendinfo( actor_index );
	return 0;
}
