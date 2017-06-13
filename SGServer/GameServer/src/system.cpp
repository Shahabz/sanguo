#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <time.h>
#include "system.h"
#include "actor.h"
#include "gameproc.h"
#include "utils.h"
#include "db.h"
#include "gamelog.h"
#include "server_netsend_auto.h"
#include "actor_send.h"
#include "script_auto.h"
#include "actor_notify.h"

extern MYSQL *myGame;
extern Actor *g_actors;
extern int g_maxactornum;
extern int g_max_connection;
extern SConfig g_Config;

static int s_begin_time = -1;
static int s_open_time = -1;

// 获取开服流逝的时间 开服时间
int system_getruntime()
{
	if( s_begin_time < 0 )
	{
		int timestring = g_Config.server_start_date;
		struct tm BeginTm;
		if( timestring < 19000101 )
			timestring = 19000101;
		BeginTm.tm_year = timestring/10000 - 1900;
		timestring = timestring%10000;
		BeginTm.tm_mon  = timestring/100 - 1;
		timestring = timestring%100;
		BeginTm.tm_mday = timestring;
		BeginTm.tm_hour = g_Config.server_start_hour;
		BeginTm.tm_min  = g_Config.server_start_min;
		BeginTm.tm_sec  = 0;

		s_begin_time = (int)mktime( &BeginTm );
	}
	return (int)time(NULL)-s_begin_time;
}

int system_getopentime()
{
	if ( s_open_time < 0 )
	{
		int timestring = g_Config.server_start_date;
		struct tm BeginTm;
		if ( timestring < 19000101 )
			timestring = 19000101;
		BeginTm.tm_year = timestring / 10000 - 1900;
		timestring = timestring % 10000;
		BeginTm.tm_mon = timestring / 100 - 1;
		timestring = timestring % 100;
		BeginTm.tm_mday = timestring;
		BeginTm.tm_hour = g_Config.server_start_hour;
		BeginTm.tm_min = g_Config.server_start_min;
		BeginTm.tm_sec = 0;

		s_open_time = (int)mktime( &BeginTm );
	}
	return s_open_time;
}

int system_getfday()
{
	int thisday = ((int)time( NULL ) - (int)timezone) / 86400;
	return thisday;
}

int system_gettoday()
{
	time_t t;
	time(&t);
	struct tm *newtime = localtime( &t );

	return ((1900+newtime->tm_year)*10000 + (newtime->tm_mon+1)*100 + newtime->tm_mday);
}
/* 获取月份 */
int system_getmonth()
{
	time_t t;
	time( &t );
	struct tm *newtime = localtime( &t );

	return (newtime->tm_mon + 1);
}
/* 获取星期 */
int system_getweek()
{
	time_t t;
	time( &t );
	struct tm *newtime = localtime( &t );

	return newtime->tm_wday;
}

/* 获取最大天数 */
int system_getmaxday( int year, int month )
{
	int maxday = 0;
	if ( month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12 )
		maxday = 31;
	else if ( month == 4 || month == 6 || month == 9 || month == 11 )
		maxday = 30;
	else if ( month == 2 )
	{
		if ( IS_LEAP_YEAR( year ) )
			maxday = 29;
		else
			maxday = 28;
	}
	return maxday;
}

/* 获取当月最大天数 */
int system_getcurmonth_maxday()
{
	time_t t;
	time( &t );
	struct tm *newtime = localtime( &t );

	return system_getmaxday( (1900 + newtime->tm_year), (newtime->tm_mon + 1) );
}

int system_timestring( int timestamp, char *out )
{
	int len = 0;
	sc_TimeString( timestamp, out, &len );
	return len;
}

int system_countdownstring( int interval, char *out )
{
	int hour = interval / 3600 ;
	int min =  interval / 60  % 60;
	int sec = interval % 60;

	char strHour[32] = { 0 };
	char strMin[32] = { 0 };
	char strSec[32] = { 0 };

	if ( hour < 10 )
		sprintf( strHour, "0%d", hour );
	else
		sprintf( strHour, "%d", hour );
	
	if ( min < 10 )
		sprintf( strMin, "0%d", min );
	else
		sprintf( strMin, "%d", min );
	
	if ( sec < 10 )
		sprintf( strSec, "0%d", sec );
	else
		sprintf( strSec, "%d", sec );
	
	sprintf( out, "%s:%s:%s", strHour, strMin, strSec );
	return 0;
}

/* 随机 */
int random( int min, int max )
{
	int value = (max - min + 1);
	if ( value <= 0 )
		value = 1;
	return min + rand() % value;
}

/* 洗牌 */
void ruffle( int *list, int count )
{
	for ( int tmpi = 0; tmpi < count; tmpi++ )
	{
		int index = rand() % count;
		int swap = list[index];
		swap = list[tmpi];
		list[tmpi] = list[index];
		list[index] = swap;
	}
}

// 产生长度为length的随机字符串  
void random_str( char *out, int length )
{
	for ( int tmpi = 0; tmpi < length; tmpi++ )
	{
		int flag = rand() % 3;
		switch ( flag )
		{
		case 0:
			out[tmpi] = 'A' + rand() % 26;
			break;
		case 1:
			out[tmpi] = 'a' + rand() % 26;
			break;
		case 2:
			out[tmpi] = '0' + rand() % 10;
			break;
		default:
			out[tmpi] = 'x';
			break;
		}
	}
	out[length] = '\0';
}


DelayExec g_DelayExec[DELAYEXEC_MAX];
void delay_exec_add( char cmd, void *p, int value, int cd )
{
	for ( int tmpi = 0; tmpi < DELAYEXEC_MAX; tmpi++ )
	{
		if ( g_DelayExec[tmpi].cmd <= 0 )
		{
			g_DelayExec[tmpi].cmd = cmd;
			g_DelayExec[tmpi].ptr = p;
			g_DelayExec[tmpi].value = value;
			g_DelayExec[tmpi].exec_time = (int)time( NULL ) + cd;
			break;
		}
	}
}

void delay_exec_fetch()
{
	int nowtime = (int)time( NULL );
	for ( int tmpi = 0; tmpi < DELAYEXEC_MAX; tmpi++ )
	{
		if ( g_DelayExec[tmpi].cmd <= 0 )
			continue;
		if ( g_DelayExec[tmpi].exec_time > nowtime )
			continue;

		
		g_DelayExec[tmpi].cmd = 0;
		g_DelayExec[tmpi].ptr = NULL;
		g_DelayExec[tmpi].value = 0;
		g_DelayExec[tmpi].exec_time = 0;

	}
}
