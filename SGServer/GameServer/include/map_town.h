#ifndef _MAP_TOWN_H_
#define _MAP_TOWN_H_
#include "define.h"

#define MAPUNIT_TYPE_TOWN_XIAN		1	// ��
#define MAPUNIT_TYPE_TOWN_JUN		2	// ��
#define MAPUNIT_TYPE_TOWN_ZHISUO	3	// ����
#define MAPUNIT_TYPE_TOWN_ZHFD		4	// �����
#define MAPUNIT_TYPE_TOWN_GJFD		5	// ���ҷ��
#define MAPUNIT_TYPE_TOWN_LUOYANG	6	// ����
#define MAPUNIT_KING_TOWNID			200		// ����

#define MAP_TOWN_JOIN_MAX				128	// ��ս��Ա
#define MAP_TOWN_MONSTER_MAX			16	// ��������
#define MAP_TOWN_UNDERFIRE_GROUP_MAX	4

int map_town_load();
int map_town_save( FILE *fp );
MapTown *map_town_getptr( int townid );

// ��ʾ��Ԫ����
void map_town_makeunit( int index, SLK_NetS_AddMapUnit *pAttr );

// λ��
void map_town_getpos( int index, short *posx, short *posy );

// ��ȡ����
char map_town_getnation( int townid );

// ��ȡ��������
char map_town_getzone( int townid );

// �����ؾ�
void map_town_monster_reset( int townid, char reset );

// ���г���ÿ���߼�
void map_town_alllogic();

// �������
int map_town_ask_owner( int actor_index, int townid );

// �������
int map_town_alloc_owner( int townid );

// ��������
int map_town_owner_leave( int actor_index, int townid );

// ����
int map_town_soldiers_repair( int actor_index, int townid );

// ��������
int map_town_levy( int actor_index, int townid, int type );

// ������������
int map_town_owner_award();

// ���ǳ�����������������
int map_town_attack_checkstart();
// ���ǳ���������
int map_town_attack( int townid );

// ���ǿ������辭��
int map_town_dev_expmax( int townid );

// ��Ӷ��ǿ�������
int map_town_dev_addexp( int townid, int exp );

// ���ǿ���
int map_town_dev( int actor_index, int townid );

// ���ǿ�������cd
int map_town_dev_delcd( int actor_index, int townid );

// �޸�����
int map_town_changename( int actor_index, int townid, char *pname );

// ��ȡ������
int map_town_sendaward( int actor_index, int townid );

// ��ȡ������Ϣ
void map_town_info_makestruct( SLK_NetS_MapTownInfo *pValue, MapTown *pTown, int actorid, char path );
int map_town_sendinfo( int actor_index, int townid );

// ��ȡ������Ϣ
int map_town_ex_sendinfo( int actor_index, int townid );

// �����б�
int map_town_ask_owner_sendlist( int actor_index, int townid );

// ���򱻹�����Ϣ���
int map_town_underfire_groupadd( int townid, int group_index );

// ���򱻹�����Ϣ�Ƴ�
int map_town_underfire_groupdel( int townid, int group_index );
#endif
