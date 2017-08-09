#include "server_process_auto.h"
#include "server_netsend_auto.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "login.h"
#include "gameproc.h"
#include "system_askinfo.h"
#include "actor.h"
#include "chat.h"
extern SConfig g_Config;

void proc_userawarded_S( int client_index, SLK_NetU_UserAwarded *pValue )
{
	// process.
	// ╫ги╚ап╠М
	client_setwait( client_index, 0 );
	SLK_NetS_List ListValue = { 0 };
	actor_getlist( client_getplatid( client_index ), client_getuserid( client_index ), &ListValue );
	netsend_list_S( client_index, SENDTYPE_ACTOR, &ListValue );
	write_gamelog( "[Serv_List]_idx:%d", client_index );
}

void proc_gmlocalcmd_S( int client_index, SLK_NetU_Gmlocalcmd *pValue )
{
	// process.
	int result = actor_command( client_index, pValue->m_cmd, pValue->m_value, pValue->m_msg );
	if ( client_index < 0 )
	{
		int gm_set_result( int client_index, int value );
		gm_set_result( client_index, result );
	}
}

void proc_logined_S( int client_index, SLK_NetU_Logined *pValue )
{
	// process.
	SClientInfo ClientInfo = { 0 };
	ClientInfo.userid = atoll( pValue->m_userid );
	ClientInfo.usertype = pValue->m_usertype;
	ClientInfo.result = pValue->m_result;
	strncpy( ClientInfo.username, pValue->m_username, MAX_USERNAME_LENGTH + 1 );
	strncpy( ClientInfo.deviceid, pValue->m_deviceid, 64 );
	if ( pValue->m_expires_in > 0 )
	{
		strncpy( ClientInfo.access_token, pValue->m_access_token, 65 );
		ClientInfo.expires_in = (int)time( NULL ) + pValue->m_expires_in - 60;
		strncpy( ClientInfo.refresh_token, pValue->m_refresh_token, 65 );
	}
	user_logined( pValue->m_client_index, pValue->m_authid, &ClientInfo );
}

void proc_timer_S( int client_index, char *pValue )
{
	// process.
	if ( client_index < 0 )
		process_logic();
}

void proc_login_S( int client_index, SLK_NetC_Login *pValue )
{
	// process.
	user_login( client_index, pValue->m_username, pValue->m_password, pValue->m_deviceid );
	write_gamelog( "[Serv_LOGIN]_idx:%d_user:%s", client_index, pValue->m_username );
	client_setisnew( client_index, (char)pValue->m_isnew );
	client_setipcountry( client_index, pValue->m_ipcountry );
}

void proc_create_S( int client_index, SLK_NetC_Create *pValue )
{
	// process.
	client_setwait( client_index, 0 );
	actor_create( client_index, pValue->m_nation, pValue->m_name );
	write_gamelog( "[Serv_CREATE]_idx:%d_nation:%d_name:%s", client_index, pValue->m_nation, pValue->m_name );
}

void proc_list_S( int client_index, char *pValue )
{
	// process.
	client_setwait( client_index, 0 );
	SLK_NetS_List ListValue = { 0 };
	actor_getlist( client_getplatid( client_index ), client_getuserid( client_index ), &ListValue );
	netsend_list_S( client_index, SENDTYPE_ACTOR, &ListValue );
	client_setnationaward( client_index, ListValue.m_nation_award );
	write_gamelog( "[Serv_List]_idx:%d", client_index );
}

void proc_entergame_S( int client_index, int *pValue )
{
	// process.
	client_setwait( client_index, 0 );
	actor_entergame( client_index, *pValue );
	write_gamelog( "[Serv_ENTERGAME]_idx:%d_id:%d", client_index, *pValue );
}

void proc_delete_S( int client_index, int *pValue )
{
	// process.
}

void proc_heart_S( int client_index, SLK_NetC_Heart *pValue )
{
	// process.
}

void proc_gmcmd_S( int client_index, SLK_NetC_Gmcmd *pValue )
{
	// process.
	actor_command( client_index, pValue->m_cmd, pValue->m_value, pValue->m_msg );
}

void proc_askinfo_S( int client_index, SLK_NetC_AskInfo *pValue )
{
	// process.
	system_askinfo( client_index, pValue->m_msgid, pValue->m_msg, pValue->m_value );
}

void proc_entercity_S( int client_index, SLK_NetC_EnterCity *pValue )
{
	// process.
	actor_entercity( client_index );
}

void proc_chat_S( int client_index, SLK_NetC_Chat *pValue )
{
	// process.
	chat_actortalk( client_index, pValue->m_channel, pValue->m_msg );
}

