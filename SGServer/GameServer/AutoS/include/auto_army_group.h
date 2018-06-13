#ifndef __ARMYGROUP_AUTO_H
#define __ARMYGROUP_AUTO_H
#include "define.h"

struct _army_group {
//--Automatically generated
				int index;	//��������
				int id;	//����ΨһID,�Ϸ�����֤
				char state;	//����״̬
				int statetime;	//״̬ʱ���
				int stateduration;	//״̬����������
				char type;	//��������
				char from_type;	//�����ߵĶ�������
				int from_id;	//�����ߵĶ���ID
				int from_index;	//�����ߵĶ�������
				short from_posx;	//�����ߵĶ�������
				short from_posy;	//�����ߵĶ�������
				char from_nation;	//�����ߵĹ���
				char from_helpnum;	//����������ʣ�����
				char to_type;	//Ŀ�Ķ�������
				int to_id;	//Ŀ�Ķ���ID
				int to_index;	//Ŀ�Ķ�������
				short to_posx;	//Ŀ�Ķ�������
				short to_posy;	//Ŀ�Ķ�������
				char to_nation;	//Ŀ�Ĺ���
				char to_helpnum;	//Ŀ������ʣ�����
				int leader_index;	//�ӳ�����
				int attack_armyindex[128];	//��ʱ�洢���������Ჿ������
				int defense_armyindex[128];	//��ʱ�洢���������Ჿ������
				int attack_helpernum[12];	//��Ӷ��
				int defense_helpernum[12];	//��Ӷ��
};
typedef struct _army_group ArmyGroup;

typedef ArmyGroup * (*LPCB_GETARMYGROUP)( int index );
typedef int (*LPCB_LOADARMYGROUP)( int index );
int army_group_load_auto( LPCB_GETARMYGROUP pCB_GetArmyGroup, LPCB_LOADARMYGROUP pCB_LoadArmyGroup, const char *pTab );
int army_group_save_auto( ArmyGroup *pArmyGroup, const char *pTab, FILE *fp );

#endif
