#ifndef _ARMY_GROUP_H_
#define _ARMY_GROUP_H_
#include "define.h"
#include "fight.h"

#define ARMYGROUP_MAXCOUNT			128	// �����128�˼���
#define ARMYGROUP_FIGHTHELPER		12	// ��Ӷ��

#define ARMYGROUP_STATE_IDLE		0	// ����
#define ARMYGROUP_STATE_ING			1	// ������


int armygroup_load();
int armygroup_save( FILE *fp );
ArmyGroup *armygroup_getptr( int group_index );

int armygroup_create( char type, char from_type, int from_id, char to_type, int to_id, int stateduration );
void armygroup_delete( int group_index );
void armygroup_alllogic();
void armygroup_logic( int group_index );

int armygroup_addarmy( int army_index, char zone );
void armygroup_delarmy( int army_index, char zone );
void armygroup_setleader( int group_index, int army_index );
int armygroup_dismiss( int army_index );
int armygroup_statetime( int group_index );
int armygroup_stateduration( int group_index );

// ����˫������
int armygroup_from_totals( int group_index );
int armygroup_to_totals( int group_index );

// ��ս���
int armygroup_vs_city( int group_index, Fight *pFight );
// ��սս���
int armygroup_vs_town( int group_index, Fight *pFight );

// ��ս�б�
int armygroup_city_sendlist( int actor_index, int unit_index );

// ��ս�б�
int armygroup_town_sendlist( int actor_index, int unit_index );

// ��ս����-��Ҵ���
int armygroup_nation_askcreate( int actor_index, int townid );

// ��ս����-AI���򴴽�
int armygroup_nation_askcreate_ai( int from_townid, int to_townid );

// ���й�ս��Ϣ
int armygroup_nation_sendlist( int actor_index );

// �������
int armygroup_askhelp( int actor_index, int group_index, int group_id );

// ���������˷����ʼ�
int armygroup_mail( int group_index, char attack, City *defenseCity, char type, char *title, char *content, char *attach, Fight *fight, char *name );
// ����פ����Ա�ʼ�
int armygroup_helpcity_mail( City *pCity, char type, char *title, char *content, char *attach, Fight *fight );
// ��Ӷ��
int armygroup_fighthelper_sendlist( int actor_index, int group_index );
int armygroup_fighthelper_buy( int actor_index, int group_index, int kind );
#endif
