#ifndef __NETFUNC_H
#define __NETFUNC_H

#ifdef _DEBUG
#define TRACE_LOG() printf( "%s(%d)-<%s>\n", __FILE__, __LINE__, __FUNCTION__ )
#else
#define TRACE_LOG()
#endif

//#define MAX_QUEUE_NUM		8192		// 读写队列长度
#define MAX_QUEUE_NUM		2048		// 服务器缩小－缩小规模
//#define MAX_QUEUE_NUM		256			// 读写队列长度

#define MAX_WRITEQUEUE_LEN	10				// 单个sock的写队列长度
#define MAX_SBUF_COUNT		(MAX_QUEUE_NUM*MAX_WRITEQUEUE_LEN)	// 缓冲区个数,内存池

typedef int ( *LPCALLBACK_PROCESSPACK)( int cmd, char *pack_data, int packlen, int exec_code );
typedef int ( *LPCALLBACK_DISCONNECTED)( int fd );
typedef int ( *LPCALLBACK_ONERROR)( int client_index, int err );
typedef int ( *LPCALLBACK_ONDESTORY)();

int server_init( unsigned int nIP, unsigned short port, int max_connection, unsigned int client_ver );
int server_start( LPCALLBACK_PROCESSPACK pCallbackProcessPack, LPCALLBACK_DISCONNECTED pCallbackDisConnected, LPCALLBACK_ONERROR pCallbackOnError, LPCALLBACK_ONDESTORY pCallbackOnDestory );
#ifdef _USEGATE
int gs_start( LPCALLBACK_PROCESSPACK pCallbackProcessPack, LPCALLBACK_DISCONNECTED pCallbackDisConnected, LPCALLBACK_ONERROR pCallbackOnError, LPCALLBACK_ONDESTORY pCallbackOnDestory );
#endif
int server_destroy();

int readtoqueue( int client_index, char *pBuf, int readsize, char stat  );
int sendtoqueue( int client_index, char *buf, int bufsize );

#endif
