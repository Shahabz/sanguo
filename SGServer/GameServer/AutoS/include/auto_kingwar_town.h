#ifndef __KINGWARTOWN_AUTO_H
#define __KINGWARTOWN_AUTO_H
#include "define.h"

struct _kingwar_town {
//--Automatically generated
				int id;	//id
				char nation;	//占领国
				int attack_total;	//攻击方总兵力
				int defense_total;	//防御方总兵力
				int sec;	//战斗倒计时
				int attack_queue[4096];	//攻击队列
				int attack_queue_head;	//攻击队列头
				int attack_queue_tail;	//攻击队列尾
				int defense_queue[4096];	//防守队列
				int defense_queue_head;	//防守队列头
				int defense_queue_tail;	//防守队列尾
};
typedef struct _kingwar_town KingwarTown;

typedef KingwarTown * (*LPCB_GETKINGWARTOWN)( int id );
typedef int (*LPCB_LOADKINGWARTOWN)( int id );
int kingwar_town_load_auto( LPCB_GETKINGWARTOWN pCB_GetKingwarTown, LPCB_LOADKINGWARTOWN pCB_LoadKingwarTown, const char *pTab );
int kingwar_town_save_auto( KingwarTown *pKingwarTown, const char *pTab, FILE *fp );
int kingwar_town_batch_save_auto( KingwarTown *pKingwarTown, int maxcount,  const char *pTab, FILE *fp );

#endif
