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
				short view_areaindex;	//当前视口的地图区域
				char name[22];	//角色名称
				char aclass;	//角色种类
				unsigned char shape;	//形象
				unsigned char level;	//级别
				int city_index;	//城池索引
				int token;	//钻石
				int total_charge;	//总充值
				unsigned char itemext;	//扩展的背包栏位数
				Item item[500];	//角色携带的道具
				Item equip[10];	//角色装备栏
				int sflag;	//特定标志位
				int fdate;	//刷新的日子是哪天
				char today_char[128];	//char类型每日次数
				int today_int[8];	//int类型每日次数
				int cd[8];	//通用CD时间
				char config[8];	//玩家配置
				int view_lastframe;	//上次发送的区域信息时间
				short subscribe_cmd[8];	//订阅的消息ID
				float charge_dollar;	//总共充值多少美元
				int cdkey1;	//是否领取该批次兑换码
				int cdkey2;	//是否领取该批次兑换码
				int cdkey3;	//是否领取该批次兑换码
				int cdkey4;	//是否领取该批次兑换码
				int talkspeed_frame;	//说话延迟
				char cdkeywait;	//CDKEY等待
};
typedef struct _actor Actor;

int actor_load_auto( int actorid, Actor *pActor, char *pTab );
int actor_save_auto( Actor *pActor, char *pTab, FILE *fp );

#endif
