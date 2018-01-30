#ifndef _ARMY_FIGHT_H_
#define _ARMY_FIGHT_H_

// 与流寇战斗结果
int army_vs_enemy( int army_index, Fight *pFight );

// 与采集资源的部队战斗结果
int army_vs_res( int army_index, Fight *pFight );

// 与国家名将战斗结果
int army_vs_nationhero( int army_index, Fight *pFight );
#endif

