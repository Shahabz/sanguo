#ifndef _ITEM_H_
#define _ITEM_H_
#include "auto_actor_item.h"

#define ITEM_ABILITY_NUM	4	// ��ҵ��߶�̬���Ը���
#define ITEM_EXCHANGE_NUM	5	// ���߶һ����ϸ���

/* ��Ʒ��ɫ */
#define ITEM_COLOR_LEVEL_WHITE	0 // ��ɫ
#define ITEM_COLOR_LEVEL_GREEN	1 // ��ɫ
#define ITEM_COLOR_LEVEL_BLUE	2 // ��ɫ
#define ITEM_COLOR_LEVEL_PURPLE	3 // ��ɫ
#define ITEM_COLOR_LEVEL_GOLD	4 // ��ɫ
#define ITEM_COLOR_LEVEL_RED	5 // ��ɫ


#define ITEM_COLOR_LEVEL_MAX	6 // �����ɫ����


/* ��Ʒ���� */
#define ITEM_TYPE_NORMAL_USE		1	// �������ߣ�����ʹ�ã�
#define ITEM_TYPE_SCRIPT_USE		2	// �ű�ʹ�������
#define	ITEM_TYPE_EQUIP_MATERIAL	3	// װ������
#define	ITEM_TYPE_EQUIP_DRAWING		4	// װ��ͼֽ
#define	ITEM_TYPE_ACTIVITY			5	// ��ࣨ�޵����ť������ʹ�ã�
#define	ITEM_TYPE_AUTO				6	// �Զ�ʹ����
#define	ITEM_TYPE_HEROEXP			7	// �佫����
#define ITEM_TYPE_QUICK				8	// ��������ߣ����ڣ�

/* ��̬���� */
#define ITEM_ABILITY_NONE					0
#define ITEM_ABILITY_AWARDGROUP		1	// �����
#define ITEM_ABILITY_SCRIPTID		2	// �ű�����ID
#define ITEM_ABILITY_ADDBODY		3	// ������
#define ITEM_ABILITY_ADDEXP			4	// ����������
#define ITEM_ABILITY_ADDHEROEXP		5	// ��Ӣ�۾���
#define ITEM_ABILITY_AWARDID		6	// ����id
#define ITEM_ABILITY_CITYRES_SILVER 11	// ��������
#define ITEM_ABILITY_CITYRES_WOOD	12	// ľ������
#define ITEM_ABILITY_CITYRES_FOOD	13	// ��ʳ����
#define ITEM_ABILITY_CITYRES_IRON	14	// ������
#define ITEM_ABILITY_BUFF			15	// buff
#define ITEM_ABILITY_CITYMOVE		16	// �ͼ�Ǩ��
#define ITEM_ABILITY_CORPS1			17	// �Ӳ���
#define ITEM_ABILITY_CORPS2			18	// �����
#define ITEM_ABILITY_CORPS3			19	// �ӹ���
#define ITEM_ABILITY_VIPEXP			20	// VIP����
#define ITEM_ABILITY_GIRLTYPE		21	// Ů������
#define ITEM_ABILITY_GIRLLOVEEXP	22	// Ů�����Ƕ�

/* ���߲��� */ 
#define ITEM_PROCESS_INFO		1 // ��ȡ������Ϣ
#define ITEM_PROCESS_USE		2 // ʹ�õ���
#define ITEM_PROCESS_SELL		3 // ���۵���
#define ITEM_PROCESS_TOKENUSE	4 // ��ʯ��ʽʹ�õ���
#define ITEM_PROCESS_BUY		5 // ����

/* ���� */
#define ITEM_SITUATION_NOUSEMORE	0x01	// ��ֹʹ�ö��

/* ������Ʒ�ɹ��󷵻صĽṹ */
typedef struct _itemout
{
	int m_itemkind;			// ��Ʒ����
	short m_item_offset;	// ��Ʒ����λ��
	short m_count;			// ���ӵĸ���
}ItemOut;

/* ��Ʒ��Ϣ��ʼ�� */
int itemkind_init();
int itemkind_reload();
int item_maxid_init();

/* ��ȡ��Ʒȫ��������Ϣ */
char *item_getname( int itemkind );
short item_getlevel( int itemkind );
int item_getprice( int itemkind );
int item_gettoken( int itemkind );
short item_gettype( int itemkind );
unsigned char item_getsituation( int itemkind );
short item_get_overlap( int itemkind );
char item_getcolorlevel( int itemkind );

/* ��Ʒ��̬���� */
short item_get_base_ability( int itemkind, char pos );
int item_get_base_value( int itemkind, char pos );
int item_get_ability_value_withpos( int itemkind, char pos, short *ability, int *value );
char item_get_ability_value( int itemkind, short ability, int *value );
int _item_get_ability_value( Item *pItem, short ability );
int _item_set_ability_value( Item *pItem, short ability, int value );

/* ��offsetת���ɶ�Ӧ�����Խṹ */
Item *item_getptr( int actor_index, int offset );

/* ���������߲������� */
int item_load( int actor_index );	// ��Ҷ�ȡ����
int item_save( int actor_index, FILE *fp ); // ��ұ������е���
int item_insert( int actor_index, short offset ); // �������ߵ�ʱ����ã�����մ����ĵ���
int item_getitem( int actor_index, int itemkind, int itemnum, char color, char path ); // ��һ�õ���
int item_create( int actor_index, int itemkind, int itemnum, char color, ItemOut *pOut ); // ��Ҵ�������
int item_lost( int actor_index, int itemkind, int num, char path ); // �������������ߣ�����kind
int item_lostitem( int actor_index, int itemoffset, int num, char path ); // �������������ߣ�����offset
int item_deletebox( int actor_index, int item_offset ); // ɾ��һ�����ӵĵ���
int item_deleteitemdb( int actorid, i64 itemid ); // ɾ��һ�����߸���
int item_hasitem( int actor_index, int itemkind, int num ); // �Ƿ��������������Ʒ
int item_hasitem_between( int actor_index, int minitemkind, int maxitemkind ); // �Ƿ��������������Ʒ����
int item_getitemnum( int actor_index, int itemkind );// ��ȡ������ߵ����� 
int item_getitemindex( int actor_index, int itemkind );//��ȡ���ߵ�����
int item_getitemindex_whihitemid( int actor_index, i64 itemid );//��ȡ���ߵ�����

// ---------------------------------- ��������Ӧ���߲��� item_proc.cpp -----------------------------------------------------
int item_use( int actor_index, short itemindex, short itemnum, int herokind, int ask ); // ��������Ӧ���߲���-ʹ��
int item_use_withtoken( int actor_index, short itemkind, short itemnum, int herokind, int ask ); // ��������Ӧ���߲���-ֱ�ӻ�Ǯʹ��
int item_use_quick( int actor_index, short itemkind, char op, int buildingkind, int buildingoffset ); // ��������Ӧ���߲���-���ٵ���ʹ��
int item_drop( int actor_index, short itemindex, short dorpcount ); // ��������Ӧ���߲���-����
int item_buy( int actor_index, short itemkind, short itemnum ); // ��������Ӧ���߲���-����
int item_settle( int actor_index );   // ��������Ӧ���߲���-����

// ---------------------------------- ���������͵���Э�� item_send.cpp -----------------------------------------------------
int item_sendnotuse( int actor_index, short itemindex, int err );
void item_sendlost( int actor_index, int target_index, short item_index, short item_num, char path );
void item_sendget( int actor_index, int target_index, short item_index, short item_num, char path );
int item_list( int actor_index );	// ���͵����б�
int item_info( int actor_index, short itemindex ); // ���͵�����ϸ��Ϣ
int item_info_withitem( Item *pItem, int actor_index, short itemindex );// �����ķ��͵�����Ϣ����
int item_tokenbuy_getinfo( int actor_index, int itemkind );
int item_gm_getall( int actor_index );
#endif
