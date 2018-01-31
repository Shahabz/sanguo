#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>

#include "db.h"
#include "dbredis.h"
#include "account.h"
#include "gameproc.h"
#include "actor.h"
#include "script.h"
#include "utils.h"
#include "gamelog.h"
#include "define.h"
#include "client.h"
#include "server_process_auto.h"
#include "login.h"
#include "timegmcmd.h"
#include "wqueue.h"
#include "server_netrecv_auto.h"
#include "actor_notify.h"
#include "system.h"
#include "item.h"
#include "equip.h"
#include "global.h"
#include "map.h"
#include "mapunit.h"
#include "zoneunit.h"
#include "gmcmd.h"
#include "award.h"
#include "global_netprocess.h"
#include "script_auto.h"
#include "activity.h"
#include "script_auto.h"
#include "global_cmdqueue.h"
#include "city.h"
#include "quest.h"
#include "world_quest.h"
#include "world_boss.h"
#include "hero.h"
#include "chat.h"
#include "army.h"
#include "army_group.h"
#include "map_zone.h"
#include "map_town.h"
#include "map_enemy.h"
#include "map_res.h"
#include "map_event.h"
#include "map_call.h"
#include "nation.h"
#include "nation_hero.h"
#include "king_war.h"
#include "rank.h"
#include "pay.h"

#ifndef WIN32 // 这些头文件用来看ulimit设置的
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif

extern MYSQL *myGame;
SConfig g_Config;
extern Global global;

int g_speed = 0;
int g_sec = 0;	// 秒针

int g_mysql_wait = 0;
int g_save_wait = 0;

int g_last_cmd = 0;

int g_maxactornum = 512;
int g_frame_count;
extern int g_nReadQueueHead;
extern int g_nReadQueueTail;
extern Actor *g_actors;
extern char g_user_queue_logic;

#ifndef WIN32
extern int linuxmsg_recv();
#endif

extern int serv_setstat( int stat );

int process_init( int max_connection )
{
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
#ifndef WIN32
	struct rlimit out_data={0};
	if ( getrlimit( RLIMIT_CORE, &out_data ) == 0 )
	{
		if ( out_data.rlim_cur != -1 )
		{
			out_data.rlim_cur = -1;
			out_data.rlim_max = -1;
			if ( setrlimit( RLIMIT_CORE, &out_data ) != 0 )
			{
				printf_msg( "setrlimit( RLIMIT_CORE, &out_data ) Error!" );
				return -1;
			}
		}
	}
	else
	{
		printf_msg( "getrlimit( RLIMIT_CORE, &out_data ) Error!" );
		return -1;
	}

	if ( getrlimit( RLIMIT_NOFILE, &out_data ) == 0 )
	{
		if ( out_data.rlim_cur < max_connection )
		{
			out_data.rlim_cur = 8196;
			out_data.rlim_max = 8196;
			if ( setrlimit( RLIMIT_NOFILE, &out_data ) != 0 )
			{
				printf_msg( "setrlimit( RLIMIT_NOFILE, &out_data ) Error!" );
				return -1;
			}
		}
	}
	else
	{
		printf_msg( "getrlimit( RLIMIT_NOFILE, &out_data ) Error!" );
		return -1;
	}

	// 再次检测
	if ( getrlimit( RLIMIT_CORE, &out_data ) == 0 )
	{
		if ( out_data.rlim_cur != -1 )
		{
			printf_msg( "getrlimit( RLIMIT_CORE, &out_data ) too small!" );
			return -1;
		}
	}
	else
	{
		printf_msg( "getrlimit( RLIMIT_CORE, &out_data ) Error!" );
		return -1;
	}

	if ( getrlimit( RLIMIT_NOFILE, &out_data ) == 0 )
	{
		if ( out_data.rlim_cur < max_connection )
		{
			printf_msg( "getrlimit( RLIMIT_NOFILE, &out_data ) too small!" );
			return -1;
		}
	}
	else
	{
		printf_msg( "getrlimit( RLIMIT_NOFILE, &out_data ) Error!" );
		return -1;
	}
#endif
	g_frame_count = 0;
	g_maxactornum = max_connection;

	if ( g_Config.server_code > 2000 )
	{
		printf_msg( "g_Config.server_code > 2000!" );
		return -1;
	}
	serv_setstat( 2 );
	// 队列初始化
	init_wqueue();

	// 执行队列初始化
	exec_queue_init();
	brush_enemy_queue_init();

	// 数据库初始化
	if ( db_init() >= 0 )
		printf_msg( "Mysql Module ready!" );
	else
	{
		printf_msg( "Mysql Module Error!" );
		return -1;
	}
	serv_setstat( 4 );

	/*if ( dbredis_init() >= 0 )
		printf_msg( "Redis Module ready!" );
	else
	{
		printf_msg( "Redis Module Error!" );
		return -1;
	}*/
	serv_setstat( 5 );

	// 脚本初始化
	if ( lua_init( 0 ) >= 0 )
		printf_msg( "Lua Module ready!" );
	else
	{
		printf_msg( "Lua Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 6 );

	// account初始化
	if ( account_init() >= 0 )
		printf_msg( "Account Module ready!" );
	else
	{
		printf_msg( "Account Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 6 );

	// 全局数据初始化
	if ( global_init() >= 0 )
		printf_msg( "Global Module ready!" );
	else
	{
		printf_msg( "Global Module Error!" );
		return -1;
	}
	serv_setstat( 5 );

	// 平台信息初始化
	if ( platinfo_init_auto() >= 0 )
		printf_msg( "Plat Module ready!" );
	else
	{
		printf_msg( "Plat Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 7 );

	//// 商品初始化
	//if ( paygoods_init() >= 0 )
	//	printf_msg( "PayGoods Module ready!" );
	//else
	//{
	//	printf_msg( "PayGoods Module Error!" );
	//	return -1;
	//}
	//LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	//paycountry_init();
	//serv_setstat( 10 );

	//// 支付商店初始化
	//if ( paystore_init() >= 0 )
	//	printf_msg( "PayStore Module ready!" );
	//else
	//{
	//	printf_msg( "PayStore Module Error!" );
	//	return -1;
	//}
	//LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	//serv_setstat( 11 );

	// 角色数据初始化
	if ( actors_init() >= 0 )
		printf_msg( "Actor Module ready!" );
	else
	{
		printf_msg( "Actor Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 13 );

	// 登陆线程启动
	if ( login_init() >= 0 )
		printf_msg( "Login Module ready!" );
	else
	{
		printf_msg( "Login Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 14 );

	// GMTOOLS
	if( gm_init() < 0 )
	{
		printf_msg( "GM Module Error!" );
		return -1;
	}
	LOGI("%s-%d",__FUNCTION__,__LINE__);
	//global_process_init();
	serv_setstat( 15 );

	// 道具数据初始化
	if ( itemkind_init() < 0 )
	{
		printf_msg( "ItemKind Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 装备数据初始化
	if ( equipinfo_init_auto() < 0 )
	{
		printf_msg( "equipinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 装备洗练数据初始化
	if ( equipwashinfo_init_auto() < 0 )
	{
		printf_msg( "equipwashinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 装备洗练数据初始化
	if ( equipwashrule_init_auto() < 0 )
	{
		printf_msg( "equipwashrule_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 英雄数据初始化
	if ( heroinfo_init_auto() < 0 )
	{
		printf_msg( "heroinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 英雄突破数据初始化
	if ( herocolorupinfo_init_auto() < 0 )
	{
		printf_msg( "herocolorupinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );
	
	// 英雄技能数据初始化
	if ( heroskillinfo_init_auto() < 0 )
	{
		printf_msg( "heroskillinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 国器初始化
	if ( nationequipinfo_init_auto() < 0 )
	{
		printf_msg( "nationequipinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 国器初始化
	if ( nationequipopen_init_auto() < 0 )
	{
		printf_msg( "nationequipopen_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 国器初始化
	if ( nationequipremake_init_auto() < 0 )
	{
		printf_msg( "nationequipremake_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 国家升级初始化
	if ( nationupgrade_init_auto() < 0 )
	{
		printf_msg( "nationupgrade_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 爵位初始化
	if ( nationplace_init_auto() < 0 )
	{
		printf_msg( "nationplace_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 官职初始化
	if ( nationofficial_init_auto() < 0 )
	{
		printf_msg( "nationofficial_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 国家任务初始化-个人
	if ( nationquest_init_auto() < 0 )
	{
		printf_msg( "nationquest_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 国家任务初始化-集体
	if ( nationmission_init_auto() < 0 )
	{
		printf_msg( "nationmission_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 国家名将
	if ( nationheroinfo_init_auto() < 0 )
	{
		printf_msg( "nationheroinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 英兵种相克数据初始化
	if ( herocorpsmulinfo_init_auto() < 0 )
	{
		printf_msg( "herocorpsmulinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 英雄寻访
	if ( herovisit_init_auto() < 0 )
	{
		printf_msg( "herovisit_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 英雄寻访
	if ( hero_visit_init() < 0 )
	{
		printf_msg( "hero_visit_init Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 怪物信息初始化
	if ( monsterinfo_init_auto() < 0 )
	{
		printf_msg( "monsterinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 奖励包数据初始化
	if ( awardgroup_init() < 0 )
	{
		printf_msg( "AwardGroup Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 升级数据初始化
	if ( upgradeinfo_init_auto() < 0 )
	{
		printf_msg( "upgradeinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 建筑升级数据初始化
	if ( buildingupgrade_init_auto() < 0 )
	{
		printf_msg( "buildingupgrade_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 建筑升级数据初始化
	if ( buildingresunlock_init_auto() < 0 )
	{
		printf_msg( "buildingresunlock_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 科技
	if ( techinfo_init_auto() < 0 )
	{
		printf_msg( "techinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 雇佣
	if ( officialforging_init_auto() < 0 )
	{
		printf_msg( "officialforging_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 雇佣
	if ( officialgov_init_auto() < 0 )
	{
		printf_msg( "officialgov_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 雇佣
	if ( officialtech_init_auto() < 0 )
	{
		printf_msg( "officialtech_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// VIP
	if ( vipinfo_init_auto() < 0 )
	{
		printf_msg( "vipinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// VIP礼包
	if ( vipbag_init_auto() < 0 )
	{
		printf_msg( "vipbag_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// VIP商店
	if ( vipshop_init_auto() < 0 )
	{
		printf_msg( "vipshop_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 多国支付价格初始化
	if ( payprice_init() >= 0 )
		printf_msg( "PayPrice Module ready!" );
	else
	{
		printf_msg( "PayPrice Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 8 );

	// 多国支付地区信息
	if ( paycountry_init() >= 0 )
		printf_msg( "paycountry_init Module ready!" );
	else
	{
		printf_msg( "paycountry_init Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 9 );

	// 商品初始化
	if ( paygoods_init_auto() >= 0 )
		printf_msg( "paygoods_init_auto Module ready!" );
	else
	{
		printf_msg( "paygoods_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 10 );

	// 支付商店初始化
	if ( paystore_init_auto() >= 0 )
		printf_msg( "paystore_init_auto Module ready!" );
	else
	{
		printf_msg( "paystore_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 11 );

	// 募兵时长
	if ( trainlonginfo_init_auto() < 0 )
	{
		printf_msg( "trainlonginfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 募兵队列
	if ( trainqueueinfo_init_auto() < 0 )
	{
		printf_msg( "trainqueueinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 任务
	if ( questinfo_init_auto() < 0 )
	{
		printf_msg( "questinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 任务对话
	if ( questtalk_init_auto() < 0 )
	{
		printf_msg( "questtalk_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 城墙守卫
	if ( cityguardinfo_init_auto()  < 0 )
	{
		printf_msg( "cityguardinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 购买体力所需钻石
	if ( bodytokeninfo_init_auto() < 0 )
	{
		printf_msg( "bodytokeninfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 材料生产
	if ( materialmakeinfo_init_auto() < 0 )
	{
		printf_msg( "materialmakeinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 副本
	if ( storyinfo_init_auto() < 0 )
	{
		printf_msg( "storyinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 世界地图-区域
	if ( mapzoneinfo_init_auto() < 0 )
	{
		printf_msg( "mapzoneinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 世界地图-城镇
	if ( maptowninfo_init_auto() < 0 )
	{
		printf_msg( "maptowninfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 世界地图-流寇
	if ( mapenemyinfo_init_auto() < 0 )
	{
		printf_msg( "mapenemyinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 世界地图-资源点
	if ( mapresinfo_init_auto() < 0 )
	{
		printf_msg( "mapresinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 世界地图-随机事件
	if ( mapeventinfo_init_auto() < 0 )
	{
		printf_msg( "mapeventinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );

	// 世界boss
	if ( worldbossinfo_init_auto() < 0 )
	{
		printf_msg( "worldbossinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );

	// 世界任务
	if ( worldquestinfo_init_auto() < 0 )
	{
		printf_msg( "worldquestinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 世界任务列表
	if ( worldquest_init() < 0 )
	{
		printf_msg( "worldquest_init Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 血战皇城
	if ( kingwarconfig_init_auto() < 0 )
	{
		printf_msg( "kingwarconfig_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 血战皇城据点
	if ( kingwartowninfo_init_auto() < 0 )
	{
		printf_msg( "kingwartowninfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 商店
	if ( shop_init_auto() < 0 )
	{
		printf_msg( "shop_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	// 天气
	if ( weatherinfo_init_auto() < 0 )
	{
		printf_msg( "weatherinfo_init_auto Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 19 );

	activity_init();
	time_gmcmd_init();
	db_closedata();
	sc_Script_Init();
	//pushwork_init();
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 48 );

	// 天气
	weather_load();

	// 装备最大ID初始化
	if ( equip_maxid_init() < 0 )
	{
		printf_msg( "equip_maxid_init Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 英雄最大ID初始化
	if ( hero_maxid_init() < 0 )
	{
		printf_msg( "hero_maxid_init Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 17 );

	// 世界地图初始化（严格顺序要求，不允许改变）
	if ( map_init() < 0 )
	{
		printf_msg( "WorldMap Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 101 );

	// 加地图地区（严格顺序要求，不允许改变）
	if ( map_zone_load() < 0 )
	{
		printf_msg( "MapZone Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 107 );

	// 加载所有地图显示单元结构，在城池和出征军队之前初始化（严格顺序要求，不允许改变）
	if ( mapunit_init() < 0 )
	{
		printf_msg( "MapUnit Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 102 );

	// 加载所有地图地区的显示单元结构，在城池和出征军队之前初始化（严格顺序要求，不允许改变）
	if ( zoneunit_init() < 0 )
	{
		printf_msg( "ZoneUnit Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 102 );

	// 加载所有城市（严格顺序要求，不允许改变）
	if ( city_load() < 0 )
	{
		printf_msg( "CityLoad Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 107 );

	// 加载城镇（严格顺序要求，不允许改变）
	if ( map_town_load() < 0 )
	{
		printf_msg( "map_town_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );

	// 加载国家（严格顺序要求，不允许改变）
	if ( nation_load() < 0 )
	{
		printf_msg( "nation_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );

	// 加载流寇（严格顺序要求，不允许改变）
	if ( map_enemy_load() < 0 )
	{
		printf_msg( "map_enemy_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 111 );

	// 加载资源点（严格顺序要求，不允许改变）
	if ( map_res_load() < 0 )
	{
		printf_msg( "map_res_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 112 );

	// 加载国家名将（严格顺序要求，不允许改变）
	if ( nation_hero_load() < 0 )
	{
		printf_msg( "nation_hero_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 111 );

	// 加载皇城血战据点（严格顺序要求，不允许改变）
	if ( kingwar_town_load() < 0 )
	{
		printf_msg( "kingwar_town_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );

	// 加载所有部队（严格顺序要求，不允许改变）
	if ( army_load() < 0 )
	{
		printf_msg( "army_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 116 );

	// 加载所有集结（严格顺序要求，不允许改变）
	if ( armygroup_load() < 0 )
	{
		printf_msg( "armygroup_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 116 );

	// 加载所有地图事件（严格顺序要求，不允许改变）
	if ( map_event_load() < 0 )
	{
		printf_msg( "map_event_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 116 );

	// 加载世界boss（严格顺序要求，不允许改变）
	if ( world_boss_load() < 0 )
	{
		printf_msg( "world_boss_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );

	// 加载地图召唤
	if ( map_call_load() < 0 )
	{
		printf_msg( "map_call_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 116 );

	// 加载推送的礼包
	if ( paybag_load() < 0 )
	{
		printf_msg( "paybag_load Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );

	// 全局数据
	if ( world_data_init() < 0 )
	{
		printf_msg( "TalkCacheLoad Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 122 );

	// 聊天缓存
	chat_cache_load();

	// 国家日志缓存
	nationlog_cache_load();

	// 刷地图
	sc_OnWorldMapBrush();
	serv_setstat( 117 );
	
	// 排行榜
	if ( rank_init() < 0 )
	{
		printf_msg( "Rank Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 118 );

	// 国家荣誉排行榜
	if ( nation_rank_calc() < 0 )
	{
		printf_msg( "nation_rank_calc Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 118 );

	// 数据库多线程启动
	if ( dbwork_start() >= 0 )
		printf_msg( "dbwork Module ready!" );
	else
	{
		printf_msg( "dbwork Module Error!" );
		return -1;
	}
	LOGI( "%s-%d", __FUNCTION__, __LINE__ );
	serv_setstat( 119 );
	return 0;
}

// 服务器关闭
void process_close()
{
	int tmpi;
	// 关闭每个客户端
	for ( tmpi = 0; tmpi < g_maxactornum; tmpi++ )
	{
		actor_remove( tmpi );
		printf_msg( "actor_save %d/%d\r", tmpi + 1, g_maxactornum );
	}
	printf_msg( "\n" );


	// 开启一个新事务
	mysql_query( myGame, "START TRANSACTION" );

	// 所有城池保存
	city_save( NULL );
	printf_msg( "\n" );

	// 所有国家保存
	nation_save( NULL );
	printf_msg( "\n" );

	// 所有部队保存
	army_save( NULL );
	printf_msg( "\n" );

	// 所有集结保存
	armygroup_save( NULL );
	printf_msg( "\n" );

	// 所有地区保存
	map_zone_save( NULL );
	printf_msg( "\n" );

	// 所有城镇保存
	map_town_save( NULL );
	printf_msg( "\n" );

	// 所有流寇保存
	map_enemy_save( NULL );
	printf_msg( "\n" );

	// 所有资源点保存
	map_res_save( NULL );
	printf_msg( "\n" );

	// 所有随机事件点保存
	map_event_save( NULL );
	printf_msg( "\n" );

	// 所有召唤保存
	map_call_save( NULL );
	printf_msg( "\n" );

	// 所有国家名将保存
	nation_hero_save( NULL );
	printf_msg( "\n" );

	// 所有世界boss
	world_boss_save( NULL );
	printf_msg( "\n" );
	
	// 所有血战皇城据点保存
	kingwar_town_save( NULL );
	printf_msg( "\n" );

	// 提交
	mysql_query( myGame, "COMMIT;" );

	dbwork_stop();
	db_closegame();
	dbredis_close();
	lua_exit();
	/*pushwork_destory();*/
}

// 返回-1,表示可以直接清除这个client
int process_onclose( int client_index )
{
	return actor_del( client_index );
}

int process_error( int client_index, int err )
{
	return 0;
}

int actors_save_everyone( char type );
int process_ondestory()
{
	// 所有角色数据保存
	actors_save_everyone( 1 );
	return 0;
}
int Clear_Welcome();
int process_logic_wait_in();
int process_logic_wait_out();

int g_nExitHour = -1;
int g_nExitMin = -1;
int g_nExitNotify = 0;
time_t g_tExitTime = 0;
void process_exitlogic()
{
	static int s_count = 0;
	time_t t;
	char szMsg[MAX_PATH];
	int min = -1;

	if ( g_tExitTime <= 0 || g_nExitHour < 0 || g_nExitHour > 23 || g_nExitMin < 0 || g_nExitMin > 59 )
		return;
	time( &t );

	if ( g_tExitTime - t <= 0 )	// 到点
		min = 0;
	else if ( g_tExitTime - t <= 60 )	// 1分钟
		min = 1;
	else if ( g_tExitTime - t <= 180 )	// 3分钟
		min = 3;
	else if ( g_tExitTime - t <= 300 )	// 5分钟
		min = 5;
	else if ( g_tExitTime - t <= 600 )	// 10分钟
		min = 10;
	else if ( g_tExitTime - t <= 14400 )	// 4小时以内(240分钟)
		min = -2;

	if ( min > 0 )
	{
		sprintf( szMsg, "亲爱的玩家朋友你们好！因正常维护需要，服务器即将在%d分钟内停机，为避免意外，请尽早下线，祝您游戏愉快！", min );
		//system_talk( szMsg, TALK_CHANNEL_SYSTEM );
	}
	else if ( min == 0 )
	{
		g_nExitHour = -1;
		g_nExitMin = -1;
		g_tExitTime = 0;
		server_exit();
	}
	else if ( min == -2 )
	{
		if ( g_nExitNotify && (s_count % 60) == 0 )
		{
			sprintf( szMsg, "亲爱的玩家朋友你们好！按照正常维护的需要，服务器将于%d点%d分左右停机！", g_nExitHour, g_nExitMin );
			//system_talk( szMsg, TALK_CHANNEL_SYSTEM );
		}
		s_count++;
	}
}

// 0~6 0~23 0~60 0~1
int process_set_exit( int weekday, int hour, int min, int isnotify )
{
	time_t t;
	struct tm *newtime;
	int weekdiff;

	if ( hour < 0 || min < 0 || hour > 23 || min > 59 )
	{
		g_nExitHour = -1;
		g_nExitMin = -1;
		g_tExitTime = 0;
		return -1;
	}

	time( &t );
	newtime = localtime( &t );

	if ( weekday < 0 || weekday > 6 )	// 当天
	{
		g_tExitTime = t + (hour - newtime->tm_hour) * 60 * 60 + (min - newtime->tm_min) * 60;
		if ( g_tExitTime < t )
			g_tExitTime += 24 * 60 * 60;
	}
	else
	{
		weekdiff = weekday - newtime->tm_wday;
		if ( weekdiff <= 0 )
			weekdiff += 7;
		g_tExitTime = t + (hour - newtime->tm_hour) * 60 * 60 + (min - newtime->tm_min) * 60 + weekdiff * 24 * 60 * 60;
	}

	g_nExitHour = hour;
	g_nExitMin = min;
	g_nExitNotify = isnotify;

	return 0;
}

// 所有需要整点执行的任务可以放到这里来
// 如果小时为-1，表示首次执行，依据情形可略过
int process_oclock_process( int hour )
{
	if ( hour == 0 )
	{ // 月卡发放
		paycard_give();
		// 刷新国家荣誉任务
		nation_mission_update();

		// 刷新国家荣誉排行榜
		if ( system_getweek() == 5 )
		{
			nation_rank_update();
		}
	}
	else if ( hour == 1 )
	{
	}
	
	
	// 减忠诚
	if ( hour == 4 || hour == 8 || hour == 12 || hour == 16 || hour == 20 )
	{
		nation_hero_subloyal();
	}

	if ( hour == global.nation_quest_timer[0] || hour == global.nation_quest_timer[1] || hour == global.nation_quest_timer[2] )
	{ // 刷新国家任务
		nation_quest_update();
	}

	if ( hour == global.town_owner_award )
	{ // 城主奖励
		map_town_owner_award();
	}
	sc_OnClockProcess( hour );
	return 0;
}

static int nextclock_timestump = 0;
static int nexclock_hour = -1;
// 整点判定操作（将整点需要处理的事件放在这里来）
int process_oclock_check()
{
	time_t t;
	time( &t );

	// 当前时间已经超过下个时钟的时间了
	if ( t > nextclock_timestump )
	{
		process_oclock_process( nexclock_hour );
		// 将时间向后推移一个小时
		t += 3600;
		// 获取时间
		struct tm *newtime;
		newtime = localtime( &t );
		// 生成新的整点
		struct tm BeginTm = { 0 };
		BeginTm.tm_year = newtime->tm_year;
		BeginTm.tm_mon = newtime->tm_mon;
		BeginTm.tm_mday = newtime->tm_mday;
		BeginTm.tm_hour = newtime->tm_hour;
		BeginTm.tm_min = 0;
		BeginTm.tm_sec = 0;
		// 重新生成时间戳
		nextclock_timestump = (int)mktime( &BeginTm );
		nexclock_hour = newtime->tm_hour;
	}
	return 0;
}

// 每秒执行30次,目前设置是30帧逻辑
int process_logic()
{
	int tick = 0;
	g_speed++;

	// 每帧执行的延迟队列操作
	for ( int tmpi = 0; tmpi < 16; tmpi++ )
	{
		if ( exec_queue_fetch() < 0 )
			break;
	}

	// 刷流寇队列
	brush_enemy_queue_fetch();

	// 1/10秒才允许一次mysql操作
	if ( g_speed % 6 == 0 )
	{
		process_logic_wait_in();
	}

	// 1/6秒才允许一次写操作
	if ( g_speed % 10 == 1 )
	{
		process_logic_wait_out();
	}

	// 10秒一次客户端逻辑
	if ( g_speed % 300 == 0 )
	{
		client_logic();
	}

	// 5帧一次逻辑
	if ( g_speed % 5 == 0 )
	{
		actors_logic(); // 执行所有的角色logic
		map_logic(); // 世界地图的逻辑,主要处理区域
	}

	//	1秒钟一次逻辑
	tick = g_speed % 30;
	if ( tick == 0 )
	{
		g_sec++;		// 秒针可当做服务器启动多少秒了
		dbwork_fetchcomplete();
		globalcmd_fetch();
#ifndef WIN32
		linuxmsg_recv();
#endif
	}
	else if ( tick == 1 )
	{
		city_logic_sec( 0, 0 );
	}
	//else if ( tick == 2 )
	//{
	//	city_logic_sec();
	//}
	//else if ( tick == 3 )
	//{
	//	city_logic_sec();
	//}
	else if ( tick == 4 )
	{
		map_town_alllogic();
	}
	else if ( tick == 5 )
	{
		army_alllogic();
		armygroup_alllogic();
	}
	else if ( tick == 6 )
	{
		kingwar_activity_logic();
		nation_official_logic();
		map_call_logic();
		nation_hero_logic();
	}
	
	// 1分钟一次逻辑
	tick = g_speed % 1800;
	if ( tick == 0 )
	{
		wcount(); // 记录人数
		map_res_logic(); // 资源点闲置删除检查
		sc_Script_Timer();
		process_exitlogic();
		process_oclock_check();
		weather_logic();
		time_gmcmd_logic();
		activity_logic();
	}
	else if ( tick == 10 )
	{
		
	}
	else if ( tick == 11 )
	{
		rank_reset();
		rank_server();
		paybag_logic();
	}

	if ( g_speed > 108000 )
	{
		tick = g_speed % 108000;
		if ( tick == 0 )
		{ // 每小时一次逻辑
			sc_Script_Hour();
		}
	}
	g_frame_count++;
	return 0;
}

int process_client( int client_index, char *tmpbuf, int readsize, int exec_code )
{
	short cmd;
	char *ptr;
	int tmpsize;
	int packsize;
	int allsize;
	char clientstat;

	//if( readsize < sizeof(short) )
	//	return -1;
	//ptr = tmpbuf; 
	//cmd = *((short *)ptr); ptr+=sizeof(short);
	//tmpsize = readsize - sizeof(short);

	ptr = tmpbuf;
	allsize = 0;

	g_last_cmd = -100;

	if ( client_getwait( client_index ) )
	{
		write_gamelog( "need wait:%d", client_index );
		return -1;
	}

	clientstat = client_getstat( client_index );
	while ( allsize < readsize )
	{
		packsize = *((short *)ptr); ptr += sizeof(short);
		if ( packsize <= 0 || packsize>1024 )
			break;
		cmd = *((short *)ptr); ptr += sizeof(short);
		tmpsize = packsize - sizeof(short);
		g_last_cmd = cmd;
		if ( client_index >= 0 && clientstat < CLIENT_STAT_LOGIN )
		{
			// 当玩家尚未登录( 仅仅可以调用登录部分指令 )
			if ( cmd != CMDC_LOGIN )
			{
				write_gamelog( "wrong command(no login):%d ip:%s", cmd, client_getip(client_index) );
				net_closesock_direct( client_index );
				return -1;
			}
		}
		else if ( clientstat == CLIENT_STAT_LOGIN )
		{
			// 当玩家仅仅登录而未进入游戏(可以创建角色，可以列角色，可以进入游戏) 非这些协议一律踢掉
			if ( cmd != CMDC_CREATE && cmd != CMDC_LIST && cmd != CMDC_DELETE && cmd != CMDC_ENTERGAME /*&& cmd != CMDC_HEARTBEAT*/ )
			{
				net_closesock_direct( client_index );
				return -1;
			}
		}
		else if ( clientstat == CLIENT_STAT_VER_ERR )
		{
			return -1;
		}
		if ( proc_command_S( client_index, cmd, ptr, tmpsize, exec_code ) < 0 )
		{
			if ( clientstat == CLIENT_STAT_LOGIN )
			{
				SClientInfo ClientInfo;
				char szUserID[21];
				client_getuserinfo( client_index, &ClientInfo );
				lltoa( ClientInfo.userid, szUserID, 10 );
				write_gamelog( "wrong command:%d userid:%s ip:%s", cmd, szUserID, client_getip(client_index) );
				net_closesock_direct( client_index );
			}
			else if ( clientstat == CLIENT_STAT_ENTERGAME )
			{
				write_gamelog( "wrong command:%d ip:%s", cmd, client_getip(client_index) );
				actor_remove( client_index );
			}
			else
			{
				write_gamelog( "wrong command:%d ip:%s", cmd, client_getip(client_index) );
				net_closesock_direct( client_index );
			}
			return -1;
		}
		ptr += tmpsize;
		allsize += packsize + sizeof(short);
	}
	return 0;
}

//-----------------------------------------------------------------------------
// process_logic_wait_in
// 函数说明: 角色进入游戏的队列
//-----------------------------------------------------------------------------
int process_logic_wait_in()
{
	int client_index;
	short cmd;
	char tmpbuf[MAX_CMDPACKAGE_SIZE];
	int size;
	int exec_code;

	g_mysql_wait = 1;
	if ( fetch_wqueue( &client_index, &cmd, tmpbuf, &size, &exec_code, 0 ) == 0 )
	{
		//		if( cmd != WCMD_LEAVE && client_get_exec_code( client_index ) != exec_code )
#ifndef _USEGATE
		if ( client_get_exec_code( client_index ) != exec_code )
			return 0;
#endif
		// 开始处理一个
		switch ( cmd )
		{
		case WCMD_ENTERGAME:
			return netrecv_entergame_S( client_index, tmpbuf, size );
			break;
		case WCMD_CREATE:
			return netrecv_create_S( client_index, tmpbuf, size );
			break;
		case WCMD_LIST:
			return netrecv_list_S( client_index, tmpbuf, size );
			break;
		}
	}
	// 队列是空的，不做任何处理
	return 0;
}

//-----------------------------------------------------------------------------
// process_logic_wait_out
// 函数说明: 角色退出游戏，保存数据的队列
//-----------------------------------------------------------------------------
int process_logic_wait_out()
{
	int client_index;
	short cmd;
	char tmpbuf[MAX_CMDPACKAGE_SIZE];
	int size;
	int exec_code;

	g_save_wait = 1;
	if ( fetch_wqueue( &client_index, &cmd, tmpbuf, &size, &exec_code, 1 ) == 0 )
	{
		// 开始处理一个
		switch ( cmd )
		{
		case WCMD_LEAVE:
			return actor_remove( client_index );
			break;
		case WCMD_SAVE:
			return actor_real_saveall( client_index, NULL );
			break;
		}
	}

	// 队列是空的，不做任何处理
	return 0;
}

int process_dbreload()
{
	db_opendata();

	global_reload();
	awardgroup_reload();
	platinfo_reload_auto();
	itemkind_reload();

	db_closedata();
	return 0;
}
