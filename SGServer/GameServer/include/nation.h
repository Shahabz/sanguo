#ifndef _NATION_H_
#define _NATION_H_
#include "define.h"

#define NATION_MAX			4		// 算群雄4个国家
#define NATION_NOTICE_MAX	512		// 公告
#define NATION_OFFICIAL_MAX	16		// 官员个数
#define NATION_TOWN_WAR_MAX	256		// 国战数量
#define NATION_CITY_WAR_MAX	1024	// 城战数量

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


#endif
