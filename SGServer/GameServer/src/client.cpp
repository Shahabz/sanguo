#include "client.h"
#include "login.h"

#define HEARTBEAT_OUTTIME	18			// 2分钟
#define ENTERGAME_WAITTIME	60			// 10分钟

int g_max_connection = 100;
#ifdef _USEGATE
#else
#endif

extern SOCKET_SET *g_sockset;
#ifdef _USEGATE
extern CLIENT_SET *g_clientset;
extern BOOL g_bGateMode;
#endif

extern int g_net_time;
extern int g_max_connection;
static int s_max_recv_count = 0;

void client_setstat( int client_index, char stat )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if( g_bGateMode )
			g_clientset[client_index].m_stat = stat;
		else
#else
			g_sockset[client_index].m_stat = stat;
#endif
	}
}

char client_getstat( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if( g_bGateMode )
			return g_clientset[client_index].m_stat;
		else
#else
			return g_sockset[client_index].m_stat;
#endif
	}
	return CLIENT_STAT_NULL;
}

void client_setwait( int client_index, char iswait )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if( g_bGateMode )
			g_clientset[client_index].m_iswait = iswait;
		else
#else
			g_sockset[client_index].m_iswait = iswait;
#endif
	}
}

char client_getwait( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if( g_bGateMode )
			return g_clientset[client_index].m_iswait;
		else
#else
			return g_sockset[client_index].m_iswait;
#endif
	}
	return 0;
}

void client_setisnew( int client_index, char isnew )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if( g_bGateMode )
			g_clientset[client_index].m_isnew = isnew;
		else
#else
		g_sockset[client_index].m_isnew = isnew;
#endif
	}
}

char client_getisnew( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if( g_bGateMode )
			return g_clientset[client_index].m_isnew;
		else
#else
		return g_sockset[client_index].m_isnew;
#endif
	}
	return 0;
}

void client_setipcountry( int client_index, char *country )
{
	if ( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if ( g_bGateMode )
			strncpy( g_clientset[client_index].m_ipcountry, country, 3 );
		else
#else
		strncpy( g_sockset[client_index].m_ipcountry, country, 3 );
#endif
	}
}
char* client_getipcountry( int client_index )
{
	if ( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if ( g_bGateMode )
			return g_clientset[client_index].m_ipcountry;
		else
#else
		return g_sockset[client_index].m_ipcountry;
#endif
	}
	return 0;
}

void client_setcountry( int client_index, short country )
{
	if ( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if ( g_bGateMode )
			g_clientset[client_index].m_country = country;
		else
#else
		g_sockset[client_index].m_country = country;
#endif
	}
}
short client_getcountry( int client_index )
{
	if ( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if ( g_bGateMode )
			return g_clientset[client_index].m_country;
		else
#else
		return g_sockset[client_index].m_country;
#endif
	}
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
#ifdef _USEGATE
		if( g_bGateMode )
			{
				if( ip[tmpi] != g_clientset[client_index].m_ip[tmpi] )
					return curflag;
			}
			else
#else
			{
				if( ip[tmpi] != g_sockset[client_index].m_ip[tmpi] )
					return curflag;
			}
#endif
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
	{
#ifdef _USEGATE
		if( g_bGateMode )
			return g_clientset[client_index].m_ip;
		else
#else
			return g_sockset[client_index].m_ip;
#endif
	}
	return NULL;
}

char *client_getdeviceid( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if( g_bGateMode )
			return g_clientset[client_index].m_deviceid;
		else
#else
			return g_sockset[client_index].m_deviceid;
#endif
	}
	return NULL;
}

void client_setuserinfo( int client_index, SClientInfo *pClientInfo )
{
	if( client_index >= 0 && client_index < g_max_connection && pClientInfo )
	{
#ifdef _USEGATE
		if( g_bGateMode )
		{
			if( pClientInfo->userid > 0 )
				g_clientset[client_index].m_userid = pClientInfo->userid;
			if( pClientInfo->username[0] )
				memcpy( g_clientset[client_index].m_username, pClientInfo->username, MAX_USERNAME_LENGTH );
			g_clientset[client_index].m_username[MAX_USERNAME_LENGTH] = 0;
			g_clientset[client_index].m_usertype = pClientInfo->usertype;
			memcpy( g_clientset[client_index].m_deviceid, pClientInfo->deviceid, 65 );
		}
		else
#else
		{
			if( pClientInfo->userid > 0 )
				g_sockset[client_index].m_userid = pClientInfo->userid;
			if( pClientInfo->username[0] )
				memcpy( g_sockset[client_index].m_username, pClientInfo->username, MAX_USERNAME_LENGTH );
			g_sockset[client_index].m_username[MAX_USERNAME_LENGTH] = 0;
			g_sockset[client_index].m_usertype = pClientInfo->usertype;
			memcpy( g_sockset[client_index].m_deviceid, pClientInfo->deviceid, 65 );
		}
#endif
	}
}

i64 client_getuserinfo( int client_index, SClientInfo *pClientInfo )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if( g_bGateMode )
		{
			if( pClientInfo )
			{
				pClientInfo->userid = g_clientset[client_index].m_userid;
				memcpy( pClientInfo->username, g_clientset[client_index].m_username, MAX_USERNAME_LENGTH );
				pClientInfo->username[MAX_USERNAME_LENGTH] = 0;
				pClientInfo->usertype = g_clientset[client_index].m_usertype;
			}
			return g_clientset[client_index].m_userid;

		}
		else
#else
		{
			if( pClientInfo )
			{
				pClientInfo->userid = g_sockset[client_index].m_userid;
				memcpy( pClientInfo->username, g_sockset[client_index].m_username, MAX_USERNAME_LENGTH );
				pClientInfo->username[MAX_USERNAME_LENGTH] = 0;
				pClientInfo->usertype = g_sockset[client_index].m_usertype;
			}
			return g_sockset[client_index].m_userid;
		}
#endif
	}
	return -1;
}

i64 client_getuserid( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if( g_bGateMode )
			return g_clientset[client_index].m_userid;
		else
#else
			return g_sockset[client_index].m_userid;
#endif
	}
	return -1;
}

int client_get_exec_code( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if( g_bGateMode )
			return g_clientset[client_index].m_exec_code;
		else
#else
			return g_sockset[client_index].m_exec_code;
#endif
	}
	return -1;
}

short client_getusertype( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if( g_bGateMode )
			return g_clientset[client_index].m_usertype;
		else
#else
			return g_sockset[client_index].m_usertype;
#endif
	}
	return -1;
}

char *client_getusername( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if( g_bGateMode )
			return g_clientset[client_index].m_username;
		else
#else
			return g_sockset[client_index].m_username;
#endif
	}
	return NULL;
}

int client_getplatid( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if( g_bGateMode )
			return g_clientset[client_index].m_platid;
		else
#else
			return g_sockset[client_index].m_platid;
#endif
	}
	return -1;
}

char client_getlanguage( int client_index )
{
	if ( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if( g_bGateMode )
			return g_clientset[client_index].m_language;
		else
#else
			return g_sockset[client_index].m_language;
#endif
	}
	return 0;
}

int client_getinvcode( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if( g_bGateMode )
			return g_clientset[client_index].m_invcode;
		else
#else
			return g_sockset[client_index].m_invcode;
#endif
	}
	return -1;
}

int client_getos( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if( g_bGateMode )
			return g_clientset[client_index].m_os;
		else
#else
			return g_sockset[client_index].m_os;
#endif
	}
	return -1;
}

int client_getchannelid( int client_index )
{
	if ( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if ( g_bGateMode )
			return g_clientset[client_index].m_channelid;
		else
#else
		return g_sockset[client_index].m_channelid;
#endif
	}
	return -1;
}

char* client_getdevdata( int client_index )
{
	if ( client_index >= 0 && client_index < g_max_connection )
	{
#ifdef _USEGATE
		if ( g_bGateMode )
			return g_clientset[client_index].m_devdata;
		else
#else
		return g_sockset[client_index].m_devdata;
#endif
	}
	return "";
}

int client_get_maxrecvcount()
{
	return s_max_recv_count;
}

// 退出标志
char client_isexited( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection  )
	{
#ifdef _USEGATE
		if( g_bGateMode )
		{
			if( g_clientset[client_index].m_gate_index == -2 )
				return 1;
		}
		else
#else
		{
			if( g_sockset[client_index].m_fd == EXITED_SOCKET )
				return 1;
		}
#endif
	}
	return 0;
}

// 每个客户端的处理, 每10秒一次
int client_logic()
{
	int client_index;
	g_net_time++;
#ifdef _USEGATE
	if( g_bGateMode )
	{
		for( client_index = 0; client_index < g_max_connection; client_index++ )
		{
			if( g_clientset[client_index].m_gate_index != -1 )
			{
#ifndef _DEBUG
				++g_clientset[client_index].m_idle_time;
#endif
				//			if( g_clientset[client_index].m_idle_time >= HEARTBEAT_OUTTIME ||
				if( g_clientset[client_index].m_idle_time >= 90 )
				{
					write_netlog( "message:close client(m_stat:%d),gate=%d", g_clientset[client_index].m_stat, g_clientset[client_index].m_gate_index );
					close_client( client_index );
				}
			}
		}
	}
	else
#else
	{
		for( client_index = 0; client_index < g_max_connection; client_index++ )
		{
			if( g_sockset[client_index].m_fd != INVALID_SOCKET )
			{
#ifndef _DEBUG
				++g_sockset[client_index].m_idle_time;
#endif
				//			if( g_sockset[client_index].m_idle_time >= HEARTBEAT_OUTTIME ||
				if( g_sockset[client_index].m_idle_time >= 90 ||	// 先定15分钟，客户端有心跳协议后再改
					(g_sockset[client_index].m_stat < CLIENT_STAT_ENTERGAME && g_net_time - g_sockset[client_index].m_enter_time > ENTERGAME_WAITTIME) )
				{
					write_gamelog( "message:close client(m_stat:%d),fd=%d", g_sockset[client_index].m_stat, g_sockset[client_index].m_fd );
					close_client( client_index );
				}
			}
			if( g_sockset[client_index].m_recv_count > s_max_recv_count )
				s_max_recv_count = g_sockset[client_index].m_recv_count;
			g_sockset[client_index].m_recv_count = 0;
		}

	}
#endif
	return 0;
}
