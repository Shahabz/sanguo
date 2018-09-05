#ifndef _ARMY_FIGHT_H_
#define _ARMY_FIGHT_H_

// 与流寇战斗结果
int army_vs_enemy( int army_index, Fight *pFight );

// 与采集资源的部队战斗结果
int army_vs_res( int army_index, Fight *pFight );

// 与活动怪的部队战斗结果
int army_vs_activity( int army_index, Fight *pFight );

// 与国家名将战斗结果
int army_vs_nationhero( int army_index, Fight *pFight );

// 活动怪和玩家城池
int army_activity_vs_city( int army_index, City *pTargetCity, Fight *pFight );
#endif

