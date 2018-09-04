
#ifndef __ACTIVITY_H
#define __ACTIVITY_H

#define MAX_ACTIVITY_COUNT					48

#define ACTIVITY_NORMAL						0 
#define ACTIVITY_1	1	//	�׳����
#define ACTIVITY_2	2	//	���ǵȼ�
#define ACTIVITY_3	3	//	�����Ե�
#define ACTIVITY_4	4	//	���տ�
#define ACTIVITY_5	5	//	���ǰݽ�
#define ACTIVITY_6	6	//	ȫ������
#define ACTIVITY_7	7	//	�󿧴���
#define ACTIVITY_8	8	//	�ɳ��ƻ�
#define ACTIVITY_9	9	//	��������
#define ACTIVITY_10	10	//	�ؼ����
#define ACTIVITY_11	11	//	��ʦ����
#define ACTIVITY_12	12	//	��������
#define ACTIVITY_13	13	//	����ͼֽ
#define ACTIVITY_14	14	//	������Դ
#define ACTIVITY_15	15	//	���鷭��
#define ACTIVITY_16	16	//	��������
#define ACTIVITY_17	17	//	��ֵ����
#define ACTIVITY_18	18	//	����ת��
#define ACTIVITY_19	19	//	�ɼ�����
#define ACTIVITY_20	20	//	����
#define ACTIVITY_21	21	//	��������
#define ACTIVITY_22	22	//	�ʳǲɼ�
#define ACTIVITY_23	23	//	��Ա�ٻ�
#define ACTIVITY_24	24	//	ļ������
#define ACTIVITY_25	25	//	�������
#define ACTIVITY_26	26	//	ǿ����
#define ACTIVITY_27	27	//	��������
#define ACTIVITY_28	28	//	�ƽ������
#define ACTIVITY_29	29	//	��������
#define ACTIVITY_30	30	//	ͼֽ�
#define ACTIVITY_31	31	//	ȫ��Ѱ��
#define ACTIVITY_32	32	//	������
#define ACTIVITY_33	33	//	��ֵ����
#define ACTIVITY_34	34	//	��������
#define ACTIVITY_35	35	//	��ľ����
#define ACTIVITY_36	36	//	��������
#define ACTIVITY_37	37	//	��������
#define ACTIVITY_38	38	//	��ս����
#define ACTIVITY_39	39	//	��ս����
#define ACTIVITY_40	40	//	ϴ������
#define ACTIVITY_41	41	//	��������
#define ACTIVITY_42	42	//	��������
#define ACTIVITY_43	43	//	��ֵ����
#define ACTIVITY_44	44	//	��������

typedef struct _activity_item
{
	int m_activityid;
	int m_warningtime;	// �Ԥ��ʱ��
	int m_starttime;	// �����ʱ��
	int m_endtime;		// �����ʱ��
	int m_closetime;	// ��ر�ʱ�䣬�ö�Ϊ����ҿ���������ʲô�ģ������������ʱ��
	char m_openstat;	// �Ƿ���ù���������
	char m_endstat;		// �Ƿ���ù���������
}ActivityItem;

int activity_init();
int activity_set( int activityid, int warningtime, int starttime, int endtime, int closetime );
int activity_setendtime( int activityid, int endtime, int closetime );
int activity_force_end( int activityid );
int activity_force_close( int activityid );
int activity_setopenstat( int activityid, int stat );
int activity_setendstat( int activityid, int stat );
int activity_intime( int activityid );
int activity_inclose( int activityid );
int activity_lefttime( int activityid );
int activity_totaltime( int activityid );
int activity_logic();

int activity_onwarning( int activityid, int lefttime );
int activity_onopen( int activityid );
int activity_onend( int activityid );
int activity_onclose( int activityid );
int activity_onlogic( int activityid );

// ��б�
int activity_sendlist( int actor_index );
char activity_checkred( int actor_index );
// �׳�����
int activity_01_sendinfo( int actor_index );
int activity_01_get( int actor_index );

// ���ǵȼ��
int activity_02_sendinfo( int actor_index );
int activity_02_get( int actor_index, int id );

// �����Եػ
int activity_03_sendinfo( int actor_index );
int activity_03_get( int actor_index, int id );

// ���ǰݽ��
int activity_05_sendinfo( int actor_index );
int activity_05_buy( int actor_index, int index );
int activity_05_update( int actor_index );
int activity_05_callhero( int actor_index );

// ȫ�������
void activity_06_onopen();
void activity_06_onend();
void activity_06_onclose();
int activity_06_sendinfo( int actor_index );
int activity_06_get( int actor_index, int id );

// �ɳ��ƻ��
int activity_08_sendinfo( int actor_index );
int activity_08_open( int actor_index );
int activity_08_get( int actor_index, int id );

// �ؼ����
int activity_10_init();

// ��ʦ����
int activity_11_sendinfo( int actor_index );
int activity_11_get( int actor_index );

// ��ֵ����
void activity_17_onopen();
void activity_17_onend();
void activity_17_onclose();
int activity_17_sendinfo( int actor_index );
int activity_17_get( int actor_index, int id );

// �������
int activity_25_sendinfo( int actor_index );
int activity_25_get( int actor_index );

// ��������
void activity_27_onwarning( int lefttime );
void activity_27_onopen();
void activity_27_onend();
void activity_27_onclose();
void activity_27_onlogic();
int activity_27_brush();
int activity_27_brush_withzoneid( int zoneid );
int activity_27_sendinfo( int actor_index );

// ��ֵ�����
#define ACTIVITY33_MEMBERMAX	100
typedef struct _activity_33_rank
{
	int actorid;
	int city_index;
}Activity33Rank;
int activity_33_load();
void activity_33_onopen();
void activity_33_onend();
void activity_33_onclose();
void activity_33_calc_rank( City *pCity );
int activity_33_addvalue( int actor_index, int value );
int activity_33_sendinfo( int actor_index );
#endif
