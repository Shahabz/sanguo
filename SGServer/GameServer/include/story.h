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
// 更新副本状态
int story_sendrankstate( int actor_index, int id, int savetype, int saveoffset );
// 资源副本攻打
int story_res_get( int actor_index, int id );
// 资源副本重置
int story_res_reset( int actor_index, int id );
// 招募副本免费招募
int story_hero_free( int actor_index, int id );
// 招募副本花钱招募
int story_hero_token( int actor_index, int id );
// 图纸副本购买
int story_drawing_buy( int actor_index, int id );
// 图纸副本攻打
int story_drawing_get( int actor_index, int id );

// 副本扫荡
int story_sweep( int actor_index, int id, int herokind0, int herokind1, int herokind2, int herokind3 );
#endif
