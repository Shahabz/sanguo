
#ifndef __ACTIVITY_H
#define __ACTIVITY_H

#define MAX_ACTIVITY_COUNT					64

#define ACTIVITY_NORMAL						0 

#define ACTIVITY_1	1	//	首充礼包
#define ACTIVITY_2	2	//	主城等级
#define ACTIVITY_3	3	//	攻城略地
#define ACTIVITY_4	4	//	七日狂欢
#define ACTIVITY_5	5	//	七星拜将
#define ACTIVITY_6	6	//	全服返利
#define ACTIVITY_7	7	//	大咖带队
#define ACTIVITY_8	8	//	成长计划
#define ACTIVITY_9	9	//	幸运罗盘
#define ACTIVITY_PAYBAG	10	//	特价礼包
#define ACTIVITY_11	11	//	出师大宴
#define ACTIVITY_12	12	//	副本掉落
#define ACTIVITY_13	13	//	流寇图纸
#define ACTIVITY_14	14	//	流寇资源
#define ACTIVITY_15	15	//	经验翻倍
#define ACTIVITY_16	16	//	作坊加速
#define ACTIVITY_EVERYDAY_PAY	17	//	每日充值
#define ACTIVITY_18	18	//	镔铁转盘
#define ACTIVITY_19	19	//	采集翻倍
#define ACTIVITY_20	20	//	屯田活动
#define ACTIVITY_21	21	//	国器暴击
#define ACTIVITY_22	22	//	皇城采集
#define ACTIVITY_23	23	//	官员召唤
#define ACTIVITY_24	24	//	募兵翻倍
#define ACTIVITY_25	25	//	消费有礼
#define ACTIVITY_26	26	//	强国策
#define ACTIVITY_27	27	//	西凉暴乱
#define ACTIVITY_28	28	//	黄巾军暴乱
#define ACTIVITY_29	29	//	体力大卖
#define ACTIVITY_30	30	//	图纸活动
#define ACTIVITY_31	31	//	全民寻宝
#define ACTIVITY_32	32	//	抢夺名城
#define ACTIVITY_33	33	//	充值排行
#define ACTIVITY_34	34	//	屯银排行
#define ACTIVITY_35	35	//	屯木排行
#define ACTIVITY_36	36	//	屯粮排行
#define ACTIVITY_37	37	//	屯铁排行
#define ACTIVITY_38	38	//	国战排行
#define ACTIVITY_39	39	//	城战排行
#define ACTIVITY_40	40	//	洗练排行
#define ACTIVITY_41	41	//	建设排行
#define ACTIVITY_42	42	//	锻造排行
#define ACTIVITY_43	43	//	充值排行
#define ACTIVITY_44	44	//	兵力排行

typedef struct _activity_item
{
	int m_activityid;
	int m_starttime;	// 活动启动时间
	int m_endtime;		// 活动结束时间
	int m_closetime;	// 活动关闭时间，好多活动为了玩家看积分排名什么的，会有最后隐藏时间
	int m_nexttime;		// 下次开启时间
	int m_value[4];
	char m_strvalue[256];
	char m_openstat;
	char m_endstat;
}ActivityItem;

int activity_init();
int activity_logic();
int activity_onwarning( int activityid, int surplus );
int activity_onopen( int activityid );
int activity_onend( int activityid );
int activity_onclose( int activityid );
int activity_onlogic( int activityid );

// 获得活动启动时间
int activity_starttime( int activityid );

// 活动结束时间
int activity_endtime( int activityid );

// 活动关闭时间
int activity_closetime( int activityid );

// 获得活动剩余时间
int activity_lefttime( int activityid );

// 获得活动应该持续时间
int activity_totaltime( int activityid );

int activity_intime( int activityid );
int activity_inclose( int activityid );
int activity_settime( int activityid, int begin_min, int len_min, int value0, int value1, int value2, int value3, char *pstr );
int activity_setendtime( int activityid, int endtime );
int activity_getdata( int activityid, int *value, char *pstr );
int activity_setdata( int activityid, int value0, int value1, int value2, int value3, char* pstr );
int activity_getvalue( int activityid, int index );
int activity_setopenstat( int activityid, int stat );
int activity_setendstat( int activityid, int stat );
int activity_countdown( int activityid );

// 活动列表
int activity_sendlist( int actor_index );

// 攻城略地活动
int activity_03_sendinfo( int actor_index );
int activity_03_get( int actor_index, int id );

// 成长计划活动
int activity_08_sendinfo( int actor_index );
int activity_08_open( int actor_index );
int activity_08_get( int actor_index, int id );

// 出师大宴活动
int activity_body_sendinfo( int actor_index );
int activity_body_get( int actor_index );
#endif
