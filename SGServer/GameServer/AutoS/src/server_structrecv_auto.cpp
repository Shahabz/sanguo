#include <string.h>
#include "server_structrecv_auto.h"
#include "actor_send.h"

int struct_NetC_Login_recv( char **pptr, int *psize, SLK_NetC_Login *pValue )
{
	int tmpi = 0;

	LKSET_WORD_RECV( &pValue->m_username_length, (*pptr), (*psize) );
	if( pValue->m_username_length < 0 || pValue->m_username_length > (*psize)*(int)sizeof(char) || pValue->m_username_length > 51 )
		return -1;
	LKSET_MEM_RECV( pValue->m_username, (*pptr), pValue->m_username_length*sizeof(char), (*psize) );
	LKSET_WORD_RECV( &pValue->m_password_length, (*pptr), (*psize) );
	if( pValue->m_password_length < 0 || pValue->m_password_length > (*psize)*(int)sizeof(char) || pValue->m_password_length > 256 )
		return -1;
	LKSET_MEM_RECV( pValue->m_password, (*pptr), pValue->m_password_length*sizeof(char), (*psize) );
	LKSET_WORD_RECV( &pValue->m_deviceid_length, (*pptr), (*psize) );
	if( pValue->m_deviceid_length < 0 || pValue->m_deviceid_length > (*psize)*(int)sizeof(char) || pValue->m_deviceid_length > 65 )
		return -1;
	LKSET_MEM_RECV( pValue->m_deviceid, (*pptr), pValue->m_deviceid_length*sizeof(char), (*psize) );
	LKSET_WORD_RECV( &pValue->m_isnew, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_ipcountry_length, (*pptr), (*psize) );
	if( pValue->m_ipcountry_length < 0 || pValue->m_ipcountry_length > (*psize)*(int)sizeof(char) || pValue->m_ipcountry_length > 16 )
		return -1;
	LKSET_MEM_RECV( pValue->m_ipcountry, (*pptr), pValue->m_ipcountry_length*sizeof(char), (*psize) );
	return 0;
}

int struct_NetU_Logined_recv( char **pptr, int *psize, SLK_NetU_Logined *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_RECV( &pValue->m_result, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_client_index, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_authid, (*pptr), (*psize) );
	LKSET_MEM_RECV( pValue->m_userid, (*pptr), 21*sizeof(char), (*psize) );
	LKSET_MEM_RECV( pValue->m_username, (*pptr), 51*sizeof(char), (*psize) );
	LKSET_WORD_RECV( &pValue->m_usertype, (*pptr), (*psize) );
	LKSET_SBYTE_RECV( &pValue->m_checkflag, (*pptr), (*psize) );
	LKSET_SBYTE_RECV( &pValue->m_isdeep, (*pptr), (*psize) );
	LKSET_MEM_RECV( pValue->m_access_token, (*pptr), 65*sizeof(char), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_expires_in, (*pptr), (*psize) );
	LKSET_MEM_RECV( pValue->m_refresh_token, (*pptr), 65*sizeof(char), (*psize) );
	LKSET_MEM_RECV( pValue->m_deviceid, (*pptr), 65*sizeof(char), (*psize) );
	return 0;
}

int struct_NetC_Create_recv( char **pptr, int *psize, SLK_NetC_Create *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_RECV( &pValue->m_nation, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_name_length, (*pptr), (*psize) );
	if( pValue->m_name_length < 0 || pValue->m_name_length > (*psize)*(int)sizeof(char) || pValue->m_name_length > 22 )
		return -1;
	LKSET_MEM_RECV( pValue->m_name, (*pptr), pValue->m_name_length*sizeof(char), (*psize) );
	return 0;
}

int struct_NetC_Heart_recv( char **pptr, int *psize, SLK_NetC_Heart *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_RECV( &pValue->m_value, (*pptr), (*psize) );
	return 0;
}

int struct_NetC_Gmcmd_recv( char **pptr, int *psize, SLK_NetC_Gmcmd *pValue )
{
	int tmpi = 0;

	LKSET_WORD_RECV( &pValue->m_cmd, (*pptr), (*psize) );
	LKSET_MEM_RECV( pValue->m_value, (*pptr), 4*sizeof(int), (*psize) );
	LKSET_WORD_RECV( &pValue->m_msg_length, (*pptr), (*psize) );
	if( pValue->m_msg_length < 0 || pValue->m_msg_length > (*psize)*(int)sizeof(char) || pValue->m_msg_length > 1024 )
		return -1;
	LKSET_MEM_RECV( pValue->m_msg, (*pptr), pValue->m_msg_length*sizeof(char), (*psize) );
	return 0;
}

int struct_NetU_UserAwarded_recv( char **pptr, int *psize, SLK_NetU_UserAwarded *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_RECV( &pValue->m_result, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_client_index, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_authid, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_cdkey_index, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_awardgroup, (*pptr), (*psize) );
	LKSET_MEM_RECV( pValue->m_cardnumber, (*pptr), 32*sizeof(char), (*psize) );
	return 0;
}

int struct_NetU_Gmlocalcmd_recv( char **pptr, int *psize, SLK_NetU_Gmlocalcmd *pValue )
{
	int tmpi = 0;

	LKSET_WORD_RECV( &pValue->m_cmd, (*pptr), (*psize) );
	LKSET_MEM_RECV( pValue->m_value, (*pptr), 4*sizeof(int), (*psize) );
	LKSET_WORD_RECV( &pValue->m_msglen, (*pptr), (*psize) );
	if( pValue->m_msglen < 0 || pValue->m_msglen > (*psize)*(int)sizeof(char) || pValue->m_msglen > 1024 )
		return -1;
	LKSET_MEM_RECV( pValue->m_msg, (*pptr), pValue->m_msglen*sizeof(char), (*psize) );
	return 0;
}

int struct_NetC_AskInfo_recv( char **pptr, int *psize, SLK_NetC_AskInfo *pValue )
{
	int tmpi = 0;

	LKSET_WORD_RECV( &pValue->m_msgid, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_valuenum, (*pptr), (*psize) );
	if( pValue->m_valuenum < 0 || pValue->m_valuenum > (*psize)*(int)sizeof(int) || pValue->m_valuenum > 10 )
		return -1;
	LKSET_MEM_RECV( pValue->m_value, (*pptr), pValue->m_valuenum*sizeof(int), (*psize) );
	LKSET_WORD_RECV( &pValue->m_msg_length, (*pptr), (*psize) );
	if( pValue->m_msg_length < 0 || pValue->m_msg_length > (*psize)*(int)sizeof(char) || pValue->m_msg_length > 1024 )
		return -1;
	LKSET_MEM_RECV( pValue->m_msg, (*pptr), pValue->m_msg_length*sizeof(char), (*psize) );
	return 0;
}

int struct_NetC_EnterCity_recv( char **pptr, int *psize, SLK_NetC_EnterCity *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_RECV( &pValue->m_value, (*pptr), (*psize) );
	return 0;
}

int struct_NetU_InviteCodeed_recv( char **pptr, int *psize, SLK_NetU_InviteCodeed *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_RECV( &pValue->m_result, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_client_index, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_authid, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_path, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_isaward, (*pptr), (*psize) );
	return 0;
}

int struct_NetC_Chat_recv( char **pptr, int *psize, SLK_NetC_Chat *pValue )
{
	int tmpi = 0;

	LKSET_WORD_RECV( &pValue->m_msglen, (*pptr), (*psize) );
	if( pValue->m_msglen < 0 || pValue->m_msglen > (*psize)*(int)sizeof(char) || pValue->m_msglen > 127 )
		return -1;
	LKSET_MEM_RECV( pValue->m_msg, (*pptr), pValue->m_msglen*sizeof(char), (*psize) );
	LKSET_SBYTE_RECV( &pValue->m_channel, (*pptr), (*psize) );
	return 0;
}

int struct_NetC_StoryBattle_recv( char **pptr, int *psize, SLK_NetC_StoryBattle *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_RECV( &pValue->m_storyid, (*pptr), (*psize) );
	LKSET_MEM_RECV( pValue->m_herokind, (*pptr), 4*sizeof(short), (*psize) );
	return 0;
}

int struct_NetC_WorldMapAsk_recv( char **pptr, int *psize, SLK_NetC_WorldMapAsk *pValue )
{
	int tmpi = 0;

	LKSET_WORD_RECV( &pValue->m_to_posx, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_to_posy, (*pptr), (*psize) );
	return 0;
}

int struct_NetC_WorldMapAreaIndex_recv( char **pptr, int *psize, SLK_NetC_WorldMapAreaIndex *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_RECV( &pValue->m_areaindex, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_posx, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_posy, (*pptr), (*psize) );
	LKSET_SBYTE_RECV( &pValue->m_areaupdate, (*pptr), (*psize) );
	return 0;
}

int struct_NetC_MapBattle_recv( char **pptr, int *psize, SLK_NetC_MapBattle *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_RECV( &pValue->m_to_unit_type, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_to_unit_index, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_id, (*pptr), (*psize) );
	LKSET_MEM_RECV( pValue->m_herokind, (*pptr), 4*sizeof(short), (*psize) );
	LKSET_WORD_RECV( &pValue->m_to_posx, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_to_posy, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_appdata, (*pptr), (*psize) );
	LKSET_SBYTE_RECV( &pValue->m_action, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_group_index, (*pptr), (*psize) );
	return 0;
}

int struct_NetC_MailAsk_recv( char **pptr, int *psize, SLK_NetC_MailAsk *pValue )
{
	int tmpi = 0;

	LKSET_LONG_RECV( &pValue->m_minid, (*pptr), (*psize) );
	LKSET_LONG_RECV( &pValue->m_maxid, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_op, (*pptr), (*psize) );
	return 0;
}

int struct_NetC_MailOp_recv( char **pptr, int *psize, SLK_NetC_MailOp *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_RECV( &pValue->m_op, (*pptr), (*psize) );
	LKSET_LONG_RECV( &pValue->m_mailid, (*pptr), (*psize) );
	return 0;
}

int struct_NetC_MailAllDel_recv( char **pptr, int *psize, SLK_NetC_MailAllDel *pValue )
{
	int tmpi = 0;

	LKSET_WORD_RECV( &pValue->m_count, (*pptr), (*psize) );
	if( pValue->m_count < 0 || pValue->m_count > (*psize)*(int)sizeof(i64) || pValue->m_count > 128 )
		return -1;
	LKSET_MEM_RECV( pValue->m_mailid, (*pptr), pValue->m_count*sizeof(i64), (*psize) );
	return 0;
}

int struct_NetS_MailShare_recv( char **pptr, int *psize, SLK_NetS_MailShare *pValue )
{
	int tmpi = 0;

	LKSET_LONG_RECV( &pValue->m_mailid, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_a_name_len, (*pptr), (*psize) );
	if( pValue->m_a_name_len < 0 || pValue->m_a_name_len > (*psize)*(int)sizeof(char) || pValue->m_a_name_len > 32 )
		return -1;
	LKSET_MEM_RECV( pValue->m_a_name, (*pptr), pValue->m_a_name_len*sizeof(char), (*psize) );
	LKSET_WORD_RECV( &pValue->m_d_name_len, (*pptr), (*psize) );
	if( pValue->m_d_name_len < 0 || pValue->m_d_name_len > (*psize)*(int)sizeof(char) || pValue->m_d_name_len > 32 )
		return -1;
	LKSET_MEM_RECV( pValue->m_d_name, (*pptr), pValue->m_d_name_len*sizeof(char), (*psize) );
	LKSET_SBYTE_RECV( &pValue->m_type, (*pptr), (*psize) );
	return 0;
}

int struct_NetC_MailSend_recv( char **pptr, int *psize, SLK_NetC_MailSend *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_RECV( &pValue->m_unit_index, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_actorid, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_content_length, (*pptr), (*psize) );
	if( pValue->m_content_length < 0 || pValue->m_content_length > (*psize)*(int)sizeof(char) || pValue->m_content_length > 512 )
		return -1;
	LKSET_MEM_RECV( pValue->m_content, (*pptr), pValue->m_content_length*sizeof(char), (*psize) );
	return 0;
}

int struct_NetC_MailReply_recv( char **pptr, int *psize, SLK_NetC_MailReply *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_RECV( &pValue->m_actorid, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_content_length, (*pptr), (*psize) );
	if( pValue->m_content_length < 0 || pValue->m_content_length > (*psize)*(int)sizeof(char) || pValue->m_content_length > 512 )
		return -1;
	LKSET_MEM_RECV( pValue->m_content, (*pptr), pValue->m_content_length*sizeof(char), (*psize) );
	LKSET_WORD_RECV( &pValue->m_reply_length, (*pptr), (*psize) );
	if( pValue->m_reply_length < 0 || pValue->m_reply_length > (*psize)*(int)sizeof(char) || pValue->m_reply_length > 512 )
		return -1;
	LKSET_MEM_RECV( pValue->m_reply, (*pptr), pValue->m_reply_length*sizeof(char), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_reply_recvtime, (*pptr), (*psize) );
	return 0;
}

int struct_NetC_WorldBossBattle_recv( char **pptr, int *psize, SLK_NetC_WorldBossBattle *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_RECV( &pValue->m_bossid, (*pptr), (*psize) );
	LKSET_MEM_RECV( pValue->m_herokind, (*pptr), 4*sizeof(short), (*psize) );
	return 0;
}

int struct_NetC_HeroGuardSort_recv( char **pptr, int *psize, SLK_NetC_HeroGuardSort *pValue )
{
	int tmpi = 0;

	LKSET_MEM_RECV( pValue->m_herokind, (*pptr), 4*sizeof(short), (*psize) );
	return 0;
}

int struct_NetC_QuestTalkNext_recv( char **pptr, int *psize, SLK_NetC_QuestTalkNext *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_RECV( &pValue->m_talkid, (*pptr), (*psize) );
	LKSET_SBYTE_RECV( &pValue->m_type, (*pptr), (*psize) );
	return 0;
}

int struct_NetC_RankAsk_recv( char **pptr, int *psize, SLK_NetC_RankAsk *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_RECV( &pValue->m_type, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_page, (*pptr), (*psize) );
	LKSET_SBYTE_RECV( &pValue->m_myrange, (*pptr), (*psize) );
	return 0;
}

int struct_NetC_FriendOp_recv( char **pptr, int *psize, SLK_NetC_FriendOp *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_RECV( &pValue->m_op, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_target_actorid, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_target_cityindex, (*pptr), (*psize) );
	LKSET_SBYTE_RECV( &pValue->m_target_namelen, (*pptr), (*psize) );
	if( pValue->m_target_namelen < 0 || pValue->m_target_namelen > (*psize)*(int)sizeof(char) || pValue->m_target_namelen > 32 )
		return -1;
	LKSET_MEM_RECV( pValue->m_target_name, (*pptr), pValue->m_target_namelen*sizeof(char), (*psize) );
	return 0;
}

