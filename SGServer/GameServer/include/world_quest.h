#ifndef _WORLD_QUEST_H_
#define _WORLD_QUEST_H_
#include "define.h"

#define WORLDQUEST_ID1			1	//	击杀流寇5个
#define WORLDQUEST_ID2			2	//	击杀流寇10个
#define WORLDQUEST_ID3			3	//	击杀流寇20个
#define WORLDQUEST_ID4			4	//	攻克县城
#define WORLDQUEST_ID5			5	//	攻克郡城
#define WORLDQUEST_ID6			6	//	攻克治所
#define WORLDQUEST_WORLDBOSS1	8	//	世界boss虎牢关-之后可以前往司隶
#define WORLDQUEST_WORLDBOSS2	9	//	世界boss董卓-之后开启血战皇城活动
#define WORLDQUEST_LUOYANG		10	//	皇城血战开启

int worldquest_init();
int worldquest_reload();
int worldquest_give( int actor_index );
int worldquest_addvalue( int questid, int value );
int worldquest_setvalue( int questid, int value );
int worldquest_setcomplete( int actor_index, int questid );
int worldquest_getcomplete( int actor_index, int questid, int *value );
int worldquest_check( int actor_index, int questid, int *value );
int worldquest_check_server( int questid );
int worldquest_checkcomplete( int actor_index, char type );
int worldquest_sendinfo( int actor_index );
int worldquest_sendaward( int actor_index, int questid );
int worldquest_getaward( int actor_index, int questid );
int worldquest_updateopen();
#endif
