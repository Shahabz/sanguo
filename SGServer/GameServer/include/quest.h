#ifndef _QUEST_H_
#define _QUEST_H_
#include "define.h"

#define QUEST_COMPLETEFLAG_NORMAL	0	// 缺省
#define QUEST_COMPLETEFLAG_SUCCESS	1	// 完成
#define QUEST_COMPLETEFLAG_AWARDED	2	// 已经领取奖励

#define QUEST_DATATYPE_WELCOME				0	// 欢迎任务
#define	QUEST_DATATYPE_CREATENAME			1	// 创建名称
#define	QUEST_DATATYPE_CLICK				2	// 点击操作 datakind=1马岱 2前往世界 3返回城内 4查看铁匠铺 5前往副本
#define	QUEST_DATATYPE_NEWFIGHT				3	// 围剿扰乱民居的山贼
#define QUEST_DATATYPE_ACTOR_LEVEL			11	// 主公等级 datatype=11 datakind=0 needvalue=等级
#define	QUEST_DATATYPE_BUILDING_LEVEL		12	// 建筑等级 datatype=12 datakind=建筑kind dataoffset=资源建筑编号 needvalue=建筑等级
#define	QUEST_DATATYPE_BUILDING_SILVER		13	// N个民居达到N级 datatype=13 datakind=等级 needvalue=建筑数量
#define	QUEST_DATATYPE_BUILDING_WOOD		14	// N个木场达到N级 datatype=14 datakind=等级 needvalue=建筑数量
#define	QUEST_DATATYPE_BUILDING_FOOD		15	// N个农田达到N级 datatype=15 datakind=等级 needvalue=建筑数量
#define	QUEST_DATATYPE_BUILDING_IRON		16	// N个矿场达到N级 datatype=16 datakind=等级 needvalue=建筑数量
#define QUEST_DATATYPE_LEVYNUM				17	// 征收次数 datatype=17 datakind=0 needvalue=数量
#define QUEST_DATATYPE_STORY				18	// 副本 datatype=18 datakind=副本编号 needvalue=1
#define	QUEST_DATATYPE_KILLMONSTER			19	// 打流寇N次 datatype=19 datakind=0 needvalue=数量
#define	QUEST_DATATYPE_KILLMONSTER_LEVEL	20	// 打流寇N级N次 datatype=20 datakind=等级 needvalue=数量
#define	QUEST_DATATYPE_EQUIP_FORGING		21	// 打造N装备N数量 datatype=21 datakind=装备kind needvalue=数量
#define	QUEST_DATATYPE_EQUIP_WASH			22	// 装备洗炼次数 datatype=22 datakind=0 needvalue=数量
#define	QUEST_DATATYPE_EQUIP_COLORWASH		23	// 将N品质装备的属性洗炼至满级 datatype=23 datakind=颜色 needvalue=1
#define	QUEST_DATATYPE_TRAIN				24	// 募兵N兵种N数量 datatype=24 datakind=兵种（1，2，3） needvalue=数量
#define	QUEST_DATATYPE_TRAINCOUNT			25	// 募兵N次 datatype=25 datakind=0 needvalue=数量
#define	QUEST_DATATYPE_CITY_TECH			26	// 研究N科技N级 datatype=26 datakind=科技ID needvalue=等级
#define	QUEST_DATATYPE_CITY_TECHONE			27	// 研究一次N科技 datatype=27 datakind=科技ID needvalue=1
#define	QUEST_DATATYPE_HERO_WASHCOUNT		28	// 武将洗髓N次 datatype=28 datakind=0 needvalue=次数
#define	QUEST_DATATYPE_HERO_COLORWASH		29	// 将一名N品质的武将属性洗满 datatype=29 datakind=颜色 needvalue=1
#define	QUEST_DATATYPE_HERO_CALL			30	// 招募武将 datatype=30 datakind=kind needvalue=1
#define	QUEST_DATATYPE_WISHING				31	// 聚宝盆N次 datatype=31 datakind=0 needvalue=次数
#define	QUEST_DATATYPE_CITYFIGHT			32	// 成功击败N名敌国玩家 datatype=32 datakind=0 needvalue=次数
#define	QUEST_DATATYPE_EQUIP_UP				33	// 给N武将穿N装备 datatype=33 datakind=武将kind dataoffset=装备编号
#define	QUEST_DATATYPE_BUILDING_UPGRADE		34	// 操作-升级 datatype=34 datakind=建筑kind dataoffset=资源建筑编号 needvalue=建筑等级
#define	QUEST_DATATYPE_TRAIN_OP				35	// 操作-募兵N兵种N数量 datatype=24 datakind=兵种（1，2，3） needvalue=数量
#define	QUEST_DATATYPE_TRAINCOUNT_OP		36	// 操作-募兵N次 datatype=25 datakind=0 needvalue=数量
#define	QUEST_DATATYPE_BRANCH_QUEST			37	// 完成任意N个支线任务 datatype=37 datakind=0 needvalue=数量
#define	QUEST_DATATYPE_EQUIP_FORGING_QUICK	38	// 进行一次铁匠铺加速打造 datatype=38 datakind=0 needvalue=1
#define	QUEST_DATATYPE_HERO_UP				39	// 前往聚贤馆让N上阵 datatype=39 datakind=武将kind needvalue=1
#define	QUEST_DATATYPE_SMITHYOFFHIRE		40	// 铁匠铺内雇佣N级或更高级铁匠 datatype=40 datakind=0 needvalue=等级
#define	QUEST_DATATYPE_NEQUIP_FORGING		41	// 打造国器N datatype=41 datakind=国器kind needvalue=1
#define	QUEST_DATATYPE_NEQUIP_LEVELNUM		42	// 国器升级N次 datatype=42 datakind=0 needvalue=次数
#define	QUEST_DATATYPE_MATERIAL_MAKE		43	// 材料作坊生产材料N次（操作） datatype=43 datakind=0 needvalue=次数
#define	QUEST_DATATYPE_HEROGUARD_UP			44	// 御林卫上阵任意武将N个 datatype=44 datakind=0 needvalue=数量
#define	QUEST_DATATYPE_HERO_VISIT			45	// 进行良将寻访N次 datatype=45 datakind=0 needvalue=数量
#define	QUEST_DATATYPE_HERO_CALL_STORY		46	// 副本招募武将 datatype=46 datakind=副本id needvalue=1
#define	QUEST_DATATYPE_EQUIP_FORGING_OP		47	// 操作-打造N装备N数量 datatype=21 datakind=装备kind needvalue=数量

// 任务类型
enum 
{
	QUEST_TYPE_MAIN = 0,	// 主线任务
	QUEST_TYPE_BRANCH,		// 支线任务
	QUEST_TYPE_MAXNUM
};
QuestInfo *quest_config( int questid );
// 第一个任务
int quest_newplayer( int actor_index );

// 给与一个主线任务
int quest_give_main( int actor_index, int questid );

// 给与一个支线任务
int quest_give_branch( int actor_index, int questid );

// 添加任务数值并主动检查任务是否完成
int quest_addvalue( City *pCity, int datatype, int datakind, int dataoffset, int value );

// 添加任务数值，并主动检查任务是否完成(主线任务使用的)
int quest_main_addvalue( City *pCity, int datatype, int datakind, int dataoffset, int value );

// 客户端发过来的操作任务完成
int quest_client_op( int actor_index, int questid, int datakind );

// 被动检查身上任务是否完成
int quest_checkcomplete( int actor_index );

// 任务数值检查返回是否完成
int quest_check( int actor_index, int questid, int *value );

// 领取任务奖励
int quest_getaward( int actor_index, int questid );

// 发送任务列表
int quest_sendlist( int actor_index );

// 发送任务奖励
int quest_sendawardinfo( int actor_index, int questid );

// 任务说话
int quest_talk( int actor_index, int talkid );

// 客户端点击后，获取下一条对话
int quest_talk_next( int actor_index, int talkid );

// 任务说话检查
int quest_talk_check( int actor_index );

// 客户端请求一个对话
int quest_talk_client_ask( int actor_index, int talkid );

// 改名
int quest_changename( int actor_index );
// 战斗
int quest_fight( int actor_index, int storyid );

// GM重置所有任务
int quest_gm_reset( int actor_index );

// city数据记录
#define DATA_RECORD_MAXNUM						16
#define DATA_RECORD_KILLENEMY					0	// 流寇数量
#define DATA_RECORD_KILLCITY					1	// 城战数量
#define DATA_RECORD_KILLTOWN					2	// 国战数量
#define DATA_RECORD_KILLTOWNTYPE1				3	// 成功攻打营地次数
#define DATA_RECORD_KILLTOWNTYPE2				4	// 成功攻打城门次数
#define DATA_RECORD_KILLTOWNTYPE3				5	// 成功攻打郡城次数
#define DATA_RECORD_KILLTOWNTYPE4				6	// 成功攻打州郡次数
#define DATA_RECORD_KILLTOWNTYPE5				7	// 成功攻打州府次数
#define DATA_RECORD_KILLTOWNTYPE6				8	// 成功攻打州城次数
#define DATA_RECORD_KILLTOWNTYPE7				9	// 成功攻打名城次数
int data_record_addvalue( struct _city *pCity, int offset, int value );
int data_record_setvalue( struct _city *pCity, int offset, int value );
int data_record_getvalue( struct _city *pCity, int offset );

// 天策府任务
int tiance_quest_sendinfo( int actor_index );
int tiance_quest_get( int actor_index, short kind );
int tiance_quest_addnum( City *pCity );
int tiance_quest_complete( int actor_index, short kind );
int tiance_quest_tech_activate( int actor_index, short kind );
#endif
