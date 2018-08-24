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

	// 角色表也插入
	char szSQL[1024];
	char ActorName[64];
	db_escape( (const char *)city.name, ActorName, 0 );
	sprintf( szSQL, "insert into actor_list (actorid,offset,create_time,name,nation) values('%d','%d','%d','%s','%d')",
		city.actorid, 0, city.createtime, ActorName, city.nation );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
		{
			db_reconnect_game();
		}
		//return -3;// 可能是角色重名了
	}

	// 插入记录到 actor
	sprintf( szSQL, "insert into actor (actorid,name,nation,createtime) values('%d','%s','%d','%d')",
		city.actorid, ActorName, city.nation, city.createtime );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf_msg( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		//return -4;	// 可能是id错误同名了
	}

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
	g_city[city_index].silver = g_robot_ai[ai].silver_init;
	g_city[city_index].wood = g_robot_ai[ai].wood_init;
	g_city[city_index].food = g_robot_ai[ai].food_init;
	g_city[city_index].iron = g_robot_ai[ai].iron_init;
	return 0;
}

void robot_logic_min()
{

}

void robot_logic_hour()
{
	for ( int tmpi = 0; tmpi < g_city_maxindex/*注意：使用索引位置，为了效率*/; tmpi++ )
	{
		if ( g_city[tmpi].robot_ai > 0 )
		{
			robot_logic( &g_city[tmpi] );
		}
	}
}

// 加资源
void robot_logic( City *pCity )
{
	int ai = pCity->robot_ai;
	if ( ai <= 0 || ai >= g_robot_ai_maxnum )
		return;

	int level = pCity->building[0].level;
	if ( level <= 0 || level >= g_robot_base_maxnum )
		return;
	Building *pWall = building_getptr_kind( pCity->index, BUILDING_Wall );
	if ( pWall && pWall->level < level )
	{
		pWall->level = level;
	}
	else
	{
		building_give( pCity->index, BUILDING_Wall, 1 );
	}

	int actorlevel = random( g_robot_base[level].actorlevel_min, g_robot_base[level].actorlevel_max );
	if ( actorlevel > pCity->level )
	{
		pCity->level = actorlevel;
	}

	// 加资源
	int silver_max = (int)(g_robot_base[level].silver_max * g_robot_ai[ai].silver_max);
	int silver_add = (int)(g_robot_base[level].silver_add * g_robot_ai[ai].silver_add);
	if ( pCity->silver < silver_max )
	{
		pCity->silver += silver_add;
	}

	int wood_max = (int)(g_robot_base[level].wood_max * g_robot_ai[ai].wood_max);
	int wood_add = (int)(g_robot_base[level].wood_add * g_robot_ai[ai].wood_add);
	if ( pCity->wood < wood_max )
	{
		pCity->wood += wood_add;
	}

	int food_max = (int)(g_robot_base[level].food_max * g_robot_ai[ai].food_max);
	int food_add = (int)(g_robot_base[level].food_add * g_robot_ai[ai].food_add);
	if ( pCity->food < food_max )
	{
		pCity->food += food_add;
	}

	int iron_max = (int)(g_robot_base[level].iron_max * g_robot_ai[ai].iron_max);
	int iron_add = (int)(g_robot_base[level].iron_add * g_robot_ai[ai].iron_add);
	if ( pCity->iron < iron_max )
	{
		pCity->iron += iron_add;
	}

	// 自动升级
	// 空闲建造队是否满足
	if ( pCity->worker_sec <= 0 )
	{
		char kind = 0;
		char offset = -1;
		for ( int tmpi = 0; tmpi < BUILDING_MAXNUM; tmpi++ )
		{
			if ( pCity->building[tmpi].kind <= 0 )
				continue;
			if ( pCity->building[tmpi].kind == pCity->worker_kind || pCity->building[tmpi].kind == pCity->worker_kind_ex )
				continue;
			BuildingUpgradeConfig *config = building_getconfig( pCity->building[tmpi].kind, pCity->building[tmpi].level + 1 );
			if ( config )
			{
				// 角色等级是否满足
				if ( pCity->level < config->actorlevel )
					continue;
				// 官府等级是否满足
				if ( pCity->building[0].level < config->citylevel )
					continue;
				// 资源是否满足
				if ( pCity->silver < config->silver )
					continue;
				if ( pCity->wood < config->wood )
					continue;
				kind = pCity->building[tmpi].kind;
				offset = tmpi;
				break;
			}
		}

		if ( kind > 0 )
		{
			building_upgrade( pCity->index, kind, offset );
		}
	}

	// 兵力库存
	pCity->soldiers[0] = random( 0, 20000 );
	pCity->soldiers[1] = random( 0, 20000 );
	pCity->soldiers[2] = random( 0, 20000 );

	// 城墙守卫
	if ( pWall )
	{
		for ( int tmpi = 0; tmpi < pWall->level; tmpi++ )
		{
			if ( pCity->guard[tmpi].monsterid <= 0 )
			{
				BuildingUpgradeConfig *buildingconfig = building_getconfig( BUILDING_Wall, pWall->level );
				if ( buildingconfig )
				{
					char monsterid = random( 1, g_cityguardinfo_maxnum - 1 );
					char color = ITEM_COLOR_LEVEL_GREEN;
					char corps = random( 0, 2 );
					char shape = random( 1, 15 );
					unsigned char minlevel = buildingconfig->value[0];
					unsigned char maxlevel = buildingconfig->value[1];
					unsigned char level = random( minlevel, maxlevel );

					CityGuardInfoConfig *config = city_guard_config( monsterid, color );
					if ( config )
					{
						pCity->guard[tmpi].monsterid = monsterid;
						pCity->guard[tmpi].corps = corps;
						pCity->guard[tmpi].color = color;
						pCity->guard[tmpi].shape = shape;
						pCity->guard[tmpi].level = level;
						pCity->guard[tmpi].soldiers = TROOPS( level, config->troops, config->troops_growth );
						pCity->guardnum += 1;
					}
				}
			}
		}
	}
	// 设置武将
	int list[12] = {0};
	int count = 0;
	for ( int tmpi = 0; tmpi < 8; tmpi++ )
	{
		if ( g_robot_base[level].herokind[tmpi] > 0 )
		{
			list[count] = g_robot_base[level].herokind[tmpi];
			count += 1;
		}
	}
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		if ( g_robot_ai[ai].herokind[tmpi] > 0 )
		{
			list[count] = g_robot_ai[ai].herokind[tmpi];
			count += 1;
		}
	}
	ruffle( list, count );
	for ( int tmpi = 0; tmpi < g_robot_base[level].heroup_max; tmpi++ )
	{
		short kind = list[tmpi];
		char color = hero_defaultcolor( kind );
		HeroInfoConfig *config = hero_getconfig( kind, color );
		if ( !config )
			continue;
		if ( config->sound < 0 )
			continue;
		memset( &pCity->hero[tmpi], 0, sizeof( Hero ) );
		pCity->hero[tmpi].id = 0;
		pCity->hero[tmpi].actorid = 0;
		pCity->hero[tmpi].offset = HERO_BASEOFFSET + tmpi;
		pCity->hero[tmpi].kind = (short)kind;	//英雄种类
		pCity->hero[tmpi].color = color;	//颜色
		pCity->hero[tmpi].level = pCity->level;	//等级
		pCity->hero[tmpi].exp = 0;	//经验
		pCity->hero[tmpi].attack_wash = config->attack_wash;	//洗髓攻击资质
		pCity->hero[tmpi].defense_wash = config->defense_wash;	//洗髓防御资质
		pCity->hero[tmpi].troops_wash = config->troops_wash;	//洗髓兵力资质
		pCity->hero[tmpi].soldiers = 0;
		pCity->hero[tmpi].equip[0].kind = g_robot_base[level].equipkind[0];
		pCity->hero[tmpi].equip[1].kind = g_robot_base[level].equipkind[1];
		pCity->hero[tmpi].equip[2].kind = g_robot_base[level].equipkind[2];
		pCity->hero[tmpi].equip[3].kind = g_robot_base[level].equipkind[3];
		pCity->hero[tmpi].equip[4].kind = g_robot_base[level].equipkind[4];
		pCity->hero[tmpi].equip[5].kind = g_robot_base[level].equipkind[5];
		hero_attr_calc( pCity, &pCity->hero[tmpi] );
		pCity->hero[tmpi].soldiers = pCity->hero[tmpi].troops;
	}
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
