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

	LKSET_MEM_RECV( pValue->m_value, (*pptr), 2*sizeof(int), (*psize) );
	return 0;
}

int struct_NetC_Gmcmd_recv( char **pptr, int *psize, SLK_NetC_Gmcmd *pValue )
{
	int tmpi = 0;

	LKSET_WORD_RECV( &pValue->m_cmd, (*pptr), (*psize) );
	LKSET_MEM_RECV( pValue->m_value, (*pptr), 4*sizeof(int), (*psize) );
	LKSET_WORD_RECV( &pValue->m_msg_length, (*pptr), (*psize) );
	if( pValue->m_msg_length < 0 || pValue->m_msg_length > (*psize)*(int)sizeof(char) || pValue->m_msg_length > 256 )
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
	if( pValue->m_msg_length < 0 || pValue->m_msg_length > (*psize)*(int)sizeof(char) || pValue->m_msg_length > 256 )
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

