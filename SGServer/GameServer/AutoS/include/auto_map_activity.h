#ifndef __MAPACTIVITY_AUTO_H
#define __MAPACTIVITY_AUTO_H
#include "define.h"

struct _map_activity {
//--Automatically generated
				int index;	//����
				short posx;	//λ��
				short posy;	//λ��
				short kind;	//����
				int deltime;	//ɾ��ʱ��
				int actorid;	//���ڽ�ɫ
				int hp;	//ʣ��Ѫ��
				int maxhp;	//Ѫ��
				char activityid;	//�����
				int unit_index;	//��ʾ����
};
typedef struct _map_activity MapActivity;

typedef MapActivity * (*LPCB_GETMAPACTIVITY)( int index );
typedef int (*LPCB_LOADMAPACTIVITY)( int index );
int map_activity_load_auto( LPCB_GETMAPACTIVITY pCB_GetMapActivity, LPCB_LOADMAPACTIVITY pCB_LoadMapActivity, const char *pTab );
int map_activity_save_auto( MapActivity *pMapActivity, const char *pTab, FILE *fp );
int map_activity_batch_save_auto( MapActivity *pMapActivity, int maxcount,  const char *pTab, FILE *fp );

#endif
