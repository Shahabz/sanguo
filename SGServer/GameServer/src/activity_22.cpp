#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "actor.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "script.h"
#include "db.h"
#include "script_auto.h"
#include "server_netsend_auto.h"
#include "activity.h"
#include "wqueue.h"
#include "timegmcmd.h"
#include "gmcmd.h"
#include "system.h"
#include "global.h"
#include "pay.h"
#include "actor_times.h"
#include "city.h"
#include "chat.h"
#include "quest.h"
#include "hero.h"
#include "nation.h"
#include "mail.h"
#include "activity_04.h"
#include "equip.h"
#include "map.h"
#include "map_activity.h"
#include "map_zone.h"
#include "mapunit.h"
#include "army.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern Actor *g_actors;
extern int g_maxactornum;

extern Global global;
extern CommandEntry *g_command_entry;
extern int g_command_count;

extern City * g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern Army *g_army;
extern int g_army_maxcount;

extern ActivityItem *g_activity_item;
extern int g_activity_count;


void activity_22_onwarning( int lefttime )
{
	if ( lefttime < 0 )
		return;
	int countdown = lefttime / 60; //µ¹¼ÆÊ±£¨·ÖÖÓ£©

	if ( countdown == 30 || countdown == 5 || countdown == 1 )
	{
		char v1[64] = { 0 };
		sprintf( v1, "%d", countdown );
		//system_talkjson_world( 6033, v1, NULL, NULL, NULL, NULL, NULL, 2 );
	}
}
void activity_22_onopen()
{
	SLK_NetS_RedInfo pValue = { 0 };
	pValue.m_path = 8;
	pValue.m_has = 1;
	netsend_redinfo_S( 0, SENDTYPE_WORLD, &pValue );
}
void activity_22_onend()
{
	SLK_NetS_RedInfo pValue = { 0 };
	pValue.m_path = 8;
	pValue.m_has = 0;
	netsend_redinfo_S( 0, SENDTYPE_WORLD, &pValue );
}
void activity_22_onclose()
{

}
void activity_22_onlogic()
{

}
int activity_22_sendinfo( int actor_index )
{

}

