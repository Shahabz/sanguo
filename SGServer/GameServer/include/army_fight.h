#ifndef _ARMY_FIGHT_H_
#define _ARMY_FIGHT_H_

// ������ս�����
int army_vs_enemy( int army_index, Fight *pFight );

// ��ɼ���Դ�Ĳ���ս�����
int army_vs_res( int army_index, Fight *pFight );

// ���ֵĲ���ս�����
int army_vs_activity( int army_index, Fight *pFight );

// ���������ս�����
int army_vs_nationhero( int army_index, Fight *pFight );

// ��ֺ���ҳǳ�
int army_activity_vs_city( int army_index, City *pTargetCity, Fight *pFight );
#endif

