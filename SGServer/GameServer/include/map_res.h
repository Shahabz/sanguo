#ifndef _MAP_RES_H_
#define _MAP_RES_H_
#include "define.h"
#include "auto_data_map_resinfo.h"

int map_res_load();
int map_res_save( FILE *fp );
int map_res_maxcount_set( int maxcount );
MapRes *map_res_getptr( int index );
MapResInfo *map_res_getconfig( int kind );

// ��ʾ��Ԫ����
void map_res_makeunit( int index, SLK_NetS_AddMapUnit *pAttr );

// λ��
void map_res_getpos( int index, short *posx, short *posy );

// ����һ����Դ��
int map_res_create( short kind, short posx, short posy );

// ɾ��һ����Դ��
int map_res_delete( int index, char checkarmy );

// ��Դ������
int map_res_num( int zoneid, int kind );

// ÿ����
int map_res_logic();

// �������ڲɼ��Ĳ���
int map_res_setarmy( int index, int army_index );

// ��ȡ���ڲɼ��Ĳ���
int map_res_getarmy( int index );

// ��ȡ��ʾ����
int map_res_getunit( int index );

// ��Դ����Ϣ
int map_res_sendinfo( int actor_index, int unit_index );
#endif
