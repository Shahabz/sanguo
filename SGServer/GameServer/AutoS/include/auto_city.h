#ifndef __CITY_AUTO_H
#define __CITY_AUTO_H
#include "define.h"

struct _city {
//--Automatically generated
				int actorid;	//角色ID
				int index;	//索引
				char name[22];	//名称
				char type;	//城池类型
				char shape;	//领主形象(选取)
				int headid;	//领主头像(自定义)
				short country;	//选择的国家(真实)
				char ipcountry[3];	//IP地址所在国家
				char language;	//选择语言
				char os;	//系统1ios 2android 3win
				char platid;	//平台
				int createtime;	//创建时间
				int lastlogin;	//上次登陆时间
				int lastlogout;	//上次登出时间
				char state;	//城池当前状态
				short posx;	//位置
				short posy;	//位置
				int sflag;	//标志位
				short level;	//玩家等级
				int exp;	//玩家经验
				char viplevel;	//VIP等级
				int vipexp;	//VIP经验
				char nation;	//国家(魏蜀吴)
				char official;	//官职
				char place;	//爵位
				char zone;	//区域
				int battlepower;	//战力
				char mokilllv;	//击杀野怪最高级别
				short body;	//体力
				short bodysec;	//体力恢复时间(每秒-1)
				int silver;	//银币
				int wood;	//木材
				int food;	//粮草
				int iron;	//镔铁
				unsigned char levynum;	//当前征收次数
				short levysec;	//下次征收时间(每秒-1)
				int people;	//人口
				int prestige;	//威望值
				int friendship;	//友谊积分
				int function;	//功能是否开启
				char equip_washnum;	//装备洗练免费次数
				short equip_washsec;	//装备下次免费洗练时间
				short forgingkind;	//打造装备种类
				int forgingsec;	//打造剩余时间
				char hero_washnum;	//英雄洗练次数
				short hero_washsec;	//英雄下次洗练免费时间
				char autobuild;	//自动建造剩余次数
				char autobuildopen;	//自动建造是否开启
				short guardsec;	//城墙守卫冷却时间
