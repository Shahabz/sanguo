#ifndef __ACTOR_NOTIFY_H
#define __ACTOR_NOTIFY_H
#include "server_structsend_auto.h"

#define POP( actor_index, msgid ) actor_notify_pop( actor_index, msgid )
#define SUBSCRIBE_CMD_KINGWARDLG	1

#define NOTIFY_NORMAL		0	
#define	NOTIFY_TEXTTABLE	1	
#define	NOTIFY_ITEM			2
#define	NOTIFY_ITEMNUM		3
#define	NOTIFY_LOGIN_QUEUE	4
#define	NOTIFY_CHANGESHAPE	5	// ���ͷ���޸�
#define NOTIFY_CHANGENAME	6	// ����޸�����
#define NOTIFY_NPCTALK		7	// NPC�Ի�
#define NOTIFY_EQUIP		8	// װ��
#define NOTIFY_ACTOR		9	// ��ɫ
#define NOTIFY_ACTORSFLAG	10	// ��ɫ��־λ
#define NOTIFY_WORLDMAP		11	// �����ͼ
#define NOTIFY_VALUECHANGE	12	// ĳЩֵ�ı��ͨ��֪ͨ
#define NOTIFY_MAIL			13	// �ʼ�
#define NOTIFY_ALERT		14	// ����ȷ����Ϣ
#define NOTIFY_LOSTREBUILD	15	// �߼��ؽ�����
#define NOTIFY_MAPZONEGOZC	16	// ǰ���ݳǵ���ʾ������
#define NOTIFY_MSGBOX		17	// ������Ϣѡ���
#define NOTIFY_BOX			18	// ������Ϣ��
#define NOTIFY_POP			19	// ������Ϣ���޿�
#define NOTIFY_BUILDINGFINISH	20	// �������
#define NOTIFY_CITYGUARDNUM		21	// �Ƿ�������
#define NOTIFY_HERO_VISIT		22	// �佫Ѱ��
#define NOTIFY_MAPCALL			23	// ��ͼ�ٻ�
#define NOTIFY_HEROBACK			24	// �佫�س�
#define NOTIFY_HEROGOD			25	// ��ͻ�ɹ�
#define NOTIFY_ACTIVITY			26	// �
#define NOTIFY_GUIDE			27	// ָ��
#define NOTIFY_INVITECODE		28	// ������
#define NOTIFY_DELAYQUEUEPLAY	29	// �ͻ����ӳٶ��в���

// ���͸���ɫ����Ϣ �޲���
int actor_notify( int actor_index, short msgid, const char *msg );

// ���͸���ɫ����Ϣ ������
int actor_notify_value( int actor_index, short msgid, char count, const int *data, const char *msg );

// ����Ϣ��ʾ��ͨ����ϢID
int actor_notify_pop( int actor_index, int textid );
int actor_notify_pop_v( int actor_index, int textid, char *v1, char *v2 );

// npc˵��
int npc_talk( int actor_index, int textid, int btntextid );

// �佫˵��
int hero_talk( int actor_index, int herokind, int textid );

// ������Ϣ
int actor_notify_box( int actor_index, int textid );
int actor_notify_box_v( int actor_index, int textid, char *v1, char *v2 );

// ����ȷ����Ϣ
int actor_notify_alert( int actor_index, int textid );

// ����ȷ����Ϣ������
int actor_notify_alert_v( int actor_index, int textid, char *v1, char *v2 );

// ��Ϣ��ʾ��
#define MSGBOX_CALLBACK_KINGWAR_DEFENSE	1 // Ѫս��ʾǰ��ת�ط�
#define MSGBOX_CALLBACK_KINGWAR_SNEAK	2 // Ѫ��ս��ʾ����ת͵Ϯ
// ������Ϣѡ���
int actor_notify_msgbox_v( int actor_index, int msgid, int value1, int value2, int textid, char *v1, char *v2 );
int actor_notify_msgbox_callback( int actor_index, int msgid, int value1, int value2 );

// ֪ͨUI����
#define UI_UPDATE_NATIONFIGHT		1 // ��ս�����б仯
#define UI_UPDATE_FIGHTINFO			2 // ս����Ϣ�仯
#define UI_UPDATE_NATIONOFFICIAL	3 // ��Ա״̬�仯
#define UI_UPDATE_NATIONHEROPOS		4 // ����λ�ñ仯
void ui_update( int actor_index, char send_type, char ui );
#endif
