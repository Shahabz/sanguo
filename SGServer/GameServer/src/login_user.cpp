#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <mysql.h>
#include "netfunc.h"
#include "db.h"
#include "login.h"
#include "actor.h"
#include "gameproc.h"
#include "define.h"
#include "netserver.h"
#include "actor_send.h"
#include "http.h"
#include "server_netsend_auto.h"
#include "actor_notify.h"
#include "mail.h"

extern SConfig g_Config;
extern int g_tick;

extern MYSQL *myGame;

extern SLoginQueue *login_queue;
extern int g_nLoginQueueHead;
extern int g_nLoginQueueTail;

extern Mthr	*g_pthr_login;
extern ThrMux	*g_login_mmux;

extern int g_maxactorid;
extern Actor *g_actors;
extern int g_maxactornum;
extern char g_bServerIsInit;
extern PlatInfo *g_platinfo;
extern int g_platinfo_maxnum;
extern MYSQL *myData;
char g_user_queue_logic = 1;

const char *plat_loginpath( int platid )
{
	if ( platid < 0 || platid >= g_platinfo_maxnum )
		return "";
	return g_platinfo[platid].loginpath;
}

const char *plat_cdkeypath( int platid )
{
	if ( platid < 0 || platid >= g_platinfo_maxnum )
		return "";
	return g_platinfo[platid].cdkeypath;
}

const char *plat_invitecodepath( int platid )
{
	if ( platid < 0 || platid >= g_platinfo_maxnum )
		return "";
	return g_platinfo[platid].invitecodepath;
}

// �û����͵�½���󵽵�½����
int user_login( int client_index, const char *szUserName, const char *szPassWord, char *szDeviceID )
{
	int authid;
	int queue_tail;
	char tmpstr[2048];

	authid = server_getautuid( client_index );
	if( authid < 0 )
		return -1;
	// ��סд���л���
	mmux_lock( g_login_mmux );
	queue_tail = g_nLoginQueueTail + 1;
	if( queue_tail >= MAX_LOGINQUEUENUM )
	{
		queue_tail = 0;
	}
	if( g_nLoginQueueHead == queue_tail )
	{
		write_netlog("login queue full");
		mmux_unlock( g_login_mmux );
		// ֪ͨ��¼�̵߳øϽ��ɻ�, �����˰�
		mcond_broadcast( g_pthr_login );

		SLK_NetS_Login Value = {0};
		Value.m_result = -10000;
		netsend_login_S( client_index, SENDTYPE_ACTOR, &Value );
		return -1;
	}
	int platid = client_getplatid( client_index );
	// ����Ҫ����Ŀͻ����õ���¼����
	login_queue[g_nLoginQueueTail].client_index = client_index;
	login_queue[g_nLoginQueueTail].authid = authid;
	login_queue[g_nLoginQueueTail].command = USERCMDC_LOGIN;
	strcpy( login_queue[g_nLoginQueueTail].path, plat_loginpath( platid ) );
	sprintf( login_queue[g_nLoginQueueTail].data, "&v1=%s&v2=%s&v3=%d&v4=%s&v5=%s", szUserName, HttpString( szPassWord, tmpstr ), platid, client_getip( client_index ), szDeviceID );
	g_nLoginQueueTail = queue_tail;
	mmux_unlock( g_login_mmux );
	// ֪ͨ��¼�̸߳ɻ�
	mcond_broadcast( g_pthr_login );
	return 0;
}

// �û����������ص�½���
int user_logined( int client_index, int authid, SClientInfo *pClientInfo )
{
	char client_stat;
	SLK_NetS_Login Value = {0};

	if ( pClientInfo == NULL )
		return -1;

	if ( server_getautuid( client_index ) != authid )
	{
		Value.m_result = -10;
		netsend_login_S( client_index, SENDTYPE_ACTOR, &Value );
		return -1;
	}
	client_stat = client_getstat( client_index );
	if ( client_stat < 0 || client_stat > CLIENT_STAT_LOGIN )
	{
		Value.m_result = -11;
		netsend_login_S( client_index, SENDTYPE_ACTOR, &Value );
		return -1;
	}

	if ( pClientInfo->result >= 0 )
	{
		client_setstat( client_index, CLIENT_STAT_LOGIN );
		client_setuserinfo( client_index, pClientInfo );
	}

	Value.m_result = pClientInfo->result;
	Value.m_usertype = pClientInfo->usertype;

	// �����û���
	Value.m_username_length = (int)strlen( pClientInfo->username ) + 1;
	strcpy( Value.m_username, pClientInfo->username );

	// ��½ʧ��
	if ( pClientInfo->result < 0  )
	{
		netsend_login_S( client_index, SENDTYPE_ACTOR, &Value );
		return -1;
	}

	// ��½�ɹ�
	Value.m_token_length = 65;
	memcpy( Value.m_token, pClientInfo->access_token, Value.m_token_length*sizeof(char) );
	netsend_login_S( client_index, SENDTYPE_ACTOR, &Value );
	return 0;
}


int actor_set_cdkey( int actor_index, int batchno )
{
	int flag;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	if ( batchno >= 0 && batchno < 32 )
	{
		flag = (1 << batchno);
		g_actors[actor_index].cdkey1 |= flag;
	}
	else if ( batchno >= 32 && batchno < 64 )
	{
		batchno = batchno - 32;
		flag = (1 << batchno);
		g_actors[actor_index].cdkey2 |= flag;
	}
	else if ( batchno >= 64 && batchno < 96 )
	{
		batchno = batchno - 64;
		flag = (1 << batchno);
		g_actors[actor_index].cdkey3 |= flag;
	}
	else if ( batchno >= 96 && batchno < 128 )
	{
		batchno = batchno - 96;
		flag = (1 << batchno);
		g_actors[actor_index].cdkey4 |= flag;
	}
	return 0;
}

int actor_get_cdkey( int actor_index, char *outkey, int len )
{
	if ( outkey == NULL )
		return -1;
	memset( outkey, 0, len );
	for ( int tmpi = 0; tmpi < len; tmpi++ )
	{
		int batchno = tmpi;
		if ( batchno >= 0 && batchno < 32 )
		{
			if ( g_actors[actor_index].cdkey1 & (1 << batchno) )
				outkey[tmpi] = '1';
			else
				outkey[tmpi] = '0';
		}
		else if ( batchno >= 32 && batchno < 64 )
		{
			batchno = batchno - 32;
			if ( g_actors[actor_index].cdkey2 & (1 << batchno) )
				outkey[tmpi] = '1';
			else
				outkey[tmpi] = '0';
		}
		else if ( batchno >= 64 && batchno < 96 )
		{
			batchno = batchno - 64;
			if ( g_actors[actor_index].cdkey3 & (1 << batchno) )
				outkey[tmpi] = '1';
			else
				outkey[tmpi] = '0';
		}
		else if ( batchno >= 96 && batchno < 128 )
		{
			batchno = batchno - 96;
			if ( g_actors[actor_index].cdkey4 & (1 << batchno) )
				outkey[tmpi] = '1';
			else
				outkey[tmpi] = '0';
		}
	}
	return 0;
}

// ���Ͷһ�������Ϣ
int user_cdkey( int client_index, char *cardnumber )
{
	if ( client_index < 0 || client_index >= g_maxactornum )
		return -1;
	if ( !cardnumber )
		return -1;
	int len = (int)strlen( cardnumber );
	if ( len <= 0 || len >= 32 )
		return -1;
	int authid = server_getautuid( client_index );
	if ( authid < 0 )
		return -1;

	if ( g_actors[client_index].cdkeywait )
		return -1;
	g_actors[client_index].cdkeywait = 1;

	char tmpstr[2048] = {0};
	char szUserID[32] = {0};
	lltoa( client_getuserid( client_index ), szUserID, 10 );

	char cdkey[129] = {0};
	actor_get_cdkey( client_index, cdkey, 128 );
	cdkey[128] = 0;

	int queue_tail;

	// ��סд���л���
	mmux_lock( g_login_mmux );
	queue_tail = g_nLoginQueueTail + 1;
	if ( queue_tail >= MAX_LOGINQUEUENUM )
	{
		queue_tail = 0;
	}
	if ( g_nLoginQueueHead == queue_tail )
	{
		write_netlog( "login queue full" );
		mmux_unlock( g_login_mmux );
		// ֪ͨ��¼�̵߳øϽ��ɻ�, �����˰�
		mcond_broadcast( g_pthr_login );
		return -1;
	}
	// ����Ҫ����Ŀͻ����õ���¼����
	int platid = client_getplatid( client_index );
	login_queue[g_nLoginQueueTail].client_index = client_index;
	login_queue[g_nLoginQueueTail].authid = authid;
	login_queue[g_nLoginQueueTail].command = USERCMDC_CDKEY;
	strcpy( login_queue[g_nLoginQueueTail].path, plat_cdkeypath( platid ) );
	sprintf( login_queue[g_nLoginQueueTail].data, "&v1=%d&v2=%d&v3=%s&v4=%s&v5=%s&v6=%s&v7=%d&v8=%d&v9=%d", g_actors[client_index].actorid, g_Config.server_code, szUserID, client_getip( client_index ), HttpString( cardnumber, tmpstr ), cdkey, client_getos( client_index ), client_getchannelid( client_index ), platid );

	g_nLoginQueueTail = queue_tail;
	mmux_unlock( g_login_mmux );
	// ֪ͨ��¼�̸߳ɻ�
	mcond_broadcast( g_pthr_login );
	return 0;
}

// �û����������ؽ�����Ϣ
int user_cdkeyed( int client_index, int authid, int cdkey_index, int awardgroup, int result, char *cardnumber )
{
	if ( client_index < 0 || client_index >= g_maxactornum )
		return -1;
	
	g_actors[client_index].cdkeywait = 0;
	if ( server_getautuid( client_index ) != authid )
	{
		return -1;
	}

	if ( result == 0 && cardnumber )
	{
		// ֱ���ʼ�����
		char v1[32] = { 0 };
		char attach[256] = { 0 };
		sprintf( v1, "%s", cardnumber );
		awardgroup_mail( awardgroup, 0, attach );
		mail_system( client_index, g_actors[client_index].actorid, 5048, 5543, v1, NULL, NULL, attach, 0 );

		// ��¼cdkey
		actor_set_cdkey( client_index, cdkey_index );
		actor_notify_pop( client_index, 490 );
	}
	else if ( result == -3 )
	{
		actor_notify_pop( client_index, 491 ); // ���Ѿ�ʹ�ù������ζһ�����
	}
	else if ( result == -4 )
	{
		actor_notify_pop( client_index, 492 ); // ����һ����Ѿ���������ʹ�ù���
	}
	else if ( result == -5 )
	{
		actor_notify_pop( client_index, 493 ); // ����һ���ֻ����iosƽ̨ʹ��
	}
	else if ( result == -6 )
	{
		actor_notify_pop( client_index, 494 ); // ����һ���ֻ����androidƽ̨ʹ��
	}
	else if ( result == -7 )
	{
		actor_notify_pop( client_index, 495 ); // ����һ���ֻ����ָ���������ص���Ϸ��ʹ��
	}
	else if ( result == -8 )
	{
		actor_notify_pop( client_index, 496 ); // �öһ����Ѿ�����
	}
	else
	{
		actor_notify_pop( client_index, 497 ); // �һ�����Ч
	}

	return 0;
}

// ������������Ϣ
int user_invitecode( int client_index, int path, char *msg )
{
	if ( client_index < 0 || client_index >= g_maxactornum )
		return -1;
	if ( !msg )
		return -1;
	int len = (int)strlen( msg );
	if ( len <= 0 || len >= 32 )
		return -1;
	int authid = server_getautuid( client_index );
	if ( authid < 0 )
		return -1;

	if ( g_actors[client_index].cdkeywait )
		return -1;
	g_actors[client_index].cdkeywait = 1;

	char tmpstr[2048] = { 0 };
	char szUserID[32] = { 0 };
	lltoa( client_getuserid( client_index ), szUserID, 10 );

	int queue_tail;

	// ��סд���л���
	mmux_lock( g_login_mmux );
	queue_tail = g_nLoginQueueTail + 1;
	if ( queue_tail >= MAX_LOGINQUEUENUM )
	{
		queue_tail = 0;
	}
	if ( g_nLoginQueueHead == queue_tail )
	{
		write_netlog( "login queue full" );
		mmux_unlock( g_login_mmux );
		// ֪ͨ��¼�̵߳øϽ��ɻ�, �����˰�
		mcond_broadcast( g_pthr_login );
		return -1;
	}
	// ����Ҫ����Ŀͻ����õ���¼����
	int platid = client_getplatid( client_index );
	login_queue[g_nLoginQueueTail].client_index = client_index;
	login_queue[g_nLoginQueueTail].authid = authid;
	login_queue[g_nLoginQueueTail].command = USERCMDC_INVITECODE;
	strcpy( login_queue[g_nLoginQueueTail].path, plat_invitecodepath( platid ) );
	sprintf( login_queue[g_nLoginQueueTail].data, "&v1=%d&v2=%d&v3=%s&v4=%d&v5=%s", g_actors[client_index].actorid, g_Config.server_code, szUserID, path, HttpString( msg, tmpstr ) );

	g_nLoginQueueTail = queue_tail;
	mmux_unlock( g_login_mmux );
	// ֪ͨ��¼�̸߳ɻ�
	mcond_broadcast( g_pthr_login );
	return 0;
}
// �û�������������������Ϣ
int user_invitecodeed( int client_index, int authid, int result, int path, int isaward )
{
	if ( client_index < 0 || client_index >= g_maxactornum )
		return -1;

	g_actors[client_index].cdkeywait = 0;
	if ( server_getautuid( client_index ) != authid )
	{
		return -1;
	}

	if ( result == 0 )
	{
		if ( path == 1 || path == 2 || path == 3 )
		{
			if ( isaward == 0 )
			{
				AwardGetInfo getinfo = { 0 };
				awardgroup_withindex( client_index, 200, 0, PATH_INVITECODE, &getinfo );
			}
		}
		else if ( path == 4 )
		{
			if ( isaward == 0 )
			{
				AwardGetInfo getinfo = { 0 };
				awardgroup_withindex( client_index, 201, 0, PATH_INVITECODE, &getinfo );
			}
		}
		else if ( path == 5 )
		{
			if ( isaward == 0 )
			{
				AwardGetInfo getinfo = { 0 };
				awardgroup_withindex( client_index, 202, 0, PATH_INVITECODE, &getinfo );
			}
		}
	}

	int value[2] = { 0 };
	value[0] = result;
	value[1] = path;
	actor_notify_value( client_index, NOTIFY_INVITECODE, 2, value, NULL );
	return 0;
}

// �����Ŷ���Ϣ����ʱ����
int user_queue_logic()
{
	int tmpi;

	// ��סд���л���
	mmux_lock( g_login_mmux );
	
	int nTail = g_nLoginQueueTail;
	int nHead = g_nLoginQueueHead;

	if ( nTail != nHead )
	{
		int queuecount = 0;
		if ( nTail >= nHead )
		{
			queuecount = nTail - nHead;
		}
		else
		{
			queuecount = MAX_LOGINQUEUENUM - nHead + nTail;
		}
		// ����Ҫ����Ŀͻ����õ���¼����
		for ( tmpi = 0; tmpi < MAX_LOGINQUEUENUM; tmpi++ )
		{
			int client_index = login_queue[tmpi].client_index;
			if ( client_index < 0 || client_index >= g_maxactornum )
				continue;		
			if ( client_getstat( client_index ) >= CLIENT_STAT_LOGIN )
				continue;

			int value[1] = { 0 };
			value[0] = queuecount;
			actor_notify_value( client_index, NOTIFY_LOGIN_QUEUE, 1, value, NULL );
		}
	}
	
	// �������л���
	mmux_unlock( g_login_mmux );
	return 0;
}

// ��յ�½����
void login_queue_clear()
{
	// ��סд���л���
	mmux_lock( g_login_mmux );

	g_nLoginQueueHead = 0;
	g_nLoginQueueTail = 0;
	memset( login_queue, 0, sizeof(SLoginQueue)*MAX_LOGINQUEUENUM );
	for ( int tmpi = 0; tmpi < MAX_LOGINQUEUENUM; tmpi++ )
	{
		login_queue[tmpi].client_index = -1;
	}
	// �������л���
	mmux_unlock( g_login_mmux );
}
