#include "client.h"

int g_max_connection = 100;
int g_connectioncount = 0;
int g_last_cmd = 0;

extern SOCKET_SET *g_sockset;

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

char *client_getdeviceid( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
		return g_sockset[client_index].m_deviceid;
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
		memcpy( g_sockset[client_index].m_deviceid, pClientInfo->deviceid, 65 );
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

int client_getplatid( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		return g_sockset[client_index].m_platid;
	}
	return -1;
}

int client_getlanguage( int client_index )
{
	if ( client_index >= 0 && client_index < g_max_connection )
	{
		return g_sockset[client_index].m_language;
	}
	return 1;
}

int client_getinvcode( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		return g_sockset[client_index].m_invcode;
	}
	return -1;
}

int client_getclientoptype( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		return g_sockset[client_index].m_optype;
	}
	return -1;
}

// 退出标志
char client_isexited( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection && g_sockset[client_index].m_fd == EXITED_SOCKET )
		return 1;
	return 0;
}

// 每个客户端的处理
int client_getindex( i64 userid, int begin_index )
{
	int client_index;
	for( client_index = begin_index; client_index < g_max_connection; client_index++ )
	{
		if( g_sockset[client_index].m_fd != INVALID_SOCKET && g_sockset[client_index].m_userid == userid )
		{
			return client_index;
		}
	}
	return -1;
}

int client_nextindex( int cur_index )
{
	int client_index;
	for( client_index = cur_index + 1; client_index < g_max_connection; client_index++ )
	{
		if( g_sockset[client_index].m_fd != INVALID_SOCKET )
		{
			return client_index;
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
// actors_logic
// 函数说明: 所有角色逻辑 5帧一次
//-----------------------------------------------------------------------------
void clients_logic()
{
	int tmpi;
	for ( tmpi = 0; tmpi < g_max_connection; tmpi++ )
	{
		// 退出标志
		if ( client_isexited( tmpi ) )
		{
			net_closesock_direct( tmpi );
		}
	}
}
