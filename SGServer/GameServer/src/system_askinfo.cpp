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
#include "building.h"
#include "hero.h"
#include "story.h"
#include "map_zone.h"
#include "map_town.h"
#include "map_enemy.h"
#include "map_res.h"
#include "map_event.h"
#include "army_march.h"
#include "army_group.h"

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
		if ( pvalue[0] == 1 )
		{ // 获取任务奖励
			quest_getaward( actor_index, pvalue[1], 0 );
		}
		else if ( pvalue[0] == 2 )
		{ // 完成特殊类型的任务
			quest_setcomplete( actor_index, pvalue[1], QUEST_COMPLETEFLAG_SUCCESS );
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
			hero_addsoldiers( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 2 )
		{// 使用经验道具
			hero_useexpitem( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 3 )
		{// 上阵
			hero_up( actor_index, pvalue[1], pvalue[2], pvalue[3] );
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
	default:
		break;
	}
	return 0;
}
