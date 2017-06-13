#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
//#include <Ws2tcpip.h>
#else
#include <strings.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "netserver.h"
#include "netcode.h"

unsigned int g_holdrand = 0;

extern SOCKET_SET *g_sockset;

void netcode_init()
{
	g_holdrand = (unsigned int)time(NULL);
#ifdef _DEBUG
	g_holdrand = 0;
#endif
}

unsigned int netcode_create()
{
	return (((g_holdrand = g_holdrand * 214015L + 2531077L) >> 16) & 0xffff) | ((g_holdrand = g_holdrand * 214015L + 2531077L) & 0xffff0000);
}

short netcode_getrecvkey( int client_index )
{
//	write_netlog( "recvkey %d", (unsigned short)(( ( g_sockset[client_index].m_recv_key * 214015L + 2531077L) >> 16) & 0xffff ) );
	return (((g_sockset[client_index].m_recv_key = g_sockset[client_index].m_recv_key * 214015L + 2531077L) >> 16) & 0xffff);
}
short netcode_getsendkey( int client_index )
{
//	write_netlog( "sendkey %d", (unsigned short)(( ( g_sockset[client_index].m_send_key * 214015L + 2531077L) >> 16) & 0xffff ) );
	return (((g_sockset[client_index].m_send_key = g_sockset[client_index].m_send_key * 214015L + 2531077L) >> 16) & 0xffff);
}

short netcode_lookrecvkey( int client_index )
{
	return (short)(((g_sockset[client_index].m_recv_key * 214015L + 2531077L) >> 16) & 0xffff);
}

short netcode_looksendkey( int client_index )
{
	return (short)(((g_sockset[client_index].m_send_key * 214015L + 2531077L) >> 16) & 0xffff);
}

void netcode_encode( int client_index, char *dst, const char *src, int size )
{
	int tmpi;
	for( tmpi = 0; tmpi < size; tmpi += 2 )
	{
		if( size - 1 == tmpi )
			*((unsigned char *)(dst+tmpi)) = *((unsigned char *)(src+tmpi)) ^ (unsigned char)netcode_getsendkey( client_index );
		else
			*((unsigned short *)(dst+tmpi)) = *((unsigned short *)(src+tmpi)) ^ (unsigned short)netcode_getsendkey( client_index );
	}
}

void netcode_decode( int client_index, char *dst, const char *src, int size )
{
	int tmpi;
	for( tmpi = 0; tmpi < size; tmpi += 2 )
	{
		if( size - 1 == tmpi )
			*((unsigned char *)(dst+tmpi)) = *((unsigned char *)(src+tmpi)) ^ (unsigned char)netcode_getrecvkey( client_index );
		else
			*((unsigned short *)(dst+tmpi)) = *((unsigned short *)(src+tmpi)) ^ (unsigned short)netcode_getrecvkey( client_index );
	}
}

short netcode_decode_size( int client_index, short datasize )
{
	return datasize ^ netcode_lookrecvkey( client_index );
}
