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
#include "global.h"
#include "building.h"
#include "actor.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "actor_friend.h"
#include "award.h"
#include "rank.h"
#include "map_zone.h"
#include "actor_times.h"
#include "auto_data_teacher_award.h"
#include "auto_data_teacher_shop.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;
extern Actor g_temp_actor[2];

extern City *g_city;
extern int g_city_maxcount;

extern TeacherAward *g_teacher_award;
extern int g_teacher_award_maxnum;

extern TeacherShop *g_teacher_shop;
extern int g_teacher_shop_maxnum;

ActorFriend *actor_friend_getptr( int actor_index, int offset )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return NULL;
	if ( offset < 0 || offset >= ACTOR_FRIEND_MAXCOUNT )
		return NULL;
	return &g_actors[actor_index].friends[offset];
}

// �Һ����б��Ƿ�����
inline int _friend_my_has_target( int my_index, int targetid )
{
	ACTOR_CHECK_INDEX( my_index );
	for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
	{
		if ( g_actors[my_index].friends[tmpi].friend_actorid == targetid )
		{
			return 1;
		}
	}
	return 0;
}

// ���ĺ����б��Ƿ�����
inline int _friend_target_has_my( int target_index, int myid )
{
	ACTOR_CHECK_INDEX( target_index );
	for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
	{
		if ( g_actors[target_index].friends[tmpi].friend_actorid == myid )
		{
			return 1;
		}
	}
	return 0;
}

// ��������¼�
int actor_friend_addevent( int event, int actorid, int target_actorid )
{
	char szSQL[256] = { 0 };
	sprintf( szSQL, "REPLACE INTO actor_friend_event ( actorid, event, target_actorid, optime ) VALUES ('%d','%d','%d','%d')", actorid, event, target_actorid, (int)time( NULL ) );
	dbwork_addsql( szSQL, DBWORK_CMD_NORMAL, 0 );
	return 0;
}

// ��ȡ���������¼�
int actor_friend_loadevent( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[256] = { 0 };
	char has = 0;
	sprintf( szSQL, "select `event`,`target_actorid` from actor_friend_event where actorid='%d'", g_actors[actor_index].actorid );
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
		int event = atoi( row[0] );
		int target_actorid = atoi( row[1] );
		actor_friend_enevt( actor_index, event, target_actorid );
		has = 1;
	}
	mysql_free_result( res );

	if ( has == 1 )
	{
		// ɾ����
		sprintf( szSQL, "delete from actor_friend_event where actorid='%d'", g_actors[actor_index].actorid );
		if ( mysql_query( myGame, szSQL ) )
		{
			printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
			write_gamelog( "%s", szSQL );
			if ( mysql_ping( myGame ) != 0 )
				db_reconnect_game();
		}
	}
	return 0;
}

// �����¼�����
int actor_friend_enevt( int actor_index, int event, int target_actorid )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( event == ACTOR_FRIEND_EVENT_ASK )
	{
		for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
		{
			if ( g_actors[actor_index].friends[tmpi].friend_actorid <= 0 )
			{
				g_actors[actor_index].friends[tmpi].actorid = g_actors[actor_index].actorid;
				g_actors[actor_index].friends[tmpi].friend_actorid = target_actorid;
				g_actors[actor_index].friends[tmpi].offset = tmpi;
				g_actors[actor_index].friends[tmpi].city_index = -1;
				g_actors[actor_index].friends[tmpi].jointime = 0;
				break;
			}
		}
	}
	else if ( event == ACTOR_FRIEND_EVENT_AGREE )
	{
		for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
		{
			if ( g_actors[actor_index].friends[tmpi].friend_actorid <= 0 )
			{
				g_actors[actor_index].friends[tmpi].actorid = g_actors[actor_index].actorid;
				g_actors[actor_index].friends[tmpi].friend_actorid = target_actorid;
				g_actors[actor_index].friends[tmpi].offset = tmpi;
				g_actors[actor_index].friends[tmpi].city_index = -1;
				g_actors[actor_index].friends[tmpi].jointime = (int)time( NULL );
				break;
			}
		}
	}
	else if ( event == ACTOR_FRIEND_EVENT_DEL )
	{
		for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
		{
			if ( g_actors[actor_index].friends[tmpi].friend_actorid == target_actorid )
			{
				g_actors[actor_index].friends[tmpi].actorid = 0;
				g_actors[actor_index].friends[tmpi].friend_actorid = 0;
				g_actors[actor_index].friends[tmpi].offset = -1;
				g_actors[actor_index].friends[tmpi].city_index = -1;
				g_actors[actor_index].friends[tmpi].jointime = 0;
				actor_friend_delete_db( g_actors[actor_index].actorid, target_actorid );
				break;
			}
		}
	}
	return 0;
}

// ����Ӻ���
int actor_friend_ask( int actor_index, int target_actorid, const char *target_name )
{
	ACTOR_CHECK_INDEX( actor_index );
	int target_city_index = -1;
	if ( target_actorid > 0 )
	{
		target_city_index = city_getindex_withactorid( target_actorid );
	}
	else
	{
		target_city_index = city_getindex_withactorname( target_name );
		if ( target_city_index >= 0 && target_city_index < g_city_maxcount )
		{
			target_actorid = g_city[target_city_index].actorid;
		}
	}
	if ( target_city_index < 0 || target_actorid <= 0 )
	{
		actor_notify_alert( actor_index, 2055 ); // ��Ҳ�����
		return -1;
	}

	if ( g_actors[actor_index].actorid == target_actorid )
	{
		return -1;
	}

	if ( city_getnation( g_actors[actor_index].city_index ) != g_city[target_city_index].nation )
	{
		actor_notify_alert( actor_index, 2056 ); // ���ܼ��������ҵ����Ϊ����
		return -1;
	}
	
	// �Һ����б��Ƿ�����
	if ( _friend_my_has_target( actor_index, target_actorid ) == 1 )
	{
		actor_notify_alert( actor_index, 2051 ); // �����Ѿ��Ǻ�����
		return -1;
	}

	// �Է����ߣ�ֱ�ӷ��������б�
	int target_actor_index = g_city[target_city_index].actor_index;
	if ( target_actor_index >= 0 && target_actor_index < g_maxactornum )
	{ 
		if ( _friend_target_has_my( target_actor_index, g_actors[actor_index].actorid ) == 1 )
		{
			actor_notify_alert( actor_index, 2052 ); // �Ѿ�������ѣ������ĵȴ��Է���Ӧ
			return -1;
		}

		int asksuccess = 0;
		for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
		{
			if ( g_actors[target_actor_index].friends[tmpi].friend_actorid <= 0 )
			{
				g_actors[target_actor_index].friends[tmpi].actorid = g_actors[target_actor_index].actorid;
				g_actors[target_actor_index].friends[tmpi].friend_actorid = g_actors[actor_index].actorid;
				g_actors[target_actor_index].friends[tmpi].offset = tmpi;
				g_actors[target_actor_index].friends[tmpi].city_index = g_actors[actor_index].city_index;
				g_actors[target_actor_index].friends[tmpi].jointime = 0;
				asksuccess = 1;
				break;
			}
		}
		if ( asksuccess == 0 )
		{
			actor_notify_alert( actor_index, 2053 ); // �Է������б�����
			return -1;
		}
	}
	else
	{ 
		// �����ߣ��������ݿ�
		actor_friend_addevent( ACTOR_FRIEND_EVENT_ASK, target_actorid, g_actors[actor_index].actorid );
	}

	actor_notify_alert( actor_index, 2054 ); // �Ѿ�������Ӻ�������
	return 0;
}

// ͬ�����
int actor_friend_agree( int actor_index, int target_actorid, int target_city_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( target_city_index < 0 || target_city_index >= g_city_maxcount )
	{
		target_city_index = city_getindex_withactorid( target_actorid );
	}
	else if ( g_city[target_city_index].actorid != target_actorid )
	{
		target_city_index = city_getindex_withactorid( target_actorid );
	}
	if ( target_city_index < 0 || target_city_index >= g_city_maxcount )
	{
		return -1;
	}

	for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
	{
		if ( g_actors[actor_index].friends[tmpi].friend_actorid == target_actorid )
		{
			g_actors[actor_index].friends[tmpi].jointime = (int)time(NULL);
			break;
		}
	}

	// �Է��Ƿ�����
	int target_actor_index = g_city[target_city_index].actor_index;
	if ( target_actor_index >= 0 && target_actor_index < g_maxactornum )
	{
		if ( _friend_target_has_my( target_actor_index, g_actors[actor_index].actorid ) == 1 )
		{
			return -1;
		}

		for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
		{
			if ( g_actors[target_actor_index].friends[tmpi].friend_actorid <= 0 )
			{
				g_actors[target_actor_index].friends[tmpi].actorid = g_actors[target_actor_index].actorid;
				g_actors[target_actor_index].friends[tmpi].friend_actorid = g_actors[actor_index].actorid;
				g_actors[target_actor_index].friends[tmpi].offset = tmpi;
				g_actors[target_actor_index].friends[tmpi].city_index = g_actors[actor_index].city_index;
				g_actors[target_actor_index].friends[tmpi].jointime = (int)time( NULL );
				break;
			}
		}
	}
	else
	{
		// �����ߣ��������ݿ�
		actor_friend_addevent( ACTOR_FRIEND_EVENT_AGREE, target_actorid, g_actors[actor_index].actorid );
	}
	return 0;
}

// �ܾ���Ӻ���
int actor_friend_reject( int actor_index, int target_actorid, int target_city_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( target_city_index < 0 || target_city_index >= g_city_maxcount )
	{
		target_city_index = city_getindex_withactorid( target_actorid );
	}
	else if ( g_city[target_city_index].actorid != target_actorid )
	{
		target_city_index = city_getindex_withactorid( target_actorid );
	}
	if ( target_city_index < 0 || target_city_index >= g_city_maxcount )
	{
		return -1;
	}

	for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
	{
		if ( g_actors[actor_index].friends[tmpi].friend_actorid == target_actorid )
		{
			g_actors[actor_index].friends[tmpi].actorid = 0;
			g_actors[actor_index].friends[tmpi].friend_actorid = 0;
			g_actors[actor_index].friends[tmpi].offset = -1;
			g_actors[actor_index].friends[tmpi].city_index = -1;
			g_actors[actor_index].friends[tmpi].jointime = 0;
			actor_friend_delete_db( g_actors[actor_index].actorid, target_actorid );
			break;
		}
	}

	// �Է��Ƿ�����
	int target_actor_index = g_city[target_city_index].actor_index;
	if ( target_actor_index >= 0 && target_actor_index < g_maxactornum )
	{
		
	}
	else
	{
		// �����ߣ��������ݿ�
		//actor_friend_addevent( ACTOR_FRIEND_EVENT_REJECT, target_actorid, g_actors[actor_index].actorid );
	}
	return 0;
}

// ɾ������
int actor_friend_delete( int actor_index, int target_actorid, int target_city_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( target_city_index < 0 || target_city_index >= g_city_maxcount )
	{
		target_city_index = city_getindex_withactorid( target_actorid );
	}
	else if ( g_city[target_city_index].actorid != target_actorid )
	{
		target_city_index = city_getindex_withactorid( target_actorid );
	}
	if ( target_city_index < 0 || target_city_index >= g_city_maxcount )
	{
		return -1;
	}

	for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
	{
		if ( g_actors[actor_index].friends[tmpi].friend_actorid == target_actorid )
		{
			g_actors[actor_index].friends[tmpi].actorid = 0;
			g_actors[actor_index].friends[tmpi].friend_actorid = 0;
			g_actors[actor_index].friends[tmpi].offset = -1;
			g_actors[actor_index].friends[tmpi].city_index = -1;
			g_actors[actor_index].friends[tmpi].jointime = 0;
			actor_friend_delete_db( g_actors[actor_index].actorid, target_actorid );
			break;
		}
	}

	// �Է��Ƿ�����
	int target_actor_index = g_city[target_city_index].actor_index;
	if ( target_actor_index >= 0 && target_actor_index < g_maxactornum )
	{
		for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
		{
			if ( g_actors[target_actor_index].friends[tmpi].friend_actorid <= g_actors[actor_index].actorid )
			{
				g_actors[target_actor_index].friends[tmpi].actorid = 0;
				g_actors[target_actor_index].friends[tmpi].friend_actorid = 0;
				g_actors[target_actor_index].friends[tmpi].offset = -1;
				g_actors[target_actor_index].friends[tmpi].city_index = -1;
				g_actors[target_actor_index].friends[tmpi].jointime = 0;
				actor_friend_delete_db( target_actorid, g_actors[actor_index].actorid );
				break;
			}
		}
	}
	else
	{
		// �����ߣ��������ݿ�
		actor_friend_addevent( ACTOR_FRIEND_EVENT_DEL, target_actorid, g_actors[actor_index].actorid );
	}
	return 0;
}

// ���ݿ�ɾ��
int actor_friend_delete_db( int actorid, int target_actorid )
{
	char szSQL[256] = { 0 };
	sprintf( szSQL, "delete from actor_friend where actorid='%d' and friend_actorid='%d'", actorid, target_actorid );
	//if ( mysql_query( myGame, szSQL ) )
	//{
	//	printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
	//	write_gamelog( "%s", szSQL );
	//	if ( mysql_ping( myGame ) != 0 )
	//		db_reconnect_game();
	//}
	dbwork_addsql( szSQL, DBWORK_CMD_NORMAL, 0 );
	return 0;
}

// �����б�
int actor_friend_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	SLK_NetS_FriendList pValue = { 0 };

	// ��ʼ����
	pValue.m_op = 1;
	pValue.m_count = 0;
	netsend_friendlist_S( actor_index, SENDTYPE_ACTOR, &pValue );

	// ���͹���
	pValue.m_op = 2;
	for ( int tmpi = 0; tmpi < ACTOR_FRIEND_MAXCOUNT; tmpi++ )
	{
		if ( g_actors[actor_index].friends[tmpi].actorid <= 0 )
			continue;
		int city_index = g_actors[actor_index].friends[tmpi].city_index;
		if ( city_index < 0 || city_index >= g_city_maxcount )
		{
			city_index = city_getindex_withactorid( g_actors[actor_index].friends[tmpi].friend_actorid );
			g_actors[actor_index].friends[tmpi].city_index = city_index;
		}
		if ( city_index < 0 || city_index >= g_city_maxcount )
			continue;
		pValue.m_list[pValue.m_count].m_actorid = g_city[city_index].actorid;
		pValue.m_list[pValue.m_count].m_city_index = city_index;
		pValue.m_list[pValue.m_count].m_shape = g_city[city_index].shape;
		pValue.m_list[pValue.m_count].m_place = g_city[city_index].place;
		pValue.m_list[pValue.m_count].m_level = g_city[city_index].level;
		pValue.m_list[pValue.m_count].m_battlepower = g_city[city_index].battlepower;
		strncpy( pValue.m_list[pValue.m_count].m_name, g_city[city_index].name, sizeof( char )*NAME_SIZE );
		pValue.m_list[pValue.m_count].m_namelen = strlen( pValue.m_list[pValue.m_count].m_name );
		if ( g_actors[actor_index].friends[tmpi].jointime == 0 )
			pValue.m_list[pValue.m_count].m_ask = 1;
		else
			pValue.m_list[pValue.m_count].m_ask = 0;
		pValue.m_count += 1;
		if ( pValue.m_count >= 40 )
		{
			netsend_friendlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
			pValue.m_count = 0;
		}
	}
	if ( pValue.m_count > 0 )
	{
		netsend_friendlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}

	// ���ͽ���
	pValue.m_op = 3;
	pValue.m_count = 0;
	netsend_friendlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// ʦͽ
// ͽ���б�
int actor_student_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_StudentList pValue = { 0 };
	// ʦ��
	if ( pCity->teacherid > 0 )
	{ 
		int city_index = city_getindex_withactorid( pCity->teacherid );
		if ( city_index >= 0 && city_index < g_city_maxcount )
		{
			pValue.m_list[pValue.m_count].m_actorid = g_city[city_index].actorid;
			pValue.m_list[pValue.m_count].m_city_index = city_index;
			pValue.m_list[pValue.m_count].m_shape = g_city[city_index].shape;
			pValue.m_list[pValue.m_count].m_place = g_city[city_index].place;
			pValue.m_list[pValue.m_count].m_level = g_city[city_index].level;
			pValue.m_list[pValue.m_count].m_battlepower = g_city[city_index].battlepower;
			strncpy( pValue.m_list[pValue.m_count].m_name, g_city[city_index].name, sizeof( char )*NAME_SIZE );
			pValue.m_list[pValue.m_count].m_namelen = strlen( pValue.m_list[pValue.m_count].m_name );
			pValue.m_list[pValue.m_count].m_isteacher = 1;
			pValue.m_count += 1;
		}
	}

	// ͽ��
	for ( int tmpi = 0; tmpi < ACTOR_STUDENT_MAXCOUNT; tmpi++ )
	{
		if ( g_actors[actor_index].student[tmpi] <= 0 )
			continue;
		int city_index = city_getindex_withactorid( g_actors[actor_index].student[tmpi] );
		if ( city_index < 0 || city_index >= g_city_maxcount )
			continue;
		pValue.m_list[pValue.m_count].m_actorid = g_city[city_index].actorid;
		pValue.m_list[pValue.m_count].m_city_index = city_index;
		pValue.m_list[pValue.m_count].m_shape = g_city[city_index].shape;
		pValue.m_list[pValue.m_count].m_place = g_city[city_index].place;
		pValue.m_list[pValue.m_count].m_level = g_city[city_index].level;
		pValue.m_list[pValue.m_count].m_battlepower = g_city[city_index].battlepower;
		strncpy( pValue.m_list[pValue.m_count].m_name, g_city[city_index].name, sizeof( char )*NAME_SIZE );
		pValue.m_list[pValue.m_count].m_namelen = strlen( pValue.m_list[pValue.m_count].m_name );
		pValue.m_count += 1;
	}

	// ����ȡ���Ѿ���ȡ����
	for ( int tmpi = 0; tmpi < 10; tmpi++ )
	{
		pValue.m_te_award[tmpi] = g_actors[actor_index].te_award[tmpi];
		pValue.m_te_awarded[tmpi] = g_actors[actor_index].te_awarded[tmpi];
	}

	// ��ʦ����
	if ( pCity->teacherid > 0 && actor_get_sflag( actor_index, ACTOR_SFLAG_TEACHERAWARD ) == 0 )
	{
		for ( int tmpi = 0; tmpi < 5; tmpi++ )
		{
			if ( g_teacher_award[0].awardkind[tmpi] <= 0 )
				continue;
			pValue.m_teacheraward[pValue.m_teacheraward_count].m_kind = g_teacher_award[0].awardkind[tmpi];
			pValue.m_teacheraward[pValue.m_teacheraward_count].m_num = g_teacher_award[0].awardnum[tmpi];
			pValue.m_teacheraward_count += 1;
		}
	}

	netsend_studentlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// ��ʦ
int actor_take_teacher( int actor_index, int teacher_actorid )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	City *pTeacherCity = city_getptr_withactorid( teacher_actorid );
	if ( !pTeacherCity )
		return -1;
	if ( pCity->level > global.take_teacher_maxlevel )
	{
		return -1;
	}
	if ( pTeacherCity->level <= global.take_teacher_maxlevel )
	{
		return -1;
	}
	int teacher_actor_index = actor_getindex_withid( teacher_actorid );
	if ( teacher_actor_index >= 0 && teacher_actor_index < g_maxactornum )
	{ // ����
		int free_index = -1;
		for ( int tmpi = 0; tmpi < ACTOR_STUDENT_MAXCOUNT; tmpi++ )
		{
			if ( g_actors[teacher_actor_index].student[tmpi] <= 0 )
			{
				free_index = tmpi;
			}
		}
		if ( free_index >= ACTOR_STUDENT_MAXCOUNT )
		{
			// 2428	�Է�ͽ����������
			actor_notify_alert( actor_index, 2428 );
			return -1;
		}
		g_actors[teacher_actor_index].student[free_index] = g_actors[actor_index].actorid;
		pCity->teacherid = teacher_actorid;
	}
	else
	{ // ������
		memset( &g_temp_actor[0], 0, sizeof( Actor ) );
		actor_load_auto( teacher_actorid, &g_temp_actor[0], "actor" );
		int free_index = -1;
		for ( int tmpi = 0; tmpi < ACTOR_STUDENT_MAXCOUNT; tmpi++ )
		{
			if ( g_temp_actor[0].student[tmpi] <= 0 )
			{
				free_index = tmpi;
			}
		}
		if ( free_index >= ACTOR_STUDENT_MAXCOUNT )
		{
			// 2428	�Է�ͽ����������
			actor_notify_alert( actor_index, 2428 );
			return -1;
		}
		g_temp_actor[0].student[free_index] = g_actors[actor_index].actorid;
		pCity->teacherid = teacher_actorid;
		actor_save_auto( &g_temp_actor[0], "actor", NULL );
	}
	actor_studentlevelup( teacher_actorid, pCity->level );
	actor_student_sendlist( actor_index );
	return 0;
}

// ��ȡ��ʦ����
int actor_take_teacher_awardget( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( pCity->teacherid <= 0 )
		return -1;
	if ( actor_get_sflag( actor_index, ACTOR_SFLAG_TEACHERAWARD ) == 1 )
		return -1;
	for ( int tmpi = 0; tmpi < 5; tmpi++ )
	{
		if ( g_teacher_award[0].awardkind[tmpi] <= 0 )
			continue;
		award_getaward( actor_index, g_teacher_award[0].awardkind[tmpi], g_teacher_award[0].awardnum[tmpi], -1, PATH_TEACHER, NULL );
	}
	actor_set_sflag( actor_index, ACTOR_SFLAG_TEACHERAWARD, 1 );
	actor_student_sendlist( actor_index );
	return 0;
}

// ͽ�ܵȼ�����
int actor_studentlevel_awardget( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( id <= 0 || id >= g_teacher_award_maxnum )
		return -1;
	 
	if ( g_actors[actor_index].te_award[id] <= 0 )
		return -1;
	if ( g_actors[actor_index].te_awarded[id] >= g_teacher_award[id].maxnum )
		return -1;

	for ( int tmpi = 0; tmpi < 5; tmpi++ )
	{
		if ( g_teacher_award[id].awardkind[tmpi] <= 0 )
			continue;
		award_getaward( actor_index, g_teacher_award[id].awardkind[tmpi], g_teacher_award[id].awardnum[tmpi], -1, PATH_TEACHER, NULL );
	}
	g_actors[actor_index].te_award[id] -= 1;
	g_actors[actor_index].te_awarded[id] += 1;
	actor_student_sendlist( actor_index );
	return 0;
}

// ͽ�ܵȼ���������
void actor_studentlevelup( int teacher_actorid, int level )
{
	if ( teacher_actorid <= 0 )
		return;
	int id = 0;
	for ( int tmpi = 1; tmpi < g_teacher_award_maxnum; tmpi++ )
	{
		if ( level == g_teacher_award[tmpi].student_level )
		{
			id = tmpi;
			break;
		}
	}
	if ( id == 0 )
		return;

	int teacher_actor_index = actor_getindex_withid( teacher_actorid );
	if ( teacher_actor_index >= 0 && teacher_actor_index < g_maxactornum )
	{ // ����
		award_getaward( teacher_actor_index, AWARDKIND_TEACHERAWARDNUM, id, -1, PATH_TEACHER, NULL );
	}
	else
	{ // ������
		gift( teacher_actorid, AWARDKIND_TEACHERAWARDNUM, id, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, PATH_TEACHER );
	}
	return;
}

int actor_studentlevel_awardadd( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( id <= 0 || id >= g_teacher_award_maxnum )
		return -1;
	if ( g_actors[actor_index].te_award[id] >= ACTOR_STUDENT_MAXCOUNT )
		return -1;
	g_actors[actor_index].te_award[id] += 1;
	return 0;
}

// ʦͽ�̵�
int teacher_shoplist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	SLK_NetS_TeacherShopList pValue = { 0 };
	pValue.m_mypoint = pCity->friendship;
	for ( int id = 1; id < g_teacher_shop_maxnum ; id++ )
	{
		if ( pCity->level < g_teacher_shop[id].showlevel )
			continue;
		if ( (g_actors[actor_index].te_shop & (1 << id)) == 1 )
			continue;
		pValue.m_list[pValue.m_count].m_id = id;
		pValue.m_list[pValue.m_count].m_awardkind = g_teacher_shop[id].awardkind;
		pValue.m_list[pValue.m_count].m_buylevel = g_teacher_shop[id].buylevel;
		pValue.m_list[pValue.m_count].m_point = g_teacher_shop[id].point;
		pValue.m_count += 1;
	}
	netsend_teachershoplist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

int teacher_shopbuy( int actor_index, int id )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( id <= 0 || id >= g_teacher_shop_maxnum || id > 30 )
		return -1;
	if ( pCity->level < g_teacher_shop[id].buylevel )
		return -1;
	if ( g_actors[actor_index].te_shop & (1 << id) )
		return -1;
	if ( pCity->friendship < g_teacher_shop[id].point )
		return -1;
	award_getaward( actor_index, g_teacher_shop[id].awardkind, g_teacher_shop[id].awardnum, PATH_TEACHERSHOP, -1, NULL );
	city_changefriendship( pCity->index, -g_teacher_shop[id].point, PATH_TEACHERSHOP );
	g_actors[actor_index].te_shop |= (1 << id);
	teacher_shoplist( actor_index );
	return 0;
}
