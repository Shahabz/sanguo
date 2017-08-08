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

		}
		else if ( pvalue[0] == 3 )
		{ // 替换

		}
		else if ( pvalue[0] == 4 )
		{ // 购买装备栏
			equip_buyext( actor_index );
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
			item_use_quick( actor_index, pvalue[1], pvalue[2], pvalue[3], pvalue[3] );
		}
		break;
	case ASKINFO_ACTOR:
		if ( pvalue[0] == 0 )
		{ // 购买体力
			actor_buybody( actor_index, pvalue[1] );
		}
		else if( pvalue[0] == 1 )
		{ 

		}
		break;
	default:
		break;
	}
	return 0;
}
