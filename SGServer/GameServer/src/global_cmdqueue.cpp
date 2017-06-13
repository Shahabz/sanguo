#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <time.h>
#include "db.h"
#include "actor.h"
#include "gameproc.h"
#include "utils.h"
#include "db.h"
#include "gamelog.h"
#include "server_netsend_auto.h"
#include "actor_send.h"
#include "script_auto.h"
#include "actor_notify.h"
#include "global_cmdqueue.h"

extern MYSQL *myGame;
extern Actor *g_actors;
extern int g_maxactornum;
extern int g_max_connection;

extern SConfig g_Config;

GlobalCmd g_globalcmd[GLOBALCMD_MAXNUM];
int g_globalcmd_maxid = 0;

int globalcmd_init()
{
	memset( g_globalcmd, 0, sizeof(GlobalCmd)*GLOBALCMD_MAXNUM );
	g_globalcmd_maxid = 1;
	return 0;
}

int globalcmd_add( short cmd, int city_index )
{
	for ( int tmpi = 0; tmpi < GLOBALCMD_MAXNUM; tmpi++ )
	{
		if ( g_globalcmd[tmpi].id <= 0 )
		{
			g_globalcmd[tmpi].id = ++g_globalcmd_maxid;
			g_globalcmd[tmpi].cmd = cmd;
			g_globalcmd[tmpi].city_index = city_index;
			g_globalcmd[tmpi].timeout = 6;
			return g_globalcmd[tmpi].id;
		}
	}
	return 0;
}

void globalcmd_del( int index )
{
	if ( index < 0 || index >= GLOBALCMD_MAXNUM )
		return;
	g_globalcmd[index].id = 0;
	g_globalcmd[index].cmd = 0;
	g_globalcmd[index].city_index = -1;
	g_globalcmd[index].timeout = 0;
}

int globalcmd_response( int id )
{
	for ( int tmpi = 0; tmpi < GLOBALCMD_MAXNUM; tmpi++ )
	{
		if ( g_globalcmd[tmpi].id == id )
		{
			globalcmd_del( tmpi );
			return 0;
		}
	}
	return -1;
}

void globalcmd_fetch()
{
	for ( int tmpi = 0; tmpi < GLOBALCMD_MAXNUM; tmpi++ )
	{
		if ( g_globalcmd[tmpi].id <= 0 )
			continue;
		
		g_globalcmd[tmpi].timeout -= 1;
		if ( g_globalcmd[tmpi].timeout <= 0 )
		{ // Ö´ÐÐ³¬Ê±²Ù×÷
			
			globalcmd_del( tmpi );
		}
	}
}
