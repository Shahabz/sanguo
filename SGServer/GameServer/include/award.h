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
#define AWARDKIND_TRAIN_QUICK	50017 // 募兵加速
#define AWARDKIND_BUILD_QUICK	50018 // 建造加速
#define AWARDKIND_HERO_WASH		50019 // 免费洗髓次数
#define AWARDKIND_EQUIP_WASH	50020 // 免费洗练次数
#define AWARDKIND_LOSTREBUILD	50021 // 高级重建次数
#define AWARDKIND_CITYGUARDNUM	50022 // 城防补充次数
#define AWARDKIND_QUESTFIGHT	50023 // 任务战斗
#define AWARDKIND_CHANGENAME	50024 // 任务改名
#define AWARDKIND_BUFF_TRAIN	50025 // 武卒官加速N%，时间1天
#define AWARDKIND_BUFF_MARCH	50026 // 行军耗时降低N%，时间1天
#define AWARDKIND_LEVY_SILVER	50027 // 奖励N次银币征收量
#define AWARDKIND_LEVY_WOOD		50028 // 奖励N次木材征收量
#define AWARDKIND_LEVY_FOOD		50029 // 奖励N次粮食征收量
#define AWARDKIND_LEVY_IRON		50030 // 奖励N次镔铁征收量
#define AWARDKIND_TIANCE_POINT		50031 // 天策府点数
#define	AWARDKIND_TIANCE_POINT_1	50032 // 天策府点数魏国
#define	AWARDKIND_TIANCE_POINT_2	50033 // 天策府点数蜀国
#define	AWARDKIND_TIANCE_POINT_3	50034 // 天策府点数吴国
#define	AWARDKIND_TEACHERAWARDNUM	50035 // 师徒奖励次数
#define AWARDKIND_PERMISSION_1	50051 // 购买步兵权限（只用于显示）
#define AWARDKIND_PERMISSION_2	50052 // 购买骑兵权限（只用于显示）
#define AWARDKIND_PERMISSION_3	50053 // 购买弓兵权限（只用于显示）
#define AWARDKIND_PERMISSION_4	50054 // 科技快研
#define AWARDKIND_PERMISSION_5	50055 // 装备回收图纸
#define AWARDKIND_PERMISSION_6	50056 // 作坊预设

#define AWARDKIND_SHAPE_0		51000 // 男1
#define AWARDKIND_SHAPE_1		51001 // 男2
#define AWARDKIND_SHAPE_2		51002 // 男3
#define AWARDKIND_SHAPE_20		51020 // 女1
#define AWARDKIND_SHAPE_21		51021 // 女2
#define AWARDKIND_SHAPE_22		51022 // 女3
#define AWARDKIND_SHAPE_END		51100 // 头像结束

#define AWARDKIND_RES_SILVER	60001 // 奖励未启用资源点（民居）
#define AWARDKIND_RES_WOOD		60002 // 奖励未启用资源点（木材厂）
#define AWARDKIND_RES_FOOD		60003 // 奖励未启用资源点（农田）
#define AWARDKIND_RES_IRON		60004 // 奖励未启用资源点（矿厂）

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
int awardgroup_sendinfo( int actor_index, int awardgroup, int callback_code, int value, int limitcount );

// 随机奖励组，不直接给
int awardgroup_random( int awardgroup, int level, AwardGetInfo *getinfo );

// 奖励结构序列化字符串
int awardgroup_makestr( AwardGetInfo *awardInfo, char *awardContent );

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
