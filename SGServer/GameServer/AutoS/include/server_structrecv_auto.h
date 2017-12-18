#ifndef __STRUCTRECV0_AUTO_H
#define __STRUCTRECV0_AUTO_H
#include "server_struct_auto.h"

struct _slk_NetC_Login {
	short m_username_length;	//客户端请求-用户名长度
	char m_username[51];	//客户端请求-用户名
	short m_password_length;	//客户端请求-密码长度
	char m_password[256];	//客户端请求-密码
	short m_deviceid_length;	//客户端请求-设备标示长度
	char m_deviceid[65];	//客户端请求-设备标示
	short m_isnew;	//客户端请求-是否新用户
	short m_ipcountry_length;	//客户端请求-IP地址所在国家
	char m_ipcountry[16];	//客户端请求-IP地址所在国家
};
typedef struct _slk_NetC_Login SLK_NetC_Login;	//客户端请求登录

struct _slk_NetC_Create {
	char m_nation;	//客户端请求-创建的角色国家
	short m_name_length;	//客户端请求-创建的角色名称长度
	char m_name[22];	//客户端请求-创建的角色名称
};
typedef struct _slk_NetC_Create SLK_NetC_Create;	//客户端请求创建角色

struct _slk_NetC_Heart {
	int m_value;	//心跳-数值
};
typedef struct _slk_NetC_Heart SLK_NetC_Heart;	//心跳

struct _slk_NetC_Gmcmd {
	short m_cmd;	//客户端发来-GM指令
	int m_value[4];	//客户端发来-GM指令数值参数
	short m_msg_length;	//客户端发来-GM指令字符串长度
	char m_msg[1024];	//客户端发来-GM指令字符串参数
};
typedef struct _slk_NetC_Gmcmd SLK_NetC_Gmcmd;	//GM指令

struct _slk_NetC_AskInfo {
	short m_msgid;	//客户端发送-短请求ID
	short m_valuenum;	//客户端发送-短请求参数个数
	int m_value[10];	//客户端发送-短请求参数
	short m_msg_length;	//客户端发送-短请求字符串长度
	char m_msg[256];	//客户端发送-短请求字符串
};
typedef struct _slk_NetC_AskInfo SLK_NetC_AskInfo;	//客户端发送短请求

struct _slk_NetC_EnterCity {
	char m_value;	//客户端发送-我已初始化好，发过来玩家信息吧
};
typedef struct _slk_NetC_EnterCity SLK_NetC_EnterCity;	//请求进入城池所有信息

struct _slk_NetC_Chat {
	short m_msglen;	//客户端发送聊天-信息
	char m_msg[127];	//客户端发送聊天-信息
	char m_channel;	//客户端发送聊天-选择渠道
};
typedef struct _slk_NetC_Chat SLK_NetC_Chat;	//聊天

struct _slk_NetC_StoryBattle {
	int m_storyid;	//副本id
	short m_herokind[4];	//副本出战的英雄
};
typedef struct _slk_NetC_StoryBattle SLK_NetC_StoryBattle;	//副本战斗

struct _slk_NetC_WorldMapAsk {
	short m_to_posx;	//请求前往世界地图
	short m_to_posy;	//请求前往世界地图
};
typedef struct _slk_NetC_WorldMapAsk SLK_NetC_WorldMapAsk;	//请求前往世界地图

struct _slk_NetC_WorldMapAreaIndex {
	int m_areaindex;	//逻辑区域索引
	short m_posx;	//逻辑区域坐标
	short m_posy;	//逻辑区域坐标
};
typedef struct _slk_NetC_WorldMapAreaIndex SLK_NetC_WorldMapAreaIndex;	//世界地图逻辑区域索引

struct _slk_NetC_MapBattle {
	char m_to_unit_type;	//出征目标
	int m_to_unit_index;	//出征目标
	int m_id;	//出征目标
	short m_herokind[4];	//出征英雄
	short m_to_posx;	//出征目的点
	short m_to_posy;	//出征目的点
	int m_appdata;	//出征附加数据
	char m_action;	//出征行为
	int m_group_index;	//出征集结索引
};
typedef struct _slk_NetC_MapBattle SLK_NetC_MapBattle;	//出征

struct _slk_NetC_MailAsk {
	i64 m_minid;	//邮件-缓存最小的id
	i64 m_maxid;	//邮件-缓存最大的id
	short m_op;	//邮件-操作
};
typedef struct _slk_NetC_MailAsk SLK_NetC_MailAsk;	//请求更多邮件

struct _slk_NetC_MailOp {
	char m_op;	//邮件操作
	i64 m_mailid;	//邮件操作
};
typedef struct _slk_NetC_MailOp SLK_NetC_MailOp;	//邮件操作

struct _slk_NetC_MailAllDel {
	short m_count;	//批量删除邮件
	i64 m_mailid[128];	//批量删除邮件
};
typedef struct _slk_NetC_MailAllDel SLK_NetC_MailAllDel;	//邮件批量删除

struct _slk_NetS_MailShare {
	i64 m_mailid;	//邮件分享
	short m_a_name_len;	//邮件分享
	char m_a_name[32];	//邮件分享
	short m_d_name_len;	//邮件分享
	char m_d_name[32];	//邮件分享
	char m_type;	//邮件分享
};
typedef struct _slk_NetS_MailShare SLK_NetS_MailShare;	//邮件分享

struct _slk_NetC_MailSend {
	int m_unit_index;	//邮件发送
	int m_actorid;	//邮件发送
	short m_content_length;	//邮件发送
	char m_content[512];	//邮件发送
};
typedef struct _slk_NetC_MailSend SLK_NetC_MailSend;	//邮件发送

struct _slk_NetC_MailReply {
	int m_actorid;	//邮件回复
	short m_content_length;	//邮件回复
	char m_content[512];	//邮件回复
	short m_reply_length;	//邮件回复
	char m_reply[512];	//邮件回复
	int m_reply_recvtime;	//邮件回复
};
typedef struct _slk_NetC_MailReply SLK_NetC_MailReply;	//邮件回复

struct _slk_NetC_WorldBossBattle {
	int m_bossid;	//世界bossid
	short m_herokind[4];	//世界boss出战的英雄
};
typedef struct _slk_NetC_WorldBossBattle SLK_NetC_WorldBossBattle;	//世界boss战斗

struct _slk_NetC_HeroGuardSort {
	short m_herokind[4];	//御林卫武将更改顺序
};
typedef struct _slk_NetC_HeroGuardSort SLK_NetC_HeroGuardSort;	//御林卫守卫顺序

struct _slk_NetC_QuestTalkNext {
	int m_talkid;	//任务对话点击返回
	char m_type;	//任务对话点击类型
};
typedef struct _slk_NetC_QuestTalkNext SLK_NetC_QuestTalkNext;	//任务对话点击下一个

int struct_NetC_Login_recv( char **pptr, int *psize, SLK_NetC_Login *pValue );
int struct_NetC_Create_recv( char **pptr, int *psize, SLK_NetC_Create *pValue );
int struct_NetC_Heart_recv( char **pptr, int *psize, SLK_NetC_Heart *pValue );
int struct_NetC_Gmcmd_recv( char **pptr, int *psize, SLK_NetC_Gmcmd *pValue );
int struct_NetC_AskInfo_recv( char **pptr, int *psize, SLK_NetC_AskInfo *pValue );
int struct_NetC_EnterCity_recv( char **pptr, int *psize, SLK_NetC_EnterCity *pValue );
int struct_NetC_Chat_recv( char **pptr, int *psize, SLK_NetC_Chat *pValue );
int struct_NetC_StoryBattle_recv( char **pptr, int *psize, SLK_NetC_StoryBattle *pValue );
int struct_NetC_WorldMapAsk_recv( char **pptr, int *psize, SLK_NetC_WorldMapAsk *pValue );
int struct_NetC_WorldMapAreaIndex_recv( char **pptr, int *psize, SLK_NetC_WorldMapAreaIndex *pValue );
int struct_NetC_MapBattle_recv( char **pptr, int *psize, SLK_NetC_MapBattle *pValue );
int struct_NetC_MailAsk_recv( char **pptr, int *psize, SLK_NetC_MailAsk *pValue );
int struct_NetC_MailOp_recv( char **pptr, int *psize, SLK_NetC_MailOp *pValue );
int struct_NetC_MailAllDel_recv( char **pptr, int *psize, SLK_NetC_MailAllDel *pValue );
int struct_NetS_MailShare_recv( char **pptr, int *psize, SLK_NetS_MailShare *pValue );
int struct_NetC_MailSend_recv( char **pptr, int *psize, SLK_NetC_MailSend *pValue );
int struct_NetC_MailReply_recv( char **pptr, int *psize, SLK_NetC_MailReply *pValue );
int struct_NetC_WorldBossBattle_recv( char **pptr, int *psize, SLK_NetC_WorldBossBattle *pValue );
int struct_NetC_HeroGuardSort_recv( char **pptr, int *psize, SLK_NetC_HeroGuardSort *pValue );
int struct_NetC_QuestTalkNext_recv( char **pptr, int *psize, SLK_NetC_QuestTalkNext *pValue );

#endif
