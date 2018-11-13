#ifndef __LOGIN_H
#define __LOGIN_H
#include "define.h"
#include "server_structsend_auto.h"

#define MAX_LOGINQUEUENUM			256 // 登陆队列长度

#define COMMAND_PACK_LEN			1024
#define RETURN_PACK_LEN				256

#define USERCMDS_LOGIN				1
#define USERCMDS_CDKEY				2
#define USERCMDS_INVITECODE			3

#define USERCMDC_LOGIN				1
#define USERCMDC_CDKEY				2
#define USERCMDC_INVITECODE			3

// 登陆队列
struct _sloginqueue
{
	int client_index;
	int authid;
	short command;
	char path[64];
	char data[COMMAND_PACK_LEN];
};
typedef struct _sloginqueue SLoginQueue;

int login_init();
int login_send_proc();
int login_recv_proc();

// 用户发送登陆请求到登陆队列
int user_login( int client_index, const char *szUserName, const char *szPassWord, char *szDeviceID );
// 用户服务器返回登陆结果
int user_logined( int client_index, int authid, SClientInfo *pClientInfo );

// 发送兑换奖励信息
int user_cdkey( int client_index, char *cardnumber );
// 用户服务器返回奖励信息
int user_cdkeyed( int client_index, int authid, int cdkey_index, int awardgroup, int result, char *cardnumber );

// 发送邀请码信息
int user_invitecode( int client_index, int path, char *msg );
// 用户服务器返回邀请码信息
int user_invitecodeed( int client_index, int authid, int result, int path, int isaward );

// 排队机制
int user_queue_logic();
void login_queue_clear();
#endif



