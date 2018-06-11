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
#include "actor.h"
#include "actor_send.h"
#include "actor_times.h"
#include "server_netsend_auto.h"
#include "fight.h"
#include "actor_notify.h"
#include "award.h"
#include "global.h"
#include "activity.h"
#include "system.h"
#include "mail.h"
#include "city.h"
#include "army_group.h"
#include "chat.h"
#include "world_quest.h"
#include "world_boss.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;
extern Global global;

extern Actor *g_actors;
extern int g_maxactornum;

extern City * g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern Army *g_army;
extern int g_army_maxcount;

extern MonsterInfo *g_monster;
extern int g_monster_maxnum;

extern ArmyGroup *g_armygroup;
extern int g_armygroup_maxcount;

extern WorldBossInfo *g_worldbossinfo;
extern int g_worldbossinfo_maxnum;
extern Fight g_fight;

WorldBoss *g_world_boss = NULL;
int g_world_boss_maxcount = 0;


// 读档完毕的回调
int world_boss_loadcb( int bossid )
{
	if ( bossid <= 0 || bossid >= g_world_boss_maxcount )
		return -1;

	if ( g_world_boss[bossid].maxhp <= 0 )
	{
		g_world_boss[bossid].maxhp = g_worldbossinfo[bossid].hp;
	}
	return 0;
}

int world_boss_load()
{
	g_world_boss_maxcount = g_worldbossinfo_maxnum;
	g_world_boss = (WorldBoss*)malloc( sizeof( WorldBoss ) * g_world_boss_maxcount );
	memset( g_world_boss, 0, sizeof( WorldBoss ) * g_world_boss_maxcount );
	printf_msg( "WorldBoss  maxcount=%d  memory=%0.2fMB(memory=%0.2fKB)\n", g_world_boss_maxcount, (sizeof( WorldBoss )*g_world_boss_maxcount) / 1024.0 / 1024.0, sizeof( WorldBoss ) / 1024.0 );
	for ( int bossid = 1; bossid < g_world_boss_maxcount; bossid++ )
	{
		if ( g_worldbossinfo[bossid].bossid <= 0 )
			continue;
		g_world_boss[bossid].bossid = bossid;

		// 部队列表
		char **pptable = NULL;
		int groupcount = 0;
		pptable = u_strcut_ex( g_worldbossinfo[bossid].monster, ',', &groupcount );
		for ( int tmpi = 0; tmpi < groupcount; tmpi++ )
		{
			if ( groupcount >= WORLDBOSS_MONSTER_MAX )
				break;
			g_world_boss[bossid].monster[tmpi] = atoi( pptable[tmpi] );
		}
		u_free_vec( pptable );

		g_world_boss[bossid].hp = g_worldbossinfo[bossid].hp;
		g_world_boss[bossid].maxhp += g_worldbossinfo[bossid].hp;
	}
	world_boss_load_auto( world_boss_getptr, world_boss_loadcb, "world_boss" );
	return 0;
}

int world_boss_save( FILE *fp )
{
	world_boss_batch_save_auto( g_world_boss, g_world_boss_maxcount, "world_boss", fp );
	return 0;
}

WorldBoss *world_boss_getptr( int bossid )
{
	if ( bossid <= 0 || bossid >= g_world_boss_maxcount )
		return NULL;
	return &g_world_boss[bossid];
}

// 发送世界boss信息
int worldboss_sendinfo( int actor_index, int bossid )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( bossid <= 0 || bossid >= g_world_boss_maxcount )
		return -1;
	SLK_NetS_WorldBoss pValue = { 0 };
	pValue.m_bossid = bossid;
	pValue.m_hp = g_world_boss[bossid].hp;
	pValue.m_maxhp = g_world_boss[bossid].maxhp;
	pValue.m_actorlevel = g_worldbossinfo[bossid].actorlevel;
	pValue.m_isfight = actor_get_today_char_times( actor_index, TODAY_CHAR_WORLDBOSS );
	pValue.m_resetnum = actor_get_today_char_times( actor_index, TODAY_CHAR_WORLDBOSS_RESET );
	netsend_worldboss_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 给全服玩家发
int worldboss_sendworld( int bossid )
{
	if ( bossid <= 0 || bossid >= g_world_boss_maxcount )
		return -1;
	SLK_NetS_WorldBoss pValue = { 0 };
	pValue.m_bossid = bossid;
	pValue.m_hp = g_world_boss[bossid].hp;
	pValue.m_maxhp = g_world_boss[bossid].maxhp;
	pValue.m_actorlevel = g_worldbossinfo[bossid].actorlevel;
	pValue.m_isfight = -1;
	pValue.m_resetnum = -1;
	netsend_worldboss_S( 0, SENDTYPE_WORLDMAP, &pValue );
	return 0;
}

int worldboss_sendbattleinfo( int actor_index, int bossid )
{
	if ( bossid <= 0 || bossid >= g_world_boss_maxcount )
		return -1;
	SLK_NetS_StoryRank pValue = { 0 };
	pValue.m_type = 1;
	for ( int tmpi = 0; tmpi < WORLDBOSS_MONSTER_MAX; tmpi++ )
	{
		int monsterid = g_world_boss[bossid].monster[tmpi];
		if ( monsterid <= 0 || monsterid >= g_monster_maxnum )
			break;
		pValue.m_list[pValue.m_count].m_monsterid = monsterid;
		pValue.m_list[pValue.m_count].m_shape = g_monster[monsterid].shape;
		pValue.m_list[pValue.m_count].m_level = g_monster[monsterid].level;
		pValue.m_list[pValue.m_count].m_color = (char)g_monster[monsterid].color;
		pValue.m_list[pValue.m_count].m_corps = (char)g_monster[monsterid].corps;
		pValue.m_list[pValue.m_count].m_hp = g_monster[monsterid].troops;
		pValue.m_count++;
	}
	netsend_storyrank_S( actor_index, SENDTYPE_ACTOR, &pValue );
	return 0;
}

// 攻打世界boss
int worldboss_battle( int actor_index, SLK_NetC_WorldBossBattle *pValue )
{
	ACTOR_CHECK_INDEX( actor_index );
	City *pCity = city_getptr( actor_index );
	if ( !pCity )
		return -1;
	if ( !pValue )
		return -1;
	if ( actor_get_today_char_times( actor_index, TODAY_CHAR_WORLDBOSS ) > 0 )
		return -1;
	int bossid = pValue->m_bossid;
	if ( bossid <= 0 || bossid >= g_world_boss_maxcount )
		return -1;
	if ( pCity->level < g_worldbossinfo[bossid].actorlevel )
	{
		return -1;
	}

	// 开始战斗
	if ( fight_start_byworldboss( actor_index, pValue ) < 0 )
		return -1;

	// 伤害
	char v1[32] = { 0 };
	sprintf( v1, "%d", g_fight.attack_total_damage );

	// 给予经验奖励
	int exp = (int)ceil( (g_fight.attack_total_damage / (float)g_world_boss[bossid].maxhp) * g_worldbossinfo[bossid].exp );
	city_actorexp( pCity->index, exp, PATH_WORLDBOSS );

	//// 物品奖励
	//if ( g_worldbossinfo[bossid].awardgroup > 0 )
	//{
	//	// 最后一击
	//	char attach[256] = { 0 };
	//	awardgroup_mail( g_worldbossinfo[bossid].awardgroup, 0, attach );
	//	mail_system( actor_index, pCity->actorid, 5032, 5528, v1, NULL, NULL,0 );
	//}

	g_world_boss[bossid].hp -= g_fight.attack_total_damage;
	if ( g_world_boss[bossid].hp <= 0 )
	{ // 击杀
		g_world_boss[bossid].hp = 0;

		// 最后一击
		mail_system( actor_index, pCity->actorid, 5033, 5529, g_worldbossinfo[bossid].kill_awardgroup, 0 );

		if ( bossid == 1 )
		{
			// 世界任务完成
			worldquest_setvalue( WORLDQUEST_WORLDBOSS1, 1 );
			system_talkjson_world( 6006, NULL, NULL, NULL, NULL, NULL, NULL, 1 );
		}
		else if ( bossid == 2 )
		{
			// 世界任务完成
			worldquest_setvalue( WORLDQUEST_WORLDBOSS2, 1 );
			system_talkjson_world( 6007, NULL, NULL, NULL, NULL, NULL, NULL, 1 );
			// 开启皇城争夺战

		}
	}
	actor_notify_alert_v( actor_index, 1364, v1, NULL );
	actor_add_today_char_times( actor_index, TODAY_CHAR_WORLDBOSS );
	worldboss_sendworld( bossid );
	return 0;
}

// 重置
int worldboss_reset( int actor_index, int bossid )
{
	if ( bossid <= 0 || bossid >= g_world_boss_maxcount )
		return -1;
	if ( actor_get_today_char_times( actor_index, TODAY_CHAR_WORLDBOSS ) == 0 )
		return -1;

	int num = actor_get_today_char_times( actor_index, TODAY_CHAR_WORLDBOSS_RESET );
	if ( num > 100 )
	{
		num = 100;
	}
	int token = 100 + num * 100;
	if ( actor_change_token( actor_index, -token, PATH_WORLDBOSS, 0 ) < 0 )
	{
		return -1;
	}
	
	actor_set_today_char_times( actor_index, TODAY_CHAR_WORLDBOSS, 0 );
	if ( num < 100 )
	{
		actor_add_today_char_times( actor_index, TODAY_CHAR_WORLDBOSS_RESET );
	}
	worldboss_sendinfo( actor_index, bossid );
	return 0;
}

int worldboss_gm( int bossid, int value )
{
	if ( bossid <= 0 || bossid >= g_world_boss_maxcount )
		return -1;
	g_world_boss[bossid].hp += value;
	if ( g_world_boss[bossid].hp <= 0 )
	{ 
		g_world_boss[bossid].hp = 1;
	}
	else if ( g_world_boss[bossid].hp > g_world_boss[bossid].maxhp )
	{
		g_world_boss[bossid].hp = g_world_boss[bossid].maxhp;
	}
	worldboss_sendworld( bossid );
	return 0;
}
