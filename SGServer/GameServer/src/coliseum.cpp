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
#include "actor.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "actor_times.h"
#include "award.h"
#include "rank.h"
#include "nation.h"
#include "hero.h"
#include "system.h"
#include "fight.h"
#include "mail.h"
#include "chat.h"
#include "server_netsend_auto.h"
#include "coliseum.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern City *g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern Fight g_fight;
extern char g_szFightSql[65535];
extern char g_szFightJson[FIGHT_JSON_MAX];


extern LastName *g_lastname;
extern int g_lastname_maxnum;

extern FirstName *g_firstname;
extern int g_firstname_maxnum;

int g_maxrank = 1;
SLK_NetS_ColiseumRankList g_ColiseumRankList = { 0 };

int coliseum_init()
{
	// A.��ȡ��������
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];
	sprintf( szSQL, "select max(rank) from match_list" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	if ( !((row = mysql_fetch_row( res ))) )
	{
		mysql_free_result( res );
		return -1;
	}
	if ( row[0] )
		g_maxrank = atoi( row[0] ) + 1;
	else
		g_maxrank = 1;
	mysql_free_result( res );

	// ��ʼ�����а�
	coliseum_load_ranklist();
	return 0;
}

// ���뾺����
int coliseum_enter( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	_check_fday( actor_index );
	if ( actor_check_uselimit_cd( actor_index, USELIMIT_CD_COLISEUM_ENTER ) > 0 )
	{//	���뾺����̫��Ƶ����
		actor_notify_pop( actor_index, 4301 );
		return -1;
	}

	// ��ȡ�Լ���������
	g_actors[actor_index].coliseum_rank = coliseum_getrank( g_actors[actor_index].actorid );
	// �����Լ��Ĵ浵
	coliseum_save( actor_index );
	// �������
	coliseum_checkrank();
	// ƥ�����
	if ( g_actors[actor_index].coliseum_list[0].m_actorid <= 0 )
	{
		coliseum_match( actor_index );
	}
	coliseum_sendlist( actor_index );

	// Ƶ������
	actor_set_uselimit_cd( actor_index, USELIMIT_CD_COLISEUM_ENTER, 3 );
	return 0;
}

// �����б�
int coliseum_sendlist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	SLK_NetS_ColiseumList pValue = { 0 };
	pValue.m_updatecd = actor_check_uselimit_cd( actor_index, USELIMIT_CD_COLISEUM_UPDATE );
	pValue.m_replacecd = actor_check_uselimit_cd( actor_index, USELIMIT_CD_COLISEUM_TEAM );
	pValue.m_todaynum = global.coliseum_todaynum-actor_get_today_char_times( actor_index, TODAY_CHAR_COLISEUM_TODAYNUM );
	pValue.m_maxtodaynum = global.coliseum_todaynum;
	pValue.m_maxrank = g_actors[actor_index].coliseum_maxrank;
	pValue.m_myrank = g_actors[actor_index].coliseum_rank;
	memcpy( pValue.m_list, g_actors[actor_index].coliseum_list, sizeof( SLK_NetS_ColiseumCity ) * 3 );
	pValue.m_count = COLISEUMLIST_MAX;
	pValue.m_myteam[0] = g_actors[actor_index].coliseum_team[0];
	pValue.m_myteam[1] = g_actors[actor_index].coliseum_team[1];
	pValue.m_myteam[2] = g_actors[actor_index].coliseum_team[2];
	netsend_coliseumlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// �������
int coliseum_getrank( int actorid )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];
	int rank = -1;

	sprintf( szSQL, "select rank from match_list where actorid='%d'", actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	if ( !((row = mysql_fetch_row( res ))) )
	{
		mysql_free_result( res );
		rank = g_maxrank++;
		return rank;
	}
	if ( row[0] )
		rank = atoi( row[0] );
	mysql_free_result( res );

	if ( rank < 0 )
	{
		rank = g_maxrank++;
	}
	return rank;
}

// ��������
int coliseum_setrank( int actorid, int rank, SLK_NetS_ColiseumCity *pInfo )
{
	char szSQL[2048];
	sprintf( szSQL, "update match_list set rank='%d' where actorid = '%d'", rank, actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}

	if ( pInfo && rank > 0 && rank <= 30 )
	{
		strncpy( g_ColiseumRankList.m_list[rank-1].m_name, pInfo->m_name, NAME_SIZE );
		g_ColiseumRankList.m_list[rank-1].m_namelen = pInfo->m_namelen;
		g_ColiseumRankList.m_list[rank-1].m_nation = pInfo->m_nation;
		g_ColiseumRankList.m_list[rank-1].m_bpower = pInfo->m_bpower;
		g_ColiseumRankList.m_list[rank-1].m_level = pInfo->m_level;
	}
	return 0;
}

// ��������Ƿ���ȷ��һ��ʱ�����һ�ΰ�
void coliseum_checkrank()
{
	char szSQL[2048];
	sprintf( szSQL, "set @rownum=0" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return;
	}
	sprintf( szSQL, "update match_list set rank=(@rownum:=@rownum+1) order by rank" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return;
	}
}

// �����Լ��ľ�������Ϣ
int coliseum_save( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	// �Լ����飬Ĭ�������佫
	char empty = 1;
	for ( int tmpi = 0; tmpi < COLISEUMLIST_HERO_MAX; tmpi++ )
	{
		if ( g_actors[actor_index].coliseum_team[tmpi] > 0 )
		{
			empty = 0;
			break;
		}
	}
	if ( empty == 1 )
	{
		for ( int tmpi = 0; tmpi < COLISEUMLIST_HERO_MAX; tmpi++ )
		{
			g_actors[actor_index].coliseum_team[tmpi] = pCity->hero[tmpi].kind;
		}
	}

	// ������Ϣ�浽�б�
	coliseum_list_save( actor_index, "match_list" );
	return 0;
}

// �����б�
int coliseum_list_save( int actor_index, char *pTab )
{
	char szSQL[2048];
	char tmpstr[256];
	int member = 0;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	memset( &g_actors[actor_index].coliseum_my, 0, sizeof( SLK_NetS_ColiseumCity ) );

	int totalpower = 0;
	// ��ȡ��ǰ������������
	Hero tempHero[COLISEUMLIST_HERO_MAX] = { 0 };
	for ( int tmpi = 0; tmpi < COLISEUMLIST_HERO_MAX; tmpi++ )
	{
		int herokind = g_actors[actor_index].coliseum_team[tmpi];
		Hero *pHero = hero_getptr( actor_index, herokind );
		if ( !pHero )
			continue;
		memcpy( &tempHero[tmpi], pHero, sizeof( Hero ) );
		tempHero->offset = HERO_COLISEUMOFFSET + tmpi;
		hero_attr_calc( pCity, &tempHero[tmpi] );
		tempHero[tmpi].colorup = hero_getline( pCity, HERO_STATE_FIGHT ); // ���⴦��
		totalpower += tempHero[tmpi].bp_hero + tempHero[tmpi].bp_equip + tempHero[tmpi].bp_tech + tempHero[tmpi].bp_nequip + tempHero[tmpi].bp_girl;
	}

	// ����
	char szText_name[64] = { 0 };
	sprintf( szSQL, "REPLACE INTO %s VALUES( '%d','%d','%s','%d','%d','%d','%d'", pTab, 
		g_actors[actor_index].actorid,
		g_actors[actor_index].coliseum_rank,
		db_escape( (const char *)g_actors[actor_index].name, szText_name, 0 ),
		strlen(g_actors[actor_index].name),
		g_actors[actor_index].nation,
		g_actors[actor_index].level,
		totalpower );

	// ��ʱ��һ��
	g_actors[actor_index].coliseum_my.m_actorid = g_actors[actor_index].actorid;
	g_actors[actor_index].coliseum_my.m_rank = g_actors[actor_index].coliseum_rank;
	g_actors[actor_index].coliseum_my.m_nation = g_actors[actor_index].nation;
	g_actors[actor_index].coliseum_my.m_level = g_actors[actor_index].level;
	g_actors[actor_index].coliseum_my.m_bpower = totalpower;
	strncpy( g_actors[actor_index].coliseum_my.m_name, g_actors[actor_index].name, NAME_SIZE );
	g_actors[actor_index].coliseum_my.m_namelen = strlen( g_actors[actor_index].coliseum_my.m_name );

	// Ӣ�ۻ�����Ϣ
	for ( member = 0; member < COLISEUMLIST_HERO_MAX; member++ )
	{
		if ( tempHero[member].kind <= 0 )
			break;
		sprintf( tmpstr, ",'%d','%d','%d'", tempHero[member].kind,tempHero[member].color,tempHero[member].level );
		strcat( szSQL, tmpstr );

		// ��ʱ��һ��
		g_actors[actor_index].coliseum_my.m_hero[member].m_kind = tempHero[member].kind;
		g_actors[actor_index].coliseum_my.m_hero[member].m_color = tempHero[member].color;
		g_actors[actor_index].coliseum_my.m_hero[member].m_level = tempHero[member].level;
	}
	for ( ; member < COLISEUMLIST_HERO_MAX; member++ )
	{
		sprintf( tmpstr, ",'0','0','0'" );
		strcat( szSQL, tmpstr );
	}
	strcat( szSQL, ");" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}

	// ������Ϣ
	coliseum_match_attr_delete( g_actors[actor_index].actorid, "match_attr" );
	for ( int tmpi = 0; tmpi < COLISEUMLIST_HERO_MAX; tmpi++ )
	{
		if ( tempHero[tmpi].kind > 0 )
		{
			coliseum_match_attr_save( g_actors[actor_index].actorid, tmpi, &tempHero[tmpi], "match_attr" );
		}
	}
	return 0;
}

// �����б�
int coliseum_list_load( int actor_index, char *pTab )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[1024];
	int index;
	memset( g_actors[actor_index].coliseum_list, 0, sizeof( SLK_NetS_ColiseumCity ) * COLISEUMLIST_MAX );
	// ���ƥ�����
	if ( coliseum_random( actor_index ) < 0 )
		return -1;
	for ( int tmpi = 0; tmpi < COLISEUMLIST_MAX; tmpi++ )
	{
		sprintf( szSQL, "SELECT actorid,rank,name,namelen,nation,level,bpower,herokind0,herocolor0,herolevel0,herokind1,herocolor1,herolevel1,herokind2,herocolor2,herolevel2 from %s where rank='%d'", pTab, g_actors[actor_index].coliseum_list[tmpi].m_rank );
		if ( mysql_query( myGame, szSQL ) )
		{
			printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
			write_gamelog( "%s", szSQL );
			if ( mysql_ping( myGame ) != 0 )
				db_reconnect_game();
			return -1;
		}
		res = mysql_store_result( myGame );

		if ( !((row = mysql_fetch_row( res ))) )
		{
			mysql_free_result( res );
			continue;
		}
		index = 0;
		// ���ǻ�����Ϣ
		g_actors[actor_index].coliseum_list[tmpi].m_actorid = atoi( row[index++] );
		g_actors[actor_index].coliseum_list[tmpi].m_rank = atoi( row[index++] );
		strcpy( g_actors[actor_index].coliseum_list[tmpi].m_name, row[index++] );
		g_actors[actor_index].coliseum_list[tmpi].m_namelen = atoi( row[index++] );
		g_actors[actor_index].coliseum_list[tmpi].m_nation = atoi( row[index++] );
		g_actors[actor_index].coliseum_list[tmpi].m_level = atoi( row[index++] );
		g_actors[actor_index].coliseum_list[tmpi].m_bpower = atoi( row[index++] );
		// Ӣ����Ϣ
		for ( int i = 0; i < COLISEUMLIST_HERO_MAX; i++ )
		{
			g_actors[actor_index].coliseum_list[tmpi].m_hero[i].m_kind = atoi( row[index++] );
			g_actors[actor_index].coliseum_list[tmpi].m_hero[i].m_color = atoi( row[index++] );
			g_actors[actor_index].coliseum_list[tmpi].m_hero[i].m_level = atoi( row[index++] );
		}
		mysql_free_result( res );
	}
	return 0;
}

// ����������ݿ�ɾ��
int coliseum_match_attr_delete( int actorid, char *pTab )
{
	char	szSQL[2048] = { 0 };
	char reconnect_flag = 0;
	char sz64_partnerid[21] = { 0 };
RE_DELETE_DELETE:
	sprintf( szSQL, "DELETE FROM %s WHERE `actorid`='%d'", pTab, actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( reconnect_flag )
			return -1;
		if ( mysql_ping( myGame ) != 0 )
		{
			db_reconnect_game();
			reconnect_flag = 1;
			goto RE_DELETE_DELETE;
		}
		return -1;
	}
	return 0;
}

// Ӣ����Ϣ�浵
int coliseum_match_attr_save( int actorid, int offset, Hero *pHero, char *pTab )
{
	char szSQL[512] = { 0 };
	char reconnect_flag = 0;
	if ( pHero == NULL || pTab == NULL )
		return -1;
RE_ACTORATTR_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (actorid,offset,kind,color,level,attack,defense,troops,attack_increase,defense_increase,line) "
		"Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')", pTab, actorid, offset,
		pHero->kind, pHero->color, pHero->level, pHero->attack, pHero->defense, pHero->troops, pHero->attack_increase, pHero->defense_increase, pHero->colorup );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( reconnect_flag )
			return -1;
		if ( mysql_ping( myGame ) != 0 )
		{
			db_reconnect_game();
			reconnect_flag = 1;
			goto RE_ACTORATTR_UPDATE;
		}
		return -1;
	}
	return 0;
}

// ��ȡӢ������
int coliseum_match_attr_load( int actorid, Hero *pHero, int count, char *pTab )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[1024];
	int index;

	sprintf( szSQL, "SELECT offset,kind,color,level,attack,defense,troops,attack_increase,defense_increase,line from %s where actorid='%d'", pTab, actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( row = mysql_fetch_row( res ))
	{
		int offset = atoi( row[0] );
		if ( offset < 0 || offset >= count )
			break;
		index = 1;
		pHero[offset].offset = HERO_COLISEUMOFFSET + offset;
		pHero[offset].kind = atoi( row[index++] );
		pHero[offset].color = atoi( row[index++] );
		pHero[offset].level = atoi( row[index++] );
		pHero[offset].attack = atoi( row[index++] );
		pHero[offset].defense = atoi( row[index++] );
		pHero[offset].troops = atoi( row[index++] );
		pHero[offset].attack_increase = atoi( row[index++] );
		pHero[offset].defense_increase = atoi( row[index++] );
		pHero[offset].colorup = atoi( row[index++] );
	}
	mysql_free_result( res );
	return 0;
}

// �������
int coliseum_random( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	int rank = g_actors[actor_index].coliseum_rank;

	/* ǰ10�����������Ĺ̶����� */
	if ( rank == 1 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = 2;
		g_actors[actor_index].coliseum_list[1].m_rank = 3;
		g_actors[actor_index].coliseum_list[2].m_rank = 4;
	}
	else if ( rank == 2 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = 1;
		g_actors[actor_index].coliseum_list[1].m_rank = 3;
		g_actors[actor_index].coliseum_list[2].m_rank = 4;
	}
	else if ( rank == 3 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = 1;
		g_actors[actor_index].coliseum_list[1].m_rank = 2;
		g_actors[actor_index].coliseum_list[2].m_rank = 4;
	}
	else if ( rank == 4 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = 1;
		g_actors[actor_index].coliseum_list[1].m_rank = 2;
		g_actors[actor_index].coliseum_list[2].m_rank = 3;
	}
	else if ( rank == 5 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = random( 1, 2 );
		g_actors[actor_index].coliseum_list[1].m_rank = 3;
		g_actors[actor_index].coliseum_list[2].m_rank = 4;
	}
	else if ( rank == 6 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = random( 1, 2 );
		g_actors[actor_index].coliseum_list[1].m_rank = random( 3, 4 );
		g_actors[actor_index].coliseum_list[2].m_rank = 5;
	}
	else if ( rank == 7 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = random( 1, 2 );
		g_actors[actor_index].coliseum_list[1].m_rank = random( 3, 4 );
		g_actors[actor_index].coliseum_list[2].m_rank = random( 5, 6 );
	}
	else if ( rank == 8 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = random( 2, 3 );
		g_actors[actor_index].coliseum_list[1].m_rank = random( 4, 5 );
		g_actors[actor_index].coliseum_list[2].m_rank = random( 6, 7 );
	}
	else if ( rank == 9 )
	{
		g_actors[actor_index].coliseum_list[0].m_rank = random( 3, 4 );
		g_actors[actor_index].coliseum_list[1].m_rank = random( 5, 6 );
		g_actors[actor_index].coliseum_list[2].m_rank = random( 7, 8 );
	}
	else
	{
		g_actors[actor_index].coliseum_list[0].m_rank = (int)(random( rank * 3, rank * 5 ) / 10);
		g_actors[actor_index].coliseum_list[1].m_rank = (int)(random( rank * 6, rank * 8 ) / 10);
		g_actors[actor_index].coliseum_list[2].m_rank = (int)(random( rank * 9, rank * 10 ) / 10);
		if ( g_actors[actor_index].coliseum_list[2].m_rank == g_actors[actor_index].coliseum_rank )
			g_actors[actor_index].coliseum_list[2].m_rank -= 1;
	}
	return 0;
}

int coliseum_match( int actor_index )
{
	// ��ȡ��������������
	coliseum_list_load( actor_index, "match_list" );
	return 0;
}

// ���ý��մ���
int coliseum_todaynum_reset( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( actor_get_today_char_times( actor_index, TODAY_CHAR_COLISEUM_TODAYNUM ) < global.coliseum_todaynum )
	{
		return -1;
	}

	if ( actor_change_token( actor_index, -80, PATH_COLISEUM, 1 ) < 0 )
	{
		return -1;
	}

	actor_set_today_char_times( actor_index, TODAY_CHAR_COLISEUM_TODAYNUM, 0 );
	int value[2] = { 0 };
	value[0] = 1;
	value[1] = global.coliseum_todaynum - actor_get_today_char_times( actor_index, TODAY_CHAR_COLISEUM_TODAYNUM );
	actor_notify_value( actor_index, NOTIFY_COLISEUM, 2, value, NULL );
	return 0;
}

// ��һ��
int coliseum_update( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( actor_check_uselimit_cd( actor_index, USELIMIT_CD_COLISEUM_UPDATE ) > 0 )
	{ // ��ȴ���ȴ����
		actor_notify_alert( actor_index, 4300 );
		return -1;
	}
	actor_set_uselimit_cd( actor_index, USELIMIT_CD_COLISEUM_UPDATE, 180 );

	// ����ƥ��
	coliseum_match( actor_index );
	coliseum_sendlist( actor_index );
	return 0;
}

// �滻����
int coliseum_replaceteam( int actor_index, int kind0, int kind1, int kind2 )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( actor_check_uselimit_cd( actor_index, USELIMIT_CD_COLISEUM_TEAM ) > 0 )
	{//	�����滻��̫��Ƶ���ˣ��Ե�һ��
		actor_notify_pop( actor_index, 4306 );
		return -1;
	}

	if ( kind0 > 0 && kind1 > 0 )
	{
		if ( kind0 == kind1 )
			return -1;
	}
	if ( kind0 > 0 && kind2 > 0 )
	{
		if ( kind0 == kind2 )
			return -1;
	}
	if ( kind1 > 0 && kind2 > 0 )
	{
		if ( kind1 == kind2 )
			return -1;
	}

	// ����Ƿ��Ѿ�������佫��
	if ( kind0 > 0 )
	{
		Hero *pHasHero = hero_getptr( actor_index, kind0 );
		if ( !pHasHero )
			return -1;
	}

	if ( kind1 > 0 )
	{
		Hero *pHasHero = hero_getptr( actor_index, kind1 );
		if ( !pHasHero )
			return -1;
	}

	if ( kind2 > 0 )
	{
		Hero *pHasHero = hero_getptr( actor_index, kind2 );
		if ( !pHasHero )
			return -1;
	}

	g_actors[actor_index].coliseum_team[0] = kind0;
	g_actors[actor_index].coliseum_team[1] = kind1;
	g_actors[actor_index].coliseum_team[2] = kind2;

	// �����Լ��Ĵ浵
	coliseum_save( actor_index );

	// Ƶ������
	actor_set_uselimit_cd( actor_index, USELIMIT_CD_COLISEUM_TEAM, 30 );

	int value[5] = { 0 };
	value[0] = 0;
	value[1] = g_actors[actor_index].coliseum_team[0];
	value[2] = g_actors[actor_index].coliseum_team[1];
	value[3] = g_actors[actor_index].coliseum_team[2];
	value[4] = actor_check_uselimit_cd( actor_index, USELIMIT_CD_COLISEUM_TEAM );
	actor_notify_value( actor_index, NOTIFY_COLISEUM, 5, value, NULL );
	return 0;
}

int coliseum_fight( int actor_index, int index )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( actor_get_today_char_times( actor_index, TODAY_CHAR_COLISEUM_TODAYNUM ) >= global.coliseum_todaynum )
	{
		return -1;
	}
	if ( index < 0 || index >= COLISEUMLIST_MAX )
	{
		return -1;
	}

	// �ҵ�����
	Hero pAttack[COLISEUMLIST_HERO_MAX] = {0};
	coliseum_match_attr_load( g_actors[actor_index].actorid, pAttack, COLISEUMLIST_HERO_MAX, "match_attr" );

	Hero pDefense[COLISEUMLIST_HERO_MAX] = { 0 };
	coliseum_match_attr_load( g_actors[actor_index].coliseum_list[index].m_actorid, pDefense, COLISEUMLIST_HERO_MAX, "match_attr" );

	// ��ʼս��
	if ( fight_start_bycoliseum( actor_index, pAttack, pDefense, COLISEUMLIST_HERO_MAX ) < 0 )
		return -1;

	if ( g_fight.result == FIGHT_WIN )
	{
		// ս��ʤ����Ҫ���Լ�����ʤ����
		coliseum_serieswin_set( actor_index, 1 );
	}
	else
	{
		// ս��ʧ�ܣ�Ҫ�ս��Լ�����ʤ
		coliseum_serieswin_set( actor_index, 0 );

	}

	// ����ս��
	char content[256] = { 0 };
	sprintf( content, "{\"lastrank\":%d,\"nowrank\":%d}", g_actors[actor_index].coliseum_rank, g_actors[actor_index].coliseum_list[index].m_rank );
	fight_play( actor_index, g_fight.unit_json, content );

	// ս����Ϣ����
	int fightid = coliseum_fight_insert( g_fight.unit_json, (int)time(NULL), "match_fight" );
	if ( fightid > 0 )
	{
		coliseum_log_insert( &g_actors[actor_index].coliseum_my, &g_actors[actor_index].coliseum_list[index], g_fight.result, fightid, "match_log" );
	}

	// ���մ���
	actor_add_today_char_times( actor_index, TODAY_CHAR_COLISEUM_TODAYNUM );

	// ��������
	g_actors[actor_index].coliseum_rank = coliseum_getrank( g_actors[actor_index].actorid );
	g_actors[actor_index].coliseum_list[index].m_rank = coliseum_getrank( g_actors[actor_index].coliseum_list[index].m_actorid );

	// ���ʶԾ���Ϣ����
	if ( g_fight.result == FIGHT_WIN && g_actors[actor_index].coliseum_list[index].m_rank <= 5 )
	{
		int fightid = coliseum_fight_insert( g_fight.unit_json, (int)time( NULL ), "match_fight_wonderful" );
		if ( fightid > 0 )
		{
			coliseum_log_insert( &g_actors[actor_index].coliseum_my, &g_actors[actor_index].coliseum_list[index], g_fight.result, fightid, "match_log_wonderful" );
		}
	}

	// ���������Լ���¼��Ϣ
	coliseum_change_rank( actor_index, index, g_fight.result );
	return 0;
}

// ��������
int coliseum_change_rank( int actor_index, int index, char winflag )
{
	ACTOR_CHECK_INDEX( actor_index );
	int winner1 = 1;
	int winner2 = 0;
	int rank_change1 = 0;
	int rank_change2 = 0;
	int rank_transform = 0;
	int fight_index = -1;
	bool topchange = false;

	if ( winflag == FIGHT_WIN )
	{
		winner1 = 0;
		winner2 = 1;
		rank_change1 = g_actors[actor_index].coliseum_list[index].m_rank;
		rank_change2 = g_actors[actor_index].coliseum_rank;
		if ( rank_change1 >= rank_change2 )
		{
			// ����������������Լ�(���ܷ����������˵�ս��Ӱ��������)���Ͳ�����������
			rank_change1 = 0;
			rank_change2 = 0;
		}
		else
		{
			/* ��һ�δ� */
			if ( g_actors[actor_index].coliseum_maxrank <= 0 )
			{
				g_actors[actor_index].coliseum_maxrank = g_actors[actor_index].coliseum_rank;
			}

			rank_transform = rank_change2 - rank_change1;
			g_actors[actor_index].coliseum_rank = rank_change1;
			g_actors[actor_index].coliseum_list[index].m_rank = rank_change2;

			coliseum_setrank( g_actors[actor_index].actorid, g_actors[actor_index].coliseum_rank, &g_actors[actor_index].coliseum_my );
			coliseum_setrank( g_actors[actor_index].coliseum_list[index].m_actorid, g_actors[actor_index].coliseum_list[index].m_rank, &g_actors[actor_index].coliseum_list[index] );
			if ( g_actors[actor_index].coliseum_rank <= 5 )
			{
				// 6038	<color = d95df4ff>[������]< / color><color = 03DE27FF>[{0}]{1}< / color>������<color = 03DE27FF>[{2}]{3}< / color>���ɹ����<color = 03DE27FF>��{ 4 }��< / color>��ͷ�Σ����Ѽ��롰���ʶԾ�����
				char v1[64] = { 0 };
				char v2[64] = { 0 };
				char v3[64] = { 0 };
				char v4[64] = { 0 };
				char v5[64] = { 0 };
				sprintf( v1, "%s%d", TAG_NATION, g_actors[actor_index].nation );
				sprintf( v2, "%s", g_actors[actor_index].name );
				sprintf( v3, "%s%d", TAG_NATION, g_actors[actor_index].coliseum_list[index].m_nation );
				sprintf( v4, "%s", g_actors[actor_index].coliseum_list[index].m_name );
				sprintf( v5, "%d", g_actors[actor_index].coliseum_rank );
				system_talkjson_world( 6038, v1, v2, v3, v4, v5, NULL, 2 );
			}

			/* ��������ʷ������� */
			topchange = coliseum_check_Historyrank( actor_index ) == 1;
		}
	}

	if ( winflag == FIGHT_WIN )
	{
		coliseum_match( actor_index );
		coliseum_sendlist( actor_index );
	}
	else
	{
		int value[2] = { 0 };
		value[0] = 1;
		value[1] = global.coliseum_todaynum - actor_get_today_char_times( actor_index, TODAY_CHAR_COLISEUM_TODAYNUM );
		actor_notify_value( actor_index, NOTIFY_COLISEUM, 2, value, NULL );
	}
	return 0;
}

// ��ʷ����������
int coliseum_check_Historyrank( int actor_index )
{
	double fcurvalue;
	double fhistoryvalue;
	int token = 0;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	/* ������� */
	if ( g_actors[actor_index].coliseum_maxrank > g_actors[actor_index].coliseum_rank )
	{   // max{ ���������-����������/100 + 2000/���������5 }
		// max{ ����߱Ƚ�-���������/100 + 2000/���������5 }
		token = (int)((g_actors[actor_index].coliseum_maxrank-g_actors[actor_index].coliseum_rank) / 10.0f + 2000 / g_actors[actor_index].coliseum_rank);
		if ( token < 5 )
			token = 5;
		
		/* ���ͽ����ʼ� */
		// 5549	��ϲ����������<color = 03de27ff>( { 0 } )< / color>����������ʷ�������<color = 03de27ff>( { 1 } )< / color>��Ϊ�˱������ھ������е�ͻ�����֣��Լ���Խ���ң����ͽ������£�
		char v1[32] = { 0 };
		char v2[32] = { 0 };
		char attach[128] = { 0 };
		sprintf( v1, "%d", g_actors[actor_index].coliseum_rank );
		sprintf( v2, "%d", g_actors[actor_index].coliseum_maxrank );
		sprintf( attach, "%d,%d@", AWARDKIND_TOKEN, token );
		mail_system( MAIL_ACTORID, g_actors[actor_index].actorid, 5056, 5549, v1, v2, NULL, attach, 0 );

		/* ��������������� */
		g_actors[actor_index].coliseum_maxrank = g_actors[actor_index].coliseum_rank;
		return 1;
	}
	return 0;
}

// ս����־
int coliseum_fight_insert( char *json, int optime, char *pTab )
{
	if ( json )
		db_escape( json, g_szFightJson, 0 );
	sprintf( g_szFightSql, "INSERT INTO %s (content,optime) VALUES ('%s','%d')", pTab, g_szFightJson, optime );
	if ( mysql_query( myGame, g_szFightSql ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", g_szFightSql );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	return (int)mysql_insert_id( myGame );
}

// ������Ϣ
int coliseum_log_insert( SLK_NetS_ColiseumCity *pAttack, SLK_NetS_ColiseumCity *pDefense, char result, int fightid, char *pTab )
{
	if ( !pAttack || !pDefense )
		return -1;
	char szSQL[2048];
	char tmpstr[256];
	char szText_name[64] = { 0 };
	int member = 0;

	// ������
	sprintf( szSQL, "INSERT INTO %s (a_actorid,a_name,a_rank,a_nation,a_level,a_bpower,a_herokind0,a_herocolor0,a_herolevel0,a_herokind1,a_herocolor1,a_herolevel1,a_herokind2,a_herocolor2,a_herolevel2,d_actorid,d_name,d_rank,d_nation,d_level,d_bpower,d_herokind0,d_herocolor0,d_herolevel0,d_herokind1,d_herocolor1,d_herolevel1,d_herokind2,d_herocolor2,d_herolevel2,win,fightid,optime) "
		"VALUES( '%d','%s','%d','%d','%d','%d'", pTab,
		pAttack->m_actorid, db_escape( (const char *)pAttack->m_name, szText_name, 0 ), pAttack->m_rank, pAttack->m_nation, pAttack->m_level, pAttack->m_bpower );

	member = 0;
	for ( member = 0; member < COLISEUMLIST_HERO_MAX; member++ )
	{
		if ( pAttack->m_hero[member].m_kind <= 0 )
			break;
		sprintf( tmpstr, ",'%d','%d','%d'", pAttack->m_hero[member].m_kind, pAttack->m_hero[member].m_color, pAttack->m_hero[member].m_level );
		strcat( szSQL, tmpstr );
	}
	for ( ; member < COLISEUMLIST_HERO_MAX; member++ )
	{
		sprintf( tmpstr, ",'0','0','0'" );
		strcat( szSQL, tmpstr );
	}
	
	// ������
	sprintf( tmpstr, ",'%d','%s','%d','%d','%d','%d'",
		pDefense->m_actorid, db_escape( (const char *)pDefense->m_name, szText_name, 0 ), pDefense->m_rank, pDefense->m_nation, pDefense->m_level, pDefense->m_bpower );
	strcat( szSQL, tmpstr );

	member = 0;
	for ( member = 0; member < COLISEUMLIST_HERO_MAX; member++ )
	{
		if ( pDefense->m_hero[member].m_kind <= 0 )
			break;
		sprintf( tmpstr, ",'%d','%d','%d'", pDefense->m_hero[member].m_kind, pDefense->m_hero[member].m_color, pDefense->m_hero[member].m_level );
		strcat( szSQL, tmpstr );
	}
	for ( ; member < COLISEUMLIST_HERO_MAX; member++ )
	{
		sprintf( tmpstr, ",'0','0','0'" );
		strcat( szSQL, tmpstr );
	}

	//
	sprintf( tmpstr, ",'%d','%d','%d'", result, fightid, ( int )time( NULL ) );
	strcat( szSQL, tmpstr );

	strcat( szSQL, ");" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}

	return 0;
}

// ɾ������
int coliseum_log_overdue()
{
	char szSQL[2048] = { 0 };
	int thistime = (int)time( NULL );
	sprintf( szSQL, "DELETE FROM `match_log` WHERE optime < %d", (thistime - 7 * 86400) );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	sprintf( szSQL, "DELETE FROM `match_fight` WHERE optime < %d", (thistime - 7 * 86400) );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	return 0;
}

// ս����־�б�
int coliseum_loglist( int actor_index, char type )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[1024];
	ACTOR_CHECK_INDEX( actor_index );
	SLK_NetS_ColiseumLogList pValue = { 0 };
	//int size = sizeof( SLK_NetS_ColiseumLogList );
	if ( type == 0 )
	{
		sprintf( szSQL, "select a_actorid,a_name,a_rank,a_nation,a_level,a_bpower,a_herokind0,a_herocolor0,a_herolevel0,a_herokind1,a_herocolor1,a_herolevel1,a_herokind2,a_herocolor2,a_herolevel2,d_actorid,d_name,d_rank,d_nation,d_level,d_bpower,d_herokind0,d_herocolor0,d_herolevel0,d_herokind1,d_herocolor1,d_herolevel1,d_herokind2,d_herocolor2,d_herolevel2,win,fightid,optime from match_log where a_actorid='%d' or d_actorid='%d' order by optime desc limit 8;", g_actors[actor_index].actorid, g_actors[actor_index].actorid );
	}
	else if ( type == 1 )
	{
		sprintf( szSQL, "select a_actorid,a_name,a_rank,a_nation,a_level,a_bpower,a_herokind0,a_herocolor0,a_herolevel0,a_herokind1,a_herocolor1,a_herolevel1,a_herokind2,a_herocolor2,a_herolevel2,d_actorid,d_name,d_rank,d_nation,d_level,d_bpower,d_herokind0,d_herocolor0,d_herolevel0,d_herokind1,d_herocolor1,d_herolevel1,d_herokind2,d_herocolor2,d_herolevel2,win,fightid,optime from match_log_wonderful order by optime desc limit 8;" );
	}
	else
		return -1;

	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( row = mysql_fetch_row( res ) )
	{
		int index = 0;

		//
		pValue.m_list[pValue.m_count].m_attack.m_actorid = atoi( row[index++] );
		strncpy( pValue.m_list[pValue.m_count].m_attack.m_name, row[index++], NAME_SIZE );
		pValue.m_list[pValue.m_count].m_attack.m_namelen = strlen( pValue.m_list[pValue.m_count].m_attack.m_name );
		pValue.m_list[pValue.m_count].m_attack.m_rank = atoi( row[index++] );
		pValue.m_list[pValue.m_count].m_attack.m_nation = atoi( row[index++] );
		pValue.m_list[pValue.m_count].m_attack.m_level = atoi( row[index++] );
		pValue.m_list[pValue.m_count].m_attack.m_bpower = atoi( row[index++] );
		for ( int i = 0; i < 3; i++ )
		{
			pValue.m_list[pValue.m_count].m_attack.m_hero[i].m_kind = atoi( row[index++] );
			pValue.m_list[pValue.m_count].m_attack.m_hero[i].m_color = atoi( row[index++] );
			pValue.m_list[pValue.m_count].m_attack.m_hero[i].m_level = atoi( row[index++] );
		}

		// 
		pValue.m_list[pValue.m_count].m_defense.m_actorid = atoi( row[index++] );
		strncpy( pValue.m_list[pValue.m_count].m_defense.m_name, row[index++], NAME_SIZE );
		pValue.m_list[pValue.m_count].m_defense.m_namelen = strlen( pValue.m_list[pValue.m_count].m_defense.m_name );
		pValue.m_list[pValue.m_count].m_defense.m_rank = atoi( row[index++] );
		pValue.m_list[pValue.m_count].m_defense.m_nation = atoi( row[index++] );
		pValue.m_list[pValue.m_count].m_defense.m_level = atoi( row[index++] );
		pValue.m_list[pValue.m_count].m_defense.m_bpower = atoi( row[index++] );
		for ( int i = 0; i < 3; i++ )
		{
			pValue.m_list[pValue.m_count].m_defense.m_hero[i].m_kind = atoi( row[index++] );
			pValue.m_list[pValue.m_count].m_defense.m_hero[i].m_color = atoi( row[index++] );
			pValue.m_list[pValue.m_count].m_defense.m_hero[i].m_level = atoi( row[index++] );
		}
		pValue.m_list[pValue.m_count].m_win = atoi( row[index++] );
		pValue.m_list[pValue.m_count].m_fightid = atoi( row[index++] );
		pValue.m_list[pValue.m_count].m_optime = atoi( row[index++] );
		pValue.m_count += 1;
		if ( pValue.m_count >= 10 )
		{
			break;
		}
	}
	pValue.m_type = type;
	netsend_coliseumloglist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// ��ȡ���а��б�
int coliseum_load_ranklist()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[256];
	int index = 0;
	sprintf( szSQL, "select actorid,rank,name,namelen,nation,level,bpower,herokind0,herocolor0,herolevel0,herokind1,herocolor1,herolevel1,herokind2,herocolor2,herolevel2 from match_list where rank <= 30 and rank > 0" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	while ( row = mysql_fetch_row( res ) )
	{
		if ( row[0] == NULL || row[1] == NULL )
			continue;
		int rank = atoi( row[1] ) - 1;
		if ( rank < 0 || rank >= 30 )
			continue;
		index = 0;
		// ���ǻ�����Ϣ
		int actorid = atoi( row[index++] );
		g_ColiseumRankList.m_list[rank].m_rank = atoi( row[index++] );
		strncpy( g_ColiseumRankList.m_list[rank].m_name, row[index++], NAME_SIZE );
		g_ColiseumRankList.m_list[rank].m_namelen = atoi( row[index++] );
		g_ColiseumRankList.m_list[rank].m_nation = atoi( row[index++] );
		g_ColiseumRankList.m_list[rank].m_level = atoi( row[index++] );
		g_ColiseumRankList.m_list[rank].m_bpower = atoi( row[index++] );
		g_ColiseumRankList.m_count += 1;
	}
	mysql_free_result( res );
	return 0;
}

// ���а��б�
int coliseum_ranklist( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	netsend_coliseumranklist_S( actor_index, SENDTYPE_ACTOR, &g_ColiseumRankList );
	return 0;
}

// ��ʤ
int coliseum_serieswin_set( int actor_index, char flag )
{
	ACTOR_CHECK_INDEX( actor_index );
	//if ( flag == 1 )
	//{// �����Լ�����ʤ����
	//	actor_add_today_times( actor_index, TODAY_INDEX_COLISEUMWIN );
	//}
	//else
	//{
	//	actor_set_today_times( actor_index, TODAY_INDEX_COLISEUMWIN, 0 );
	//}
	return 0;
}

// ÿ�ս���
int coliseum_award_everyday()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[256];
	int actorid = 0;
	int rank = 0;
	sprintf( szSQL, "select actorid, rank from match_list where rank <= 2000 and rank > 0 and actorid > 100000" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( row = mysql_fetch_row( res ) )
	{
		actorid = 0;
		rank = 0;
		if ( row[0] )
			actorid = atoi( row[0] );
		if ( row[1] )
			rank = atoi( row[1] );
		if ( actorid <= 0 || rank <= 0 )
			continue;
		coliseum_awardget( actorid, rank );
	}
	mysql_free_result( res );
	return 0;
}

int coliseum_awardget( int actorid, int rank )
{
	int awardgroup = 0;
	if ( rank == 1 )
		awardgroup = 320;
	else if ( rank == 2 )
		awardgroup = 321;
	else if ( rank == 3 )
		awardgroup = 322;
	else if ( rank == 4 )
		awardgroup = 323;
	else if ( rank == 5 )
		awardgroup = 324;
	else if ( rank >= 6 && rank <= 10 )
		awardgroup = 325;
	else if ( rank >= 11 && rank <= 20 )
		awardgroup = 326;
	else if ( rank >= 21 && rank <= 50 )
		awardgroup = 327;
	else if ( rank >= 51 && rank <= 100 )
		awardgroup = 328;
	else if ( rank >= 101 && rank <= 200 )
		awardgroup = 329;
	else if ( rank >= 201 && rank <= 400 )
		awardgroup = 330;
	else if ( rank >= 401 && rank <= 1000 )
		awardgroup = 331;
	else
		awardgroup = 332;

	char attach[256] = { 0 };
	awardgroup_mail( awardgroup, 0, attach );

	// 5550	���ľ���������Ϊ<color=03de27ff>{0}</color>��Ϊ�˱������ھ������е�ͻ�����֣����ͽ������£�
	char v1[32] = { 0 };
	sprintf( v1, "%d", rank );
	mail_system( MAIL_ACTORID, actorid, 5057, 5550, v1, NULL, NULL, attach, 1 );
	return 0;
}

int coliseum_rankaward_sendlist( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	SLK_NetS_ColiseumAwardList pValue = { 0 };

	awardgroup_allget( 320, &pValue.m_list[pValue.m_count] );
	pValue.m_list[pValue.m_count].m_minrank = 1;
	pValue.m_list[pValue.m_count].m_maxrank = 1;
	pValue.m_count += 1;

	awardgroup_allget( 321, &pValue.m_list[pValue.m_count] );
	pValue.m_list[pValue.m_count].m_minrank = 2;
	pValue.m_list[pValue.m_count].m_maxrank = 2;
	pValue.m_count += 1;

	awardgroup_allget( 322, &pValue.m_list[pValue.m_count] );
	pValue.m_list[pValue.m_count].m_minrank = 3;
	pValue.m_list[pValue.m_count].m_maxrank = 3;
	pValue.m_count += 1;

	awardgroup_allget( 323, &pValue.m_list[pValue.m_count] );
	pValue.m_list[pValue.m_count].m_minrank = 4;
	pValue.m_list[pValue.m_count].m_maxrank = 4;
	pValue.m_count += 1;

	awardgroup_allget( 324, &pValue.m_list[pValue.m_count] );
	pValue.m_list[pValue.m_count].m_minrank = 5;
	pValue.m_list[pValue.m_count].m_maxrank = 5;
	pValue.m_count += 1;

	awardgroup_allget( 325, &pValue.m_list[pValue.m_count] );
	pValue.m_list[pValue.m_count].m_minrank = 6;
	pValue.m_list[pValue.m_count].m_maxrank = 10;
	pValue.m_count += 1;

	awardgroup_allget( 326, &pValue.m_list[pValue.m_count] );
	pValue.m_list[pValue.m_count].m_minrank = 11;
	pValue.m_list[pValue.m_count].m_maxrank = 20;
	pValue.m_count += 1;

	awardgroup_allget( 327, &pValue.m_list[pValue.m_count] );
	pValue.m_list[pValue.m_count].m_minrank = 21;
	pValue.m_list[pValue.m_count].m_maxrank = 50;
	pValue.m_count += 1;

	awardgroup_allget( 328, &pValue.m_list[pValue.m_count] );
	pValue.m_list[pValue.m_count].m_minrank = 51;
	pValue.m_list[pValue.m_count].m_maxrank = 100;
	pValue.m_count += 1;

	awardgroup_allget( 329, &pValue.m_list[pValue.m_count] );
	pValue.m_list[pValue.m_count].m_minrank = 101;
	pValue.m_list[pValue.m_count].m_maxrank = 200;
	pValue.m_count += 1;

	awardgroup_allget( 330, &pValue.m_list[pValue.m_count] );
	pValue.m_list[pValue.m_count].m_minrank = 201;
	pValue.m_list[pValue.m_count].m_maxrank = 400;
	pValue.m_count += 1;

	awardgroup_allget( 331, &pValue.m_list[pValue.m_count] );
	pValue.m_list[pValue.m_count].m_minrank = 401;
	pValue.m_list[pValue.m_count].m_maxrank = 1000;
	pValue.m_count += 1;

	awardgroup_allget( 332, &pValue.m_list[pValue.m_count] );
	pValue.m_list[pValue.m_count].m_minrank = 1001;
	pValue.m_list[pValue.m_count].m_maxrank = 2000;
	pValue.m_count += 1;

	netsend_coliseumawardlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// ��ȡս����ϸ����
int coliseum_fight_read( int actor_index, int fightid, char type )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[256];
	if ( type == 0 )
	{ 
		sprintf( szSQL, "select content from match_fight where id='%d'", fightid );
	}
	else if ( type == 1 )
	{
		sprintf( szSQL, "select content from match_fight_wonderful where id='%d'", fightid );
	}
	else
		return -1;
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( (row = mysql_fetch_row( res )) )
	{
		coliseum_fight_send( actor_index, row[0] );
	}
	else
	{
		// ����һ���Ѿ������ڵ���Ϣ
		mysql_free_result( res );
		actor_notify_pop( actor_index, 4307 );
		return -1;
	}
	mysql_free_result( res );
	return 0;
}

// ����ս����ϸ����
int coliseum_fight_send( int actor_index, char *content )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( !content )
		return -1;
	SLK_NetS_ColiseumFight pValue = { 0 };

	// ���Ϳ�ʼ
	pValue.m_flag = 0;
	pValue.m_content_length = 0;
	netsend_coliseumfight_S( actor_index, SENDTYPE_ACTOR, &pValue );

	// �����ݹ���
	pValue.m_flag = 1;
	int total_length = strlen( content );
	int offset = 0;
	while ( total_length > 0 )
	{
		int length = strlen( content + offset );
		if ( length > 1600 )
			length = 1600;
		pValue.m_content_length = length;
		memcpy( pValue.m_content, content + offset, sizeof( char )*length );
		offset += length;
		total_length -= length;
		netsend_coliseumfight_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}

	// �������
	pValue.m_flag = 2;
	pValue.m_content_length = 0;
	pValue.m_type = 0;
	netsend_coliseumfight_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// ---------------------------------------------������---------------------------------------------------------
int coliseum_robot_checkinit()
{
	return coliseum_robot_init();
}
/* �����������˳�ʼ�� */
int coliseum_robot_init()
{
	/* �ȼ�龺�����б����Ƿ������� */
	if ( coliseum_robot_check() > 0 )
		return -1;

	/* 1.�����һ�Σ��������������� */
	coliseum_robot_delete( "match_list" );
	coliseum_robot_delete( "match_attr" );

	/* 2.��ʼ���� */
	printf( "coliseum robot create begin \n" );
	coliseum_robot_create( COLISEUM_ROBOT_COUNT );
	printf( "coliseum robot create end \n" );

	// ��ʼ�����а�
	coliseum_load_ranklist();
	return 0;
}

/* ��龺�����б����Ƿ������� */
int coliseum_robot_check()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[256];
	int matchcount = 0;
	sprintf( szSQL, "select count(*) from match_list;" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( row = mysql_fetch_row( res ) )
	{
		if ( row[0] )
			matchcount = atoi( row[0] );
	}
	else
	{
		mysql_free_result( res );
		return 0;
	}
	mysql_free_result( res );
	return matchcount;
}

/* ��վ������б������������ */
int coliseum_robot_delete( char *szTable )
{
	char szSQL[256];
	sprintf( szSQL, "TRUNCATE `%s`", szTable );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	return 0;
}

/* ��������ɫ�б��� */
int coliseum_robot_match_list_insert( Actor *pActor, City *pCity, Hero *pHero, int count, char *pTab )
{
	char szSQL[2048];
	char tmpstr[256];
	int member = 0;
	int totalpower = 0;

	// ��ȡ��ǰ������������
	for ( int tmpi = 0; tmpi < count; tmpi++ )
	{
		int herokind = pActor->coliseum_team[tmpi];
		hero_attr_calc( pCity, &pHero[tmpi] );
		pHero[tmpi].colorup = hero_getline( pCity, HERO_STATE_FIGHT ); // ���⴦��
		totalpower += pHero[tmpi].bp_hero + pHero[tmpi].bp_equip + pHero[tmpi].bp_tech + pHero[tmpi].bp_nequip + pHero[tmpi].bp_girl;
	}

	// ����
	char szText_name[64] = { 0 };
	sprintf( szSQL, "REPLACE INTO %s VALUES( '%d','%d','%s','%d','%d','%d','%d'", pTab,
		pActor->actorid,
		pActor->coliseum_rank,
		db_escape( (const char *)pActor->name, szText_name, 0 ),
		strlen( pActor->name ),
		pActor->nation,
		pActor->level,
		totalpower );

	// Ӣ�ۻ�����Ϣ
	for ( member = 0; member < COLISEUMLIST_HERO_MAX; member++ )
	{
		if ( pHero[member].kind <= 0 )
			break;
		sprintf( tmpstr, ",'%d','%d','%d'", pHero[member].kind, pHero[member].color, pHero[member].level );
		strcat( szSQL, tmpstr );
	}
	for ( ; member < COLISEUMLIST_HERO_MAX; member++ )
	{
		sprintf( tmpstr, ",'0','0','0'" );
		strcat( szSQL, tmpstr );
	}
	strcat( szSQL, ");" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}

	// ������Ϣ
	coliseum_match_attr_delete( pActor->actorid, "match_attr" );
	for ( int tmpi = 0; tmpi < COLISEUMLIST_HERO_MAX; tmpi++ )
	{
		if ( pHero[tmpi].kind > 0 )
		{
			coliseum_match_attr_save( pActor->actorid, tmpi, &pHero[tmpi], "match_attr" );
		}
	}
	return 0;
}

/* �������� */
int coliseum_robot_create( int robotcount )
{
	int partnerid = 1;
	int itemid = 1;
	for ( int tmpi = 0; tmpi < robotcount; tmpi++ )
	{
		Actor actor = { 0 };
		City city = { 0 };
		Hero hero[COLISEUMLIST_HERO_MAX] = { 0 };
		int nation_level = 0;
		int nation_count = 0;
		char hero_randomtype = 0;
		/***************************************************** ��ɫ���� ******************************************************/
		/* ��ɫ���������� */
		actor.coliseum_rank = g_maxrank++;
		/* ��ɫid */
		actor.actorid = COLISEUM_ROBOT_ACTORID_OFFSET + tmpi;
		/* ���ǵȼ� */
		if ( tmpi <= robotcount * 1 / 5 )
		{
			actor.level = random( 95, 105 );
			nation_level = random( actor.level - 10, actor.level );
			nation_count = 5;
			city.place = random( 10, 20 );
			city.attr.hero_row_num = 2;
			hero_randomtype = 1;
		}
		else if ( tmpi <= robotcount * 2 / 5 )
		{
			actor.level = random( 85, 95 );
			nation_level = random( actor.level - 20, actor.level - 5 );
			nation_count = 4;
			city.place = random( 8, 15 );
			city.attr.hero_row_num = 2;
			hero_randomtype = 2;
		}
		else if ( tmpi <= robotcount * 3 / 5 )
		{
			actor.level = random( 75, 85 );
			nation_level = random( actor.level - 30, actor.level - 10 );
			nation_count = 3;
			city.place = random( 5, 10 );
			city.attr.hero_row_num = 2;
			hero_randomtype = 3;
		}
		else if ( tmpi <= robotcount * 4 / 5 )
		{
			actor.level = random( 65, 75 );
			nation_level = random( actor.level - 30, actor.level - 15 );
			nation_count = 2;
			city.place = random( 3, 8 );
			city.attr.hero_row_num = 1;
			hero_randomtype = 4;
		}
		else
		{
			actor.level = random( 50, 65 );
			nation_level = random( actor.level - 40, actor.level - 20 );
			nation_count = 1;
			city.place = random( 1, 5 );
			city.attr.hero_row_num = 1;
			hero_randomtype = 5;
		}
		/* ��ɫ���� */
		coliseum_randomname( actor.name );
		/* ���� */
		actor.nation = random( 1, 3 );

		/***************************************************** �ǳز��� ******************************************************/
		// ����
		for ( int type = 0; type < nation_count; type++ )
		{
			nation_level = nation_level <= 0 ? 1 : nation_level;
			city.neq_lv[type] = nation_level;
		}


		/***************************************************** �佫���� ******************************************************/
		static int s_herokindlist1[] = { 14, 15, 17, 22, 23, 25, 26, 27, 28, 29, 30, 31, 41, 42, 44, 45, 46, 47, 48, 49, 50, 56, 57, 51, 52, 58, 59, 60, 61, 62, 63, 64, 65, 66, 91, 92, 93, 94, 97, 99 };
		static int s_herokindlist2[] = { 31, 41, 42, 44, 45, 46, 47, 48, 49, 50, 56, 57, 51, 52, 58, 59, 60, 61, 62, 63, 64, 65, 66, 91, 92, 93, 94, 97, 99 };
		static int s_herokindlist_token[] = { 50, 58, 59, 60, 61, 62, 63, 64, 65, 66, 91, 92, 93, 94, 95, 96, 97, 98, 99, 71 };

		int herokind[3] = { 0, 0, 0 };
		int herocolor[3] = { 0, 0, 0 };
		int index = 0;
		if ( hero_randomtype == 1 )
		{
			ruffle( s_herokindlist_token, sizeof( s_herokindlist_token ) / sizeof( s_herokindlist_token[0] ) );
			herokind[0] = s_herokindlist_token[0];	herocolor[0] = hero_defaultcolor( herokind[0] );
			herokind[1] = s_herokindlist_token[1];	herocolor[1] = hero_defaultcolor( herokind[1] );
			herokind[2] = s_herokindlist_token[2];	herocolor[2] = hero_defaultcolor( herokind[2] );
		}
		else if ( hero_randomtype == 2 )
		{
			ruffle( s_herokindlist2, sizeof( s_herokindlist2 ) / sizeof( s_herokindlist2[0] ) );
			herokind[0] = s_herokindlist2[0];	herocolor[0] = hero_defaultcolor( herokind[0] );
			herokind[1] = s_herokindlist2[1];	herocolor[1] = hero_defaultcolor( herokind[1] );
			herokind[2] = s_herokindlist2[2];	herocolor[2] = hero_defaultcolor( herokind[2] );
		}
		else
		{
			ruffle( s_herokindlist1, sizeof( s_herokindlist1 ) / sizeof( s_herokindlist1[0] ) );
			herokind[0] = s_herokindlist1[0];	herocolor[0] = hero_defaultcolor( herokind[0] );
			herokind[1] = s_herokindlist1[1];	herocolor[1] = hero_defaultcolor( herokind[1] );
			herokind[2] = s_herokindlist1[2];	herocolor[2] = hero_defaultcolor( herokind[2] );
		}
		

		int sortindex[3] = { 0, 1, 2 };
		ruffle( sortindex, 3 );
		for ( int member = 0; member < 3; member++ )
		{
			hero[member].offset = HERO_COLISEUMOFFSET + member;
			hero[member].kind = herokind[sortindex[member]];
			hero[member].color = herocolor[sortindex[member]];
			hero[member].level = random( actor.level - 3, actor.level );
		}

		/* �����б� */
		coliseum_robot_match_list_insert( &actor, &city, hero, 3, "match_list" );
		char szMsg[16] = { 0 };
		sprintf( szMsg, "%d\r", tmpi );
		puts( szMsg );
	}
	return 0;
}

// �������
int coliseum_randomname( char *name )
{
	int lastindex = random( 1, g_lastname_maxnum - 1 );
	int firstindex = random( 1, g_firstname_maxnum - 1 );
	snprintf( name, NAME_SIZE, "%s%s", g_lastname[lastindex].str, g_firstname[lastindex].str );
	name[NAME_SIZE - 1] = 0;
	return 0;
}

