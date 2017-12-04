#ifndef __GAMELOG_H
#define __GAMELOG_H

#include "utils.h"

/* 操作 */
#define LOGOP_TOKEN				1	// 钻石
#define LOGOP_HERO				2	// 英雄
#define LOGOP_ITEMGET			3	// 获得道具
#define LOGOP_ITEMLOST			4	// 丢失道具
#define LOGOP_ACTOREXP			5	// 角色经验
#define LOGOP_UPGRADE			6	// 角色升级
#define LOGOP_BODY				7	// 体力
#define LOGOP_VIPEXP			8	// vip经验
#define LOGOP_BUILDING			9	// 建筑
#define LOGOP_EQUIPGET			10	// 获得装备
#define LOGOP_EQUIPLOST			11	// 丢失装备
#define LOGOP_QUEST				12	// 任务
#define LOGOP_SILVER			13	// 银币
#define LOGOP_WOOD				14	// 木材
#define LOGOP_FOOD				15	// 粮食
#define LOGOP_IRON				16	// 镔铁
#define LOGOP_TECH				17	// 科技
#define LOGOP_BARRACKS			18	// 兵营操作
#define LOGOP_FORGING			19	// 打造
#define LOGOP_HEROUPGRADE		20	// 英雄升级
#define LOGOP_KWPOINT			21	// 血战积分

/* 途径 */
#define PATH_SYSTEM						1	// 系统
#define PATH_GM							2	// GM
#define PATH_PAY						3	// 充值
#define PATH_ITEMUSE					4	// 道具使用
#define PATH_TOKENITEMUSE				5	// 钻石直接使用
#define PATH_SELL						6	// 售卖丢弃
#define PATH_BUILDING_UPGRADE			7	// 建筑
#define PATH_QUEST						8	// 任务
#define PATH_GUARD_UPGRADE				9	// 守卫升级
#define PATH_LEVY						10	// 征收
#define PATH_TRAIN						11	// 训练
#define PATH_RESOLVE					12	// 分解
#define PATH_EQUIPEXT					13	// 购买装备容量
#define PATH_TECH_UPGRADE				14	// 科技升级
#define PATH_TECH_FINISH				15	// 科技完成
#define PATH_TECH_GET					16	// 科技领取
#define PATH_TECH_QUICK					17	// 科技加速
#define PATH_TECH_FREEQUICK				18	// 科技免费加速
#define PATH_HIRE_FORGING				19	// 雇佣
#define PATH_HIRE_GOV					20	// 雇佣
#define PATH_HIRE_TECH					21	// 雇佣
#define PATH_BUILDING_WORKER			22	// 商用建造队列
#define PATH_TRAIN_QUEUE				23	// 招募队列
#define PATH_TRAIN_LONG					24	// 训练时长
#define PATH_TRAIN_GET					25	// 训练领取
#define PATH_TRAIN_CANCEL				26	// 取消招募
#define PATH_EQUIP_FORGING				27	// 装备打造
#define PATH_FORGING_QUICK				28	// 打造加速
#define PATH_FORGING_FREEQUICK			29	// 铁匠加速
#define PATH_BODYBUY					30	// 购买体力
#define PATH_CHANGENAME					31	// 改名
#define PATH_GUARD_LIFE					32	// 守卫补血
#define PATH_EQUIP_UP					33	// 装备上
#define PATH_EQUIP_DOWN					34	// 装备下
#define PATH_HERO_ADDEXP				35	// 加英雄经验
#define PATH_HERO_ADDSOLDIERS			36	// 补兵
#define PATH_HERO_SOLDIERS_EQUIP		37	// 脱卸装备带来的兵力损失
#define PATH_HERO_WASH					38	// 英雄洗髓
#define PATH_EQUIP_WASH					39	// 装备洗练
#define PATH_MATERIALMAKE_QUEUE			40	// 材料制造队列
#define PATH_MATERIALMAKE				41	// 材料制造
#define PATH_STORY						42	// 副本
#define PATH_GATHER						43	// 采集
#define PATH_FIGHT						44	// 战斗
#define PATH_MARCH						45	// 行军
#define PATH_MAPEVENT					46	// 地图随机事件
#define PATH_MAIL_ATTACH				47	// 附件
#define PATH_ENEMY						48	// 流寇
#define PATH_SPY						49	// 侦察
#define PATH_TOWNASK					50	// 申请据点城主
#define PATH_TOWNREPAIR					51	// 据点修复
#define PATH_TOWNLEVY					52	// 据点征收
#define PATH_TOWNDEV_CONTRIBUTE			53	// 都城开发捐献
#define PATH_WORLDBOSS					54	// 世界boss
#define PATH_LOSTREBUILD				55	// 重建家园
#define PATH_KINGWAR_SNEAK				56	// 血战活动-偷袭
#define PATH_KINGWAR_PK					57	// 血战活动-单挑
#define PATH_KINGWAR_DEFENSE			58	// 血战活动-回防
#define PATH_KINGWAR_REBIRTH			59	// 血战活动-复活
#define PATH_KINGWAR_CHANGE				60	// 血战积分兑换
#define PATH_KINGWAR_TREASURE			61	// 血战挖宝
#define PATH_KINGWAR_TREASURE_CO		62	// 血战恭贺
#define PATH_HEROVISIT					63	// 武将寻访
#define PATH_HEROVISIT_CHANGE			64	// 武将寻访-英雄转道具
#define PATH_HERO_UP					65	// 上阵
#define PATH_HERO_DOWN					66	// 下阵
#define PATH_HERO_GUARD					67	// 御林卫
#define PATH_HERO_GUARD_AUTO			68	// 御林卫自动

int log_init();
int wlog( char type, int op, short path, int object, int objectnum, i64 objectid, int source, int target );
int wlog_token( char type, int op, short path, int object, int objectnum, i64 objectid, int source, int target, i64 userid );
int wcount();

#endif

