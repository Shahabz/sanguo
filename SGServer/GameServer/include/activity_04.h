#ifndef _ACTIVITY_04_H
#define _ACTIVITY_04_H
#include "define.h"

#define ACTIVITY_SEVENDAY_TYPE1		1	//	�ۼƳ�ֵ{ 0 }Ԫ��v
#define ACTIVITY_SEVENDAY_TYPE2		2	//	����о��Ƽ�{ 0 } get
#define ACTIVITY_SEVENDAY_TYPE3		3	//	��Ӷ��ɫ�о�Աv
#define ACTIVITY_SEVENDAY_TYPE4		4	//	��Ӷ��ɫ������v
#define ACTIVITY_SEVENDAY_TYPE5		5	//	�����ﵽ{ 0 }�� get
#define ACTIVITY_SEVENDAY_TYPE6		6	//	����{ 0 }���ؼ����v
#define ACTIVITY_SEVENDAY_TYPE7		7	//	������ȡ��˳ get
#define ACTIVITY_SEVENDAY_TYPE8		8	//	���ڵ�ͼ�������ӵ��{ 0 }���س� get
#define ACTIVITY_SEVENDAY_TYPE9		9	//	���ڵ�ͼ�������ӵ��{ 0 }������ get
#define ACTIVITY_SEVENDAY_TYPE10	10	//	���빥��{ 0 }��Ӫ�� get
#define ACTIVITY_SEVENDAY_TYPE11	11	//	���빥��{ 0 }���س� get
#define ACTIVITY_SEVENDAY_TYPE12	12	//	���빥��{ 0 }������ get
#define ACTIVITY_SEVENDAY_TYPE13	13	//	��ɱ6 - 10������{0}��v
#define ACTIVITY_SEVENDAY_TYPE14	14	//	���߸���ͨ�ص�{ 0 }�� get
#define ACTIVITY_SEVENDAY_TYPE15	15	//	�ٸ��ȼ��ﵽ{ 0 }�� get
#define ACTIVITY_SEVENDAY_TYPE16	16	//	����5������ȼ�����ҳ�v
#define ACTIVITY_SEVENDAY_TYPE17	17	//	�������4������8������ҳ�v
#define ACTIVITY_SEVENDAY_TYPE18	18	//	�������3������10������ҳ�v
#define ACTIVITY_SEVENDAY_TYPE19	19	//	�������2������11������ҳ�v
#define ACTIVITY_SEVENDAY_TYPE20	20	//	�������1������12������ҳ�v
#define ACTIVITY_SEVENDAY_TYPE21	21	//	�ռ�����{ 0 }����ɫ������Ʒ��װ��v
#define ACTIVITY_SEVENDAY_TYPE22	22	//	�ռ�����{ 0 }����ɫ������Ʒ��װ��v
#define ACTIVITY_SEVENDAY_TYPE23	23	//	�ռ�{ 0 }����ɫ������Ʒ�ʱ���v
#define ACTIVITY_SEVENDAY_TYPE24	24	//	�ռ�{ 0 }����ɫ������Ʒ��ͷ��v
#define ACTIVITY_SEVENDAY_TYPE25	25	//	�ռ�{ 0 }����ɫ������Ʒ��ͷ��v
#define ACTIVITY_SEVENDAY_TYPE26	26	//	����ӵ��{ 0 }����ɫ�����Ͻ�v
#define ACTIVITY_SEVENDAY_TYPE27	27	//	����ӵ��{ 0 }����ɫ�����Ͻ�v
#define ACTIVITY_SEVENDAY_TYPE28	28	//	����ӵ��{ 0 }����ɫ�����Ͻ�v
#define ACTIVITY_SEVENDAY_TYPE29	29	//	����ӵ��{ 0 }����ɫ�����Ͻ�v
#define ACTIVITY_SEVENDAY_TYPE30	30	//	����{ 0 }���佫������ϴ�赽����v
#define ACTIVITY_SEVENDAY_TYPE31	31	//	����{ 0 }����ɫ��������ϴ�赽����v
#define ACTIVITY_SEVENDAY_TYPE32	32	//	{0}����ɫ������Ʒ��װ��ϴ��������v
#define ACTIVITY_SEVENDAY_TYPE33	33	//	{0}����ɫ������Ʒ��װ��ϴ��������v
#define ACTIVITY_SEVENDAY_TYPE34	34	//	���ٴ���{ 0 }������v
#define ACTIVITY_SEVENDAY_TYPE35	35	//	����{0}������20�� get
#define ACTIVITY_SEVENDAY_TYPE36	36	//	����{0}������30�� get
#define ACTIVITY_SEVENDAY_TYPE37	37	//	����{0}������40�� get
#define ACTIVITY_SEVENDAY_TYPE38	38	//	ս���ﵽ{ 0 } get
#define ACTIVITY_SEVENDAY_TYPE39	39	//	���յ������� get
#define ACTIVITY_SEVENDAY_TYPE_MAXNUM 40
#define ACTIVITY_SEVENDAY_TIME		(7*86400)
int activity_04_addvalue( int city_index, int type, int value );
int activity_04_setvalue( int city_index, int type, int value );
int activity_04_addvalue_cityfight( int city_index, int targetlevel );
int activity_04_addvalue_equip( int city_index, int equiptype, int color );
int activity_04_addvalue_equipwash( int actor_index );
int activity_04_addvalue_hero( int actor_index );
int activity_04_addvalue_herowash( int actor_index );
int activity_04_getvalue( int actor_index, int type, int id );
int activity_04_sendinfo( int actor_index );
int activity_04_get( int actor_index, int id );
#endif
