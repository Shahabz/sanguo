#ifndef __ARMY_AUTO_H
#define __ARMY_AUTO_H
#include "define.h"

struct _army {
//--Automatically generated
				int index;	//部队索引
				int id;	//部队唯一ID,合法性验证
				int unit_index;	//显示索引
				char state;	//状态
				int statetime;	//状态时间戳
				int stateduration;	//状态持续多少秒
				int action;	//行为
				short from_type;	//出发的对象类型
				int from_id;	//出发的对象ID
				int from_index;	//出发的对象索引
				short from_posx;	//出发的对象坐标
				short from_posy;	//出发的对象坐标
				short to_type;	//目的对象类型
				int to_id;	//目的对象ID
				int to_index;	//目的对象索引
				short to_posx;	//目的对象坐标
				short to_posy;	//目的对象坐标
				short posx;	//当前坐标
				short posy;	//当前坐标
				short walk_len;	//已经移动长度
				SLK_WalkPath walk_path;	//部队移动路径
				char name[22];	//自定义名称
				int soldiers;	//人数
				int wounded;	//伤员
				char body;	//体力
				char morale;	//士气
				short attack;	//攻击
				short defense;	//防御
				short attack_speed;	//攻击速度
				short move_speed;	//移动速度
				short attrpoint;	//未分配点数
				short level;	//战斗等级
				char exp;	//战斗经验
				short equip[6];	//拥有装备
				short skillid[4];	//拥有技能
				int food;	//携带粮食
				int money;	//携带金钱
				short itemkind[4];	//携带物品种类
				int itemnum[4];	//携带物品数量
				int sec;	//秒计数
};
typedef struct _army Army;

typedef Army * (*LPCB_GETARMY)( int index );
typedef int (*LPCB_LOADARMY)( int index );
int army_load_auto( LPCB_GETARMY pCB_GetArmy, LPCB_LOADARMY pCB_LoadArmy, char *pTab );
int army_save_auto( Army *pArmy, char *pTab, FILE *fp );
int army_batch_save_auto( Army *pArmy, int maxcount,  char *pTab, FILE *fp );

#endif
