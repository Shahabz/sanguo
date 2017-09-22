#ifndef __CITY_AUTO_H
#define __CITY_AUTO_H
#include "define.h"
#include "server_structsend_auto.h"
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
				int battlepower_building;	//建筑提供战力
				int battlepower_hero;	//英雄提供战力
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
				int ptsec;	//城池保护状态剩余时间
				short peoplesec;	//城池人口回复时间
				int eventsec;	//事件倒计时
				short questid[6];	//任务ID
				int questvalue[6];	//任务数值
				char techlevel[40];	//科技
				char techprogress[40];	//科技进度
				int data_record[16];	//数据记录
				char matkind[8];	//材料生产kind
				char matnum[8];	//材料生产数量
				int matsec[8];	//材料已经生产时间
				char matquenum;	//材料生产队列数量
				char matkind_will[8];	//材料预定生产kind
				char matnum_will[8];	//材料预定生产数量
				char matkind_over[8];	//材料生产完毕
				char matnum_over[8];	//材料生产完毕
				short nequip[6];	//国器等级
				char nequip_kind;	//正在升级的国器
				int nequip_sec;	//国器升级时间
				short guardsec;	//城墙守卫冷却时间
				CityGuard guard[30];	//城墙守卫
				Hero hero[12];	//上阵英雄
				Building building[6];	//普通建筑
				BuildingBarracks building_barracks[4];	//兵营建筑
				BuildingRes building_res[64];	//资源建筑
				char worker_op;	//建造队列操作
				int worker_sec;	//建造剩余时间(每秒-1)
				char worker_kind;	//当前升级建筑类型
				char worker_offset;	//当前升级建筑索引
				char worker_free;	//是否使用过免费
				char worker_op_ex;	//建造队列操作(商用)
				int worker_sec_ex;	//建造剩余时间(每秒-1)(商用)
				char worker_kind_ex;	//当前升级建筑类型(商用)
				char worker_offset_ex;	//当前升级建筑索引(商用)
				char worker_free_ex;	//是否使用过免费
				int worker_expire_ex;	//商用建造队列到期时间
				int wnsec;	//建造队列所需时间
				int wnsec_ex;	//建造队列所需时间
				int wnquick;	//建造队列奖励的加速
				int wnquick_ex;	//建造队列奖励的加速
				short ofkind[3];	//建筑官员种类
				int ofsec[3];	//建筑官员剩余秒
				int offree[3];	//建筑官员免费情况
				int ofquick[3];	//建筑官员已经使用的加速时间
				SLK_NetS_CityEvent city_event[4];	//城内事件
				SLK_NetS_BattleEvent battle_event[4];	//军事事件
				CityAttr attr;	//属性加成
				int actor_index;	//角色索引
				int unit_index;	//显示索引
				int battle_armyindex[8];	//出征部队
				int underfire_armyindex[8];	//目标部队（暂别用）
				int help_armyindex[32];	//协防部队
				int mapevent_index[6];	//地图事件
				int underfire_groupindex[16];	//目标集结索引
};
typedef struct _city City;

typedef City * (*LPCB_GETCITY)( int index );
typedef int (*LPCB_LOADCITY)( int index );
int city_load_auto( LPCB_GETCITY pCB_GetCity, LPCB_LOADCITY pCB_LoadCity, const char *pTab );
int city_save_auto( City *pCity, const char *pTab, FILE *fp );

#endif
