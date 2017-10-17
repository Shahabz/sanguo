#ifndef _CITY_H_
#define _CITY_H_
#include "define.h"

#define CITY_QUEST_MAX				6	// 任务数量
#define CITY_TECH_MAX				40	// 城池科技
#define CITY_GUARD_MAX				30	// 城墙守卫
#define CITY_DATA_RECORD_MAX		16	// 数据记录
#define CITY_EVENT_MAX				4	// 事件
#define CITY_TRAINQUEUE_MAX			11	// 可扩容的训练队列数量
#define CITY_MATERIALMAKE_MAX		6	// 6个材料生产队列
#define CITY_BATTLEQUEUE_MAX		8	// 出征队列
#define CITY_UNDERFIRE_MAX			8	// 最多受多少支部队攻击
#define	CITY_UNDERFIRE_GROUP_MAX	16  // 最多受多少个城战
#define CITY_HELPDEFENSE_MAX		32	// 最多受多少支部队驻防
#define CITY_MAPEVENT_MAX			6	// 地图事件最大数量
#define CITY_MAPEVENT_ING_MAX		2	// 地图事件正在进行数量

// 功能入口
#define	CITY_FUNCTION_SMITHY		0	// 铁匠铺
#define	CITY_FUNCTION_WASH			1	// 洗炼铺
#define	CITY_FUNCTION_FANGSHI		2	// 坊市
#define	CITY_FUNCTION_SHOP			3	// 商店
#define	CITY_FUNCTION_HERO			4	// 聚贤馆
#define	CITY_FUNCTION_WISHING		5	// 聚宝盆

#define CITY_FUNCTION_NATION		10	// 国家
#define CITY_FUNCTION_WORLD			11	// 世界
#define CITY_FUNCTION_STORY			12	// 副本
#define CITY_FUNCTION_MAIL			13	// 邮件
#define CITY_FUNCTION_FRIEND		14	// 好友
#define CITY_FUNCTION_NATIONEQUIP	15	// 国器
#define CITY_FUNCTION_RANK			16	// 排行榜
#define CITY_FUNCTION_AUTOBUILD		17	// 自动建造
#define CITY_FUNCTION_CHAT			18	// 聊天
#define CITY_FUNCTION_WARKEREX		19	// 商业建造队

#define CITY_EVENT_BUILDING			1	// 建筑升级
#define CITY_EVENT_TRAIN			2	// 募兵
#define CITY_EVENT_FORGING			3	// 装备打造
#define CITY_EVENT_TECH				4	// 科技研究

#define CITY_BATTLE_EVENT_SPY				1	// 侦查
#define CITY_BATTLE_EVENT_BESPY				2	// 被侦查
#define CITY_BATTLE_EVENT_ASSAULT			3	// 攻城
#define CITY_BATTLE_EVENT_DEFEND			4	// 守城
#define CITY_BATTLE_EVENT_NATION_ASSAULT	5	// 国战
#define CITY_BATTLE_EVENT_NATION_DEFEND		6	// 国战防守

#define CITY_STATE_FIRE				0x01	// 着火中
#define CITY_STATE_ARMYGROUP		0x02	// 被城战中

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

// 获取国家
char city_getnation( int city_index );

// 显示单元属性
void city_makeunit( int city_index, SLK_NetS_AddMapUnit *pAttr );

// 城池位置
void city_getpos( int city_index, short *posx, short *posy );

// 创建一个新城池
int city_new( City *pCity );

// 所有城市每秒的逻辑
void city_logic_sec();

// 城市主城等级
int city_mainlevel( int city_index );

// 设置城市状态
void city_setstate( City *pCity, char state );

// 移除城市状态
void city_delstate( City *pCity, char state );

// 32位整数，标志位操作0或1 标示开没开，有没有，做没做（永久存档）
void city_set_sflag( City *pCity, int offset, char value );
int city_get_sflag( City *pCity, int offset );

// 功能获取
void city_function_open( City *pCity, int offset );
int city_function_check( City *pCity, int offset );

// 事件
int city_event_load( City *pCity );
int city_event_save( City *pCity, FILE *fp );
int city_event_add( int city_index, char type, short kind, int value );
int city_battle_event_add( int city_index, char type, char *name, char value, i64 mailid );
int city_event_sendlist( int actor_index );

// 主角经验升级
int city_actorexp( int city_index, int exp, char path );
int city_actorupgrade( int city_index, short path, AwardGetInfo *getinfo );

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
// 保护时间
int city_changeprotect( int city_index, int value, short path );
// 兵力
int city_changesoldiers( int city_index, short corps, int value, short path );
// 自动建造次数
int city_change_autobuild( int city_index, int value, short path );
// 洗髓次数
int city_change_herowash( int city_index, int value, short path );
// 洗练次数
int city_change_equipwash( int city_index, int value, short path );

// 城墙守卫
CityGuardInfoConfig *city_guard_config( int monsterid, int color );
CityGuard *city_guard_getptr( int city_index, int offset );
int city_guard_call( int city_index );
int city_guard_upgrade( int city_index, int offset );
int city_guard_soldiers( int city_index, int offset );
int city_guard_subsoldiers( int city_index, int offset, int sub );
int city_guard_clearcd( int city_index );
int city_guard_send( int actor_index, int offset );
int city_guard_sendsec( int actor_index );
int city_guard_sendlist( int actor_index );

// 征收
int city_levy( int actor_index );
int city_levy_sendinfo( int actor_index );

// 招募
int city_train( int actor_index, int kind, int trainsec );
int city_train_cancel( int actor_index, int kind, int queue );
int city_train_quick( int actor_index, int kind, int sec );
int city_train_finish( City *pCity, BuildingBarracks *barracks );
int city_train_get( int actor_index, int kind );
int city_train_buyqueue( int actor_index, int kind );
int city_train_buylong( int actor_index, int kind );
int city_train_sendinfo( int actor_index, int kind );
int city_train_awardquick( int city_index, int sec );
int city_train_awardquick_get( int actor_index, int kind );

// 材料生产
int city_material_sendinfo( int actor_index, char sendchange );
int city_material_will_sendinfo( int actor_index );
int city_material_updatebuilding( City *pCity );
int city_material_needsec( City *pCity, int itemkind );
int city_material_make( int actor_index, int id, int itemkind, int willindex );
int city_material_finish( City *pCity, int index );
int city_material_get( int actor_index );
int city_material_buyqueue( int actor_index );
int city_material_gm( City *pCity, int sec );

// 雇佣官
int city_officialhire( int actor_index, int type, int kind );
int city_officialhire_sendinfo( City *pCity, int type );

// 获取士兵数量
int city_soldiers( int city_index, short corps );

// 加入出征队列
int city_battlequeue_add( City *pCity, int army_index );

// 删除出征队列
int city_battlequeue_del( City *pCity, int army_index );

// 出征队列
void city_battlequeue_sendlist( int actor_index, int unit_index );

// 城市出征队列
void city_battlequeue_sendupdate( int army_index );

// 被攻击信息添加
int city_underfire_add( City *pCity, int army_index );

// 被攻击信息移除
int city_underfire_del( City *pCity, int army_index );

// 被攻击信息移除
int city_underfire_del_equal( City *pCity, int equal_army_index );

// 被攻击信息数量
int city_underfire_getnum( City *pCity );

// 被攻击信息添加
int city_underfire_groupadd( City *pCity, int group_index );

// 被攻击信息移除
int city_underfire_groupdel( City *pCity, int group_index );

// 添加驻防部队
int city_helparmy_add( City *pCity, int army_index );

// 删除驻防部队
int city_helparmy_del( City *pCity, int army_index );

// 获取驻防部队数量
int city_helparmy_getnum( City *pCity );

// 获取驻防部队数量上限
int city_helparmy_maxnum( City *pCity );

// 获取驻防部信息
int city_helparmy_sendlist( int actor_index, int unit_index );

// 迁城
int city_move_actor( int actor_index, short posx, short posy, int itemkind );
int city_move( City *pCity, short posx, short posy );

// 侦察
int city_spy( int actor_index, int unit_index, int type );
#endif
