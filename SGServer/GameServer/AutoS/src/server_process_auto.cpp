#include "server_process_auto.h"
#include "server_netsend_auto.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "login.h"
#include "gameproc.h"
#include "system_askinfo.h"
#include "actor.h"
#include "chat.h"
#include "story.h"
#include "map.h"
#include "army.h"
#include "city.h"
#include "mail.h"
#include "world_boss.h"
#include "hero.h"
#include "quest.h"
extern SConfig g_Config;

void proc_userawarded_S( int client_index, SLK_NetU_UserAwarded *pValue )
{
	// process.
	// 角色列表
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
	chat_actortalk( client_index, pValue->m_channel, CHAT_MSGTYPE_ACTORCHAT, pValue->m_msg );
}

void proc_storybattle_S( int client_index, SLK_NetC_StoryBattle *pValue )
{
	// process.
	story_battle( client_index, pValue );
}

void proc_worldmapask_S( int client_index, SLK_NetC_WorldMapAsk *pValue )
{
	// process.
	map_sendinfo( client_index, pValue->m_to_posx, pValue->m_to_posy );
}

void proc_worldmapareaindex_S( int client_index, SLK_NetC_WorldMapAreaIndex *pValue )
{
	// process.
	map_areaenter( client_index, pValue->m_areaindex, pValue->m_posx, pValue->m_posy );
}

void proc_mapbattle_S( int client_index, SLK_NetC_MapBattle *pValue )
{
	// process.
	if ( pValue->m_action == ARMY_ACTION_HELP_TROOP || pValue->m_action == ARMY_ACTION_KINGWAR )
	{ // 驻防和皇城战要拆开单个武将
		SLK_NetC_MapBattle tmpValue = { 0 };
		for ( int tmpi = 0; tmpi < 4; tmpi++ )
		{
			if ( pValue->m_herokind[tmpi] > 0 )
			{
				memcpy( &tmpValue, pValue, sizeof( SLK_NetC_MapBattle ) );
				tmpValue.m_herokind[0] = pValue->m_herokind[tmpi];
				tmpValue.m_herokind[1] = 0;
				tmpValue.m_herokind[2] = 0;
				tmpValue.m_herokind[3] = 0;
				army_battle( city_getptr( client_index ), &tmpValue );
			}
		}
	}
	else
	{
		army_battle( city_getptr( client_index ), pValue );
	}
}

void proc_mailask_S( int client_index, SLK_NetC_MailAsk *pValue )
{
	// process.
	mail_getlist( client_index, pValue->m_op, pValue->m_minid, pValue->m_maxid );
}

void proc_mailop_S( int client_index, SLK_NetC_MailOp *pValue )
{
	// process.
	if ( pValue->m_op == 1 )
	{ // 设置已读状态
		mail_readed( client_index, pValue->m_mailid );
	}
	else if ( pValue->m_op == 2 )
	{ // 附件领取
		mail_attachget( client_index, pValue->m_mailid );
	}
	else if ( pValue->m_op == 3 )
	{ // 删除
		mail_delete_actor( client_index, pValue->m_mailid );
	}
	else if ( pValue->m_op == 4 )
	{ // 全部已读
		mail_readed_all( client_index );
	}
	else if ( pValue->m_op == 5 )
	{ // 锁定
		mail_locked( client_index, pValue->m_mailid );
	}
	else if ( pValue->m_op == 6 )
	{ // 解除锁定
		mail_unlocked( client_index, pValue->m_mailid );
	}
	else if ( pValue->m_op == 7 )
	{ // 战斗信息
		mail_fight_read( client_index, pValue->m_mailid );
	}
	else if ( pValue->m_op == 8 )
	{ // 获取分享的邮件内容
		mail_share_getcontent( client_index, pValue->m_mailid );
	}
	else if ( pValue->m_op == 9 )
	{ // 获取分享他内容的战斗信息
		mail_share_getfightcontent( client_index, pValue->m_mailid );
	}
}

void proc_mailalldel_S( int client_index, SLK_NetC_MailAllDel *pValue )
{
	// process.
	mail_delete_all( client_index, pValue );
}

void proc_mailshare_S( int client_index, SLK_NetS_MailShare *pValue )
{
	// process.
	mail_share( client_index, pValue );
}

void proc_mailsend_S( int client_index, SLK_NetC_MailSend *pValue )
{
	// process.
	mail_actor_send( client_index, pValue );
}

void proc_mailreply_S( int client_index, SLK_NetC_MailReply *pValue )
{
	// process.
	mail_actor_reply( client_index, pValue );
}

void proc_worldbossbattle_S( int client_index, SLK_NetC_WorldBossBattle *pValue )
{
	// process.
	worldboss_battle( client_index, pValue );
}

void proc_heroguardsort_S( int client_index, SLK_NetC_HeroGuardSort *pValue )
{
	// process.
	hero_guard_sort( client_index, pValue );
}

void proc_questtalknext_S( int client_index, SLK_NetC_QuestTalkNext *pValue )
{
	// process.
	if ( pValue->m_type == 0 )
	{
		quest_talk_next( client_index, pValue->m_talkid );
	}
	else if( pValue->m_type == 1 )
	{
		quest_talk_client_ask( client_index, pValue->m_talkid );
	}
}

