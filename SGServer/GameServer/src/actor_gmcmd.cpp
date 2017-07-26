#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "gmcmd.h"
#include "system.h"
#include "script.h"
#include "map.h"
#include "item.h"
#include "equip.h"
#include "award.h"
#include "global_netprocess.h"
#include "timegmcmd.h"
#include "global.h"
#include "login.h"
#include "actor_notify.h"
#include "gameproc.h"
#include "city.h"
#include "chat.h"
#include "vip.h"
#include "hero.h"

extern Global global;
extern MYSQL *myGame;
extern Actor *g_actors;
extern int g_actornum;
extern int g_maxactornum;
extern char g_bServerIsInit;

int actor_command( int actor_index, short cmd, int *pValue, char *pMsg )
{
	if ( g_bServerIsInit == 0 )
		return -1;
	
	int tmpi;
	char szMsg[1024] = { 0 };
	int actorid = pValue[3];
	City *pCity = NULL;

	// 有些指令必须玩家在线
	if ( actorid > 0 && (cmd == GMC_SYSTALK) )
	{
		int target_index = actor_getindex_withid( actorid );
		if ( target_index < 0 )
		{
			return GMR_NOT_ONLINE;
		}
	}

	if ( actor_index >= 0 )
	{ // 客户端使用GM指令情况
#ifndef	WIN32
		if ( g_actors[actor_index].admin < 100 )
		{
			system_talkto( actor_index, "Access denied!" );
			return GMR_ACCESS_DENIED;
		}
#endif // DEBUG
		if ( pMsg )
			write_gamelog( "<GmProcess>cmd:%d value:%d,%d,%d,%d,%s [%s(%d)]", cmd, pValue[0], pValue[1], pValue[2], pValue[3], pMsg, g_actors[actor_index].name, g_actors[actor_index].actorid );
		else
			write_gamelog( "<GmProcess>cmd:%d value:%d,%d,%d,%d,NULL [%s(%d)]", cmd, pValue[0], pValue[1], pValue[2], pValue[3], g_actors[actor_index].name, g_actors[actor_index].actorid );

		if ( actorid > 0 )
			pCity = city_getptr_withactorid( actorid ); // 给别人使用
		else
			pCity = city_getptr( actor_index ); // 自己使用

		if ( !pCity )
			return GMR_ERROR;
		
	}
	else
	{ // GM工具使用情况
		if ( pMsg )
			write_gamelog( "<GmTools>cmd:%d value:%d,%d,%d,%d,%s", cmd, pValue[0], pValue[1], pValue[2], pValue[3], pMsg );
		else
			write_gamelog( "<GmTools>cmd:%d value:%d,%d,%d,%d,NULL", cmd, pValue[0], pValue[1], pValue[2], pValue[3] );

		if ( actorid > 0 )
		{
			pCity = city_getptr_withactorid( actorid );
			if ( !pCity )
				return GMR_ERROR;
		}
	}

	switch ( cmd )
	{
	case GMC_TEST:
		break;
	case GMC_SC:
		sc_Script_Command( pValue[0], pValue[1], pValue[2], pValue[3], pMsg, actor_index );
		break;
	case GMC_LUA:// 重载lua
		tmpi = lua_reload( -1 );
		sprintf( szMsg, "ThisTimer:%d", tmpi );
		if ( actor_index >= 0 )
			system_talkto( actor_index, szMsg );
		break;
	case GMC_RELOAD:// 重读数据表
		break;
	case GMC_RESET:// 重置等级数据
		break;
	case GMC_EXP:// 经验
		if ( pCity )
		{
			city_actorexp( pCity->index, pValue[0], PATH_GM );
		}
		break;
	case GMC_TOKEN:// 钻石
		if ( pCity )
		{
			if ( pCity->actor_index >= 0 )
				actor_change_token( actor_index, pValue[0], PATH_GM, 0 );
			else
				gift( pCity->actorid, AWARDKIND_TOKEN, pValue[0], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, PATH_GM );
		}
		break;
	case GMC_ITEM:// 道具
		if ( pValue[0] == -1 && actor_index >= 0 )
		{
			int item_clear( int actorindex );
			item_clear( actor_index );
			break;
		}
		else if ( pValue[0] == 0 && actor_index >= 0 )
		{
			void item_searchname( int actor_index, char *pName, int totle );
			item_searchname( actor_index, pMsg, pValue[1] > 0 ? pValue[1] : 10 );
			break;
		}
		if ( pValue[1] == 0 )
			tmpi = 1;
		else
			tmpi = pValue[1];
		if ( tmpi < 0 && pCity )
		{
			if ( item_lost( pCity->actor_index, pValue[0], -tmpi, PATH_GM ) >= 0 )
			{
				if ( actor_index >= 0 )
					system_talkto( actor_index, "Actor lost item." );
			}
		}
		else if ( pCity )
		{
			if ( item_getitem( pCity->actor_index, pValue[0], tmpi, pValue[2], PATH_GM ) >= 0 )
			{
				if ( actor_index >= 0 )
					system_talkto( actor_index, "Actor get item." );
			}
			else
			{
				if ( actor_index >= 0 )
					system_talkto( actor_index, "Actor not get item." );
			}
		}
		break;
	case GMC_EQUIP:// 装备
		if ( pCity && pValue[0] == -1 && actor_index >= 0 )
		{
			int equip_clear( int actorindex );
			equip_clear( pCity->actor_index );
			break;
		}

		if ( pCity )
		{
			if ( equip_getequip( pCity->actor_index, pValue[0], PATH_GM ) >= 0 )
			{
				if ( actor_index >= 0 )
					system_talkto( actor_index, "Actor get equip." );
			}
			else
			{
				if ( actor_index >= 0 )
					system_talkto( actor_index, "Actor not get equip." );
			}
		}
		break;
	case GMC_HERO:// 英雄
		if ( pCity )
		{
			hero_gethero( pCity->actor_index, pValue[0], PATH_GM );
		}
		break;
	case GMC_SILVER:// 银币
		if ( pCity )
		{
			city_changesilver( pCity->index, pValue[0], PATH_GM );
		}
		break;
	case GMC_WOOD:// 木材
		if ( pCity )
		{
			city_changewood( pCity->index, pValue[0], PATH_GM );
		}
		break;
	case GMC_FOOD:// 粮食
		if ( pCity )
		{
			city_changefood( pCity->index, pValue[0], PATH_GM );
		}
		break;
	case GMC_IRON:// 镔铁
		if ( pCity )
		{
			city_changeiron( pCity->index, pValue[0], PATH_GM );
		}
		break;
	case GMC_BODY:// 体力
		if ( pCity )
		{
			city_changebody( pCity->index, pValue[0], PATH_GM );
		}
		break;
	case GMC_LEVY:// 收割次数
		if ( pCity )
		{
			city_changelevy( pCity->index, pValue[0], PATH_GM );
		}
		break;
	case GMC_AWARD:// 奖励
		if ( pCity )
		{
			awardgroup_withid( pCity->actorid, pValue[0], PATH_GM, NULL );
		}
		break;
	case GMC_QUEST:// 任务
		break;
	case GMC_ROLLMSG:// 跑马灯
		break;
	case GMC_MAIL:// 邮件
		break;
	case GMC_SYSTALK:// 系统喊话
		system_talk( pMsg );
		break;
	case GMC_VIPEXP:// vip经验
		if ( pCity )
		{
			vip_exp( pCity->index, pValue[0], PATH_GM );
		}
		break;
	case GMC_ACTIVITY:// 活动
		break;
	default:
		break;
	}
	return GMR_OK;
}
