
#ifndef __CLIENT_H
#define __CLIENT_H

#ifdef WIN32
#include <winsock2.h>
#include <mswsock.h>
#endif
#include "netserver.h"

#define CLIENT_STAT_VER_ERR		-2 // 客户端版本错误 [9/4/2009 yxx]
#define CLIENT_STAT_NULL		-1		// 无意义
#define CLIENT_STAT_WAIT		0		// 等待客户端发送版本信息
#define CLIENT_STAT_CHECK		1		// 客户端获得了验证
#define CLIENT_STAT_NOFEE		2		// 客户端没有费用
#define CLIENT_STAT_LOGIN		3		// 客户端已经登入
#define CLIENT_STAT_ENTERGAME	4		// 客户端进入了游戏

struct _clientinfo
{
	int result;
	char ekey_type;
	char challengeNum[16];
	i64 userid;	// 用户id
	char username[MAX_USERNAME_LENGTH+1];	// 用户名
	short usertype;		// 用户类型
	short isgolden;	// 是否黄金账户
	char isdeep;	// 是否防沉迷
	char cardstat[4];	// 新手卡状态
	int point_totle;	// 总充值量
};
typedef struct _clientinfo SClientInfo;

void client_setstat( int client_index, char stat );
char client_getstat( int client_index );
void client_setwait( int client_index, char iswait );
char client_getwait( int client_index );

char *client_getip( int client_index );
void client_setuserinfo( int client_index, SClientInfo *pClientInfo );
i64 client_getuserinfo( int client_index, SClientInfo *pClientInfo );
i64 client_getuserid( int client_index );
short client_getusertype( int client_index );
char *client_getusername( int client_index );
char client_isexited( int client_index );	// 退出标志
int client_logic();
char client_getpasskey( int client_index );
char client_setsyspwd( int client_index, char *syspwd ); // 设置这个socket的验证码
char client_getsyspwd( int client_index, char *outpwd ); // 获取这个socket的验证码
short client_addsyspwd_error( int client_index ); // 设置这个socket的验证码输入错误的次数
short client_getsyspwd_error( int client_index ); // 获取这个socket的验证码输入错误的次数
short client_clearsyspwd_error( int client_index ); // 清空这个socket的验证码输入错误的次数
int client_get_maxrecvcount();

int client_get_exec_code( int client_index );


char client_is_same_mac( int client_index, short nMac, char *pMac );
char client_is_same_cpu( int client_index, short nCpu, char *pCpu );
short client_get_mac( int client_index, char *pMac );
short client_get_cpu( int client_index, char *pCpu );
void client_setmaccpu_info( int client_index, short nMac, char *pMac, short nCpu, char *pCpu );

char cleint_ipcomp( int client_index, char *ip );

#endif
