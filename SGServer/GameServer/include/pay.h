#ifndef __PAY_H_
#define __PAY_H_
#include "define.h"

#define PAY_GOODSTYPE_BASE					0		// �����̵�����
#define PAY_GOODSTYPE_PUSH					1		// ��������
#define PAY_GOODSTYPE_DEFAULT				2		// �ֲ�ͼ���
#define PAY_GOODSTYPE_ACTIVITY				3		// �����

#define PAY_GOODSTYPE_MONTHCARD             101     // �¿����
#define PAY_GOODSTYPE_WEEKCARD              102     // �ܿ����

#define PAY_ACTIVITY_MAXCOUNT			10
#define PAY_INT_MONTH_DAY				30         // һ����30��
#define PAY_INT_WEEK_DAY				7          // һ��7��

#define PAY_MONTH_AWARDGROUP				160         // �¿�����
#define PAY_WEEK_AWARDGROUP					159          // �ܿ�����

// �������ֶ���
// ע�⣺�޸�����ͬʱ��Դ�ļ�����һ���ַ������壬��һ���޸�
enum PayCoin
{
	USD = 0,//	��Ԫ
	CNY,	//	�����Ԫ
	PAYCOINMAX,
};

// ���֧���۸�
typedef struct _payprice
{
	int price[PAYCOINMAX];
}PayPrice;


typedef struct _paybag
{
	short goodsid;
	int begintime;
	int endtime;
	int count;
	char pushcity;
}PayBag;

int payprice_init();

// �����������͵����
int paybag_load();
int paybag_insert( short goodsid, int begintime, int endtime, int count );
int paybag_delete( short goodsid );
void paybag_logic();

int paystore_list( int actor_index );

// ͨ��type��ȡID�б�
int paystore_goodslist_withtype( int type, int *goodsid, int count );

// ������Ʒ
int paystore_buy( int actor_index, int goodsid );

// ��Ӷ���
int paystore_addorder( int actor_index, int goodsid, char *pOrderID );

// ������߶���
int paystore_payoffline( int actor_index, int goodsid, char *pOrderID );
int paystore_payoffline_get( int actor_index );

// �����¿�
void paystore_monthcard_buy( int actorid );

// ��ȡ�¿�
void paystore_monthcard_buy_get( int actor_index );

// �����Ϣ
int pay_goodsinfo( int actor_index, int type, int goodsid );

// �޹���Ӵ���
int actor_limitybuy_addnum( int actor_index, int index );
int actor_limitybuy_getnum( int actor_index, int index );
int actor_limitybuy_addnum_db( int actorid, int index );
int actor_limitybuy_getnum_db( int actorid, int index );

// ÿ���޹���Ӵ���
int actor_everyday_limitybuy_addnum( int actor_index, int index );
int actor_everyday_limitybuy_getnum( int actor_index, int index );
int actor_everyday_limitybuy_addnum_db( int actorid, int index );
int actor_everyday_limitybuy_getnum_db( int actorid, int index );

// ��ȡ�����Ʒ���̵����ID
int paystore_getid( int goodsid );

// ֧���ɹ�
int actor_pay( int actorid, int goodsid, char *pOrderID, char *money, char *currency );

// ���������Ϣ
int actor_pushbaginfo( int actor_index );

// �������
int city_paybag_add( struct _city *pCity, short goodsid, int cd, int limitcount, char path );
int city_paybag_sub( struct _city *pCity, short goodsid, short goodstype );
int city_paybag_del( struct _city *pCity, short goodsid );
int city_paybag_del_withtype( struct _city *pCity, int type );
char city_paybag_has( struct _city *pCity, short goodsid );

// ��������
int activity_paybag_open( int activityid );
int activity_paybag_close( int activityid );
int activity_paybag_citynew( struct _city *pCity );
int activity_paybag_list( int actor_index, int path );

// ��ȡ���������ڹ���Ӧ�õ����
int paygoods_getawardgroup( int actor_index, short goodsid );

// �¿����ܿ�ÿ��ķ���
int paycard_give();
#endif


