#include <stdio.h>
#include <string.h>
#include <setjmp.h>

#include "gameproc.h"
#include "utils.h"
#include "define.h"
#include "client.h"

#ifndef WIN32 // 这些头文件用来看ulimit设置的
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif

SConfig g_Config;

int g_speed = 0;
int g_sec = 0;	// 秒针

int g_mysql_wait = 0;
int g_save_wait = 0;

int g_last_server_code = 0;

int g_maxactornum = 512;
int g_frame_count;
extern int g_nReadQueueHead;
extern int g_nReadQueueTail;
extern char g_bNoHeader;

#ifndef WIN32
extern int linuxmsg_recv();
#endif

int process_init( int max_connection )
{
	LOGI("%s-%d",__FUNCTION__,__LINE__);
	g_bNoHeader = 1;
#ifndef WIN32
	struct rlimit out_data={0};
	if ( getrlimit( RLIMIT_CORE, &out_data ) == 0 )
	{
		if ( out_data.rlim_cur != -1 )
		{
			out_data.rlim_cur = -1;
			out_data.rlim_max = -1;
			if ( setrlimit( RLIMIT_CORE, &out_data ) != 0 )
			{
				printf_msg( "setrlimit( RLIMIT_CORE, &out_data ) Error!" );
				return -1;
			}
		}
	}
	else
	{
		printf_msg( "getrlimit( RLIMIT_CORE, &out_data ) Error!" );
		return -1;
	}

	if ( getrlimit( RLIMIT_NOFILE, &out_data ) == 0 )
	{
		if ( out_data.rlim_cur < max_connection )
		{
			out_data.rlim_cur = 8196;
			out_data.rlim_max = 8196;
			if ( setrlimit( RLIMIT_NOFILE, &out_data ) != 0 )
			{
				printf_msg( "setrlimit( RLIMIT_NOFILE, &out_data ) Error!" );
				return -1;
			}
		}
	}
	else
	{
		printf_msg( "getrlimit( RLIMIT_NOFILE, &out_data ) Error!" );
		return -1;
	}

	// 再次检测
	if ( getrlimit( RLIMIT_CORE, &out_data ) == 0 )
	{
		if ( out_data.rlim_cur != -1 )
		{
			printf_msg( "getrlimit( RLIMIT_CORE, &out_data ) too small!" );
			return -1;
		}
	}
	else
	{
		printf_msg( "getrlimit( RLIMIT_CORE, &out_data ) Error!" );
		return -1;
	}

	if ( getrlimit( RLIMIT_NOFILE, &out_data ) == 0 )
	{
		if ( out_data.rlim_cur < max_connection )
		{
			printf_msg( "getrlimit( RLIMIT_NOFILE, &out_data ) too small!" );
			return -1;
		}
	}
	else
	{
		printf_msg( "getrlimit( RLIMIT_NOFILE, &out_data ) Error!" );
		return -1;
	}
#endif
	g_frame_count = 0;
	g_maxactornum = max_connection;

	LOGI("%s-%d",__FUNCTION__,__LINE__);
	return 0;
}

// 服务器关闭
void process_close()
{
	int tmpi;
	// 关闭每个客户端
	for( tmpi = 0; tmpi < g_maxactornum; tmpi++ )
	{
		printf( "server_exit %d/%d\r", tmpi+1, g_maxactornum );
	}
	printf( "\n" );
}

// 返回-1,表示可以直接清除这个client
int process_onclose( int client_index )
{
	return -1;
}

int process_error( int client_index, int err )
{
	return 0;
}

int process_ondestory()
{
	return 0;
}
int Clear_Welcome();

int g_nExitHour = -1;
int g_nExitMin = -1;
int g_nExitNotify = 0;
time_t g_tExitTime = 0;

int process_client( int client_index, char *tmpbuf, int readsize, int exec_code )
{
	short server_code;
	char *ptr;
	int tmpsize;
	int packsize;
	int allsize;

	ptr = tmpbuf;
	allsize = 0;

	g_last_server_code = -100;
	int userid;

	while( allsize < readsize )
	{
		packsize = *((short *)ptr); ptr+=sizeof(short);
		if(packsize<=0 || packsize>2048)
			break;
		server_code = *((short *)ptr);	// cmd
		tmpsize = packsize;
		g_last_server_code = server_code;

		userid = (int)client_getuserid( client_index );
		if ( client_index == -2 )
		{
			process_logic();
		}
		else if( userid <= 0 && tmpsize == 14 )
		{
			SClientInfo ClientInfo = {};
			ClientInfo.userid = server_code;
			client_setuserinfo( client_index, &ClientInfo );
		}
		else if( server_code < 0 )
		{
			int target_index = -1;
			*((short *)ptr) = (short)client_getuserid( client_index );
			while( (target_index = client_nextindex( target_index )) >= 0 )
			{
				sendtoclient( target_index, ptr, tmpsize );
			}
		}
		else
		{
			int target_index = 0;
			*((short *)ptr) = (short)client_getuserid( client_index );
			while( target_index >= 0 )
			{
				target_index = client_getindex( server_code, target_index );
				if( target_index < 0 )
					break;
				sendtoclient( target_index, ptr, tmpsize );
				target_index++;
			}
		}
		ptr+=tmpsize;
		allsize += packsize+sizeof(short);
	}
	return 0;
}

int process_logic()
{
	int tick = 0;
	g_speed++;

	// 5帧一次逻辑
	if ( g_speed % 5 == 0 )
	{
		clients_logic(); // 执行所有的角色logic
	}

	//	1秒钟一次逻辑
	tick = g_speed % 30;
	if ( tick == 0 )
	{
#ifndef WIN32
		linuxmsg_recv();
#endif
	}
	return 0;
}