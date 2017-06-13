#ifndef _CITY_H_
#define _CITY_H_
#include "define.h"

// 城池领主类型
typedef enum
{
	CityLairdType_Player = 0,		//	玩家的城池
	CityLairdType_Robot = 2,		//  机器人城池
}CityLairdType;

// 服务器启动读取所有城池信息到内存
int city_load();

// 服务器关闭，所有城池信息存到数据库
int city_save( FILE *fp );
int city_single_save( City *pCity, FILE *fp );

// 获取这个城池属性
City *city_indexptr( int city_index );

// 获取这个玩家的城池属性
City *city_getptr( int actor_index );

// 获取这个城池属性
City *city_getptr_withcityid( int cityid );

// 获取这个城池属性
City *city_getptr_withactorid( int actorid );

// 根据玩家id找到城池索引
int city_getindex_withactorid( int actorid );

// 根据城池id找到城池索引
int city_getindex_withcityid( int cityid );

// 创建一个新城池
int city_new( City *pCity );
#endif
