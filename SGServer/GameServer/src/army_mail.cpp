#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "db.h"
#include "define.h"
#include "dserver.h"
#include "building.h"
#include "actor.h"
#include "hero.h"
#include "area.h"
#include "actor_send.h"
#include "actor_times.h"
#include "server_netsend_auto.h"
#include "mapunit.h"
#include "fight.h"
#include "actor_notify.h"
#include "item.h"
#include "award.h"
#include "global.h"
#include "quest.h"
#include "activity.h"
#include "system.h"
#include "army.h"
#include "army_march.h"
#include "army_mail.h"
#include "city.h"
#include "map_town.h"
#include "map_enemy.h"
#include "map_res.h"
#include "map.h"
#include "mail.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern HeroInfo *g_HeroInfo;
extern short g_HeroCount;


extern City * g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;

extern ItemKind *g_itemkind;
extern int g_itemkindnum;

extern MapZoneInfo *g_zoneinfo;
extern int g_zoneinfo_maxnum;

extern MapEnemyInfo *g_enemyinfo;
extern int g_enemyinfo_maxnum;

extern MapEnemy *g_map_enemy;
extern int g_map_enemy_maxcount;

extern MapRes *g_map_res;
extern int g_map_res_maxcount;

extern MapTownInfo *g_towninfo;
extern int g_towninfo_maxnum;

extern Map g_map;

extern Army *g_army;
extern int g_army_maxcount;
extern int g_army_count;
extern int g_army_maxindex;

// 采集邮件
int army_mail_gather( int army_index )
{
	if ( army_index < 0 || army_index >= g_army_maxcount )
		return -1;
	City *pCity = army_getcityptr( army_index );
	if ( !pCity )
		return -1;

	MapResInfo *config = map_res_getconfig( g_army[army_index].appdata );
	if ( !config )
		return -1;

	// 资源类型和数量
	int restype = 0;
	int resnum = 0;
	if ( g_army[army_index].silver > 0 )
	{
		restype = 1;
		resnum = g_army[army_index].silver;
	}
	else if ( g_army[army_index].wood > 0 )
	{
		restype = 2;
		resnum = g_army[army_index].wood;
	}
	else if ( g_army[army_index].food > 0 )
	{
		restype = 3;
		resnum = g_army[army_index].food;
	}
	else if ( g_army[army_index].iron > 0 )
	{
		restype = 4;
		resnum = g_army[army_index].iron;
	}
	
	else if ( g_army[army_index].token > 0 )
	{
		restype = 5;
		resnum = g_army[army_index].token;
	}

	// 获得经验 = 每次采集的经验收益就是采集时长/采集总时间*配置总的采集经验
	int exp = (int)ceil( g_army[army_index].statetime / (float)config->sec * config->exp );

	char title[MAIL_TITLE_MAXSIZE] = { 0 };
	char content[MAIL_CONTENT_MAXSIZE] = { 0 };
	if ( g_army[army_index].reback == ARMY_REBACK_NORMAL )
	{
		sprintf( title, "%s%d", TAG_TEXTID, 5007 );
	}
	else if ( g_army[army_index].reback == ARMY_REBACK_RETURN )
	{
		sprintf( title, "%s%d", TAG_TEXTID, 5008 );
	}
	else if ( g_army[army_index].reback == ARMY_REBACK_FIGHTLOSE )
	{
		sprintf( title, "%s%d", TAG_TEXTID, 5009 );
	}

	int hero_index = city_hero_getindex( pCity->index, g_army[army_index].herokind[0] );
	if ( hero_index < 0 || hero_index >= HERO_CITY_MAX )
		return -1;
	Hero *pHero = &pCity->hero[hero_index];

	// 在这里把武将获得经验加上吧
	hero_addexp( pCity, pHero, exp, PATH_GATHER );

	// 拼邮件
	sprintf( content, "{\"reback\":%d, \"pos\":\"%d,%d\",\"time\":%d,\"res\":%d,\"num\":%d,\"buff\":%d,\"hkind\":%d,\"hlv\":%d,\"hexp\":%d}", 
		g_army[army_index].reback, g_army[army_index].to_posx, g_army[army_index].to_posy, g_army[army_index].statetime, restype, resnum, g_army[army_index].other, pHero->kind, pHero->level, exp );

	mail( pCity->actor_index, pCity->actorid, MAIL_TYPE_GATHER, title, content, "", 0, 0 );
	return 0;
}
