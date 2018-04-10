#ifndef __GIRL_AUTO_H
#define __GIRL_AUTO_H
#include "define.h"

struct _actor_girl {
//--Automatically generated
				int actorid;	//角色
				char kind;	//种类
				char color;	//颜色
				char sflag;	//标记
				unsigned char love_level;	//亲密等级
				unsigned short soul;	//碎片
				unsigned short love_exp;	//亲密度
				unsigned short love_today;	//今天获取亲密度
				short herokind;	//关联男将
};
typedef struct _actor_girl Girl;

typedef Girl * (*LPCB_GETGIRL)( int actorid, int kind );
int actor_girl_load_auto( int actorid, int city_index, LPCB_GETGIRL pCB_GetGirl, const char *pTab );
int actor_girl_save_auto( int actorid, int offset, Girl *pGirl, const char *pTab, FILE *fp );
int actor_girl_batch_save_auto( int actorid, Girl *pGirl, int maxcount, const char *pTab, FILE *fp );

#endif
