#ifndef __ACTOR_AUTO_H
#define __ACTOR_AUTO_H
#include "define.h"

struct _actor {
//--Automatically generated
				int actorid;	//角色编号
				int accountid;	//登陆id
				i64 userid;	//账号编号
				char username[51];	//账号名
				short admin;	//管理员级别
				char lastip[16];	//上次登陆ip
				int createtime;	//创建时间
				int forbidtime;	//禁言时间
				int savetime;	//上次保存时间
				unsigned char isexit;	//是否离开游戏中
				short view_areaindex;	//当前视口的地图区域（逻辑）
				char name[22];	//角色名称
				char nation;	//角色国家
				unsigned char shape;	//形象
				short level;	//级别
				short view_zoneid;	//当前视口的地图区域（游戏）
				int city_index;	//城池索引
				int token;	//钻石
				int total_charge;	//总充值
				float charge_dollar;	//总消费多少钱
				unsigned char itemext;	//扩展的背包栏位数
				Item item[300];	//角色携带的道具
				Equip equip[180];	//角色携带的装备
				unsigned char equipext;	//扩展的装备栏位数
				Hero hero[80];	//未上阵英雄
				int quest_talkid;	//任务对话
				int worldquest_complete;	//世界任务领取情况
				short worldquestid;	//世界任务
				ActorFriend friends[100];	//好友
				int blacklist[50];	//黑名单
				int sflag;	//特定标志位
				int fdate;	//刷新的日子是哪天
				char today_char[128];	//char类型每日次数
				short today_short[32];	//short类型每日次数
				int today_int[8];	//int类型每日次数
				int cd[8];	//通用CD时间
				char config[8];	//玩家配置
				int permission;	//权限是否开启
				short storyid;	//副本进度章节
				char story_star[128];	//精英副本星级
				char story_hero[32];	//招募副本
				char story_resnum[32];	//资源副本次数
				int story_restime[32];	//资源副本时间
				char story_resreset[32];	//资源副本重置次数
				char story_itemnum[64];	//道具副本掉落次数
				char story_drawing[16];	//装备图纸副本是否购买
				short story_sweephero[4];	//扫荡武将
				int view_lastframe;	//上次发送的区域信息时间
				short subscribe_cmd[8];	//订阅的消息ID
				int lastpeople_c;	//上次本城人口
				int lastpeople_n;	//上次国家人口
				int cdkey1;	//是否领取该批次兑换码
				int cdkey2;	//是否领取该批次兑换码
				int cdkey3;	//是否领取该批次兑换码
				int cdkey4;	//是否领取该批次兑换码
				char hv_ln;	//良将寻访次数
				char hv_hn;	//神将寻访次数
				short hv_pro;	//当前寻访进度
				int vipbag;	//vip礼包是否购买
				char shop_useitem;	//商店是否优先使用道具购买符
				int charge_point;	//充值积分
				short pay_maxtier;	//单笔付费最大级别
				short shop_saleitem[6];	//商店打折物品
				char neq_crit[6];	//国器暴击状态
				char mapcallfree;	//免费召唤次数
				short wishingid[9];	//聚宝盆道具列表
				char wishingopen[9];	//聚宝盆道具开启
				int wishingday;	//聚宝盆道具哪天的
				int wishingcd;	//聚宝盆寻宝冷却
				int shape_bag;	//形象背包
				int student[30];	//徒弟
				char te_award[10];	//师徒奖励可领取次数
				char te_awarded[10];	//师徒奖励已经领取次数
				int te_shop;	//师徒商店
				int act02_state;	//主城等级活动领取奖励
				char act03_state;	//攻城掠地活动领取奖励
				char act04_state[128];	//七日狂欢活动状态
				int act04_value[40];	//七日狂欢活动数值
				short act05_item[6];	//七星拜将活动-道具列表
				char act05_isbuy;	//七星拜将活动-道具是否购买
				char act05_buynum;	//七星拜将活动-道具购买次数
				char act05_xw;	//七星拜将活动-信物
				char act05_upnum;	//七星拜将活动-剩余刷新次数
				int act05_upstamp;	//七星拜将活动-刷新时间
				short act08_state;	//成长计划活动领取奖励
				char fs_nodeid;	//坊市节点
				int fs_awardkind[14];	//坊市寻访奖励
				int fs_awardnum[14];	//坊市寻访奖励
				int fs_awardfday;	//坊市寻访奖励日期
				int fs_weekkind[5];	//皇宫内院奖励
				int fs_weeknum[5];	//皇宫内院奖励
				int fs_fweek;	//坊市周数
				short girlshop[9];	//女将商店
				short girlshop_buy;	//女将商店是否购买
				int girlshop_fday;	//女将商店刷新天数
				char maidname[22];	//侍女名称
				int token_sale;	//元宝抵扣点数
				int token_ret;	//返利次数
				int talkspeed_frame;	//说话延迟
				char cdkeywait;	//CDKEY等待
				short mail_notreadnum;	//未读邮件数量
};
typedef struct _actor Actor;

int actor_load_auto( int actorid, Actor *pActor, const char *pTab );
int actor_save_auto( Actor *pActor, const char *pTab, FILE *fp );

#endif
