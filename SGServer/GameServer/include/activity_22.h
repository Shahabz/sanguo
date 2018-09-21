#ifndef _ACTIVITY_22_H
#define _ACTIVITY_22_H

#define ACTIVITY22_QUEUE_MAX		256		// ս�����г���
#define ACTIVITY22_QUEUEMARCH_MAX	1024	// �о����г���
#define ACTIVITY22_ATTACK	1
#define ACTIVITY22_DEFENSE	2
#define ACTIVITY22_RANK_MAX	30	// Ѫս���а�

struct _activity22info {
	char nation;	//ռ���
	int actorid;
	char actorname[NAME_SIZE];
	int attack_total;	//�������ܱ���
	int defense_total;	//�������ܱ���
	int sec;	//ս������ʱ
	int attack_queue[ACTIVITY22_QUEUE_MAX];		//��������
	int defense_queue[ACTIVITY22_QUEUE_MAX];	//���ض���

	int attack_march[ACTIVITY22_QUEUEMARCH_MAX];	//�����о�����
	int defense_march[ACTIVITY22_QUEUEMARCH_MAX];	//�����о�����
};
typedef struct _activity22info Activity22Info;

typedef struct _activity22rank
{
	int actorid;
	int city_index;
}Activity22Rank;

// ����Ѫս
void activity_22_load();
void activity_22_onwarning( int lefttime );
void activity_22_onopen();
void activity_22_onend();
void activity_22_onclose();
void activity_22_onlogic();
int activity_22_sendinfo( int actor_index );

char activity22_attack_or_defense( int army_index );
int activity22_queueadd( char attack, int army_index );
int activity22_queuedel( char attack, int army_index );
int activity22_queueget( char attack );

int activity22_marchadd( char attack, int army_index );
int activity22_marchdel( char attack, int army_index );
int activity22_marcharrived( char attack, int army_index );

int activity22_totalcalc( char attack );
int activity22_change_nation( int nation );
void activity22_fightlogic();
void activity22_fight();
int activity22_sendlist( int actor_index, char attack );
int activity22_sendupdate( int value );

int activity22_addkill( int army_index, int value );
int activity22_rank_calc( City *pCity );
int activity22_rank_sendlist( int actor_index );

// ���Ϣ
int activity22_mapsendinfo( int actor_index );
#endif
