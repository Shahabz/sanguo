#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "actor_send.h"
#include "server_netsend_auto.h"
#include "actor_notify.h"
#include "system.h"
#include "actor_times.h"
#include "script.h"
#include "award.h"
#include "city.h"
#include "quest.h"
#include "global.h"
#include "hero.h"
#include "mail.h"

extern SConfig g_Config;
extern MYSQL *myData;
extern MYSQL *myGame;
extern Actor *g_actors;
extern int g_maxactornum;
extern Global global;

// ���ñ�־λ
void actor_set_sflag( int actor_index, int offset, char value )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return;
	if ( value == 0 )
	{
		g_actors[actor_index].sflag &= ~(1 << offset);
	}
	else
	{
		if ( g_actors[actor_index].sflag & (1 << offset) )
			return;
		g_actors[actor_index].sflag |= (1 << offset);
	}
	actor_notify_value( actor_index, NOTIFY_ACTORSFLAG, 1, &g_actors[actor_index].sflag, NULL );
}

// ��ȡ��־λֵ
int actor_get_sflag( int actor_index, int offset )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return 0;
	if ( g_actors[actor_index].sflag & (1 << offset) )
		return 1;
	return 0;
}

// ����Ƿ���Ҫˢ��
void _check_fday( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return;
	int thisday = system_getfday();
	if ( g_actors[actor_index].fdate != thisday )
	{
		g_actors[actor_index].fdate = thisday;
		memset( g_actors[actor_index].today_char, 0, MAX_TODAY_COUNT + 1 );
		memset( g_actors[actor_index].today_short, 0, sizeof( short )*MAX_TODAY_SHORT_COUNT );
		memset( g_actors[actor_index].today_int, 0, sizeof( int )*MAX_TODAY_INTNUM );
		actor_set_sflag( actor_index, ACTOR_SFLAG_BODY_FREEBUY, 0 );
		everyday_quest_reset( actor_index );
		hero_colorup_reset( actor_index );
		write_gamelog( "[reset fdate:%d] %d", g_actors[actor_index].fdate, g_actors[actor_index].actorid );
	}
}

// char ���͵�ÿ�����ݣ�ÿ����£�
int actor_get_today_char_times( int actor_index, int offset )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( offset >= MAX_TODAY_COUNT || offset < 0 )
		return -1;
	_check_fday( actor_index );
	return g_actors[actor_index].today_char[offset];
}
int actor_add_today_char_times( int actor_index, int offset )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( offset >= MAX_TODAY_COUNT || offset < 0 )
		return -1;
	_check_fday( actor_index );
	if ( g_actors[actor_index].today_char[offset] < 127 )
		g_actors[actor_index].today_char[offset]++;
	return 0;
}
int actor_set_today_char_times( int actor_index, int offset, char value )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( offset >= MAX_TODAY_COUNT || offset < 0 )
		return -1;
	_check_fday( actor_index );
	g_actors[actor_index].today_char[offset] = value;
	return 0;
}

// short ���͵�ÿ�����ݣ�ÿ����£�
int actor_get_today_short_times( int actor_index, int offset )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( offset >= MAX_TODAY_SHORT_COUNT || offset < 0 )
		return -1;
	_check_fday( actor_index );
	return g_actors[actor_index].today_short[offset];
}
int actor_add_today_short_times( int actor_index, int offset, short value )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( offset >= MAX_TODAY_SHORT_COUNT || offset < 0 )
		return -1;
	_check_fday( actor_index );
	g_actors[actor_index].today_short[offset] += value;
	return 0;
}
int actor_set_today_short_times( int actor_index, int offset, short value )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( offset >= MAX_TODAY_SHORT_COUNT || offset < 0 )
		return -1;
	_check_fday( actor_index );
	g_actors[actor_index].today_short[offset] = value;
	return 0;
}

// int ���͵�ÿ�����ݣ�ÿ����£�
int actor_get_today_int_times( int actor_index, int offset )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( offset >= MAX_TODAY_INTNUM || offset < 0 )
		return -1;
	_check_fday( actor_index );
	return g_actors[actor_index].today_int[offset];
}
int actor_add_today_int_times( int actor_index, int offset )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( offset >= MAX_TODAY_INTNUM || offset < 0 )
		return -1;
	_check_fday( actor_index );
	g_actors[actor_index].today_int[offset]++;
	return 0;
}
int actor_set_today_int_times( int actor_index, int offset, int value )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( offset >= MAX_TODAY_INTNUM || offset < 0 )
		return -1;
	_check_fday( actor_index );
	g_actors[actor_index].today_int[offset] = value;
	return 0;
}


// ����ʹ�����Ƶ�CDʱ��
int actor_set_uselimit_cd( int actor_index, char index, int cdtime )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( index < 0 || index >= USELIMIT_CD_MAXCOUNT )
		return -1;
	int thistime = (int)time( NULL );
	g_actors[actor_index].cd[index] = thistime + cdtime;
	return 0;
}
int actor_add_uselimit_cd( int actor_index, char index, int cdtime )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( index < 0 || index >= USELIMIT_CD_MAXCOUNT )
		return -1;
	int thistime = (int)time( NULL );
	if ( g_actors[actor_index].cd[index] > 0 )
	{
		g_actors[actor_index].cd[index] += cdtime;
	}
	else
	{
		g_actors[actor_index].cd[index] = thistime + cdtime;
	}
	return 0;
}
int actor_check_uselimit_cd( int actor_index, char index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( index < 0 || index >= USELIMIT_CD_MAXCOUNT )
		return -1;
	int thistime = (int)time( NULL );
	if ( g_actors[actor_index].cd[index] <= thistime )
	{
		g_actors[actor_index].cd[index] = 0;
		return 0;
	}
	return g_actors[actor_index].cd[index] - thistime;
}
int actor_clear_uselimit_cd( int actor_index, char index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( index < 0 || index >= USELIMIT_CD_MAXCOUNT )
		return -1;
	g_actors[actor_index].cd[index] = 0;
	return 0;
}

// ÿ�յ�½����
int actor_loginaward( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( actor_get_today_char_times( actor_index, TODAY_CHAR_LOGINAWARD ) > 0 )
		return -1;
	actor_add_today_char_times( actor_index, TODAY_CHAR_LOGINAWARD );
	if ( g_actors[actor_index].level <= 50 )
	{
		mail_system( actor_index, g_actors[actor_index].actorid, 5001, 5501, 131, 0 );
	}
	else if ( g_actors[actor_index].level <= 100 )
	{
		mail_system( actor_index, g_actors[actor_index].actorid, 5001, 5501, 132, 0 );
	}
	else
	{
		mail_system( actor_index, g_actors[actor_index].actorid, 5001, 5501, 133, 0 );
	}
	return 0;
}
