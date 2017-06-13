
#ifndef __ACTIVITY_H
#define __ACTIVITY_H

#define MAX_ACTIVITY_COUNT					50

#define ACTIVITY_NORMAL						1	// 
#define ACTIVITY_AD1						30	// 广告类
#define ACTIVITY_AD2						31	// 广告类
#define ACTIVITY_AD3						32	// 广告类
#define ACTIVITY_AD4						33	// 广告类
#define ACTIVITY_AD5						34	// 广告类
#define ACTIVITY_PAYBAG1					40	// 活动礼包
#define ACTIVITY_PAYBAG2					41	// 活动礼包
#define ACTIVITY_PAYBAG3					42	// 活动礼包
#define ACTIVITY_PAYBAG4					43	// 活动礼包
#define ACTIVITY_PAYBAG5					44	// 活动礼包

typedef struct _activity_item
{
	int m_activityid;
	int m_starttime;	// 活动启动时间
	int m_endtime;		// 活动结束时间
	int m_closetime;	// 活动关闭时间，好多活动为了玩家看积分排名什么的，会有最后隐藏时间
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
#endif
