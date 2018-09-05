#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "netserver.h"
#ifndef WIN32
#include <sys/time.h>
#endif
#include "db.h"
#include "gameproc.h"
#include "actor.h"
#include "mail.h"
#include "utils.h"
#include "define.h"
#include "actor_send.h"
#include "script.h"
#include "server_netsend_auto.h"
#include "actor_notify.h"
#include "city.h"
#include "item.h"
#include "award.h"
#include "army.h"
#include "mapunit.h"
#include "system.h"
#include "global.h"
#include "chat.h"

extern MYSQL *myGame;
extern SConfig g_Config;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;
extern City *g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;
extern int g_awardgroup_count;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;

char g_szFightSql[65535] = { 0 };
char g_szFightJson[FIGHT_JSON_MAX] = { 0 };

SLK_NetS_MailView g_MailSearchCache[MAIL_SEARCHCACHE_MAX];
MailFightSearchCache g_MailFightSearchCache[MAIL_SEARCHCACHE_MAX];

int mail_make( int actorid, char type, char *title, int *title_len, char *content, int *content_len, char *attach, int *attach_len, i64 fightid, char *out )
{
	if ( actorid < MINACTORID )
		return -1;
	char szTitle[MAIL_TITLE_MAXSIZE * 2 + 1] = { 0 };
	char szContent[MAIL_CONTENT_MAXSIZE * 2 + 1] = { 0 };
	char szAttach[MAIL_ATTACH_MAXSIZE * 2 + 1] = { 0 };
	int thistime = (int)time( NULL );
	int deltime = thistime + MAX_MAIL_SAVETIME;
	if ( title )
	{
		int len = strlen( title );
		if ( len >= MAIL_TITLE_MAXSIZE )
			return -1;
		*title_len = len;
		db_escape( title, szTitle, 0 );
	}
	if ( content )
	{
		int len = strlen( content );
		if ( len >= MAIL_CONTENT_MAXSIZE )
			return -1;
		*content_len = len;
		db_escape( content, szContent, 0 );
	}
	if ( attach )
	{
		int len = strlen( attach );
		if ( len >= MAIL_ATTACH_MAXSIZE )
			return -1;
		*attach_len = len;
		db_escape( attach, szAttach, 0 );
	}

	char bigint[21];
	lltoa( fightid, bigint, 10 );

	// �������ݿ�
	sprintf( out, "INSERT INTO mail ( actorid, type, title, content, attach, recvtime, deltime, fightid ) VALUES ('%d','%d','%s','%s','%s','%d','%d','%s')",
		 actorid, type, szTitle, szContent, szAttach, thistime, deltime, bigint );
	return 0;
}

// �����ʼ�
i64 mail( int actor_index, int actorid, char type, char *title, char *content, char *attach, i64 fightid, char queue )
{
	if ( title == NULL || content == NULL )
		return -1;
	i64 mailid;
	int title_len = 0;
	int content_len = 0;
	int attach_len = 0;
	char szSQL[4096] = { 0 };
	mail_make( actorid, type, title, &title_len, content, &content_len, attach, &attach_len, fightid, szSQL );

	if ( queue == 1 )
	{
		dbwork_addsql( szSQL, DBWORK_CMD_MAIL_INSERT, 0 );
		if ( actor_index >= 0 && actor_index < g_maxactornum )
		{
			// ֪ͨδ������
			g_actors[actor_index].mail_notreadnum += 1;
			mail_noread_send( actor_index );
		}
		return 0;
	}

	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	mailid = mysql_insert_id( myGame );
	if ( actor_index == MAIL_ACTORID )
	{
		actor_index = actor_getindex_withid( actorid );
	}
	
	if ( actor_index >= 0 && actor_index < g_maxactornum )
	{ // ֪ͨ��������ʼ�
		SLK_NetS_Mail pValue = { 0 };
		pValue.m_mailid = mailid;
		pValue.m_type = type;

		pValue.m_title_len = title_len;
		strncpy( pValue.m_title, title, MAIL_TITLE_MAXSIZE );

		pValue.m_content_len = content_len;
		strncpy( pValue.m_content, content, MAIL_CONTENT_MAXSIZE );

		pValue.m_attach_len = attach_len;
		strncpy( pValue.m_attach, attach, MAIL_ATTACH_MAXSIZE );

		pValue.m_attachget = 0;
		pValue.m_read = 0;
		pValue.m_lock = 0;
		pValue.m_recvtime = (int)time(NULL);
		pValue.m_fightid = fightid;
		pValue.m_actorid = actorid;

		netsend_mail_S( actor_index, SENDTYPE_ACTOR, &pValue );

		// ֪ͨδ������
		g_actors[actor_index].mail_notreadnum += 1;
		mail_noread_send( actor_index );
	}
	return mailid;
}

// ϵͳ�ʼ�
i64 mail_system( int actor_index, int actorid, int titleid, int contentid, char *attach, char queue )
{
	char title[64] = { 0 };
	sprintf( title, "%s%d", TAG_TEXTID, titleid );

	char content[128] = { 0 };
	sprintf( content, "{\"text\":\"%s%d\"}", TAG_TEXTID, contentid );

	i64 mailid;
	mailid = mail( actor_index, actorid, MAIL_TYPE_SYSTEM, title, content, attach, 0, queue );
	return mailid;
}

// ϵͳ�ʼ�
i64 mail_system( int actor_index, int actorid, int titleid, int contentid, AwardGetInfo *getinfo, char queue )
{
	char title[64] = { 0 };
	sprintf( title, "%s%d", TAG_TEXTID, titleid );

	char content[128] = { 0 };
	sprintf( content, "{\"text\":\"%s%d\"}", TAG_TEXTID, contentid );

	char attach[256] = { 0 };
	int count = getinfo->count;
	if ( count > 16 )
		count = 16; // ��ʱ������16������
	if ( getinfo && getinfo->count > 0 )
	{
		for ( int tmpi = 0; tmpi < count; tmpi++ )
		{
			if ( getinfo->kind[tmpi] <= 0 )
				continue;
			char tempitem[32] = { 0 };
			sprintf( tempitem, "%d,%d@", getinfo->kind[tmpi], getinfo->num[tmpi] );
			strcat( attach, tempitem );
		}
	}

	i64 mailid;
	mailid = mail( actor_index, actorid, MAIL_TYPE_SYSTEM, title, content, attach, 0, queue );
	return mailid;
}

// ϵͳ�ʼ�
i64 mail_system( int actor_index, int actorid, int titleid, int contentid, int awardgroup, char queue )
{
	char title[64] = { 0 };
	sprintf( title, "%s%d", TAG_TEXTID, titleid );

	char content[128] = { 0 };
	sprintf( content, "{\"text\":\"%s%d\"}", TAG_TEXTID, contentid );

	char attach[256] = { 0 };
	if ( awardgroup > 0 && awardgroup < g_awardgroup_count )
	{
		awardgroup_mail( awardgroup, 0, attach );
	}

	i64 mailid;
	mailid = mail( actor_index, actorid, MAIL_TYPE_SYSTEM, title, content, attach, 0, queue );
	return mailid;
}

// ϵͳ�ʼ�
i64 mail_system( int actor_index, int actorid, int titleid, int contentid, char *v1, char *v2, char *v3, char *attach, char queue )
{
	char title[64] = { 0 };
	sprintf( title, "%s%d", TAG_TEXTID, titleid );

	char content[256] = { 0 };
	if ( v1 && v2 && v3 )
	{
		sprintf( content, "{\"text\":\"%s%d\",\"v1\":\"%s\",\"v2\":\"%s\",\"v3\":\"%s\"}", TAG_TEXTID, contentid, v1, v2, v3 );
	}
	else if ( v1 && v2 )
	{
		sprintf( content, "{\"text\":\"%s%d\",\"v1\":\"%s\",\"v2\":\"%s\"}", TAG_TEXTID, contentid, v1, v2 );
	}
	else if ( v1 )
	{
		sprintf( content, "{\"text\":\"%s%d\",\"v1\":\"%s\"}", TAG_TEXTID, contentid, v1 );
	}
	else
	{
		sprintf( content, "{\"text\":\"%s%d\"}", TAG_TEXTID, contentid );
	}

	i64 mailid = 0;
	mailid = mail( actor_index, actorid, MAIL_TYPE_SYSTEM, title, content, attach, 0, queue );
	return mailid;
}

// �ʼ�������ϵķ���
int mailqueue_insert_complete()
{
	// ���ռ������ߣ�֪ͨ���µ���Ϣ
	for ( int tmpi = 0; tmpi < g_maxactornum; tmpi++ )
	{
		if ( g_actors[tmpi].actorid <= 0 )
			continue;
		int value[2] = { 0 };
		value[0] = 3;
		actor_notify_value( tmpi, NOTIFY_MAIL, 2, value, NULL );
	}
	return 0;
}

// ս�������ʼ�
int mail_fight( i64 mailid, int actorid, char *json )
{
	char bigint[21];
	lltoa( mailid, bigint, 10 );
	if ( json )
		db_escape( json, g_szFightJson, 0 );
	sprintf( g_szFightSql, "INSERT INTO mail_fight ( id, actorid, content, recvtime ) VALUES ('%s','%d','%s','%d')",
		bigint, actorid, g_szFightJson, (int)time( NULL ) );
	if ( mysql_query( myGame, g_szFightSql ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", g_szFightSql );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	return 0;
}

// ��ȡս����ϸ����
int mail_fight_read( int actor_index, i64 mailid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[256];
	char bigint[21];
	lltoa( mailid, bigint, 10 );
	sprintf( szSQL, "select content from mail_fight where id='%s'", bigint );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( (row = mysql_fetch_row( res )) )
	{
		mail_fight_send( actor_index, mailid, row[0] );
	}
	else
	{
		// ����һ���Ѿ������ڵ���Ϣ
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );
	return 0;
}

// ����ս����ϸ����
int mail_fight_send( int actor_index, i64 mailid, char *content )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( !content )
		return -1;
	SLK_NetS_MailFight pValue = { 0 };

	// ���Ϳ�ʼ
	pValue.m_flag = 0;
	pValue.m_content_length = 0;
	netsend_mailfight_S( actor_index, SENDTYPE_ACTOR, &pValue );

	// �����ݹ���
	pValue.m_flag = 1;
	int total_length = strlen( content );
	int offset = 0;
	while ( total_length > 0 )
	{
		int length = strlen( content + offset );
		if ( length > 1600 )
			length = 1600;
		pValue.m_content_length = length;
		memcpy( pValue.m_content, content + offset, sizeof( char )*length );
		offset += length;
		total_length -= length;
		netsend_mailfight_S( actor_index, SENDTYPE_ACTOR, &pValue );
	}

	// �������
	pValue.m_flag = 2;
	pValue.m_content_length = 0;
	pValue.m_mailid = mailid;
	netsend_mailfight_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// �����Ѷ�״̬
int mail_readed( int actor_index, i64 mailid )
{
	char szSQL[256];
	char bigint[21];
	lltoa( mailid, bigint, 10 );
	sprintf( szSQL, "update mail set `read`='%d' where `id`='%s'", 1, bigint );
	dbwork_addsql( szSQL, DBWORK_CMD_MAIL_UPDATE, 0 );

	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	g_actors[actor_index].mail_notreadnum -= 1;
	if ( g_actors[actor_index].mail_notreadnum < 0 )
		g_actors[actor_index].mail_notreadnum = 0;
	mail_noread_send( actor_index );
	return 0;
}

// ����ȫ���Ѷ�״̬
int mail_readed_all( int actor_index )
{
	char szSQL[256];
	char bigint[21];
	sprintf( szSQL, "update mail set `read`='1' where `actorid`='%d'", g_actors[actor_index].actorid );
	dbwork_addsql( szSQL, DBWORK_CMD_MAIL_UPDATE, 0 );

	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	g_actors[actor_index].mail_notreadnum = 0;
	mail_noread_send( actor_index );
	return 0;
}

// ɾ���ʼ�
int mail_delete( i64 mailid )
{
	char szSQL[256];
	char bigint[21];
	lltoa( mailid, bigint, 10 );
	sprintf( szSQL, "DELETE FROM `mail` WHERE id='%s'", bigint );
	dbwork_addsql( szSQL, DBWORK_CMD_MAIL_DELETE, 0 );

	sprintf( szSQL, "DELETE FROM `mail_fight` WHERE id='%s'", bigint );
	dbwork_addsql( szSQL, DBWORK_CMD_MAIL_DELETE, 0 );
	return 0;
}

// ɾ���ʼ�
int mail_delete_actor( int actor_index, i64 mailid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( mailid <= 0 )
		return -1;
	char szSQL[256];
	char bigint[21];
	lltoa( mailid, bigint, 10 );
	sprintf( szSQL, "DELETE FROM `mail` WHERE id='%s' and `actorid`='%d'", bigint, g_actors[actor_index].actorid );
	dbwork_addsql( szSQL, DBWORK_CMD_MAIL_DELETE, 0 );

	sprintf( szSQL, "DELETE FROM `mail_fight` WHERE id='%s' and `actorid`='%d'", bigint, g_actors[actor_index].actorid );
	dbwork_addsql( szSQL, DBWORK_CMD_MAIL_DELETE, 0 );
	return 0;
}

// ����ɾ���ʼ�
int mail_delete_all( int actor_index, SLK_NetC_MailAllDel *pValue )
{
	for ( int tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		mail_delete_actor( actor_index, pValue->m_mailid[tmpi] );
	}
	return 0;
}

// ��������״̬
int mail_locked( int actor_index, i64 mailid )
{
	char szSQL[256];
	char bigint[21];
	lltoa( mailid, bigint, 10 );
	sprintf( szSQL, "update mail set `lock`='1' where `id`='%s'", bigint );
	dbwork_addsql( szSQL, DBWORK_CMD_MAIL_UPDATE, 0 );
	return 0;
}

// ���ý������״̬
int mail_unlocked( int actor_index, i64 mailid )
{
	char szSQL[256];
	char bigint[21];
	lltoa( mailid, bigint, 10 );
	sprintf( szSQL, "update mail set `lock`='0' where `id`='%s'", bigint );
	dbwork_addsql( szSQL, DBWORK_CMD_MAIL_UPDATE, 0 );
	return 0;
}

// δ���ʼ�����
int mail_noread_check( int actor_index )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[256];
	int count = 0;
	i64 minid = 0;
	i64 maxid = 0;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	// ��ȡδ������
	sprintf( szSQL, "select count(*) from mail where `actorid`='%d' and `read`=0 and deltime>'%d'", g_actors[actor_index].actorid, (int)time( NULL ) );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( (row = mysql_fetch_row( res )) )
	{
		count = atoi( row[0] );
	}
	mysql_free_result( res );

	//// ��ȡ���ID����СID
	//sprintf( szSQL, "select min(id), max(id) from mail where `actorid`='%d' and deltime>'%d'", g_actors[actor_index].actorid, (int)time( NULL ) );
	//if ( mysql_query( myGame, szSQL ) )
	//{
	//	printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
	//	write_gamelog( "%s", szSQL );
	//	if ( mysql_ping( myGame ) != 0 )
	//		db_reconnect_game();
	//	return -1;
	//}
	//res = mysql_store_result( myGame );
	//if ( (row = mysql_fetch_row( res )) )
	//{
	//	minid = atoll( row[0] );
	//	maxid = atoll( row[1] );
	//}

	// ����δ��������������С���ʼ�ID
	g_actors[actor_index].mail_notreadnum = (short)count;
	mail_noread_send( actor_index );
	//mysql_free_result( res );
	return 0;
}

// ����δ���ʼ�����
int mail_noread_send( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	int value[2] = { 0 };
	value[0] = 1;
	value[1] = g_actors[actor_index].mail_notreadnum;
	actor_notify_value( actor_index, NOTIFY_MAIL, 2, value, NULL );
	return 0;
}


// ��ȡ�ʼ�
int mail_getlist( int actor_index, int op, i64 min, i64 max )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[256];
	int count = 0;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	char bigint[21];
	lltoa( min, bigint, 10 );

	if ( op == 0 )
	{
		if ( min == 0 )
		{
			// ��ȡ���µ�6��
			sprintf( szSQL, "select `id`, `type`, `title`, `content`, `attach`, `attachget`, `recvtime`, `read`, `lock`, `fightid` from `mail` where `actorid`='%d' and `deltime`>'%d' order by `id` desc limit 6;", g_actors[actor_index].actorid, (int)time( NULL ) );
		}
		else
		{ // ��ȡС��min��6��
			sprintf( szSQL, "select `id`, `type`, `title`, `content`, `attach`, `attachget`, `recvtime`, `read`, `lock`, `fightid` from `mail` where `id`<'%s' and `actorid`='%d' and `deltime`>'%d' order by `id` desc limit 6;", bigint, g_actors[actor_index].actorid, (int)time( NULL ) );
		}
	}
	// ��ȡ֮ǰ��6��
	else if( op == 1 )
	{
		sprintf( szSQL, "select `id`, `type`, `title`, `content`, `attach`, `attachget`, `recvtime`, `read`, `lock`, `fightid` from `mail` where `id`<'%s' and `actorid`='%d' and `deltime`>'%d' order by `id` desc limit 6;", bigint, g_actors[actor_index].actorid, (int)time( NULL ) );
	}
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( (row = mysql_fetch_row( res )) )
	{
		SLK_NetS_Mail pValue = { 0 };
		pValue.m_mailid = atoll( row[0] );
		pValue.m_type = atoi( row[1] );

		strncpy( pValue.m_title, row[2], MAIL_TITLE_MAXSIZE );
		pValue.m_title_len = strlen( pValue.m_title );

		strncpy( pValue.m_content, row[3], MAIL_CONTENT_MAXSIZE );
		pValue.m_content_len = strlen( pValue.m_content );

		strncpy( pValue.m_attach, row[4], MAIL_ATTACH_MAXSIZE );
		pValue.m_attach_len = strlen( pValue.m_attach );
		
		pValue.m_attachget = atoi( row[5] );
		pValue.m_recvtime = atoi( row[6] );
		pValue.m_read = atoi( row[7] );
		pValue.m_lock = atoi( row[8] );
		pValue.m_fightid = atoll( row[9] );
		pValue.m_actorid = g_actors[actor_index].actorid;
		netsend_mail_S( actor_index, SENDTYPE_ACTOR, &pValue );
		count += 1;
	}

	int value[3] = { 0 };
	value[0] = 2;
	value[1] = op;
	value[2] = count;
	actor_notify_value( actor_index, NOTIFY_MAIL, 3, value, NULL );
	mysql_free_result( res );
	return 0;
}

// ��ȡ����
int mail_attachget( int actor_index, i64 mailid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[1024];
	char attach[MAIL_ATTACH_MAXSIZE] = { 0 };
	char bigint[21];
	lltoa( mailid, bigint, 10 );
	sprintf( szSQL, "select attach from mail where id='%s' and attachget=0", bigint );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( (row = mysql_fetch_row( res )) )
	{
		memcpy( attach, row[0], MAIL_ATTACH_MAXSIZE - 1 );
	}
	else
	{
		//�ʼ��Ѿ���ȡ
		SLK_NetS_MailOpResult info = { 0 };
		info.m_op = 2;
		info.m_mailid = mailid;
		netsend_mailopresult_S( actor_index, SENDTYPE_ACTOR, &info );
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	if ( sc_OnMailReadAttach( actor_index, attach ) < 0 )
	{
		return -1;
	}
	else
	{
		// ��Ϊ�Ѿ���ȡ״̬
		char szSQL[256];
		char bigint[21];
		lltoa( mailid, bigint, 10 );
		sprintf( szSQL, "update `mail` set `attachget`='1' where id='%s'", bigint );
		dbwork_addsql( szSQL, DBWORK_CMD_MAIL_UPDATE, 0 );
	}
	return 0;
}

// �����ʼ�
int mail_share( int actor_index, SLK_NetS_MailShare *pValue )
{
	char msg[127] = { 0 };
	char bigint[21];
	lltoa( pValue->m_mailid, bigint, 10 );

	if ( pValue->m_type == MAIL_TYPE_FIGHT_ENEMY || pValue->m_type == MAIL_TYPE_FIGHT_CITY || pValue->m_type == MAIL_TYPE_FIGHT_NATION || pValue->m_type == MAIL_TYPE_FIGHT_NATIONHERO || pValue->m_type == MAIL_TYPE_GATHER_FIGHT || pValue->m_type == MAIL_TYPE_FIGHT_ACTIVITY || pValue->m_type == MAIL_TYPE_FIGHT_ACTIVITY12 )
	{
		snprintf( msg, 127, "{\"mailid\":\"%s\",\"aname\":\"%s\",\"dname\":\"%s\"}", bigint, pValue->m_a_name, pValue->m_d_name );
		chat_actortalk( actor_index, CHAT_CHANNEL_NATION, CHAT_MSGTYPE_VS, msg );
	}
	else if ( pValue->m_type == MAIL_TYPE_CITY_SPY )
	{
		snprintf( msg, 127, "{\"mailid\":\"%s\",\"aname\":\"%s\",\"dname\":\"%s\"}", bigint, pValue->m_a_name, pValue->m_d_name );
		chat_actortalk( actor_index, CHAT_CHANNEL_NATION, CHAT_MSGTYPE_SPY, msg );
	}
	return 0;
}

// ��ҵ��������ʼ���ȡ����
int mail_share_getcontent( int actor_index, i64 mailid )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[256];
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	// ���ڻ�������
	for ( int tmpi = MAIL_SEARCHCACHE_MAX - 1; tmpi >= 0; tmpi-- )
	{
		if ( g_MailSearchCache[tmpi].m_mail.m_mailid == mailid )
		{
			netsend_mailview_S( actor_index, SENDTYPE_ACTOR, &g_MailSearchCache[tmpi] );
			return 0;
		}
	}

	// ����û�У������ݿ�
	char bigint[21];
	lltoa( mailid, bigint, 10 );
	sprintf( szSQL, "select `id`,`actorid`,`type`,`title`,`content`,`attach`,`attachget`,`recvtime`,`read`,`lock`,`fightid` from `mail` where `id`='%s';", bigint );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( (row = mysql_fetch_row( res )) )
	{
		SLK_NetS_MailView pValue = { 0 };
		pValue.m_mail.m_mailid = atoll( row[0] );
		pValue.m_mail.m_actorid = atoi( row[1] );
		pValue.m_mail.m_type = atoi( row[2] );

		strncpy( pValue.m_mail.m_title, row[3], MAIL_TITLE_MAXSIZE );
		pValue.m_mail.m_title_len = strlen( pValue.m_mail.m_title );

		strncpy( pValue.m_mail.m_content, row[4], MAIL_CONTENT_MAXSIZE );
		pValue.m_mail.m_content_len = strlen( pValue.m_mail.m_content );

		strncpy( pValue.m_mail.m_attach, row[5], MAIL_ATTACH_MAXSIZE );
		pValue.m_mail.m_attach_len = strlen( pValue.m_mail.m_attach );

		pValue.m_mail.m_attachget = atoi( row[6] );
		pValue.m_mail.m_recvtime = atoi( row[7] );
		pValue.m_mail.m_read = atoi( row[8] );
		pValue.m_mail.m_lock = atoi( row[9] );
		pValue.m_mail.m_fightid = atoll( row[10] );
		pValue.m_mail.m_viewpath = 1;
		netsend_mailview_S( actor_index, SENDTYPE_ACTOR, &pValue );

		// �ӵ�����
		int tmpi = 0;
		for ( tmpi = 0; tmpi < MAIL_SEARCHCACHE_MAX; tmpi++ )
		{
			if ( g_MailSearchCache[tmpi].m_mail.m_actorid <= 0 )
			{
				memcpy( &g_MailSearchCache[tmpi], &pValue, sizeof( SLK_NetS_MailView ) );
				break;
			}
		}
		if ( tmpi >= MAIL_SEARCHCACHE_MAX )
		{
			memmove( &g_MailSearchCache[0], &g_MailSearchCache[1], sizeof( SLK_NetS_MailView )*(MAIL_SEARCHCACHE_MAX - 1) );
			memcpy( &g_MailSearchCache[MAIL_SEARCHCACHE_MAX - 1], &pValue, sizeof( SLK_NetS_MailView ) );
		}
	}
	else
	{
		// ����һ���Ѿ������ڵ���Ϣ
		actor_notify_pop( actor_index, 1176 );
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );
	return 0;
}

// ��ҵ��������ʼ���ȡ����
int mail_share_getfightcontent( int actor_index, i64 mailid )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[256];
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	// ���ڻ�������
	for ( int tmpi = MAIL_SEARCHCACHE_MAX - 1; tmpi >= 0; tmpi-- )
	{
		if ( g_MailFightSearchCache[tmpi].m_mailid == mailid )
		{
			mail_fight_send( actor_index, mailid, g_MailFightSearchCache[tmpi].m_content );
			return 0;
		}
	}

	// ����û�У������ݿ�
	char bigint[21];
	lltoa( mailid, bigint, 10 );
	sprintf( szSQL, "select content from mail_fight where id='%s'", bigint );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( (row = mysql_fetch_row( res )) )
	{
		mail_fight_send( actor_index, mailid, row[0] );

		// �ӵ�����
		int tmpi = 0;
		for ( tmpi = 0; tmpi < MAIL_SEARCHCACHE_MAX; tmpi++ )
		{
			if ( g_MailFightSearchCache[tmpi].m_mailid <= 0 )
			{
				MailFightSearchCache cache = { 0 };
				cache.m_mailid = mailid;
				strncpy( cache.m_content, row[0], FIGHT_JSON_MAX - 1 );
				memcpy( &g_MailFightSearchCache[tmpi], &cache, sizeof( MailFightSearchCache ) );
				break;
			}
		}
		if ( tmpi >= MAIL_SEARCHCACHE_MAX )
		{
			memmove( &g_MailFightSearchCache[0], &g_MailFightSearchCache[1], sizeof( MailFightSearchCache )*(MAIL_SEARCHCACHE_MAX - 1) );

			MailFightSearchCache cache = { 0 };
			cache.m_mailid = mailid;
			strncpy( cache.m_content, row[0], FIGHT_JSON_MAX - 1 );
			memcpy( &g_MailFightSearchCache[MAIL_SEARCHCACHE_MAX - 1], &cache, sizeof( MailFightSearchCache ) );
		}
	}
	else
	{
		// ����һ���Ѿ������ڵ���Ϣ
		actor_notify_pop( actor_index, 1176 );
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );
	return 0;
}

// ����ʼ����ͣ������ڻظ���
int mail_actor_send( int actor_index, SLK_NetC_MailSend *pValue )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	int actorid = pValue->m_actorid;
	if ( actorid <= 0 )
	{
		if ( pValue->m_unit_index < 0 || pValue->m_unit_index >= g_mapunit_maxcount )
			return -1;
		MapUnit *unit = &g_mapunit[pValue->m_unit_index];
		if ( !unit )
			return -1;
		if ( unit->type != MAPUNIT_TYPE_CITY )
			return -1;
		if ( unit->index < 0 || unit->index >= g_city_maxcount )
			return -1;
		actorid = g_city[unit->index].actorid;
	}
	if ( actorid <= 0 )
		return -1;

	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	char title[MAIL_TITLE_MAXSIZE] = { 0 };
	sprintf( title, "{\"n\":%d,\"lv\":%d,\"na\":\"%s\"}", pCity->nation, pCity->level, pCity->name );

	char content[MAIL_CONTENT_MAXSIZE] = { 0 };
	sprintf( content, "{\"fromid\":%d,\"msg\":\"%s\"}", pCity->actorid, pValue->m_content );

	mail( MAIL_ACTORID, actorid, MAIL_TYPE_ACTOR_SEND, title, content, "", 0, 0 );
	return 0;
}

// ��Ҽ��ʼ��ظ�
int mail_actor_reply( int actor_index, SLK_NetC_MailReply *pValue )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;

	int actorid = pValue->m_actorid;
	if ( actorid <= 0 )
		return -1;
	City *pTargetCity = city_getptr_withactorid( actorid );
	if ( !pTargetCity )
		return -1;

	char title[MAIL_TITLE_MAXSIZE] = { 0 };
	sprintf( title, "{\"n\":%d,\"lv\":%d,\"na\":\"%s\"}", pCity->nation, pCity->level, pCity->name );

	char content[MAIL_CONTENT_MAXSIZE] = { 0 };
	snprintf( content, MAIL_CONTENT_MAXSIZE, "{\"fromid\":%d,\"msg\":\"%s\",\"reply\":\"%s\",\"t\":%d,\"n\":%d,\"na\":\"%s\"}", pCity->actorid, pValue->m_content, pValue->m_reply, pValue->m_reply_recvtime, pTargetCity->nation, pTargetCity->name );

	mail( MAIL_ACTORID, actorid, MAIL_TYPE_ACTOR_REPLY, title, content, "", 0, 0 );
	return 0;
}

// �����гǳ��ʼ�
void mail_sendall( int titleid, int contentid, char *v1, char *v2, char *v3, char *attach )
{
	int currtime = (int)time( NULL );
	int offlinesec = 15 * 86400;
	for ( int tmpi = 0; tmpi < g_city_maxindex/*ע�⣺ʹ������λ�ã�Ϊ��Ч��*/; tmpi++ )
	{
		if ( g_city[tmpi].actorid <= 0 )
			continue;
		if ( g_city[tmpi].type == CityLairdType_Robot )
			continue;
		if ( (currtime - g_city[tmpi].lastlogin) >= offlinesec )
			continue;
		if ( g_city[tmpi].actor_index >= 0 && g_city[tmpi].actor_index < g_maxactornum )
		{
			mail_system( g_city[tmpi].actor_index, g_city[tmpi].actorid, titleid, contentid, v1, v2, v3, attach, 0 );
		}
		else
		{
			mail_system( g_city[tmpi].actor_index, g_city[tmpi].actorid, titleid, contentid, v1, v2, v3, attach, 1 );
		}
	}
	//dbwork_addsql( "", DBWORK_CMD_MAIL_INSERTCOMPLETE, 0 );
}

// �����гǳط��͹����ʼ�
void mail_notice( int titleid, int contentid, char *attach )
{
	char title[64] = { 0 };
	sprintf( title, "%s%d", TAG_TEXTID, titleid );

	char content[128] = { 0 };
	sprintf( content, "{\"text\":\"%s%d\"}", TAG_TEXTID, contentid );

	int currtime = (int)time( NULL );
	int offlinesec = 15 * 86400;
	for ( int tmpi = 0; tmpi < g_city_maxindex/*ע�⣺ʹ������λ�ã�Ϊ��Ч��*/; tmpi++ )
	{
		if ( g_city[tmpi].actorid <= 0 )
			continue;
		if ( g_city[tmpi].type == CityLairdType_Robot )
			continue;
		if ( (currtime - g_city[tmpi].lastlogin) >= offlinesec )
			continue;
		if ( g_city[tmpi].actor_index >= 0 && g_city[tmpi].actor_index < g_maxactornum )
		{
			mail( g_city[tmpi].actor_index, g_city[tmpi].actorid, MAIL_TYPE_NOTIFY, title, content, attach, 0, 0 );
		}
		else
		{
			mail( g_city[tmpi].actor_index, g_city[tmpi].actorid, MAIL_TYPE_NOTIFY, title, content, attach, 0, 1 );
		}
	}
}
