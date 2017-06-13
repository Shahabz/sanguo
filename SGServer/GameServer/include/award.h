#ifndef _AWARD_H_
#define _AWARD_H_

// -1~-100000 组套组的编号
// 1~100000 物品
// 100000~200000普通资源奖励
#define AWARDKIND_RESBASE	100000
#define AWARDKIND_WOOD		 AWARDKIND_RESBASE+1		//	木材

// 离线情况下的给予属性
#define AWARDKIND_OTHERBASE	200000
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
