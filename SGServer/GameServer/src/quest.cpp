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
#include "city.h"
#include "building.h"
#include "actor.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "award.h"
#include "server_netsend_auto.h"
#include "quest.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

extern City *g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern QuestInfo *g_questinfo;
extern int g_questinfo_maxnum;

int *g_questlist[QUEST_TYPE_MAXNUM] = { NULL, NULL };

// 任务信息初始化
int questlist_init()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];

	// 任务列表
	int questlist_index[QUEST_TYPE_MAXNUM] = { 0 };
	for ( int tmpi = 0; tmpi < QUEST_TYPE_MAXNUM; tmpi++ )
	{
		g_questlist[tmpi] = (int *)malloc( sizeof( int )*g_questinfo_maxnum );
		memset( g_questlist[tmpi], 0, sizeof( int )*g_questinfo_maxnum );
	}
	// 按照顺序读取任务列表
	sprintf( szSQL, "select type, questid from quest order by sort asc;" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while ( (row = mysql_fetch_row( res )) )
	{
		int questtype = atoi( row[0] );
		if ( questtype == QUEST_TYPE_MAIN || questtype == QUEST_TYPE_BRANCH )
		{
			int index = questlist_index[questtype];
			if ( index >= 0 && index <= g_questinfo_maxnum )
			{
				g_questlist[questtype][index] = atoi( row[1] );
				questlist_index[questtype] += 1;
			}
		}
	}
	mysql_free_result( res );
	return 0;
}

//重读
int questlist_reload()
{
	for ( int tmpi = 0; tmpi < QUEST_TYPE_MAXNUM; tmpi++ )
	{
		if ( g_questlist[tmpi] )
		{
			free( g_questlist[tmpi] );
			g_questlist[tmpi] = NULL;
		}
	}	
	questlist_init();
	return 0;
}

// 如果玩家有这个任务，并且这个任务处于正在进行中。返回1
static int _quest_doing( City *pCity, int questid )
{
	for ( int tmpj = 0; tmpj < CITY_QUEST_MAX; tmpj++ )
	{
		if ( pCity->questid[tmpj] > 0 && pCity->questid[tmpj] == questid )
		{ // 有这个任务
			int flag = quest_getcomplete( pCity->actor_index, questid, NULL );
			if ( flag == QUEST_COMPLETEFLAG_NORMAL || flag == QUEST_COMPLETEFLAG_SUCCESS )
			{
				return 1;
			}
		}
	}
	return 0;
}

// 获取任务
int quest_give( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	// 找到一个未完成或者未领取奖励的主线任务
	for ( int tmpi = 0; tmpi < g_questinfo_maxnum; tmpi++ )
	{
		int questid = g_questlist[QUEST_TYPE_MAIN][tmpi];
		if ( questid <= 0 || questid >= QUEST_MAXNUM )
			break;
		QuestInfo *questinfo = &g_questinfo[questid];
		if ( questinfo == NULL )
			continue;
		int preid = questinfo->preid;
		if ( quest_getcomplete( actor_index, questid, NULL ) == QUEST_COMPLETEFLAG_AWARDED )
			continue;
		if ( preid > 0 && quest_getcomplete( actor_index, preid, NULL ) == QUEST_COMPLETEFLAG_NORMAL )
			continue;
		if ( pCity->questid[0] != questid )
		{
			wlog( 0, LOGOP_QUEST, 0, 0, pCity->questid[0], 0, pCity->actorid, pCity->building[0].level );
			pCity->questid[0] = questid;
			pCity->questvalue[0] = 0;

			for ( int i = 0; i < 2; i++ )
			{
				switch ( questinfo->trigger_type[i] )
				{
				case QUEST_TRIGGER_TYPE_NPCTALK: // 触发NPC说一段话	
					npc_talk( actor_index, questinfo->trigger_kind[i] );
					break;
				case QUEST_TRIGGER_TYPE_AWARD: //奖励
					award_getaward( actor_index, questinfo->trigger_kind[i], questinfo->trigger_value[i], 0, PATH_QUEST, NULL );
					break;
				}
			}
			
			if ( questid == 1 )
			{
				quest_setcomplete( actor_index, questid, QUEST_COMPLETEFLAG_SUCCESS );
			}

		}
		break;
	}

	// 找到5个未完成或者未领取奖励支线任务
	for ( int tmpi = 0; tmpi < g_questinfo_maxnum; tmpi++ )
	{
		int questid = g_questlist[QUEST_TYPE_BRANCH][tmpi];
		if ( questid <= 0 || questid >= QUEST_MAXNUM )
			break;
		QuestInfo *questinfo = &g_questinfo[questid];
		int preid = questinfo->preid;
		if ( quest_getcomplete( actor_index, questid, NULL ) == QUEST_COMPLETEFLAG_AWARDED )
			continue;
		if ( preid > 0 && quest_getcomplete( actor_index, preid, NULL ) == QUEST_COMPLETEFLAG_NORMAL )
			continue;
		if ( _quest_doing( pCity, questid ) == 1 )
			continue;

		// 正在进行中的不允许覆盖
		char has = 0;
		for ( int tmpj = 1; tmpj < CITY_QUEST_MAX; tmpj++ )
		{
			int flag = quest_getcomplete( actor_index, pCity->questid[tmpj], NULL );
			if ( pCity->questid[tmpj] > 0 && ( flag == QUEST_COMPLETEFLAG_NORMAL || flag == QUEST_COMPLETEFLAG_SUCCESS ) )
				continue;

			has = 1;
			wlog( 0, LOGOP_QUEST, 0, 1, pCity->questid[tmpj], 0, pCity->actorid, pCity->building[0].level );
			pCity->questid[tmpj] = questid;
			pCity->questvalue[tmpj] = 0;
			break;
		}
		if ( has == 0 )
			break;
	}

	quest_sendlist( actor_index );
	return 0;
}

// 设置任务值
int quest_addvalue( City *pCity, int datatype, int datakind, int dataoffset, int value )
{
	if ( !pCity )
		return -1;

	for ( int tmpi = 0; tmpi < CITY_QUEST_MAX; tmpi++ )
	{
		int questid = pCity->questid[tmpi];
		if ( questid <= 0 || questid >= QUEST_MAXNUM )
			continue;
		QuestInfo *questinfo = &g_questinfo[questid];
		if ( !questinfo )
			continue;
		if ( questinfo->datatype == datatype )
		{
			if ( questinfo->datakind > 0 && questinfo->datakind == datakind ||
				questinfo->datakind == 0 )
			{ // 正常情况
				pCity->questvalue[tmpi] += value;
				if ( pCity->questvalue[tmpi] >= questinfo->needvalue )
				{ 
					if ( pCity->actor_index >= 0 && pCity->actor_index < g_maxactornum )
					{
						quest_setcomplete( pCity->actor_index, questid, QUEST_COMPLETEFLAG_SUCCESS );
					}
					else
					{
						gift( pCity->actorid, AWARDKIND_QUEST_SUCCESS, questid, 0,0,0,0,0,0,0,0,0,0,0 );
					}
				}
			}
		}
	}

	return 0;
}

// 获取任务状态
int quest_getcomplete( int actor_index, int questid, int *value )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( questid <= 0 || questid >= QUEST_MAXNUM )
		return -1;
	// 每个unsigned char 存4个任务状态，每个任务占2位, 2位最大表示3，3的二进制是11
	int offset = (questid - 1) * 2 / 8;
	int bit = (questid - 1) * 2 % 8;
	int complete = g_actors[actor_index].quest_complete[offset] >> bit & 3;
	if ( complete == QUEST_COMPLETEFLAG_NORMAL )
	{ // 检查是否已经达到要求完成了
		complete = quest_check( actor_index, questid, value );
	}
	return complete;
}

// 设置任务状态
int quest_setcomplete( int actor_index, int questid, int flag )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( questid <= 0 || questid >= QUEST_MAXNUM )
		return -1;
	int offset = (questid - 1) * 2 / 8;
	int bit = (questid - 1) * 2 % 8;
	int complete = g_actors[actor_index].quest_complete[offset] >> bit & 3;
	if ( complete < flag )
	{
		g_actors[actor_index].quest_complete[offset] &= ~(3 << bit);
		g_actors[actor_index].quest_complete[offset] |= (flag << bit);
		//quest_sendnotify( actor_index );
		if ( flag == QUEST_COMPLETEFLAG_SUCCESS )
			quest_sendawardinfo( actor_index, questid );
	}
	return 0;
}

// 任务检查
int quest_check( int actor_index, int questid, int *value )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return 0;
	if ( questid <= 0 || questid >= g_questinfo_maxnum )
		return 0;
	QuestInfo *questinfo = &g_questinfo[questid];
	if ( !questinfo )
		return 0;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return 0;
	// 正数索引自定义，负数索引为data_record索引
	if ( questinfo->datatype > 0 )
	{
		if ( questinfo->datatype == QUEST_DATAINDEX_ACTOR_LEVEL )
		{ // 领主等级
			if ( value )
				*value = g_actors[actor_index].level;
			if ( g_actors[actor_index].level >= questinfo->needvalue )
			{
				return 1;
			}
		}
		else if( questinfo->datatype == QUEST_DATAINDEX_BUILDING_LEVEL )
		{ // 建筑等级
			int buildinglevel = building_getlevel( g_actors[actor_index].city_index, questinfo->datakind, questinfo->dataoffset );
			if ( value )
				*value = buildinglevel;
			if ( buildinglevel >= questinfo->needvalue )
			{
				return 1;
			}
		}
		else if ( questinfo->datatype == QUEST_DATAINDEX_BUILDING_SILVER )
		{ // 民居达N级N数量
			int buildingnum = 0;
			for ( int tmpi = 0; tmpi < BUILDING_RES_MAXNUM; tmpi++ )
			{
				if ( pCity->building_res[tmpi].kind == BUILDING_Silver && pCity->building_res[tmpi].level >= questinfo->datakind )
					buildingnum++;
			}
			if ( value )
				*value = buildingnum;
			if ( buildingnum >= questinfo->needvalue )
			{
				return 1;
			}
		}
		else if ( questinfo->datatype == QUEST_DATAINDEX_BUILDING_WOOD )
		{ // 伐木场达N级N数量
			int buildingnum = 0;
			for ( int tmpi = 0; tmpi < BUILDING_RES_MAXNUM; tmpi++ )
			{
				if ( pCity->building_res[tmpi].kind == BUILDING_Wood && pCity->building_res[tmpi].level >= questinfo->datakind )
					buildingnum++;
			}
			if ( value )
				*value = buildingnum;
			if ( buildingnum >= questinfo->needvalue )
			{
				return 1;
			}
		}
		else if ( questinfo->datatype == QUEST_DATAINDEX_BUILDING_FOOD )
		{ // 农田达N级N数量
			int buildingnum = 0;
			for ( int tmpi = 0; tmpi < BUILDING_RES_MAXNUM; tmpi++ )
			{
				if ( pCity->building_res[tmpi].kind == BUILDING_Food && pCity->building_res[tmpi].level >= questinfo->datakind )
					buildingnum++;
			}
			if ( value )
				*value = buildingnum;
			if ( buildingnum >= questinfo->needvalue )
			{
				return 1;
			}
		}
		else if ( questinfo->datatype == QUEST_DATAINDEX_BUILDING_IRON )
		{ // 矿场达N级N数量
			int buildingnum = 0;
			for ( int tmpi = 0; tmpi < BUILDING_RES_MAXNUM; tmpi++ )
			{
				if ( pCity->building_res[tmpi].kind == BUILDING_Iron && pCity->building_res[tmpi].level >= questinfo->datakind )
					buildingnum++;
			}
			if ( value )
				*value = buildingnum;
			if ( buildingnum >= questinfo->needvalue )
			{
				return 1;
			}
		}
		else if ( questinfo->datatype == QUEST_DATAINDEX_CITY_TECH )
		{ // 研究N科技N级
			if ( questinfo->datakind < 0 || questinfo->datakind >= CITY_TECH_MAX )
				return 0;
			if ( value )
				*value = pCity->techlevel[questinfo->datakind];
			if ( pCity->techlevel[questinfo->datakind] >= questinfo->needvalue )
			{
				return 1;
			}
		}
		else
		{
			for ( int tmpi = 0; tmpi < CITY_QUEST_MAX; tmpi++ )
			{
				if ( pCity->questid[tmpi] == questid )
				{
					if ( value )
						*value = pCity->questvalue[tmpi];
					if ( pCity->questvalue[tmpi] >= questinfo->needvalue )
					{
						return 1;
					}
					break;
				}
			}
		}
	}
	// 负数索引为data_record索引
	else if ( questinfo->datatype < 0 )
	{
		//int data_record_index = -questinfo->datatype;
		//int data_record_value = data_record_getvalue( city_getptr( actor_index ), data_record_index );
		//if ( value )
		//	*value = data_record_value;
		//if ( data_record_value >= questinfo->needvalue )
		//{
		//	return 1;
		//}
	}
	return 0;
}

// 任务检查
int quest_checkcomplete( City *pCity )
{
	if ( !pCity )
		return -1;
	if ( pCity->actor_index < 0 )
		return -1;
	int complete = 0;
	for ( int tmpi = 0; tmpi < CITY_QUEST_MAX; tmpi++ )
	{
		int questid = pCity->questid[tmpi];
		if ( questid <= 0 )
			continue;
		if ( quest_check( pCity->actor_index, questid, NULL ) == QUEST_COMPLETEFLAG_SUCCESS )
		{
			//complete = 1;
			quest_sendawardinfo( pCity->actor_index, questid );
			break;
		}
	}
	//if ( complete )
	//{
		//quest_sendnotify( pCity->actor_index );
	//}
	return 0;
}

// 重置所有任务
int quest_reset( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	memset( &g_actors[actor_index].quest_complete, 0, sizeof( char )*ACTOR_QUEST_MAX );
	memset( &pCity->questid, 0, sizeof(short)*CITY_QUEST_MAX );
	memset( &pCity->questvalue, 0, sizeof(int)*CITY_QUEST_MAX );
	quest_give( actor_index );
	//quest_sendnotify( actor_index );
	quest_sendlist( actor_index );
	return 0;
}

// 领取任务奖励
int quest_getaward( int actor_index, int questid, char type )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( questid <= 0 || questid >= g_questinfo_maxnum )
		return -1;
	if ( quest_getcomplete( actor_index, questid, NULL ) != QUEST_COMPLETEFLAG_SUCCESS )
		return -1;

	for ( int tmpi = 0; tmpi < 5; tmpi++ )
	{
		if ( g_questinfo[questid].awardkind[tmpi] == 0 )
			continue;
		award_getaward( actor_index, g_questinfo[questid].awardkind[tmpi], g_questinfo[questid].awardnum[tmpi], -1, PATH_QUEST, NULL );
	}
	quest_setcomplete( actor_index, questid, QUEST_COMPLETEFLAG_AWARDED );
	quest_give( actor_index );
	if ( type == 0 )
	{
		quest_sendlist( actor_index );
	}
	else if ( type == 1 )
	{
		//quest_sendnotify( actor_index );
	}
	return 0;
}

// 任务列表
int quest_sendlist( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	SLK_NetS_QuestList pValue = { 0 };
	for ( int tmpi = 0; tmpi < CITY_QUEST_MAX; tmpi++ )
	{
		int questid = pCity->questid[tmpi];
		if ( questid > 0 && questid < QUEST_MAXNUM )
		{
			int nowvalue = 0;
			int flag = quest_getcomplete( actor_index, questid, &nowvalue );
			pValue.m_list[pValue.m_count].m_questid = questid;
			pValue.m_list[pValue.m_count].m_flag = flag;
			pValue.m_list[pValue.m_count].m_datatype = g_questinfo[questid].datatype;
			pValue.m_list[pValue.m_count].m_datakind = g_questinfo[questid].datakind;
			pValue.m_list[pValue.m_count].m_dataoffset = (char)g_questinfo[questid].dataoffset;
			pValue.m_list[pValue.m_count].m_nameid = g_questinfo[questid].nameid;
			if ( flag == QUEST_COMPLETEFLAG_SUCCESS )
			{ 
				pValue.m_list[pValue.m_count].m_value = g_questinfo[questid].needvalue;
			}
			else
			{
				pValue.m_list[pValue.m_count].m_value = nowvalue;
			}
			pValue.m_list[pValue.m_count].m_needvalue = g_questinfo[questid].needvalue;
			
			for ( int i = 0; i < 5; i++ )
			{
				pValue.m_list[pValue.m_count].m_awardkind[i] = g_questinfo[questid].awardkind[i];
				pValue.m_list[pValue.m_count].m_awardnum[i] = g_questinfo[questid].awardnum[i];
			}
			pValue.m_count += 1;
		}
	}
	netsend_questlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 任务奖励
int quest_sendawardinfo( int actor_index, int questid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( questid <= 0 || questid >= g_questinfo_maxnum )
		return -1;
	SLK_NetS_QuestAward pValue = { 0 };
	pValue.m_questid = questid;
	pValue.m_datatype = g_questinfo[questid].datatype;
	pValue.m_datakind = g_questinfo[questid].datakind;
	pValue.m_dataoffset = (char)g_questinfo[questid].dataoffset;
	pValue.m_nameid = g_questinfo[questid].nameid;
	pValue.m_value = g_questinfo[questid].needvalue;
	pValue.m_needvalue = g_questinfo[questid].needvalue;
	pValue.m_count = 0;
	for ( int tmpi = 0; tmpi < 5; tmpi++ )
	{
		if ( g_questinfo[questid].awardkind[tmpi] == 0 )
			continue;
		pValue.m_list[pValue.m_count].m_kind = g_questinfo[questid].awardkind[tmpi];
		pValue.m_list[pValue.m_count].m_num = g_questinfo[questid].awardnum[tmpi];
		pValue.m_count++;
	}
	netsend_questaward_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

//// 发送任务可领取奖励的数量
//int quest_sendnotify( int actor_index )
//{
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//	City *pCity = city_getptr( actor_index );
//	if ( !pCity )
//		return -1;
//	SLK_NetS_QuestNotify info = { 0 };
//
//	// 找到一个未完成主线任务
//	int questid = 0;
//	for ( int tmpi = 0; tmpi < g_questinfo_maxnum; tmpi++ )
//	{
//		int id = g_questlist[QUEST_TYPE_MAIN][tmpi];
//		if ( id <= 0 || id >= QUEST_MAXNUM )
//			break;
//		QuestInfo *questinfo = &g_questinfo[id];
//		if ( questinfo == NULL )
//			continue;
//		int preid = questinfo->preid;
//		if ( preid > 0 && quest_getcomplete( actor_index, preid, NULL ) == QUEST_COMPLETEFLAG_NORMAL )
//			continue;
//		if ( quest_getcomplete( actor_index, id, NULL ) == QUEST_COMPLETEFLAG_NORMAL )
//		{
//			questid = id;
//			break;
//		}
//	}
//
//	// 主线任务
//	if ( questid > 0 && questid < QUEST_MAXNUM )
//	{
//		int nowvalue = 0;
//		int flag = QUEST_COMPLETEFLAG_NORMAL;
//		info.m_advice_quest.m_questid = questid;
//		info.m_advice_quest.m_flag = flag;
//		info.m_advice_quest.m_value = nowvalue;
//		info.m_advice_quest.m_needvalue = g_questinfo[questid].needvalue;
//		info.m_advice_quest.m_dataindex = g_questinfo[questid].dataindex;
//		info.m_advice_quest.m_datakind = g_questinfo[questid].datakind;
//		info.m_advice_quest.m_awardkind = g_questinfo[questid].awardkind[0];
//		info.m_advice_quest.m_awardnum = g_questinfo[questid].awardnum[0];
//	}
//
//	// 任务是否可领取
//	if ( quest_getcomplete( actor_index, pCity->questid[0], NULL ) == QUEST_COMPLETEFLAG_SUCCESS )
//	{
//		info.m_award_questid = pCity->questid[0];
//	}
//
//	// 可领取奖励数量
//	int count = 0;
//	for ( int tmpi = 0; tmpi < CITY_QUEST_MAX; tmpi++ )
//	{
//		int questid = pCity->questid[tmpi];
//		if ( questid <= 0 )
//			continue;
//		if ( quest_getcomplete( actor_index, questid, NULL ) == QUEST_COMPLETEFLAG_SUCCESS )
//		{
//			count++;
//		}
//	}
//	info.m_warningnum = count;
//	netsend_questnotify_S( actor_index, SENDTYPE_ACTOR, &info );
//	return 0;
//}

int data_record_addvalue( struct _city *pCity, int offset, int value )
{
	if ( pCity == NULL )
		return -1;
	if ( offset < 0 || offset >= DATA_RECORD_MAXNUM )
		return -1;
	if ( pCity->data_record[offset] > INT_MAX - value )
		pCity->data_record[offset] = INT_MAX;
	else
		pCity->data_record[offset] += value;

	return 0;
}

int data_record_setvalue( struct _city *pCity, int offset, int value )
{
	if ( pCity == NULL )
		return -1;
	if ( offset < 0 || offset >= DATA_RECORD_MAXNUM )
		return -1;
	if ( value >= INT_MAX )
	{
		value = INT_MAX;
	}
	pCity->data_record[offset] = value;
	return 0;
}

int data_record_getvalue( struct _city *pCity, int offset )
{
	if ( pCity == NULL )
		return 0;
	if ( offset < 0 || offset >= DATA_RECORD_MAXNUM )
		return 0;
	return pCity->data_record[offset];
}
