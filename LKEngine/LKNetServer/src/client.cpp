#include "client.h"
//#define HEARTBEAT_OUTTIME	100
#define HEARTBEAT_OUTTIME	18			// 2分钟
#define ENTERGAME_WAITTIME	30			// 5分钟

extern SOCKET_SET *g_sockset;
extern int g_net_time;
extern int g_max_connection;

static int s_max_recv_count = 0;

char client_is_same_mac( int client_index, short nMac, char *pMac )
{
	int tmpi;
	if(  nMac <= 0 || nMac > 64 || pMac == NULL )
		return 0;

	if( client_index >= 0 && client_index < g_max_connection )
	{
		if( g_sockset[client_index].m_mac_length <= 0 || nMac != g_sockset[client_index].m_mac_length )
			return 0;
		for( tmpi = 0; tmpi < nMac; tmpi++ )
		{
			if( g_sockset[client_index].m_mac_id[tmpi] != pMac[tmpi] )
				return 0;
		}
		return 1;
	}
	return 0;
}

char client_is_same_cpu( int client_index, short nCpu, char *pCpu )
{
	int tmpi;
	if(  nCpu <= 0 || nCpu > 64 || pCpu == NULL )
		return 0;

	if( client_index >= 0 && client_index < g_max_connection )
	{
		if( g_sockset[client_index].m_cpu_length <= 0 || nCpu != g_sockset[client_index].m_cpu_length )
			return 0;
		for( tmpi = 0; tmpi < nCpu; tmpi++ )
		{
			if( g_sockset[client_index].m_cpu_id[tmpi] != pCpu[tmpi] )
				return 0;
		}
		return 1;
	}
	return 0;
}

short client_get_mac( int client_index, char *pMac )
{
	if( pMac == NULL )
		return 0;
	if( client_index >= 0 && client_index < g_max_connection )
	{
		if( g_sockset[client_index].m_mac_length <= 0 || g_sockset[client_index].m_mac_length > 64 )
			return 0;
		memcpy( pMac, g_sockset[client_index].m_mac_id, g_sockset[client_index].m_mac_length );
		return g_sockset[client_index].m_mac_length;
	}
	return 0;
}

short client_get_cpu( int client_index, char *pCpu )
{
	if( pCpu == NULL )
		return 0;
	if( client_index >= 0 && client_index < g_max_connection )
	{
		if( g_sockset[client_index].m_cpu_length <= 0 || g_sockset[client_index].m_cpu_length > 64 )
			return 0;
		memcpy( pCpu, g_sockset[client_index].m_cpu_id, g_sockset[client_index].m_cpu_length );
		return g_sockset[client_index].m_cpu_length;
	}
	return 0;
}

void client_setmaccpu_info( int client_index, short nMac, char *pMac, short nCpu, char *pCpu )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		if( nMac > 64 || nMac < 0 )
			nMac = 0;
		if( nCpu > 64 || nCpu < 0 )
			nCpu = 0;

		if( nMac > 0 )
			memcpy( g_sockset[client_index].m_mac_id, pMac, nMac );
		if( nCpu > 0 )
			memcpy( g_sockset[client_index].m_cpu_id, pCpu, nCpu );

		g_sockset[client_index].m_mac_length = nMac;
		g_sockset[client_index].m_cpu_length = nCpu;
	}
}

void client_setstat( int client_index, char stat )
{
	if( client_index >= 0 && client_index < g_max_connection )
		g_sockset[client_index].m_stat = stat;
}

char client_getstat( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
		return g_sockset[client_index].m_stat;
	return CLIENT_STAT_NULL;
}

void client_setwait( int client_index, char iswait )
{
	if( client_index >= 0 && client_index < g_max_connection )
		g_sockset[client_index].m_iswait = iswait;
}

char client_getwait( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
		return g_sockset[client_index].m_iswait;
	return 0;
}

// 0完全不同 1.一个高位相同 2.两个高位相同 3.三个高位相同 4完全相同
char cleint_ipcomp( int client_index, char *ip )
{
	int tmpi;
	int curflag = 0;
	if( ip == NULL )
		return 0;
	if( client_index >= 0 && client_index < g_max_connection )
	{
		for( tmpi = 0; tmpi < 16; tmpi++ )
		{
			if( ip[tmpi] != g_sockset[client_index].m_ip[tmpi] )
				return curflag;
			if( ip[tmpi] == 0 )
				return curflag+1;
			if( ip[tmpi] == '.' )
				curflag++;
		}
	}
	return 0;
}

char *client_getip( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
		return g_sockset[client_index].m_ip;
	return NULL;
}
void client_setuserinfo( int client_index, SClientInfo *pClientInfo )
{
	if( client_index >= 0 && client_index < g_max_connection && pClientInfo )
	{
		if( pClientInfo->userid > 0 )
			g_sockset[client_index].m_userid = pClientInfo->userid;
		if( pClientInfo->username[0] )
			memcpy( g_sockset[client_index].m_username, pClientInfo->username, MAX_USERNAME_LENGTH );
		g_sockset[client_index].m_username[MAX_USERNAME_LENGTH] = 0;
		g_sockset[client_index].m_usertype = pClientInfo->usertype;
		if( pClientInfo->isgolden > 0 )
			g_sockset[client_index].m_isgolden = pClientInfo->isgolden;
		g_sockset[client_index].m_isdeep = pClientInfo->isdeep;
		g_sockset[client_index].m_point_totle = pClientInfo->point_totle;
		g_sockset[client_index].m_cardstat[0] = pClientInfo->cardstat[0];
		g_sockset[client_index].m_cardstat[1] = pClientInfo->cardstat[1];
		g_sockset[client_index].m_cardstat[2] = pClientInfo->cardstat[2];
		g_sockset[client_index].m_cardstat[3] = pClientInfo->cardstat[3];
	}
}

i64 client_getuserinfo( int client_index, SClientInfo *pClientInfo )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		if( pClientInfo )
		{
			pClientInfo->userid = g_sockset[client_index].m_userid;
			memcpy( pClientInfo->username, g_sockset[client_index].m_username, MAX_USERNAME_LENGTH );
			pClientInfo->username[MAX_USERNAME_LENGTH] = 0;
			pClientInfo->usertype = g_sockset[client_index].m_usertype;
			pClientInfo->isgolden = g_sockset[client_index].m_isgolden;
			pClientInfo->isdeep = g_sockset[client_index].m_isdeep;
			pClientInfo->point_totle = g_sockset[client_index].m_point_totle;
			pClientInfo->cardstat[0] = g_sockset[client_index].m_cardstat[0];
			pClientInfo->cardstat[1] = g_sockset[client_index].m_cardstat[1];
			pClientInfo->cardstat[2] = g_sockset[client_index].m_cardstat[2];
			pClientInfo->cardstat[3] = g_sockset[client_index].m_cardstat[3];
		}
		return g_sockset[client_index].m_userid;
	}
	return -1;
}

i64 client_getuserid( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		return g_sockset[client_index].m_userid;
	}
	return -1;
}

int client_get_exec_code( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		return g_sockset[client_index].m_exec_code;
	}
	return -1;
}

short client_getusertype( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		return g_sockset[client_index].m_usertype;
	}
	return -1;
}

char *client_getusername( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		return g_sockset[client_index].m_username;
	}
	return NULL;
}

char client_getpasskey( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		return (char)g_sockset[client_index].m_pass_key;
	}
	return 0;
}
// 设置这个socket的验证码
char client_setsyspwd( int client_index, char *syspwd )
{
	if ( !syspwd )
		return -1;
	if( client_index >= 0 && client_index < g_max_connection )
	{
		strncpy( g_sockset[client_index].m_syspwd, syspwd, MAX_SYSPWD_LENGTH );
	}
	return 0;
}
// 获取这个socket的验证码
char client_getsyspwd( int client_index, char *outpwd )
{
	if ( !outpwd )
		return -1;
	if( client_index >= 0 && client_index < g_max_connection )
	{
		strncpy( outpwd, g_sockset[client_index].m_syspwd, MAX_SYSPWD_LENGTH );
	}
	return 0;
}
// 设置这个socket的验证码输入错误的次数
short client_addsyspwd_error( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		g_sockset[client_index].m_syspwd_error += 1;
	}
	return 0;
}
// 获取这个socket的验证码输入错误的次数
short client_getsyspwd_error( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		return g_sockset[client_index].m_syspwd_error;
	}
	return 0;
}
// 清空这个socket的验证码输入错误的次数
short client_clearsyspwd_error( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		g_sockset[client_index].m_syspwd_error = 0;
	}
	return 0;
}
int client_get_maxrecvcount()
{
	return s_max_recv_count;
}

// 退出标志
char client_isexited( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection && g_sockset[client_index].m_fd == EXITED_SOCKET )
		return 1;
	return 0;
}

// 每个客户端的处理, 每10秒一次
int client_logic()
{
	int client_index;
	g_net_time++;
	for( client_index = 0; client_index < g_max_connection; client_index++ )
	{
		if( g_sockset[client_index].m_fd != INVALID_SOCKET )
		{
#ifndef _DEBUG
			++g_sockset[client_index].m_idle_time;
#endif
			if( g_sockset[client_index].m_idle_time >= HEARTBEAT_OUTTIME ||
				( g_sockset[client_index].m_stat < CLIENT_STAT_ENTERGAME && g_net_time - g_sockset[client_index].m_enter_time > ENTERGAME_WAITTIME ) )
			{
				write_netlog( "message:close client(m_stat:%d),fd=%d", g_sockset[client_index].m_stat, g_sockset[client_index].m_fd );
				close_client( client_index );
			}
		}
		if( g_sockset[client_index].m_recv_count > s_max_recv_count )
			s_max_recv_count = g_sockset[client_index].m_recv_count;
		g_sockset[client_index].m_recv_count = 0;
	}
	return 0;
}
