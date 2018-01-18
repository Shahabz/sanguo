
#ifndef __GMCMD_H
#define __GMCMD_H

#define GMC_BASE				0
#define GMC_TEST				(GMC_BASE+1)	// 测试
#define GMC_SC					(GMC_BASE+2)	// 脚本使用
#define GMC_LUA					(GMC_BASE+3)	// 重载lua
#define GMC_RELOAD				(GMC_BASE+4)	// 重读数据表
#define GMC_RESET				(GMC_BASE+5)	// 重置等级数据
#define GMC_EXP					(GMC_BASE+6)	// 经验
#define GMC_TOKEN				(GMC_BASE+7)	// 钻石
#define GMC_ITEM				(GMC_BASE+8)	// 道具
#define GMC_EQUIP				(GMC_BASE+9)	// 装备
#define GMC_HERO				(GMC_BASE+10)	// 英雄
#define GMC_SILVER				(GMC_BASE+11)	// 银币
#define GMC_WOOD				(GMC_BASE+12)	// 木材
#define GMC_FOOD				(GMC_BASE+13)	// 粮食
#define GMC_IRON				(GMC_BASE+14)	// 镔铁
#define GMC_BODY				(GMC_BASE+15)	// 体力
#define GMC_LEVY				(GMC_BASE+16)	// 收割次数
#define GMC_AWARD				(GMC_BASE+17)	// 奖励
#define GMC_QUEST				(GMC_BASE+18)	// 任务
#define GMC_ROLLMSG				(GMC_BASE+19)	// 跑马灯
#define GMC_MAIL				(GMC_BASE+20)	// 邮件
#define GMC_SYSTALK				(GMC_BASE+21)	// 系统喊话
#define GMC_VIPEXP				(GMC_BASE+22)	// vip经验
#define GMC_ACTIVITY			(GMC_BASE+23)	// 活动
#define GMC_TECH				(GMC_BASE+24)	// 科技
#define GMC_BUILDING			(GMC_BASE+25)	// 建筑
#define GMC_PEOPLE				(GMC_BASE+26)	// 人口
#define GMC_MAT					(GMC_BASE+27)	// 材料生产
#define GMC_FUN					(GMC_BASE+28)	// 获取功能
#define GMC_EVENT				(GMC_BASE+29)	// 地图事件刷新
#define GMC_VW					(GMC_BASE+30)	// 威望
#define GMC_TOWNATTACK			(GMC_BASE+31)	// 都城攻击
#define GMC_WORLDBOSS			(GMC_BASE+32)	// 世界boss
#define GMC_KWPOINT				(GMC_BASE+33)	// 血战积分
#define GMC_GIVERES				(GMC_BASE+34)	// 给与一个未启用资源点
#define GMC_AUTOBUILD			(GMC_BASE+35)	// 自动建造次数
#define GMC_PAYBAG				(GMC_BASE+36)	// 礼包
#define GMC_BUILDINGLEVEL		(GMC_BASE+37)	// 建筑等级
#define GMC_STORY				(GMC_BASE+38)	// 打通副本
#define GMC_PRESTIGE			(GMC_BASE+39)	// 威望
#define GMC_NATIONMISSION		(GMC_BASE+40)	// 国家荣誉任务
#define GMC_NATIONQUEST			(GMC_BASE+41)	// 国家任务
#define GMC_NATIONRANK			(GMC_BASE+42)	// 国家荣誉排行榜
#define GMC_NATIONEXP			(GMC_BASE+43)	// 国家经验

// GM指令使用信息
#define GMR_OK					1		// 成功
#define GMR_NOTGMTOOLS			0		// 未使用/不允许Gmtools
#define GMR_CHECK				-1		// 已使用/超时
#define GMR_ERROR				-10
#define GMR_ACCESS_DENIED		-11
#define GMR_NOT_ONLINE			-12
#define GMR_ARG_ERROR			-13

struct _cmdinfo {
	char m_allowgmtools;			// 是否允许GMTools访问
};
typedef struct _cmdinfo SCmdInfo;

int gm_init();
char gm_isallow_gmtools( short cmdid );

int gm_proc( short cmd, int arg1, int arg2, int arg3, int arg4, char *str );
int gm_set_result( int client_index, int value );
int gm_get_result( int client_index );
#endif
