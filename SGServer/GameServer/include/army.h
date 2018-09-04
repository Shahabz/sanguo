#ifndef _ARMY_H_
#define _ARMY_H_
#include "define.h"

// ��֧����״̬
#define ARMY_STATE_IDLE				0	// ����
#define ARMY_STATE_GROUP_END		3	// �������
#define ARMY_STATE_READY			4	// ׼����
#define ARMY_STATE_MARCH			10	// �о���
#define ARMY_STATE_FIGHT			20	// ս����
#define ARMY_STATE_OCCUPY			21	// פ����
#define ARMY_STATE_GATHER			22	// �ɼ���
#define ARMY_STATE_SHELTER			23	// �ӻ���
#define ARMY_STATE_HELP				24	// Ԯ����
#define ARMY_STATE_TALK				25	// ��̸��
#define ARMY_STATE_REBACK			30	// ������
#define ARMY_STATE_KINGWAR_READY	40	// �ʳ�ս�ѵ������
#define ARMY_STATE_KINGWAR_FIGHT	41	// �ʳ�ս�Ŷ�ս����
#define ARMY_STATE_KINGWAR_WAITSOS	42	// �ʳ�ս�����ȴ���Ԯ��
#define ARMY_STATE_KINGWAR_DEAD		43	// �ʳ�սս���Ѿ�����

// ��Ϊ
#define ARMY_ACTION_IDLE				0	// ����
#define ARMY_ACTION_FIGHT				1	// ȥս��
#define ARMY_ACTION_OCCUPY				2	// ȥפ��
#define ARMY_ACTION_GATHER				3	// ȥ�ɼ�
#define ARMY_ACTION_HELP_TROOP			6	// ȥʿ��Ԯ��
#define ARMY_ACTION_GROUP_CREATE		10	// ��������
#define ARMY_ACTION_GROUP_ATTACK		11	// ���뼯��(������)
#define ARMY_ACTION_GROUP_DEFENSE		12	// ���뼯��(������)
#define ARMY_ACTION_NATION_ATTACK		13	// �����ս(������)
#define ARMY_ACTION_NATION_DEFENSE		14	// �����ս(������)
#define ARMY_ACTION_KINGWAR				20	// �ʳ�ս

#define ARMY_REBACK_NORMAL			0	// ��������-����
#define ARMY_REBACK_RETURN			1	// ��������-��������
#define ARMY_REBACK_CALLBACK		2	// ��������-�����ٻ�
#define ARMY_REBACK_FIGHTWIN		3	// ��������-ս��ʤ������
#define ARMY_REBACK_FIGHTLOSE		4	// ��������-ս��ʧ�ܴ��
#define ARMY_REBACK_REPATRIAT		5	// ��������-����ǲ��

#define ARMY_STATE_FIGHT_TIME		2	// ս��ʱ��(S)
#define ARMY_CARRY_ITEMMAX			8	// ����Я���ĵ���
#define ARMY_HERO_MAX				4	// ��������佫

int army_load();
int army_save( FILE *fp );
Army *army_getptr( int army_index );
Army *army_getptr_cityhero( City *pCity, int herokind );

// ��ʾ��Ԫ����
void army_makeunit( int army_index, SLK_NetS_AddMapUnit *pAttr );

// ����λ��
void army_getpos( int army_index, short *posx, short *posy );

// ����
int army_battle( City *pCity, SLK_NetC_MapBattle *info );

// ����һ֧����
int army_create( char from_type, int from_id, char to_type, int to_id, char stat, SLK_NetC_MapBattle *info );

// ɾ��һ֧����
void army_delete( int army_index );

// ������Ϣ��ԭ���ǳ�
int army_tocity( int army_index );

// ���ò���״̬
void army_setstate( int army_index, char state );

// �����߼�
void army_alllogic();

// ��ֻ�����߼�
void army_logic( int army_index );

// ��ȡ���������ǳ�
City *army_getcityptr( int army_index );

// ��ȡ����Ŀ�������ǳ�
City *army_getcityptr_target( int army_index );

// ��ȡ��������
char *army_getname( int army_index );

// ��ȡ����Ŀ������
char *army_getname_target( int army_index );

// ��ȡ���������ǳ�
int army_getcityid( int army_index );

// ��ȡ����Ŀ��ǳ�
int army_getcityid_target( int army_index );

// ��ȡ������������
int army_getnation( int army_index );

// ��ȡ����Ŀ�����
int army_getnation_target( int army_index );

// �������Я������
int army_carry_additem( int army_index, int itemkind, int itemnum );

// �ɼ�ʱ��
int army_gather_time( int army_index );
int army_gather_time_reset( City *pCity );

// ���Ӳɼ�������
int army_gather_calc( int army_index );

// ���Ӳɼ���
int army_gather( int army_index );

// ���ӱӻ���
void army_shelter( int army_index );

// ����Ԯ����
void army_help( int army_index );

// פ����
void army_occupy( int army_index );

// ���ӽ�̸��
void army_talk( int army_index );

// ����ս����
void army_fight( int army_index );

// ��鲿���ǲ����Լ���
int army_myself( int actor_index, int army_index );

// �ʳ�Ѫս���д�ս
void army_kingwar_queue( int army_index );

// �ʳ�Ѫս����CD
void army_kingwar_waitsos( int army_index );
#endif
