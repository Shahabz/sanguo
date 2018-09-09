#ifndef __DEFINE_H
#define __DEFINE_H
#include "utils.h"
#include "gamelog.h"
#include "client.h"
#include "auto_actor_item.h"
#include "auto_actor_equip.h"
#include "auto_actor_girl.h"
#include "server_struct_auto.h"
#include "server_structrecv_auto.h"
#include "global_netsend_auto.h"
#include "auto_data_item.h"
#include "auto_data_equip.h"
#include "auto_data_equip_wash.h"
#include "auto_data_equip_washrule.h"
#include "auto_data_platinfo.h"
#include "auto_data_paygoods.h"
#include "auto_data_paystore.h"
#include "auto_data_upgrade.h"
#include "auto_data_building_res_unlock.h"
#include "auto_data_building_upgrade.h"
#include "auto_data_vip.h"
#include "auto_data_vipbag.h"
#include "auto_data_vipshop.h"
#include "auto_data_hero.h"
#include "auto_data_hero_colorup.h"
#include "auto_data_hero_corpsmul.h"
#include "auto_data_hero_skill.h"
#include "auto_data_hero_visit.h"
#include "auto_data_quest.h"
#include "auto_data_quest_talk.h"
#include "auto_data_city_guard.h"
#include "auto_data_city_tech.h"
#include "auto_data_trainlong.h"
#include "auto_data_trainqueue.h"
#include "auto_data_bodytoken.h"
#include "auto_data_official_forging.h"
#include "auto_data_official_gov.h"
#include "auto_data_official_tech.h"
#include "auto_data_material_make.h"
#include "auto_data_monster.h"
#include "auto_data_fight_helper.h"
#include "auto_data_map_zoneinfo.h"
#include "auto_data_map_towninfo.h"
#include "auto_data_map_enemyinfo.h"
#include "auto_data_map_resinfo.h"
#include "auto_data_map_eventinfo.h"
#include "auto_data_map_pickupinfo.h"
#include "auto_data_map_activityinfo.h"
#include "auto_data_nation_equip.h"
#include "auto_data_nation_equip_open.h"
#include "auto_data_nation_equip_remake.h"
#include "auto_data_nation_upgrade.h"
#include "auto_data_nation_place.h"
#include "auto_data_nation_quest.h"
#include "auto_data_nation_mission.h"
#include "auto_data_nation_official.h"
#include "auto_data_nation_heroinfo.h"
#include "auto_data_weather.h"
#include "auto_data_wishing_shop.h"
#include "auto_data_wishing_pack.h"
#include "auto_data_shop.h"
#include "auto_data_story.h"
#include "auto_data_world_bossinfo.h"
#include "auto_data_world_questinfo.h"
#include "auto_data_kingwar_towninfo.h"
#include "auto_data_kingwar.h"
#include "auto_data_tiance_quest.h"
#include "auto_data_teacher_award.h"
#include "auto_data_teacher_shop.h"
#include "auto_data_activity_02.h"
#include "auto_data_activity_03.h"
#include "auto_data_activity_04.h"
#include "auto_data_activity_05.h"
#include "auto_data_activity_06.h"
#include "auto_data_activity_08.h"
#include "auto_data_activity_10.h"
#include "auto_data_activity_12.h"
#include "auto_data_activity_17.h"
#include "auto_data_activity_33.h"
#include "auto_data_girl.h"
#include "auto_data_girl_love.h"
#include "auto_data_girl_son.h"
#include "auto_data_girl_shop.h"
#include "auto_data_girl_shop_update.h"
#include "auto_data_fangshi_node.h"
#include "auto_data_fangshi_palace.h"
#include "auto_data_random_lastname.h"
#include "auto_data_random_firstname.h"
#include "auto_data_robot_ai.h"
#include "auto_data_robot_base.h"
#include "auto_data_everyday_event.h"
#include "auto_data_everyday_quest.h"
#include "auto_actor_equip.h"
#include "auto_actor_hero.h"
#include "auto_actor_girl.h"
#include "auto_actor_friend.h"
#include "auto_city_building.h"
#include "auto_city_building_barracks.h"
#include "auto_city_building_res.h"
#include "auto_city_attr.h"
#include "auto_city_guard.h"
#include "auto_city_nation_hero.h"
#include "auto_city.h"
#include "auto_army.h"
#include "auto_army_group.h"
#include "auto_map_activity.h"
#include "auto_map_zone.h"
#include "auto_map_town.h"
#include "auto_map_enemy.h"
#include "auto_map_res.h"
#include "auto_map_event.h"
#include "auto_map_call.h"
#include "auto_map_pickup.h"
#include "auto_nation.h"
#include "auto_nation_hero.h"
#include "auto_world_boss.h"
#include "auto_kingwar_town.h"
#include "actor_send.h"
#include "award.h"

#define LOG_PATH	"./log/"

#ifndef NAME_SIZE
#define NAME_SIZE					22		// 玩家名字和物品名字长度
#endif
#define SIGNATURE_SIZE				128		// 个性签名
#define EPSINON 0.000001

// 最小的角色ID值
#define MINACTORID	1000

#define ACTOR_QUEST_MAX	2048

/* 背包最大数量*/
#define MAX_DEFAULT_ITEMNUM	200
#define MAX_ACTOR_ITEMNUM	300

/* 装备最大数量*/
#define MAX_DEFAULT_EQUIPNUM	30  // 缺省格子数
#define MAX_EXT_EQUIPNUM		150	// 允许开到多少格子
#define MAX_ACTOR_EQUIPNUM		180	// 装备栏上限

#define HERO_ACTOR_MAX				80	// 未上英雄
#define HERO_CITY_MAX				12	// 上阵英雄
#define HERO_BASEOFFSET				1000	// 上阵英雄索引起始
#define HERO_COLISEUMOFFSET			2000	// 竞技场索引起始
#define NATIONHERO_MAX				15		// 国家名将数量
#define SHAPE_MAX					32		// 形象
#define ACTOR_BLACKLIST_MAX			50		// 黑名单
#define FIGHT_JSON_MAX	16384	// 战斗json数据长度

// 推送秘钥长度
#define CLIENT_PUSHKEY_MAX	256

#define ACTOR_CHECK_INDEX(actor_index)		{if( actor_index < 0 || actor_index >= g_maxactornum ) return -1;}
#define CITY_CHECK_INDEX(city_index)		{if( city_index < 0 || city_index >= g_city_maxcount ) return -1;}
#define ARMY_CHECK_INDEX(army_index)		{if( army_index < 0 || army_index >= g_army_maxcount ) return -1;}

#define ATTACK( level, attack, attack_growth ) ( attack + (int)((level-1)*(attack_growth/15.0f)) )
#define DEFENSE( level, defense, defense_growth ) ( defense + (int)((level-1)*(defense_growth/15.0f)) )
#define TROOPS( level, troops, troops_growth ) ( troops + (int)((level-1)*(troops_growth/15.0f)) )

// 闰年
#define IS_LEAP_YEAR(y)  (((y) % 4 == 0 && (y) % 100 != 0) || (y) % 400 == 0)

/* 配置文件 */
typedef struct _sconfig {
	int area_code;
	int server_code;
	unsigned long ipaddress;
	unsigned short port;
	unsigned long user_ipaddress;
	unsigned short user_port;
	char user_host[32];
	unsigned long global_ipaddress;
	unsigned short global_port;

	unsigned long ds_ipaddress;
	unsigned short ds_port;

	char datadb_host[64];
	char datadb_user[32];
	char datadb_pass[32];
	char datadb_database[32];

	char gamedb_host[64];
	char gamedb_user[32];
	char gamedb_pass[32];
	char gamedb_database[32];

	char logdb_host[64];
	char logdb_user[32];
	char logdb_pass[32];
	char logdb_database[32];

	char redis_host[64];
	short redis_port;

	char server_name[32];
	int server_start_date;
	int server_start_hour;
	int server_start_min;
	int max_connection;
	int max_citycount;
	int max_mapunitcount;
	int server_exit_week;
	int server_exit_hour;
	int server_exit_min;
	char noprint;
	char showflag;
	char showcdkey;
	char showvip;
	char showmcard;
	char use_zh;
	char usegate;
	char usenetlog;
	char servplat;

	char log_host[64];
	unsigned short log_port;
	char log_platform[16];

	union
	{
		int value; 
		struct
		{
			unsigned char m1;
			unsigned char m2;
			unsigned short m3;
		}ss;
	}minver;
} SConfig;

void sendtoclient( int actor_index, char *tmpbuf, int size );
void sendtoclient_team( int actor_index, char *tmpbuf, int size );
#endif
