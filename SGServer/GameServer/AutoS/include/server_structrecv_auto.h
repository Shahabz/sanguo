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
	char m_aclass;	//客户端请求-创建的角色种类
	short m_name_length;	//客户端请求-创建的角色名称长度
	char m_name[22];	//客户端请求-创建的角色名称
};
typedef struct _slk_NetC_Create SLK_NetC_Create;	//客户端请求创建角色

struct _slk_NetC_Heart {
	int m_value[2];	//心跳-数值
};
typedef struct _slk_NetC_Heart SLK_NetC_Heart;	//心跳

struct _slk_NetC_Gmcmd {
	short m_cmd;	//客户端发来-GM指令
	int m_value[4];	//客户端发来-GM指令数值参数
	short m_msg_length;	//客户端发来-GM指令字符串长度
	char m_msg[256];	//客户端发来-GM指令字符串参数
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

int struct_NetC_Login_recv( char **pptr, int *psize, SLK_NetC_Login *pValue );
int struct_NetC_Create_recv( char **pptr, int *psize, SLK_NetC_Create *pValue );
int struct_NetC_Heart_recv( char **pptr, int *psize, SLK_NetC_Heart *pValue );
int struct_NetC_Gmcmd_recv( char **pptr, int *psize, SLK_NetC_Gmcmd *pValue );
int struct_NetC_AskInfo_recv( char **pptr, int *psize, SLK_NetC_AskInfo *pValue );

#endif
