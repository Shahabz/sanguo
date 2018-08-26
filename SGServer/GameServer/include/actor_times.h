#ifndef _ACTOR_TIMES_H_
#define _ACTOR_TIMES_H_

// 32λ��������־λ����0��1 ��ʾ��û������û�У���û�������ô浵��
#define ACTOR_SFLAG_BODY_FREEBUY	0	// �״ι����������
#define ACTOR_SFLAG_MAPZONE_GO_ZC	1	// �״����ǰ���ݳ�
#define ACTOR_SFLAG_QUEST_AUTOBUILD	2	// ���������Ƿ���ȡ���Զ�����
#define ACTOR_SFLAG_SKIPFIGHT		3	// �Ƿ��������ս��
#define ACTOR_SFLAG_PALACE			4	// �Ƿ����ʹ���Ժ
#define ACTOR_SFLAG_STORYSWEEP		5	// �Ƿ���ɨ������
#define	ACTOR_SFLAG_OFFICIAL_TECH	6   // �Ƿ�����ɫ�о�Ա
#define ACTOR_SFLAG_EQUPIPDRAWING	7	// �Ƿ���װ���ֽ���ͼֽ
#define	ACTOR_SFLAG_MATERIAL_MAKEWILL 8 // �Ƿ�������Ԥ��
#define	ACTOR_SFLAG_TEACHERAWARD	9 // �Ƿ���ȡ��ʦ����
#define	ACTOR_SFLAG_ACTIVITY08_OPEN	10 // �Ƿ����ɳ��ƻ�
#define	ACTOR_SFLAG_ACTIVITY05_CALL	11 // �Ƿ�������ǰݽ�
#define	ACTOR_SFLAG_FRISTPAY_AWARDGET	12 // �׳�����Ƿ���ȡ
void actor_set_sflag( int actor_index, int offset, char value );
int actor_get_sflag( int actor_index, int offset );

// char ���͵�ÿ�����ݣ�ÿ����£�
#define MAX_TODAY_COUNT		127
#define TODAY_CHAR_BUYBODY			0 // ���칺����������
#define	TODAY_CHAR_LOGINAWARD		1 // �����½����
#define TODAY_CHAR_WORLDBOSS		2 // �����Ƿ�������boss
#define TODAY_CHAR_SHOP_SALEITEM	3 // �����Ƿ�����˴�����Ʒ
#define TODAY_CHAR_ITEMUSE_BODY		4 // ����ʹ���������߶��ٴ���
#define TODAY_CHAR_NATION_DONATE	5 // ������ҽ������
#define TODAY_CHAR_NATION_MISSION_AWARD1	6 // �����Ƿ���ȡ��������������
#define TODAY_CHAR_NATION_MISSION_AWARD2	7 // �����Ƿ���ȡ��������������
#define TODAY_CHAR_NATION_MISSION_AWARD3	8 // �����Ƿ���ȡ��������������
#define TODAY_CHAR_MAPCALLNUM		9	// �����ٻ�����	
#define TODAY_CHAR_WISHINGSHOP_TODAYBUY		10	// �۱�������Ƿ��Ѿ�����	
#define TODAY_CHAR_WISHINGPACK_SILVER	11	// �۱��������������Ҵ������
#define TODAY_CHAR_WISHINGPACK_WOOD		12	// �۱�����������ľ�Ĵ������
#define TODAY_CHAR_WISHINGPACK_FOOD 	13	// �۱�������������ʳ�������
#define TODAY_CHAR_ACTIVITY_BODYGET1	14	// ����
#define TODAY_CHAR_ACTIVITY_BODYGET2	15	// ����
#define TODAY_CHAR_FANSHI_VISIT_FREE	16	// �������Ѱ�ô���
#define TODAY_CHAR_GIRLSHOP_UPDATE		17	// Ů���̵�ˢ�´���
#define TODAY_CHAR_WORLDBOSS_RESET		18 // ������������boss����
#define TODAY_CHAR_MAP_PICKUP			19 // ����ʰȡ��������

int actor_get_today_char_times( int actor_index, int offset );
int actor_add_today_char_times( int actor_index, int offset );
int actor_set_today_char_times( int actor_index, int offset, char value );

// short ���͵�ÿ�����ݣ�ÿ����£�
#define MAX_TODAY_SHORT_COUNT		32
#define TODAY_SHORT_VIPSHOP_BUYNUM_BASE		0	// ���칺��ָ�����ߴ���
#define TODAY_SHORT_VIPSHOP_BUYNUM_END		24	// ���칺��ָ�����ߴ���
int actor_get_today_short_times( int actor_index, int offset );
int actor_add_today_short_times( int actor_index, int offset, short value );
int actor_set_today_short_times( int actor_index, int offset, short value );

// int ���͵�ÿ�����ݣ�ÿ����£�
#define MAX_TODAY_INTNUM	8
int actor_get_today_int_times( int actor_index, int offset );
int actor_add_today_int_times( int actor_index, int offset );
int actor_set_today_int_times( int actor_index, int offset, int value );

// CDʱ��
#define USELIMIT_CD_MAXCOUNT 8
#define USELIMIT_CD_TOWN_DEV		0 // ���ǿ�����ȴ
#define USELIMIT_CD_WISHING_CHANGE	1 // �۱���һ���ȴ
int actor_set_uselimit_cd( int actor_index, char index, int cdtime );
int actor_add_uselimit_cd( int actor_index, char index, int cdtime );
int actor_check_uselimit_cd( int actor_index, char index );
int actor_clear_uselimit_cd( int actor_index, char index );

void _check_fday( int actor_index );

// ÿ�յ�½����
int actor_loginaward( int actor_index );
#endif
