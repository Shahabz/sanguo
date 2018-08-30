#ifndef _AWARD_H_
#define _AWARD_H_

// -1~ ������ı��
#define AWARDKIND_ITEMBASE		0	  // ���ߣ�0+���߱�ţ�
#define AWARDKIND_EQUIPBASE		10000 // װ����10000+װ����ţ�
#define AWARDKIND_HEROBASE		20000 // Ӣ�ۣ�20000+Ӣ�۱�ţ�
#define AWARDKIND_BUILDINGBASE	30000 // ������30000+������ţ�
#define AWARDKIND_FUNCTION		40000 // ���ܣ�40000+���ܱ�ţ�
#define AWARDKIND_VALUEBASE		50000 // ��ֵ
#define AWARDKIND_SILVER		50001 // ����
#define AWARDKIND_WOOD			50002 // ľ��
#define AWARDKIND_FOOD			50003 // ��ʳ
#define AWARDKIND_IRON			50004 // ����
#define AWARDKIND_TOKEN			50005 // Ԫ��
#define AWARDKIND_BODY			50006 // ����
#define AWARDKIND_INFANTRY		50007 // ����
#define AWARDKIND_CAVALRY		50008 // ���
#define AWARDKIND_ARCHER		50009 // ����
#define AWARDKIND_EXP			50010 // ��ɫ����
#define AWARDKIND_VIPEXP		50011 // VIP����
#define AWARDKIND_AUTOBUILD		50012 // �Զ��������
#define AWARDKIND_LEVYNUM		50013 // ���մ���
#define AWARDKIND_PEOPLE		50014 // �˿�
#define AWARDKIND_PRESTIGE		50015 // ����ֵ
#define AWARDKIND_FRIENDSHIP	50016 // �������
#define AWARDKIND_TRAIN_QUICK	50017 // ļ������
#define AWARDKIND_BUILD_QUICK	50018 // �������
#define AWARDKIND_HERO_WASH		50019 // ���ϴ�����
#define AWARDKIND_EQUIP_WASH	50020 // ���ϴ������
#define AWARDKIND_LOSTREBUILD	50021 // �߼��ؽ�����
#define AWARDKIND_CITYGUARDNUM	50022 // �Ƿ��������
#define AWARDKIND_QUESTFIGHT	50023 // ����ս��
#define AWARDKIND_BUFF_TRAIN	50025 // ����ټ���N%��ʱ��1��
#define AWARDKIND_BUFF_MARCH	50026 // �о���ʱ����N%��ʱ��1��
#define AWARDKIND_LEVY_SILVER	50027 // ����N������������
#define AWARDKIND_LEVY_WOOD		50028 // ����N��ľ��������
#define AWARDKIND_LEVY_FOOD		50029 // ����N����ʳ������
#define AWARDKIND_LEVY_IRON		50030 // ����N������������
#define AWARDKIND_TIANCE_POINT		50031 // ��߸�����
#define	AWARDKIND_TIANCE_POINT_1	50032 // ��߸�����κ��
#define	AWARDKIND_TIANCE_POINT_2	50033 // ��߸��������
#define	AWARDKIND_TIANCE_POINT_3	50034 // ��߸��������
#define	AWARDKIND_TEACHERAWARDNUM	50035 // ʦͽ��������
#define	AWARDKIND_ACTIVITY05_XW		50036 // �¹�����
#define	AWARDKIND_ACTIVITY04_VALUE_ADD	50037 // ���ջ��ֵ
#define	AWARDKIND_ACTIVITY04_VALUE_SET	50038 // ���ջ��ֵ
#define	AWARDKIND_FORBIDTALK	50039 // ����
#define	AWARDKIND_BUILDINGEXT	50040 // �ڶ��������
#define	AWARDKIND_TOKEN_SALE	50041 // Ԫ���ֿ۵���
#define	AWARDKIND_TOKEN_RET		50042 // Ԫ����������
#define	AWARDKIND_EVERYDAYPOINT	50043 // ��Ծ��
#define AWARDKIND_PERMISSION_1	50051 // ���򲽱�Ȩ�ޣ�ֻ������ʾ��
#define AWARDKIND_PERMISSION_2	50052 // �������Ȩ�ޣ�ֻ������ʾ��
#define AWARDKIND_PERMISSION_3	50053 // ���򹭱�Ȩ�ޣ�ֻ������ʾ��
#define AWARDKIND_PERMISSION_4	50054 // �Ƽ�����
#define AWARDKIND_PERMISSION_5	50055 // װ������ͼֽ
#define AWARDKIND_PERMISSION_6	50056 // ����Ԥ��

#define AWARDKIND_SHAPE_0		51000 // ��1
#define AWARDKIND_SHAPE_1		51001 // ��2
#define AWARDKIND_SHAPE_2		51002 // ��3
#define AWARDKIND_SHAPE_20		51020 // Ů1
#define AWARDKIND_SHAPE_21		51021 // Ů2
#define AWARDKIND_SHAPE_22		51022 // Ů3
#define AWARDKIND_SHAPE_END		51100 // ͷ�����

#define AWARDKIND_RES_SILVER	60001 // ����δ������Դ�㣨��ӣ�
#define AWARDKIND_RES_WOOD		60002 // ����δ������Դ�㣨ľ�ĳ���
#define AWARDKIND_RES_FOOD		60003 // ����δ������Դ�㣨ũ�
#define AWARDKIND_RES_IRON		60004 // ����δ������Դ�㣨�󳧣�

#define	AWARDKIND_GIRLBASE		70000 // Ů��(70000+Ů�����)
#define	AWARDKIND_GIRLSOULBASE	71000 // Ů����Ƭ(71000+Ů�����)

// ��������µĸ�������
#define AWARDKIND_OTHERBASE	100000
#define AWARDKIND_QUEST_SUCCESS	AWARDKIND_OTHERBASE+1	// �������

#define AWARDGETINFO_MAXCOUNT	24

// ������(�����Ϊ�����)
typedef struct _award_group
{
	int *kind;			// ����
	int *minnum;		// ��������
	int *maxnum;		// ��������
	int *value;			// ����ֵ
	char *color;		// ��ɫ
	char *show;			// ��ʾ��ʾ
	short allcount;		// ���齱����Ʒ����
	int allvalue;		// ���齱��������ֵ
	short *type;        // ����
	int *appendnum;     // ����ֵ
}AwardGroup;

// ��ȡ���صĽ���
typedef struct _award_get_info
{
	int kind[AWARDGETINFO_MAXCOUNT];
	int num[AWARDGETINFO_MAXCOUNT];
	char color[AWARDGETINFO_MAXCOUNT];
	short count;
}AwardGetInfo;

// ��ʼ��
int awardgroup_init();

// �ض�
int awardgroup_reload();

// ���͸��ͻ�����ʾ�Ľ�����
int awardgroup_sendinfo( int actor_index, int awardgroup, int callback_code, int value, int limitcount );

// ��������飬��ֱ�Ӹ�
int awardgroup_random( int awardgroup, int level, AwardGetInfo *getinfo );

// �����ṹ���л��ַ���
int awardgroup_makestr( AwardGetInfo *awardInfo, char *awardContent );

// ������ ���ŵ��ʼ���������
int awardgroup_mail( int awardgroup, int level, char * itemContent );

// ��������飬ͨ��������Ĭ���������
int awardgroup_withindex( int actor_index, int awardgroup, int level, char path, AwardGetInfo *getinfo );

// ��������飬ͨ�����id����ȷ���Ƿ�����
int awardgroup_withid( int actorid, int awardgroup, char path, AwardGetInfo *getinfo );

// ������
int award_getaward( int actor_index, int kind, int num, char color, char path, AwardGetInfo *getinfo );

// ���߽�������
int gift( int actorid, int kind1, int num1, char color1, int kind2, int num2, char color2, int kind3, int num3, char color3, int kind4, int num4, char color4, unsigned char path );
int gift_check( int actor_index );

// ���߽������֣�����uid��
int gift_uid_check( int actor_index );

// ���߻�ȡ�������uid��
int gift_uid_get( int actor_index );

// �����
int awardgroup_randnum( int awardgroup, int level, int index );

// Ȩ�����
int weight_random( int *kind, int *num, int *weight, int count, int *totalweight, int *outkind, int *outnum );
#endif
