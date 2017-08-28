#ifndef _AWARD_H_
#define _AWARD_H_

// -1~ 组套组的编号
#define AWARDKIND_ITEMBASE		0	  // 道具（0+道具编号）
#define AWARDKIND_EQUIPBASE		10000 // 装备（10000+装备编号）
#define AWARDKIND_HEROBASE		20000 // 英雄（20000+英雄编号）
#define AWARDKIND_BUILDINGBASE	30000 // 建筑（30000+建筑编号）
#define AWARDKIND_FUNCTION		40000 // 功能（40000+功能编号）
#define AWARDKIND_VALUEBASE		50000 // 数值
#define AWARDKIND_SILVER		50001 // 银币
#define AWARDKIND_WOOD			50002 // 木材
#define AWARDKIND_FOOD			50003 // 粮食
#define AWARDKIND_IRON			50004 // 镔铁
#define AWARDKIND_TOKEN			50005 // 元宝
#define AWARDKIND_BODY			50006 // 体力
#define AWARDKIND_INFANTRY		50007 // 步兵
#define AWARDKIND_CAVALRY		50008 // 骑兵
#define AWARDKIND_ARCHER		50009 // 弓兵
#define AWARDKIND_EXP			50010 // 角色经验
#define AWARDKIND_VIPEXP		50011 // VIP经验
#define AWARDKIND_AUTOBUILD		50012 // 自动建造次数
#define AWARDKIND_LEVYNUM		50013 // 征收次数
#define AWARDKIND_PEOPLE		50014 // 人口
#define AWARDKIND_PRESTIGE		50015 // 威望值
#define AWARDKIND_FRIENDSHIP	50016 // 友谊积分

// 离线情况下的给予属性
#define AWARDKIND_OTHERBASE	100000
#define AWARDKIND_QUEST_SUCCESS	AWARDKIND_OTHERBASE+1	// 任务完成

#define AWARDGETINFO_MAXCOUNT	24

// 奖励组(可理解为掉落包)
typedef struct _award_group
{
	int *kind;			// 种类
	int *minnum;		// 数量区间
	int *maxnum;		// 数量区间
	int *value;			// 评价值
	char *color;		// 颜色
	char *show;			// 显示标示
	short allcount;		// 该组奖励物品数量
	int allvalue;		// 该组奖励总评价值
	short *type;        // 类型
	int *appendnum;     // 附加值
}AwardGroup;

// 获取返回的奖励
typedef struct _award_get_info
{
	int kind[AWARDGETINFO_MAXCOUNT];
	int num[AWARDGETINFO_MAXCOUNT];
	char color[AWARDGETINFO_MAXCOUNT];
	short count;
}AwardGetInfo;

// 初始化
int awardgroup_init();

// 重读
int awardgroup_reload();

// 发送给客户端显示的奖励组
int awardgroup_sendinfo( int actor_index, int awardgroup, int callback_code, int unit_index, int limitcount );

// 随机奖励组，不直接给
int awardgroup_random( int awardgroup, int level, AwardGetInfo *getinfo );

// 奖励组 发放到邮件内容里面
int awardgroup_mail( int awardgroup, int level, char * itemContent );

// 随机奖励组，通过索引，默认在线情况
int awardgroup_withindex( int actor_index, int awardgroup, int level, char path, AwardGetInfo *getinfo );

// 随机奖励组，通过玩家id，不确定是否在线
int awardgroup_withid( int actorid, int awardgroup, char path, AwardGetInfo *getinfo );

// 给奖励
int award_getaward( int actor_index, int kind, int num, char color, char path, AwardGetInfo *getinfo );

// 离线奖励部分
int gift( int actorid, int kind1, int num1, char color1, int kind2, int num2, char color2, int kind3, int num3, char color3, int kind4, int num4, char color4, unsigned char path );
int gift_check( int actor_index );

// 离线奖励部分（根据uid）
int gift_uid_check( int actor_index );

// 离线获取礼物（根据uid）
int gift_uid_get( int actor_index );

// 随机数
int awardgroup_randnum( int awardgroup, int level, int index );


#endif
