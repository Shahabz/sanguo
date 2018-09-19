#ifndef _GIRL_H_
#define _GIRL_H_
#include "define.h"
#include "auto_data_girl.h"
#define ACTOR_GIRL_MAX	80

#define GIRL_SFLAG_MARRY	0	// �Ƿ���
#define GIRL_SFLAG_MAKELOVE	1	// �����Ƿ����ܻ�����

#define FANGSHI_AWARDNUM	14

// ��offsetת���ɶ�Ӧ�����Խṹ
Girl *girl_getptr( int city_index, int kind );
GirlInfoConfig *girl_getconfig( int kind, int color );
void girl_makestruct( City *pCity, Girl *pGirl, SLK_NetS_Girl *pValue );
int girl_getgirl( City *pCity, int kind, char path ); // ��һ��Ů��
int girl_getsoul( City *pCity, int kind, int soul, char path ); // ��һ��Ů����Ƭ
int girl_info( City *pCity, Girl *pGirl ); // Ů����Ϣ
int girl_list( int actor_index ); // Ů���б�
void girl_update(); //  ÿ���������
int girl_allot( int actor_index, short herokind, short girlkind ); // ί���н�
int girl_unallot( int actor_index, short kind ); // ���ί��
int girl_colorup( int actor_index, short kind ); // ͻ��
int girl_loveitemuse( int actor_index, short kind, short itemkind );// ���ܶȵ���ʹ��
int girl_marry( int actor_index, short kind ); // ϲ������
int girl_makelove( int actor_index, short kind ); // ���ܻ���
float girl_loveexp_speed( short herokind, short kind, char color );
int girl_addloveexp_killenemy( City *pCity, short herokind, short kind );
int girl_addloveexp_citywar( City *pCity, short herokind, short kind );
int girl_addloveexp_nationwar( City *pCity, short herokind, short kind );
int girl_addloveexp_kingwar( City *pCity, short herokind, short kind );
int girl_addloveexp( City *pCity, Girl *pGirl, int exp, short path ); // ������Ƕ�
int girl_son_growth( int actor_index, short kind ); // ��Ů��ʦ
void girl_gm_getall( City *pCity );
int girl_gm_son_growth_quick( City *pCity, short herokind );
char girl_withherokind( short herokind );

int girl_shop_sendlist( int actor_index );
int girl_shop_buy( int actor_index, int index, short id );
int girl_shop_update( int actor_index );
int girl_shop_update_manual( int actor_index );

int fangshi_sendinfo( int actor_index );
int fangshi_visit( int actor_index, int type );
int fangshi_visit_getaward( int actor_index );
int fangshi_node_sendaward( int actor_index, int nodeid );
int fangshi_palace_update( int actor_index );
int fangshi_palace_sendinfo( int actor_index );
int fangshi_palace_see( int actor_index, int index );
#endif
