#ifndef _MAP_EVENT_H_
#define _MAP_EVENT_H_
#include "define.h"
#include "auto_data_map_eventinfo.h"

int map_event_load();
int map_event_save( FILE *fp );
int map_event_maxcount_set( int maxcount );
MapEvent *map_event_getptr( int index );
MapEventInfo *map_event_getconfig( int kind );

// ��ʾ��Ԫ����
void map_event_makeunit( int index, SLK_NetS_AddMapUnit *pAttr, int actorid );

// λ��
void map_event_getpos( int index, short *posx, short *posy );

// ����һ���¼�
int map_event_create( short kind, short posx, short posy, int city_index );

// ɾ��һ���¼�
int map_event_delete( int index, int city_index );

// �ı��¼�����
int map_event_changepos( int index, short posx, short posy );
int map_event_changepos_rand( int index );
int map_event_changepos_randhaspos( int index, short posx, short posy );

// ÿ����ҵ��¼��߼�
int map_event_citylogic( int city_index );

// ɾ����������ҵ��¼�
int map_event_delete_actor( int actorid, int city_index );

// �¼�����
int map_event_num( int actorid );

// ����һ����ķ�Χ��������
int map_event_range_brush( short kind, short posx, short posy, int range, int city_index );

int map_event_addcity( int city_index, int index );
int map_event_deletecity( int city_index, int index );
int map_event_addcity( int city_index, int index );

int map_event_gather( int actor_index,int unit_index );
int map_event_getaward( int actor_index, int unit_index );
#endif
