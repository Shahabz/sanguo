#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <math.h>
#include <limits.h>
#include "db.h"
#include "dserver.h"
#include "global.h"
#include "wqueue.h"
#include "actor.h"
#include "actor_times.h"
#include "city.h"
#include "city_attr.h"
#include "city_tech.h"
#include "building.h"
#include "hero.h"
#include "equip.h"
#include "mapunit.h"
#include "zoneunit.h"
#include "map.h"
#include "map_zone.h"
#include "server_netsend_auto.h"
#include "system.h"
#include "item.h"
#include "vip.h"
#include "actor_notify.h"
#include "army.h"
#include "army_group.h"
#include "map_enemy.h"
#include "map_res.h"
#include "map_event.h"
#include "map_call.h"
#include "mail.h"
#include "nation.h"
#include "nation_equip.h"
#include "nation_hero.h"
#include "robot.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern UpgradeInfo *g_upgradeinfo;
extern int g_upgradeinfo_maxnum;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;
extern Map g_map;

extern Actor *g_actors;
extern int g_maxactornum;

extern CityGuardInfo *g_cityguardinfo;
extern int g_cityguardinfo_maxnum;

extern TechInfo *g_techinfo;
extern int g_techinfo_maxnum;

extern MapZoneInfo *g_zoneinfo;
extern int g_zoneinfo_maxnum;

extern Army *g_army;
extern int g_army_maxcount;

extern BuildingUpgrade *g_building_upgrade;
extern int g_building_upgrade_maxnum;

extern ArmyGroup *g_armygroup;
extern int g_armygroup_maxcount;

extern char g_gm_outresult[MAX_OUTRESULT_LEN];

extern int g_city_maxindex;
extern City *g_city ;
extern int g_city_maxcount;
extern int g_maxactorid;

extern RobotBase *g_robot_base;
extern int g_robot_base_maxnum;

extern RobotAi *g_robot_ai;
extern int g_robot_ai_maxnum;

extern LastName *g_lastname;
extern int g_lastname_maxnum;

extern FirstName *g_firstname;
extern int g_firstname_maxnum;

// 随机名称
int robot_randomname( char *name )
{
	int lastindex = random( 1, g_lastname_maxnum - 1 );
	int firstindex = random( 1, g_firstname_maxnum - 1 );
	snprintf( name, NAME_SIZE, "%s%s", g_lastname[lastindex].str, g_firstname[lastindex].str );
	name[NAME_SIZE - 1] = 0;

	MYSQL_RES * res;
	MYSQL_ROW	row;
	char szSQL[1024];
	char szText_newname[NAME_SIZE * 2 + 1] = { 0 };
	db_escape( (const char *)name, szText_newname, 0 );
	sprintf( szSQL, "SELECT count(*) FROM city WHERE name='%s'", szText_newname );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( row = mysql_fetch_row( res ) )
	{
		if ( atoi( row[0] ) >= 1 )
		{
			mysql_free_result( res );
			return -1;
		}
	}
	mysql_free_result( res );
	return 0;
}

// 创建机器人
int robot_create( char ai, char nation )
{
	if ( ai <= 0 || ai >= g_robot_ai_maxnum )
		return -1;

	City city = { 0 };
	city.actorid = g_maxactorid++;
	city.type = CityLairdType_Robot;
	city.shape = random(0,5);
	city.createtime = (int)time( NULL );
	city.lastlogin = (int)time( NULL );
	city.nation = nation;
	city.level = 1;
	city.body = global.body_max;
	city.robot_ai = ai;
	city.robot_cd = (int)(g_robot_base[city.level].minute * (float)g_robot_ai[ai].actorexp_add);

	// 随机名字
	char step = 0;
	while ( step < 10 )
	{
		if ( robot_randomname( city.name ) >= 0 )
			break;
		step++;
	}

	// 随机位置
	if ( map_getrandcitypos( &city.posx, &city.posy ) < 0 )
	{
		return -1;
	}
	int city_index = city_new( &city );
	if ( city_index <= 0 )
		return -1;

	// 主城
	building_create( city_index, BUILDING_Main, -1 );
	g_city[city_index].building[0].level = 2;
	city_building_save_auto( g_city[city_index].actorid, 0, &g_city[city_index].building[0], "city_building", NULL );

	g_city[city_index].unit_index = mapunit_add( MAPUNIT_TYPE_CITY, city_index );
	g_city[city_index].zoneunit_index = zoneunit_add( MAPUNIT_TYPE_CITY, city_index );
	map_addobject( MAPUNIT_TYPE_CITY, g_city[city_index].unit_index, city.posx, city.posy );
	g_city[city_index].zone = map_zone_getid( city.posx, city.posy );

	// 通知小地图
	SLK_NetS_ZoneUnit info = { 0 };
	info.m_posx = g_city[city_index].posx;
	info.m_posy = g_city[city_index].posy;
	info.m_nation = g_city[city_index].nation;
	info.m_level = (unsigned char)g_city[city_index].level;
	info.m_zoneunit_index = g_city[city_index].zoneunit_index;
	netsend_addzoneunit_S( g_city[city_index].zone, SENDTYPE_ZONE, &info );

	// 新手保护
	g_city[city_index].ptsec = global.newplayer_protect;

	// 征收时间
	g_city[city_index].levysec = global.levy_max;
	return 0;
}


// 创建队列
RobotCreateQueue *g_robotcreate_queue = NULL;
int g_nRobotCreateQueueHead;
int g_nRobotCreateQueueTail;
int robotcreate_queue_init()
{
	g_nRobotCreateQueueHead = 0;
	g_nRobotCreateQueueTail = 0;
	g_robotcreate_queue = (RobotCreateQueue *)malloc( sizeof( RobotCreateQueue )*ROBOTCREATE_QUEUE_MAXNUM );
	return 0;
}

int robotcreate_queue_add( char ai, char nation )
{
	// 获取尾部
	int queue_tail = g_nRobotCreateQueueTail + 1;
	if ( queue_tail >= ROBOTCREATE_QUEUE_MAXNUM )
	{
		queue_tail = 0;
	}

	// 当队列已经满了
	if ( g_nRobotCreateQueueHead == queue_tail )
	{
		return -1;
	}

	g_robotcreate_queue[g_nRobotCreateQueueTail].ai = ai;
	g_robotcreate_queue[g_nRobotCreateQueueTail].nation = nation;

	// 尾部步进
	g_nRobotCreateQueueTail = queue_tail;
	return 0;
}

int robotcreate_queue_fetch()
{
	if ( g_nRobotCreateQueueTail == g_nRobotCreateQueueHead )
	{
		return -1;
	}

	// 从队列中取出一项
	robot_create( g_robotcreate_queue[g_nRobotCreateQueueHead].ai, g_robotcreate_queue[g_nRobotCreateQueueHead].nation);

	// 头部步进
	g_nRobotCreateQueueHead++;
	if ( g_nRobotCreateQueueHead >= ROBOTCREATE_QUEUE_MAXNUM )
	{
		g_nRobotCreateQueueHead = 0;
	}
	return 0;
}
