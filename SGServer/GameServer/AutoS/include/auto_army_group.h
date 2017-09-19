#ifndef __ARMYGROUP_AUTO_H
#define __ARMYGROUP_AUTO_H
#include "define.h"

struct _army_group {
//--Automatically generated
				int index;	//集结索引
				int id;	//集结唯一ID,合法性验证
				char state;	//集结状态
				int statetime;	//状态时间戳
				int stateduration;	//状态持续多少秒
				char from_type;	//发起者的对象类型
				int from_id;	//发起者的对象ID
				int from_index;	//发起者的对象索引
				short from_posx;	//发起者的对象坐标
				short from_posy;	//发起者的对象坐标
				char to_type;	//目的对象类型
				int to_id;	//目的对象ID
				int to_index;	//目的对象索引
				short to_posx;	//目的对象坐标
				short to_posy;	//目的对象坐标
				int attack_armyindex[128];	//临时存储攻击方集结部队索引
				int defense_armyindex[128];	//临时存储防御方集结部队索引
};
typedef struct _army_group ArmyGroup;

typedef ArmyGroup * (*LPCB_GETARMYGROUP)( int index );
typedef int (*LPCB_LOADARMYGROUP)( int index );
int army_group_load_auto( LPCB_GETARMYGROUP pCB_GetArmyGroup, LPCB_LOADARMYGROUP pCB_LoadArmyGroup, const char *pTab );
int army_group_save_auto( ArmyGroup *pArmyGroup, const char *pTab, FILE *fp );

#endif
