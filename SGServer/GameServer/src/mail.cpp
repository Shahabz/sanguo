#include <string.h>
#include <time.h>
#include <math.h>
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

int mail_make( int actorid, char type, char *title, char *content, char *attach, i64 fightid, char *out )
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
		db_escape( title, szTitle, 0 );
	}
	if ( content )
	{
		int len = strlen( content );
		if ( len >= MAIL_CONTENT_MAXSIZE )
			return -1;
		db_escape( content, szContent, 0 );
	}
	if ( attach )
	{
		int len = strlen( attach );
		if ( len >= MAIL_ATTACH_MAXSIZE )
			return -1;
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
	char szSQL[4096] = { 0 };
	mail_make( actorid, type, title, content, attach, fightid, szSQL );
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
	{
		
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
	mail_readed( mailid );
	return 0;
}

// 设置已读状态
int mail_readed( i64 mailid )
{
	char szSQL[256];
	char bigint[21];
	lltoa( mailid, bigint, 10 );
	sprintf( szSQL, "update mail set `read`='%d' where `id`='%s'", 1, bigint );
	dbwork_addsql( szSQL, DBWORK_CMD_MAIL_UPDATE, 0 );
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
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	int actorid = g_actors[actor_index].actorid;
	char bigint[21];
	lltoa( maxid, bigint, 10 );
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
		int count = atoi( row[0] );
		if ( count > 0 )
		{
			//int value[2] = { 0 };
			//value[0] = 2;
			//value[1] = count;
			//actor_notify_value( actor_index, NOTIFY_MAIL, 2, value, NULL );
		}
	}
	mysql_free_result( res );
	return 0;
}
