#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <signal.h>
#include <syslog.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/ipc.h>   
#include <sys/msg.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include "netserver.h"

#include "mytimer.h"
#include "netfunc.h"
#include "gameproc.h"
#include "utils.h"
#include "define.h"

const char *GetRootPath();

extern SConfig g_Config;

extern SOCKET_SET *g_sockset;
extern int g_max_connection;

char g_szWorkPath[MAX_PATH];
int fps_limit = 30;

// 设置当前服务器状态，用于鉴定当前服务器状态
int serv_setstat( int stat )
{
	char szStatFile[MAX_PATH];
	sprintf( szStatFile, "%s/.stat", GetRootPath() );

	FILE *fp;
	fp = fopen( szStatFile, "wb" );
	if( fp == NULL )
		return -1;

	fprintf( fp, "%d\r\n", stat );
	fclose( fp );
	return 0;
}

#ifndef WIN32
#define LOCKFILE "gh.pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

int lockfile( int ff )
{
	struct flock fl;
	fl.l_type = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;
	return ( fcntl(ff,F_SETLK,&fl) );
}

void already_running()
{
	int fd;
	char buf[16];
	char szLockFile[MAX_PATH];

	sprintf(szLockFile, "%s/%s", GetRootPath(), LOCKFILE );

	fd = open( szLockFile,O_RDWR|O_CREAT,LOCKMODE );
	if( fd < 0 )
	{
		printf( "already running.[1]\n" );
		exit(1);
	}

	if( lockfile(fd) < 0 )
	{
		if( errno == EACCES || errno == EAGAIN )
		{
			close( fd );
			printf( "already running.[2]\n" );
			exit(1);
		}
		printf( "already running.[3]\n" );
		exit(1);
	}
	ftruncate( fd, 0 );

	sprintf( buf,"%d", getpid() );
	write( fd, buf, strlen(buf) );
}

int getmypid()
{
	int fd;
	int size;
	char buf[16];
	char szLockFile[MAX_PATH];

	sprintf(szLockFile, "%s/%s", GetRootPath(), LOCKFILE );

	fd = open( szLockFile,O_RDWR|O_CREAT,LOCKMODE );
	if( fd < 0 )
		return -1;

	if( lockfile(fd) < 0 )
	{
		if( errno == EACCES || errno == EAGAIN )
		{
			size = read( fd, buf, 16 );
			buf[size] = 0;
			close(fd);
			return atoi(buf);
		}
		size = read( fd, buf, 16 );
		buf[size] = 0;
		close(fd);
		return atoi(buf);
	}
	return -1;
}

// linux 消息队列
struct linuxmsg_info
{
	long int msg_type;
};
#define LINUXMSG_KEY 198800
int linuxmsg_send( int type )
{
	// 检查消息队列是否存在,不存在就创建
	int linuxmsgid = msgget( LINUXMSG_KEY, IPC_EXCL );
	if ( linuxmsgid < 0 )
	{
		linuxmsgid = msgget( LINUXMSG_KEY, 0666 | IPC_CREAT );
		if ( linuxmsgid < 0 )
		{
			printf_msg( "msgget failed with error: %d\n", errno );
			return -1;
		}
	}

	struct linuxmsg_info msginfo;
	msginfo.msg_type = type;
	if ( msgsnd( linuxmsgid, &msginfo, sizeof(struct linuxmsg_info), IPC_NOWAIT ) < 0 )
	{
		printf_msg( "msgsnd() write msg failed,errno=%d[%s]\n", errno, strerror( errno ) );
		return -1;
	}

	printf_msg( "linuxmsg_send linuxmsgid:%d\n", linuxmsgid );
	return 0;
}
int linuxmsg_recv()
{
	int linuxmsgid = msgget( LINUXMSG_KEY, IPC_EXCL );
	if ( linuxmsgid < 0 )
	{
		//printf_msg( "linuxmsg_recv linuxmsgid:%d\n", linuxmsgid );
		return -1;
	}

	struct linuxmsg_info msginfo;
	if ( msgrcv( linuxmsgid, &msginfo, sizeof(struct linuxmsg_info), 0, IPC_NOWAIT ) == -1 )
	{
		return -1;
	}

	printf_msg( "linuxmsg_recv msginfo.msg_type:%d\n", msginfo.msg_type );
	if ( msginfo.msg_type == 1 )
	{ // 查看
		for( int client_index = 0; client_index < g_max_connection; client_index++ )
		{
			if( g_sockset[client_index].m_fd != INVALID_SOCKET )
			{
				printf_msg( "%d	%s\n", (int)client_getuserid(client_index), client_getip( client_index ) );
				write_netlog( "%d	%s", (int)client_getuserid(client_index), client_getip( client_index ) );
			}
		}
		
	}

	// 用完就删除消息队列
	if ( msgctl( linuxmsgid, IPC_RMID, 0 ) == -1 )
	{
		printf_msg( "msgctl(IPC_RMID) failed,errno=%d[%s]\n", errno, strerror( errno ) );

		//char cmdMsg[64]={0};
		//sprintf( cmdMsg, "ipcrm -Q %d", LINUXMSG_KEY );
		//system( cmdMsg );
	}
	return 0;
}

#endif

int configprocess( const char *pSign, const char *pValue )
{
	if( strcmp( pSign, "SERVER_HOST" ) == 0 )
	{
		g_Config.ipaddress = inet_addr( pValue );
	}
	else if( strcmp( pSign, "SERVER_PORT" ) == 0 )
	{
		g_Config.port = atoi(pValue);
	}
	else if( strcmp( pSign, "MAX_CONNECTION" ) == 0 )
	{
		g_Config.max_connection = atoi(pValue);
	}

	return 0;
}

#define CMDC_TIMER  -1;
int netsend_timer_S( int actor_index, char send_type, char *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr;
	short cmd = CMDC_TIMER;

	if ( actor_index >= 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr += sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDC_TIMER; ptr += sizeof(short); tmpsize += sizeof(short);

	LKSET_SBYTE_SEND( ptr, pValue, tmpsize );

	*(unsigned short *)tmpbuf = tmpsize;

	readtoqueue( actor_index, tmpbuf, tmpsize + sizeof(short), 0 );
	return 0;
}

// 计时器发送指令
void target_incrementor()
{
	char ch = 0;
	netsend_timer_S( -2, 0, &ch );
}

// 这里截获崩溃吧
void sig_process( int signo )
{
#ifndef WIN32
	// 先恢复信号为缺省，免得死循环进入了
    signal( SIGSEGV, SIG_DFL );
    signal( SIGBUS, SIG_DFL );
    signal( SIGFPE, SIG_DFL );
	signal( SIGABRT, SIG_DFL );
    printf("System core signal:%d\n",signo);
#endif
}

int main( int argc, char *argv[] )
{
	char isoutput = 0;
	const char *rootpath;
#ifndef WIN32
	int childpid;//, fd, fdtablesize;
//	int fp;
	int waittimes;
	chdir( GetRootPath() );
#else
	SetCurrentDirectory( GetRootPath() );
#endif
	rootpath = GetRootPath();
	if( rootpath )
		strcpy( g_szWorkPath, rootpath );
	else
		g_szWorkPath[0] = 0;

	if( argc > 1 )
	{
		if( strncmp( argv[1], "-o", 2 ) == 0 )
			isoutput = 1;
#ifndef WIN32
		else if( strncmp( argv[1], "start", 5 ) == 0 )
		{
			// 开始启动
			serv_setstat( 3 );

			signal(SIGTTOU,SIG_IGN);
			signal(SIGTTIN,SIG_IGN);
			signal(SIGTSTP,SIG_IGN);
			signal(SIGHUP,SIG_IGN);

			if( fork() != 0 )
			{
				sleep(8);
				exit(1);
			}

			already_running();
		}
		else if( strncmp( argv[1], "stop", 4 ) == 0 )
		{
			// 开始关闭
			serv_setstat( 4 );

			childpid = getmypid();
			if( childpid > 0 )
			{
				waittimes = 10;
				kill( childpid, SIGHUP );
				while( waittimes > 0 )
				{
					sleep(1);
					childpid = getmypid();
					if( childpid <= 0 )
					{
						printf( "Server Stop.\n" );
						// 已经关闭
						serv_setstat( 2 );
						return 0;
					}
				}
				printf( "Server Cannot Stop.\n" );
				// 关闭失败
				serv_setstat( 6 );
			}
			else
			{
				// 已经关闭
				serv_setstat( 2 );
				printf( "no run pid:%d\n", -1 );
				return 0;
			}
			return -1;
		}
		else if ( strncmp( argv[1], "v", 1 ) == 0 )
		{
			childpid = getmypid();
			if( childpid > 0 )
			{
				linuxmsg_send( 1 );
			}
			else
			{
				// 已经关闭
				// serv_setstat( -20 );
				printf_msg( "no run pid:%d\n", -1 );
				return 0;
			}
			return -1;
		}
#endif

	}
#ifdef WIN32
	g_Config.max_connection = 512;
	// 开始启动
	serv_setstat( 3 );
#else
	g_Config.max_connection = 512;
	// 设置退出方式
	signal( SIGSEGV, sig_process );
	signal( SIGBUS, sig_process );
	signal( SIGFPE, sig_process );
	signal( SIGABRT, sig_process );
#endif
	u_getcfg( "globalserver.cfg", configprocess );
	u_loginit( LOG_PATH );
	open_netlog();
	open_gamelog();
	
	// 随机种子
	srand( (unsigned int)time(NULL) );

	// 服务进程初始化
	if( process_init( g_Config.max_connection ) < 0 )
	{
		// 启动失败
		serv_setstat( 5 );
#ifdef WIN32
		Sleep( 5000 );
#else
		sleep( 5 );
#endif
		return -1;
	}
#ifdef WIN32
	WORD tSocketVerNo;
	WSADATA tVerData;
	tSocketVerNo = MAKEWORD( 2, 0 );
	WSAStartup( tSocketVerNo, &tVerData );
#endif

	if( server_init( g_Config.ipaddress, g_Config.port, g_Config.max_connection, 0 ) < 0 )
	{
#ifdef WIN32
		Sleep( 3000 );
#else
		sleep( 3 );
#endif
		// 启动失败
		serv_setstat( 5 );
		return -1;
	}
	init_timer();
	append_timer( (void *)target_incrementor, 0, 1193181/fps_limit );

	// 基本启动成功了
	serv_setstat( 1 );
	server_start( process_client, process_onclose, process_error, process_ondestory );

	//output_setmsg( "Now Exitting..." );
	exit_timer();
	printf( "Now Exitting...\n" );
	server_destroy();
	process_close();
	u_logcloseall();
	//output_setmsg( "System Exited." );
#ifdef WIN32
		Sleep( 1000 );
#else
		sleep( 1 );
#endif
	printf( "System Exited.\n" );
	// 已经关闭
	serv_setstat( 2 );
	return 0;
}
