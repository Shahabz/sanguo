#ifndef _MAIL_H_ 
#define _MAIL_H_
#include "define.h"

#define MAX_MAIL_SAVETIME			86400*7		// 邮件保存时间7天
#define MAIL_TITLE_MAXSIZE			64			// 邮件标题长度
#define MAIL_CONTENT_MAXSIZE		1024		// 邮件信息长度
#define MAIL_ATTACH_MAXSIZE			256			// 邮件附件长度
#define MAIL_ACTORID				-2			//

// 邮件类型
#define MAIL_TYPE_SYSTEM		0	// 系统信息邮件
#define MAIL_TYPE_NOTIFY		1	// 公告邮件，内容外部http服务器获取
#define MAIL_TYPE_FIGHT_ENEMY	10	// 流寇
#define MAIL_TYPE_FIGHT_CITY	11	// 城战
#define MAIL_TYPE_FIGHT_NATION	12	// 国战

#define TAG_TEXTID			"#$"// 标记--标记为文字表id
#define TAG_ITEMKIND		"$$"// 标记--标记为道具装备专用
#define TAG_POS				"$#"// 标记--标记为坐标

// {\"textid\":\"10001\", \"vlist\":[]}
// 

// 发送邮件
i64 mail( int actor_index, int actorid, char type, char *title, char *content, char *attach, i64 fightid );

// 拼标题
i64 mail_maktitle( int titleid, char *v1, char *v2, char *v3, char *v4 );

// 拼内容
i64 mail_makcontent( int contentid, char *v1, char *v2, char *v3, char *v4 );

// 读取详细内容
int mail_read_content( int actor_index, i64 mailid );

// 设置已读状态
int mail_readed( int actor_index, i64 mailid );

// 删除邮件
int mail_delete( i64 mailid );

// 未读邮件数量
int mail_noread_check( int actor_index );

// 获取邮件列表
int mail_getlist( int actor_index, int op, i64 min, i64 max );

// 领取附件
int mail_attachget( int actor_index, i64 mailid );
#endif
