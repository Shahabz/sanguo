#ifndef _MAIL_H_ 
#define _MAIL_H_
#include "define.h"

#define MAX_MAIL_SAVETIME			86400*7		// 邮件保存时间7天
#define MAIL_TITLE_MAXSIZE			128			// 邮件标题长度
#define MAIL_CONTENT_MAXSIZE		1024		// 邮件信息长度
#define MAIL_ATTACH_MAXSIZE			256			// 邮件附件长度
#define MAIL_ACTORID				-2			//
#define MAIL_SEARCHCACHE_MAX		128			// 邮件分享缓存

// 邮件类型
#define MAIL_TYPE_SYSTEM			0	// 系统信息邮件
#define MAIL_TYPE_NOTIFY			1	// 公告邮件，内容外部http服务器获取
#define MAIL_TYPE_ACTOR_SEND		2	// 玩家发送邮件
#define MAIL_TYPE_ACTOR_REPLY		3	// 玩家回复邮件
#define MAIL_TYPE_FIGHT_ENEMY		10	// 流寇
#define MAIL_TYPE_FIGHT_CITY		11	// 城战
#define MAIL_TYPE_FIGHT_NATION		12	// 国战
#define MAIL_TYPE_CITY_SPY			13	// 侦察
#define MAIL_TYPE_CITY_BESPY		14	// 被侦察
#define MAIL_TYPE_GATHER			20	// 采集
#define MAIL_TYPE_GATHER_FIGHT		21	// 采集战斗

#define TAG_TEXTID			"#$"// 标记--标记为文字表id
#define TAG_ITEMKIND		"$$"// 标记--标记为道具专用
#define TAG_EQUIPKIND		"$%"// 标记--标记为装备专用
#define TAG_ZONEID			"$*"// 标记--标记为地区专用
#define TAG_TOWNID			"$&"// 标记--标记为城镇专用
#define TAG_TIMEDAY			"$D"// 标记--标记为时间天
#define TAG_TIMEHOUR		"$H"// 标记--标记为时间小时
#define TAG_TIMEMIN			"$M"// 标记--标记为时间分钟
#define TAG_TIMESEC			"$S"// 标记--标记为时间秒
#define TAG_DATE			"$T"// 标记--标记为时间年月日：时分秒
#define TAG_POS				"$#"// 标记--标记为坐标
#define TAG_NATION			"$N"// 标记--标记为国家
#define TAG_AWARD			"$A"// 标记--标记为奖励
#define TAG_KINGWAR			"$K"// 标记--标记为皇城战据点
#define TAG_HERO			"$R"// 标记--标记为武将
#define TAG_COLOR			"$C"// 标记--标记为颜色
#define TAG_OFFICIAL		"$O"// 标记--标记为官职

typedef struct _mailfight_searchcache
{
	i64 m_mailid;
	char m_content[FIGHT_JSON_MAX];
}MailFightSearchCache;

// 发送邮件
i64 mail( int actor_index, int actorid, char type, char *title, char *content, char *attach, i64 fightid, char queue );
i64 mail_system( int actor_index, int actorid, int titleid, int contentid, char *attach, char queue );
i64 mail_system( int actor_index, int actorid, int titleid, int contentid, AwardGetInfo *getinfo, char queue );
i64 mail_system( int actor_index, int actorid, int titleid, int contentid, int awardgroup, char queue );
i64 mail_system( int actor_index, int actorid, int titleid, int contentid, char *v1, char *v2, char *v3, char *attach, char queue );
// 插入完mail的回调
int mailqueue_insert_complete();

// 战斗详情邮件
int mail_fight( i64 mailid, int actorid, char *json );

// 拼标题
i64 mail_maketitle( int titleid, char *v1, char *v2, char *v3, char *v4 );

// 拼内容
i64 mail_makecontent( int contentid, char *v1, char *v2, char *v3, char *v4 );

// 读取战斗详细内容
int mail_fight_read( int actor_index, i64 mailid );

// 发送战斗详细内容
int mail_fight_send( int actor_index, i64 mailid, char *content );

// 设置已读状态
int mail_readed( int actor_index, i64 mailid );

// 设置全部已读状态
int mail_readed_all( int actor_index );

// 删除邮件
int mail_delete( i64 mailid );

// 删除邮件
int mail_delete_actor( int actor_index, i64 mailid );

// 批量删除邮件
int mail_delete_all( int actor_index, SLK_NetC_MailAllDel *pValue );

// 设置锁定状态
int mail_locked( int actor_index, i64 mailid );

// 设置解除锁定状态
int mail_unlocked( int actor_index, i64 mailid );

// 获取未读邮件数量
int mail_noread_check( int actor_index );

// 发送未读邮件数量
int mail_noread_send( int actor_index );

// 获取邮件列表
int mail_getlist( int actor_index, int op, i64 min, i64 max );

// 领取附件
int mail_attachget( int actor_index, i64 mailid );

// 分享邮件
int mail_share( int actor_index, SLK_NetS_MailShare *pValue );

// 玩家点击分享的邮件获取内容
int mail_share_getcontent( int actor_index, i64 mailid );

// 玩家点击分享的邮件获取内容
int mail_share_getfightcontent( int actor_index, i64 mailid );

// 玩家间邮件发送
int mail_actor_send( int actor_index, SLK_NetC_MailSend *pValue );

// 玩家间邮件回复
int mail_actor_reply( int actor_index, SLK_NetC_MailReply *pValue );

// 给所有城池邮件
void mail_sendall( int titleid, int contentid, char *v1, char *v2, char *v3, char *attach );
#endif
