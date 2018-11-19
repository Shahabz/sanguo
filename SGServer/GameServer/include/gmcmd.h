
#ifndef __GMCMD_H
#define __GMCMD_H

#define GMC_BASE				0
#define GMC_TEST				(GMC_BASE+1)	// ����
#define GMC_SC					(GMC_BASE+2)	// �ű�ʹ��
#define GMC_LUA					(GMC_BASE+3)	// ����lua
#define GMC_RELOAD				(GMC_BASE+4)	// �ض����ݱ�
#define GMC_RESET				(GMC_BASE+5)	// ���õȼ�����
#define GMC_EXP					(GMC_BASE+6)	// ����
#define GMC_TOKEN				(GMC_BASE+7)	// ��ʯ
#define GMC_ITEM				(GMC_BASE+8)	// ����
#define GMC_EQUIP				(GMC_BASE+9)	// װ��
#define GMC_HERO				(GMC_BASE+10)	// Ӣ��
#define GMC_SILVER				(GMC_BASE+11)	// ����
#define GMC_WOOD				(GMC_BASE+12)	// ľ��
#define GMC_FOOD				(GMC_BASE+13)	// ��ʳ
#define GMC_IRON				(GMC_BASE+14)	// ����
#define GMC_BODY				(GMC_BASE+15)	// ����
#define GMC_LEVY				(GMC_BASE+16)	// �ո����
#define GMC_AWARDGROUP			(GMC_BASE+17)	// ������
#define GMC_QUEST				(GMC_BASE+18)	// ����
#define GMC_ROLLMSG				(GMC_BASE+19)	// �����
#define GMC_MAIL				(GMC_BASE+20)	// �ʼ�
#define GMC_SYSTALK				(GMC_BASE+21)	// ϵͳ����
#define GMC_VIPEXP				(GMC_BASE+22)	// vip����
#define GMC_ACTIVITY			(GMC_BASE+23)	// �
#define GMC_TECH				(GMC_BASE+24)	// �Ƽ�
#define GMC_BUILDING			(GMC_BASE+25)	// ����
#define GMC_PEOPLE				(GMC_BASE+26)	// �˿�
#define GMC_MAT					(GMC_BASE+27)	// ��������
#define GMC_FUN					(GMC_BASE+28)	// ��ȡ����
#define GMC_EVENT				(GMC_BASE+29)	// ��ͼ�¼�ˢ��
#define GMC_VW					(GMC_BASE+30)	// ����
#define GMC_TOWNATTACK			(GMC_BASE+31)	// ���ǹ���
#define GMC_WORLDBOSS			(GMC_BASE+32)	// ����boss
#define GMC_KWPOINT				(GMC_BASE+33)	// Ѫս����
#define GMC_GIVERES				(GMC_BASE+34)	// ����һ��δ������Դ��
#define GMC_AUTOBUILD			(GMC_BASE+35)	// �Զ��������
#define GMC_PAYBAG				(GMC_BASE+36)	// ���
#define GMC_BUILDINGLEVEL		(GMC_BASE+37)	// �����ȼ�
#define GMC_STORY				(GMC_BASE+38)	// ��ͨ����
#define GMC_PRESTIGE			(GMC_BASE+39)	// ����
#define GMC_NATIONMISSION		(GMC_BASE+40)	// ������������
#define GMC_NATIONQUEST			(GMC_BASE+41)	// ��������
#define GMC_NATIONRANK			(GMC_BASE+42)	// �����������а�
#define GMC_NATIONEXP			(GMC_BASE+43)	// ���Ҿ���
#define GMC_NATIONOF			(GMC_BASE+44)	// ���ҹ�Աϵͳ
#define GMC_NHERO				(GMC_BASE+45)	// ��������
#define GMC_DB					(GMC_BASE+46)	// ���ݿ�����
#define GMC_PAY					(GMC_BASE+47)	// ֧�� 
#define GMC_BUFFCLEAR			(GMC_BASE+48)	// buff��� 
#define GMC_AWARD				(GMC_BASE+49)	// ���� 
#define GMC_CITYINFO			(GMC_BASE+50)	// �ǳ�����
#define GMC_MAIL_ACTOR			(GMC_BASE+51)	// ����ʼ�
#define GMC_MAIL_SERVER			(GMC_BASE+52)	// ȫ���ʼ�
#define GMC_MAIL_NOTICE			(GMC_BASE+53)	// �����ʼ�
#define GMC_ACTIVITY_END		(GMC_BASE+54)	// ǿ�ƽ����
#define GMC_ACTIVITY_CLOSE		(GMC_BASE+55)	// ǿ�ƹرջ
#define GMC_KICK				(GMC_BASE+56)	// ������
#define GMC_LOCKACTOR			(GMC_BASE+57)	// ����ɫ
#define GMC_LOCKUSER			(GMC_BASE+58)	// ���˺�
#define GMC_UNLOCKACTOR			(GMC_BASE+59)	// ������ɫ
#define GMC_UNLOCKUSER			(GMC_BASE+60)	// �����˻�
#define GMC_GIRL				(GMC_BASE+61)	// Ů��
#define GMC_GIRLSOUL			(GMC_BASE+62)	// Ů����Ƭ
#define GMC_SONQUICK			(GMC_BASE+63)	// ��ʦ����
#define GMC_ROBOTADD			(GMC_BASE+64)	// �����˴���
#define GMC_ROBOTDEL			(GMC_BASE+65)	// ������ɾ��
#define GMC_ROBOTLOGIC			(GMC_BASE+66)	// �������߼�
#define GMC_ROBOTQUEUE			(GMC_BASE+67)	// �������߼�-���д���
#define GMC_PAYMODE				(GMC_BASE+68)	// ֧��ģʽ
#define GMC_FANGSHI				(GMC_BASE+69)	// ����
#define GMC_A12					(GMC_BASE+70)	// ��������
#define GMC_RECHARGE			(GMC_BASE+71)	// ��ֵ������ 
#define GMC_ARMYDELETE			(GMC_BASE+72)	// ɾ��������в��� 
#define GMC_HEROSTATE			(GMC_BASE+73)	// �佫״̬

// GMָ��ʹ����Ϣ
#define GMR_OK					1		// �ɹ�
#define GMR_NOTGMTOOLS			0		// δʹ��/������Gmtools
#define GMR_CHECK				-1		// ��ʹ��/��ʱ
#define GMR_ERROR				-10
#define GMR_ACCESS_DENIED		-11
#define GMR_NOT_ONLINE			-12
#define GMR_ARG_ERROR			-13

struct _cmdinfo {
	char m_allowgmtools;			// �Ƿ�����GMTools����
	char m_online;					// �Ƿ�������
};
typedef struct _cmdinfo SCmdInfo;

int gm_init();
char gm_isallow_gmtools( short cmdid );
char gm_check_online( short cmdid );

int gm_proc( short cmd, int arg1, int arg2, int arg3, int arg4, char *str );
int gm_set_result( int client_index, int value );
int gm_get_result( int client_index );
#endif
