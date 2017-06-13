// 网络引擎 - 如果定义WIN32, 支持Windows系统, 否则支持linux系统

//--------------------------------------------------------------------------------------
// 头文件包含
//--------------------------------------------------------------------------------------
#ifdef WIN32
#include <assert.h>
#else

#include <strings.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
//#define NONECODE
#ifndef NONECODE
#include "netcode.h"
#endif
#include "netfunc.h"
#include "netserver.h"
#include "global_net.h"

#ifdef EXTERN_C
#undef EXTERN_C
#endif

#ifndef _DEBUG
#ifndef _USEGPK
//#define _USEGPK
#endif
#endif

#ifdef _USEGPK
#include "GPKitSvr.h"
#endif

//--------------------------------------------------------------------------------------
// 全局变量
//--------------------------------------------------------------------------------------
jmp_buf g_mark;
//FILE *g_fp = NULL;

#ifdef WIN32
DWORD g_dwThreadCount = 0;					// 线程个数
HANDLE g_hIOCP = INVALID_HANDLE_VALUE;		// 完成端口句柄
// 工作线程
Mthr	*g_pthr_worker[MAX_WORKER_THREAD];
#else
int *g_fd_map;

// epoll句柄
struct epoll_event *g_events;
int g_maxfd;
int g_epfd = -1;

#endif

BOOL g_bGateMode = FALSE;

int g_net_time = 0;
extern int g_max_connection;
short g_max_send_count = 0;

char g_bNoHeader = 0;
int g_extcode = 0;
extern int g_last_cmd;

SOCKET listenfd = INVALID_SOCKET;			// 监听socket

// 线程, 发送线程,接收线程
Mthr	*g_pthr_recv;

// 本地IP地址,监听端口,当前客户版本支持
unsigned int g_ipaddress;
short g_port;

NetProtect g_net_protect;

union
{
	int value; 
	struct
	{
		unsigned char m1;
		unsigned char m2;
		unsigned short m3;
	}ss;
}g_client_ver;

//unsigned int g_client_ver = 0;

// 客户端集合
SOCKET_SET *g_sockset;

// 接收队列
SReadQueue *read_queue;
int g_nReadQueueHead;
int g_nReadQueueTail;

// 接收的互斥体变量
ThrMux	*g_read_mmux;

// 内存池
SBufPool *g_pBuf;
// 访问内存池的互斥体变量
ThrMux	*g_sbuf_mmux;

#ifndef WIN32
char g_bIsDown = 0;
#endif

char g_bServerIsInit = 0;
char g_bExit = 0;			// 是否需要退出系统
int g_curauthid;			// 认证ID

// 外部系统需要的统计数据
extern int g_connectioncount;

int g_log_recvcount = 0;
int g_log_sendcount = 0;
int g_sbuf_count = 0;
int g_exec_code = 0;

// 回调函数, 外部调用处理接收包以及断开连接
LPCALLBACK_PROCESSPACK g_pCallbackProcessPack = NULL;
LPCALLBACK_DISCONNECTED g_pCallbackDisConnected = NULL;
LPCALLBACK_ONERROR g_pCallbackOnError = NULL;
LPCALLBACK_ONDESTORY g_pCallbackOnDestory = NULL;
extern LPCALLBACK_GLOBALPACK g_pCallbackGlobalPack;

#ifdef _USEGATE
void netserver_setgatemode( char usegate )
{
	TRACE_LOG( );
	if( usegate )
		g_bGateMode = TRUE;
	else
		g_bGateMode = FALSE;
}
#endif

const char *GetRootPath()
{
	static char szPath[MAX_PATH];
	static char bFirstTime = 1;

	int slen;
	int tmpi;
	if( bFirstTime )
	{
		bFirstTime = 0;
		GetModuleFileName( NULL, szPath, sizeof(szPath) );
		slen = (int)strlen( szPath );
		for( tmpi = slen-1; tmpi > 0; tmpi-- )
		{
			if( szPath[tmpi] == '\\' || szPath[tmpi] == '/' )
			{
				szPath[tmpi] = 0;
				break;
			}
			else if( szPath[tmpi] == ':' )
			{
				szPath[tmpi+1] = 0;
				break;
			}
		}
	}
	return szPath;
}

//////////////////////////////////////////////////////////////////
// GPK
//////////////////////////////////////////////////////////////////
#ifdef _USEGPK

//#ifdef _DEBUG
//#pragma comment(lib, "GPKitSvrD.lib")
//#else
//#pragma comment(lib, "GPKitSvr.lib")
//#endif

#ifdef WIN32
#ifdef _DEBUG
#define GPK_DYNAMIC_LIB "GPKitSvrD.dll"
#else
#define GPK_DYNAMIC_LIB "GPKitSvr.dll"
#endif
#else
#define GPK_DYNAMIC_LIB "libGPKitSvr.so"
#endif

static struct {
	void *handle;
	PFN_GPKCreateSvrDynCode_C GPKCreateSvrDynCode_C;
	PFN_GPKLoadBinary GPKLoadBinary;
	PFN_GPKGetRandIdx GPKGetRandIdx;
	PFN_GPKGetCltDynCode GPKGetCltDynCode;
	PFN_GPKEncode GPKEncode;
	PFN_GPKDecode GPKDecode;
	PFN_GPKRelease GPKRelease;
} lib;
static int GPKlib_isloaded = 0;

LPGPKSvrDynCode g_pSvrDynCode = NULL;

int InitGPKServer()
{
	char szSvrDir[MAX_PATH];
	char szCltDir[MAX_PATH];
	int nBinCount;

	if ( GPKlib_isloaded != 0 )
		return 0;
	lib.handle = load_dll( GPK_DYNAMIC_LIB );
	if ( lib.handle == NULL )
	{
		printf_msg( "GPK lib not found[%s]", GPK_DYNAMIC_LIB );
		return -1;
	}
	lib.GPKCreateSvrDynCode_C = ( PFN_GPKCreateSvrDynCode_C )load_function( lib.handle, "GPKCreateSvrDynCode_C" );
	if ( lib.GPKCreateSvrDynCode_C == NULL )
	{
		unload_dll( lib.handle );
		return -1;
	}

	lib.GPKLoadBinary = ( PFN_GPKLoadBinary )load_function( lib.handle, "GPKLoadBinary" );
	if ( lib.GPKLoadBinary == NULL )
	{
		unload_dll( lib.handle );
		return -1;
	}
	lib.GPKGetRandIdx = ( PFN_GPKGetRandIdx )load_function( lib.handle, "GPKGetRandIdx" );
	if ( lib.GPKGetRandIdx == NULL )
	{
		unload_dll( lib.handle );
		return -1;
	}
	lib.GPKGetCltDynCode = ( PFN_GPKGetCltDynCode )load_function( lib.handle, "GPKGetCltDynCode" );
	if ( lib.GPKGetCltDynCode == NULL )
	{
		unload_dll( lib.handle );
		return -1;
	}
	lib.GPKEncode = ( PFN_GPKEncode )load_function( lib.handle, "GPKEncode" );
	if ( lib.GPKEncode == NULL )
	{
		unload_dll( lib.handle );
		return -1;
	}
	lib.GPKDecode = ( PFN_GPKDecode )load_function( lib.handle, "GPKDecode" );
	if ( lib.GPKDecode == NULL )
	{
		unload_dll( lib.handle );
		return -1;
	}
	lib.GPKRelease = ( PFN_GPKRelease )load_function( lib.handle, "GPKRelease" );
	if ( lib.GPKRelease == NULL )
	{
		unload_dll( lib.handle );
		return -1;
	}

	GPKlib_isloaded = 1;


	g_pSvrDynCode = lib.GPKCreateSvrDynCode_C();

	if(NULL == g_pSvrDynCode)
	{
		printf_msg("Create SvrDynCode component failed\n");
		return -1;
	}
#ifdef BIT64
	sprintf(szSvrDir, "%s/DynCodeBin/Server64", GetRootPath());
#else
	sprintf(szSvrDir, "%s/DynCodeBin/Server0", GetRootPath());
#endif
	sprintf(szCltDir, "%s/DynCodeBin/Client0", GetRootPath());

	nBinCount = lib.GPKLoadBinary(g_pSvrDynCode, szSvrDir, szCltDir);
	printf_msg("Load Binary: %d binary are loaded\n", nBinCount);

	if( nBinCount == 0 )
	{
		printf_msg("Load DynCode failed.\n");
		return -1;
	}

	return 0;
} 

int ReloadGPK( int code )
{
	char szSvrDir[MAX_PATH];
	char szCltDir[MAX_PATH];
	int nBinCount;

	sprintf(szSvrDir, "%s/DynCodeBin/Server%d", GetRootPath(),code);
	sprintf(szCltDir, "%s/DynCodeBin/Client%d", GetRootPath(),code);

	nBinCount = lib.GPKLoadBinary(g_pSvrDynCode, szSvrDir, szCltDir);
	if( nBinCount == 0 )
	{
		return -1;
	}
	return 0;
}
#else
int ReloadGPK( int code )
{
	return 0;
}
#endif
//////////////////////////////////////////////////////////////////////

const char *Byte2Hex(unsigned char byte )
{
	static const char HexTable[] = "0123456789ABCDEF";
    static char str[] = "00";
    int tmpi;
    for( tmpi = 1; tmpi >= 0; tmpi-- )
    {
        str[tmpi] = HexTable[byte & 0x0F];
        byte >>= 4;
    }
    return str;
}

#ifndef WIN32
//--------------------------------------------------------------------------------------
// linux下设置socket为非阻塞模式
//--------------------------------------------------------------------------------------
int setnonblocking( SOCKET fd )
{
	int opts;
	opts = fcntl( fd, F_GETFL );
	if( opts < 0 )
	{
		write_netlog("fcntl( fd, GETFL )");
		return -1;
	}
	opts = opts | O_NONBLOCK;
	if( fcntl( fd, F_SETFL, opts ) < 0 )
	{
		write_netlog("fcntl( fd, SETFL, opts )");
		return -1;
	}
	return 0;
}
#endif
//--------------------------------------------------------------------------------------
// 获得该客户的认证ID
//--------------------------------------------------------------------------------------
// #GATECHECK
int server_getautuid( int client_index )
{
	TRACE_LOG( );
#ifdef _USEGATE
	if( g_bGateMode )
		return gs_getautuid( client_index );
#endif
	if( client_index < 0 || client_index >= g_max_connection )
		return -1;
	if( (int)g_sockset[client_index].m_fd < 0 )
		return -1;
	return g_sockset[client_index].m_authid;
}

//--------------------------------------------------------------------------------------
// 新增一个客户端(针对g_sockset的操作),主线程调用
//--------------------------------------------------------------------------------------
int add_socketindex( SOCKET fd )
{
	TRACE_LOG( );
	// 这里针对g_sockset的操作, 只允许一个线程使用, 否则同时进入这个函数就乱了
	int client_index;
	for( client_index = 0; client_index < g_max_connection; client_index++ )
	{
		if( g_sockset[client_index].m_fd == INVALID_SOCKET )
		{
			g_sockset[client_index].m_fd = fd;
			g_sockset[client_index].m_stat = -1;
			g_sockset[client_index].m_iswait = 0;
			memset( g_sockset[client_index].m_dev, 0, sizeof(char *)*DEVDATA_COUNT );
			
			g_sockset[client_index].m_authid = g_curauthid++;
			return client_index;
		}
	}
	return -1;
}

void free_sbuf( int buf_index )
{
	TRACE_LOG( );
	int cur_index, next_index;
	cur_index = buf_index;
	if( !g_pBuf )
		return;
	while( cur_index >= 0 )
	{
		next_index = g_pBuf[cur_index].next;
		g_pBuf[cur_index].next = -1;
		g_pBuf[cur_index].buf_size = 0;
		cur_index = next_index;
		g_sbuf_count--;
	}
}

void free_onesbuf( int buf_index )
{
	TRACE_LOG( );
	if( !g_pBuf )
		return;
	if( buf_index >= 0 )
	{
		g_pBuf[buf_index].next = -1;
		g_pBuf[buf_index].buf_size = 0;
		g_sbuf_count--;
	}
}

//--------------------------------------------------------------------------------------
// 去掉一个客户端(针对g_sockset的操作),工作线程和主线程调用
//--------------------------------------------------------------------------------------
int unset_socket( int client_index, char is_remove )
{
	TRACE_LOG( );
	// 只允许一个线程进入, 否则就乱了
	if( client_index >= 0 && client_index < g_max_connection )
	{
		free_sbuf( g_sockset[client_index].m_send_head );
		g_sockset[client_index].m_send_head = -1;
		g_sockset[client_index].m_send_tail = -1;

		if( is_remove )
		{
			g_sockset[client_index].m_fd = INVALID_SOCKET;
			g_sockset[client_index].m_userid = 0;
			g_sockset[client_index].m_username[0] = 0;
			g_sockset[client_index].m_usertype = 0;

			g_sockset[client_index].m_stat = -1;
			g_sockset[client_index].m_iswait = 0;
			g_sockset[client_index].m_send_count = 0;
			g_sockset[client_index].m_recv_count = 0;
			g_sockset[client_index].m_exec_code = -1;
			memset( g_sockset[client_index].m_dev, 0, sizeof(char *)*DEVDATA_COUNT );
		}
		else
		{
			g_sockset[client_index].m_fd = EXITED_SOCKET;
		}
	}
	return 0;
}
//--------------------------------------------------------------------------------------
// 断开一个客户端, 工作线程和主线程调用
//--------------------------------------------------------------------------------------
void net_closesock( int client_index )
{
	TRACE_LOG( );
	SOCKET fd;
#ifndef WIN32
	struct epoll_event ev;
#endif
	mmux_lock( g_sockset[client_index].m_mmux );

	fd = g_sockset[client_index].m_fd;
	if( fd == INVALID_SOCKET || fd == EXITED_SOCKET )
	{
		mmux_unlock( g_sockset[client_index].m_mmux );
		return;
	}

	// 如果游戏逻辑内部仅仅只设置退出标志,g_sockset[client_index].m_fd应该置为EXITED_SOCKET,否则unset
	if( g_pCallbackDisConnected && g_pCallbackDisConnected( client_index ) == 0 )
	{
#ifndef WIN32
		if( fd >= 0 )
		{
			g_fd_map[fd] = -1;
		}
#endif
		unset_socket( client_index, 0 );
		--g_connectioncount;
#ifndef WIN32
		ev.data.fd = fd;
		ev.events = 0;
		epoll_ctl( g_epfd, EPOLL_CTL_DEL, fd, &ev );
#endif

		shutdown( fd, 2 );
#ifdef WIN32
		closesocket( fd );
#else
		close( fd );
#endif
		mmux_unlock( g_sockset[client_index].m_mmux );
		return;
	}
	unset_socket( client_index, 1 );
#ifndef WIN32
	ev.data.fd = fd;
	ev.events = 0;
	epoll_ctl( g_epfd, EPOLL_CTL_DEL, fd, &ev );
#endif

	shutdown( fd, 2 );
#ifdef WIN32
	closesocket( fd );
#else
	close( fd );
#endif
	--g_connectioncount;
	mmux_unlock( g_sockset[client_index].m_mmux );
}
//--------------------------------------------------------------------------------------
// 直接断开一个客户端, 游戏线程调用
//--------------------------------------------------------------------------------------
// #GATECHECK
void net_closesock_direct( int client_index )
{
	TRACE_LOG();
#ifdef _USEGATE
	if( g_bGateMode )
		return gs_closesock_direct( client_index );
#endif
	SOCKET fd;
#ifndef WIN32
	struct epoll_event ev;




#endif

	mmux_lock( g_sockset[client_index].m_mmux );
	fd = g_sockset[client_index].m_fd;
	if( fd == INVALID_SOCKET )
	{
		mmux_unlock( g_sockset[client_index].m_mmux );
		return;
	}

	if( fd != INVALID_SOCKET && fd != EXITED_SOCKET )
	{
#ifndef WIN32
		if( fd >= 0 )
		{
			g_fd_map[fd] = -1;
		}
#endif
		if( fd != EXITED_SOCKET )
			--g_connectioncount;
		unset_socket( client_index, 1 );
#ifndef WIN32
		ev.data.fd = fd;
		ev.events = 0;
		epoll_ctl( g_epfd, EPOLL_CTL_DEL, fd, &ev );
#endif
		shutdown( fd, 2 );
#ifdef WIN32
		closesocket( fd );
#else
		close( fd );
#endif
		mmux_unlock( g_sockset[client_index].m_mmux );
		return;
	}
	unset_socket( client_index, 1 );
	mmux_unlock( g_sockset[client_index].m_mmux );
}
//--------------------------------------------------------------------------------------
// 关闭一个客户端, 工作线程和主线程调用
//--------------------------------------------------------------------------------------
// #GATECHECK
void close_client( int client_index )
{
	TRACE_LOG( );
#ifdef _USEGATE
	if( g_bGateMode )
		return gs_close_client( client_index );
#endif
#ifdef WIN32
	LINGER  lingerStruct;
#endif
	if( client_index < 0 || client_index >= g_max_connection )
		return;

#ifdef WIN32
	// LOCK-这里是不是应该保护一下g_sockset?
	lingerStruct.l_onoff = 1;
	lingerStruct.l_linger = 0;
	setsockopt( g_sockset[client_index].m_fd, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct) );
#endif
	net_closesock( client_index );
	// UNLOCK
	return;
}
//--------------------------------------------------------------------------------------
// 创建一个监听端口, 主线程调用
//--------------------------------------------------------------------------------------
int create_listen()
{
	TRACE_LOG( );
#ifndef WIN32
	struct epoll_event ev;
	int on;
#else
	int nRet = 0;
	int nValue = 0;
#endif
	struct sockaddr_in serveraddr;

#ifdef WIN32
	// Windows模式下使用socket2.0
	listenfd = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
#else
	// Linux下设置sock为非阻塞
	listenfd = socket( AF_INET, SOCK_STREAM, 0 );
	setnonblocking( listenfd );

	// 允许地址重用
	on = 1;
	setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );

#endif
	memset( &serveraddr, 0, sizeof(serveraddr) );

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = g_ipaddress;
	serveraddr.sin_port = htons( g_port );

#ifndef WIN32
	ev.data.fd = listenfd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl( g_epfd, EPOLL_CTL_ADD, listenfd, &ev);
#endif

	// 绑定监听端口
	if( bind( listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) != 0 )
	{
		printf_msg( "Ip config error!\n" );
		printf_msg( "Now exiting...\n" );
#ifdef WIN32
		Sleep( 5000 );
#else
		sleep( 5 );
#endif
		return -1;
	}
	// 开始监听了
	listen( listenfd, LISTEN_QUEUE );

#ifdef WIN32
	// Windows下不使用发送缓冲区,自己搞得快!!!
	nValue = 0;
	if( setsockopt( listenfd, SOL_SOCKET, SO_SNDBUF, (char *)&nValue, sizeof(int) ) < 0 )
	{
		write_netlog( "setsockopt(SNDBUF) failed: %d\n", WSAGetLastError() );
		return -1;
	}
#endif

	return 0;
}

int get_sbuf_count()
{
	TRACE_LOG( );
	int tmpi;
	int sbuf_count = 0;
	for( tmpi = 0; tmpi < MAX_SBUF_COUNT; tmpi++ )
	{
		if( g_pBuf[tmpi].buf_size != 0 )
		{
			sbuf_count++;
		}
	}
	return sbuf_count;
}

//--------------------------------------------------------------------------------------
// 设置池中的一块内存并且加密, 然后得到它的索引
//--------------------------------------------------------------------------------------
int get_sbuf_index( int client_index, const char *buf, int buf_size )
{
	TRACE_LOG( );
	int tmpi;
	int buf_index = -1;
	if( buf_size <= 0 || buf_size > MAX_PACKAGE_SIZE )
		return -1;
	if( g_sockset[client_index].m_send_count > MAX_SENDLIST_COUNT )
		return -2;
	mmux_lock( g_sbuf_mmux );
	for( tmpi = 0; tmpi < MAX_SBUF_COUNT; tmpi++ )
	{
		if( g_pBuf[tmpi].buf_size == 0 )
		{
			g_pBuf[tmpi].buf_size = buf_size;
			g_pBuf[tmpi].client_index = client_index;
			buf_index = tmpi;
			break;
		}
	}
//	mmux_unlock( g_sbuf_mmux );
	if( buf_index >= 0 )
	{
//		write_netlog( "encode %d[%d]", buf_size, (unsigned short)netcode_looksendkey( client_index ) );
#ifndef NONECODE
		// 这里对发送的数据加密
//		mmux_lock( g_sockset[client_index].m_mmux );
		netcode_encode( client_index, g_pBuf[buf_index].buf, buf, buf_size );
//		write_netlog("size=%d encode=%d", *((short *)buf), *((short *)g_pBuf[buf_index].buf));
//		mmux_unlock( g_sockset[client_index].m_mmux );
#else
		memcpy( g_pBuf[buf_index].buf, buf, buf_size );
#endif
//		write_netlog( "		[%d]", (unsigned short)netcode_looksendkey( client_index ) );
#ifdef WIN32
		assert( g_pBuf[buf_index].next < 0 );
#endif
		g_pBuf[buf_index].next = -1;
		g_sbuf_count++;
	}
	mmux_unlock( g_sbuf_mmux );
	return buf_index;
}

// 检查sbuf,查找bug
int check_sbuf_count( int type, int *pcount_inv, int *pcount_exit )
{
	TRACE_LOG( );
	int tmpi;
	FILE *fp;
	int client_index;
	int count = 0;
	int count_inv = 0;
	int count_exit = 0;

	if( g_pBuf == NULL )
		return 0;

	fp = NULL;
	if( type )
		fp = fopen( "checkbuf.txt", "wb" );

	for( tmpi = 0; tmpi < MAX_SBUF_COUNT; tmpi++ )
	{
		if( g_pBuf[tmpi].buf_size != 0 )
		{
			client_index = g_pBuf[tmpi].client_index;
			if( g_sockset[client_index].m_fd == INVALID_SOCKET )
			{
				if( fp )
					fprintf( fp, "INVALID_SOCKET %d\n", client_index );
				count_inv++;
			}
			else if( g_sockset[client_index].m_fd == EXITED_SOCKET )
			{
				if( fp )
					fprintf( fp, "EXITED_SOCKET  %d\n", client_index );
				count_exit++;
			}
			else
			{
				if( fp )
					fprintf( fp, "USING_SOCKET   %d\n", client_index );
			}
			count++;
		}
	}
	if( pcount_inv )
		*pcount_inv = count_inv;
	if( pcount_exit )
		*pcount_exit = count_exit;
	if( fp )
		fclose( fp );
	return count;
}

//--------------------------------------------------------------------------------------
// 将接收到的一个客户的完整包数据放置到接收队列中, 工作线程调用(问题:已经接收的数据需要memcpy一次,有优化方案吗? )
//--------------------------------------------------------------------------------------
// #GATECHECK
int readtoqueue( int client_index, char *pBuf, int readsize, char stat )
{
	if( client_index != -2 )
	{
		TRACE_LOG();
	}
#ifdef _USEGATE
	if( g_bGateMode )
		return gs_readtoqueue( -1, client_index, pBuf, readsize, stat );
#endif
	int queue_tail;

	if ( readsize == 0 )
	{
		return -1;
    }
	// 锁住读队列缓冲
	mmux_lock( g_read_mmux );

	queue_tail = g_nReadQueueTail + 1;
	if( queue_tail >= MAX_QUEUE_NUM )
	{
		queue_tail = 0;
	}
	if( g_nReadQueueHead == queue_tail )
	{
		// 队列已经满了, 断开这个客户,已经是非常的危险时刻,放弃所有的数据包
		g_nReadQueueHead = g_nReadQueueTail;
		mmux_unlock( g_read_mmux );
		write_netlog( "warning:close client(read queue full)" );
		close_client( client_index );
		mcond_broadcast( g_pthr_recv );
		return -1;
	}

#ifndef NONECODE
	// 这里解密客户端的数据,将明文复制到 read_queue[g_nReadQueueTail].buf
	// 如果client_index是-2(内部数据), 不用解密
	if( stat && client_index >= 0 )
	{
//		write_netlog( "decode %d[%d]", readsize, (unsigned short)netcode_lookrecvkey( client_index ) );
		netcode_decode( client_index, read_queue[g_nReadQueueTail].buf, pBuf, readsize );
//		write_netlog( "		[%d]", (unsigned short)netcode_lookrecvkey( client_index ) );
		g_sockset[client_index].m_recv_count++;
	}
	else
		memcpy( read_queue[g_nReadQueueTail].buf, pBuf, readsize );
#else
	memcpy( read_queue[g_nReadQueueTail].buf, pBuf, readsize );
#endif
	read_queue[g_nReadQueueTail].buf_size = readsize;
	read_queue[g_nReadQueueTail].client_index = client_index;
	if( client_index >= 0 )
	{
		read_queue[g_nReadQueueTail].exec_code = g_sockset[client_index].m_exec_code;
	}
	else
	{
		read_queue[g_nReadQueueTail].exec_code = 0;
	}

	// 跨服专用数据
	read_queue[g_nReadQueueTail].type = 0;
	read_queue[g_nReadQueueTail].server_code = 0;
	read_queue[g_nReadQueueTail].cmd = 0;
	read_queue[g_nReadQueueTail].value[0] = 0;
	read_queue[g_nReadQueueTail].value[1] = 0;

	g_nReadQueueTail = queue_tail;

	mmux_unlock( g_read_mmux );

//	if(readsize>14)
//		printf_msg("readtoqueue,%d\n",readsize);
	// 通知接收线程开始工作
	mcond_broadcast( g_pthr_recv );
	return 0;
}
//--------------------------------------------------------------------------------------
// 将需要发送的数据放置到发送队列中, 游戏线程调用
//--------------------------------------------------------------------------------------
int sendtoqueue( int client_index, char *buf, int bufsize )
{
	TRACE_LOG( );
	int tail_index;
	int buf_index;
#ifdef _USEGPK
	char packbuf[MAX_PACKAGE_SIZE];
#endif

	if( client_index < 0 || client_index >= g_max_connection )
		return -1;
	if( buf == NULL || bufsize <= 0 || bufsize >= MAX_PACKAGE_SIZE )
	{
		write_netlog("error: sendtoqueue bufsize:%d",bufsize);
		return -1;
	}
//	printf_msg( "sending[%d]...\n", bufsize );
	if( g_bExit )
		return -1;

	if( g_sockset[client_index].m_fd == INVALID_SOCKET || g_sockset[client_index].m_fd == EXITED_SOCKET )
		return -1;

#ifdef _USEGPK
	if( bufsize > sizeof(short) )
	{
		memcpy( packbuf, buf, bufsize );
		lib.GPKEncode( g_pSvrDynCode, (unsigned char*)(packbuf+sizeof(short)), bufsize-sizeof(short), g_sockset[client_index].m_gpk_key );
	}
	// 将待发数据放置到池中
	buf_index = get_sbuf_index( client_index, packbuf, bufsize );
#else
	// 将待发数据放置到池中
	buf_index = get_sbuf_index( client_index, buf, bufsize );
#endif
	if( buf_index < 0 )
	{
		// 池满了,断开这个客户
		close_client( client_index );
		if( buf_index == -2 )
			write_netlog("warning:send count max.bufsize:%d",bufsize);
		else
			write_netlog("error:buf pool full.bufsize:%d",bufsize);
		return -1;
	}

	// 锁住这个客户的发送缓冲区
	mmux_lock( g_sockset[client_index].m_mmux );
	// 锁定后了再检测一次
	if( g_sockset[client_index].m_fd == INVALID_SOCKET || g_sockset[client_index].m_fd == EXITED_SOCKET )
	{
		mmux_unlock( g_sockset[client_index].m_mmux );
		// 既然不用就应该退还申请空间
		free_onesbuf( buf_index );
		return -1;
	}
	// 把buf_index放到客户的的发送缓冲区中
	if( g_sockset[client_index].m_send_tail < 0 )
	{
		// 客户的发送区是空的, 将他挂在开始的位置
		g_sockset[client_index].m_send_head = buf_index;
		g_sockset[client_index].m_send_tail = buf_index;
		g_sockset[client_index].m_send_count = 1;
	}
	else
	{
		// 如果有数据没有发完, 将他挂在尾部
		tail_index = g_sockset[client_index].m_send_tail;
		if( tail_index < 0 || tail_index >= MAX_SBUF_COUNT )
		{
			// 数据错误,断开这个客户
			mmux_unlock( g_sockset[client_index].m_mmux );
			close_client( client_index );
			// 既然不用就应该退还申请空间
			free_onesbuf( buf_index );
			write_netlog("pool data error[tail_index:%d]",tail_index);
			return -1;
		}
		g_pBuf[tail_index].next = buf_index;
		g_sockset[client_index].m_send_tail = buf_index;
		g_sockset[client_index].m_send_count++;
		if( g_sockset[client_index].m_send_count > g_max_send_count )
			g_max_send_count = g_sockset[client_index].m_send_count;
	}
	if( g_sockset[client_index].m_send_buf.m_nTotalBytes <= 0 )
		fill_send_buf( client_index );
	mmux_unlock( g_sockset[client_index].m_mmux );

	return 0;
}

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
// 接收的工作线程, 会调用游戏进程处理包的操作, 如果这个操作比较慢, 会对整个数据处理有影响, 慢速的操作应该扔到一个慢速的处理流程中
int recv_proc()
{
	TRACE_LOG( );
	int jmpret;

	int client_index;
	int readsize;
	int truesize;
	char tmpbuf[MAX_PACKAGE_SIZE];
	int exec_code;

	char type;
	short server_code;
	short cmd;
	int value1;
	int value2;
//	int tmpi;
//	static char szmsg[MAX_PACKAGE_SIZE+32];

	g_extcode = 0;
	srand( (unsigned int)time(NULL) );
	while( g_pCallbackProcessPack == NULL )
	{
		printf_msg( "g_pCallbackProcessPack Is empty, Wait...\n" );
#ifdef WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
	}
	printf_msg( "recv_proc is run!\n" );

	jmpret = setjmp( g_mark );
/*	if( jmpret != 0 )
	{
		g_bExit = 1;
		if( listenfd != INVALID_SOCKET )
		{
#ifdef WIN32
			closesocket(listenfd);
#else
			close(listenfd);
#endif
			listenfd = INVALID_SOCKET;
		}
	}
*/
	g_extcode = 1;
	while( !g_bExit )
	{
		g_extcode = 2;
		readsize = 0;
		// 锁住队列缓冲
		mmux_lock( g_read_mmux );
		while( g_nReadQueueTail == g_nReadQueueHead )
		{
			mcond_wait( g_pthr_recv, g_read_mmux );
			if( g_bExit )
			{
				mmux_unlock( g_read_mmux );
				write_netlog( "[thread]Recv Proc Exited." );
				return 0;
			}
		}
		// 到这了, 应该就有可接收的数据了
		g_extcode = 3;

		// 从队列中取出一项
		type = read_queue[g_nReadQueueHead].type;
		if ( type == 1 )
		{// 跨服数据类型
			// 将数据从接收队列中取出来, 然后就可以自己工作了
			memcpy( tmpbuf, read_queue[g_nReadQueueHead].buf, read_queue[g_nReadQueueHead].buf_size );
			truesize = read_queue[g_nReadQueueHead].buf_size;
			exec_code = read_queue[g_nReadQueueHead].exec_code;
			server_code = read_queue[g_nReadQueueHead].server_code;
			cmd = read_queue[g_nReadQueueHead].cmd;
			value1 = read_queue[g_nReadQueueHead].value[0];
			value2 = read_queue[g_nReadQueueHead].value[1];

			g_nReadQueueHead++;
			if ( g_nReadQueueHead >= MAX_QUEUE_NUM )
			{
				g_nReadQueueHead = 0;
			}
			mmux_unlock( g_read_mmux );
			if ( g_pCallbackGlobalPack )
				g_pCallbackGlobalPack( server_code, cmd, value1, value2, tmpbuf + 2, truesize - 2 );
		}
		else
		{
			client_index = read_queue[g_nReadQueueHead].client_index;
			if ( client_index == -2 || client_index <= -10000 )
			{
				memcpy( tmpbuf, read_queue[g_nReadQueueHead].buf, read_queue[g_nReadQueueHead].buf_size );
				readsize = read_queue[g_nReadQueueHead].buf_size;

				g_nReadQueueHead++;
				if ( g_nReadQueueHead >= MAX_QUEUE_NUM )
				{
					g_nReadQueueHead = 0;
				}
				mmux_unlock( g_read_mmux );
				g_extcode = 4;
				g_pCallbackProcessPack( client_index, tmpbuf, readsize, 0 );
				g_extcode = 5;
				continue;
			}
			if ( client_index < 0 )
			{
				// 这个数据是非法的
				g_nReadQueueHead++;
				if ( g_nReadQueueHead >= MAX_QUEUE_NUM )
				{
					g_nReadQueueHead = 0;
				}
				mmux_unlock( g_read_mmux );
				g_extcode = 6;
				close_client( client_index );
				write_netlog( "[recv_proc] error client_index:%d", client_index );
				g_extcode = 7;
				continue;
			}
			// 将数据从接收队列中取出来, 然后就可以自己工作了
			memcpy( tmpbuf, read_queue[g_nReadQueueHead].buf, read_queue[g_nReadQueueHead].buf_size );
			truesize = read_queue[g_nReadQueueHead].buf_size;
			exec_code = read_queue[g_nReadQueueHead].exec_code;
			g_nReadQueueHead++;
			if ( g_nReadQueueHead >= MAX_QUEUE_NUM )
			{
				g_nReadQueueHead = 0;
			}
			mmux_unlock( g_read_mmux );
			//		strcpy( szmsg, "recv data:" );
			//		for( tmpi = 0; tmpi < truesize; tmpi++ )
			//		{
			//			strcat( szmsg, Byte2Hex( tmpbuf[tmpi] ) );
			//			strcat( szmsg, " " );
			//		}
			//		write_netlog( szmsg );

			//		printf_msg("recv:%d\n",truesize);
			g_extcode = 8;
			if ( exec_code == g_sockset[client_index].m_exec_code )
			{
				if ( g_sockset[client_index].m_stat < 0 && g_bNoHeader == 0 )
				{
					g_extcode = 9;
					net_recv_clientver( client_index, tmpbuf, truesize );
				}
				else
				{
					if ( g_sockset[client_index].m_stat < 0 )
						g_sockset[client_index].m_stat = 0;
					g_extcode = 10;
					g_pCallbackProcessPack( client_index, tmpbuf, truesize, exec_code );
				}
				g_extcode = 11;
			}
		}
		
	}
	write_netlog( "[thread]Recv Proc Exited." );
	g_extcode = 12;
	return 0;
}

//--------------------------------------------------------------------------------------
// 填充发送缓冲区,并且调用一次发送通知, 游戏线程和主epoll轮询线程调用
//--------------------------------------------------------------------------------------
int fill_send_buf( int client_index )
{
	TRACE_LOG( );
	int buf_index;
	int writesize;
#ifdef WIN32
	DWORD dwSendNumBytes = 0;
	DWORD dwFlags = 0;
	int nRet;
#else
	struct epoll_event ev;
#endif
	SOCKET_BUF *pSocketBuf;

	// 客户的发送的互斥对象已经在外面锁住了

	// 先获得池中的索引
	buf_index = g_sockset[client_index].m_send_head;
	if( buf_index < 0 )
		return -1;

	pSocketBuf = &g_sockset[client_index].m_send_buf;
#ifdef WIN32
	// 让WSABuf指向客户的发送指针上
	g_sockset[client_index].m_send_buf.m_wsabuf.buf = pSocketBuf->m_pBuffer;
#endif
	// 合并所有的待发块, 复制到发送指针上
	writesize = 0;
	while( writesize + g_pBuf[buf_index].buf_size <= MAX_BUF_SIZE )
	{
		if( g_pBuf[buf_index].buf_size <= 0 )
			break;
		// 这里没有锁住池子, 因为数据是在使用中, 应该没必要
		memcpy( pSocketBuf->m_pBuffer+writesize, g_pBuf[buf_index].buf, g_pBuf[buf_index].buf_size );
		writesize += g_pBuf[buf_index].buf_size;
		g_sockset[client_index].m_send_head = g_pBuf[buf_index].next;
		g_sockset[client_index].m_send_count--;

		g_pBuf[buf_index].next = -1;
		g_pBuf[buf_index].buf_size = 0;
		g_sbuf_count--;
		buf_index = g_sockset[client_index].m_send_head;
		if( buf_index < 0 )
		{
			g_sockset[client_index].m_send_tail = -1;
			if( g_sockset[client_index].m_send_count > 0 )
			{
				write_netlog( "Warning:send list is empty but sendcount > 0" );
				g_sockset[client_index].m_send_count = 0;
			}
			break;
		}
	}
	if( writesize > 0 )
	{
		pSocketBuf->m_nTotalBytes = writesize;
		pSocketBuf->m_nProcBytes  = 0;
#ifdef WIN32
		pSocketBuf->m_wsabuf.len  = writesize;
		dwFlags = 0;
		nRet = WSASend( g_sockset[client_index].m_fd, &g_sockset[client_index].m_send_buf.m_wsabuf, 1, &dwSendNumBytes, dwFlags, &(g_sockset[client_index].m_send_buf.m_Overlapped), NULL );
		if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) )
		{
			printf_msg("WSASend() failed: %d\n", WSAGetLastError());
			close_client( client_index );
		}
#else
		ev.data.fd = g_sockset[client_index].m_fd;
		ev.events = EPOLLOUT | EPOLLET;
		epoll_ctl( g_epfd, EPOLL_CTL_MOD, g_sockset[client_index].m_fd, &ev );
#endif
		return 0;
	}
	return -1;
}

//--------------------------------------------------------------------------------------
// 解析数据包, 放置到接收队列, 返回已经处理完的字节数
//--------------------------------------------------------------------------------------
// 对于网络底层, 是简单的包结构: len[short]|data[len]
int recv_package( int client_index, char *buf, int size )
{
	TRACE_LOG( );
	char *ptr;
	int package_size;
	int proc_size;
	int remain_size;
	char stat;
	if( size > MAX_BUF_SIZE )
		return -1;

	proc_size = 0;
	remain_size = size;
	ptr = buf;
	while(1)
	{
		if( remain_size < (int)sizeof(short) )
			return proc_size;
		if( g_sockset[client_index].m_stat < 0 && g_bNoHeader == 0 )
		{
//			package_size = 32;
			package_size = *((short *)ptr);
			package_size += sizeof(short);
			stat = 0;
		}
		else
		{
#ifndef NONECODE
			// 如果包长度也加密, 这里的包长度也需要解密
			package_size = netcode_decode_size( client_index, *( (short *)ptr ) );	// package_size 包括自己，所以至少必须大于sizeof(short)
#else
			package_size = *( (short *)ptr );	// package_size 包括自己，所以至少必须大于sizeof(short)
#endif
			package_size += sizeof(short);
			stat = 1;
		}
		if( package_size < (int)sizeof(short) || package_size >= MAX_PACKAGE_SIZE )
			return -1;
		if( remain_size < package_size )
			return proc_size;
		g_sockset[client_index].m_idle_time = 0;
#ifdef _USEGPK
//		write_netlog("gpk decode size:%d",package_size);
		if( stat && package_size > sizeof(short) )
			lib.GPKDecode( g_pSvrDynCode, (unsigned char*)(ptr+sizeof(short)), package_size-sizeof(short), g_sockset[client_index].m_gpk_key );
//		write_netlog("gpk decode over:%d",package_size);
#endif
#ifdef _USEGATE
		//printf( "recv from client package_size:%d client_index:%d\n", package_size, client_index );
#endif
		readtoqueue( client_index, ptr, package_size, stat );
		proc_size += package_size;
		remain_size -= package_size;
		ptr += package_size;
	}
	return -1;
}

//--------------------------------------------------------------------------------------
// 创建一个新的socket, 主进程调用
//--------------------------------------------------------------------------------------
int new_socket_buf( SOCKET fd )
{
	TRACE_LOG( );
	int client_index;
	// LOCK g_sockset
	client_index = add_socketindex( fd );
	if( client_index < 0 )
		return -1;
	g_sockset[client_index].m_fd = fd;
	g_sockset[client_index].m_idle_time = 0;
#ifdef WIN32
	g_sockset[client_index].m_fd = fd;
	g_sockset[client_index].m_recv_buf.m_Overlapped.Internal = 0;
	g_sockset[client_index].m_recv_buf.m_Overlapped.InternalHigh = 0;
	g_sockset[client_index].m_recv_buf.m_Overlapped.Offset = 0;
	g_sockset[client_index].m_recv_buf.m_Overlapped.OffsetHigh = 0;
	g_sockset[client_index].m_recv_buf.m_Overlapped.hEvent = NULL;

	g_sockset[client_index].m_send_buf.m_Overlapped.Internal = 0;
	g_sockset[client_index].m_send_buf.m_Overlapped.InternalHigh = 0;
	g_sockset[client_index].m_send_buf.m_Overlapped.Offset = 0;
	g_sockset[client_index].m_send_buf.m_Overlapped.OffsetHigh = 0;
	g_sockset[client_index].m_send_buf.m_Overlapped.hEvent = NULL;

	g_sockset[client_index].m_recv_buf.m_nOption = IOREAD;
	g_sockset[client_index].m_recv_buf.m_nTotalBytes = 0;
	g_sockset[client_index].m_recv_buf.m_nProcBytes = 0;
	g_sockset[client_index].m_recv_buf.m_wsabuf.buf = g_sockset[client_index].m_recv_buf.m_pBuffer;
	g_sockset[client_index].m_recv_buf.m_wsabuf.len = sizeof( g_sockset[client_index].m_recv_buf.m_pBuffer );
	memset( g_sockset[client_index].m_recv_buf.m_wsabuf.buf, 0, g_sockset[client_index].m_recv_buf.m_wsabuf.len );

	g_sockset[client_index].m_send_buf.m_nOption = IOWRITE;
	g_sockset[client_index].m_send_buf.m_nTotalBytes = 0;
	g_sockset[client_index].m_send_buf.m_nProcBytes = 0;
	g_sockset[client_index].m_send_buf.m_wsabuf.buf = g_sockset[client_index].m_send_buf.m_pBuffer;
	g_sockset[client_index].m_send_buf.m_wsabuf.len = sizeof( g_sockset[client_index].m_send_buf.m_pBuffer );
	memset( g_sockset[client_index].m_send_buf.m_wsabuf.buf, 0, g_sockset[client_index].m_send_buf.m_wsabuf.len );
#else
	g_sockset[client_index].m_recv_buf.m_nTotalBytes = MAX_BUF_SIZE;
	g_sockset[client_index].m_recv_buf.m_nProcBytes = 0;
	g_sockset[client_index].m_recv_buf.m_ptr = g_sockset[client_index].m_recv_buf.m_pBuffer;
	memset( g_sockset[client_index].m_recv_buf.m_ptr, 0, MAX_BUF_SIZE );

	g_sockset[client_index].m_send_buf.m_nTotalBytes = 0;
	g_sockset[client_index].m_send_buf.m_nProcBytes = 0;
	g_sockset[client_index].m_send_buf.m_ptr = g_sockset[client_index].m_send_buf.m_pBuffer;
	memset( g_sockset[client_index].m_send_buf.m_ptr, 0, MAX_BUF_SIZE );
#endif
	// UNLOCK
	return client_index;
}
//--------------------------------------------------------------------------------------
// 更新fd的映射, 当新建立了一个客户的连接, 主线程调用
//--------------------------------------------------------------------------------------
int update_fdmap( SOCKET fd, char *ip )
{
	TRACE_LOG( );
#ifdef WIN32
	HANDLE hIOCP;
#endif
	int client_index;
	client_index = new_socket_buf( fd );
	if( client_index < 0 )
		return -1;

	g_sockset[client_index].m_enter_time = (int)time(NULL);
	g_sockset[client_index].m_exec_code = g_exec_code ++;
#ifndef NONECODE
	g_sockset[client_index].m_send_key = netcode_create();
	g_sockset[client_index].m_recv_key = netcode_create();
	//printf("%d,%d\n", g_sockset[client_index].m_recv_key, g_sockset[client_index].m_send_key);
#endif
#ifdef WIN32
	hIOCP = CreateIoCompletionPort( (HANDLE)fd, g_hIOCP, (DWORD)client_index, 0 );
	if( hIOCP == NULL )
	{
		printf_msg("CreateIoCompletionPort() failed: %d\n", GetLastError());
		return -1;
	}
#else
	g_fd_map[fd] = client_index;
#endif
	strcpy( g_sockset[client_index].m_ip, ip );
	return client_index;
}

#ifdef WIN32
//--------------------------------------------------------------------------------------
// Windows系统下的工作线程
//--------------------------------------------------------------------------------------
int work_proc( LPVOID Value )
{
	TRACE_LOG( );
	HANDLE hIOCP = (HANDLE)Value;
	BOOL bSuccess = FALSE;
	int nRet = 0;
	LPWSAOVERLAPPED lpOverlapped = NULL;

	SOCKET_BUF *pSocketBuf = NULL; 

	DWORD dwRecvNumBytes = 0;
	DWORD dwSendNumBytes = 0;
	DWORD dwFlags = 0;
	DWORD dwIoSize = 0;

	int proc_size;
	int client_index;

	while(1)
	{
		// 查询完成端口状态
		bSuccess = GetQueuedCompletionStatus(hIOCP, &dwIoSize,
											 (PDWORD_PTR)&client_index,
											 (LPOVERLAPPED *)&lpOverlapped, 
											 INFINITE);
		if( !bSuccess )
			write_netlog( "GetQueuedCompletionStatus() failed: %d\n", GetLastError() );

		// 如果参数不正确, 或者系统准备退出, 就返回
		if( client_index < 0 || g_bExit )
		{
			// Exit
			printf_msg( "[thread]Work Proc Exited.\n" );
			return 0;
		}
		// 查询失败或者没有任何数据, 关闭该客户端, 继续查询
		if( !bSuccess || ( bSuccess && (dwIoSize == 0) ) )
		{
			// 客户端断开连接
			close_client( client_index );
			continue;
		}

		pSocketBuf = (SOCKET_BUF *)lpOverlapped;
		switch( pSocketBuf->m_nOption )
		{
		case IOREAD:
			g_log_recvcount += dwIoSize;
//			printf_msg( "Totle Recv:%d\n", g_log_recvcount );
			// 完成读dwIoSize
			pSocketBuf->m_nTotalBytes += dwIoSize;
			proc_size = recv_package( client_index, pSocketBuf->m_pBuffer, pSocketBuf->m_nTotalBytes );
			if( proc_size < 0 )
			{
				write_netlog( "recv_package() failed: %d\n", proc_size );
				close_client( client_index );
				break;
			}

			dwRecvNumBytes = 0;
			dwFlags = 0;

			if( proc_size > 0 )
			{
				// 将未处理完的数据往前移
				pSocketBuf->m_nTotalBytes -= proc_size;
				memmove( pSocketBuf->m_pBuffer, pSocketBuf->m_pBuffer+proc_size, pSocketBuf->m_nTotalBytes );
			}
			pSocketBuf->m_wsabuf.buf = pSocketBuf->m_pBuffer + pSocketBuf->m_nTotalBytes;
			pSocketBuf->m_wsabuf.len = MAX_BUF_SIZE - pSocketBuf->m_nTotalBytes;

			// 永远要不停的读
			nRet = WSARecv( g_sockset[client_index].m_fd, &pSocketBuf->m_wsabuf, 1, &dwRecvNumBytes, &dwFlags, &pSocketBuf->m_Overlapped, NULL);
			if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) )
			{
				write_netlog( "WSARecv() failed: %d\n", WSAGetLastError() );
				close_client( client_index );
			}
			break;

		case IOWRITE:
			// 完整式写
			g_log_sendcount += dwIoSize;
//			printf_msg( "Totle Send:%d\n", g_log_sendcount );
			pSocketBuf->m_nProcBytes  += dwIoSize;
			dwFlags = 0;
			mmux_lock( g_sockset[client_index].m_mmux );
			if( pSocketBuf->m_nProcBytes < pSocketBuf->m_nTotalBytes )
			{
				pSocketBuf->m_wsabuf.buf = pSocketBuf->m_pBuffer + pSocketBuf->m_nProcBytes;
				pSocketBuf->m_wsabuf.len = pSocketBuf->m_nTotalBytes - pSocketBuf->m_nProcBytes;
				nRet = WSASend( g_sockset[client_index].m_fd, &pSocketBuf->m_wsabuf, 1, &dwSendNumBytes, dwFlags, &(pSocketBuf->m_Overlapped), NULL );
				if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) )
				{
					write_netlog( "WSASend() failed: %d\n", WSAGetLastError() );
					close_client( client_index );
				}
			}
			else
			{
				if( fill_send_buf( client_index ) < 0 )
				{
					pSocketBuf->m_nTotalBytes = 0;
					pSocketBuf->m_nProcBytes  = 0;
					pSocketBuf->m_wsabuf.len  = 0;
				}
			}
			mmux_unlock( g_sockset[client_index].m_mmux );
			break;

		} //switch
	} //while
	return 0;
}
#else
//--------------------------------------------------------------------------------------
// Epoll下获得读操作时的处理
//--------------------------------------------------------------------------------------
void do_read_client( int client_index )
{
	int readsize;
	int proc_size;
	int buflen;
	SOCKET_BUF *pSocketBuf = &g_sockset[client_index].m_recv_buf;

	if( client_index < 0 )
		return;

	pSocketBuf->m_ptr = pSocketBuf->m_pBuffer + pSocketBuf->m_nProcBytes;
	buflen = pSocketBuf->m_nTotalBytes - pSocketBuf->m_nProcBytes;

	// 直接从sock中读数据到队列的缓冲
	if ( ( readsize = read( g_sockset[client_index].m_fd, pSocketBuf->m_ptr, buflen ) ) <= 0)
	{
		if( errno == ECONNRESET )
		{
			// 连接断开了(连接重置了)
			write_netlog( "read error, socket reset! close socket.fd=%d", g_sockset[client_index].m_fd );
			close_client( client_index );
			return;
		}
		else
		{
			// 读取错误
			write_netlog( "read error size=%d fd=%d! close socket.", readsize, g_sockset[client_index].m_fd );
			close_client( client_index );
			return;
		}
	}
	g_log_recvcount += readsize;
//	write_netlog("recv: [%d-%d]", readsize, g_sockset[client_index].m_fd );

	pSocketBuf->m_nProcBytes += readsize;
	proc_size = recv_package( client_index, pSocketBuf->m_pBuffer, pSocketBuf->m_nProcBytes );
	if( proc_size < 0 )
	{
		write_netlog( "recv_package() failed: %d", proc_size );
		close_client( client_index );
		return;
	}
	else if( proc_size > 0 )
	{
		// 将未处理完的数据往前移
		pSocketBuf->m_nProcBytes -= proc_size;
		memmove( pSocketBuf->m_pBuffer, pSocketBuf->m_pBuffer+proc_size, pSocketBuf->m_nProcBytes );
	}
}
//--------------------------------------------------------------------------------------
// Epoll下获得写操作时的处理
//--------------------------------------------------------------------------------------
void do_write_client( int client_index )
{
	int writesize;
	int buflen;
	struct epoll_event ev;
	SOCKET_BUF *pSocketBuf;

	if( client_index < 0 )
		return;

	pSocketBuf = &g_sockset[client_index].m_send_buf;

	mmux_lock( g_sockset[client_index].m_mmux );
	pSocketBuf->m_ptr = pSocketBuf->m_pBuffer + pSocketBuf->m_nProcBytes;
	buflen = pSocketBuf->m_nTotalBytes - pSocketBuf->m_nProcBytes;

	// 直接从sock中读数据到队列的缓冲
	if ( ( writesize = write( g_sockset[client_index].m_fd, pSocketBuf->m_ptr, buflen ) ) <= 0)
	{
		if( errno == ECONNRESET )
		{
			// 连接断开了(连接重置了)
			mmux_unlock( g_sockset[client_index].m_mmux );
			close_client( client_index );
			write_netlog( "write error, socket reset! close socket." );
			return;
		}
		else
		{
			// 写入错误
			write_netlog( "write error size=%d fd=%d!continue.", writesize, g_sockset[client_index].m_fd );
			write_netlog( "write error buflen:%d pSocketBuf->m_nTotalBytes:%d pSocketBuf->m_nProcBytes:%d", buflen, pSocketBuf->m_nTotalBytes, pSocketBuf->m_nProcBytes );
			mmux_unlock( g_sockset[client_index].m_mmux );
			return;
		}
	}
////////////////////////	write_netlog( "write packet size=%d index=%d fd=%d!", writesize, client_index, g_sockset[client_index].m_fd );

	g_log_sendcount += writesize;
//	write_netlog("send: [%d-%d]", writesize, g_sockset[client_index].m_fd );

	pSocketBuf->m_nProcBytes += writesize;

	if( pSocketBuf->m_nProcBytes < pSocketBuf->m_nTotalBytes )
	{
		// 还要继续写
		ev.data.fd = g_sockset[client_index].m_fd;
		ev.events = EPOLLOUT | EPOLLET;
		epoll_ctl( g_epfd, EPOLL_CTL_MOD, g_sockset[client_index].m_fd, &ev );
	}
	else
	{
		// 写完了,再填充新的需要写的内容
		if( fill_send_buf( client_index ) < 0 )
		{
			// 无数据填充
			pSocketBuf->m_nTotalBytes = 0;
			pSocketBuf->m_nProcBytes  = 0;

			ev.data.fd = g_sockset[client_index].m_fd;
			ev.events = EPOLLIN | EPOLLET;
			epoll_ctl( g_epfd, EPOLL_CTL_MOD, g_sockset[client_index].m_fd, &ev );
		}
	}
	mmux_unlock( g_sockset[client_index].m_mmux );
}

#endif


void net_setdev( int client_index )
{
	char *strp;
	int tmpi = 0;

	if ( client_index < 0 || client_index >= g_max_connection )
		return;
	char devdata[256] = { 0 };
	strncpy( devdata, g_sockset[client_index].m_devdata, 255 );

	char *str = devdata;
	char sign = '|';

	strp = str;
	while( *str )
	{
		if( *str == sign )
		{
			g_sockset[client_index].m_dev[tmpi++] = strp;
			*str = 0;
			str++;
//			while( *str == ' ' || *str == '\t' )
//				str++;
			strp = str;
			if( tmpi >= DEVDATA_COUNT )
				return;
		}
		else
			str++;
	}
	g_sockset[client_index].m_dev[tmpi++] = strp;
	for( ; tmpi < DEVDATA_COUNT; tmpi++ )
		g_sockset[client_index].m_dev[tmpi] = str;

}

//--------------------------------------------------------------------------------------
// 获得客户的版本信息, 然后发送版本信息给客户
//--------------------------------------------------------------------------------------
int net_recv_clientver( int client_index, char *buf, int truesize )
{
	TRACE_LOG( );
	int ver[3] = { 0 };
	int os;
	short platid;
	short country;
	short language;
	short invcode;
	int channelid;
	if( truesize < 32 )
	{
		//add_forbid_ip( &g_net_protect, g_sockset[client_index].m_ip );
		if( g_pCallbackOnError )
			g_pCallbackOnError( client_index, 1 );
		write_netlog("warning:ver size error [%s]", g_sockset[client_index].m_ip );
		close_client( client_index );
		return 0;
	}
	// 长度|版本1|版本2|版本2|操作系统类型|。。。|
	ver[0]		= *((int *)(buf+2));
	ver[1]		= *((int *)(buf+6));
	ver[2]		= *((int *)(buf + 10));
	os			= *((int *)(buf + 14));
	platid		= *((short *)(buf + 18));
	country		= *((short *)(buf + 20));
	language	= *((short *)(buf + 22));
	invcode		= *((int *)(buf + 24));
	channelid	= *((int *)(buf + 28));

	if( truesize == 32 )
	{
		g_sockset[client_index].m_devdata[0] = 0;
		//net_setdev( client_index );
	}
	else
	{
		int devsize = *((short *)(buf + 32));
		if( devsize >= MAX_DEVDATA_SIZE || devsize < 0 )
		{
			g_sockset[client_index].m_devdata[0] = 0;
		}
		else
		{
			memcpy( g_sockset[client_index].m_devdata, (buf + 34), devsize );
			g_sockset[client_index].m_devdata[devsize] = 0;

			//net_setdev( client_index );
		}
	}

	if( ver[1] < g_client_ver.ss.m2 || ( ver[1] == g_client_ver.ss.m2 && ver[2] < g_client_ver.ss.m3 ) )
	{
		if( g_pCallbackOnError )
			g_pCallbackOnError( client_index, 2 );
		write_netlog("warning:ver error [%s]", g_sockset[client_index].m_ip );

		g_sockset[client_index].m_stat = -2;
		net_send_clientver( client_index, 1 );
		//close_client( client_index );
		return 0;
	}
	g_sockset[client_index].m_platid = platid;
	g_sockset[client_index].m_language = (char)language;
	g_sockset[client_index].m_invcode = invcode;
	g_sockset[client_index].m_os = os;
	g_sockset[client_index].m_channelid = (short)channelid;
	g_sockset[client_index].m_ver[0] = ver[0];
	g_sockset[client_index].m_ver[1] = ver[1];
	g_sockset[client_index].m_ver[2] = ver[2];
	g_sockset[client_index].m_stat = 0;
	g_sockset[client_index].m_country = country;
	net_send_clientver( client_index, 0 );
	return 0;
}

int net_send_onepackeg( int client_index, char *pack_data, short pack_size )
{
	TRACE_LOG( );
	// 发送第一个包(主要是版本号,以及加密字) |序列号[4]|接收密字[4]|发送密字[4]|版本号[4](已经加密)|
	int tmpi;
	int buf_index = -1;
	int tail_index;

	if( client_index < 0 )
		return -1;
	mmux_lock( g_sbuf_mmux );
	for( tmpi = 0; tmpi < MAX_SBUF_COUNT; tmpi++ )
	{
		if( g_pBuf[tmpi].buf_size == 0 )
		{
			g_pBuf[tmpi].buf_size = pack_size;
			g_pBuf[tmpi].client_index = client_index;
			buf_index = tmpi;
			g_sbuf_count++;
			break;
		}
	}

	if( buf_index < 0 )
	{
		mmux_unlock( g_sbuf_mmux );
		if( buf_index == -2 )
			write_netlog("warning:send count max");
		else
			write_netlog("error:buf pool full");
		close_client( client_index );
		return -1;
	}

	memcpy( g_pBuf[buf_index].buf, pack_data, pack_size );
	g_pBuf[buf_index].next = -1;
	mmux_unlock( g_sbuf_mmux );

	// 锁住这个客户的发送缓冲区
	mmux_lock( g_sockset[client_index].m_mmux );
	// 把buf_index放到客户的的发送缓冲区中
	if( g_sockset[client_index].m_send_tail < 0 )
	{
		// 客户的发送区是空的, 将他挂在开始的位置
		g_sockset[client_index].m_send_head = buf_index;
		g_sockset[client_index].m_send_tail = buf_index;
		g_sockset[client_index].m_send_count = 1;
	}
	else
	{
		// 如果有数据没有发完, 将他挂在尾部
		tail_index = g_sockset[client_index].m_send_tail;
		if( tail_index < 0 || tail_index >= MAX_SBUF_COUNT )
		{
			// 数据错误,断开这个客户
			mmux_unlock( g_sockset[client_index].m_mmux );
			close_client( client_index );
			// 既然不用就应该退还申请空间
			free_onesbuf( buf_index );
			write_netlog("pool data error[tail_index:%d]",tail_index);
			return -1;
		}
		g_pBuf[tail_index].next = buf_index;
		g_sockset[client_index].m_send_tail = buf_index;
		g_sockset[client_index].m_send_count++;
		if( g_sockset[client_index].m_send_count > g_max_send_count )
			g_max_send_count = g_sockset[client_index].m_send_count;
	}

	if( g_sockset[client_index].m_send_buf.m_nTotalBytes <= 0 )
		fill_send_buf( client_index );
	mmux_unlock( g_sockset[client_index].m_mmux );

	return 0;
}

int net_send_long_packeg( int client_index, char *long_data, int long_size )
{
	char *pack_data;
	short pack_size;
	int leftsize;

	leftsize = long_size;
	pack_data = long_data;
	//MAX_PACKAGE_SIZE
	while( leftsize > 0 )
	{
		pack_size = leftsize > MAX_PACKAGE_SIZE ? MAX_PACKAGE_SIZE : leftsize;
		if( net_send_onepackeg( client_index, pack_data, pack_size ) < 0 )
			return -1;
		pack_data += pack_size;
		leftsize -= pack_size;
	}
	return 0;
}

//--------------------------------------------------------------------------------------
// 发送版本信息(特别的发送)
//--------------------------------------------------------------------------------------
int net_send_clientver( int client_index, int err_code )
{
	TRACE_LOG( );
	// 发送第一个包(主要是版本号,以及加密字) |序列号[4]|接收密字[4]|发送密字[4]|版本号[4](已经加密)|
	int bufsize;
	char *ptr;
	int tmpi;
	int buf_index = -1;
#ifdef _USEGPK
	const unsigned char *pCode = NULL;
#endif
	int nCodeLen = 0;
	int nCodeIdx = 0;

	if( client_index < 0 )
		return -1;
	mmux_lock( g_sbuf_mmux );
	for( tmpi = 0; tmpi < MAX_SBUF_COUNT; tmpi++ )
	{
		if( g_pBuf[tmpi].buf_size == 0 )
		{
			g_pBuf[tmpi].buf_size = 38;
			g_pBuf[tmpi].client_index = client_index;
			buf_index = tmpi;
			g_sbuf_count++;
			break;
		}
	}
//	mmux_unlock( g_sbuf_mmux );
	if( buf_index < 0 )
	{
		mmux_unlock( g_sbuf_mmux );
		if( buf_index == -2 )
			write_netlog("warning:send count max");
		else
			write_netlog("error:buf pool full");
		close_client( client_index );
		return -1;
	}

#ifdef _USEGPK
	nCodeIdx = lib.GPKGetRandIdx( g_pSvrDynCode );
	nCodeLen = lib.GPKGetCltDynCode( g_pSvrDynCode, nCodeIdx, &pCode );

	if(nCodeLen < 0)
	{
		write_netlog("GPK: nCodeLen < 0");
		mmux_unlock( g_sbuf_mmux );
		return -1;
	}
#endif

	ptr = g_pBuf[buf_index].buf;
	bufsize = 0;
	*(unsigned short *)(ptr + bufsize) = 36; bufsize += sizeof(unsigned short); // 数据长度
	*(unsigned short *)(ptr + bufsize) = 2015; bufsize += sizeof(unsigned short); // 协议编号
	*(unsigned short *)(ptr + bufsize) = 32; bufsize += sizeof(unsigned short); // 协议长度
	*(unsigned int *)( ptr + bufsize ) = ((rand()<<1)|err_code); bufsize += sizeof(unsigned int);
	*(unsigned int *)( ptr + bufsize ) = g_sockset[client_index].m_recv_key; bufsize += sizeof(unsigned int);
	*(unsigned int *)( ptr + bufsize ) = g_sockset[client_index].m_send_key; bufsize += sizeof(unsigned int);
	*(unsigned int *)( ptr + bufsize ) = nCodeLen; bufsize += sizeof(unsigned int);
	*(unsigned int *)( ptr + bufsize) = 0; bufsize += sizeof(unsigned int);
	*(unsigned int *)( ptr + bufsize) = 0; bufsize += sizeof(unsigned int);
	*(unsigned int *)( ptr + bufsize) = 0; bufsize += sizeof(unsigned int);
	*(unsigned int *)(ptr + bufsize) = (unsigned int)time(NULL); bufsize += sizeof(unsigned int); // 服务器时间戳
	g_pBuf[buf_index].next = -1;
	g_sockset[client_index].m_pass_key = (char)g_sockset[client_index].m_send_key;
	g_sockset[client_index].m_pass_key |= 0x80;
	mmux_unlock( g_sbuf_mmux );

//	printf_msg( "sending[%d]...\n", 16 );

	// 锁住这个客户的发送缓冲区
	mmux_lock( g_sockset[client_index].m_mmux );
	// 把buf_index放到客户的的发送缓冲区中
	if( g_sockset[client_index].m_send_tail < 0 )
	{
		// 客户的发送区是空的, 将他挂在开始的位置
		g_sockset[client_index].m_send_head = buf_index;
		g_sockset[client_index].m_send_tail = buf_index;
		g_sockset[client_index].m_send_count = 1;
	}
	else
	{
		// 第一次发送不应该有数据
		mmux_unlock( g_sockset[client_index].m_mmux );
		// 既然不用就应该退还申请空间
		free_onesbuf( buf_index );
		close_client( client_index );
		write_netlog("first send,but has data!");
		return -1;
	}
	if( g_sockset[client_index].m_send_buf.m_nTotalBytes <= 0 )
		fill_send_buf( client_index );
	g_sockset[client_index].m_gpk_key = nCodeIdx;
	mmux_unlock( g_sockset[client_index].m_mmux );

#ifdef _USEGPK
	net_send_long_packeg( client_index, (char *)pCode, nCodeLen );
#endif
	return 0;
}

#ifdef WIN32
//--------------------------------------------------------------------------------------
// 完成端口的初始化
//--------------------------------------------------------------------------------------
int iocp_init()
{
	TRACE_LOG( );
	g_hIOCP = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if( g_hIOCP == NULL )
	{
		printf_msg( "Create IOCP failed! [%d]\n", GetLastError());
		return -1;
	}
	return 0;
}
#else
//--------------------------------------------------------------------------------------
// Epoll的初始化
//--------------------------------------------------------------------------------------
int epoll_init()
{
	int epfd;
	struct rlimit rl;
	g_maxfd = MAX_FD;
	if( getrlimit( RLIMIT_NOFILE, &rl ) == 0 && rl.rlim_cur != RLIM_INFINITY )
		g_maxfd = rl.rlim_cur;
	if( g_maxfd < 8192 )
	{
		rl.rlim_cur = 8192;
		if( setrlimit( RLIMIT_NOFILE, &rl ) == 0 )
			g_maxfd = rl.rlim_cur;
	}

	printf_msg( "epoll maxfd=%d\n", g_maxfd );

	if( ( epfd = epoll_create( g_maxfd ) ) == -1 )
	{
		write_netlog( "Error:epoll_create" );
		return -1;
	}

	g_events = (struct epoll_event *)malloc( g_maxfd * sizeof(struct epoll_event) );
	if( g_events == NULL )
	{
		write_netlog( "Error:epoll_init->malloc" );
		return -1;
	}
	return epfd;
}
#endif
//--------------------------------------------------------------------------------------
// 服务器系统初始化, 主线程调用
//--------------------------------------------------------------------------------------
// #GATECHECK
int server_init( unsigned int nIP, unsigned short port, int max_connection, unsigned int client_ver )
{
	TRACE_LOG( );
#ifdef _USEGATE
	if( g_bGateMode )
		return gs_init( nIP, port, max_connection, client_ver );
#endif
	int tmpi;
//	g_fp = fopen( "send.dat", "wb" );
#ifdef WIN32
	SYSTEM_INFO systemInfo;
#endif
	g_curauthid = 0;

	if( nIP == 0 )
		g_ipaddress = INADDR_ANY;
	else
		g_ipaddress = nIP;

	if( port == 0 )
		g_port = SERV_PORT;
	else
		g_port = port;

	g_max_connection = max_connection;
	g_client_ver.value = client_ver;

	g_nReadQueueHead = 0;
	g_nReadQueueTail = 0;

#ifdef WIN32
	// 获得工作线程数量
	GetSystemInfo(&systemInfo);
	g_dwThreadCount = systemInfo.dwNumberOfProcessors * 2;
#else
	// socket映射到client_index的表
	g_fd_map = (int *)malloc( sizeof(int)*MAX_FD );
	for( tmpi = 0; tmpi < MAX_FD; tmpi++ )
	{
		g_fd_map[tmpi] = -1;
	}
#endif
	// 创建发送池
	g_pBuf = (SBufPool *)malloc( sizeof(SBufPool)*MAX_SBUF_COUNT );
	for( tmpi = 0; tmpi < MAX_SBUF_COUNT; tmpi++ )
	{
		g_pBuf[tmpi].buf_size = 0;
		g_pBuf[tmpi].next = -1;
	}
	// 创建发送池的互斥体
	g_sbuf_mmux = mmux_open();

	// 分配接收队列内存空间
	read_queue = (SReadQueue *)malloc( sizeof(SReadQueue)*MAX_QUEUE_NUM );
	printf_msg( "SReadQueue  memory=%0.2fMB\n", (sizeof(SReadQueue)*MAX_QUEUE_NUM) / 1024.0 / 1024.0 );

	// 分配socket集合的内存空间
	g_sockset = (SOCKET_SET *)malloc( sizeof(SOCKET_SET)*g_max_connection );
	for( tmpi = 0; tmpi < g_max_connection; tmpi++ )
	{
		g_sockset[tmpi].m_fd = INVALID_SOCKET;
		g_sockset[tmpi].m_stat = -1;
		g_sockset[tmpi].m_iswait = 0;
		g_sockset[tmpi].m_recv_buf.m_pBuffer = (char *)malloc( MAX_BUF_SIZE );
		g_sockset[tmpi].m_send_buf.m_pBuffer = (char *)malloc( MAX_BUF_SIZE );
		g_sockset[tmpi].m_mmux = mmux_open();
		g_sockset[tmpi].m_send_head = -1;
		g_sockset[tmpi].m_send_tail = -1;
		g_sockset[tmpi].m_send_count = 0;
		g_sockset[tmpi].m_recv_count = 0;
		memset( g_sockset[tmpi].m_dev, 0, sizeof(char *)*DEVDATA_COUNT );
	}
	// 创建接收队列的互斥体
	g_read_mmux = mmux_open();
#ifdef WIN32
	for( tmpi = 0; tmpi < MAX_WORKER_THREAD; tmpi++ )
		g_pthr_worker[tmpi] = mthr_open();
#endif
	// 创建接收线程对象
	g_pthr_recv = mthr_open();
	// 初始化接收线程对象同步变量
	mcond_init( g_pthr_recv );

#ifdef WIN32
	// 完成端口初始化
	if( iocp_init() < 0 )
		return -1;
#else
	// Epoll初始化
	g_epfd = epoll_init();
	if( g_epfd < 0 )
		return -1;
#endif
	memset( &g_net_protect, 0, sizeof(NetProtect) );
	g_net_protect.iplist_mutex = mmux_open();

	// 创建接收线程
	mthr_create( g_pthr_recv,(void *)recv_proc,0 );

#ifdef WIN32
	// 创建完成端口的工作线程
	for( tmpi = 0; tmpi < (int)g_dwThreadCount; tmpi++ )
	{
		if( mthr_create( g_pthr_worker[tmpi], work_proc, g_hIOCP ) < 0 )
		{
			printf_msg("CreateThread() failed to create worker thread: %d\n", GetLastError());
			break;
		}
	}
#endif
	// 开始监听
	if( create_listen() < 0 )
		g_bExit = 1;

#ifdef _USEGPK
	if ( InitGPKServer() < 0 )
		g_bExit = 1;
#endif
	g_bServerIsInit = 1;
	return 0;
}
// #GATECHECK
void server_exit()
{
	TRACE_LOG( );
#ifdef _USEGATE
	if( g_bGateMode )
		return gs_exit();
#endif
	g_bExit = 1;
	write_netlog( "g_bExit is Set." );
	if( listenfd != INVALID_SOCKET )
	{
#ifdef WIN32
		closesocket(listenfd);
#else
		close(listenfd);
#endif
		listenfd = INVALID_SOCKET;
	}
}

#ifdef WIN32
BOOL WINAPI server_event(DWORD CEvent)
{
	TRACE_LOG( );
	switch( CEvent )
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
//	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		{
			server_exit();
		}
		return TRUE;
	}
	return FALSE;
}
#else
void sig_exit( int signo )
{
//	int sleep_count;
	server_exit();

//	sleep(1);
//	printf_msg("Exiting...[g_bExit:%d-g_bIsDown:%d]\n", g_bExit, g_bIsDown );
//	sleep(1);
}

#endif

//--------------------------------------------------------------------------------------
//  服务器系统销毁, 主线程调用
//--------------------------------------------------------------------------------------
// #GATECHECK
int server_destroy()
{
	TRACE_LOG( );
#ifdef _USEGATE
	if( g_bGateMode )
		return gs_destroy();
#endif

	int tmpi;
	// 设置退出标志
	g_bExit = 1;
	// 终止监听
	write_netlog( "server destroy begin [g_extcode:%d] [g_last_cmd:%d]", g_extcode, g_last_cmd );
	if( listenfd != INVALID_SOCKET )
	{
#ifdef WIN32
		closesocket(listenfd);
#else
		close( listenfd );
#endif
		listenfd = INVALID_SOCKET;
	}
#ifdef WIN32
	// 完成端口的销毁
	if( g_hIOCP )
	{
		for( tmpi = 0; tmpi < (int)g_dwThreadCount; tmpi++ )
			PostQueuedCompletionStatus( g_hIOCP, 0, 0, NULL );
	}
	// 中止工作线程
	for( tmpi = 0; tmpi < (int)g_dwThreadCount; tmpi++ )
	{
		mthr_join( g_pthr_worker[tmpi] );
		mthr_close( g_pthr_worker[tmpi] );
	}
	// 关闭完成端口的句柄
	if( g_hIOCP )
	{
		CloseHandle(g_hIOCP);
		g_hIOCP = NULL;
	}
#else
	// Epoll句柄的关闭
	if( g_epfd >= 0 )
	{
		close( g_epfd );
		g_epfd = -1;
	}
	write_netlog( "epoll closed." );
#endif
	if( g_pCallbackOnDestory )
		g_pCallbackOnDestory();
	write_netlog( "CallbackOnDestoryed." );
	// 释放每个socket的缓存
	for( tmpi = 0; tmpi < g_max_connection; tmpi++ )
	{
		close_client( tmpi );
		if( g_sockset[tmpi].m_recv_buf.m_pBuffer )
			free( g_sockset[tmpi].m_recv_buf.m_pBuffer );
		if( g_sockset[tmpi].m_send_buf.m_pBuffer )
			free( g_sockset[tmpi].m_send_buf.m_pBuffer );
	}
	write_netlog( "all client closed." );
	// 释放内存池
	if( g_pBuf )
	{
		free( g_pBuf );
		g_pBuf = NULL;
	}
	write_netlog( "g_pBuf freed." );

	mcond_broadcast( g_pthr_recv );
	// 关闭接收线程
	mthr_join( g_pthr_recv );
	mthr_close( g_pthr_recv );

	write_netlog( "pthr_recv closed." );
	// 销毁互斥体
	mmux_close( g_sbuf_mmux );
	mmux_close( g_read_mmux );

	mmux_close( g_net_protect.iplist_mutex );
	// 销毁同步变量 mthr_close( g_pthr_recv );已经完成了这个工作
	//mcond_destroy( g_pthr_recv );

	write_netlog( "server destroyed." );
	return 0;
}

//--------------------------------------------------------------------------------------
// 服务启动
//--------------------------------------------------------------------------------------
// #GATECHECK
int server_start( LPCALLBACK_PROCESSPACK pCallbackProcessPack, LPCALLBACK_DISCONNECTED pCallbackDisConnected, LPCALLBACK_ONERROR pCallbackOnError, LPCALLBACK_ONDESTORY pCallbackOnDestory )
{
	TRACE_LOG( );
#ifdef _USEGATE
	if( g_bGateMode )
		return gs_start( pCallbackProcessPack, pCallbackDisConnected, pCallbackOnError, pCallbackOnDestory );
#endif

	SOCKET connfd;
	int client_index;
#ifdef WIN32
	int nRet;
	DWORD dwRecvNumBytes = 0;
	DWORD dwFlags = 0;
#else
	int tmpi;
	int nfds;
	struct epoll_event ev;
	int sockfd;
#endif
	socklen_t clilen;
	struct sockaddr_in clientaddr;

	if( pCallbackProcessPack == NULL || pCallbackDisConnected == NULL || pCallbackOnDestory == NULL )
		return -1;

	g_pCallbackProcessPack = pCallbackProcessPack;
	g_pCallbackDisConnected = pCallbackDisConnected;
	g_pCallbackOnError = pCallbackOnError;
	g_pCallbackOnDestory = pCallbackOnDestory;

#ifdef WIN32
	// 设置退出进程
	if (SetConsoleCtrlHandler( (PHANDLER_ROUTINE)server_event, TRUE) == FALSE )
	{
		printf_msg("Unable to install event handler!\n");
		return -1;
	}

	while( !g_bExit )
	{
		clilen = sizeof(clientaddr);
		connfd = WSAAccept( listenfd, (struct sockaddr *)&clientaddr, &clilen, NULL, 0 );
		if( connfd < 0 )
		{
			printf_msg( "Accept Error! [%d]\n",WSAGetLastError() );
			write_netlog( "Accept Error! [%d]",WSAGetLastError() );
			continue;
		}
// 		if( check_forbid_ip( &g_net_protect, clientaddr.sin_addr.s_addr ) != 0 )
// 		{
// 			shutdown( connfd, 2 );
// 			closesocket( connfd );
// 			write_netlog( "bad error: [%s] in forbid list ", inet_ntoa(clientaddr.sin_addr) );
// 			continue;
// 		}
		// g_sockset;
		client_index = update_fdmap( connfd, inet_ntoa(clientaddr.sin_addr) );
		if( client_index < 0 )
		{
			// 人数可能到达上限了
			shutdown( connfd, 2 );
			closesocket( connfd );
			write_netlog( "update_fdmap < 0" );
			continue;
		}
		g_connectioncount++;

		// 触发完成端口的读
		nRet = WSARecv( connfd, &(g_sockset[client_index].m_recv_buf.m_wsabuf), 1, &dwRecvNumBytes, &dwFlags,
			&(g_sockset[client_index].m_recv_buf.m_Overlapped), NULL);
		if( nRet == SOCKET_ERROR && ( WSAGetLastError() != ERROR_IO_PENDING ) )
		{
			write_netlog( "WSARecv() Failed: %d\n", WSAGetLastError() );
			close_client( client_index );
		}
	} //while
#else
	// 设置退出方式
	signal( SIGHUP, sig_exit );
	signal( SIGTERM, sig_exit );
	signal( SIGINT, sig_exit );
	signal( SIGKILL, sig_exit );

	// 循环获取客户端连接
	while( !g_bExit )
	{
		nfds = epoll_wait( g_epfd, g_events, g_maxfd, 30000 );
		if( nfds < 0 || g_bExit )
			break;
		for( tmpi = 0; tmpi < nfds; tmpi++ )
		{
			if( g_events[tmpi].data.fd == listenfd )
			{
				while(1)
				{
					clilen = sizeof(clientaddr);
					connfd = accept( listenfd, (struct sockaddr *)&clientaddr, &clilen );
					if( connfd < 0 )
					{
//						write_netlog( "connfd < 0 [%d]%s", errno, strerror(errno) );
						break;
					}
					if( connfd >= MAX_FD )
					{
						shutdown( connfd, 2 );
						close( connfd );
						write_netlog( "connfd >= MAX_FD [fd:%d]", connfd );
						break;
					}
					// 检查该连接是否存在于禁止列表中
// 					if( check_forbid_ip( &g_net_protect, clientaddr.sin_addr.s_addr ) != 0 )
// 					{
// 						shutdown( connfd, 2 );
// 						close( connfd );
// 						write_netlog( "bad error: [%s] in forbid list ", inet_ntoa(clientaddr.sin_addr) );
// 						continue;
// 					}
					setnonblocking( connfd );
					client_index = update_fdmap( connfd, inet_ntoa(clientaddr.sin_addr) );
					if( client_index < 0 )
					{
						// 人数可能到达上限了
						shutdown( connfd, 2 );
						close( connfd );
						write_netlog( "update_fdmap < 0" );
						break;
					}

//					char *str = inet_ntoa(clientaddr.sin_addr);
					ev.events = EPOLLIN | EPOLLET;
					ev.data.fd = connfd;
					if( epoll_ctl( g_epfd, EPOLL_CTL_ADD, connfd, &ev ) < 0 )
					{
						struct epoll_event ev = {0};
						write_netlog( "epoll set insertion error: fd=%d", connfd );
						epoll_ctl( g_epfd, EPOLL_CTL_DEL, connfd, &ev );
						close( connfd );
//						return -1;
					}
					++g_connectioncount;
//					write_netlog( "accept: %d", connfd );
				}
			}
			else
			{
				sockfd = g_events[tmpi].data.fd;
				if ( sockfd < 0 || sockfd >= MAX_FD )
				{
					write_netlog("sockfd < 0 [%d]%s",errno,strerror(errno));
					continue;
				}
				if( g_events[tmpi].events & EPOLLIN )
					do_read_client( g_fd_map[sockfd] );
				if( g_events[tmpi].events & EPOLLOUT )
					do_write_client( g_fd_map[sockfd] );
			}
		}
	} //while
	g_bIsDown = 1;
	write_netlog( "[thread]ServerMain Proc Exited.\n" );
#endif

	return 0;
}

int add_forbid_ip( NetProtect *np, char *ip )
{
	int tmpi = 0;
	time_t now = (int)time(NULL);
	long l_ip = 0;
	if( !np )
		return 0;
	if( ! np->iplist_mutex || !ip )
		return 0;
	l_ip = inet_addr( ip );
	
	mmux_lock(np->iplist_mutex);
	for( tmpi = 0; tmpi < FORBID_IP_MAX; tmpi ++ )
	{
		if( np->forbid_ip_list[tmpi] <= 0 )
		{
			np->forbid_ip_list[tmpi] = l_ip;
			np->forbid_time[tmpi] = now;
			mmux_unlock( np->iplist_mutex );
			return 0;
		}
	}
	np->all_forbid_time = now;
	mmux_unlock( np->iplist_mutex );
	return 0;
}

int check_forbid_ip( NetProtect *np, int ip )
{
	int tmpi = 0;
	time_t now = (int)time(NULL);
	if( !np )
		return 0;
	if( ! np->iplist_mutex )
		return 0;
	
	mmux_lock( np->iplist_mutex );
	if( FORBID_ALL_TIME > now - np->all_forbid_time )
	{
		mmux_unlock( np->iplist_mutex );
		return -1;
	}
	np->all_forbid_time = 0;
	for( tmpi = 0; tmpi < FORBID_IP_MAX; tmpi ++ )
	{
		if( now - np->forbid_time[tmpi] > FORBID_ALL_TIME )
		{
			np->forbid_time[tmpi] = 0;
			np->forbid_ip_list[tmpi] = 0;
		}
		if( np->forbid_ip_list[tmpi] == ip )
		{
			mmux_unlock( np->iplist_mutex );
			return -1;
		}
	}
	mmux_unlock( np->iplist_mutex );
	return 0;
}
// #GATECHECK
void sendtoclient( int actor_index, char *tmpbuf, int size )
{
	TRACE_LOG( );
#ifdef _USEGATE
	if( g_bGateMode )
	{
		gs_sendtoqueue( actor_index, tmpbuf, size );
		return;
	}
#endif
	//printf_msg( "send to client package_size:%d client_index:%d\n", size, actor_index );
	//write_netlog( "size:%d, %d", size, *(short *)(tmpbuf+2) );
	sendtoqueue( actor_index, tmpbuf, size );
}
