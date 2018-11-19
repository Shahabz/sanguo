
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
#define GMC_AWARDGROUP			(GMC_BASE+17)	// 奖励组
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
#define GMC_NATIONOF			(GMC_BASE+44)	// 国家官员系统
#define GMC_NHERO				(GMC_BASE+45)	// 国家名将
#define GMC_DB					(GMC_BASE+46)	// 数据库重置
#define GMC_PAY					(GMC_BASE+47)	// 支付 
#define GMC_BUFFCLEAR			(GMC_BASE+48)	// buff清除 
#define GMC_AWARD				(GMC_BASE+49)	// 奖励 
#define GMC_CITYINFO			(GMC_BASE+50)	// 城池详情
#define GMC_MAIL_ACTOR			(GMC_BASE+51)	// 玩家邮件
#define GMC_MAIL_SERVER			(GMC_BASE+52)	// 全服邮件
#define GMC_MAIL_NOTICE			(GMC_BASE+53)	// 公告邮件
#define GMC_ACTIVITY_END		(GMC_BASE+54)	// 强制结束活动
#define GMC_ACTIVITY_CLOSE		(GMC_BASE+55)	// 强制关闭活动
#define GMC_KICK				(GMC_BASE+56)	// 踢下线
#define GMC_LOCKACTOR			(GMC_BASE+57)	// 锁角色
#define GMC_LOCKUSER			(GMC_BASE+58)	// 锁账号
#define GMC_UNLOCKACTOR			(GMC_BASE+59)	// 解锁角色
#define GMC_UNLOCKUSER			(GMC_BASE+60)	// 解锁账户
#define GMC_GIRL				(GMC_BASE+61)	// 女将
#define GMC_GIRLSOUL			(GMC_BASE+62)	// 女将碎片
#define GMC_SONQUICK			(GMC_BASE+63)	// 出师加速
#define GMC_ROBOTADD			(GMC_BASE+64)	// 机器人创建
#define GMC_ROBOTDEL			(GMC_BASE+65)	// 机器人删除
#define GMC_ROBOTLOGIC			(GMC_BASE+66)	// 机器人逻辑
#define GMC_ROBOTQUEUE			(GMC_BASE+67)	// 机器人逻辑-队列创建
#define GMC_PAYMODE				(GMC_BASE+68)	// 支付模式
#define GMC_FANGSHI				(GMC_BASE+69)	// 坊市
#define GMC_A12					(GMC_BASE+70)	// 南蛮入侵
#define GMC_RECHARGE			(GMC_BASE+71)	// 充值任意额度 
#define GMC_ARMYDELETE			(GMC_BASE+72)	// 删除玩家所有部队 
#define GMC_HEROSTATE			(GMC_BASE+73)	// 武将状态

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
	char m_online;					// 是否检查在线
};
typedef struct _cmdinfo SCmdInfo;

int gm_init();
char gm_isallow_gmtools( short cmdid );
char gm_check_online( short cmdid );

int gm_proc( short cmd, int arg1, int arg2, int arg3, int arg4, char *str );
int gm_set_result( int client_index, int value );
int gm_get_result( int client_index );
#endif
