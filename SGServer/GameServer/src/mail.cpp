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

extern MYSQL *myGame;
extern SConfig g_Config;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;
extern City *g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;
extern int g_awardgroup_count;

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

	// 插入数据库
	sprintf( out, "INSERT INTO mail ( actorid, type, title, content, attach, recvtime, deltime, fightid ) VALUES ('%d','%d','%s','%s','%s','%d','%d','%s')",
		 actorid, type, szTitle, szContent, szAttach, thistime, deltime, bigint );
	return 0;
}

// 发送邮件
i64 mail( int actor_index, int actorid, char type, char *title, char *content, char *attach, i64 fightid )
{
	if ( title == NULL || content == NULL )
		return -1;
	i64 mailid;
	int title_len = 0;
	int content_len = 0;
	int attach_len = 0;
	char szSQL[4096] = { 0 };
	mail_make( actorid, type, title, &title_len, content, &content_len, attach, &attach_len, fightid, szSQL );
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
	{ // 通知玩家有新邮件
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
		pValue.m_recvtime = (int)time(NULL);
		pValue.m_fightid = fightid;

		netsend_mail_S( actor_index, SENDTYPE_ACTOR, &pValue );

		// 通知未读数量
		g_actors[actor_index].mail_notreadnum += 1;
		if ( g_actors[actor_index].mail_notreadnum > 0 )
		{
			int value[2] = { 0 };
			value[0] = 1;
			value[1] = g_actors[actor_index].mail_notreadnum;
			actor_notify_value( actor_index, NOTIFY_MAIL, 2, value, NULL );
		}
	}
	return mailid;
}

// 读取详细内容
int mail_read_content( int actor_index, i64 mailid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[256];
	char bigint[21];
	lltoa( mailid, bigint, 10 );
	sprintf( szSQL, "select content, attach from mail where id='%s'", bigint );
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

	}
	else
	{
		// 发送一个已经不存在的消息
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );
	mail_readed( actor_index, mailid );
	return 0;
}

// 设置已读状态
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
	if ( g_actors[actor_index].mail_notreadnum > 0 )
	{
		int value[2] = { 0 };
		value[0] = 1;
		value[1] = g_actors[actor_index].mail_notreadnum;
		actor_notify_value( actor_index, NOTIFY_MAIL, 2, value, NULL );
	}
	return 0;
}

// 删除邮件
int mail_delete( i64 mailid )
{
	char szSQL[256];
	char bigint[21];
	lltoa( mailid, bigint, 10 );
	sprintf( szSQL, "DELETE FROM `mail` WHERE id='%s'", bigint );
	dbwork_addsql( szSQL, DBWORK_CMD_MAIL_DELETE, 0 );

	sprintf( szSQL, "DELETE FROM `mail_fightdetail` WHERE id='%s'", bigint );
	dbwork_addsql( szSQL, DBWORK_CMD_MAIL_DELETE, 0 );
	return 0;
}

// 未读邮件数量
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
	int actorid = g_actors[actor_index].actorid;

	// 读取未读数量
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

	// 读取最大ID和最小ID
	sprintf( szSQL, "select min(id), max(id) from mail where `actorid`='%d' and deltime>'%d'", g_actors[actor_index].actorid, (int)time( NULL ) );
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
		minid = atoll( row[0] );
		maxid = atoll( row[1] );
	}

	// 发送未读数量，最大和最小的邮件ID
	g_actors[actor_index].mail_notreadnum = (short)count;
	if ( g_actors[actor_index].mail_notreadnum > 0 )
	{
		int value[5] = { 0 };
		value[0] = 1;
		value[1] = g_actors[actor_index].mail_notreadnum;
		value[2] = maxid;
		value[3] = minid;
		actor_notify_value( actor_index, NOTIFY_MAIL, 5, value, NULL );
	}
	mysql_free_result( res );
	return 0;
}

// 获取邮件
int mail_getlist( int actor_index, int op, unsigned int min, unsigned int max )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[256];
	int count = 0;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	if ( op == 0 )
	{
		if ( min == 0 )
		{
			// 获取最新的6条
			sprintf( szSQL, "select `id`, `type`, `title`, `content`, `attach`, `attachget`, `recvtime`, `read`, `fightid` from `mail` where `actorid`='%d' and `deltime`>'%d' order by `id` desc limit 6;", g_actors[actor_index].actorid, (int)time( NULL ) );
		}
		else
		{ // 获取小于min的6条
			sprintf( szSQL, "select `id`, `type`, `title`, `content`, `attach`, `attachget`, `recvtime`, `read`, `fightid` from `mail` where `id`<'%d' and `actorid`='%d' and `deltime`>'%d' order by `id` desc limit 6;", min, g_actors[actor_index].actorid, (int)time( NULL ) );
		}
	}
	// 获取之前的6条
	else if( op == 1 )
	{
		sprintf( szSQL, "select `id`, `type`, `title`, `content`, `attach`, `attachget`, `recvtime`, `read`, `fightid` from `mail` where `id`<'%d' and `actorid`='%d' and `deltime`>'%d' order by `id` desc limit 6;", min, g_actors[actor_index].actorid, (int)time( NULL ) );
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
		pValue.m_mailid = (unsigned int)atoll( row[0] );
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
		pValue.m_fightid = (unsigned int)atoll( row[8] );

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
