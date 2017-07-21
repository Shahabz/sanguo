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

	LKSET_MEM_SEND( (*pptr), pValue->m_value, 2*sizeof(int), (*psize) );
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
	if( pValue->m_msg_length > 0 && pValue->m_msg_length <= 256 )
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
	LKSET_WORD_SEND( (*pptr), &pValue->m_silver, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_wood, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_food, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_iron, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_infantry_num, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_cavalry_num, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_archer_num, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_mokilllv, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_sflag, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_autobuild, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_autobuildopen, (*psize) );
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
	if( pValue->m_short_value_count > 0 && pValue->m_short_value_count <= 8 )
		LKSET_MEM_SEND( (*pptr), pValue->m_short_value, pValue->m_short_value_count*sizeof(short), (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_int_value_count, (*psize) );
	if( pValue->m_int_value_count > 0 && pValue->m_int_value_count <= 8 )
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

	LKSET_WORD_SEND( (*pptr), &pValue->m_map_width, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_map_height, (*psize) );
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
	LKSET_DWORD_SEND( (*pptr), &pValue->m_from_clubid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_to_actorid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_to_clubid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_army_index, (*psize) );
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_action, (*psize) );
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

int struct_NetS_Equip_send( char **pptr, int *psize, SLK_NetS_Equip *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_offset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_washid, 6*sizeof(short), (*psize) );
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
	LKSET_WORD_SEND( (*pptr), &pValue->m_offset, (*psize) );
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
	return 0;
}

int struct_NetS_HeroGet_send( char **pptr, int *psize, SLK_NetS_HeroGet *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_path, (*psize) );
	struct_NetS_Hero_send( pptr, psize, &pValue->m_hero );
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
	LKSET_MEM_SEND( (*pptr), pValue->m_old_value, 8*sizeof(int), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_new_value, 8*sizeof(int), (*psize) );
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

	LKSET_SBYTE_SEND( (*pptr), &pValue->m_corpse, (*psize) );
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
	LKSET_SBYTE_SEND( (*pptr), &pValue->m_dataoffset, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_value, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_needvalue, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_awardkind, 5*sizeof(int), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_awardnum, 5*sizeof(int), (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_nameid, (*psize) );
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
	return 0;
}

