#ifndef _STORY_H_
#define _STORY_H_
#include "define.h"

#define STORY_TYPE_NORMAL	1	// 普通
#define STORY_TYPE_BOSS		2	// 精英
#define STORY_TYPE_RES		3	// 资源
#define STORY_TYPE_ITEM		4	// 道具
#define STORY_TYPE_HERO		5	// 武将招募
#define STORY_TYPE_DRAWING	6	// 装备图纸

#define STORY_STAR_OFFSETMAX		128	// 精英副本星级
#define STORY_HERO_OFFSETMAX		32	// 招募副本
#define STORY_RESTIME_OFFSETMAX		32	// 资源副本时间
#define STORY_RESNUM_OFFSETMAX		32	// 资源副本次数
#define STORY_RESRESET_OFFSETMAX	32	// 资源副本重置次数
#define STORY_ITEMNUM_OFFSETMAX		64	// 道具副本掉落次数
#define STORY_DRAWING_OFFSETMAX		16	// 装备图纸副本是否购买

int story_sendinfo( int actor_index );
int story_sendrank( int actor_index, int id );
int story_battle( int actor_index, SLK_NetC_StoryBattle *pValue );
#endif
