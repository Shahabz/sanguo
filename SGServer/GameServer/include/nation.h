#ifndef _NATION_H_
#define _NATION_H_
#include "define.h"

#define NATION_MAX			4		// 算群雄4个国家
#define NATION_NOTICE_MAX	512		// 公告
#define NATION_OFFICIAL_MAX	10		// 官员个数
#define NATION_TOWN_WAR_MAX	256		// 国战数量
#define NATION_CITY_WAR_MAX	1024	// 城战数量

#define NATION_QUEST_MAX		3
#define NATION_QUESTKIND_TOWN	1	// 国家任务-国战
#define NATION_QUESTKIND_CITY	2	// 国家任务-城战
#define NATION_QUESTKIND_ENEMY	3	// 国家任务-流寇

#define NATION_MISSION_MAX			3
#define NATION_MISSIONKIND_BUILD	1	// 国家荣誉任务-建设
#define NATION_MISSIONKIND_CITY		2	// 国家荣誉任务-城战
#define NATION_MISSIONKIND_TOWN		3	// 国家荣誉任务-国战

#define NATION_RANK_MAX			3
#define NATION_RANK_BUILD		1	// 国家荣誉任务-建设
#define NATION_RANK_CITY		2	// 国家荣誉任务-城战
#define NATION_RANK_TOWN		3	// 国家荣誉任务-国战

#define NATION_RANK_MEMBERNUM	5	// 国家荣誉任务-选择人数

#define NATION_OFFICIAL_STATE_OPEN		0 // 首次开启
#define NATION_OFFICIAL_STATE_SELECT	1 // 选举期
#define NATION_OFFICIAL_STATE_TERM		2 // 任期

#define NATION_CANDIDATE_MAX	10 // 候选人数
#define NATION_OFFICIAL_R1	1 // 魏王
#define NATION_OFFICIAL_R2	2 // 蜀王
#define NATION_OFFICIAL_R3	3 // 吴王
#define NATION_OFFICIAL_R4	4 // 丞相
#define NATION_OFFICIAL_R5	5 // 军事
#define NATION_OFFICIAL_R6	6 // 将军

// 国家荣誉排行榜
typedef struct _nation_rank_member
{
	int actorid;
	int city_index;
}NationRankMember;

// 国家荣誉排行榜
typedef struct _nation_rank
{
	NationRankMember member[NATION_RANK_MAX][NATION_RANK_MEMBERNUM];
}NationRank;

int nation_load();
int nation_save( FILE *fp );
Nation *nation_getptr( int nation );

// 设置国家都城
int nation_capital_townid( int nation, int townid );
int nation_capital_townid_get( int nation );

// 国家人口
int nation_people_calc( int nation );
int nation_people_get( int nation );
int nation_people_famous_calc( int nation );
int nation_people_capital_calc( int nation );
int nation_people_capital_set( int nation, int people );

// 该国家指定类型城池数量
int nation_town_num( int nation, int towntype );

// 城战攻击信息
int nation_city_war_add( int nation, int group_index );
int nation_city_war_del( int nation, int group_index );

// 国战攻击信息
int nation_town_war_add( int nation, int group_index );
int nation_town_war_del( int nation, int group_index );

// 国王名字
void nation_kingname( char nation, char *out );

// 国家经验
int nation_exp( char nation, int exp );

// 国家信息
int nation_sendinfo( int actor_index );
// 国家信息
int nation_sendbase( int actor_index );

// 国家建设升级
int nation_build( int actor_index );

// 国家爵位晋升
int nation_place_upgrade( int actor_index );

// 国家城池
int nation_town_sendinfo( int actor_index, int townid, char path );

// 国家城池列表
int nation_town_sendlist( int actor_index );

// 修复
int nation_town_repair( int actor_index, int townid );

// 国家战争列表
int nation_town_warlist( int actor_index );
int nation_city_warlist( int actor_index );

// 国家任务
int nation_quest_sendlist( int actor_index );
int nation_quest_update();
int nation_quest_addvalue( City *pCity, char kind, int value );
int nation_quest_getaward( int actor_index, int kind );

// 国家荣誉任务
int nation_mission_sendlist( int actor_index );
int nation_mission_addvalue( char nation, char kind, int value );
int nation_mission_update();
int nation_mission_getaward( int actor_index, int baglevel );

// 国家荣誉排行榜
int nation_rank_sendlist( int actor_index );
void nation_rank_addvalue( City *pCity, char kind, int value );
int nation_rank_calc();
void nation_rank_update();

// 官员功能
int nation_official_open();
void nation_official_logic();
int nation_official_candidate_create();
int nation_official_create();
int nation_official_ballot( int actor_index, int target_actorid, int istoken );
int nation_official_sendlist( int actor_index );
int nation_official_replace_sendlist( int actor_index );
int nation_official_replace_up( int actor_index, int target_actorid );
int nation_official_replace_down( int actor_index, int target_actorid );
#endif
