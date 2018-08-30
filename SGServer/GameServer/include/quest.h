#ifndef _QUEST_H_
#define _QUEST_H_
#include "define.h"

#define QUEST_COMPLETEFLAG_NORMAL	0	// ȱʡ
#define QUEST_COMPLETEFLAG_SUCCESS	1	// ���
#define QUEST_COMPLETEFLAG_AWARDED	2	// �Ѿ���ȡ����

#define QUEST_DATATYPE_WELCOME				0	// ��ӭ����
#define	QUEST_DATATYPE_CREATENAME			1	// ��������
#define	QUEST_DATATYPE_CLICK				2	// ������� datakind=1��� 2ǰ������ 3���س��� 4�鿴������ 5ǰ������
#define	QUEST_DATATYPE_NEWFIGHT				3	// Χ��������ӵ�ɽ��
#define QUEST_DATATYPE_ACTOR_LEVEL			11	// �����ȼ� datatype=11 datakind=0 needvalue=�ȼ�
#define	QUEST_DATATYPE_BUILDING_LEVEL		12	// �����ȼ� datatype=12 datakind=����kind dataoffset=��Դ������� needvalue=�����ȼ�
#define	QUEST_DATATYPE_BUILDING_SILVER		13	// N����ӴﵽN�� datatype=13 datakind=�ȼ� needvalue=��������
#define	QUEST_DATATYPE_BUILDING_WOOD		14	// N��ľ���ﵽN�� datatype=14 datakind=�ȼ� needvalue=��������
#define	QUEST_DATATYPE_BUILDING_FOOD		15	// N��ũ��ﵽN�� datatype=15 datakind=�ȼ� needvalue=��������
#define	QUEST_DATATYPE_BUILDING_IRON		16	// N���󳡴ﵽN�� datatype=16 datakind=�ȼ� needvalue=��������
#define QUEST_DATATYPE_LEVYNUM				17	// ���մ��� datatype=17 datakind=0 needvalue=����
#define QUEST_DATATYPE_STORY				18	// ���� datatype=18 datakind=������� needvalue=1
#define	QUEST_DATATYPE_KILLMONSTER			19	// ������N�� datatype=19 datakind=0 needvalue=����
#define	QUEST_DATATYPE_KILLMONSTER_LEVEL	20	// ������N��N�� datatype=20 datakind=�ȼ� needvalue=����
#define	QUEST_DATATYPE_EQUIP_FORGING		21	// ����Nװ��N���� datatype=21 datakind=װ��kind needvalue=����
#define	QUEST_DATATYPE_EQUIP_WASH			22	// װ��ϴ������ datatype=22 datakind=0 needvalue=����
#define	QUEST_DATATYPE_EQUIP_COLORWASH		23	// ��NƷ��װ��������ϴ�������� datatype=23 datakind=��ɫ needvalue=1
#define	QUEST_DATATYPE_TRAIN				24	// ļ��N����N���� datatype=24 datakind=���֣�1��2��3�� needvalue=����
#define	QUEST_DATATYPE_TRAINCOUNT			25	// ļ��N�� datatype=25 datakind=0 needvalue=����
#define	QUEST_DATATYPE_CITY_TECH			26	// �о�N�Ƽ�N�� datatype=26 datakind=�Ƽ�ID needvalue=�ȼ�
#define	QUEST_DATATYPE_CITY_TECHONE			27	// �о�һ��N�Ƽ� datatype=27 datakind=�Ƽ�ID needvalue=1
#define	QUEST_DATATYPE_HERO_WASHCOUNT		28	// �佫ϴ��N�� datatype=28 datakind=0 needvalue=����
#define	QUEST_DATATYPE_HERO_COLORWASH		29	// ��һ��NƷ�ʵ��佫����ϴ�� datatype=29 datakind=��ɫ needvalue=1
#define	QUEST_DATATYPE_HERO_CALL			30	// ��ļ�佫 datatype=30 datakind=kind needvalue=1
#define	QUEST_DATATYPE_WISHING				31	// �۱���N�� datatype=31 datakind=0 needvalue=����
#define	QUEST_DATATYPE_CITYFIGHT			32	// �ɹ�����N���й���� datatype=32 datakind=0 needvalue=����
#define	QUEST_DATATYPE_EQUIP_UP				33	// ��N�佫��Nװ�� datatype=33 datakind=�佫kind dataoffset=װ�����
#define	QUEST_DATATYPE_BUILDING_UPGRADE		34	// ����-���� datatype=34 datakind=����kind dataoffset=��Դ������� needvalue=�����ȼ�
#define	QUEST_DATATYPE_TRAIN_OP				35	// ����-ļ��N����N���� datatype=24 datakind=���֣�1��2��3�� needvalue=����
#define	QUEST_DATATYPE_TRAINCOUNT_OP		36	// ����-ļ��N�� datatype=25 datakind=0 needvalue=����
#define	QUEST_DATATYPE_BRANCH_QUEST			37	// �������N��֧������ datatype=37 datakind=0 needvalue=����
#define	QUEST_DATATYPE_EQUIP_FORGING_QUICK	38	// ����һ�������̼��ٴ��� datatype=38 datakind=0 needvalue=1
#define	QUEST_DATATYPE_HERO_UP				39	// ǰ�����͹���N���� datatype=39 datakind=�佫kind needvalue=1
#define	QUEST_DATATYPE_SMITHYOFFHIRE		40	// �������ڹ�ӶN������߼����� datatype=40 datakind=0 needvalue=�ȼ�
#define	QUEST_DATATYPE_NEQUIP_FORGING		41	// �������N datatype=41 datakind=����kind needvalue=1
#define	QUEST_DATATYPE_NEQUIP_LEVELNUM		42	// ��������N�� datatype=42 datakind=0 needvalue=����
#define	QUEST_DATATYPE_MATERIAL_MAKE		43	// ����������������N�Σ������� datatype=43 datakind=0 needvalue=����
#define	QUEST_DATATYPE_HEROGUARD_UP			44	// ���������������佫N�� datatype=44 datakind=0 needvalue=����
#define	QUEST_DATATYPE_HERO_VISIT			45	// ��������Ѱ��N�� datatype=45 datakind=0 needvalue=����
#define	QUEST_DATATYPE_HERO_CALL_STORY		46	// ������ļ�佫 datatype=46 datakind=����id needvalue=1
#define	QUEST_DATATYPE_EQUIP_FORGING_OP		47	// ����-����Nװ��N���� datatype=21 datakind=װ��kind needvalue=����

// ��������
enum 
{
	QUEST_TYPE_MAIN = 0,	// ��������
	QUEST_TYPE_BRANCH,		// ֧������
	QUEST_TYPE_MAXNUM
};
QuestInfo *quest_config( int questid );
// ��һ������
int quest_newplayer( int actor_index );

// ����һ����������
int quest_give_main( int actor_index, int questid );

// ����һ��֧������
int quest_give_branch( int actor_index, int questid );

// ���������ֵ��������������Ƿ����
int quest_addvalue( City *pCity, int datatype, int datakind, int dataoffset, int value );
int quest_setvalue( City *pCity, int datatype, int datakind, int dataoffset, int value );

// ���������ֵ����������������Ƿ����(��������ʹ�õ�)
int quest_main_addvalue( City *pCity, int datatype, int datakind, int dataoffset, int value );

// �ͻ��˷������Ĳ����������
int quest_client_op( int actor_index, int questid, int datakind );

// ����������������Ƿ����
int quest_checkcomplete( int actor_index );

// ������ֵ��鷵���Ƿ����
int quest_check( int actor_index, int questid, int *value );

// ��ȡ������
int quest_getaward( int actor_index, int questid );

// ���������б�
int quest_sendlist( int actor_index );

// ����������
int quest_sendawardinfo( int actor_index, int questid, char type );

// ����˵��
int quest_talk( int actor_index, int talkid );

// �ͻ��˵���󣬻�ȡ��һ���Ի�
int quest_talk_next( int actor_index, int talkid );

// ����˵�����
int quest_talk_check( int actor_index );

// �ͻ�������һ���Ի�
int quest_talk_client_ask( int actor_index, int talkid );

// ս��
int quest_fight( int actor_index, int storyid );

// GM������������
int quest_gm_reset( int actor_index );

// GM��������
int quest_gm_set( int actor_index, int questid );

// city���ݼ�¼
#define DATA_RECORD_MAXNUM						16
#define DATA_RECORD_KILLENEMY					0	// ��������
#define DATA_RECORD_KILLCITY					1	// ��ս����
#define DATA_RECORD_KILLTOWN					2	// ��ս����
#define DATA_RECORD_KILLTOWNTYPE1				3	// �ɹ������سǴ���
#define DATA_RECORD_KILLTOWNTYPE2				4	// �ɹ����򿤳Ǵ���
#define DATA_RECORD_KILLTOWNTYPE3				5	// �ɹ�������������
#define DATA_RECORD_KILLTOWNTYPE4				6	// �ɹ������ش���
int data_record_addvalue( struct _city *pCity, int offset, int value );
int data_record_setvalue( struct _city *pCity, int offset, int value );
int data_record_getvalue( struct _city *pCity, int offset );

// ��߸�����
int tiance_quest_sendinfo( int actor_index );
//int tiance_quest_get( int actor_index, short kind );
//int tiance_quest_addnum( City *pCity );
//int tiance_quest_complete( int actor_index, short kind );
int tiance_quest_tech_activate( int actor_index, short kind );

// ÿ������
int everyday_quest_addvalue( City *pCity, int id, int value );
int everyday_quest_getvalue( City *pCity, int id );
int everyday_quest_reset( int actor_index );
int everyday_quest_sendlist( int actor_index ); 
int everyday_quest_getaward( int actor_index, int id );
int everyday_shop( int actor_index );
int everyday_shop_buy( int actor_index, int id, int awardkind );
#endif
