#ifndef _MAP_ENEMY_H_
#define _MAP_ENEMY_H_
#include "define.h"
#include "auto_data_map_enemyinfo.h"

int map_enemy_load();
int map_enemy_save( FILE *fp );
int map_enemy_maxcount_set( int maxcount );
MapEnemy *map_enemy_getptr( int index );
MapEnemyInfo *map_enemy_getconfig( int kind );

// ��ʾ��Ԫ����
void map_enemy_makeunit( int index, SLK_NetS_AddMapUnit *pAttr );

// λ��
void map_enemy_getpos( int index, short *posx, short *posy );

// ����һ������
int map_enemy_create( short kind, short posx, short posy, int deltime, int actorid );

// ɾ��һ������
int map_enemy_delete( int index );

// Ұ������
int map_enemy_num( int zoneid, int kind );

// ����һ����ķ�Χ��������
int map_enemy_range_brush( short kind, short posx, short posy, int range, int deltime, int actorid );

// �Ѿ���ѡ������,��ѡ���ĸ����ڲ�ɾ��
void map_enemy_addselected( int index );
void map_enemy_subselected( int index );

// ��ȡҰ�ֽ���
int map_enemy_sendaward( int actor_index, int kind );
#endif
