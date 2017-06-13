#ifndef __NETSERVER_H
#define __NETSERVER_H

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <windows.h>
#include <Ws2tcpip.h>
#endif
#include "utils.h"
#include "thread.h"
#ifdef _USEGATE
#include "netserver_gs.h"
#endif

#define MAX_SOCKET_NUM		512			// 最大连接数
//#define MAX_SOCKET_NUM		1024*4			// 最大连接数

#define MAX_BUF_SIZE		1024*4			// 最大缓冲区长度
#define MAX_PACKAGE_SIZE	1024*2			// 最大网络包长度

#define MAX_SENDLIST_COUNT	1200				// 一个用户的发生链表的最大长度,超过就T

#define MAX_FD				8192

#define LISTEN_QUEUE		20				// 监听队列长度
#define SERV_PORT 11521						// 监听端口

#ifdef WIN32
#define MAX_WORKER_THREAD	16				// 工作线程个数
#define IOREAD				1				// 读操作
#define IOWRITE				2				// 写操作
#endif

#define FORBID_IP_MAX	(100)
#define FORBID_IP_HITTIME	(60)
#define FORBID_ALL_TIME		(60)

#define MAX_DEVDATA_SIZE	256
#define DEVDATA_COUNT		16
// 内存池,每一项表示一个网络包
struct _sbuf_pool
{
	int buf_size;				// 网络包长度
	char buf[MAX_PACKAGE_SIZE];	// 网络包数据
	int client_index;			// 客户端索引(用于查找bug啦)
	int next;
};
typedef struct _sbuf_pool SBufPool;

// 读到的包队列
struct _sreadqueue
{
	int client_index;			// 客户端索引
	int buf_size;				// 网络包长度
	char buf[MAX_PACKAGE_SIZE];	// 网络包数据
	int exec_code;

	// 跨服专用
	char type;
	short server_code;
	short cmd;
	int value[2];
};
typedef struct _sreadqueue SReadQueue;

// 描述一个客户的缓冲区
typedef struct _socket_buf {
#ifdef WIN32
	WSAOVERLAPPED	m_Overlapped;
	WSABUF			m_wsabuf;
	BYTE			m_nOption;		// IO操作方式
#else
	char			*m_ptr;
#endif
	char			*m_pBuffer;		//8K
	int				m_nTotalBytes;	// 总字节数
	int				m_nProcBytes;	// 处理的字节数
} SOCKET_BUF;

// 描述一个客户端,包含一个读和一个写的缓冲区
typedef struct _socket_set {
	SOCKET			m_fd;
	char			m_stat;			// 客户端的状态, 空,等待,无点数,已登入,已进入游戏
	char			m_iswait;		// 客户端是否处于等待状态（用于登入状态判断，等待状态不接受任何客户消息）
	char			m_ip[16];		// IP地址
	char			m_idle_time;	// 空闲的时间(10秒为一个单位,如果达到6个单位1分钟,将踢出这个客户)
	int				m_enter_time;	// 进入游戏的时间
	i64				m_userid;		// --数据:用户id
	char			m_username[MAX_USERNAME_LENGTH+1];	// --数据:用户名
	short			m_usertype;		// --数据:用户类型

	char			m_deviceid[65];	//	--数据:设备ID
	short			m_platid;		// --数据:客户端平台编号
	char			m_language;		// --数据:客户端使用的语言
	short			m_invcode;		// --数据:客户端内部标识
	int				m_os;		// --数据:操作系统(1:ios 2:android)
	short			m_channelid;	// --数据:渠道,区别平台，因为云起会接各种渠道
	int				m_ver[3];		//
	char			m_devdata[MAX_DEVDATA_SIZE];		// --数据:设备信息
	char			*m_dev[DEVDATA_COUNT];		// 设备信息分割
	char			m_isnew;		// 新帐号标记
	short			m_country;		// 客户端选择的国家
	char			m_ipcountry[4];	// IP地址所在国家
	int				m_authid;		// 认证客户与连接的正确性
	SOCKET_BUF		m_recv_buf;		// 接收缓冲区
	SOCKET_BUF		m_send_buf;		// 发送缓冲区
	int				m_send_head;	// 发送头
	int				m_send_tail;	// 发送尾
	short			m_recv_count;	// 接收包个数(1分钟内)
	short			m_send_count;	// 发生链表的大小
	unsigned int	m_recv_key;		// 数据接收的密字
	unsigned int	m_send_key;		// 数据发送的密字
	char			m_pass_key;		// 密码加密的xor
	int				m_gpk_key;		// GPK的索引密字

	ThrMux			*m_mmux;		// 互斥体
	int				m_exec_code;

} SOCKET_SET;


//typedef int (*ADD_FORBID_IP)( _net_protect *, int );
typedef struct  _net_protect
{
	int forbid_ip_list[FORBID_IP_MAX];
	time_t forbid_time[FORBID_IP_MAX];
	time_t all_forbid_time;
	ThrMux	*iplist_mutex;
}NetProtect;

#ifdef _USEGATE
void netserver_setgatemode( char usegate );
#endif

int fill_send_buf( int client_index );
int net_recv_clientver( int client_index, char *buf, int truesize );
int net_send_clientver( int client_index, int err_code );
int net_send_onepackeg( int client_index, char *pack_data, short pack_size );
int net_send_long_packeg( int client_index, char *long_data, int long_size );

int add_socketindex( SOCKET fd );
int unset_socket( int client_index, char is_remove );
void close_client( int client_index );
void net_closesock_direct( int client_index );
int server_getautuid( int client_index );

int get_sbuf_count();
int check_sbuf_count( int type, int *pcount_inv, int *pcount_exit );

int ReloadGPK( int code );

int add_forbid_ip( NetProtect *np, char *ip );
int check_forbid_ip( NetProtect *np, int ip );

void server_exit();

#endif
