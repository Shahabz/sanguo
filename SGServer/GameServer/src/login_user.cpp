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

extern MYSQL *myData;
char g_user_queue_logic = 1;

// 用户发送登陆请求到登陆队列
int user_login( int client_index, const char *szUserName, const char *szPassWord, char *szDeviceID )
{
	int authid;
	int queue_tail;
	char tmpstr[2048];

	authid = server_getautuid( client_index );
	if( authid < 0 )
		return -1;
	// 锁住写队列缓冲
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
		// 通知登录线程得赶紧干活, 都满了啊
		mcond_broadcast( g_pthr_login );

		SLK_NetS_Login Value = {0};
		Value.m_result = -10000;
		netsend_login_S( client_index, SENDTYPE_ACTOR, &Value );
		return -1;
	}
	// 将需要处理的客户放置到登录队列
	login_queue[g_nLoginQueueTail].client_index = client_index;
	login_queue[g_nLoginQueueTail].authid = authid;
	login_queue[g_nLoginQueueTail].command = USERCMDC_LOGIN;
	sprintf( login_queue[g_nLoginQueueTail].data, "&v1=%s&v2=%s&v3=%d&v4=%s&v5=%s", szUserName, HttpString( szPassWord, tmpstr ), client_getplatid( client_index ), client_getip( client_index ), szDeviceID );
	g_nLoginQueueTail = queue_tail;
	mmux_unlock( g_login_mmux );
	// 通知登录线程干活
	mcond_broadcast( g_pthr_login );
	return 0;
}

// 用户服务器返回登陆结果
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

	// 返回用户名
	Value.m_username_length = (int)strlen( pClientInfo->username ) + 1;
	strcpy( Value.m_username, pClientInfo->username );

	// 登陆失败
	if ( pClientInfo->result < 0  )
	{
		netsend_login_S( client_index, SENDTYPE_ACTOR, &Value );
		return -1;
	}

	// 登陆成功
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

// 发送兑换奖励信息
int user_award( int client_index, char *cardnumber )
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

	// 锁住写队列缓冲
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
		// 通知登录线程得赶紧干活, 都满了啊
		mcond_broadcast( g_pthr_login );
		return -1;
	}
	// 将需要处理的客户放置到登录队列
	login_queue[g_nLoginQueueTail].client_index = client_index;
	login_queue[g_nLoginQueueTail].authid = authid;
	login_queue[g_nLoginQueueTail].command = USERCMDC_AWARD;

	sprintf( login_queue[g_nLoginQueueTail].data, "&v1=%d&v2=%d&v3=%s&v4=%s&v5=%s&v6=%s&v7=%d&v8=%d", g_actors[client_index].actorid, g_Config.server_code, szUserID, client_getip( client_index ), HttpString(cardnumber, tmpstr ), cdkey, client_getos( client_index ), client_getchannelid( client_index ) );

	g_nLoginQueueTail = queue_tail;
	mmux_unlock( g_login_mmux );
	// 通知登录线程干活
	mcond_broadcast( g_pthr_login );
	return 0;
}

// 用户服务器返回奖励信息
int user_awarded( int client_index, int authid, int cdkey_index, int awardgroup, int result, char *cardnumber )
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
		// 记录cdkey
		actor_set_cdkey( client_index, cdkey_index );
		actor_notify( client_index, NOTIFY_TEXTTABLE, "7504" );
	}
	else if ( result == -3 )
	{
		actor_notify( client_index, NOTIFY_TEXTTABLE, "7498" ); // 您已经使用过该批次兑换码了
	}
	else if ( result == -4 )
	{
		actor_notify( client_index, NOTIFY_TEXTTABLE, "7499" ); // 这个兑换码已经被其他人使用过了
	}
	else if ( result == -5 )
	{
		actor_notify( client_index, NOTIFY_TEXTTABLE, "7500" ); // 这个兑换码只能在ios平台使用
	}
	else if ( result == -6 )
	{
		actor_notify( client_index, NOTIFY_TEXTTABLE, "7501" ); // 这个兑换码只能在android平台使用
	}
	else if ( result == -7 )
	{
		actor_notify( client_index, NOTIFY_TEXTTABLE, "7502" ); // 这个兑换码只能在指定渠道下载的游戏中使用
	}
	else if ( result == -8 )
	{
		actor_notify( client_index, NOTIFY_TEXTTABLE, "7503" ); // 该兑换码已经过期
	}
	else
	{
		actor_notify( client_index, NOTIFY_TEXTTABLE, "7497" ); // 兑换码无效
	}

	return 0;
}

// 用户锁定账号
int user_lock( int client_index, int authid, i64 player_userid, int lockmin )
{
	char *ptr;
	int queue_tail;

	if( client_index < 0 )
		player_userid = actor_getoffline_userid( authid );

	if( player_userid < 0 )
		return -1;

	// 锁住写队列缓冲
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
		// 通知登录线程得赶紧干活, 都满了啊
		mcond_broadcast( g_pthr_login );
		return -1;
	}
	// 将需要处理的客户放置到登录队列
	login_queue[g_nLoginQueueTail].client_index = client_index;
	login_queue[g_nLoginQueueTail].authid = authid;
	login_queue[g_nLoginQueueTail].command = USERCMDC_LOCKUSER;
	ptr = login_queue[g_nLoginQueueTail].data;

	char tmpBuf[MAX_PATH];
	sprintf( login_queue[g_nLoginQueueTail].data, "&v1=%s&v2=%d", lltoa(player_userid,tmpBuf,10), lockmin );

	g_nLoginQueueTail = queue_tail;
	mmux_unlock( g_login_mmux );
	// 通知登录线程干活
	mcond_broadcast( g_pthr_login );
	return 0;
}

// 用户改变这个服务器名，国王名，国王国家
int user_changesev( int client_index, short serverid, char *sevname, char *kingname, char *kingclub, short king_country )
{
	if ( client_index < 0 || client_index >= g_maxactornum )
		return -1;
	int authid = server_getautuid( client_index );
	if ( authid < 0 )
		return -1;

	char szSevName[NAME_SIZE] = { 0 };
	char szKingName[NAME_SIZE] = { 0 };
	char szKingClubName[NAME_SIZE] = { 0 };

	if ( sevname )
		strncpy( szSevName, sevname, NAME_SIZE );
	
	if ( kingname )
		strncpy( szKingName, kingname, NAME_SIZE );

	if ( kingclub )
		strncpy( szKingClubName, kingclub, NAME_SIZE );

	int queue_tail;

	// 锁住写队列缓冲
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
		// 通知登录线程得赶紧干活, 都满了啊
		mcond_broadcast( g_pthr_login );
		return -1;
	}
	// 将需要处理的客户放置到登录队列
	login_queue[g_nLoginQueueTail].client_index = client_index;
	login_queue[g_nLoginQueueTail].authid = authid;
	login_queue[g_nLoginQueueTail].command = USERCMDC_CHANGESEV;

	sprintf( login_queue[g_nLoginQueueTail].data, "&v1=%d&v2=%s&v3=%s&v4=%s&v5=%d", serverid, szSevName, szKingName, szKingClubName, king_country );

	g_nLoginQueueTail = queue_tail;
	mmux_unlock( g_login_mmux );
	// 通知登录线程干活
	mcond_broadcast( g_pthr_login );
	return 0;
}

// 修改夏日活动排行榜数据
int user_change_summerrank( int client_index, short serverid, int actorid, int cityid, char *name, int headid, short countryid, int flower )
{
	return 0;
}

// 发送排队信息，暂时无用
int user_queue_logic()
{
	int tmpi;

	// 锁住写队列缓冲
	mmux_lock( g_login_mmux );
	
	int nTail = g_nLoginQueueTail;
	int nHead = g_nLoginQueueHead;

	if ( nTail != nHead )
	{
		int queuecount = 0;
		if ( nTail > nHead )
		{
			queuecount = nTail - nHead;
		}
		else
		{
			queuecount = MAX_LOGINQUEUENUM - nHead + nTail;
		}
		// 将需要处理的客户放置到登录队列
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
	
	// 解锁队列缓冲
	mmux_unlock( g_login_mmux );
	return 0;
}

// 清空登陆队列
void login_queue_clear()
{
	// 锁住写队列缓冲
	mmux_lock( g_login_mmux );

	g_nLoginQueueHead = 0;
	g_nLoginQueueTail = 0;
	memset( login_queue, 0, sizeof(SLoginQueue)*MAX_LOGINQUEUENUM );
	for ( int tmpi = 0; tmpi < MAX_LOGINQUEUENUM; tmpi++ )
	{
		login_queue[tmpi].client_index = -1;
	}
	// 解锁队列缓冲
	mmux_unlock( g_login_mmux );
}
