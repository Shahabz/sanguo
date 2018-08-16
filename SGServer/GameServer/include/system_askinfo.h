#ifndef __SYSTEM_ASKINFO_H
#define __SYSTEM_ASKINFO_H
// �ͻ��˷������ļ�����

#define ASKINFO_NORMAL			0
#define ASKINFO_CHANGESHAPE		1
#define ASKINFO_CHANGENAME		2 // �޸�����
#define ASKINFO_CITYGUARD		3 // ��ǽ����
#define ASKINFO_QUEST			4 // ����
#define ASKINFO_BUILDING		5 // ����
#define ASKINFO_LEVY			6 // ����
#define ASKINFO_TRAIN			7 // ѵ��
#define ASKINFO_EQUIPFORGING    8  // װ������
#define ASKINFO_EQUIPWASH       9  // װ��ϴ��
#define ASKINFO_EQUIP			10 // װ��
#define ASKINFO_ITEM			11 // ����
#define ASKINFO_HERO			12 // Ӣ��
#define ASKINFO_TECH			13 // �Ƽ�
#define ASKINFO_GOV				14 // �ٸ� 
#define ASKINFO_OFFICIALHIRE	15 // ��Ӷ 
#define ASKINFO_QUICK			16 // ����
#define ASKINFO_ACTOR			17 // ��ɫ����
#define ASKINFO_MATERIALMAKE	18 // ��������
#define ASKINFO_STORY			19 // ����
#define ASKINFO_WORLDMAP		20 // �����ͼ����
#define ASKINFO_MAP_EVENT		21 // �����ͼ����¼�
#define ASKINFO_SPY_CITY		22 // ���ǳ�
#define ASKINFO_CITYHELP		23 // פ����Ϣ
#define ASKINFO_CITYARMYGROUP	24 // ��ս
#define ASKINFO_NATIONARMYGROUP	25 // ��ս
#define ASKINFO_MAPTOWN			26 // ����
#define ASKINFO_MAPZONE			27 // ����
#define ASKINFO_LOSTREBUILD		28 // ��԰�ؽ�
#define ASKINFO_KINGWAR			29 // �ʳ�Ѫս
#define ASKINFO_MSGBOX_CALLBACK 30 // ��Ϣ����ȷ���ص�
#define ASKINFO_AUTOBUILD		31 // �Զ�����
#define ASKINFO_HERO_VISIT		32 // �佫Ѱ��
#define ASKINFO_CHANGESIGN		33 // ����ǩ��
#define ASKINFO_SHOP			34 // �̵�
#define ASKINFO_PAY				35 // ֧��
#define ASKINFO_VIPSHOP			36 // VIP�̵�
#define ASKINFO_VIPBAG			37 // VIP���
#define ASKINFO_ACTIVITY		38 // �
#define ASKINFO_NATIONEQUIP		39 // ����
#define ASKINFO_NATION			40 // ����
#define ASKINFO_NATIONLOG		41 // ������־
#define ASKINFO_NATIONHERO		42 // ��������
#define ASKINFO_WISHING			43 // �۱���
#define ASKINFO_TIANCEQUEST		44 // ��߸�����
#define ASKINFO_TEACHER			45 // ʦͽ
#define ASKINFO_GUAID			46 // �������
#define	ASKINFO_CDKEY			47 // CDKEY
#define	ASKINFO_GIRL			48 // Ů��
#define	ASKINFO_FANGSHI			49 // ����
#define	ASKINFO_GIRLSHOP		50 // Ů���̵�
#define	ASKINFO_FIGHTHELPER		51 // ��Ӷ��
#define ASKINFO_INVITECODE		52 // ������
#define ASKINFO_BUYRES			53 // ������Դ

int system_askinfo( int actor_index, int msgid, char *pstr, int *pvalue );

#endif
