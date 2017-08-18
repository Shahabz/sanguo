#ifndef _STORY_H_
#define _STORY_H_
#include "define.h"

#define STORY_TYPE_NORMAL	1	// 普通
#define STORY_TYPE_BOSS		2	// 精英
#define STORY_TYPE_RES		3	// 资源
#define STORY_TYPE_ITEM		4	// 道具
#define STORY_TYPE_HERO		5	// 武将招募
#define STORY_TYPE_DRAWING	6	// 装备图纸

int story_sendinfo( int actor_index );
int story_sendrank( int actor_index, int id );
int story_battle( int actor_index, SLK_NetC_StoryBattle *pValue );
#endif
