#ifndef _NATION_H_
#define _NATION_H_
#include "define.h"

#define NATION_MAX			4		// ��Ⱥ��4������
#define NATION_NOTICE_MAX	512		// ����
#define NATION_OFFICIAL_MAX	10		// ��Ա����
#define NATION_TOWN_WAR_MAX	256		// ��ս����
#define NATION_CITY_WAR_MAX	1024	// ��ս����
#define NATION_LOG_CACHE_QUEUE_COUNT		200		// ������־

#define NATION_QUEST_MAX		3
#define NATION_QUESTKIND_TOWN	1	// ��������-��ս
#define NATION_QUESTKIND_CITY	2	// ��������-��ս
#define NATION_QUESTKIND_ENEMY	3	// ��������-����

#define NATION_MISSION_MAX			3
#define NATION_MISSIONKIND_BUILD	1	// ������������-����
#define NATION_MISSIONKIND_CITY		2	// ������������-��ս
#define NATION_MISSIONKIND_TOWN		3	// ������������-��ս

#define NATION_RANK_MAX			3
#define NATION_RANK_BUILD		1	// ������������-����
#define NATION_RANK_CITY		2	// ������������-��ս
#define NATION_RANK_TOWN		3	// ������������-��ս

#define NATION_RANK_MEMBERNUM	5	// ������������-ѡ������

#define NATION_OFFICIAL_STATE_OPEN		0 // �״ο���
#define NATION_OFFICIAL_STATE_SELECT	1 // ѡ����
#define NATION_OFFICIAL_STATE_TERM		2 // ����

#define NATION_CANDIDATE_MAX	10 // ��ѡ����
#define NATION_OFFICIAL_R1	1 // κ��
#define NATION_OFFICIAL_R2	2 // ����
#define NATION_OFFICIAL_R3	3 // ����
#define NATION_OFFICIAL_R4	4 // ة��
#define NATION_OFFICIAL_R5	5 // ����
#define NATION_OFFICIAL_R6	6 // ����

// ��ԱȨ��
#define NATION_OFFICIAL_RIGHT_DISMISSAL			1	//
#define NATION_OFFICIAL_RIGHT_FIGHT				2	//
#define NATION_OFFICIAL_RIGHT_ONLINE_NOTIFY		3	//
#define NATION_OFFICIAL_RIGHT_NOTIFY			4	//
#define NATION_OFFICIAL_RIGHT_CITYFIGHT_BODY	5	//
#define NATION_OFFICIAL_RIGHT_CALLNUM			6	//
#define NATION_OFFICIAL_RIGHT_YIELD				7	//
#define NATION_OFFICIAL_RIGHT_BUILDQUICK		8	//
#define NATION_OFFICIAL_RIGHT_CALLACTORNUM			9	//

#define NATION_LOG_TYPE_DEFENSE_LOSE	1 //1870	�ҹ�����{ 0 }{1}[{2}], ��{ 3 }��{ 4 }ռ��
#define NATION_LOG_TYPE_DEFENSE_WIN		2 //1871	�ҹ�����{ 0 }{1}[{2}], �ɹ��ĵ���������{ 3 }��{ 4 }�Ľ���
#define NATION_LOG_TYPE_REBUILD			3 //1872	{0}�ؽ���{ 1 }��{ 2 }[{3}]
#define NATION_LOG_TYPE_LEAVE			4 //1873	{0}�������壬������{ 1 }{2}[{3}]
#define NATION_LOG_TYPE_ATTACK_GIVEUP	5 //1874	{0}{1}[{2}]�����ҹ���{ 3 }���ڹ��ƣ���Ϊ�ҹ�ռ��ʳǳǳ��ѵ�7�����ޣ��ʷ���ռ�죬�óǹ�Ⱥ����������
#define NATION_LOG_TYPE_ATTACK_WIN		6 //1875	{0}{1}[{3}]�����ҹ���{ 3 }���ڹ���

// �����������а�
typedef struct _nation_rank_member
{
	int actorid;
	int city_index;
}NationRankMember;

// �����������а�
typedef struct _nation_rank
{
	NationRankMember member[NATION_RANK_MAX][NATION_RANK_MEMBERNUM];
}NationRank;

int nation_load();
int nation_save( FILE *fp );
Nation *nation_getptr( int nation );

// ���ù��Ҷ���
int nation_capital_townid( int nation, int townid );
int nation_capital_townid_get( int nation );

// �����˿�
int nation_people_calc( int nation );
int nation_people_get( int nation );
int nation_people_famous_calc( int nation );
int nation_people_capital_calc( int nation );
int nation_people_capital_set( int nation, int people );

// ���ҳ�ֵ����ȫ���������
int nation_paytoken_add( int nation, int token );
int nation_paytoken_get( int nation );

// �ù���ָ�����ͳǳ�����
int nation_town_num( int nation, int towntype );

// ��ս������Ϣ
int nation_city_war_add( int nation, int group_index );
int nation_city_war_del( int nation, int group_index );

// ��ս������Ϣ
int nation_town_war_add( int nation, int group_index );
int nation_town_war_del( int nation, int group_index );

// ��������
void nation_kingname( char nation, char *out );

// �޸Ĺ��ҹ���
int nation_notice( int actor_index, char *pMsg );

// ��������
int nation_online_notify( int actor_index );

// ���Ҿ���
int nation_exp( char nation, int exp );
int nation_tiance_point( char nation, int value );

// ������Ϣ
int nation_sendinfo( int actor_index );
// ������Ϣ
int nation_sendbase( int actor_index );

// ���ҽ�������
int nation_build( int actor_index );
int nation_build_reset( int actor_index );

// ���Ҿ�λ����
int nation_place_upgrade( int actor_index );

// ���ҳǳ�
int nation_town_sendinfo( int actor_index, int townid, char path );

// ���ҳǳ��б�
int nation_town_sendlist( int actor_index );

// �޸�
int nation_town_repair( int actor_index, int townid );

// ����ս���б�
int nation_town_warlist( int actor_index );
int nation_city_warlist( int actor_index );

// ��������
int nation_quest_sendlist( int actor_index );
int nation_quest_update();
int nation_quest_addvalue( City *pCity, char kind, int value );
int nation_quest_getaward( int actor_index, int kind );

// ������������
int nation_mission_sendlist( int actor_index );
int nation_mission_addvalue( char nation, char kind, int value );
int nation_mission_update();
int nation_mission_getaward( int actor_index, int baglevel );

// �����������а�
int nation_rank_sendlist( int actor_index );
void nation_rank_addvalue( City *pCity, char kind, int value );
int nation_rank_calc();
void nation_rank_update();

// ��Ա����
int nation_official_open();
void nation_official_logic();
int nation_official_candidate_create();
int nation_official_create();
int nation_official_ballot( int actor_index, int target_actorid, int istoken );
int nation_official_sendlist( int actor_index );
int nation_official_replace_sendlist( int actor_index );
int nation_official_replace_up( int actor_index, int target_actorid );
int nation_official_replace_down( int actor_index, int target_actorid );
int nation_official_right( char official, char right );

// ������־
int nationlog_cache_queue_add( SLK_NetS_NationLog *pCache, SLK_NetS_NationLog *pValue );
int nationlog_cache_queue_add_db( char nation, SLK_NetS_NationLog *pValue );
int nationlog_cache_loaddb( SLK_NetS_NationLog *pCache, char nation );
int nationlog_cache_load();
void nationlog_add( char nation, int type, int townid, char *name, char target_nation );
int nationlog_sendlist( int actor_index, int id );
#endif
