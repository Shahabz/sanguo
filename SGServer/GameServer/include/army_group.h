#ifndef _ARMY_GROUP_H_
#define _ARMY_GROUP_H_
#include "define.h"
#include "fight.h"

#define ARMYGROUP_MAXCOUNT			128	// 最多有128人集结

#define ARMYGROUP_STATE_IDLE		0	// 空闲
#define ARMYGROUP_STATE_ING			1	// 集结中


int armygroup_load();
int armygroup_save( FILE *fp );
ArmyGroup *armygroup_getptr( int group_index );

int armygroup_create( char type, char from_type, int from_id, char to_type, int to_id, int stateduration );
void armygroup_delete( int group_index );
void armygroup_alllogic();
void armygroup_logic( int group_index );

int armygroup_addarmy( int army_index );
void armygroup_delarmy( int army_index );
void armygroup_setleader( int group_index, int army_index );
int armygroup_dismiss( int army_index );
int armygroup_statetime( int group_index );
int armygroup_stateduration( int group_index );

// 攻击双方兵力
int armygroup_from_totals( int group_index );
int armygroup_to_totals( int group_index );

// 城战结果
int armygroup_vs_city( int group_index, Fight *pFight );
// 国战战结果
int armygroup_vs_town( int group_index, Fight *pFight );

// 城战列表
int armygroup_city_sendlist( int actor_index, int unit_index );

// 国战列表
int armygroup_town_sendlist( int actor_index, int unit_index );

// 国战创建-玩家创建
int armygroup_nation_askcreate( int actor_index, int townid );

// 国战创建-AI城镇创建
int armygroup_nation_askcreate_ai( int from_townid, int to_townid );

// 所有国战信息
int armygroup_nation_sendlist( int actor_index );

// 请求帮助
int armygroup_askhelp( int actor_index, int group_index, int group_id );

// 集结所有人发送邮件
int armygroup_mail( int group_index, char attack, City *defenseCity, char type, char *title, char *content, char *attach, Fight *fight );
#endif
