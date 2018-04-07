#ifndef __GIRL_AUTO_H
#define __GIRL_AUTO_H
#include "define.h"

struct _actor_girl {
//--Automatically generated
				int actorid;	//角色
				char kind;	//种类
				char love_level;	//亲密等级
				char love_num;	//今天亲密次数
				char color;	//颜色
				short soul;	//碎片
				short love_exp;	//亲密度
				int love_fday;	//亲密时间
};
typedef struct _actor_girl Girl;

typedef Girl * (*LPCB_GETGIRL)( int actorid, int kind );
int actor_girl_load_auto( int actorid, int city_index, LPCB_GETGIRL pCB_GetGirl, const char *pTab );
int actor_girl_save_auto( int actorid, int offset, Girl *pGirl, const char *pTab, FILE *fp );
int actor_girl_batch_save_auto( int actorid, Girl *pGirl, int maxcount, const char *pTab, FILE *fp );

#endif
