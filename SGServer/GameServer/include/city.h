#ifndef _CITY_H_
#define _CITY_H_
#include "define.h"
#include "auto_data_city_guard.h"
#define CITY_QUEST_MAX				16	// ��������
#define CITY_TECH_MAX				40	// �ǳؿƼ�
#define CITY_GUARD_MAX				30	// ��ǽ����
#define CITY_DATA_RECORD_MAX		16	// ���ݼ�¼
#define CITY_EVENT_MAX				4	// �¼�
#define CITY_TRAINQUEUE_MAX			11	// �����ݵ�ѵ����������
#define CITY_MATERIALMAKE_MAX		6	// 6��������������
#define CITY_BATTLEQUEUE_MAX		8	// ��������
#define CITY_UNDERFIRE_MAX			8	// ����ܶ���֧���ӹ���
#define	CITY_UNDERFIRE_GROUP_MAX	16  // ����ܶ��ٸ���ս
#define CITY_HELPDEFENSE_MAX		32	// ����ܶ���֧����פ��
#define CITY_MAPEVENT_MAX			6	// ��ͼ�¼��������
#define CITY_MAPEVENT_ING_MAX		2	// ��ͼ�¼����ڽ�������
#define CITY_BAG_MAX				8	// ���͵��������

// �������
#define	CITY_FUNCTION_SMITHY		0	// ������
#define	CITY_FUNCTION_WASH			1	// ϴ����
#define	CITY_FUNCTION_FANGSHI		2	// ����
#define	CITY_FUNCTION_SHOP			3	// �̵�
#define	CITY_FUNCTION_HERO			4	// ���͹�
#define	CITY_FUNCTION_WISHING		5	// �۱���

#define CITY_FUNCTION_NATION		10	// ����
#define CITY_FUNCTION_WORLD			11	// ����
#define CITY_FUNCTION_STORY			12	// ����
#define CITY_FUNCTION_MAIL			13	// �ʼ�
#define CITY_FUNCTION_FRIEND		14	// ����
#define CITY_FUNCTION_NATIONEQUIP	15	// ����
#define CITY_FUNCTION_RANK			16	// ���а�
#define CITY_FUNCTION_HEROBUTTON	17	// Ӣ��
#define CITY_FUNCTION_CHAT			18	// ����
#define CITY_FUNCTION_WARKEREX		19	// ��ҵ�����
#define CITY_FUNCTION_Militiaman	20	// ���Ӫ
#define CITY_FUNCTION_Iron			21	// ����

#define CITY_FUNCTION_AUTO_ADDHP	25	// �Զ����������ر�
#define CITY_FUNCTION_BATTLE_ADDHP	26	// սǰ���������ر�

#define CITY_EVENT_BUILDING			1	// ��������
#define CITY_EVENT_TRAIN			2	// ļ��
#define CITY_EVENT_FORGING			3	// װ������
#define CITY_EVENT_TECH				4	// �Ƽ��о�

#define CITY_BATTLE_EVENT_SPY				1	// ���
#define CITY_BATTLE_EVENT_BESPY				2	// �����
#define CITY_BATTLE_EVENT_ASSAULT			3	// ����
#define CITY_BATTLE_EVENT_DEFEND			4	// �س�
#define CITY_BATTLE_EVENT_NATION_ASSAULT	5	// ��ս
#define CITY_BATTLE_EVENT_NATION_DEFEND		6	// ��ս����

#define CITY_STATE_FIRE				0x01	// �Ż���
#define CITY_STATE_ARMYGROUP		0x02	// ����ս��

#define CITY_SFLAG_FRISTPAY			1		// �״γ�ֵ

// �ǳ���������
typedef enum
{
	CityLairdType_Player = 0,		//	��ҵĳǳ�
	CityLairdType_Robot = 2,		//  �����˳ǳ�
}CityLairdType;

// ������������ȡ���гǳ���Ϣ���ڴ�
int city_load();

// �������رգ����гǳ���Ϣ�浽���ݿ�
int city_save( FILE *fp );
int city_single_save( City *pCity, FILE *fp );

// ��ȡ����ǳ�����
City *city_indexptr( int city_index );

// ��ȡ�����ҵĳǳ�����
City *city_getptr( int actor_index );

// ��ȡ����ǳ�����
City *city_getptr_withactorid( int actorid );

// �������id�ҵ��ǳ�����
int city_getindex_withactorid( int actorid );

// ������������ҵ��ǳ�����
int city_getindex_withactorname( const char *actorname );

// ��ȡ����
char city_getnation( int city_index );

// ��ȡ��������
char* city_getname( int city_index );

// ��ʾ��Ԫ����
void city_makeunit( int city_index, SLK_NetS_AddMapUnit *pAttr );
void city_makezoneunit( int city_index, SLK_NetS_ZoneUnit *pAttr );

// �ǳ�λ��
void city_getpos( int city_index, short *posx, short *posy );

// ����һ���³ǳ�
int city_new( City *pCity );

// ���г���ÿ����߼�
void city_logic_sec( int begin, int end );

// ɾ��һ�������˳ǳ�
void city_delrobot();
// ɾ�����л����˳ǳ�
void city_delallrobot();

// ɾ��һ���ǳ�
void city_del( City *pCity, int city_index );

// AI�й�һ���ǳ�
void city_entrust( City *pCity, int ai );

// �������ǵȼ�
int city_mainlevel( int city_index );

// ���ó���״̬
void city_setstate( City *pCity, char state );

// �Ƴ�����״̬
void city_delstate( City *pCity, char state );

// 32λ��������־λ����0��1 ��ʾ��û������û�У���û�������ô浵��
void city_set_sflag( City *pCity, int offset, char value );
int city_get_sflag( City *pCity, int offset );

// ���ܻ�ȡ
void city_function_open( City *pCity, int offset );
void city_function_close( City *pCity, int offset );
int city_function_check( City *pCity, int offset );

// �¼�
int city_event_load( City *pCity );
int city_event_save( City *pCity, FILE *fp );
int city_event_add( int city_index, char type, short kind, int value );
int city_battle_event_add( int city_index, char type, char *name, char value, i64 mailid );
int city_event_sendlist( int actor_index );

// ���Ǿ�������
int city_actorexp( int city_index, int exp, char path );
int city_actorupgrade( int city_index, short path, AwardGetInfo *getinfo );

// ����
int city_changebody( int city_index, int value, short path );
// ���մ���
int city_changelevy( int city_index, int value, short path );
// ����
int city_changesilver( int city_index, int value, short path );
// ľ��
int city_changewood( int city_index, int value, short path );
// ����
int city_changefood( int city_index, int value, short path );
// ����
int city_changeiron( int city_index, int value, short path );
// �˿�
int city_changepeople( int city_index, int value, short path );
// ����ֵ
int city_changeprestige( int city_index, int value, short path );
// �������
int city_changefriendship( int city_index, int value, short path );
// Ѫս����
int city_kingwarpoint( int city_index, int value, short path );
// ����ʱ��
int city_changeprotect( int city_index, int value, short path );
// ����
int city_changesoldiers( int city_index, short corps, int value, short path );
// �Զ��������
int city_autobuild_open( int city_index );
int city_change_autobuild( int city_index, int value, short path );
// �Ƿ��������
int city_autoguard_open( int city_index );
int city_change_autoguard( int city_index, int value, short path );
// ϴ�����
int city_change_herowash( int city_index, int value, short path );
// ϴ������
int city_change_equipwash( int city_index, int value, short path );
// ѡƱ
int city_changevote( int city_index, int value, short path );
// ��ǽ����
CityGuardInfoConfig *city_guard_config( int monsterid, int color );
CityGuard *city_guard_getptr( int city_index, int offset );
int city_guard_call( int city_index );
int city_guard_upgrade( int city_index, int offset );
int city_guard_soldiers( int city_index, int offset );
int city_guard_subsoldiers( int city_index, int offset, int sub );
int city_guard_clearcd( int city_index );
int city_guard_send( int actor_index, int offset );
int city_guard_sendsec( int actor_index );
int city_guard_sendlist( int actor_index );
int city_guard_sendnum( int actor_index );

// ����
int city_yield_total( City *pCity, int kind, int num );
int city_levy( int actor_index );
int city_levy_sendinfo( int actor_index );

// ��ļ
int city_train( int actor_index, int kind, int trainsec );
int city_train_cancel( int actor_index, int kind, int queue );
int city_train_quick( int actor_index, int kind, int sec );
int city_train_finish( City *pCity, BuildingBarracks *barracks );
int city_train_get( int actor_index, int kind );
int city_train_buyqueue( int actor_index, int kind );
int city_train_buylong( int actor_index, int kind );
int city_train_sendinfo( int actor_index, int kind );
int city_train_awardquick( int city_index, int sec );
int city_train_awardquick_get( int actor_index, int kind );

// ��������
int city_material_sendinfo( int actor_index, char sendchange );
int city_material_will_sendinfo( int actor_index );
int city_material_updatebuilding( City *pCity );
int city_material_needsec( City *pCity, int itemkind );
int city_material_make( int actor_index, int id, int itemkind, int willindex );
int city_material_finish( City *pCity, int index );
int city_material_get( int actor_index );
int city_material_buyqueue( int actor_index );
int city_material_gm( City *pCity, int sec );

// ��Ӷ��
int city_officialhire( int actor_index, int type, int kind );
int city_officialhire_sendinfo( City *pCity, int type );

// ��ȡʿ������
int city_soldiers( int city_index, short corps );
int city_soldiers_withkind( int city_index, short kind );

// �����������
int city_battlequeue_add( City *pCity, int army_index );

// ɾ����������
int city_battlequeue_del( City *pCity, int army_index );

// ��������
void city_battlequeue_sendlist( int actor_index, int unit_index );

// ���г�������
void city_battlequeue_sendupdate( int army_index );

// ��������Ϣ���
int city_underfire_add( City *pCity, int army_index );

// ��������Ϣ�Ƴ�
int city_underfire_del( City *pCity, int army_index );

// ��������Ϣ�Ƴ�
int city_underfire_del_equal( City *pCity, int equal_army_index );

// ��������Ϣ����
int city_underfire_getnum( City *pCity );

// ��������Ϣ���
int city_underfire_groupadd( City *pCity, int group_index );

// ��������Ϣ�Ƴ�
int city_underfire_groupdel( City *pCity, int group_index );

// �����б�
void city_underfire_makestruct( int group_index, SLK_NetS_CityWarInfo *pValue );
int city_underfire_sendlist( int actor_index );

// ���פ������
int city_helparmy_add( City *pCity, int army_index );

// ɾ��פ������
int city_helparmy_del( City *pCity, int army_index );

// ��ȡפ����������
int city_helparmy_getnum( City *pCity );

// ��ȡפ��������������
int city_helparmy_maxnum( City *pCity );

// ��ȡפ������Ϣ
int city_helparmy_sendlist( int actor_index, int unit_index );

// Ǩ��
int city_move_actor( int actor_index, short posx, short posy, int itemkind );
int city_move( City *pCity, short posx, short posy );

// ���
int city_spy( int actor_index, int unit_index, int type );

// ��԰�ؽ�
int city_lost_rebuild( City *pCity );
int city_lost_rebuild_get( int actor_index );
int city_lost_rebuild_num( int actor_index );

// ����ϵͳ
void city_everyday_event_add( City *pCity );
int city_everyday_event_update( int actor_index );
int city_everyday_event_sendinfo( int actor_index );
int city_everyday_event_getaward( int actor_index, int select );

// GM��ȡ�ǳ���Ϣ
int city_gm_getinfo( City *pCity );
#endif
