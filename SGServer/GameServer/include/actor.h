#ifndef __ACTOR_H
#define __ACTOR_H
#include "auto_actor.h"
#include "server_structsend_auto.h"
#include "award.h"

// ------------------------------- actor.c -----------------------------------
// ���������н�ɫ��ʼ��
int actors_init();

// �������߽�ɫ����
int actors_save_everyone( char type );

// ����������֮��Ĵ浵
void actors_on_core();

// �����ɫ����
int actor_save( int actor_index, int savecity, FILE *fp );

// ��ʼ�����ɫ����
int actor_real_saveall( int actor_index, FILE *fp );

// ���ͱ�����Ϣ������
int actor_send_save( int actor_index );

// ��ɫ����������Ϣ������
int actor_send_remove( int actor_index );

// ��ɫ���߱��沢�Ͽ��ͻ���
int actor_remove( int actor_index );

// ���½�ɫ�б�����
int actor_update_list( int actor_index );

// �������ݿ��н�ɫ�������
int actor_update_online( int actorid, char isonline );

// ����ͻ���
int actor_del( int client_index );

// ������ɾ����ɫ
int actor_delete( i64 userid, int actorid );

// ����ɾ�����
int actor_delete_stoptime( i64 userid, int actorid );

// ����ɾ��
int actor_delete_cancel( int actorid );

// ���н�ɫ�߼�
void actors_logic();

// ��ɫ�߼�
void actor_logic( int actor_index );

// ��ɫ������Ϸ
int actor_entergame( int client_index, int actorid );

// ��ɫ������Ϸ����
int actor_enterworld( int client_index, int actorid, int actor_index );

// ��ɫ����ǳ�
int actor_entercity( int actor_index );

// ��ɫ������Ϸ���ȡ����
int actor_load( int actor_index, int actorid );

// ��һ�ν�����ϷҪ��ʼ����ɫ��Ϣ
int actor_new( int actor_index );

// ������ɫ
int actor_create( int client_index, char nation, char *szActorName );

// ������ɫ,���ݿ�
int actor_db_create( int client_index, int platid, i64 userid, char *username, SLK_ListInfo *pListInfo );

// ��ɫ���¿�ʼ
int actor_restart( int actor_index );

// ��ɫ���¿�ʼ
int actor_restart_notonline( int actorid );

// ����
int actor_kick( int actor_index );
// ������ɫ
int actor_lock( int actorid, int locktime );
int actor_unlock( int actorid );
// ����
int actor_forbidtalk( int actor_index, int forbidtime );

// ������
int actor_blacklist_add( int actor_index, int actorid );
int actor_blacklist_del( int actor_index, int actorid );
int actor_blacklist_sendlist( int actor_index );
int actor_blacklist_sendlist_actorid( int actor_index );

// ����һ����ʱ֪ͨ����Ϣ
int actor_subscribecmd_add( int actor_index, short cmd );
int actor_subscribecmd_del( int actor_index, short cmd );
int actor_subscribecmd_check( int actor_index, short cmd );

// Ȩ��
int actor_permission_open( int actor_index, int offset );
int actor_permission_check( int actor_index, int offset );

// ------------------------------- actor_func.c -----------------------------------
// ����������ȡ�û�id
i64 actor_getoffline_userid( int actorid );

// ���ݱ�Ż�ȡ�������
int actor_getindex_withid( int actorid );

// ���ؽ�ɫ�б�
int actor_getlist( int platid, i64 userid, SLK_NetS_List *pValue );

// ��֤һ�������ɫid�ǲ������ڵ�½������û�
int actor_verifyid( i64 userid, int actorid );

// ��ɫ��ʯ
int actor_change_token( int actor_index, int token, char path, int path_value );
// Ԫ���ֿ۵���
int actor_change_token_sale( int actor_index, int value, short path );
// Ԫ����������
int actor_change_token_ret( int actor_index, int value, short path );
// ��Ծ��
int actor_change_edpoint( int actor_index, int value, short path );
// �޸�����
int actor_changename( int actor_index, char *pname, int type );

// �޸�����
int actor_changename_gm( int actorid, char *pname );

// �޸���Ů����
int actor_maid_changename( int actor_index, char *pname, int type );

// �޸�ͷ��
int actor_changeshape( int actor_index, char shape );
void actor_change_open( int actor_index, char shape );

// �޸�ǩ��
int actor_changsign( int actor_index, char *sign );

// ��ɫ��Ϣ
int actor_getinfo( int actor_index );

// ��ɫ����
int actor_configinfo( int actor_index );
int actor_setconfig( int actor_index, int index, char value );
int actor_getconfig( int actor_index, int index );

// ��������
int actor_buybody( int actor_index, int ask );

// һ��������Դ
int actor_buyres( int actor_index, int restype, int resnum, int ask );

// ��ѯ�����Ϣ
int actor_search( int actor_index, int target_actorid, int target_city_index );

// ���
int actor_redinfo( int actor_index, char path );

// ------------------------------- actor_gmcmd.c -----------------------------------
int actor_command( int actor_index, short cmd, int *pValue, char *pMsg );


#endif
