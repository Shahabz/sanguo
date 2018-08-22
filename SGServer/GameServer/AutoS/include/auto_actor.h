#ifndef __ACTOR_AUTO_H
#define __ACTOR_AUTO_H
#include "define.h"

struct _actor {
//--Automatically generated
				int actorid;	//��ɫ���
				int accountid;	//��½id
				i64 userid;	//�˺ű��
				char username[51];	//�˺���
				short admin;	//����Ա����
				char lastip[16];	//�ϴε�½ip
				int createtime;	//����ʱ��
				int forbidtime;	//����ʱ��
				int savetime;	//�ϴα���ʱ��
				unsigned char isexit;	//�Ƿ��뿪��Ϸ��
				short view_areaindex;	//��ǰ�ӿڵĵ�ͼ�����߼���
				char name[22];	//��ɫ����
				char nation;	//��ɫ����
				unsigned char shape;	//����
				short level;	//����
				short view_zoneid;	//��ǰ�ӿڵĵ�ͼ������Ϸ��
				int city_index;	//�ǳ�����
				int token;	//��ʯ
				int total_charge;	//�ܳ�ֵ
				float charge_dollar;	//�����Ѷ���Ǯ
				unsigned char itemext;	//��չ�ı�����λ��
				Item item[300];	//��ɫЯ���ĵ���
				Equip equip[180];	//��ɫЯ����װ��
				unsigned char equipext;	//��չ��װ����λ��
				Hero hero[80];	//δ����Ӣ��
				int quest_talkid;	//����Ի�
				int worldquest_complete;	//����������ȡ���
				short worldquestid;	//��������
				ActorFriend friends[100];	//����
				int blacklist[50];	//������
				int sflag;	//�ض���־λ
				int fdate;	//ˢ�µ�����������
				char today_char[128];	//char����ÿ�մ���
				short today_short[32];	//short����ÿ�մ���
				int today_int[8];	//int����ÿ�մ���
				int cd[8];	//ͨ��CDʱ��
				char config[8];	//�������
				int permission;	//Ȩ���Ƿ���
				short storyid;	//���������½�
				char story_star[128];	//��Ӣ�����Ǽ�
				char story_hero[32];	//��ļ����
				char story_resnum[32];	//��Դ��������
				int story_restime[32];	//��Դ����ʱ��
				char story_resreset[32];	//��Դ�������ô���
				char story_itemnum[64];	//���߸����������
				char story_drawing[16];	//װ��ͼֽ�����Ƿ���
				short story_sweephero[4];	//ɨ���佫
				int view_lastframe;	//�ϴη��͵�������Ϣʱ��
				short subscribe_cmd[8];	//���ĵ���ϢID
				int lastpeople_c;	//�ϴα����˿�
				int lastpeople_n;	//�ϴι����˿�
				int cdkey1;	//�Ƿ���ȡ�����ζһ���
				int cdkey2;	//�Ƿ���ȡ�����ζһ���
				int cdkey3;	//�Ƿ���ȡ�����ζһ���
				int cdkey4;	//�Ƿ���ȡ�����ζһ���
				char hv_ln;	//����Ѱ�ô���
				char hv_hn;	//��Ѱ�ô���
				short hv_pro;	//��ǰѰ�ý���
				int vipbag;	//vip����Ƿ���
				char shop_useitem;	//�̵��Ƿ�����ʹ�õ��߹����
				int charge_point;	//��ֵ����
				short pay_maxtier;	//���ʸ�����󼶱�
				short shop_saleitem[6];	//�̵������Ʒ
				char neq_crit[6];	//��������״̬
				char mapcallfree;	//����ٻ�����
				short wishingid[9];	//�۱�������б�
				char wishingopen[9];	//�۱�����߿���
				int wishingday;	//�۱�����������
				int wishingcd;	//�۱���Ѱ����ȴ
				int shape_bag;	//���󱳰�
				int student[30];	//ͽ��
				char te_award[10];	//ʦͽ��������ȡ����
				char te_awarded[10];	//ʦͽ�����Ѿ���ȡ����
				int te_shop;	//ʦͽ�̵�
				int act02_state;	//���ǵȼ����ȡ����
				char act03_state;	//�����ӵػ��ȡ����
				char act04_state[128];	//���տ񻶻״̬
				int act04_value[40];	//���տ񻶻��ֵ
				short act05_item[6];	//���ǰݽ��-�����б�
				char act05_isbuy;	//���ǰݽ��-�����Ƿ���
				char act05_buynum;	//���ǰݽ��-���߹������
				char act05_xw;	//���ǰݽ��-����
				char act05_upnum;	//���ǰݽ��-ʣ��ˢ�´���
				int act05_upstamp;	//���ǰݽ��-ˢ��ʱ��
				short act08_state;	//�ɳ��ƻ����ȡ����
				char fs_nodeid;	//���нڵ�
				int fs_awardkind[14];	//����Ѱ�ý���
				int fs_awardnum[14];	//����Ѱ�ý���
				int fs_awardfday;	//����Ѱ�ý�������
				int fs_weekkind[5];	//�ʹ���Ժ����
				int fs_weeknum[5];	//�ʹ���Ժ����
				int fs_fweek;	//��������
				short girlshop[9];	//Ů���̵�
				short girlshop_buy;	//Ů���̵��Ƿ���
				int girlshop_fday;	//Ů���̵�ˢ������
				char maidname[22];	//��Ů����
				int token_sale;	//Ԫ���ֿ۵���
				int token_ret;	//��������
				int talkspeed_frame;	//˵���ӳ�
				char cdkeywait;	//CDKEY�ȴ�
				short mail_notreadnum;	//δ���ʼ�����
};
typedef struct _actor Actor;

int actor_load_auto( int actorid, Actor *pActor, const char *pTab );
int actor_save_auto( Actor *pActor, const char *pTab, FILE *fp );

#endif
