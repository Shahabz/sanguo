#include <string.h>
#include "server_structsend_auto.h"
#include "actor_send.h"

int struct_NetU_Logined_send( char **pptr, int *psize, SLK_NetU_Logined *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_result, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_client_index, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_authid, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_userid, 21*sizeof(char), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_username, 51*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_usertype, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_checkflag, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_isdeep, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_access_token, 65*sizeof(char), (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_expires_in, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_refresh_token, 65*sizeof(char), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_deviceid, 65*sizeof(char), (*psize) );
	return 0;
}

int struct_NetS_Login_send( char **pptr, int *psize, SLK_NetS_Login *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_result, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_username_length, (*psize) );
	if( pValue->m_username_length > 0 && pValue->m_username_length <= 51 )
		LKSET_MEM_SEND( (*pptr), pValue->m_username, pValue->m_username_length*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_token_length, (*psize) );
	if( pValue->m_token_length > 0 && pValue->m_token_length <= 256 )
		LKSET_MEM_SEND( (*pptr), pValue->m_token, pValue->m_token_length*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_usertype, (*psize) );
	return 0;
}

int struct_ListInfo_send( char **pptr, int *psize, SLK_ListInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_name, 22*sizeof(char), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_offset, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_lockstat, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_delete_stoptime, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_lock_endtime, (*psize) );
	return 0;
}

int struct_NetS_List_send( char **pptr, int *psize, SLK_NetS_List *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_actor_num, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_actor_num; tmpi++ )
	{
		struct_ListInfo_send( pptr, psize, &pValue->m_listinfo[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation_award, (*psize) );
	return 0;
}

int struct_NetS_Create_send( char **pptr, int *psize, SLK_NetS_Create *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_result, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_createtime, (*psize) );
	return 0;
}

int struct_NetS_EnterInfo_send( char **pptr, int *psize, SLK_NetS_EnterInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_data_int, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_data_short, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_data_char, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_serverid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_servertime, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_createtime, (*psize) );
	return 0;
}

int struct_NetS_Delete_send( char **pptr, int *psize, SLK_NetS_Delete *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_result, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	return 0;
}

int struct_NetS_Heart_send( char **pptr, int *psize, SLK_NetS_Heart *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_value, (*psize) );
	return 0;
}

int struct_NetS_Notify_send( char **pptr, int *psize, SLK_NetS_Notify *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_msgid, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_valuenum, (*psize) );
	if( pValue->m_valuenum > 0 && pValue->m_valuenum <= 10 )
		LKSET_MEM_SEND( (*pptr), pValue->m_value, pValue->m_valuenum*sizeof(int), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_msg_length, (*psize) );
	if( pValue->m_msg_length > 0 && pValue->m_msg_length <= 1024 )
		LKSET_MEM_SEND( (*pptr), pValue->m_msg, pValue->m_msg_length*sizeof(char), (*psize) );
	return 0;
}

int struct_NetU_UserAwarded_send( char **pptr, int *psize, SLK_NetU_UserAwarded *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_result, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_client_index, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_authid, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_cdkey_index, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_awardgroup, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_cardnumber, 32*sizeof(char), (*psize) );
	return 0;
}

int struct_NetU_Gmlocalcmd_send( char **pptr, int *psize, SLK_NetU_Gmlocalcmd *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_cmd, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_value, 4*sizeof(int), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_msglen, (*psize) );
	if( pValue->m_msglen > 0 && pValue->m_msglen <= 1024 )
		LKSET_MEM_SEND( (*pptr), pValue->m_msg, pValue->m_msglen*sizeof(char), (*psize) );
	return 0;
}

int struct_NetS_DialogUpdate_send( char **pptr, int *psize, SLK_NetS_DialogUpdate *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_ui, (*psize) );
	return 0;
}

int struct_NetS_OfficialHire_send( char **pptr, int *psize, SLK_NetS_OfficialHire *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_ofkind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_ofsec, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_offree, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_ofquick, (*psize) );
	return 0;
}

int struct_NetS_Building_send( char **pptr, int *psize, SLK_NetS_Building *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_offset, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_sec, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_needsec, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_quick, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_overvalue, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_value, (*psize) );
	return 0;
}

int struct_NetS_BuildingBarracks_send( char **pptr, int *psize, SLK_NetS_BuildingBarracks *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_offset, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_sec, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_needsec, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_quick, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_overvalue, (*psize) );
	return 0;
}

int struct_NetS_BuildingRes_send( char **pptr, int *psize, SLK_NetS_BuildingRes *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_offset, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_level, (*psize) );
	return 0;
}

int struct_NetS_BuildingList_send( char **pptr, int *psize, SLK_NetS_BuildingList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_building_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_building_count; tmpi++ )
	{
		struct_NetS_Building_send( pptr, psize, &pValue->m_building[tmpi] );
	}
	LKSET_WORD_SEND( (*pptr), &pValue->m_barracks_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_barracks_count; tmpi++ )
	{
		struct_NetS_BuildingBarracks_send( pptr, psize, &pValue->m_barracks[tmpi] );
	}
	LKSET_WORD_SEND( (*pptr), &pValue->m_res_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_res_count; tmpi++ )
	{
		struct_NetS_BuildingRes_send( pptr, psize, &pValue->m_res[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_levynum, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_worker_kind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_worker_offset, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_worker_op, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_worker_sec, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_worker_needsec, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_worker_free, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_worker_kind_ex, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_worker_offset_ex, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_worker_op_ex, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_worker_sec_ex, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_worker_needsec_ex, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_worker_free_ex, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_worker_expire_ex, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_function, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_forgingkind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_forgingsec, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_forgingsec_need, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_wnquick, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_wnquick_ex, (*psize) );
	return 0;
}

int struct_NetS_ActorInfo_send( char **pptr, int *psize, SLK_NetS_ActorInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_name, 22*sizeof(char), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_shape, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_exp, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_exp_max, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_token, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_viplevel, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_vipexp, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_vipexp_max, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_body, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_place, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_official, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_zone, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_battlepower, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_silver, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_wood, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_food, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_iron, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_infantry_num, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_cavalry_num, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_archer_num, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_mokilllv, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_sflag, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_autobuild, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_autobuildopen, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_techlevel, 40*sizeof(short), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_techprogress, 40*sizeof(short), (*psize) );
	for( tmpi = 0; tmpi < 3; tmpi++ )
	{
		struct_NetS_OfficialHire_send( pptr, psize, &pValue->m_officialhire[tmpi] );
	}
	LKSET_DWORD_SEND( (*pptr), &pValue->m_ptsec, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_forgingkind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_forgingsec, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_actor_sflag, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_bodysec, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_game_day, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_game_weather, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_state, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_guardnum, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_guardsec, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_questid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_permission, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_buff_endtime, 6*sizeof(int), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_autoguard, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_autoguardopen, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_storyid, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posy, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_open_town6, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_open_townking, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_game_day_loop, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_open_town3, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_shape_bag, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_bufftrain, (*psize) );
	return 0;
}

int struct_Corrdinate_send( char **pptr, int *psize, SLK_Corrdinate *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posy, (*psize) );
	return 0;
}

int struct_NetS_ItemUse_send( char **pptr, int *psize, SLK_NetS_ItemUse *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_itemoffset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_usenum, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_effres, (*psize) );
	return 0;
}

int struct_NetS_ItemPut_send( char **pptr, int *psize, SLK_NetS_ItemPut *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_res_offset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_res_num, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_target_offset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_target_num, (*psize) );
	return 0;
}

int struct_NetS_ItemSettle_send( char **pptr, int *psize, SLK_NetS_ItemSettle *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_itemnum, (*psize) );
	if( pValue->m_itemnum > 0 && pValue->m_itemnum <= 100 )
		LKSET_MEM_SEND( (*pptr), pValue->m_itemoffset, pValue->m_itemnum*sizeof(short), (*psize) );
	return 0;
}

int struct_NetS_LostItem_send( char **pptr, int *psize, SLK_NetS_LostItem *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_itemoffset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_itemnum, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_targetid, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_GetItem_send( char **pptr, int *psize, SLK_NetS_GetItem *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_itemoffset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_color, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_num, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_targetid, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_situation, (*psize) );
	return 0;
}

int struct_ItemSmpInfo_send( char **pptr, int *psize, SLK_ItemSmpInfo *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_offset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_num, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_situation, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_color_level, (*psize) );
	return 0;
}

int struct_NetS_ItemList_send( char **pptr, int *psize, SLK_NetS_ItemList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_itemext, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_kindnum, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_kindnum; tmpi++ )
	{
		struct_ItemSmpInfo_send( pptr, psize, &pValue->m_item[tmpi] );
	}
	return 0;
}

int struct_ItemAttr_send( char **pptr, int *psize, SLK_ItemAttr *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_ability, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_value, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_addvalue, (*psize) );
	return 0;
}

int struct_NetS_ItemInfo_send( char **pptr, int *psize, SLK_NetS_ItemInfo *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_itemoffset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_itemkind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_color_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_price, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_attr_num, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_attr_num; tmpi++ )
	{
		struct_ItemAttr_send( pptr, psize, &pValue->m_attr[tmpi] );
	}
	return 0;
}

int struct_WalkPath_send( char **pptr, int *psize, SLK_WalkPath *pValue )
{
	int tmpi = 0;

	LKSET_MEM_SEND( (*pptr), pValue->m_pathx, 100*sizeof(short), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_pathy, 100*sizeof(short), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_len, (*psize) );
	return 0;
}

int struct_NetS_ZoneUnit_send( char **pptr, int *psize, SLK_NetS_ZoneUnit *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posy, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_zoneunit_index, (*psize) );
	return 0;
}

int struct_NetS_AddMapUnit_send( char **pptr, int *psize, SLK_NetS_AddMapUnit *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_state, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posx, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_unit_index, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posy, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_namelen, (*psize) );
	if( pValue->m_namelen > 0 && pValue->m_namelen <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_namelen*sizeof(char), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_char_value_count, (*psize) );
	if( pValue->m_char_value_count > 0 && pValue->m_char_value_count <= 16 )
		LKSET_MEM_SEND( (*pptr), pValue->m_char_value, pValue->m_char_value_count*sizeof(char), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_short_value_count, (*psize) );
	if( pValue->m_short_value_count > 0 && pValue->m_short_value_count <= 16 )
		LKSET_MEM_SEND( (*pptr), pValue->m_short_value, pValue->m_short_value_count*sizeof(short), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_int_value_count, (*psize) );
	if( pValue->m_int_value_count > 0 && pValue->m_int_value_count <= 16 )
		LKSET_MEM_SEND( (*pptr), pValue->m_int_value, pValue->m_int_value_count*sizeof(int), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_prefixlen, (*psize) );
	if( pValue->m_prefixlen > 0 && pValue->m_prefixlen <= 22 )
		LKSET_MEM_SEND( (*pptr), pValue->m_prefix, pValue->m_prefixlen*sizeof(char), (*psize) );
	return 0;
}

int struct_NetS_DelMapUnit_send( char **pptr, int *psize, SLK_NetS_DelMapUnit *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_unit_index, (*psize) );
	return 0;
}

int struct_NetS_WorldMapInfo_send( char **pptr, int *psize, SLK_NetS_WorldMapInfo *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_area_width, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_area_height, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_map_area_xnum, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_map_area_ynum, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_my_city_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_my_city_posy, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_my_city_unit_index, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_citystate, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_target_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_target_posy, (*psize) );
	return 0;
}

int struct_NetS_MapUnitCorrdinate_send( char **pptr, int *psize, SLK_NetS_MapUnitCorrdinate *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_unit_index, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posy, (*psize) );
	return 0;
}

int struct_NetS_AddMarchRoute_send( char **pptr, int *psize, SLK_NetS_AddMarchRoute *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_from_type, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_from_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_from_posy, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_to_type, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_to_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_to_posy, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_state, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_from_actorid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_from_nation, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_to_actorid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_to_nation, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_army_index, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_action, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_from_grid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_to_grid, (*psize) );
	return 0;
}

int struct_NetS_DelMarchRoute_send( char **pptr, int *psize, SLK_NetS_DelMarchRoute *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_army_index, (*psize) );
	return 0;
}

int struct_NetS_UpdateMapUnit_send( char **pptr, int *psize, SLK_NetS_UpdateMapUnit *pValue )
{
	int tmpi = 0;

	struct_NetS_AddMapUnit_send( pptr, psize, &pValue->m_info );
	return 0;
}

int struct_NetS_ArmySpeedUpdate_send( char **pptr, int *psize, SLK_NetS_ArmySpeedUpdate *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_unit_index, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_state, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_statetime, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_stateduration, (*psize) );
	return 0;
}

int struct_NetS_Equip_send( char **pptr, int *psize, SLK_NetS_Equip *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_offset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_washid, 4*sizeof(short), (*psize) );
	return 0;
}

int struct_NetS_EquipList_send( char **pptr, int *psize, SLK_NetS_EquipList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_equipext, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_Equip_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_EquipGet_send( char **pptr, int *psize, SLK_NetS_EquipGet *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_offset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_washid, 4*sizeof(short), (*psize) );
	return 0;
}

int struct_NetS_EquipLost_send( char **pptr, int *psize, SLK_NetS_EquipLost *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_offset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_HeroEquip_send( char **pptr, int *psize, SLK_NetS_HeroEquip *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_herokind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_Equip_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_Hero_send( char **pptr, int *psize, SLK_NetS_Hero *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_color, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_corps, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_exp, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_exp_max, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_soldiers, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_state, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_attack_base, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_attack_wash, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_defense_base, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_defense_wash, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_troops_base, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_troops_wash, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_attack, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_defense, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_troops, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_attack_increase, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_defense_increase, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_offset, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_god, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_girlkind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_sonnum, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_sontime, (*psize) );
	return 0;
}

int struct_NetS_HeroList_send( char **pptr, int *psize, SLK_NetS_HeroList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_Hero_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	return 0;
}

int struct_NetS_HeroExp_send( char **pptr, int *psize, SLK_NetS_HeroExp *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_exp, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_exp_max, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_add, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_isup, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_HeroSoldiers_send( char **pptr, int *psize, SLK_NetS_HeroSoldiers *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_add, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_soldiers, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_soldiers_max, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_HeroState_send( char **pptr, int *psize, SLK_NetS_HeroState *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_state, (*psize) );
	return 0;
}

int struct_NetS_HeroReplace_send( char **pptr, int *psize, SLK_NetS_HeroReplace *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_up_kind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_down_kind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_up_offset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_down_offset, (*psize) );
	return 0;
}

int struct_NetS_HeroGet_send( char **pptr, int *psize, SLK_NetS_HeroGet *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	struct_NetS_Hero_send( pptr, psize, &pValue->m_hero );
	LKSET_WORD_SEND( (*pptr), &pValue->m_itemnum, (*psize) );
	return 0;
}

int struct_NetS_AwardInfo_send( char **pptr, int *psize, SLK_NetS_AwardInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_num, (*psize) );
	return 0;
}

int struct_NetS_AwardInfoList_send( char **pptr, int *psize, SLK_NetS_AwardInfoList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_AwardInfo_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_WORD_SEND( (*pptr), &pValue->m_callback_code, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_value, (*psize) );
	return 0;
}

int struct_NetS_Experience_send( char **pptr, int *psize, SLK_NetS_Experience *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_addexp, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_curexp, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_isup, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_expmax, (*psize) );
	return 0;
}

int struct_NetS_Body_send( char **pptr, int *psize, SLK_NetS_Body *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_total, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_add, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_bodysec, (*psize) );
	return 0;
}

int struct_NetS_Levy_send( char **pptr, int *psize, SLK_NetS_Levy *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_total, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_add, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_max, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_sec, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_Silver_send( char **pptr, int *psize, SLK_NetS_Silver *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_total, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_add, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_Wood_send( char **pptr, int *psize, SLK_NetS_Wood *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_total, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_add, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_Food_send( char **pptr, int *psize, SLK_NetS_Food *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_total, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_add, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_Iron_send( char **pptr, int *psize, SLK_NetS_Iron *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_total, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_add, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_People_send( char **pptr, int *psize, SLK_NetS_People *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_total, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_add, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_Prestige_send( char **pptr, int *psize, SLK_NetS_Prestige *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_total, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_add, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_Friendship_send( char **pptr, int *psize, SLK_NetS_Friendship *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_total, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_add, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_Vip_send( char **pptr, int *psize, SLK_NetS_Vip *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_addexp, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_curexp, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_expmax, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_isup, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_Token_send( char **pptr, int *psize, SLK_NetS_Token *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_total, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_add, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_BuildingUpgradeInfo_send( char **pptr, int *psize, SLK_NetS_BuildingUpgradeInfo *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_citylevel, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_actorlevel, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_silver, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_wood, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_food, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_iron, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_sec, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_maxlevel, (*psize) );
	return 0;
}

int struct_NetS_Worker_send( char **pptr, int *psize, SLK_NetS_Worker *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_worker_kind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_worker_offset, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_worker_op, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_worker_sec, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_worker_needsec, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_worker_free, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_worker_kind_ex, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_worker_offset_ex, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_worker_op_ex, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_worker_sec_ex, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_worker_needsec_ex, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_worker_free_ex, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_worker_expire_ex, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_wnquick, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_wnquick_ex, (*psize) );
	return 0;
}

int struct_NetS_BuildingGet_send( char **pptr, int *psize, SLK_NetS_BuildingGet *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	struct_NetS_Building_send( pptr, psize, &pValue->m_building );
	return 0;
}

int struct_NetS_BuildingBarracksGet_send( char **pptr, int *psize, SLK_NetS_BuildingBarracksGet *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	struct_NetS_BuildingBarracks_send( pptr, psize, &pValue->m_barracks );
	return 0;
}

int struct_NetS_BuildingResGet_send( char **pptr, int *psize, SLK_NetS_BuildingResGet *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	struct_NetS_BuildingRes_send( pptr, psize, &pValue->m_res );
	return 0;
}

int struct_NetS_Soldiers_send( char **pptr, int *psize, SLK_NetS_Soldiers *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_corps, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_soldiers, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_add, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_TrainInfo_send( char **pptr, int *psize, SLK_NetS_TrainInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_soldiers, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_soldiers_max, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_trainnum, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_trainsec, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_trainsec_need, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_queuenum, 16*sizeof(int), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_queue, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_trainlong, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_train_confnum, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_train_confsec, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_train_conffood, (*psize) );
	return 0;
}

int struct_NetS_Quest_send( char **pptr, int *psize, SLK_NetS_Quest *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_questid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_flag, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_datatype, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_datakind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_dataoffset, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_value, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_needvalue, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_awardkind, 5*sizeof(int), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_awardnum, 5*sizeof(int), (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_nameid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_descid, (*psize) );
	return 0;
}

int struct_NetS_QuestList_send( char **pptr, int *psize, SLK_NetS_QuestList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_Quest_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_QuestAward_send( char **pptr, int *psize, SLK_NetS_QuestAward *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_questid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_AwardInfo_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_datatype, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_datakind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_dataoffset, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_value, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_needvalue, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_nameid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	return 0;
}

int struct_NetS_Function_send( char **pptr, int *psize, SLK_NetS_Function *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_function, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_openoffset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_CityGuard_send( char **pptr, int *psize, SLK_NetS_CityGuard *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_corps, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_color, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_shape, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_soldiers, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_troops, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_offset, (*psize) );
	return 0;
}

int struct_NetS_CityGuardList_send( char **pptr, int *psize, SLK_NetS_CityGuardList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_CityGuard_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_WORD_SEND( (*pptr), &pValue->m_guardsec, (*psize) );
	return 0;
}

int struct_NetS_CityGuardSec_send( char **pptr, int *psize, SLK_NetS_CityGuardSec *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_guardsec, (*psize) );
	return 0;
}

int struct_NetS_BuildingSmithy_send( char **pptr, int *psize, SLK_NetS_BuildingSmithy *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_forgingkind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_forgingsec, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_forgingsec_need, (*psize) );
	return 0;
}

int struct_NetS_ChangeName_send( char **pptr, int *psize, SLK_NetS_ChangeName *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_name_length, (*psize) );
	if( pValue->m_name_length > 0 && pValue->m_name_length <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_name_length*sizeof(char), (*psize) );
	return 0;
}

int struct_NetS_BuildingAction_send( char **pptr, int *psize, SLK_NetS_BuildingAction *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_offset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_action, (*psize) );
	return 0;
}

int struct_NetS_LevyInfo_send( char **pptr, int *psize, SLK_NetS_LevyInfo *pValue )
{
	int tmpi = 0;

	LKSET_MEM_SEND( (*pptr), pValue->m_base, 4*sizeof(int), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_tech, 4*sizeof(int), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_weather, 4*sizeof(int), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_activity, 4*sizeof(int), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_offical, 4*sizeof(int), (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_sec, (*psize) );
	return 0;
}

int struct_NetS_Chat_send( char **pptr, int *psize, SLK_NetS_Chat *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_shape, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_namelen, (*psize) );
	if( pValue->m_namelen > 0 && pValue->m_namelen <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_namelen*sizeof(char), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_frame, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_zone, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_place, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_msglen, (*psize) );
	if( pValue->m_msglen > 0 && pValue->m_msglen <= 128 )
		LKSET_MEM_SEND( (*pptr), pValue->m_msg, pValue->m_msglen*sizeof(char), (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_optime, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_channel, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_msgtype, (*psize) );
	return 0;
}

int struct_NetS_ChatList_send( char **pptr, int *psize, SLK_NetS_ChatList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_Chat_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_channel, (*psize) );
	return 0;
}

int struct_NetS_SystalkidValue_send( char **pptr, int *psize, SLK_NetS_SystalkidValue *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_vlen, (*psize) );
	if( pValue->m_vlen > 0 && pValue->m_vlen <= 64 )
		LKSET_MEM_SEND( (*pptr), pValue->m_v, pValue->m_vlen*sizeof(char), (*psize) );
	return 0;
}

int struct_NetS_Systalkid_send( char **pptr, int *psize, SLK_NetS_Systalkid *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_SystalkidValue_send( pptr, psize, &pValue->m_msglist[tmpi] );
	}
	LKSET_DWORD_SEND( (*pptr), &pValue->m_textid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_optime, (*psize) );
	return 0;
}

int struct_NetS_Systalk_send( char **pptr, int *psize, SLK_NetS_Systalk *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_msglen, (*psize) );
	if( pValue->m_msglen > 0 && pValue->m_msglen <= 1024 )
		LKSET_MEM_SEND( (*pptr), pValue->m_msg, pValue->m_msglen*sizeof(char), (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_optime, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_roll, (*psize) );
	return 0;
}

int struct_NetS_Battlepower_send( char **pptr, int *psize, SLK_NetS_Battlepower *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_total, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_TechChange_send( char **pptr, int *psize, SLK_NetS_TechChange *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_progress, (*psize) );
	return 0;
}

int struct_NetS_CityEvent_send( char **pptr, int *psize, SLK_NetS_CityEvent *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_value, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_optime, (*psize) );
	return 0;
}

int struct_NetS_BattleEvent_send( char **pptr, int *psize, SLK_NetS_BattleEvent *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_name, 22*sizeof(char), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_value, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_optime, (*psize) );
	LKSET_LONG_SEND( (*pptr), &pValue->m_mailid, (*psize) );
	return 0;
}

int struct_NetS_EventList_send( char **pptr, int *psize, SLK_NetS_EventList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_cevent_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_cevent_count; tmpi++ )
	{
		struct_NetS_CityEvent_send( pptr, psize, &pValue->m_cevent_list[tmpi] );
	}
	LKSET_WORD_SEND( (*pptr), &pValue->m_bevent_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_bevent_count; tmpi++ )
	{
		struct_NetS_BattleEvent_send( pptr, psize, &pValue->m_bevent_list[tmpi] );
	}
	return 0;
}

int struct_NetS_OfficialHireChange_send( char **pptr, int *psize, SLK_NetS_OfficialHireChange *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	struct_NetS_OfficialHire_send( pptr, psize, &pValue->m_info );
	return 0;
}

int struct_NetS_CityProtect_send( char **pptr, int *psize, SLK_NetS_CityProtect *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_total, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_add, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_HeroWash_send( char **pptr, int *psize, SLK_NetS_HeroWash *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_hero_washnum, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_hero_washsec, (*psize) );
	return 0;
}

int struct_NetS_HeroColorup_send( char **pptr, int *psize, SLK_NetS_HeroColorup *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_value, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_isup, (*psize) );
	return 0;
}

int struct_NetS_CityAttr_send( char **pptr, int *psize, SLK_NetS_CityAttr *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_protectres_per, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_buildingsec_per, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_materialsec_per, 2*sizeof(short), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_movespeed_per, 3*sizeof(short), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_gather_per, 2*sizeof(short), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_hero_up_num, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_hero_row_num, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_everyday_body_buymax, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_everyday_autobuild_buynum, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_everyday_army_recall, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_ability_open_201, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_ability_open_203, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_ability_open_204, (*psize) );
	return 0;
}

int struct_NetS_EquipWash_send( char **pptr, int *psize, SLK_NetS_EquipWash *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_equip_washnum, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_equip_washsec, (*psize) );
	return 0;
}

int struct_NetS_MaterialInfo_send( char **pptr, int *psize, SLK_NetS_MaterialInfo *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_matkind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_matnum, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_matsec, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_matsec_need, (*psize) );
	return 0;
}

int struct_NetS_MaterialList_send( char **pptr, int *psize, SLK_NetS_MaterialList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_MaterialInfo_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_matquenum, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_nation_people, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_city_people, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_change_nation_people, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_change_city_people, (*psize) );
	return 0;
}

int struct_NetS_MaterialWillInfo_send( char **pptr, int *psize, SLK_NetS_MaterialWillInfo *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_matkind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_matsec, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_matkind_will, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_matnum_will, (*psize) );
	return 0;
}

int struct_NetS_MaterialWillList_send( char **pptr, int *psize, SLK_NetS_MaterialWillList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_MaterialWillInfo_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_StoryList_send( char **pptr, int *psize, SLK_NetS_StoryList *pValue )
{
	int tmpi = 0;

	LKSET_MEM_SEND( (*pptr), pValue->m_story_star, 128*sizeof(short), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_story_hero, 32*sizeof(short), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_story_restime, 32*sizeof(int), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_story_resnum, 32*sizeof(short), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_story_resreset, 32*sizeof(short), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_story_itemnum, 64*sizeof(short), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_story_drawing, 16*sizeof(short), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_sweep_herokind, 4*sizeof(short), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_storyid, (*psize) );
	return 0;
}

int struct_NetS_MonsterInfo_send( char **pptr, int *psize, SLK_NetS_MonsterInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_monsterid, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_shape, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_color, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_corps, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_hp, (*psize) );
	return 0;
}

int struct_NetS_StoryRank_send( char **pptr, int *psize, SLK_NetS_StoryRank *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_MonsterInfo_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_DWORD_SEND( (*pptr), &pValue->m_exp, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_body, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	return 0;
}

int struct_NetS_StoryState_send( char **pptr, int *psize, SLK_NetS_StoryState *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_storyid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_state, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_saveoffset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_savetype, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_actor_storyid, (*psize) );
	return 0;
}

int struct_NetS_StoryRanknum_send( char **pptr, int *psize, SLK_NetS_StoryRanknum *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_storyid, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_num, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_saveoffset, (*psize) );
	return 0;
}

int struct_NetS_StoryRanktime_send( char **pptr, int *psize, SLK_NetS_StoryRanktime *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_storyid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_time, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_saveoffset, (*psize) );
	return 0;
}

int struct_NetS_MapZoneChange_send( char **pptr, int *psize, SLK_NetS_MapZoneChange *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_zoneid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_open, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	return 0;
}

int struct_NetS_MapZoneUnitList_send( char **pptr, int *psize, SLK_NetS_MapZoneUnitList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_ZoneUnit_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_zoneid, (*psize) );
	return 0;
}

int struct_NetS_BattleInfo_send( char **pptr, int *psize, SLK_NetS_BattleInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_army_index, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_unit_index, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_state, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_statetime, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_stateduration, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_action, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_to_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_to_posy, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_herokind, 4*sizeof(short), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_to_type, (*psize) );
	return 0;
}

int struct_NetS_BattleList_send( char **pptr, int *psize, SLK_NetS_BattleList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_BattleInfo_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_DWORD_SEND( (*pptr), &pValue->m_unit_index, (*psize) );
	return 0;
}

int struct_NetS_MapResInfo_send( char **pptr, int *psize, SLK_NetS_MapResInfo *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_totalnum, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_totalsec, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_spacenum, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_herokind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_herolevel, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_herohp, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_herocolor, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_actorlevel, (*psize) );
	return 0;
}

int struct_NetS_WeatherChange_send( char **pptr, int *psize, SLK_NetS_WeatherChange *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_game_day, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_game_weather, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_game_day_loop, (*psize) );
	return 0;
}

int struct_NetS_Mail_send( char **pptr, int *psize, SLK_NetS_Mail *pValue )
{
	int tmpi = 0;

	LKSET_LONG_SEND( (*pptr), &pValue->m_mailid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_title_len, (*psize) );
	if( pValue->m_title_len > 0 && pValue->m_title_len <= 128 )
		LKSET_MEM_SEND( (*pptr), pValue->m_title, pValue->m_title_len*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_content_len, (*psize) );
	if( pValue->m_content_len > 0 && pValue->m_content_len <= 1024 )
		LKSET_MEM_SEND( (*pptr), pValue->m_content, pValue->m_content_len*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_attach_len, (*psize) );
	if( pValue->m_attach_len > 0 && pValue->m_attach_len <= 256 )
		LKSET_MEM_SEND( (*pptr), pValue->m_attach, pValue->m_attach_len*sizeof(char), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_attachget, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_read, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_recvtime, (*psize) );
	LKSET_LONG_SEND( (*pptr), &pValue->m_fightid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_lock, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_viewpath, (*psize) );
	return 0;
}

int struct_NetS_MailOpResult_send( char **pptr, int *psize, SLK_NetS_MailOpResult *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_op, (*psize) );
	LKSET_LONG_SEND( (*pptr), &pValue->m_mailid, (*psize) );
	return 0;
}

int struct_NetS_MailFight_send( char **pptr, int *psize, SLK_NetS_MailFight *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_flag, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_content_length, (*psize) );
	if( pValue->m_content_length > 0 && pValue->m_content_length <= 1800 )
		LKSET_MEM_SEND( (*pptr), pValue->m_content, pValue->m_content_length*sizeof(char), (*psize) );
	LKSET_LONG_SEND( (*pptr), &pValue->m_mailid, (*psize) );
	return 0;
}

int struct_NetS_MailView_send( char **pptr, int *psize, SLK_NetS_MailView *pValue )
{
	int tmpi = 0;

	struct_NetS_Mail_send( pptr, psize, &pValue->m_mail );
	return 0;
}

int struct_NetS_CItyHelp_send( char **pptr, int *psize, SLK_NetS_CItyHelp *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_actorname_length, (*psize) );
	if( pValue->m_actorname_length > 0 && pValue->m_actorname_length <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_actorname, pValue->m_actorname_length*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_herokind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_soldiers, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_army_index, (*psize) );
	return 0;
}

int struct_NetS_CItyHelpList_send( char **pptr, int *psize, SLK_NetS_CItyHelpList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_CItyHelp_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_WORD_SEND( (*pptr), &pValue->m_walllevel, (*psize) );
	return 0;
}

int struct_NetS_MapCItyHelpList_send( char **pptr, int *psize, SLK_NetS_MapCItyHelpList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_CItyHelp_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_WORD_SEND( (*pptr), &pValue->m_walllevel, (*psize) );
	return 0;
}

int struct_NetS_CityState_send( char **pptr, int *psize, SLK_NetS_CityState *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_state, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_change, (*psize) );
	return 0;
}

int struct_NetS_CityArmyGroup_send( char **pptr, int *psize, SLK_NetS_CityArmyGroup *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_group_index, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_group_id, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_attack, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_statetime, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_stateduration, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_t_nation, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_t_level, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_name_length, (*psize) );
	if( pValue->m_name_length > 0 && pValue->m_name_length <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_name_length*sizeof(char), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_t_name_length, (*psize) );
	if( pValue->m_t_name_length > 0 && pValue->m_t_name_length <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_t_name, pValue->m_t_name_length*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posy, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_t_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_t_posy, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_t_actorid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_total, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_t_total, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	return 0;
}

int struct_NetS_CityArmyGroupList_send( char **pptr, int *psize, SLK_NetS_CityArmyGroupList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_CityArmyGroup_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_unit_index, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_totalcount, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_flag, (*psize) );
	return 0;
}

int struct_NetS_MapTownInfo_send( char **pptr, int *psize, SLK_NetS_MapTownInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_protect_sec, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_produce_sec, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_own_actorid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_own_namelen, (*psize) );
	if( pValue->m_own_namelen > 0 && pValue->m_own_namelen <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_own_name, pValue->m_own_namelen*sizeof(char), (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_own_sec, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_hp, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_maxhp, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_myask, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_produce_num, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_townid, (*psize) );
	return 0;
}

int struct_NetS_TownArmyGroup_send( char **pptr, int *psize, SLK_NetS_TownArmyGroup *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_group_index, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_group_id, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_attack, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_statetime, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_stateduration, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_t_nation, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_total, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_t_total, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	return 0;
}

int struct_NetS_TownArmyGroupList_send( char **pptr, int *psize, SLK_NetS_TownArmyGroupList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_TownArmyGroup_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_flag, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_unit_index, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_totalcount, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_townid, (*psize) );
	return 0;
}

int struct_NetS_SystalkJson_send( char **pptr, int *psize, SLK_NetS_SystalkJson *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_msglen, (*psize) );
	if( pValue->m_msglen > 0 && pValue->m_msglen <= 1024 )
		LKSET_MEM_SEND( (*pptr), pValue->m_msg, pValue->m_msglen*sizeof(char), (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_optime, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_roll, (*psize) );
	return 0;
}

int struct_NetS_RollMsgJson_send( char **pptr, int *psize, SLK_NetS_RollMsgJson *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_msglen, (*psize) );
	if( pValue->m_msglen > 0 && pValue->m_msglen <= 1024 )
		LKSET_MEM_SEND( (*pptr), pValue->m_msg, pValue->m_msglen*sizeof(char), (*psize) );
	return 0;
}

int struct_NetS_RollMsg_send( char **pptr, int *psize, SLK_NetS_RollMsg *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_msglen, (*psize) );
	if( pValue->m_msglen > 0 && pValue->m_msglen <= 1024 )
		LKSET_MEM_SEND( (*pptr), pValue->m_msg, pValue->m_msglen*sizeof(char), (*psize) );
	return 0;
}

int struct_NetS_TownOwnerAsk_send( char **pptr, int *psize, SLK_NetS_TownOwnerAsk *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_name_len, (*psize) );
	if( pValue->m_name_len > 0 && pValue->m_name_len <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_name_len*sizeof(char), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_place, (*psize) );
	return 0;
}

int struct_NetS_TownOwnerAskList_send( char **pptr, int *psize, SLK_NetS_TownOwnerAskList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_TownOwnerAsk_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_DWORD_SEND( (*pptr), &pValue->m_sec, (*psize) );
	return 0;
}

int struct_NetS_TownFight_send( char **pptr, int *psize, SLK_NetS_TownFight *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_townid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_statetime, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_attack, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	return 0;
}

int struct_NetS_TownFightList_send( char **pptr, int *psize, SLK_NetS_TownFightList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_TownFight_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_MapTownExInfo_send( char **pptr, int *psize, SLK_NetS_MapTownExInfo *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_dev_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_dev_exp, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_dev_expmax, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_mytownid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_dev_cd, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_attackcd, (*psize) );
	return 0;
}

int struct_NetS_MapZoneTown_send( char **pptr, int *psize, SLK_NetS_MapZoneTown *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_townid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_protect_sec, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_from_nation, 4*sizeof(short), (*psize) );
	return 0;
}

int struct_NetS_MapZoneTownList_send( char **pptr, int *psize, SLK_NetS_MapZoneTownList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_MapZoneTown_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_zoneid, (*psize) );
	return 0;
}

int struct_NetS_MapCenterTown_send( char **pptr, int *psize, SLK_NetS_MapCenterTown *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_townid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	return 0;
}

int struct_NetS_MapCenterTownList_send( char **pptr, int *psize, SLK_NetS_MapCenterTownList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_MapCenterTown_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_CityWarInfo_send( char **pptr, int *psize, SLK_NetS_CityWarInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_group_index, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_group_id, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_from_nation, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_from_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_from_posy, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_namelen, (*psize) );
	if( pValue->m_namelen > 0 && pValue->m_namelen <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_namelen*sizeof(char), (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_statetime, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_stateduration, (*psize) );
	return 0;
}

int struct_NetS_CityWarList_send( char **pptr, int *psize, SLK_NetS_CityWarList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_CityWarInfo_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_CityWarDel_send( char **pptr, int *psize, SLK_NetS_CityWarDel *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_group_index, (*psize) );
	return 0;
}

int struct_NetS_WorldQuest_send( char **pptr, int *psize, SLK_NetS_WorldQuest *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_questid, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_value, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_maxvalue, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_complete, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_dd, (*psize) );
	return 0;
}

int struct_NetS_WorldBoss_send( char **pptr, int *psize, SLK_NetS_WorldBoss *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_bossid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_hp, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_maxhp, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_isfight, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_actorlevel, (*psize) );
	return 0;
}

int struct_NetS_LostRebuild_send( char **pptr, int *psize, SLK_NetS_LostRebuild *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_rb_silver, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_rb_wood, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_rb_food, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_rb_df, (*psize) );
	return 0;
}

int struct_NetS_KingwarTown_send( char **pptr, int *psize, SLK_NetS_KingwarTown *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_id, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_attack_total, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_defense_total, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	return 0;
}

int struct_NetS_KingwarTownList_send( char **pptr, int *psize, SLK_NetS_KingwarTownList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_KingwarTown_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_DWORD_SEND( (*pptr), &pValue->m_leftstamp, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_losthp, (*psize) );
	return 0;
}

int struct_NetS_KingWarActivity_send( char **pptr, int *psize, SLK_NetS_KingWarActivity *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_state, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_beginstamp, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_endstamp, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	return 0;
}

int struct_NetS_KingWarNotify_send( char **pptr, int *psize, SLK_NetS_KingWarNotify *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_a_heroid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_a_color, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_a_name_len, (*psize) );
	if( pValue->m_a_name_len > 0 && pValue->m_a_name_len <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_a_name, pValue->m_a_name_len*sizeof(char), (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_a_losthp, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_a_nation, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_d_heroid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_d_color, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_d_name_len, (*psize) );
	if( pValue->m_d_name_len > 0 && pValue->m_d_name_len <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_d_name, pValue->m_d_name_len*sizeof(char), (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_d_losthp, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_d_nation, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_result, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_id, (*psize) );
	return 0;
}

int struct_NetS_KingWarNotifyList_send( char **pptr, int *psize, SLK_NetS_KingWarNotifyList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_KingWarNotify_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_KingWarRank_send( char **pptr, int *psize, SLK_NetS_KingWarRank *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_rank, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_kill, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_name_len, (*psize) );
	if( pValue->m_name_len > 0 && pValue->m_name_len <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_name_len*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_herokind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_herocolor, (*psize) );
	return 0;
}

int struct_NetS_KingWarRankList_send( char **pptr, int *psize, SLK_NetS_KingWarRankList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_KingWarRank_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_DWORD_SEND( (*pptr), &pValue->m_mypoint, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_totalkill, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_myrank, (*psize) );
	return 0;
}

int struct_NetS_KingWarPK_send( char **pptr, int *psize, SLK_NetS_KingWarPK *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_herokind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_hp, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_id, (*psize) );
	return 0;
}

int struct_NetS_KingWarPoint_send( char **pptr, int *psize, SLK_NetS_KingWarPoint *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_point, (*psize) );
	return 0;
}

int struct_NetS_TreasureActivity_send( char **pptr, int *psize, SLK_NetS_TreasureActivity *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_state, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_endstamp, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_treasure_num, 3*sizeof(short), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_treasure_num_max, (*psize) );
	return 0;
}

int struct_NetS_TreasureHas_send( char **pptr, int *psize, SLK_NetS_TreasureHas *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_has, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_px, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_py, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_tn, (*psize) );
	return 0;
}

int struct_NetS_GotoAsyn_send( char **pptr, int *psize, SLK_NetS_GotoAsyn *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posy, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_unit_index, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_op, (*psize) );
	return 0;
}

int struct_NetS_TreasureActor_send( char **pptr, int *psize, SLK_NetS_TreasureActor *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_itemkind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_name_len, (*psize) );
	if( pValue->m_name_len > 0 && pValue->m_name_len <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_name_len*sizeof(char), (*psize) );
	return 0;
}

int struct_NetS_TreasureActorList_send( char **pptr, int *psize, SLK_NetS_TreasureActorList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_TreasureActor_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_co, (*psize) );
	return 0;
}

int struct_NetS_ChangeAutoBuild_send( char **pptr, int *psize, SLK_NetS_ChangeAutoBuild *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_autobuild, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_autobuildopen, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_HeroVisitInfo_send( char **pptr, int *psize, SLK_NetS_HeroVisitInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_hv_free_cd, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_hv_high_sec, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_hv_high_free, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_hv_low_num, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_hv_high_num, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_hv_progress, (*psize) );
	return 0;
}

int struct_NetS_NationBase_send( char **pptr, int *psize, SLK_NetS_NationBase *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_exp, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_donate_num, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_myrank, (*psize) );
	return 0;
}

int struct_NetS_HeroVisitAward_send( char **pptr, int *psize, SLK_NetS_HeroVisitAward *pValue )
{
	int tmpi = 0;

	LKSET_MEM_SEND( (*pptr), pValue->m_id, 10*sizeof(short), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	return 0;
}

int struct_NetS_FightPlay_send( char **pptr, int *psize, SLK_NetS_FightPlay *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_flag, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_content_length, (*psize) );
	if( pValue->m_content_length > 0 && pValue->m_content_length <= 1800 )
		LKSET_MEM_SEND( (*pptr), pValue->m_content, pValue->m_content_length*sizeof(char), (*psize) );
	return 0;
}

int struct_NetS_StorySweepHero_send( char **pptr, int *psize, SLK_NetS_StorySweepHero *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_color, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_pre_exp, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_exp, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	return 0;
}

int struct_NetS_StorySweepResult_send( char **pptr, int *psize, SLK_NetS_StorySweepResult *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_exp, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_silver, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_awardcount, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_awardcount; tmpi++ )
	{
		struct_NetS_AwardInfo_send( pptr, psize, &pValue->m_award[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_herocount, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_herocount; tmpi++ )
	{
		struct_NetS_StorySweepHero_send( pptr, psize, &pValue->m_hero[tmpi] );
	}
	return 0;
}

int struct_NetS_QuestTalk_send( char **pptr, int *psize, SLK_NetS_QuestTalk *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_talkid, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_herokind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_talk_textid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_btn_textid, (*psize) );
	return 0;
}

int struct_NetS_RankInfo_send( char **pptr, int *psize, SLK_NetS_RankInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_rank, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_namelen, (*psize) );
	if( pValue->m_namelen > 0 && pValue->m_namelen <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_namelen*sizeof(char), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_place, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_battlepower, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	return 0;
}

int struct_NetS_RankList_send( char **pptr, int *psize, SLK_NetS_RankList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_RankInfo_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_page, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_myrank, (*psize) );
	return 0;
}

int struct_NetS_FriendInfo_send( char **pptr, int *psize, SLK_NetS_FriendInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_city_index, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_shape, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_namelen, (*psize) );
	if( pValue->m_namelen > 0 && pValue->m_namelen <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_namelen*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_place, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_battlepower, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_ask, (*psize) );
	return 0;
}

int struct_NetS_FriendList_send( char **pptr, int *psize, SLK_NetS_FriendList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_FriendInfo_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_op, (*psize) );
	return 0;
}

int struct_NetS_ActorSearch_send( char **pptr, int *psize, SLK_NetS_ActorSearch *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_city_index, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_namelen, (*psize) );
	if( pValue->m_namelen > 0 && pValue->m_namelen <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_namelen*sizeof(char), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_signlen, (*psize) );
	if( pValue->m_signlen > 0 && pValue->m_signlen <= 128 )
		LKSET_MEM_SEND( (*pptr), pValue->m_sign, pValue->m_signlen*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_official, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_place, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_zone, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_battlepower, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_killcity, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_herocount, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_herocount; tmpi++ )
	{
		struct_NetS_StorySweepHero_send( pptr, psize, &pValue->m_hero[tmpi] );
	}
	LKSET_DWORD_SEND( (*pptr), &pValue->m_bp_hero, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_bp_equip, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_bp_tech, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_bp_nequip, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_bp_girl, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_bp_place, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_my_bp_hero, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_my_bp_equip, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_my_bp_tech, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_my_bp_nequip, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_my_bp_girl, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_my_bp_place, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_shape, (*psize) );
	return 0;
}

int struct_NetS_BlackInfo_send( char **pptr, int *psize, SLK_NetS_BlackInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_namelen, (*psize) );
	if( pValue->m_namelen > 0 && pValue->m_namelen <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_namelen*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_shape, (*psize) );
	return 0;
}

int struct_NetS_BlackList_send( char **pptr, int *psize, SLK_NetS_BlackList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_BlackInfo_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_op, (*psize) );
	return 0;
}

int struct_NetS_BlackListID_send( char **pptr, int *psize, SLK_NetS_BlackListID *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	if( pValue->m_count > 0 && pValue->m_count <= 50 )
		LKSET_MEM_SEND( (*pptr), pValue->m_actorid, pValue->m_count*sizeof(int), (*psize) );
	return 0;
}

int struct_NetS_NationEquip_send( char **pptr, int *psize, SLK_NetS_NationEquip *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_neq_lv, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_neq_exp, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_neq_pro, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_neq_sec, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_neq_crit, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_neq_star, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_kind, (*psize) );
	return 0;
}

int struct_NetS_ShopItem_send( char **pptr, int *psize, SLK_NetS_ShopItem *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_offset, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_awardkind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_awardnum, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_token, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_sale, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_buyuse, (*psize) );
	return 0;
}

int struct_NetS_ShopList_send( char **pptr, int *psize, SLK_NetS_ShopList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_ShopItem_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_PayStoreGoods_send( char **pptr, int *psize, SLK_NetS_PayStoreGoods *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_goodsid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_price, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_token, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_day, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_nameid, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_icon, (*psize) );
	return 0;
}

int struct_NetS_PayStore_send( char **pptr, int *psize, SLK_NetS_PayStore *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_PayStoreGoods_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_GoodsInfo_send( char **pptr, int *psize, SLK_NetS_GoodsInfo *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_goodsid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_price, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_nameid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_descid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_icon, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_sale, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_worth, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_bag_time, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_bag_num, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_awardcount, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_awardcount; tmpi++ )
	{
		struct_NetS_AwardInfo_send( pptr, psize, &pValue->m_award[tmpi] );
	}
	return 0;
}

int struct_NetS_PayStoreActivity_send( char **pptr, int *psize, SLK_NetS_PayStoreActivity *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_GoodsInfo_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_PayOrder_send( char **pptr, int *psize, SLK_NetS_PayOrder *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_orderid_len, (*psize) );
	if( pValue->m_orderid_len > 0 && pValue->m_orderid_len <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_orderid, pValue->m_orderid_len*sizeof(char), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_ext_len, (*psize) );
	if( pValue->m_ext_len > 0 && pValue->m_ext_len <= 64 )
		LKSET_MEM_SEND( (*pptr), pValue->m_ext, pValue->m_ext_len*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_goodsid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_productid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_nameid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_descid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_price, (*psize) );
	return 0;
}

int struct_NetS_VipShopItem_send( char **pptr, int *psize, SLK_NetS_VipShopItem *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_awardkind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_awardnum, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_token, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_itemkind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_vip_token, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_vip_buynum, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_vip_buynum_max, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_id, (*psize) );
	return 0;
}

int struct_NetS_VipShop_send( char **pptr, int *psize, SLK_NetS_VipShop *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_VipShopItem_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_useitem, (*psize) );
	return 0;
}

int struct_NetS_VipBag_send( char **pptr, int *psize, SLK_NetS_VipBag *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_vipbag, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_vipbag_count, (*psize) );
	return 0;
}

int struct_NetS_BuffChange_send( char **pptr, int *psize, SLK_NetS_BuffChange *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_buffkind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_endtime, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_bufftrain, (*psize) );
	return 0;
}

int struct_NetS_ChangeAutoGuard_send( char **pptr, int *psize, SLK_NetS_ChangeAutoGuard *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_autoguard, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_autoguardopen, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_Activity_send( char **pptr, int *psize, SLK_NetS_Activity *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_activityid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_starttime, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_endtime, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_closetime, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_red, (*psize) );
	return 0;
}

int struct_NetS_ActivityList_send( char **pptr, int *psize, SLK_NetS_ActivityList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_Activity_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_NationEquipList_send( char **pptr, int *psize, SLK_NetS_NationEquipList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_NationEquip_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_NationInfo_send( char **pptr, int *psize, SLK_NetS_NationInfo *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_exp, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_donate_num, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_myrank, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_notice_len, (*psize) );
	if( pValue->m_notice_len > 0 && pValue->m_notice_len <= 512 )
		LKSET_MEM_SEND( (*pptr), pValue->m_notice, pValue->m_notice_len*sizeof(char), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_kingname_len, (*psize) );
	if( pValue->m_kingname_len > 0 && pValue->m_kingname_len <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_kingname, pValue->m_kingname_len*sizeof(char), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_questvalue, 3*sizeof(short), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_questvalue_max, 3*sizeof(short), (*psize) );
	return 0;
}

int struct_NetS_NationTown_send( char **pptr, int *psize, SLK_NetS_NationTown *pValue )
{
	int tmpi = 0;

	struct_NetS_MapTownInfo_send( pptr, psize, &pValue->m_info );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_NationTownList_send( char **pptr, int *psize, SLK_NetS_NationTownList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_NationTown_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_op, (*psize) );
	return 0;
}

int struct_NetS_NationWar_send( char **pptr, int *psize, SLK_NetS_NationWar *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_group_index, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_group_id, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_attack, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_statetime, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_stateduration, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_t_nation, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_total, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_t_total, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_townid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_unit_index, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_town_nation, (*psize) );
	return 0;
}

int struct_NetS_NationWarList_send( char **pptr, int *psize, SLK_NetS_NationWarList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_NationWar_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_op, (*psize) );
	return 0;
}

int struct_NetS_NationCityWar_send( char **pptr, int *psize, SLK_NetS_NationCityWar *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_group_index, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_group_id, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_attack, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_statetime, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_stateduration, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_t_nation, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_t_level, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_name_length, (*psize) );
	if( pValue->m_name_length > 0 && pValue->m_name_length <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_name_length*sizeof(char), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_t_name_length, (*psize) );
	if( pValue->m_t_name_length > 0 && pValue->m_t_name_length <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_t_name, pValue->m_t_name_length*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posy, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_t_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_t_posy, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_t_actorid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_total, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_t_total, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_unit_index, (*psize) );
	return 0;
}

int struct_NetS_NationCityWarList_send( char **pptr, int *psize, SLK_NetS_NationCityWarList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_NationCityWar_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_op, (*psize) );
	return 0;
}

int struct_NetS_NationQuest_send( char **pptr, int *psize, SLK_NetS_NationQuest *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_value, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_needvalue, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_awardkind, 5*sizeof(int), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_awardnum, 5*sizeof(int), (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_other_awardkind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_other_awardnum, (*psize) );
	return 0;
}

int struct_NetS_NationQuestList_send( char **pptr, int *psize, SLK_NetS_NationQuestList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_NationQuest_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_NationMission_send( char **pptr, int *psize, SLK_NetS_NationMission *pValue )
{
	int tmpi = 0;

	LKSET_MEM_SEND( (*pptr), pValue->m_value, 3*sizeof(int), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_needvalue, 3*sizeof(int), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_awardkind, 2*sizeof(int), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_awardnum, 2*sizeof(int), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_isget, (*psize) );
	return 0;
}

int struct_NetS_NationMissionList_send( char **pptr, int *psize, SLK_NetS_NationMissionList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_NationMission_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_NationRankMember_send( char **pptr, int *psize, SLK_NetS_NationRankMember *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_name_len, (*psize) );
	if( pValue->m_name_len > 0 && pValue->m_name_len <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_name_len*sizeof(char), (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_value, (*psize) );
	return 0;
}

int struct_NetS_NationRankList_send( char **pptr, int *psize, SLK_NetS_NationRankList *pValue )
{
	int tmpi = 0;

	for( tmpi = 0; tmpi < 5; tmpi++ )
	{
		struct_NetS_NationRankMember_send( pptr, psize, &pValue->m_buildrank[tmpi] );
	}
	for( tmpi = 0; tmpi < 5; tmpi++ )
	{
		struct_NetS_NationRankMember_send( pptr, psize, &pValue->m_cityrank[tmpi] );
	}
	for( tmpi = 0; tmpi < 5; tmpi++ )
	{
		struct_NetS_NationRankMember_send( pptr, psize, &pValue->m_townrank[tmpi] );
	}
	LKSET_WORD_SEND( (*pptr), &pValue->m_vote, (*psize) );
	return 0;
}

int struct_NetS_NationOfficial_send( char **pptr, int *psize, SLK_NetS_NationOfficial *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_official, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_namelen, (*psize) );
	if( pValue->m_namelen > 0 && pValue->m_namelen <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_namelen*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_battlepower, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_zone, (*psize) );
	return 0;
}

int struct_NetS_NationOfficialList_send( char **pptr, int *psize, SLK_NetS_NationOfficialList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_NationOfficial_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_DWORD_SEND( (*pptr), &pValue->m_endtime, (*psize) );
	return 0;
}

int struct_NetS_NationCandidate_send( char **pptr, int *psize, SLK_NetS_NationCandidate *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_namelen, (*psize) );
	if( pValue->m_namelen > 0 && pValue->m_namelen <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_namelen*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_battlepower, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_ballot, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	return 0;
}

int struct_NetS_NationCandidateList_send( char **pptr, int *psize, SLK_NetS_NationCandidateList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_NationCandidate_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_DWORD_SEND( (*pptr), &pValue->m_endtime, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_myvote, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_isballot, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_tokenballot, (*psize) );
	return 0;
}

int struct_NetS_WorldDataOpen_send( char **pptr, int *psize, SLK_NetS_WorldDataOpen *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_open_town3, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_open_town6, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_open_townking, (*psize) );
	return 0;
}

int struct_NetS_NationReplace_send( char **pptr, int *psize, SLK_NetS_NationReplace *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_namelen, (*psize) );
	if( pValue->m_namelen > 0 && pValue->m_namelen <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_namelen*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_battlepower, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_place, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_official, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	return 0;
}

int struct_NetS_NationReplaceList_send( char **pptr, int *psize, SLK_NetS_NationReplaceList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_NationReplace_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_op, (*psize) );
	return 0;
}

int struct_NetS_NationLog_send( char **pptr, int *psize, SLK_NetS_NationLog *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_townid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_namelen, (*psize) );
	if( pValue->m_namelen > 0 && pValue->m_namelen <= 22 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_namelen*sizeof(char), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_target_nation, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_optime, (*psize) );
	return 0;
}

int struct_NetS_NationLogList_send( char **pptr, int *psize, SLK_NetS_NationLogList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_NationLog_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_NationHero_send( char **pptr, int *psize, SLK_NetS_NationHero *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_namelen, (*psize) );
	if( pValue->m_namelen > 0 && pValue->m_namelen <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_namelen*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posy, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_buypos, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_state, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_forever, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	return 0;
}

int struct_NetS_NationHeroList_send( char **pptr, int *psize, SLK_NetS_NationHeroList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_NationHero_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_MEM_SEND( (*pptr), pValue->m_nationlevel, 3*sizeof(short), (*psize) );
	return 0;
}

int struct_NetS_CityNationHero_send( char **pptr, int *psize, SLK_NetS_CityNationHero *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_state, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_forever, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_loyal, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_runstamp, (*psize) );
	return 0;
}

int struct_NetS_CityNationHeroList_send( char **pptr, int *psize, SLK_NetS_CityNationHeroList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_CityNationHero_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_NationHeroAttr_send( char **pptr, int *psize, SLK_NetS_NationHeroAttr *pValue )
{
	int tmpi = 0;

	struct_NetS_Hero_send( pptr, psize, &pValue->m_attr );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_namelen, (*psize) );
	if( pValue->m_namelen > 0 && pValue->m_namelen <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_namelen*sizeof(char), (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_open, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_loyal, (*psize) );
	return 0;
}

int struct_NetS_WishingShopItem_send( char **pptr, int *psize, SLK_NetS_WishingShopItem *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_id, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_color, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_awardkind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_awardnum, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_costkind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_costnum, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_open, (*psize) );
	return 0;
}

int struct_NetS_WishingShop_send( char **pptr, int *psize, SLK_NetS_WishingShop *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_WishingShopItem_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_DWORD_SEND( (*pptr), &pValue->m_openstamp, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_todaybuy, (*psize) );
	return 0;
}

int struct_NetS_WishingChange_send( char **pptr, int *psize, SLK_NetS_WishingChange *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_silver, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_wood, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_silver_to_wood, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_wood_to_silver, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_silver_to_food, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_wood_to_food, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_food, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_food_to_silver, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_food_to_wood, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_cd, (*psize) );
	return 0;
}

int struct_NetS_WishingPackItem_send( char **pptr, int *psize, SLK_NetS_WishingPackItem *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_awardkind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_awardnum, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_costkind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_costnum, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_id, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_token, (*psize) );
	return 0;
}

int struct_NetS_WishingPack_send( char **pptr, int *psize, SLK_NetS_WishingPack *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_WishingPackItem_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_TianceQuest_send( char **pptr, int *psize, SLK_NetS_TianceQuest *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_tc_state, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_tc_kind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_tc_num, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_tc_tech, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_nation_tiance_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_nation_tiance_point, (*psize) );
	return 0;
}

int struct_NetS_Student_send( char **pptr, int *psize, SLK_NetS_Student *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_city_index, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_shape, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_namelen, (*psize) );
	if( pValue->m_namelen > 0 && pValue->m_namelen <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_namelen*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_place, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_battlepower, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_isteacher, (*psize) );
	return 0;
}

int struct_NetS_StudentList_send( char **pptr, int *psize, SLK_NetS_StudentList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_Student_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_MEM_SEND( (*pptr), pValue->m_te_award, 10*sizeof(short), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_te_awarded, 10*sizeof(short), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_teacheraward_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_teacheraward_count; tmpi++ )
	{
		struct_NetS_AwardInfo_send( pptr, psize, &pValue->m_teacheraward[tmpi] );
	}
	return 0;
}

int struct_NetS_TeacherShopItem_send( char **pptr, int *psize, SLK_NetS_TeacherShopItem *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_awardkind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_buylevel, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_point, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_id, (*psize) );
	return 0;
}

int struct_NetS_TeacherShopList_send( char **pptr, int *psize, SLK_NetS_TeacherShopList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_TeacherShopItem_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_DWORD_SEND( (*pptr), &pValue->m_mypoint, (*psize) );
	return 0;
}

int struct_NetS_Activity03Info_send( char **pptr, int *psize, SLK_NetS_Activity03Info *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_value, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_state, (*psize) );
	return 0;
}

int struct_NetS_Activity03List_send( char **pptr, int *psize, SLK_NetS_Activity03List *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_Activity03Info_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_Activity05Item_send( char **pptr, int *psize, SLK_NetS_Activity05Item *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_awardkind, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_awardnum, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_token, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_isbuy, (*psize) );
	return 0;
}

int struct_NetS_Activity05List_send( char **pptr, int *psize, SLK_NetS_Activity05List *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_Activity05Item_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_buynum, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_updatenum, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_updatestamp, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_myxw, (*psize) );
	return 0;
}

int struct_NetS_Activity01List_send( char **pptr, int *psize, SLK_NetS_Activity01List *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_AwardInfo_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_fristpay, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_fristpay_award, (*psize) );
	return 0;
}

int struct_NetS_Activity04Info_send( char **pptr, int *psize, SLK_NetS_Activity04Info *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_value, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_state, (*psize) );
	return 0;
}

int struct_NetS_Activity04List_send( char **pptr, int *psize, SLK_NetS_Activity04List *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_Activity04Info_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_myday, (*psize) );
	return 0;
}

int struct_NetS_Girl_send( char **pptr, int *psize, SLK_NetS_Girl *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_color, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_soul, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_love_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_love_exp, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_love_today, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_sflag, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_herokind, (*psize) );
	return 0;
}

int struct_NetS_GirlList_send( char **pptr, int *psize, SLK_NetS_GirlList *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_Girl_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_GirlGet_send( char **pptr, int *psize, SLK_NetS_GirlGet *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_soulnum, (*psize) );
	struct_NetS_Girl_send( pptr, psize, &pValue->m_girl );
	return 0;
}

int struct_NetS_GirlLove_send( char **pptr, int *psize, SLK_NetS_GirlLove *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_love_level, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_love_exp, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_love_today, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_add, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_GirlLoveResult_send( char **pptr, int *psize, SLK_NetS_GirlLoveResult *pValue )
{
	int tmpi = 0;

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_sflag, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_born, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_makelove_exp, (*psize) );
	return 0;
}

