#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>

#include <stdio.h>
#include <time.h>
#include "timegmcmd.h"
#include "actor.h"
#include "gameproc.h"
#include "utils.h"
#include "db.h"
#include "gamelog.h"
#include "actor_send.h"
#include "script.h"
#include "activity.h"
#include "gmcmd.h"

extern MYSQL *myGame;
extern Actor *g_actors;
extern int g_maxactornum;
extern int g_max_connection;

CommandEntry *g_command_entry = NULL;
int g_command_count = 0;

// 定时指令初始化
int time_gmcmd_init()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024] = {0};

	if( g_command_entry )
	{
		free( g_command_entry );
		g_command_entry = NULL;
		g_command_count = 0;
	}

	sprintf( szSQL, "select count(*) from command_data" );
	if( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error(myGame) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myGame );
	if( ( row = mysql_fetch_row( res ) ) )
	{
		if( row[0] )
			g_command_count = atoi( row[0] );
		else
			g_command_count = 0;
	}
	else
	{
		g_command_count = 0;
	}
	mysql_free_result( res );

	if( g_command_count <= 0 )
		return 0;

	g_command_entry = (CommandEntry *)malloc( sizeof( CommandEntry )*g_command_count );
	memset( g_command_entry, 0, sizeof( CommandEntry )*g_command_count );

	sprintf( szSQL, "select id,cond,cmd,value1,value2,value3,value4,strvalue,timestr from command_data" );
	if( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error(myGame) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myGame );

	char isnotify = 0;
	int index = 0;
	while( ( row = mysql_fetch_row( res ) ) )
	{
		if ( index >= g_command_count )
			break;
		g_command_entry[index].m_id = (int)atoi( row[0] );
		g_command_entry[index].m_cond = (int)atoi( row[1] );
		g_command_entry[index].m_cmd = (int)atoi( row[2] );
		g_command_entry[index].m_value[0] = (int)atoi( row[3] );
		g_command_entry[index].m_value[1] = (int)atoi( row[4] );
		g_command_entry[index].m_value[2] = (int)atoi( row[5] );
		g_command_entry[index].m_value[3] = (int)atoi( row[6] );
		strncpy( g_command_entry[index].m_strvalue, row[7], COMMAND_STRVALUE_MAX );
		strncpy( g_command_entry[index].m_timestr, row[8], 64 );
		time_format( &g_command_entry[index].m_time, g_command_entry[index].m_timestr );
		time_checktime( &g_command_entry[index].m_time );
		index++;
	}

	mysql_free_result( res );
	return 0;
}

void time_gmcmd_reset()
{
//	db_opendata();
	time_gmcmd_init();
//	db_closedata();
}

int time_gmcmd_logic()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;

	int tmpi;
	char	szSQL[2048];
	int valueindex;
	for( tmpi = 0; tmpi < g_command_count; tmpi++ )
	{
		if( g_command_entry[tmpi].m_id <= 0 )
			continue;
		if( time_checktime( &g_command_entry[tmpi].m_time ) == 0 )
		{
			// process action
			if( g_command_entry[tmpi].m_cond == 0 )
			{
				actor_command( -1, g_command_entry[tmpi].m_cmd, g_command_entry[tmpi].m_value, g_command_entry[tmpi].m_strvalue );
			}
			else
			{
				valueindex = sc_local_GetCondSql( g_command_entry[tmpi].m_cond, szSQL );
				if( valueindex >= 0 && valueindex < 4 )
				{
					if( mysql_query( myGame, szSQL ) )
					{
						printf_msg( "Query failed (%s)\n", mysql_error(myGame) );
						write_gamelog( "%s", szSQL );
						continue;
					}
					res = mysql_store_result( myGame );
					while( ( row = mysql_fetch_row( res ) ) )
					{
						g_command_entry[tmpi].m_value[valueindex] = (int)atoi( row[0] );
						actor_command( -1, g_command_entry[tmpi].m_cmd, g_command_entry[tmpi].m_value, g_command_entry[tmpi].m_strvalue );
					}
					mysql_free_result( res );
				}
			}
		}
	}
	return 0;
}

// 距离下次启动最近的时间
int time_gmcmd_getnexttime( int cmd, int value )
{
	int mintime = 0;
	for ( int tmpi = 0; tmpi < g_command_count; tmpi++ )
	{
		if ( g_command_entry[tmpi].m_id <= 0 )
			continue;
		if ( g_command_entry[tmpi].m_time.mtime == 0 )
			time_reset( &g_command_entry[tmpi].m_time );
		if ( g_command_entry[tmpi].m_time.mtime == 0 )
			continue;
		if ( g_command_entry[tmpi].m_cmd != cmd )
			continue;
		if ( value >= 0 && g_command_entry[tmpi].m_value[0] != value )
			continue;
		if ( mintime == 0 || mintime > g_command_entry[tmpi].m_time.mtime )
		{
			mintime = (int)g_command_entry[tmpi].m_time.mtime;
		}
	}
	return mintime;
}

