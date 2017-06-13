#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
//#include <mswsock.h>
#else
#include <strings.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#endif

#include "dserver.h"
#include "thread.h"

// 目前运作方式:阻塞方式,一个连接创建一个线程


LPCALLBACK_DPROCESS g_pCallbackDProcess = NULL;
SOCKET g_dlistenfd;

// 本地IP地址,监听端口,当前客户版本支持
unsigned int g_d_ipaddress = INADDR_ANY;
short g_d_port = DSERV_PORT;

Mthr *pthr[64];
Mthr *pthr_listen;
char *g_pIP[64];
int g_IPcount = 0;
char g_gm_sendresult[MAX_SENDRESULT_LEN] = { 0 };
char g_gm_outresult[MAX_OUTRESULT_LEN] = { 0 };
extern char g_bExit;

int d_init()
{
	FILE *fp;
	char szIP[128];
	char *ptr;
	char *begin;
	char flag;
	int tmpi;
	g_IPcount = 0;

	pthr_listen = mthr_open();
	fp = fopen( "d_list.cfg", "rb" );
	if( fp == NULL )
	{
		printf_msg( "Warnning! Can not open d_list.cfg file, Dserver will allow all ip address.\n" );
		for( tmpi = 0; tmpi < 64; tmpi++ )
			pthr[tmpi] = mthr_open();
		return d_listen();
	}
	while( !feof( fp ) )
	{
		if( fgets( szIP, 128, fp ) == NULL )
			break;
		flag = 0;
		begin = NULL;
		for( ptr = szIP; *ptr != 0; ptr++ )
		{
			if( flag == 0 && ( *ptr == '.' || ( *ptr >= 0x30 && *ptr <= 0x39 ) ) )
			{
				flag = 1;
				begin = ptr;
			}
			else if( flag && ( *ptr != '.' && ( *ptr < 0x30 || *ptr > 0x39 ) ) )
			{
				*ptr = 0;
				break;
			}
		}
		if( begin )
		{
			g_pIP[g_IPcount] = u_strdup( begin );
			g_IPcount++;
			if( g_IPcount >= 64 )
				break;
		}
	}
	fclose( fp );

	for( tmpi = 0; tmpi < g_IPcount; tmpi++ )
		pthr[tmpi] = mthr_open();

	return d_listen();
}

// 创建一个监听端口, 主线程调用
int d_listen()
{
#ifndef WIN32
	int on;
	struct linger linger = { 0 };
#endif
	struct sockaddr_in serveraddr;

	g_dlistenfd = socket( AF_INET, SOCK_STREAM, 0 );
#ifndef WIN32
	// 允许地址重用
	on = 1;
	setsockopt( g_dlistenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );

	// 当连接中断时，需要延迟关闭(linger)以保证所有数据都传输
	// l_onoff为0，延迟关闭特性就被取消，反之允许套接字关闭延迟一段时间，l_linger指明延迟的时间(秒，bsd下为百分之一秒)
	linger.l_onoff = 1;
	linger.l_linger = 10;
	setsockopt( g_dlistenfd, SOL_SOCKET, SO_LINGER, (const char *) &linger, sizeof(linger) );

#endif
	memset( &serveraddr, 0, sizeof(serveraddr) );

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = g_d_ipaddress;
	serveraddr.sin_port = htons( g_d_port );

	if( bind( g_dlistenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) != 0 )
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
	if( listen( g_dlistenfd, LISTEN_QUEUE ) != 0 )
	{
		printf_msg( "listen error!\n" );
		printf_msg( "Now exiting...\n" );
#ifdef WIN32
		Sleep( 5000 );
#else
		sleep( 5 );
#endif
		return -1;
	}
	return 0;
}

void d_closesock( SOCKET s )
{
	if( s < 0 )
		return;
	shutdown( s, 2 );
#ifdef WIN32
	closesocket(s);
#else
	close(s);
#endif
}

// 发送包：short(0) 表示短连接并且返回人数
// 发送包：short(9999) 表示需要长连接
// 发送包：short(cmd) | arg1 | arg2 | arg3 | arg4 | short(str len) | string
// 返回包：int(result)
int d_process( SOCKET fd )
{
	int packsize;
	int readsize;
	int writesize;
	char recv_buf[MAX_DPACK_LEN];
	char *ptr;
	char *ptr_header;
	char *string;
	char flag;
	char exitf;
	int tmpi;

	char long_link = 0;
	short cmd;
	int arg[4];
	short slen;
	int result;

	for(;;)
	{
		slen = 0;
		memset( arg, 0, sizeof(int)*4 );
		string = NULL;
		result = 0;

		packsize = 0;
		while( packsize < MAX_DPACK_LEN )
		{
			if ( ( readsize = recv( fd, recv_buf+packsize, MAX_DPACK_LEN-packsize, 0 ) ) <= 0 )
			{
				// 连接断开了(连接重置了)
				d_closesock( fd );
#ifdef WIN32
				write_netlog( "recv[%d]", fd );
#else
				write_netlog( "recv[%d] %d-%s", fd, errno,strerror(errno) );
#endif
				return -1;
			}
			packsize += readsize;
			if( packsize >= MAX_DPACK_LEN )
			{
				// 连接断开
				d_closesock( fd );
				write_netlog( "recv packeg too long[%d]", fd );
				return -1;
			}
			if( recv_buf[packsize-1] == 0 )
			{
				break;
			}
		}
		ptr_header = ptr = recv_buf;
		flag = 0;
		exitf = 0;
		for( tmpi = 0; tmpi < MAX_DPACK_LEN; tmpi++ )
		{
			if( *ptr == '|' || *ptr == 0 )
			{
				if( *ptr == 0 )
					exitf = 1;
				else
					*ptr = 0;
				if( flag == 0 )
				{
					cmd = (short)atoi( ptr_header );
				} 
				else if( flag == 1 )
				{
					arg[0] = atoi( ptr_header );
				}
				else if( flag == 2 )
				{
					arg[1] = atoi( ptr_header );
				}
				else if( flag == 3 )
				{
					arg[2] = atoi( ptr_header );
				}
				else if( flag == 4 )
				{
					arg[3] = atoi( ptr_header );
					if( ptr+1 - recv_buf < packsize )
						string = ptr+1;
					break;
				}
				flag++;
				ptr_header = ptr+1;
				if( exitf == 1 )
					break;
			}
			ptr++;
		}
		if( cmd == 0 )
		{
			long_link = 0;
		}
		else if( cmd == 9999 )
		{
			long_link = 1;
			continue;
		}

		if( g_pCallbackDProcess )
		{
			result = g_pCallbackDProcess( cmd, arg[0], arg[1], arg[2], arg[3], string );
		}

		// 返回的内容
		memset( g_gm_sendresult, 0, MAX_SENDRESULT_LEN );
		sprintf( g_gm_sendresult, "%d|%s", result, g_gm_outresult );
		int resultlen = (int)strlen( g_gm_sendresult ) + 1;
		resultlen = resultlen >= MAX_SENDRESULT_LEN ? MAX_SENDRESULT_LEN : resultlen;

		packsize = 0;
		while( packsize < resultlen )
		{
			if ( ( writesize = send( fd, g_gm_sendresult+packsize, resultlen-packsize, 0 ) ) <= 0 )
			{
				// 连接断开了(连接重置了)
				d_closesock( fd );
				write_netlog( "d_process:send error[%d]", fd );
				return -1;
			}
			packsize += writesize;
		}
		g_gm_outresult[0] = 0;
		if( long_link == 0 )
			break;

	}
	d_closesock( fd );
	return 0;
}

int d_newclient( SOCKET connfd, const char *szIP )
{
	int tmpi;
	int index = -1;
#ifndef WIN32
	int on;
	struct linger linger = { 0 };
#endif

	if( g_IPcount > 0 )
	{
		for( tmpi = 0; tmpi < g_IPcount; tmpi++ )
		{
			if( strcmp( szIP, g_pIP[tmpi] ) == 0 )
			{
				index = tmpi;
				break;
			}
		}
		if( index < 0 )
		{
			// 连接断开了(连接重置了)
			d_closesock( connfd );
			write_netlog("Access denied for %s", szIP );
			printf_msg("Access denied for %s\n", szIP );
			return -1;
		}
	}
	// 应该检测连接是否合法
	if( index == -1 )
		index = 63;

	mthr_create( pthr[index], (void*)d_process, (void *)connfd );
	mthr_detach( pthr[index] );
	return 0;
}

int d_accept()
{
	int connfd;
#ifdef WIN32
	int clilen;
#else
	socklen_t clilen;
#endif
	struct sockaddr_in clientaddr;
	char *szIP;

	while( !g_bExit )
	{
		clilen = sizeof(struct sockaddr);
		connfd = (int)accept( g_dlistenfd, (struct sockaddr *)&clientaddr, &clilen );
		if( connfd < 0 )
		{
#ifdef WIN32
			write_netlog( "accept failed (connfd < 0): %d", GetLastError());
#else
			write_netlog( "accept failed (connfd < 0): %d-%s", errno,strerror(errno));
#endif
			continue;
		}
		szIP = inet_ntoa( clientaddr.sin_addr );
		printf_msg("accept: %s\n", szIP );
		d_newclient( connfd, szIP );
		write_netlog("d_accept: %d", connfd );
	}
	return 0;
}

int d_start( LPCALLBACK_DPROCESS pCallbackDProcess, unsigned int nIP, unsigned short port )
{
	if( nIP != 0 )
		g_d_ipaddress = nIP;

	if( port != 0 )
		g_d_port = port;

	// 系统的初始化
	if( d_init() < 0 )
	{
#ifdef WIN32
		Sleep( 3000 );
#else
		sleep( 3 );
#endif
		return -1;
	}
	g_pCallbackDProcess = pCallbackDProcess;
	mthr_create( pthr_listen, (void*)d_accept, NULL );
	return 0;
}
