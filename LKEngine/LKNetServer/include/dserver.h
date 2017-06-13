
#ifndef __DSERVER_H
#define __DSERVER_H

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
//#include <mswsock.h>
#endif
#include "utils.h"

#define DSERV_PORT			8999
#define LISTEN_QUEUE		20				// 监听队列长度

#define MAX_DPACK_LEN		2048
#define MAX_SENDRESULT_LEN	81920
#define MAX_OUTRESULT_LEN	81920

typedef int ( *LPCALLBACK_DPROCESS)( short cmd, int arg1, int arg2, int arg3, int arg4, char *str );

int d_init();
int d_listen();
int d_process( SOCKET fd );
int d_newclient( SOCKET connfd, const char *szIP );
int d_start( LPCALLBACK_DPROCESS pCallbackDProcess, unsigned int nIP, unsigned short port );

#endif
