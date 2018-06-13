#ifndef __GAMELOG_H
#define __GAMELOG_H

#include "utils.h"

/* ���� */
#define LOGOP_TOKEN				1	// ��ʯ
#define LOGOP_HERO				2	// Ӣ��
#define LOGOP_ITEMGET			3	// ��õ���
#define LOGOP_ITEMLOST			4	// ��ʧ����
#define LOGOP_ACTOREXP			5	// ��ɫ����
#define LOGOP_UPGRADE			6	// ��ɫ����
#define LOGOP_BODY				7	// ����
#define LOGOP_VIPEXP			8	// vip����
#define LOGOP_BUILDING			9	// ����
#define LOGOP_EQUIPGET			10	// ���װ��
#define LOGOP_EQUIPLOST			11	// ��ʧװ��
#define LOGOP_QUEST				12	// ����
#define LOGOP_SILVER			13	// ����
#define LOGOP_WOOD				14	// ľ��
#define LOGOP_FOOD				15	// ��ʳ
#define LOGOP_IRON				16	// ����
#define LOGOP_TECH				17	// �Ƽ�
#define LOGOP_BARRACKS			18	// ��Ӫ����
#define LOGOP_FORGING			19	// ����
#define LOGOP_HEROUPGRADE		20	// Ӣ������
#define LOGOP_KWPOINT			21	// Ѫս����
#define LOGOP_GIRL				22	// Ů��
#define LOGOP_GIRLSOUL			23	// Ů����Ƭ
#define LOGOP_HEROEXP			24	// Ӣ�۾���

/* ;�� */
#define PATH_SYSTEM						1	// ϵͳ
#define PATH_GM							2	// GM
#define PATH_PAY						3	// ��ֵ
#define PATH_ITEMUSE					4	// ����ʹ��
#define PATH_TOKENITEMUSE				5	// ��ʯֱ��ʹ��
#define PATH_SELL						6	// ��������
#define PATH_BUILDING_UPGRADE			7	// ����
#define PATH_QUEST						8	// ����
#define PATH_GUARD_UPGRADE				9	// ��������
#define PATH_LEVY						10	// ����
#define PATH_TRAIN						11	// ѵ��
#define PATH_RESOLVE					12	// �ֽ�
#define PATH_EQUIPEXT					13	// ����װ������
#define PATH_TECH_UPGRADE				14	// �Ƽ�����
#define PATH_TECH_FINISH				15	// �Ƽ����
#define PATH_TECH_GET					16	// �Ƽ���ȡ
#define PATH_TECH_QUICK					17	// �Ƽ�����
#define PATH_TECH_FREEQUICK				18	// �Ƽ���Ѽ���
#define PATH_HIRE_FORGING				19	// ��Ӷ
#define PATH_HIRE_GOV					20	// ��Ӷ
#define PATH_HIRE_TECH					21	// ��Ӷ
#define PATH_BUILDING_WORKER			22	// ���ý������
#define PATH_TRAIN_QUEUE				23	// ��ļ����
#define PATH_TRAIN_LONG					24	// ѵ��ʱ��
#define PATH_TRAIN_GET					25	// ѵ����ȡ
#define PATH_TRAIN_CANCEL				26	// ȡ����ļ
#define PATH_EQUIP_FORGING				27	// װ������
#define PATH_FORGING_QUICK				28	// �������
#define PATH_FORGING_FREEQUICK			29	// ��������
#define PATH_BODYBUY					30	// ��������
#define PATH_CHANGENAME					31	// ����
#define PATH_GUARD_LIFE					32	// ������Ѫ
#define PATH_EQUIP_UP					33	// װ����
#define PATH_EQUIP_DOWN					34	// װ����
#define PATH_HERO_ADDEXP				35	// ��Ӣ�۾���
#define PATH_HERO_ADDSOLDIERS			36	// ����
#define PATH_HERO_SOLDIERS_EQUIP		37	// ��жװ�������ı�����ʧ
#define PATH_HERO_WASH					38	// Ӣ��ϴ��
#define PATH_EQUIP_WASH					39	// װ��ϴ��
#define PATH_MATERIALMAKE_QUEUE			40	// �����������
#define PATH_MATERIALMAKE				41	// ��������
#define PATH_STORY						42	// ����
#define PATH_GATHER						43	// �ɼ�
#define PATH_FIGHT						44	// ս��
#define PATH_MARCH						45	// �о�
#define PATH_MAPEVENT					46	// ��ͼ����¼�
#define PATH_MAIL_ATTACH				47	// ����
#define PATH_ENEMY						48	// ����
#define PATH_SPY						49	// ���
#define PATH_TOWNASK					50	// ����ݵ����
#define PATH_TOWNREPAIR					51	// �ݵ��޸�
#define PATH_TOWNLEVY					52	// �ݵ�����
#define PATH_TOWNDEV_CONTRIBUTE			53	// ���ǿ�������
#define PATH_WORLDBOSS					54	// ����boss
#define PATH_LOSTREBUILD				55	// �ؽ���԰
#define PATH_KINGWAR_SNEAK				56	// Ѫս�-͵Ϯ
#define PATH_KINGWAR_PK					57	// Ѫս�-����
#define PATH_KINGWAR_DEFENSE			58	// Ѫս�-�ط�
#define PATH_KINGWAR_REBIRTH			59	// Ѫս�-����
#define PATH_KINGWAR_CHANGE				60	// Ѫս���ֶһ�
#define PATH_KINGWAR_TREASURE			61	// Ѫս�ڱ�
#define PATH_KINGWAR_TREASURE_CO		62	// Ѫս����
#define PATH_HEROVISIT					63	// �佫Ѱ��
#define PATH_HEROVISIT_CHANGE			64	// �佫Ѱ��-Ӣ��ת����
#define PATH_HERO_UP					65	// ����
#define PATH_HERO_DOWN					66	// ����
#define PATH_HERO_GUARD					67	// ������
#define PATH_HERO_GUARD_AUTO			68	// �������Զ�
#define PATH_STORY_RES					69	// ������Դ����;��
#define PATH_STORY_HERO					70	// ����Ӣ�۸���;��
#define PATH_STORY_DRAWING				71	// ����װ������;��
#define PATH_STORY_SWEEP				72	// ����ɨ��;��
#define PATH_SHOP						73	// �̵�
#define PATH_VIPSHOP					74	// VIP�ؼ��̵�
#define PATH_VIPBAG						75	// VIP���
#define PATH_WOUNDED_SOLDIERS			76	// �˱��ָ�
#define PATH_NATIONEQUIP				77	// ����
#define PATH_NATIONUPGRADE				78	// ���ҽ���
#define PATH_NATIONPLACE				79	// ��λ����
#define PATH_NATIONQUEST				80	// ��������
#define PATH_NATIONMISSION				81	// ������������
#define PATH_NATIONRANK					82	// ����������
#define PATH_NATIONBALLOT				83	// ����ѡ��
#define PATH_MAPCALL					84	// ��ͼ�ٻ�
#define PATH_NATIONHERO					85	// ��������
#define PATH_HERO_GOD					86	// Ӣ����ͻ
#define PATH_WISHINGSHOP				87	// �۱���
#define PATH_WISHINGCHANGE				88	// �۱���һ�
#define PATH_WISHINGPACK				89	// �۱�����
#define PATH_BATTLEAUTOHP				90	// �����Զ�����
#define PATH_TEACHER					91	// ʦͽ
#define PATH_ACTIVITY					92	// �
#define PATH_TEACHERSHOP				93	// ʦͽ�̵�
#define PATH_GIRLCOLORUP				94	// Ů��ͻ��
#define PATH_GIRLMAKELOVE				95	// Ů�����ܻ���
#define PATH_FANGSHI_VISIT				96	// ����
#define PATH_FANGSHI_PALACE				97	// �������
#define PATH_GIRL_SHOP					98	// Ů���̵�
#define PATH_SELECTNATION				99	// ѡ�����
#define PATH_MAPPICKUP					100 // ��ͼʰȡ�¼�
#define PATH_FIGHTHELPER				101 // ��Ӷ��

int log_init();
int wlog( char type, int op, short path, int object, int objectnum, i64 objectid, int source, int target );
int wlog_token( char type, int op, short path, int object, int objectnum, i64 objectid, int source, int target, i64 userid );
int wcount();

#endif

