#ifndef _WORLD_QUEST_H_
#define _WORLD_QUEST_H_
#include "define.h"

#define WORLDQUEST_ID1	1	//	击杀流寇5个
#define WORLDQUEST_ID2	2	//	击杀流寇10个
#define WORLDQUEST_ID3	3	//	击杀流寇30个
#define WORLDQUEST_ID4	4	//	攻克营地
#define WORLDQUEST_ID5	5	//	攻克郡县
#define WORLDQUEST_ID6	6	//	攻克郡城
#define WORLDQUEST_WORLDBOSS1	7	//	世界boss 张角
#define WORLDQUEST_ID8	8	//	攻克州郡
#define WORLDQUEST_ID9	9	//	攻克州城
#define WORLDQUEST_WORLDBOSS2	10	//	世界boss 董卓
#define WORLDQUEST_ID11	11	//	皇城血战开启

int worldquest_init();
int worldquest_reload();
int worldquest_give( int actor_index );
int worldquest_addvalue( int questid, int value );
int worldquest_setvalue( int questid, int value );
int worldquest_setcomplete( int actor_index, int questid );
int worldquest_getcomplete( int actor_index, int questid, int *value );
int worldquest_check( int actor_index, int questid, int *value );
int worldquest_checkcomplete( int actor_index, char type );
int worldquest_sendinfo( int actor_index );
int worldquest_sendaward( int actor_index, int questid );
int worldquest_getaward( int actor_index, int questid );
#endif
