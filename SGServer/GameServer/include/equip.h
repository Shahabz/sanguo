#ifndef _EQUIP_H_
#define _EQUIP_H_
#include "define.h"

#define EQUIP_TYPE_EQUIP1  1	// ����
#define EQUIP_TYPE_EQUIP2  2	// ����
#define EQUIP_TYPE_EQUIP3  3	// ����
#define EQUIP_TYPE_EQUIP4  4	// ͷ��
#define EQUIP_TYPE_EQUIP5  5	// ��ӡ
#define EQUIP_TYPE_EQUIP6  6	// ����
#define EQUIP_TYPE_MAX		6
#define EQUIP_BASEOFFSET		1000	// ����Ӣ�����ϵ�װ������ƫ��
#define EQUIP_WASHMAX		4	// ���4��ϴ������

#define EQUIP_ABILITY_ATTACK			1
#define EQUIP_ABILITY_DEFENSE			2
#define EQUIP_ABILITY_TROOPS			3
#define EQUIP_ABILITY_ATTACK_INCREASE	4
#define EQUIP_ABILITY_DEFENSE_INCREASE	5
#define EQUIP_ABILITY_ASSAULT			6
#define EQUIP_ABILITY_DEFEND			7

// �������߳ɹ��󷵻صĽṹ
typedef struct _equipout
{
	int m_equip_kind;		// ��Ʒ����
	int m_equip_offset;	// ��Ʒ����λ��
}EquipOut;

// ��offsetת���ɶ�Ӧ�����Խṹ
Equip *actor_equip_getptr( int actor_index, int offset );
Equip *actor_equip_getptr_temp( int actor_index, int offset );
Equip *city_equip_getptr( int city_index, int offset );
Equip *equip_getptr( int actor_index, int offset );
int equip_load( int actor_index );
int equip_save( int actor_index, FILE *fp );
int equip_maxid_init();

int equip_gettype( short kind );
int equip_getcolor( short kind );
int equip_insert( int actor_index, int offset ); // ����װ����ʱ����ã�����մ�����װ��
int equip_create( int actor_index, short equipkind, EquipOut *pOut ); // ��Ҵ���װ��
int equip_getequip( int actor_index, int equipkind, char path ); // ��һ��װ��
int equip_lostequip( int actor_index, int equipoffset, char path );
int equip_resolve( int actor_index, int equipoffset ); // �ֽ�
int equip_buyext( int actor_index ); // ������չ��

int equip_up( int actor_index, short herokind, int equip_offset ); // װ����,�����滻Ч��
int equip_down( int actor_index, short herokind, int index ); // ж���ϵ�װ��

int equip_down_all( int actor_index, short herokind ); // жһ���佫���ϵ�����װ��������
int equip_down_all_offline( int city_index, int actorid, short herokind ); // жһ���佫���ϵ�����װ��������

int equip_list( int actor_index ); // ����װ���б�
int equip_sendbag( int actor_index, int offset ); // ���±������װ��
int equip_sendhero( int actor_index, Hero *pHero, int index ); // ����Ӣ�۵�װ��
int equip_sendget( int actor_index, int offset, char path );
int equip_sendlost( int actor_index, short equipkind, int offset, char path );
int equip_heroupdate( int actor_index, Hero *pHero );// ����Ӣ��װ���б�
int equip_has( int actor_index, int kind );
int equip_herohas( int actor_index, int kind );

int equip_forgingtime( int city_index, short kind );
int equip_forging( int actor_index, short kind );
int equip_forging_quick( int actor_index );
int equip_forging_freequick( int actor_index );
int equip_forging_get( int actor_index );

int equip_wash_sendinfo( int actor_index );
int equip_wash_free( int actor_index, int offset );
int equip_wash_token( int actor_index, int offset );
int equip_wash_super( int actor_index, int offset );

int equip_gm_getall( int actor_index );
int equip_gm_getherogod( int actor_index );
#endif
