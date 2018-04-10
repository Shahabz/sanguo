#include <stdio.h>
#include <time.h>
#include "system_askinfo.h"
#include "actor.h"
#include "gameproc.h"
#include "utils.h"
#include "db.h"
#include "client.h"
#include "area.h"
#include "map.h"
#include "mapunit.h"
#include "actor_notify.h"
#include "server_structsend_auto.h"
#include "system.h"
#include "actor_send.h"
#include "item.h"
#include "equip.h"
#include "global.h"
#include "activity.h"
#include "script_auto.h"
#include "login.h"
#include "city.h"
#include "city_tech.h"
#include "quest.h"
#include "world_quest.h"
#include "world_boss.h"
#include "building.h"
#include "hero.h"
#include "story.h"
#include "map_zone.h"
#include "map_town.h"
#include "map_enemy.h"
#include "map_res.h"
#include "map_event.h"
#include "map_call.h"
#include "army_march.h"
#include "army_group.h"
#include "king_war.h"
#include "shop.h"
#include "pay.h"
#include "vip.h"
#include "nation_equip.h"
#include "nation.h"
#include "nation_hero.h"
#include "wishing.h"
#include "actor_friend.h"
#include "activity_04.h"
#include "girl.h"

extern Actor *g_actors;
extern int g_maxactornum;
extern SConfig g_Config;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;

extern Global global;
extern Map g_map;

// 客户端脚本直接发来的消息
int system_askinfo( int actor_index, int msgid, char *pstr, int *pvalue )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	switch ( msgid )
	{
	case ASKINFO_NORMAL:
		break;
	case ASKINFO_CHANGESHAPE:
		actor_changeshape( actor_index, (char)pvalue[0] );
		break;
	case ASKINFO_CHANGENAME:
		actor_changename( actor_index, pstr, pvalue[0] );
		break;
	case ASKINFO_CITYGUARD:
		if ( pvalue[0] == 0 )
		{// 列表
			city_guard_sendlist( actor_index );
		}
		else if ( pvalue[0] == 1 )
		{ // 招募守卫
			city_guard_call( g_actors[actor_index].city_index );
		}
		else if ( pvalue[0] == 2 )
		{ // 首守卫升级
			city_guard_upgrade( g_actors[actor_index].city_index, pvalue[1] );
		}
		else if ( pvalue[0] == 3 )
		{ // 清除守卫招募冷却
			city_guard_clearcd( g_actors[actor_index].city_index );
		}
		else if ( pvalue[0] == 4 )
		{ // 补血
			city_guard_soldiers( g_actors[actor_index].city_index, pvalue[1] );
		}
		
		break;
	case ASKINFO_QUEST:
		if ( pvalue[0] == 0 )
		{
			quest_sendlist( actor_index );
		}
		else if ( pvalue[0] == 1 )
		{ // 获取任务奖励
			quest_getaward( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 2 )
		{ // 完成特殊类型的任务
			quest_client_op( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 3 )
		{ // 发送奖励领取界面
			quest_sendawardinfo( actor_index, pvalue[1], QUEST_TYPE_BRANCH );
		}
		else if ( pvalue[0] == 10 )
		{ // 世界任务
			worldquest_sendinfo( actor_index );
		}
		else if ( pvalue[0] == 11 )
		{ // 世界任务奖励
			worldquest_sendaward( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 12 )
		{ // 世界任务领取奖励
			worldquest_getaward( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 13 )
		{ // 世界boss信息
			worldboss_sendinfo( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 14 )
		{ // 世界boss信息
			worldboss_sendbattleinfo( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 15 )
		{ // 首次免费前往州城
			map_zone_goto_zc( actor_index );
		}
		break;
	case ASKINFO_BUILDING:
		if ( pvalue[0] == 0 )
		{ // 升级信息
			building_send_upgradeinfo( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 1 )
		{ // 升级
			building_upgrade( g_actors[actor_index].city_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 2 )
		{ // 升级免费加速
			building_workerfree( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 3 )
		{ // 购买商用建造队列
			building_workerbuy( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 4 )
		{ // 加速奖励的加速
			building_awardquick_get( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 5 )
		{ // 改建
			building_rebuild( g_actors[actor_index].city_index, pvalue[1], pvalue[2], pvalue[3] );
		}
		else if ( pvalue[0] == 6 )
		{ // 资源点建设
			building_giveres_build( actor_index, pvalue[1], pvalue[2] );
		}
		break;
	case ASKINFO_LEVY:
		if ( pvalue[0] == 0 )
		{ // 征收信息
			city_levy_sendinfo( actor_index );
		}
		else if ( pvalue[0] == 1 )
		{ // 征收
			city_levy( actor_index );
		}
		break;
	case ASKINFO_TRAIN:
		if ( pvalue[0] == 0 )
		{ // 招募信息
			city_train_sendinfo( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 1 )
		{ // 招募
			city_train( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 3 )
		{ // 取消
			city_train_cancel( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 4 )
		{ // 收取
			city_train_get( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 5 )
		{ // 扩建
			city_train_buyqueue( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 6 )
		{ // 购买时长
			city_train_buylong( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 7 )
		{ // 奖励加速领取
			city_train_awardquick_get( actor_index, pvalue[1] );
		}
		
		break;
	case ASKINFO_EQUIPFORGING:
		if ( pvalue[0] == 0 )
		{ // 打造装备
			equip_forging( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 1 )
		{ // 加速打造装备
			equip_forging_quick( actor_index );
		}
		else if ( pvalue[0] == 2 )
		{ // 铁匠加速打造装备
			equip_forging_freequick( actor_index );
		}
		else if ( pvalue[0] == 4 )
		{ // 领取装备
			equip_forging_get( actor_index );
		}
		
		break;
	case ASKINFO_EQUIPWASH:
		break;
	case ASKINFO_EQUIP:
		if ( pvalue[0] == 1 )
		{ // 分解
			equip_resolve( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 2 )
		{ // 装备
			equip_up( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 3 )
		{ // 卸下
			equip_down( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 4 )
		{ // 购买装备栏
			equip_buyext( actor_index );
		}
		else if ( pvalue[0] == 5 )
		{ // 装备洗练次数
			equip_wash_sendinfo( actor_index );
		}
		else if ( pvalue[0] == 6 )
		{ // 装备免费洗练
			equip_wash_free( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 7 )
		{ // 装备钻石洗练
			equip_wash_token( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 8 )
		{ // 装备秘技洗练
			equip_wash_super( actor_index, pvalue[1] );
		}
	
		break;
	case ASKINFO_ITEM:
		if ( pvalue[0] == ITEM_PROCESS_INFO )
		{

		}
		else if ( pvalue[0] == ITEM_PROCESS_USE )
		{
			item_use( actor_index, pvalue[1], pvalue[2], pvalue[3], pvalue[4] );
		}
		else if ( pvalue[0] == ITEM_PROCESS_SELL )
		{
			item_drop( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == ITEM_PROCESS_TOKENUSE )
		{
			item_use_withtoken( actor_index, pvalue[1], pvalue[2], pvalue[3], pvalue[4] );
		}
		else if ( pvalue[0] == ITEM_PROCESS_BUY )
		{
			item_buy( actor_index, pvalue[1], pvalue[2] );
		}
		
		break;
	case ASKINFO_HERO:
		if ( pvalue[0] == 1 )
		{// 补兵
			hero_addsoldiers( actor_index, pvalue[1], PATH_HERO_ADDSOLDIERS );
		}
		else if ( pvalue[0] == 2 )
		{// 使用经验道具
			hero_useexpitem( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 3 )
		{// 上阵
			hero_up( actor_index, pvalue[1], pvalue[2], pvalue[3], pvalue[4] );
		}
		else if ( pvalue[0] == 4 )
		{ // 打开洗髓界面获取信息
			hero_wash_sendinfo( actor_index );
		}
		else if ( pvalue[0] == 5 )
		{ // 免费洗髓
			hero_wash_free( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 6 )
		{ // 至尊洗髓
			hero_wash_token( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 7 )
		{ // 良将突破信息
			hero_colorup_sendinfo( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 8 )
		{ // 良将突破
			hero_colorup( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 9 )
		{// 下阵
			hero_down( actor_index, pvalue[1], 0 );
		}
		else if ( pvalue[0] == 10 )
		{ // 补充御林卫士兵
			hero_guard_soldiers_token( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 11 )
		{ // 神级突破
			hero_godup( actor_index, pvalue[1] );
		}
		
		break;
	case ASKINFO_TECH:
		if ( pvalue[0] == 1 )
		{ // 升级
			city_tech_upgrade( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 2 )
		{ // 加速
			city_tech_quick( actor_index );
		}
		else if ( pvalue[0] == 3 )
		{ // 免费加速
			city_tech_freequick( actor_index );
		}
		else if ( pvalue[0] == 4 )
		{ // 领取
			city_tech_get( actor_index );
		}
		break;
	case ASKINFO_GOV:
		if ( pvalue[0] == 0 )
		{ // 事件
			city_event_sendlist( actor_index );
		}
		break;
	case ASKINFO_OFFICIALHIRE:
		if ( pvalue[0] == 0 )
		{
			city_officialhire( actor_index, pvalue[1], pvalue[2] );
		}
		break;
	case ASKINFO_QUICK:
		if ( pvalue[0] == 0 )
		{
			item_use_quick( actor_index, pvalue[1], pvalue[2], pvalue[3], pvalue[4] );
		}
		break;
	case ASKINFO_ACTOR:
		if ( pvalue[0] == 0 )
		{ // 购买体力
			actor_buybody( actor_index, pvalue[1] );
		}
		else if( pvalue[0] == 1 )
		{ // 重新开始角色
			actor_restart( actor_index );
		}
		else if ( pvalue[0] == 2 )
		{ // 获取角色信息
			actor_search( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 3 )
		{ // 黑名单列表
			actor_blacklist_sendlist( actor_index );
		}
		else if ( pvalue[0] == 4 )
		{ // 黑名单添加
			actor_blacklist_add( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 5 )
		{ // 黑名单删除
			actor_blacklist_del( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 6 )
		{ // 开启关闭
			if ( pvalue[1] == CITY_FUNCTION_AUTO_ADDHP )
			{ // 自动补兵
				if ( pvalue[2] == 0 )
					city_function_close( city_getptr( actor_index ), CITY_FUNCTION_AUTO_ADDHP );
				else if ( pvalue[2] == 1 )
				{
					city_function_open( city_getptr( actor_index ), CITY_FUNCTION_AUTO_ADDHP );
					hero_addsoldiers_audo( city_getptr( actor_index ) );
				}
			}
			else if ( pvalue[1] == CITY_FUNCTION_BATTLE_ADDHP )
			{ // 战前自动补兵
				if ( pvalue[2] == 0 )
					city_function_close( city_getptr( actor_index ), CITY_FUNCTION_BATTLE_ADDHP );
				else if ( pvalue[2] == 1 )
					city_function_open( city_getptr( actor_index ), CITY_FUNCTION_BATTLE_ADDHP );
			}
		}
		break;
	case ASKINFO_MATERIALMAKE:
		if ( pvalue[0] == 0 )
		{ // 获取信息
			city_material_sendinfo( actor_index, 1 );
		}
		else if ( pvalue[0] == 1 )
		{ // 制造
			city_material_make( actor_index, pvalue[1], pvalue[2], 0 );
		}
		else if ( pvalue[0] == 2 )
		{ // 预定生产信息
			city_material_will_sendinfo( actor_index );
		}
		else if ( pvalue[0] == 3 )
		{ // 购买队列
			city_material_buyqueue( actor_index );
		}
		else if ( pvalue[0] == 4 )
		{ // 领取
			city_material_get( actor_index );
		}
		else if ( pvalue[0] == 5 )
		{ // 制造
			city_material_make( actor_index, pvalue[1], pvalue[2], 1 );
		}
		break;
	case ASKINFO_STORY:
		if ( pvalue[0] == 0 )
		{
			story_sendinfo( actor_index );
		}
		else if ( pvalue[0] == 1 )
		{
			story_sendrank( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 2 )
		{ // 资源副本攻打
			story_res_get( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 3 )
		{ // 资源副本重置
			story_res_reset( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 4 )
		{ // 招募副本免费招募
			story_hero_free( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 5 )
		{ // 招募副本花钱招募
			story_hero_token( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 6 )
		{ // 图纸副本购买
			story_drawing_buy( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 7 )
		{ // 图纸副本攻打
			story_drawing_get( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 8 )
		{ // 扫荡
			story_sweep( actor_index, pvalue[1], pvalue[2], pvalue[3], pvalue[4], pvalue[5] );
		}
		break;
	case ASKINFO_WORLDMAP:
		if ( pvalue[0] == 1 )
		{ // 流寇奖励
			map_enemy_sendaward( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 2 )
		{ // 请求出征队列信息
			city_battlequeue_sendlist( actor_index, -1 );
		}
		else if ( pvalue[0] == 3 )
		{ // 已经达到目的点的撤回
			actor_army_return( actor_index, pvalue[1], -1 );
		}
		else if ( pvalue[0] == 4 )
		{ // 行军途中的召回
			actor_army_callback( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 5 )
		{ // 行军加速
			actor_army_march_speedup( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 7 )
		{ // 资源点信息
			map_res_sendinfo( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 8 )
		{ // 迁城
			city_move_actor( actor_index, pvalue[1], pvalue[2], pvalue[3] );
		}
		else if ( pvalue[0] == 9 )
		{ // 遣返
			actor_helparmy_repatriate( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 10 )
		{ // 发皇城地区的范围归属
			map_zone_center_townlist( actor_index );
		}
		else if ( pvalue[0] == 11 )
		{ // 地图召唤
			map_call( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 12 )
		{ // 响应召唤
			map_call_response( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 13 )
		{  // 召唤公告
			map_call_notify( actor_index );
		}
		else if ( pvalue[0] == 14 )
		{  // 召唤信息
			map_call_sendinfo( actor_index );
		}
		break;
	case ASKINFO_MAP_EVENT:
		if ( pvalue[0] == 1 )
		{ // 采集事件
			map_event_gather( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 2 )
		{ // 领取事件
			map_event_getaward( actor_index, pvalue[1] );
		}
		break;
	case ASKINFO_SPY_CITY:
		if ( pvalue[0] == 0 )
		{ // 侦察
			city_spy( actor_index, pvalue[1], pvalue[2] );
		}
		break;
	case ASKINFO_CITYHELP:
		if ( pvalue[0] == 0 )
		{// 驻防
			city_helparmy_sendlist( actor_index, pvalue[1] );
		}
		break;
	case ASKINFO_CITYARMYGROUP:
		if ( pvalue[0] == 0 )
		{ // 城战列表
			armygroup_city_sendlist( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 1 )
		{
			armygroup_askhelp( actor_index, pvalue[1], pvalue[2] );
		}
		break;
	case ASKINFO_NATIONARMYGROUP:
		if ( pvalue[0] == 0 )
		{ // 国战列表
			armygroup_town_sendlist( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 1 )
		{ // 创建国战
			armygroup_nation_askcreate( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 2 )
		{ // 所有有战争的城镇
			armygroup_nation_sendlist( actor_index );
		}
		break;
	case ASKINFO_MAPTOWN:
		if ( pvalue[0] == 0 )
		{ // 申请城主
			map_town_ask_owner( actor_index, pvalue[1] );
		}
		else if( pvalue[0] == 1 )
		{ // 申请列表
			map_town_ask_owner_sendlist( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 2 )
		{ // 放弃城主
			map_town_owner_leave( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 3 )
		{ // 补兵
			map_town_soldiers_repair( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 4 )
		{ // 征收
			map_town_levy( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 5 )
		{ // 城镇奖励
			map_town_sendaward( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 6 )
		{ // 城镇详细信息
			map_town_sendinfo( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 7 )
		{ // 城镇详细信息
			map_town_ex_sendinfo( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 8 )
		{ // 都城开发
			map_town_dev( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 9 )
		{ // 都城开发冷却消除
			map_town_dev_delcd( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 10 )
		{ // 修改名称
			map_town_changename( actor_index, pvalue[1], pstr );
		}
		break;
	case ASKINFO_MAPZONE:
		if ( pvalue[0] == 0 )
		{// 玩家列表
			map_zone_citylist( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 1 )
		{// 城镇列表
			map_zone_townlist( actor_index, pvalue[1] );
		}
		break;
	case ASKINFO_LOSTREBUILD:
		if ( pvalue[0] == 0 )
		{
			city_lost_rebuild_num( actor_index );
		}
		else if ( pvalue[0] == 1 )
		{
			city_lost_rebuild_get( actor_index );
		}
		break;
	case ASKINFO_KINGWAR:
		if ( pvalue[0] == -1 )
		{ // 取消订阅
			kingwar_subscribe_cancel( actor_index );
		}
		else if ( pvalue[0] == 0 )
		{ // 获取列表并订阅
			kingwar_town_sendlist( actor_index );
		}
		else if ( pvalue[0] == 1 )
		{ // 前往
			kingwar_army_goto( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 2 )
		{ // 偷袭
			kingwar_army_sneak( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 3 )
		{ // 单挑
			kingwar_army_pk( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 4 )
		{ // 进攻
			kingwar_army_attack( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 5 )
		{ // 回防
			kingwar_army_defense( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 6 )
		{ // 救援
			kingwar_army_rebirth( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 10 )
		{ // 血战排行榜
			kingwar_ranklist( actor_index );
		}
		else if ( pvalue[0] == 11 )
		{ // 获取血战积分
			kingwar_sendpoint( actor_index );
		}
		else if ( pvalue[0] == 12 )
		{ // 皇城密库兑换
			kingwar_changeitem( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 13 )
		{ // 买酒套话
			kingwar_treasure_buypos( actor_index );
		}
		else if ( pvalue[0] == 14 )
		{ // 挖宝
			kingwar_treasure_do( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 15 )
		{ // 挖宝前往
			kingwar_treasure_goto( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 16 )
		{ // 恭贺列表
			kingwar_congratulate_sendlist( actor_index );
		}
		else if ( pvalue[0] == 17 )
		{ // 恭贺
			kingwar_congratulate( actor_index );
		}
		else if ( pvalue[0] == 18 )
		{
			kingwar_treasure_cityinfo( actor_index );
		}
		break;
	case ASKINFO_MSGBOX_CALLBACK:
		actor_notify_msgbox_callback( actor_index, pvalue[0], pvalue[1], pvalue[2] );
		break;
	case ASKINFO_AUTOBUILD:
		if ( pvalue[0] == 0 )
		{
			city_autobuild_open( g_actors[actor_index].city_index );
		}
		else if ( pvalue[0] == 1 )
		{ // 客户端发过来的自动升级
			building_upgrade_auto( g_actors[actor_index].city_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 2 )
		{
			city_autoguard_open( g_actors[actor_index].city_index );
		}
		else if ( pvalue[0] == 3 )
		{
			building_upgrade_autocheck( g_actors[actor_index].city_index );
		}
		break;
	case ASKINFO_HERO_VISIT:
		if ( pvalue[0] == 0 )
		{ // 寻访信息
			hero_visit_sendinfo( actor_index );
		}
		else if ( pvalue[0] == 1 )
		{ // 良将寻访
			if ( pvalue[1] == 0 )
				hero_visit_low( actor_index );
			else
				hero_visit_low10( actor_index );
		}
		else if ( pvalue[0] == 2 )
		{ // 神将寻访
			if ( pvalue[1] == 0 )
				hero_visit_high( actor_index );
			else
				hero_visit_high10( actor_index );
		}
		break;
	case ASKINFO_CHANGESIGN:
		actor_changsign( actor_index, pstr );
		break;
	case ASKINFO_SHOP:
		if ( pvalue[0] == 0 )
		{
			shop_list( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 1 )
		{
			shop_buy( actor_index, pvalue[1], pvalue[2], pvalue[3], pvalue[4], pvalue[5] );
		}
		break;
	case ASKINFO_PAY:
		if ( pvalue[0] == 0 )
		{ // 支付商店列表
			paystore_list( actor_index );
		}
		else if ( pvalue[0] == 1 )
		{ // 礼包列表
			activity_paybag_list( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 2 )
		{ // 购买商品
			paystore_buy( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 3 )
		{ // 测试支付
			if ( g_actors[actor_index].admin >= 90 )
			{
				actor_pay( g_actors[actor_index].actorid, pvalue[1], pstr, "0", "0" );
			}
		}
		break;
	case ASKINFO_VIPSHOP:
		if ( pvalue[0] == 0 )
		{
			vipshop_list( actor_index );
		}
		else if ( pvalue[0] == 1 )
		{
			vipshop_buy( actor_index, pvalue[1], pvalue[2], pvalue[3] );
		}
		else if ( pvalue[0] == 2 )
		{
			vipshop_set_useitembuy( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 3 )
		{
			vipshop_useitem_buy( actor_index, pvalue[1], pvalue[2] );
		}
		break;
	case ASKINFO_VIPBAG:
		if ( pvalue[0] == 0 )
		{
			vipbag_list( actor_index );
		}
		else if ( pvalue[0] == 1 )
		{
			vipbag_buy( actor_index, pvalue[1] );
		}
		break;
	case ASKINFO_ACTIVITY:
		if ( pvalue[0] == 0 )
		{
			activity_sendlist( actor_index );
		}
		else if ( pvalue[0] == ACTIVITY_1 )
		{ // 首充礼包
			if ( pvalue[1] == 0 )
			{
				activity_01_sendinfo( actor_index );
			}
			else if ( pvalue[1] == 1 )
			{
				activity_01_get( actor_index );
			}
		}
		else if ( pvalue[0] == ACTIVITY_2 )
		{ // 主城等级
			if ( pvalue[1] == 0 )
			{
				activity_02_sendinfo( actor_index );
			}
			else if ( pvalue[1] == 1 )
			{
				activity_02_get( actor_index, pvalue[2] );
			}
		}
		else if ( pvalue[0] == ACTIVITY_3 )
		{ // 攻城掠地
			if ( pvalue[1] == 0 )
			{
				activity_03_sendinfo( actor_index );
			}
			else if ( pvalue[1] == 1 )
			{
				activity_03_get( actor_index, pvalue[2] );
			}
		}
		else if ( pvalue[0] == ACTIVITY_4 )
		{ // 七日活动
			if ( pvalue[1] == 0 )
			{
				activity_04_sendinfo( actor_index );
			}
			else if ( pvalue[1] == 1 )
			{
				activity_04_get( actor_index, pvalue[2] );
			}
		}
		else if ( pvalue[0] == ACTIVITY_5 )
		{ // 七星拜将
			if ( pvalue[1] == 0 )
			{
				activity_05_sendinfo( actor_index );
			}
			else if ( pvalue[1] == 1 )
			{
				activity_05_buy( actor_index, pvalue[2] );
			}
			else if ( pvalue[1] == 2 )
			{
				activity_05_update( actor_index );
			}
			else if ( pvalue[1] == 3 )
			{ 
				activity_05_callhero( actor_index );
			}
		}
		else if ( pvalue[0] == ACTIVITY_6 )
		{ // 全服返利
			if ( pvalue[1] == 0 )
			{
				activity_06_sendinfo( actor_index );
			}
			else if ( pvalue[1] == 1 )
			{
				activity_06_get( actor_index, pvalue[2] );
			}
		}
		else if ( pvalue[0] == ACTIVITY_8 )
		{ // 成长计划
			if ( pvalue[1] == 0 )
			{
				activity_08_sendinfo( actor_index );
			}
			else if ( pvalue[1] == 1 )
			{
				activity_08_open( actor_index );
			}
			else if ( pvalue[1] == 2 )
			{
				activity_08_get( actor_index, pvalue[2] );
			}
		}
		else if ( pvalue[0] == ACTIVITY_11 )
		{ // 出师大宴
			if ( pvalue[1] == 0 )
			{
				activity_11_sendinfo( actor_index );
			}
			else if ( pvalue[1] == 1 )
			{
				activity_11_get( actor_index );
			}
		}
		break;
	case ASKINFO_NATIONEQUIP:
		if ( pvalue[0] == 0 )
		{
			nation_equip_sendlist( actor_index );
		}
		else if ( pvalue[0] == 1 )
		{ // 开启
			nation_equip_open( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 2 )
		{ // 升级
			nation_equip_upgrade( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 3 )
		{ // 改造
			nation_equip_remake( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 4 )
		{ // 加速
			nation_equip_quick( actor_index, pvalue[1], pvalue[2] );
		}
		break;
	case ASKINFO_NATION:
		if ( pvalue[0] == 0 )
		{
			nation_sendinfo( actor_index );
		}
		else if ( pvalue[0] == 1 )
		{ // 国家建设
			nation_build( actor_index );
		}
		else if ( pvalue[0] == 2 )
		{ // 爵位晋升
			nation_place_upgrade( actor_index );
		}
		else if ( pvalue[0] == 3 )
		{ // 国家城池
			nation_town_sendlist( actor_index );
		}
		else if ( pvalue[0] == 4 )
		{ // 重建信息
			nation_town_sendinfo( actor_index, pvalue[1], 1 );
		}
		else if ( pvalue[0] == 5 )
		{ // 修复
			nation_town_repair( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 6 )
		{ // 国家战争列表
			nation_town_warlist( actor_index );
		}
		else if ( pvalue[0] == 7 )
		{ // 城池战争列表
			nation_city_warlist( actor_index );
		}
		else if ( pvalue[0] == 8 )
		{ // 国家任务
			nation_quest_sendlist( actor_index );
		}
		else if ( pvalue[0] == 9 )
		{ // 国家任务领取
			nation_quest_getaward( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 10 )
		{ // 国家荣誉
			nation_mission_sendlist( actor_index );
			nation_rank_sendlist( actor_index );
		}
		else if ( pvalue[0] == 11 )
		{ // 国家荣誉任务奖励领取
			nation_mission_getaward( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 12 )
		{ // 国家官员列表
			nation_official_sendlist( actor_index );
		}
		else if ( pvalue[0] == 13 )
		{ // 国家官员投票
			nation_official_ballot( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 14 )
		{ // 国家官员替换列表
			nation_official_replace_sendlist( actor_index );
		}
		else if ( pvalue[0] == 15 )
		{ // 国家官员任免
			nation_official_replace_up( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 16 )
		{ // 国家官员罢免
			nation_official_replace_down( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 17 )
		{ // 修改国家公告
			nation_notice( actor_index, pstr );
		}
		break;
	case ASKINFO_NATIONLOG:
		nationlog_sendlist( actor_index, pvalue[0] );
		break;
	case ASKINFO_NATIONHERO:
		if ( pvalue[0] == 0 )
		{ // 国家名将列表
			nation_hero_sendlist( actor_index );
		}
		else if ( pvalue[0] == 1 )
		{ // 购买名将坐标
			nation_hero_buypos( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 2 )
		{ // 奖励
			nation_hero_sendaward( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 3 )
		{ // 招募
			nation_hero_call( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 4 )
		{ // 补充忠诚度
			nation_hero_addloyal( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 5 )
		{ // 查看信息
			nation_hero_view( actor_index, pvalue[1] );
		}
		break;
	case ASKINFO_WISHING:
		if ( pvalue[0] == 0 )
		{ // 聚宝盆信息
			wishing_shop_sendinfo( actor_index );
		}
		else if ( pvalue[0] == 1 )
		{ // 聚宝盆打开
			wishing_shop_open( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 2 )
		{ // 聚宝盆购买
			wishing_shop_buy( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 3 )
		{ // 聚宝盆兑换
			wishing_change_sendinfo( actor_index );
		}
		else if ( pvalue[0] == 4 )
		{ // 聚宝盆兑换
			wishing_change_buy( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 5 )
		{ // 聚宝盆打包
			wishing_pack_sendinfo( actor_index );
		}
		else if ( pvalue[0] == 6 )
		{ // 聚宝盆打包
			wishing_pack_buy( actor_index, pvalue[1] );
		}
		break;
	case ASKINFO_TIANCEQUEST:
		if ( pvalue[0] == 0 )
		{
			tiance_quest_sendinfo( actor_index );
		}
		else if ( pvalue[0] == 1 )
		{
			tiance_quest_get( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 2 )
		{
			tiance_quest_complete( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 3 )
		{
			tiance_quest_tech_activate( actor_index, pvalue[1] );
		}
		break;
	case ASKINFO_TEACHER:
		if ( pvalue[0] == 0 )
		{ // 师徒列表
			actor_student_sendlist( actor_index );
		}
		else if ( pvalue[0] == 1 )
		{ // 拜师
			actor_take_teacher( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 2 )
		{ // 拜师奖励领取
			actor_take_teacher_awardget( actor_index );
		}
		else if ( pvalue[0] == 3 )
		{ // 徒弟等级奖励领取
			actor_studentlevel_awardget( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 4 )
		{ // 师徒商店列表
			teacher_shoplist( actor_index );
		}
		else if ( pvalue[0] == 5 )
		{ // 师徒商店购买
			teacher_shopbuy( actor_index, pvalue[1] );
		}
		break;
	case ASKINFO_CDKEY:
		// 兑换码
		user_award( actor_index, pstr );
		break;
	case ASKINFO_GIRL:
		if ( pvalue[0] == 1 )
		{ // 委派
			girl_allot( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 2 )
		{ // 解除委派
			girl_unallot( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 3 )
		{ // 突破
			girl_colorup( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 4 )
		{ // 亲密度道具使用
			girl_loveitemuse( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 5 )
		{ // 喜结连理
			girl_marry( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 6 )
		{ // 亲密互动
			girl_makelove( actor_index, pvalue[1] );
		}
		break;
	default:
		break;
	}
	return 0;
}
