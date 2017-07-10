#ifndef _CITY_H_
#define _CITY_H_
#include "define.h"

#define CITY_QUEST_MAX				6	// 任务数量
#define CITY_TECH_MAX				40	// 城池科技
#define CITY_DATA_RECORD_MAX		16	// 数据记录

// 功能入口
#define	CITY_FUNCTION_SMITHY		0	// 铁匠铺
#define	CITY_FUNCTION_WASH			1	// 洗炼铺
#define	CITY_FUNCTION_FANGSHI		2	// 坊市
#define	CITY_FUNCTION_SHOP			3	// 商店
#define	CITY_FUNCTION_HERO			4	// 聚贤馆
#define	CITY_FUNCTION_WISHING		5	// 聚宝盆

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
City *city_getptr_withactorid( int actorid );

// 根据玩家id找到城池索引
int city_getindex_withactorid( int actorid );

// 创建一个新城池
int city_new( City *pCity );

// 所有城市每秒的逻辑
void city_logic_sec();

// 城市主城等级
int city_mainlevel( int city_index );

// 32位整数，标志位操作0或1 标示开没开，有没有，做没做（永久存档）
void city_set_sflag( City *pCity, int offset, char value );
int city_get_sflag( City *pCity, int offset );

// 功能获取
void city_function_open( City *pCity, int offset );
int city_function_check( City *pCity, int offset );

// 主角经验升级
int city_actorexp( int city_index, int exp, char path );
int city_actorupgrade( int city_index, char path, AwardGetInfo *getinfo );

// 体力
int city_changebody( int city_index, int value, short path );
// 征收次数
int city_changelevy( int city_index, int value, short path );
// 银币
int city_changesilver( int city_index, int value, short path );
// 木材
int city_changewood( int city_index, int value, short path );
// 粮草
int city_changefood( int city_index, int value, short path );
// 镔铁
int city_changeiron( int city_index, int value, short path );
// 人口
int city_changepeople( int city_index, int value, short path );
// 威望值
int city_changeprestige( int city_index, int value, short path );
// 友谊积分
int city_changefriendship( int city_index, int value, short path );

#endif
