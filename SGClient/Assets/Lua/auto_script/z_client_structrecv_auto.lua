--client_structrecv_auto.lua

function struct_NetS_Login_recv( buffer )
	local recvValue = {};
	recvValue.m_result = buffer:ReadInt();
	recvValue.m_username_length = buffer:ReadShort();
	recvValue.m_username = buffer:ReadStringWithLen( recvValue.m_username_length );
	recvValue.m_token_length = buffer:ReadShort();
	recvValue.m_token = buffer:ReadStringWithLen( recvValue.m_token_length );
	recvValue.m_usertype = buffer:ReadShort();
	return recvValue;
end

function struct_ListInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_name = buffer:ReadStringWithLen( 22 );
	recvValue.m_nation = buffer:ReadSByte();
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_offset = buffer:ReadSByte();
	recvValue.m_lockstat = buffer:ReadSByte();
	recvValue.m_delete_stoptime = buffer:ReadInt();
	recvValue.m_lock_endtime = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_List_recv( buffer )
	local recvValue = {};
	recvValue.m_actor_num = buffer:ReadSByte();
	recvValue.m_listinfo = {};
	for tmpi=1,recvValue.m_actor_num,1 do
		local tmpValue={};
		tmpValue = struct_ListInfo_recv( buffer );
		table.insert( recvValue.m_listinfo, tmpValue );
	end
	recvValue.m_nation_award = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_Create_recv( buffer )
	local recvValue = {};
	recvValue.m_result = buffer:ReadSByte();
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_createtime = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_EnterInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_data_int = buffer:ReadInt();
	recvValue.m_data_short = buffer:ReadShort();
	recvValue.m_data_char = buffer:ReadSByte();
	recvValue.m_serverid = buffer:ReadShort();
	recvValue.m_servertime = buffer:ReadInt();
	recvValue.m_createtime = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_Delete_recv( buffer )
	local recvValue = {};
	recvValue.m_result = buffer:ReadInt();
	recvValue.m_actorid = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_Heart_recv( buffer )
	local recvValue = {};
	recvValue.m_value = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_Notify_recv( buffer )
	local recvValue = {};
	recvValue.m_msgid = buffer:ReadShort();
	recvValue.m_valuenum = buffer:ReadShort();
	recvValue.m_value={};
	for tmpi=1,recvValue.m_valuenum,1 do
		recvValue.m_value[tmpi] = buffer:ReadInt();
	end
	recvValue.m_msg_length = buffer:ReadShort();
	recvValue.m_msg = buffer:ReadStringWithLen( recvValue.m_msg_length );
	return recvValue;
end

function struct_NetS_DialogUpdate_recv( buffer )
	local recvValue = {};
	recvValue.m_ui = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_OfficialHire_recv( buffer )
	local recvValue = {};
	recvValue.m_ofkind = buffer:ReadShort();
	recvValue.m_ofsec = buffer:ReadInt();
	recvValue.m_offree = buffer:ReadInt();
	recvValue.m_ofquick = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_Building_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadSByte();
	recvValue.m_offset = buffer:ReadSByte();
	recvValue.m_level = buffer:ReadSByte();
	recvValue.m_sec = buffer:ReadInt();
	recvValue.m_needsec = buffer:ReadInt();
	recvValue.m_quick = buffer:ReadSByte();
	recvValue.m_overvalue = buffer:ReadInt();
	recvValue.m_value = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_BuildingBarracks_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadSByte();
	recvValue.m_offset = buffer:ReadSByte();
	recvValue.m_level = buffer:ReadSByte();
	recvValue.m_sec = buffer:ReadInt();
	recvValue.m_needsec = buffer:ReadInt();
	recvValue.m_quick = buffer:ReadSByte();
	recvValue.m_overvalue = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_BuildingRes_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadSByte();
	recvValue.m_offset = buffer:ReadSByte();
	recvValue.m_level = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_BuildingList_recv( buffer )
	local recvValue = {};
	recvValue.m_building_count = buffer:ReadShort();
	recvValue.m_building = {};
	for tmpi=1,recvValue.m_building_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_Building_recv( buffer );
		table.insert( recvValue.m_building, tmpValue );
	end
	recvValue.m_barracks_count = buffer:ReadShort();
	recvValue.m_barracks = {};
	for tmpi=1,recvValue.m_barracks_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_BuildingBarracks_recv( buffer );
		table.insert( recvValue.m_barracks, tmpValue );
	end
	recvValue.m_res_count = buffer:ReadShort();
	recvValue.m_res = {};
	for tmpi=1,recvValue.m_res_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_BuildingRes_recv( buffer );
		table.insert( recvValue.m_res, tmpValue );
	end
	recvValue.m_levynum = buffer:ReadSByte();
	recvValue.m_worker_kind = buffer:ReadSByte();
	recvValue.m_worker_offset = buffer:ReadSByte();
	recvValue.m_worker_op = buffer:ReadSByte();
	recvValue.m_worker_sec = buffer:ReadInt();
	recvValue.m_worker_needsec = buffer:ReadInt();
	recvValue.m_worker_free = buffer:ReadSByte();
	recvValue.m_worker_kind_ex = buffer:ReadSByte();
	recvValue.m_worker_offset_ex = buffer:ReadSByte();
	recvValue.m_worker_op_ex = buffer:ReadSByte();
	recvValue.m_worker_sec_ex = buffer:ReadInt();
	recvValue.m_worker_needsec_ex = buffer:ReadInt();
	recvValue.m_worker_free_ex = buffer:ReadSByte();
	recvValue.m_worker_expire_ex = buffer:ReadInt();
	recvValue.m_function = buffer:ReadInt();
	recvValue.m_forgingkind = buffer:ReadShort();
	recvValue.m_forgingsec = buffer:ReadInt();
	recvValue.m_forgingsec_need = buffer:ReadInt();
	recvValue.m_wnquick = buffer:ReadInt();
	recvValue.m_wnquick_ex = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_ActorInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_name = buffer:ReadStringWithLen( 22 );
	recvValue.m_nation = buffer:ReadSByte();
	recvValue.m_shape = buffer:ReadSByte();
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_exp = buffer:ReadInt();
	recvValue.m_exp_max = buffer:ReadInt();
	recvValue.m_token = buffer:ReadInt();
	recvValue.m_viplevel = buffer:ReadSByte();
	recvValue.m_vipexp = buffer:ReadInt();
	recvValue.m_vipexp_max = buffer:ReadInt();
	recvValue.m_body = buffer:ReadShort();
	recvValue.m_place = buffer:ReadSByte();
	recvValue.m_official = buffer:ReadSByte();
	recvValue.m_zone = buffer:ReadShort();
	recvValue.m_battlepower = buffer:ReadInt();
	recvValue.m_silver = buffer:ReadInt();
	recvValue.m_wood = buffer:ReadInt();
	recvValue.m_food = buffer:ReadInt();
	recvValue.m_iron = buffer:ReadInt();
	recvValue.m_infantry_num = buffer:ReadInt();
	recvValue.m_cavalry_num = buffer:ReadInt();
	recvValue.m_archer_num = buffer:ReadInt();
	recvValue.m_mokilllv = buffer:ReadShort();
	recvValue.m_sflag = buffer:ReadInt();
	recvValue.m_autobuild = buffer:ReadSByte();
	recvValue.m_autobuildopen = buffer:ReadSByte();
	recvValue.m_techlevel={};
	for tmpi=1,40,1 do
		recvValue.m_techlevel[tmpi] = buffer:ReadShort();
	end
	recvValue.m_techprogress={};
	for tmpi=1,40,1 do
		recvValue.m_techprogress[tmpi] = buffer:ReadShort();
	end
	recvValue.m_officialhire = {};
	for tmpi=1,3,1 do
		local tmpValue={};
		tmpValue = struct_NetS_OfficialHire_recv( buffer );
		table.insert( recvValue.m_officialhire, tmpValue );
	end
	recvValue.m_ptsec = buffer:ReadInt();
	recvValue.m_forgingkind = buffer:ReadShort();
	recvValue.m_forgingsec = buffer:ReadInt();
	recvValue.m_actor_sflag = buffer:ReadInt();
	recvValue.m_bodysec = buffer:ReadShort();
	recvValue.m_game_day = buffer:ReadSByte();
	recvValue.m_game_weather = buffer:ReadSByte();
	recvValue.m_state = buffer:ReadSByte();
	recvValue.m_guardnum = buffer:ReadSByte();
	recvValue.m_guardsec = buffer:ReadShort();
	recvValue.m_questid = buffer:ReadInt();
	recvValue.m_permission = buffer:ReadInt();
	recvValue.m_buff_endtime={};
	for tmpi=1,6,1 do
		recvValue.m_buff_endtime[tmpi] = buffer:ReadInt();
	end
	recvValue.m_autoguard = buffer:ReadSByte();
	recvValue.m_autoguardopen = buffer:ReadSByte();
	recvValue.m_storyid = buffer:ReadInt();
	recvValue.m_posx = buffer:ReadShort();
	recvValue.m_posy = buffer:ReadShort();
	recvValue.m_open_town6 = buffer:ReadSByte();
	recvValue.m_open_townking = buffer:ReadSByte();
	recvValue.m_game_day_loop = buffer:ReadSByte();
	recvValue.m_open_town3 = buffer:ReadSByte();
	recvValue.m_shape_bag = buffer:ReadInt();
	recvValue.m_bufftrain = buffer:ReadShort();
	return recvValue;
end

function struct_Corrdinate_recv( buffer )
	local recvValue = {};
	recvValue.m_posx = buffer:ReadShort();
	recvValue.m_posy = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_ItemUse_recv( buffer )
	local recvValue = {};
	recvValue.m_itemoffset = buffer:ReadShort();
	recvValue.m_usenum = buffer:ReadShort();
	recvValue.m_effres = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_ItemPut_recv( buffer )
	local recvValue = {};
	recvValue.m_res_offset = buffer:ReadShort();
	recvValue.m_res_num = buffer:ReadShort();
	recvValue.m_target_offset = buffer:ReadShort();
	recvValue.m_target_num = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_ItemSettle_recv( buffer )
	local recvValue = {};
	recvValue.m_itemnum = buffer:ReadShort();
	recvValue.m_itemoffset={};
	for tmpi=1,recvValue.m_itemnum,1 do
		recvValue.m_itemoffset[tmpi] = buffer:ReadShort();
	end
	return recvValue;
end

function struct_NetS_LostItem_recv( buffer )
	local recvValue = {};
	recvValue.m_itemoffset = buffer:ReadShort();
	recvValue.m_itemnum = buffer:ReadShort();
	recvValue.m_targetid = buffer:ReadInt();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_GetItem_recv( buffer )
	local recvValue = {};
	recvValue.m_itemoffset = buffer:ReadShort();
	recvValue.m_kind = buffer:ReadShort();
	recvValue.m_type = buffer:ReadShort();
	recvValue.m_color = buffer:ReadSByte();
	recvValue.m_num = buffer:ReadShort();
	recvValue.m_targetid = buffer:ReadInt();
	recvValue.m_path = buffer:ReadShort();
	recvValue.m_situation = buffer:ReadByte();
	return recvValue;
end

function struct_ItemSmpInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_offset = buffer:ReadShort();
	recvValue.m_kind = buffer:ReadShort();
	recvValue.m_num = buffer:ReadShort();
	recvValue.m_situation = buffer:ReadByte();
	recvValue.m_color_level = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_ItemList_recv( buffer )
	local recvValue = {};
	recvValue.m_itemext = buffer:ReadSByte();
	recvValue.m_kindnum = buffer:ReadShort();
	recvValue.m_item = {};
	for tmpi=1,recvValue.m_kindnum,1 do
		local tmpValue={};
		tmpValue = struct_ItemSmpInfo_recv( buffer );
		table.insert( recvValue.m_item, tmpValue );
	end
	return recvValue;
end

function struct_ItemAttr_recv( buffer )
	local recvValue = {};
	recvValue.m_type = buffer:ReadShort();
	recvValue.m_ability = buffer:ReadShort();
	recvValue.m_value = buffer:ReadInt();
	recvValue.m_addvalue = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_ItemInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_itemoffset = buffer:ReadShort();
	recvValue.m_itemkind = buffer:ReadShort();
	recvValue.m_type = buffer:ReadShort();
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_color_level = buffer:ReadSByte();
	recvValue.m_price = buffer:ReadInt();
	recvValue.m_attr_num = buffer:ReadShort();
	recvValue.m_attr = {};
	for tmpi=1,recvValue.m_attr_num,1 do
		local tmpValue={};
		tmpValue = struct_ItemAttr_recv( buffer );
		table.insert( recvValue.m_attr, tmpValue );
	end
	return recvValue;
end

function struct_WalkPath_recv( buffer )
	local recvValue = {};
	recvValue.m_pathx={};
	for tmpi=1,100,1 do
		recvValue.m_pathx[tmpi] = buffer:ReadShort();
	end
	recvValue.m_pathy={};
	for tmpi=1,100,1 do
		recvValue.m_pathy[tmpi] = buffer:ReadShort();
	end
	recvValue.m_len = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_ZoneUnit_recv( buffer )
	local recvValue = {};
	recvValue.m_posx = buffer:ReadShort();
	recvValue.m_posy = buffer:ReadShort();
	recvValue.m_nation = buffer:ReadSByte();
	recvValue.m_level = buffer:ReadSByte();
	recvValue.m_zoneunit_index = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_AddMapUnit_recv( buffer )
	local recvValue = {};
	recvValue.m_type = buffer:ReadSByte();
	recvValue.m_state = buffer:ReadSByte();
	recvValue.m_posx = buffer:ReadShort();
	recvValue.m_unit_index = buffer:ReadInt();
	recvValue.m_posy = buffer:ReadShort();
	recvValue.m_namelen = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_namelen );
	recvValue.m_char_value_count = buffer:ReadSByte();
	recvValue.m_char_value={};
	for tmpi=1,recvValue.m_char_value_count,1 do
		recvValue.m_char_value[tmpi] = buffer:ReadSByte();
	end
	recvValue.m_short_value_count = buffer:ReadSByte();
	recvValue.m_short_value={};
	for tmpi=1,recvValue.m_short_value_count,1 do
		recvValue.m_short_value[tmpi] = buffer:ReadShort();
	end
	recvValue.m_int_value_count = buffer:ReadSByte();
	recvValue.m_int_value={};
	for tmpi=1,recvValue.m_int_value_count,1 do
		recvValue.m_int_value[tmpi] = buffer:ReadInt();
	end
	recvValue.m_prefixlen = buffer:ReadSByte();
	recvValue.m_prefix = buffer:ReadStringWithLen( recvValue.m_prefixlen );
	return recvValue;
end

function struct_NetS_DelMapUnit_recv( buffer )
	local recvValue = {};
	recvValue.m_unit_index = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_WorldMapInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_area_width = buffer:ReadSByte();
	recvValue.m_area_height = buffer:ReadSByte();
	recvValue.m_map_area_xnum = buffer:ReadShort();
	recvValue.m_map_area_ynum = buffer:ReadShort();
	recvValue.m_my_city_posx = buffer:ReadShort();
	recvValue.m_my_city_posy = buffer:ReadShort();
	recvValue.m_my_city_unit_index = buffer:ReadInt();
	recvValue.m_citystate = buffer:ReadSByte();
	recvValue.m_target_posx = buffer:ReadShort();
	recvValue.m_target_posy = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_MapUnitCorrdinate_recv( buffer )
	local recvValue = {};
	recvValue.m_unit_index = buffer:ReadInt();
	recvValue.m_posx = buffer:ReadShort();
	recvValue.m_posy = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_AddMarchRoute_recv( buffer )
	local recvValue = {};
	recvValue.m_from_type = buffer:ReadSByte();
	recvValue.m_from_posx = buffer:ReadShort();
	recvValue.m_from_posy = buffer:ReadShort();
	recvValue.m_to_type = buffer:ReadSByte();
	recvValue.m_to_posx = buffer:ReadShort();
	recvValue.m_to_posy = buffer:ReadShort();
	recvValue.m_state = buffer:ReadSByte();
	recvValue.m_from_actorid = buffer:ReadInt();
	recvValue.m_from_nation = buffer:ReadSByte();
	recvValue.m_to_actorid = buffer:ReadInt();
	recvValue.m_to_nation = buffer:ReadSByte();
	recvValue.m_army_index = buffer:ReadInt();
	recvValue.m_action = buffer:ReadSByte();
	recvValue.m_from_grid = buffer:ReadSByte();
	recvValue.m_to_grid = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_DelMarchRoute_recv( buffer )
	local recvValue = {};
	recvValue.m_army_index = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_UpdateMapUnit_recv( buffer )
	local recvValue = {};
	recvValue.m_info = struct_NetS_AddMapUnit_recv( buffer );
	return recvValue;
end

function struct_NetS_ArmySpeedUpdate_recv( buffer )
	local recvValue = {};
	recvValue.m_unit_index = buffer:ReadInt();
	recvValue.m_state = buffer:ReadSByte();
	recvValue.m_statetime = buffer:ReadInt();
	recvValue.m_stateduration = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_Equip_recv( buffer )
	local recvValue = {};
	recvValue.m_offset = buffer:ReadInt();
	recvValue.m_kind = buffer:ReadShort();
	recvValue.m_washid={};
	for tmpi=1,4,1 do
		recvValue.m_washid[tmpi] = buffer:ReadShort();
	end
	return recvValue;
end

function struct_NetS_EquipList_recv( buffer )
	local recvValue = {};
	recvValue.m_equipext = buffer:ReadShort();
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_Equip_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_EquipGet_recv( buffer )
	local recvValue = {};
	recvValue.m_offset = buffer:ReadInt();
	recvValue.m_kind = buffer:ReadShort();
	recvValue.m_path = buffer:ReadShort();
	recvValue.m_washid={};
	for tmpi=1,4,1 do
		recvValue.m_washid[tmpi] = buffer:ReadShort();
	end
	return recvValue;
end

function struct_NetS_EquipLost_recv( buffer )
	local recvValue = {};
	recvValue.m_offset = buffer:ReadInt();
	recvValue.m_kind = buffer:ReadShort();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_HeroEquip_recv( buffer )
	local recvValue = {};
	recvValue.m_herokind = buffer:ReadShort();
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_Equip_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_Hero_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadShort();
	recvValue.m_color = buffer:ReadSByte();
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_corps = buffer:ReadSByte();
	recvValue.m_exp = buffer:ReadInt();
	recvValue.m_exp_max = buffer:ReadInt();
	recvValue.m_soldiers = buffer:ReadInt();
	recvValue.m_state = buffer:ReadSByte();
	recvValue.m_attack_base = buffer:ReadShort();
	recvValue.m_attack_wash = buffer:ReadShort();
	recvValue.m_defense_base = buffer:ReadShort();
	recvValue.m_defense_wash = buffer:ReadShort();
	recvValue.m_troops_base = buffer:ReadShort();
	recvValue.m_troops_wash = buffer:ReadShort();
	recvValue.m_attack = buffer:ReadInt();
	recvValue.m_defense = buffer:ReadInt();
	recvValue.m_troops = buffer:ReadInt();
	recvValue.m_attack_increase = buffer:ReadInt();
	recvValue.m_defense_increase = buffer:ReadInt();
	recvValue.m_offset = buffer:ReadShort();
	recvValue.m_god = buffer:ReadSByte();
	recvValue.m_girlkind = buffer:ReadSByte();
	recvValue.m_sonnum = buffer:ReadSByte();
	recvValue.m_sontime = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_HeroList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_Hero_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_type = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_HeroExp_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadShort();
	recvValue.m_exp = buffer:ReadInt();
	recvValue.m_exp_max = buffer:ReadInt();
	recvValue.m_add = buffer:ReadShort();
	recvValue.m_isup = buffer:ReadSByte();
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_HeroSoldiers_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadShort();
	recvValue.m_add = buffer:ReadInt();
	recvValue.m_soldiers = buffer:ReadInt();
	recvValue.m_soldiers_max = buffer:ReadInt();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_HeroState_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadShort();
	recvValue.m_state = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_HeroReplace_recv( buffer )
	local recvValue = {};
	recvValue.m_up_kind = buffer:ReadShort();
	recvValue.m_down_kind = buffer:ReadShort();
	recvValue.m_up_offset = buffer:ReadShort();
	recvValue.m_down_offset = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_HeroGet_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadShort();
	recvValue.m_path = buffer:ReadShort();
	recvValue.m_hero = struct_NetS_Hero_recv( buffer );
	recvValue.m_itemnum = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_AwardInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadInt();
	recvValue.m_num = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_AwardInfoList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_AwardInfo_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_callback_code = buffer:ReadShort();
	recvValue.m_value = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_Experience_recv( buffer )
	local recvValue = {};
	recvValue.m_addexp = buffer:ReadInt();
	recvValue.m_curexp = buffer:ReadInt();
	recvValue.m_isup = buffer:ReadSByte();
	recvValue.m_path = buffer:ReadShort();
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_expmax = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_Body_recv( buffer )
	local recvValue = {};
	recvValue.m_total = buffer:ReadShort();
	recvValue.m_add = buffer:ReadShort();
	recvValue.m_path = buffer:ReadShort();
	recvValue.m_bodysec = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_Levy_recv( buffer )
	local recvValue = {};
	recvValue.m_total = buffer:ReadShort();
	recvValue.m_add = buffer:ReadShort();
	recvValue.m_max = buffer:ReadShort();
	recvValue.m_sec = buffer:ReadInt();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_Silver_recv( buffer )
	local recvValue = {};
	recvValue.m_total = buffer:ReadInt();
	recvValue.m_add = buffer:ReadInt();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_Wood_recv( buffer )
	local recvValue = {};
	recvValue.m_total = buffer:ReadInt();
	recvValue.m_add = buffer:ReadInt();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_Food_recv( buffer )
	local recvValue = {};
	recvValue.m_total = buffer:ReadInt();
	recvValue.m_add = buffer:ReadInt();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_Iron_recv( buffer )
	local recvValue = {};
	recvValue.m_total = buffer:ReadInt();
	recvValue.m_add = buffer:ReadInt();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_People_recv( buffer )
	local recvValue = {};
	recvValue.m_total = buffer:ReadInt();
	recvValue.m_add = buffer:ReadInt();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_Prestige_recv( buffer )
	local recvValue = {};
	recvValue.m_total = buffer:ReadInt();
	recvValue.m_add = buffer:ReadInt();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_Friendship_recv( buffer )
	local recvValue = {};
	recvValue.m_total = buffer:ReadInt();
	recvValue.m_add = buffer:ReadInt();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_Vip_recv( buffer )
	local recvValue = {};
	recvValue.m_addexp = buffer:ReadInt();
	recvValue.m_curexp = buffer:ReadInt();
	recvValue.m_expmax = buffer:ReadInt();
	recvValue.m_isup = buffer:ReadSByte();
	recvValue.m_level = buffer:ReadSByte();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_Token_recv( buffer )
	local recvValue = {};
	recvValue.m_total = buffer:ReadInt();
	recvValue.m_add = buffer:ReadInt();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_BuildingUpgradeInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_citylevel = buffer:ReadShort();
	recvValue.m_actorlevel = buffer:ReadShort();
	recvValue.m_silver = buffer:ReadInt();
	recvValue.m_wood = buffer:ReadInt();
	recvValue.m_food = buffer:ReadInt();
	recvValue.m_iron = buffer:ReadInt();
	recvValue.m_sec = buffer:ReadInt();
	recvValue.m_maxlevel = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_Worker_recv( buffer )
	local recvValue = {};
	recvValue.m_worker_kind = buffer:ReadSByte();
	recvValue.m_worker_offset = buffer:ReadSByte();
	recvValue.m_worker_op = buffer:ReadSByte();
	recvValue.m_worker_sec = buffer:ReadInt();
	recvValue.m_worker_needsec = buffer:ReadInt();
	recvValue.m_worker_free = buffer:ReadSByte();
	recvValue.m_worker_kind_ex = buffer:ReadSByte();
	recvValue.m_worker_offset_ex = buffer:ReadSByte();
	recvValue.m_worker_op_ex = buffer:ReadSByte();
	recvValue.m_worker_sec_ex = buffer:ReadInt();
	recvValue.m_worker_needsec_ex = buffer:ReadInt();
	recvValue.m_worker_free_ex = buffer:ReadSByte();
	recvValue.m_worker_expire_ex = buffer:ReadInt();
	recvValue.m_wnquick = buffer:ReadInt();
	recvValue.m_wnquick_ex = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_BuildingGet_recv( buffer )
	local recvValue = {};
	recvValue.m_path = buffer:ReadShort();
	recvValue.m_building = struct_NetS_Building_recv( buffer );
	return recvValue;
end

function struct_NetS_BuildingBarracksGet_recv( buffer )
	local recvValue = {};
	recvValue.m_path = buffer:ReadShort();
	recvValue.m_barracks = struct_NetS_BuildingBarracks_recv( buffer );
	return recvValue;
end

function struct_NetS_BuildingResGet_recv( buffer )
	local recvValue = {};
	recvValue.m_path = buffer:ReadShort();
	recvValue.m_res = struct_NetS_BuildingRes_recv( buffer );
	return recvValue;
end

function struct_NetS_Soldiers_recv( buffer )
	local recvValue = {};
	recvValue.m_corps = buffer:ReadSByte();
	recvValue.m_soldiers = buffer:ReadInt();
	recvValue.m_add = buffer:ReadInt();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_TrainInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_soldiers = buffer:ReadInt();
	recvValue.m_soldiers_max = buffer:ReadInt();
	recvValue.m_trainnum = buffer:ReadInt();
	recvValue.m_trainsec = buffer:ReadInt();
	recvValue.m_trainsec_need = buffer:ReadInt();
	recvValue.m_queuenum={};
	for tmpi=1,16,1 do
		recvValue.m_queuenum[tmpi] = buffer:ReadInt();
	end
	recvValue.m_queue = buffer:ReadSByte();
	recvValue.m_trainlong = buffer:ReadSByte();
	recvValue.m_train_confnum = buffer:ReadInt();
	recvValue.m_train_confsec = buffer:ReadInt();
	recvValue.m_train_conffood = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_Quest_recv( buffer )
	local recvValue = {};
	recvValue.m_questid = buffer:ReadShort();
	recvValue.m_flag = buffer:ReadSByte();
	recvValue.m_datatype = buffer:ReadSByte();
	recvValue.m_datakind = buffer:ReadShort();
	recvValue.m_dataoffset = buffer:ReadShort();
	recvValue.m_value = buffer:ReadInt();
	recvValue.m_needvalue = buffer:ReadInt();
	recvValue.m_awardkind={};
	for tmpi=1,5,1 do
		recvValue.m_awardkind[tmpi] = buffer:ReadInt();
	end
	recvValue.m_awardnum={};
	for tmpi=1,5,1 do
		recvValue.m_awardnum[tmpi] = buffer:ReadInt();
	end
	recvValue.m_nameid = buffer:ReadInt();
	recvValue.m_descid = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_QuestList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_Quest_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_QuestAward_recv( buffer )
	local recvValue = {};
	recvValue.m_questid = buffer:ReadShort();
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_AwardInfo_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_datatype = buffer:ReadSByte();
	recvValue.m_datakind = buffer:ReadShort();
	recvValue.m_dataoffset = buffer:ReadSByte();
	recvValue.m_value = buffer:ReadInt();
	recvValue.m_needvalue = buffer:ReadInt();
	recvValue.m_nameid = buffer:ReadInt();
	recvValue.m_type = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_Function_recv( buffer )
	local recvValue = {};
	recvValue.m_function = buffer:ReadInt();
	recvValue.m_openoffset = buffer:ReadSByte();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_CityGuard_recv( buffer )
	local recvValue = {};
	recvValue.m_corps = buffer:ReadSByte();
	recvValue.m_color = buffer:ReadSByte();
	recvValue.m_shape = buffer:ReadSByte();
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_soldiers = buffer:ReadInt();
	recvValue.m_troops = buffer:ReadInt();
	recvValue.m_offset = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_CityGuardList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_CityGuard_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_guardsec = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_CityGuardSec_recv( buffer )
	local recvValue = {};
	recvValue.m_guardsec = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_BuildingSmithy_recv( buffer )
	local recvValue = {};
	recvValue.m_forgingkind = buffer:ReadShort();
	recvValue.m_forgingsec = buffer:ReadInt();
	recvValue.m_forgingsec_need = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_ChangeName_recv( buffer )
	local recvValue = {};
	recvValue.m_name_length = buffer:ReadShort();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_name_length );
	return recvValue;
end

function struct_NetS_BuildingAction_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadShort();
	recvValue.m_offset = buffer:ReadShort();
	recvValue.m_action = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_LevyInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_base={};
	for tmpi=1,4,1 do
		recvValue.m_base[tmpi] = buffer:ReadInt();
	end
	recvValue.m_tech={};
	for tmpi=1,4,1 do
		recvValue.m_tech[tmpi] = buffer:ReadInt();
	end
	recvValue.m_weather={};
	for tmpi=1,4,1 do
		recvValue.m_weather[tmpi] = buffer:ReadInt();
	end
	recvValue.m_activity={};
	for tmpi=1,4,1 do
		recvValue.m_activity[tmpi] = buffer:ReadInt();
	end
	recvValue.m_offical={};
	for tmpi=1,4,1 do
		recvValue.m_offical[tmpi] = buffer:ReadInt();
	end
	recvValue.m_sec = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_Chat_recv( buffer )
	local recvValue = {};
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_shape = buffer:ReadSByte();
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_namelen = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_namelen );
	recvValue.m_frame = buffer:ReadSByte();
	recvValue.m_zone = buffer:ReadSByte();
	recvValue.m_place = buffer:ReadSByte();
	recvValue.m_msglen = buffer:ReadShort();
	recvValue.m_msg = buffer:ReadStringWithLen( recvValue.m_msglen );
	recvValue.m_optime = buffer:ReadInt();
	recvValue.m_channel = buffer:ReadSByte();
	recvValue.m_nation = buffer:ReadSByte();
	recvValue.m_msgtype = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_ChatList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_Chat_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_channel = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_SystalkidValue_recv( buffer )
	local recvValue = {};
	recvValue.m_vlen = buffer:ReadSByte();
	recvValue.m_v = buffer:ReadStringWithLen( recvValue.m_vlen );
	return recvValue;
end

function struct_NetS_Systalkid_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_msglist = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_SystalkidValue_recv( buffer );
		table.insert( recvValue.m_msglist, tmpValue );
	end
	recvValue.m_textid = buffer:ReadInt();
	recvValue.m_optime = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_Systalk_recv( buffer )
	local recvValue = {};
	recvValue.m_msglen = buffer:ReadShort();
	recvValue.m_msg = buffer:ReadStringWithLen( recvValue.m_msglen );
	recvValue.m_optime = buffer:ReadInt();
	recvValue.m_roll = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_Battlepower_recv( buffer )
	local recvValue = {};
	recvValue.m_total = buffer:ReadInt();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_TechChange_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadSByte();
	recvValue.m_level = buffer:ReadSByte();
	recvValue.m_progress = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_CityEvent_recv( buffer )
	local recvValue = {};
	recvValue.m_type = buffer:ReadSByte();
	recvValue.m_kind = buffer:ReadShort();
	recvValue.m_value = buffer:ReadInt();
	recvValue.m_optime = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_BattleEvent_recv( buffer )
	local recvValue = {};
	recvValue.m_type = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( 22 );
	recvValue.m_value = buffer:ReadSByte();
	recvValue.m_optime = buffer:ReadInt();
	recvValue.m_mailid = buffer:ReadLong();
	return recvValue;
end

function struct_NetS_EventList_recv( buffer )
	local recvValue = {};
	recvValue.m_cevent_count = buffer:ReadShort();
	recvValue.m_cevent_list = {};
	for tmpi=1,recvValue.m_cevent_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_CityEvent_recv( buffer );
		table.insert( recvValue.m_cevent_list, tmpValue );
	end
	recvValue.m_bevent_count = buffer:ReadShort();
	recvValue.m_bevent_list = {};
	for tmpi=1,recvValue.m_bevent_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_BattleEvent_recv( buffer );
		table.insert( recvValue.m_bevent_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_OfficialHireChange_recv( buffer )
	local recvValue = {};
	recvValue.m_type = buffer:ReadSByte();
	recvValue.m_info = struct_NetS_OfficialHire_recv( buffer );
	return recvValue;
end

function struct_NetS_CityProtect_recv( buffer )
	local recvValue = {};
	recvValue.m_total = buffer:ReadInt();
	recvValue.m_add = buffer:ReadInt();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_HeroWash_recv( buffer )
	local recvValue = {};
	recvValue.m_hero_washnum = buffer:ReadSByte();
	recvValue.m_hero_washsec = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_HeroColorup_recv( buffer )
	local recvValue = {};
	recvValue.m_value = buffer:ReadInt();
	recvValue.m_isup = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_CityAttr_recv( buffer )
	local recvValue = {};
	recvValue.m_protectres_per = buffer:ReadShort();
	recvValue.m_buildingsec_per = buffer:ReadShort();
	recvValue.m_materialsec_per={};
	for tmpi=1,2,1 do
		recvValue.m_materialsec_per[tmpi] = buffer:ReadShort();
	end
	recvValue.m_movespeed_per={};
	for tmpi=1,3,1 do
		recvValue.m_movespeed_per[tmpi] = buffer:ReadShort();
	end
	recvValue.m_gather_per={};
	for tmpi=1,2,1 do
		recvValue.m_gather_per[tmpi] = buffer:ReadShort();
	end
	recvValue.m_hero_up_num = buffer:ReadSByte();
	recvValue.m_hero_row_num = buffer:ReadSByte();
	recvValue.m_everyday_body_buymax = buffer:ReadSByte();
	recvValue.m_everyday_autobuild_buynum = buffer:ReadSByte();
	recvValue.m_everyday_army_recall = buffer:ReadSByte();
	recvValue.m_ability_open_201 = buffer:ReadSByte();
	recvValue.m_ability_open_203 = buffer:ReadSByte();
	recvValue.m_ability_open_204 = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_EquipWash_recv( buffer )
	local recvValue = {};
	recvValue.m_equip_washnum = buffer:ReadSByte();
	recvValue.m_equip_washsec = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_MaterialInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_matkind = buffer:ReadSByte();
	recvValue.m_matnum = buffer:ReadSByte();
	recvValue.m_matsec = buffer:ReadInt();
	recvValue.m_matsec_need = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_MaterialList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_MaterialInfo_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_matquenum = buffer:ReadSByte();
	recvValue.m_nation_people = buffer:ReadInt();
	recvValue.m_city_people = buffer:ReadInt();
	recvValue.m_change_nation_people = buffer:ReadInt();
	recvValue.m_change_city_people = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_MaterialWillInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_matkind = buffer:ReadSByte();
	recvValue.m_matsec = buffer:ReadInt();
	recvValue.m_matkind_will = buffer:ReadSByte();
	recvValue.m_matnum_will = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_MaterialWillList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_MaterialWillInfo_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_StoryList_recv( buffer )
	local recvValue = {};
	recvValue.m_story_star={};
	for tmpi=1,128,1 do
		recvValue.m_story_star[tmpi] = buffer:ReadShort();
	end
	recvValue.m_story_hero={};
	for tmpi=1,32,1 do
		recvValue.m_story_hero[tmpi] = buffer:ReadShort();
	end
	recvValue.m_story_restime={};
	for tmpi=1,32,1 do
		recvValue.m_story_restime[tmpi] = buffer:ReadInt();
	end
	recvValue.m_story_resnum={};
	for tmpi=1,32,1 do
		recvValue.m_story_resnum[tmpi] = buffer:ReadShort();
	end
	recvValue.m_story_resreset={};
	for tmpi=1,32,1 do
		recvValue.m_story_resreset[tmpi] = buffer:ReadShort();
	end
	recvValue.m_story_itemnum={};
	for tmpi=1,64,1 do
		recvValue.m_story_itemnum[tmpi] = buffer:ReadShort();
	end
	recvValue.m_story_drawing={};
	for tmpi=1,16,1 do
		recvValue.m_story_drawing[tmpi] = buffer:ReadShort();
	end
	recvValue.m_sweep_herokind={};
	for tmpi=1,4,1 do
		recvValue.m_sweep_herokind[tmpi] = buffer:ReadShort();
	end
	recvValue.m_storyid = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_MonsterInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_monsterid = buffer:ReadInt();
	recvValue.m_shape = buffer:ReadShort();
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_color = buffer:ReadSByte();
	recvValue.m_corps = buffer:ReadSByte();
	recvValue.m_hp = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_StoryRank_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_MonsterInfo_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_exp = buffer:ReadInt();
	recvValue.m_body = buffer:ReadSByte();
	recvValue.m_type = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_StoryState_recv( buffer )
	local recvValue = {};
	recvValue.m_storyid = buffer:ReadInt();
	recvValue.m_state = buffer:ReadInt();
	recvValue.m_saveoffset = buffer:ReadShort();
	recvValue.m_savetype = buffer:ReadShort();
	recvValue.m_actor_storyid = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_StoryRanknum_recv( buffer )
	local recvValue = {};
	recvValue.m_storyid = buffer:ReadInt();
	recvValue.m_num = buffer:ReadShort();
	recvValue.m_saveoffset = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_StoryRanktime_recv( buffer )
	local recvValue = {};
	recvValue.m_storyid = buffer:ReadInt();
	recvValue.m_time = buffer:ReadInt();
	recvValue.m_saveoffset = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_MapZoneChange_recv( buffer )
	local recvValue = {};
	recvValue.m_zoneid = buffer:ReadSByte();
	recvValue.m_open = buffer:ReadSByte();
	recvValue.m_nation = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_MapZoneUnitList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_ZoneUnit_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_zoneid = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_BattleInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_army_index = buffer:ReadInt();
	recvValue.m_unit_index = buffer:ReadInt();
	recvValue.m_state = buffer:ReadSByte();
	recvValue.m_statetime = buffer:ReadInt();
	recvValue.m_stateduration = buffer:ReadInt();
	recvValue.m_action = buffer:ReadSByte();
	recvValue.m_to_posx = buffer:ReadShort();
	recvValue.m_to_posy = buffer:ReadShort();
	recvValue.m_herokind={};
	for tmpi=1,4,1 do
		recvValue.m_herokind[tmpi] = buffer:ReadShort();
	end
	recvValue.m_to_type = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_BattleList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_BattleInfo_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_unit_index = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_MapResInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadShort();
	recvValue.m_totalnum = buffer:ReadInt();
	recvValue.m_totalsec = buffer:ReadInt();
	recvValue.m_spacenum = buffer:ReadInt();
	recvValue.m_herokind = buffer:ReadShort();
	recvValue.m_herolevel = buffer:ReadShort();
	recvValue.m_herohp = buffer:ReadInt();
	recvValue.m_herocolor = buffer:ReadSByte();
	recvValue.m_actorlevel = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_WeatherChange_recv( buffer )
	local recvValue = {};
	recvValue.m_game_day = buffer:ReadSByte();
	recvValue.m_game_weather = buffer:ReadSByte();
	recvValue.m_game_day_loop = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_Mail_recv( buffer )
	local recvValue = {};
	recvValue.m_mailid = buffer:ReadLong();
	recvValue.m_type = buffer:ReadSByte();
	recvValue.m_title_len = buffer:ReadShort();
	recvValue.m_title = buffer:ReadStringWithLen( recvValue.m_title_len );
	recvValue.m_content_len = buffer:ReadShort();
	recvValue.m_content = buffer:ReadStringWithLen( recvValue.m_content_len );
	recvValue.m_attach_len = buffer:ReadShort();
	recvValue.m_attach = buffer:ReadStringWithLen( recvValue.m_attach_len );
	recvValue.m_attachget = buffer:ReadSByte();
	recvValue.m_read = buffer:ReadSByte();
	recvValue.m_recvtime = buffer:ReadInt();
	recvValue.m_fightid = buffer:ReadLong();
	recvValue.m_lock = buffer:ReadSByte();
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_viewpath = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_MailOpResult_recv( buffer )
	local recvValue = {};
	recvValue.m_op = buffer:ReadSByte();
	recvValue.m_mailid = buffer:ReadLong();
	return recvValue;
end

function struct_NetS_MailFight_recv( buffer )
	local recvValue = {};
	recvValue.m_flag = buffer:ReadSByte();
	recvValue.m_content_length = buffer:ReadShort();
	recvValue.m_content = buffer:ReadStringWithLen( recvValue.m_content_length );
	recvValue.m_mailid = buffer:ReadLong();
	return recvValue;
end

function struct_NetS_MailView_recv( buffer )
	local recvValue = {};
	recvValue.m_mail = struct_NetS_Mail_recv( buffer );
	return recvValue;
end

function struct_NetS_CItyHelp_recv( buffer )
	local recvValue = {};
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_actorname_length = buffer:ReadSByte();
	recvValue.m_actorname = buffer:ReadStringWithLen( recvValue.m_actorname_length );
	recvValue.m_herokind = buffer:ReadShort();
	recvValue.m_soldiers = buffer:ReadInt();
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_army_index = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_CItyHelpList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_CItyHelp_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_walllevel = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_MapCItyHelpList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_CItyHelp_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_walllevel = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_CityState_recv( buffer )
	local recvValue = {};
	recvValue.m_state = buffer:ReadSByte();
	recvValue.m_change = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_CityArmyGroup_recv( buffer )
	local recvValue = {};
	recvValue.m_group_index = buffer:ReadInt();
	recvValue.m_group_id = buffer:ReadInt();
	recvValue.m_attack = buffer:ReadSByte();
	recvValue.m_statetime = buffer:ReadInt();
	recvValue.m_stateduration = buffer:ReadInt();
	recvValue.m_nation = buffer:ReadSByte();
	recvValue.m_t_nation = buffer:ReadSByte();
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_t_level = buffer:ReadShort();
	recvValue.m_name_length = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_name_length );
	recvValue.m_t_name_length = buffer:ReadSByte();
	recvValue.m_t_name = buffer:ReadStringWithLen( recvValue.m_t_name_length );
	recvValue.m_posx = buffer:ReadShort();
	recvValue.m_posy = buffer:ReadShort();
	recvValue.m_t_posx = buffer:ReadShort();
	recvValue.m_t_posy = buffer:ReadShort();
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_t_actorid = buffer:ReadInt();
	recvValue.m_total = buffer:ReadInt();
	recvValue.m_t_total = buffer:ReadInt();
	recvValue.m_type = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_CityArmyGroupList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_CityArmyGroup_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_nation = buffer:ReadSByte();
	recvValue.m_unit_index = buffer:ReadInt();
	recvValue.m_totalcount = buffer:ReadShort();
	recvValue.m_flag = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_MapTownInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_protect_sec = buffer:ReadInt();
	recvValue.m_produce_sec = buffer:ReadInt();
	recvValue.m_own_actorid = buffer:ReadInt();
	recvValue.m_own_namelen = buffer:ReadSByte();
	recvValue.m_own_name = buffer:ReadStringWithLen( recvValue.m_own_namelen );
	recvValue.m_own_sec = buffer:ReadInt();
	recvValue.m_hp = buffer:ReadInt();
	recvValue.m_maxhp = buffer:ReadInt();
	recvValue.m_myask = buffer:ReadSByte();
	recvValue.m_produce_num = buffer:ReadShort();
	recvValue.m_townid = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_TownArmyGroup_recv( buffer )
	local recvValue = {};
	recvValue.m_group_index = buffer:ReadInt();
	recvValue.m_group_id = buffer:ReadInt();
	recvValue.m_attack = buffer:ReadSByte();
	recvValue.m_statetime = buffer:ReadInt();
	recvValue.m_stateduration = buffer:ReadInt();
	recvValue.m_nation = buffer:ReadSByte();
	recvValue.m_t_nation = buffer:ReadSByte();
	recvValue.m_total = buffer:ReadInt();
	recvValue.m_t_total = buffer:ReadInt();
	recvValue.m_type = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_TownArmyGroupList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_TownArmyGroup_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_flag = buffer:ReadSByte();
	recvValue.m_nation = buffer:ReadSByte();
	recvValue.m_unit_index = buffer:ReadInt();
	recvValue.m_totalcount = buffer:ReadShort();
	recvValue.m_townid = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_SystalkJson_recv( buffer )
	local recvValue = {};
	recvValue.m_msglen = buffer:ReadShort();
	recvValue.m_msg = buffer:ReadStringWithLen( recvValue.m_msglen );
	recvValue.m_optime = buffer:ReadInt();
	recvValue.m_roll = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_RollMsgJson_recv( buffer )
	local recvValue = {};
	recvValue.m_msglen = buffer:ReadShort();
	recvValue.m_msg = buffer:ReadStringWithLen( recvValue.m_msglen );
	return recvValue;
end

function struct_NetS_RollMsg_recv( buffer )
	local recvValue = {};
	recvValue.m_msglen = buffer:ReadShort();
	recvValue.m_msg = buffer:ReadStringWithLen( recvValue.m_msglen );
	return recvValue;
end

function struct_NetS_TownOwnerAsk_recv( buffer )
	local recvValue = {};
	recvValue.m_name_len = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_name_len );
	recvValue.m_place = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_TownOwnerAskList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_TownOwnerAsk_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_sec = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_TownFight_recv( buffer )
	local recvValue = {};
	recvValue.m_townid = buffer:ReadShort();
	recvValue.m_statetime = buffer:ReadInt();
	recvValue.m_attack = buffer:ReadSByte();
	recvValue.m_nation = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_TownFightList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_TownFight_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_MapTownExInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_dev_level = buffer:ReadSByte();
	recvValue.m_dev_exp = buffer:ReadInt();
	recvValue.m_dev_expmax = buffer:ReadInt();
	recvValue.m_mytownid = buffer:ReadShort();
	recvValue.m_dev_cd = buffer:ReadInt();
	recvValue.m_attackcd = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_MapZoneTown_recv( buffer )
	local recvValue = {};
	recvValue.m_townid = buffer:ReadShort();
	recvValue.m_nation = buffer:ReadSByte();
	recvValue.m_protect_sec = buffer:ReadInt();
	recvValue.m_from_nation={};
	for tmpi=1,4,1 do
		recvValue.m_from_nation[tmpi] = buffer:ReadShort();
	end
	return recvValue;
end

function struct_NetS_MapZoneTownList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_MapZoneTown_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_zoneid = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_MapCenterTown_recv( buffer )
	local recvValue = {};
	recvValue.m_townid = buffer:ReadShort();
	recvValue.m_nation = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_MapCenterTownList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_MapCenterTown_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_CityWarInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_group_index = buffer:ReadInt();
	recvValue.m_group_id = buffer:ReadInt();
	recvValue.m_from_nation = buffer:ReadSByte();
	recvValue.m_from_posx = buffer:ReadShort();
	recvValue.m_from_posy = buffer:ReadShort();
	recvValue.m_namelen = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_namelen );
	recvValue.m_statetime = buffer:ReadInt();
	recvValue.m_stateduration = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_CityWarList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_CityWarInfo_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_CityWarDel_recv( buffer )
	local recvValue = {};
	recvValue.m_group_index = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_WorldQuest_recv( buffer )
	local recvValue = {};
	recvValue.m_questid = buffer:ReadShort();
	recvValue.m_value = buffer:ReadShort();
	recvValue.m_maxvalue = buffer:ReadShort();
	recvValue.m_complete = buffer:ReadSByte();
	recvValue.m_dd = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_WorldBoss_recv( buffer )
	local recvValue = {};
	recvValue.m_bossid = buffer:ReadSByte();
	recvValue.m_hp = buffer:ReadInt();
	recvValue.m_maxhp = buffer:ReadInt();
	recvValue.m_isfight = buffer:ReadSByte();
	recvValue.m_actorlevel = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_LostRebuild_recv( buffer )
	local recvValue = {};
	recvValue.m_rb_silver = buffer:ReadInt();
	recvValue.m_rb_wood = buffer:ReadInt();
	recvValue.m_rb_food = buffer:ReadInt();
	recvValue.m_rb_df = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_KingwarTown_recv( buffer )
	local recvValue = {};
	recvValue.m_id = buffer:ReadSByte();
	recvValue.m_attack_total = buffer:ReadInt();
	recvValue.m_defense_total = buffer:ReadInt();
	recvValue.m_nation = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_KingwarTownList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_KingwarTown_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_leftstamp = buffer:ReadInt();
	recvValue.m_losthp = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_KingWarActivity_recv( buffer )
	local recvValue = {};
	recvValue.m_state = buffer:ReadSByte();
	recvValue.m_beginstamp = buffer:ReadInt();
	recvValue.m_endstamp = buffer:ReadInt();
	recvValue.m_nation = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_KingWarNotify_recv( buffer )
	local recvValue = {};
	recvValue.m_a_heroid = buffer:ReadShort();
	recvValue.m_a_color = buffer:ReadSByte();
	recvValue.m_a_name_len = buffer:ReadSByte();
	recvValue.m_a_name = buffer:ReadStringWithLen( recvValue.m_a_name_len );
	recvValue.m_a_losthp = buffer:ReadInt();
	recvValue.m_a_nation = buffer:ReadSByte();
	recvValue.m_d_heroid = buffer:ReadShort();
	recvValue.m_d_color = buffer:ReadSByte();
	recvValue.m_d_name_len = buffer:ReadSByte();
	recvValue.m_d_name = buffer:ReadStringWithLen( recvValue.m_d_name_len );
	recvValue.m_d_losthp = buffer:ReadInt();
	recvValue.m_d_nation = buffer:ReadSByte();
	recvValue.m_result = buffer:ReadSByte();
	recvValue.m_id = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_KingWarNotifyList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_KingWarNotify_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_KingWarRank_recv( buffer )
	local recvValue = {};
	recvValue.m_rank = buffer:ReadSByte();
	recvValue.m_kill = buffer:ReadInt();
	recvValue.m_nation = buffer:ReadSByte();
	recvValue.m_name_len = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_name_len );
	recvValue.m_herokind = buffer:ReadShort();
	recvValue.m_herocolor = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_KingWarRankList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_KingWarRank_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_mypoint = buffer:ReadInt();
	recvValue.m_totalkill = buffer:ReadInt();
	recvValue.m_myrank = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_KingWarPK_recv( buffer )
	local recvValue = {};
	recvValue.m_herokind = buffer:ReadShort();
	recvValue.m_hp = buffer:ReadInt();
	recvValue.m_id = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_KingWarPoint_recv( buffer )
	local recvValue = {};
	recvValue.m_point = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_TreasureActivity_recv( buffer )
	local recvValue = {};
	recvValue.m_state = buffer:ReadSByte();
	recvValue.m_endstamp = buffer:ReadInt();
	recvValue.m_nation = buffer:ReadSByte();
	recvValue.m_treasure_num={};
	for tmpi=1,3,1 do
		recvValue.m_treasure_num[tmpi] = buffer:ReadShort();
	end
	recvValue.m_treasure_num_max = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_TreasureHas_recv( buffer )
	local recvValue = {};
	recvValue.m_has = buffer:ReadSByte();
	recvValue.m_px = buffer:ReadShort();
	recvValue.m_py = buffer:ReadShort();
	recvValue.m_tn = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_GotoAsyn_recv( buffer )
	local recvValue = {};
	recvValue.m_posx = buffer:ReadShort();
	recvValue.m_posy = buffer:ReadShort();
	recvValue.m_type = buffer:ReadSByte();
	recvValue.m_unit_index = buffer:ReadInt();
	recvValue.m_op = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_TreasureActor_recv( buffer )
	local recvValue = {};
	recvValue.m_itemkind = buffer:ReadShort();
	recvValue.m_name_len = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_name_len );
	return recvValue;
end

function struct_NetS_TreasureActorList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_TreasureActor_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_co = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_ChangeAutoBuild_recv( buffer )
	local recvValue = {};
	recvValue.m_autobuild = buffer:ReadSByte();
	recvValue.m_autobuildopen = buffer:ReadSByte();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_HeroVisitInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_hv_free_cd = buffer:ReadInt();
	recvValue.m_hv_high_sec = buffer:ReadInt();
	recvValue.m_hv_high_free = buffer:ReadSByte();
	recvValue.m_hv_low_num = buffer:ReadSByte();
	recvValue.m_hv_high_num = buffer:ReadSByte();
	recvValue.m_hv_progress = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_NationBase_recv( buffer )
	local recvValue = {};
	recvValue.m_level = buffer:ReadSByte();
	recvValue.m_exp = buffer:ReadInt();
	recvValue.m_donate_num = buffer:ReadSByte();
	recvValue.m_myrank = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_HeroVisitAward_recv( buffer )
	local recvValue = {};
	recvValue.m_id={};
	for tmpi=1,10,1 do
		recvValue.m_id[tmpi] = buffer:ReadShort();
	end
	recvValue.m_count = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_FightPlay_recv( buffer )
	local recvValue = {};
	recvValue.m_flag = buffer:ReadSByte();
	recvValue.m_content_length = buffer:ReadShort();
	recvValue.m_content = buffer:ReadStringWithLen( recvValue.m_content_length );
	return recvValue;
end

function struct_NetS_StorySweepHero_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadShort();
	recvValue.m_color = buffer:ReadSByte();
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_pre_exp = buffer:ReadInt();
	recvValue.m_exp = buffer:ReadInt();
	recvValue.m_type = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_StorySweepResult_recv( buffer )
	local recvValue = {};
	recvValue.m_exp = buffer:ReadInt();
	recvValue.m_silver = buffer:ReadInt();
	recvValue.m_awardcount = buffer:ReadSByte();
	recvValue.m_award = {};
	for tmpi=1,recvValue.m_awardcount,1 do
		local tmpValue={};
		tmpValue = struct_NetS_AwardInfo_recv( buffer );
		table.insert( recvValue.m_award, tmpValue );
	end
	recvValue.m_herocount = buffer:ReadSByte();
	recvValue.m_hero = {};
	for tmpi=1,recvValue.m_herocount,1 do
		local tmpValue={};
		tmpValue = struct_NetS_StorySweepHero_recv( buffer );
		table.insert( recvValue.m_hero, tmpValue );
	end
	return recvValue;
end

function struct_NetS_QuestTalk_recv( buffer )
	local recvValue = {};
	recvValue.m_talkid = buffer:ReadInt();
	recvValue.m_herokind = buffer:ReadShort();
	recvValue.m_talk_textid = buffer:ReadInt();
	recvValue.m_btn_textid = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_RankInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_rank = buffer:ReadInt();
	recvValue.m_namelen = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_namelen );
	recvValue.m_nation = buffer:ReadSByte();
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_place = buffer:ReadSByte();
	recvValue.m_battlepower = buffer:ReadInt();
	recvValue.m_actorid = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_RankList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_RankInfo_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_type = buffer:ReadSByte();
	recvValue.m_page = buffer:ReadInt();
	recvValue.m_myrank = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_FriendInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_city_index = buffer:ReadInt();
	recvValue.m_shape = buffer:ReadSByte();
	recvValue.m_namelen = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_namelen );
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_place = buffer:ReadSByte();
	recvValue.m_battlepower = buffer:ReadInt();
	recvValue.m_ask = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_FriendList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_FriendInfo_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_op = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_ActorSearch_recv( buffer )
	local recvValue = {};
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_city_index = buffer:ReadInt();
	recvValue.m_namelen = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_namelen );
	recvValue.m_signlen = buffer:ReadSByte();
	recvValue.m_sign = buffer:ReadStringWithLen( recvValue.m_signlen );
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_official = buffer:ReadSByte();
	recvValue.m_place = buffer:ReadSByte();
	recvValue.m_zone = buffer:ReadSByte();
	recvValue.m_nation = buffer:ReadSByte();
	recvValue.m_battlepower = buffer:ReadInt();
	recvValue.m_killcity = buffer:ReadInt();
	recvValue.m_herocount = buffer:ReadSByte();
	recvValue.m_hero = {};
	for tmpi=1,recvValue.m_herocount,1 do
		local tmpValue={};
		tmpValue = struct_NetS_StorySweepHero_recv( buffer );
		table.insert( recvValue.m_hero, tmpValue );
	end
	recvValue.m_bp_hero = buffer:ReadInt();
	recvValue.m_bp_equip = buffer:ReadInt();
	recvValue.m_bp_tech = buffer:ReadInt();
	recvValue.m_bp_nequip = buffer:ReadInt();
	recvValue.m_bp_girl = buffer:ReadInt();
	recvValue.m_bp_place = buffer:ReadInt();
	recvValue.m_my_bp_hero = buffer:ReadInt();
	recvValue.m_my_bp_equip = buffer:ReadInt();
	recvValue.m_my_bp_tech = buffer:ReadInt();
	recvValue.m_my_bp_nequip = buffer:ReadInt();
	recvValue.m_my_bp_girl = buffer:ReadInt();
	recvValue.m_my_bp_place = buffer:ReadInt();
	recvValue.m_shape = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_BlackInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_namelen = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_namelen );
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_nation = buffer:ReadSByte();
	recvValue.m_shape = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_BlackList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_BlackInfo_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_op = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_BlackListID_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_actorid={};
	for tmpi=1,recvValue.m_count,1 do
		recvValue.m_actorid[tmpi] = buffer:ReadInt();
	end
	return recvValue;
end

function struct_NetS_NationEquip_recv( buffer )
	local recvValue = {};
	recvValue.m_neq_lv = buffer:ReadShort();
	recvValue.m_neq_exp = buffer:ReadSByte();
	recvValue.m_neq_pro = buffer:ReadSByte();
	recvValue.m_neq_sec = buffer:ReadInt();
	recvValue.m_neq_crit = buffer:ReadSByte();
	recvValue.m_neq_star = buffer:ReadSByte();
	recvValue.m_kind = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_ShopItem_recv( buffer )
	local recvValue = {};
	recvValue.m_offset = buffer:ReadShort();
	recvValue.m_awardkind = buffer:ReadInt();
	recvValue.m_awardnum = buffer:ReadInt();
	recvValue.m_token = buffer:ReadShort();
	recvValue.m_sale = buffer:ReadShort();
	recvValue.m_buyuse = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_ShopList_recv( buffer )
	local recvValue = {};
	recvValue.m_type = buffer:ReadSByte();
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_ShopItem_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_PayStoreGoods_recv( buffer )
	local recvValue = {};
	recvValue.m_goodsid = buffer:ReadShort();
	recvValue.m_price = buffer:ReadInt();
	recvValue.m_token = buffer:ReadInt();
	recvValue.m_day = buffer:ReadShort();
	recvValue.m_nameid = buffer:ReadShort();
	recvValue.m_icon = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_PayStore_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_PayStoreGoods_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_GoodsInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_goodsid = buffer:ReadShort();
	recvValue.m_price = buffer:ReadInt();
	recvValue.m_nameid = buffer:ReadInt();
	recvValue.m_descid = buffer:ReadInt();
	recvValue.m_icon = buffer:ReadSByte();
	recvValue.m_sale = buffer:ReadShort();
	recvValue.m_worth = buffer:ReadInt();
	recvValue.m_bag_time = buffer:ReadInt();
	recvValue.m_bag_num = buffer:ReadSByte();
	recvValue.m_awardcount = buffer:ReadSByte();
	recvValue.m_award = {};
	for tmpi=1,recvValue.m_awardcount,1 do
		local tmpValue={};
		tmpValue = struct_NetS_AwardInfo_recv( buffer );
		table.insert( recvValue.m_award, tmpValue );
	end
	return recvValue;
end

function struct_NetS_PayStoreActivity_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_GoodsInfo_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_path = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_PayOrder_recv( buffer )
	local recvValue = {};
	recvValue.m_orderid_len = buffer:ReadSByte();
	recvValue.m_orderid = buffer:ReadStringWithLen( recvValue.m_orderid_len );
	recvValue.m_ext_len = buffer:ReadSByte();
	recvValue.m_ext = buffer:ReadStringWithLen( recvValue.m_ext_len );
	recvValue.m_goodsid = buffer:ReadShort();
	recvValue.m_productid = buffer:ReadInt();
	recvValue.m_nameid = buffer:ReadInt();
	recvValue.m_descid = buffer:ReadInt();
	recvValue.m_price = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_VipShopItem_recv( buffer )
	local recvValue = {};
	recvValue.m_awardkind = buffer:ReadInt();
	recvValue.m_awardnum = buffer:ReadInt();
	recvValue.m_token = buffer:ReadShort();
	recvValue.m_itemkind = buffer:ReadShort();
	recvValue.m_vip_token = buffer:ReadShort();
	recvValue.m_vip_buynum = buffer:ReadShort();
	recvValue.m_vip_buynum_max = buffer:ReadShort();
	recvValue.m_id = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_VipShop_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_VipShopItem_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_useitem = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_VipBag_recv( buffer )
	local recvValue = {};
	recvValue.m_vipbag = buffer:ReadInt();
	recvValue.m_vipbag_count = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_BuffChange_recv( buffer )
	local recvValue = {};
	recvValue.m_buffkind = buffer:ReadSByte();
	recvValue.m_endtime = buffer:ReadInt();
	recvValue.m_path = buffer:ReadShort();
	recvValue.m_bufftrain = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_ChangeAutoGuard_recv( buffer )
	local recvValue = {};
	recvValue.m_autoguard = buffer:ReadSByte();
	recvValue.m_autoguardopen = buffer:ReadSByte();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_Activity_recv( buffer )
	local recvValue = {};
	recvValue.m_activityid = buffer:ReadShort();
	recvValue.m_starttime = buffer:ReadInt();
	recvValue.m_endtime = buffer:ReadInt();
	recvValue.m_closetime = buffer:ReadInt();
	recvValue.m_red = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_ActivityList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_Activity_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_NationEquipList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_NationEquip_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_NationInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_level = buffer:ReadSByte();
	recvValue.m_exp = buffer:ReadInt();
	recvValue.m_donate_num = buffer:ReadSByte();
	recvValue.m_myrank = buffer:ReadInt();
	recvValue.m_notice_len = buffer:ReadShort();
	recvValue.m_notice = buffer:ReadStringWithLen( recvValue.m_notice_len );
	recvValue.m_kingname_len = buffer:ReadSByte();
	recvValue.m_kingname = buffer:ReadStringWithLen( recvValue.m_kingname_len );
	recvValue.m_questvalue={};
	for tmpi=1,3,1 do
		recvValue.m_questvalue[tmpi] = buffer:ReadShort();
	end
	recvValue.m_questvalue_max={};
	for tmpi=1,3,1 do
		recvValue.m_questvalue_max[tmpi] = buffer:ReadShort();
	end
	return recvValue;
end

function struct_NetS_NationTown_recv( buffer )
	local recvValue = {};
	recvValue.m_info = struct_NetS_MapTownInfo_recv( buffer );
	recvValue.m_path = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_NationTownList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_NationTown_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_op = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_NationWar_recv( buffer )
	local recvValue = {};
	recvValue.m_group_index = buffer:ReadInt();
	recvValue.m_group_id = buffer:ReadInt();
	recvValue.m_attack = buffer:ReadSByte();
	recvValue.m_statetime = buffer:ReadInt();
	recvValue.m_stateduration = buffer:ReadInt();
	recvValue.m_nation = buffer:ReadSByte();
	recvValue.m_t_nation = buffer:ReadSByte();
	recvValue.m_total = buffer:ReadInt();
	recvValue.m_t_total = buffer:ReadInt();
	recvValue.m_type = buffer:ReadSByte();
	recvValue.m_townid = buffer:ReadShort();
	recvValue.m_unit_index = buffer:ReadInt();
	recvValue.m_town_nation = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_NationWarList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_NationWar_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_op = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_NationCityWar_recv( buffer )
	local recvValue = {};
	recvValue.m_group_index = buffer:ReadInt();
	recvValue.m_group_id = buffer:ReadInt();
	recvValue.m_attack = buffer:ReadSByte();
	recvValue.m_statetime = buffer:ReadInt();
	recvValue.m_stateduration = buffer:ReadInt();
	recvValue.m_nation = buffer:ReadSByte();
	recvValue.m_t_nation = buffer:ReadSByte();
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_t_level = buffer:ReadShort();
	recvValue.m_name_length = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_name_length );
	recvValue.m_t_name_length = buffer:ReadSByte();
	recvValue.m_t_name = buffer:ReadStringWithLen( recvValue.m_t_name_length );
	recvValue.m_posx = buffer:ReadShort();
	recvValue.m_posy = buffer:ReadShort();
	recvValue.m_t_posx = buffer:ReadShort();
	recvValue.m_t_posy = buffer:ReadShort();
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_t_actorid = buffer:ReadInt();
	recvValue.m_total = buffer:ReadInt();
	recvValue.m_t_total = buffer:ReadInt();
	recvValue.m_type = buffer:ReadSByte();
	recvValue.m_unit_index = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_NationCityWarList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_NationCityWar_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_op = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_NationQuest_recv( buffer )
	local recvValue = {};
	recvValue.m_level = buffer:ReadSByte();
	recvValue.m_kind = buffer:ReadSByte();
	recvValue.m_value = buffer:ReadInt();
	recvValue.m_needvalue = buffer:ReadInt();
	recvValue.m_awardkind={};
	for tmpi=1,5,1 do
		recvValue.m_awardkind[tmpi] = buffer:ReadInt();
	end
	recvValue.m_awardnum={};
	for tmpi=1,5,1 do
		recvValue.m_awardnum[tmpi] = buffer:ReadInt();
	end
	recvValue.m_other_awardkind = buffer:ReadInt();
	recvValue.m_other_awardnum = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_NationQuestList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_NationQuest_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_NationMission_recv( buffer )
	local recvValue = {};
	recvValue.m_value={};
	for tmpi=1,3,1 do
		recvValue.m_value[tmpi] = buffer:ReadInt();
	end
	recvValue.m_needvalue={};
	for tmpi=1,3,1 do
		recvValue.m_needvalue[tmpi] = buffer:ReadInt();
	end
	recvValue.m_awardkind={};
	for tmpi=1,2,1 do
		recvValue.m_awardkind[tmpi] = buffer:ReadInt();
	end
	recvValue.m_awardnum={};
	for tmpi=1,2,1 do
		recvValue.m_awardnum[tmpi] = buffer:ReadInt();
	end
	recvValue.m_isget = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_NationMissionList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_NationMission_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_NationRankMember_recv( buffer )
	local recvValue = {};
	recvValue.m_name_len = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_name_len );
	recvValue.m_value = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_NationRankList_recv( buffer )
	local recvValue = {};
	recvValue.m_buildrank = {};
	for tmpi=1,5,1 do
		local tmpValue={};
		tmpValue = struct_NetS_NationRankMember_recv( buffer );
		table.insert( recvValue.m_buildrank, tmpValue );
	end
	recvValue.m_cityrank = {};
	for tmpi=1,5,1 do
		local tmpValue={};
		tmpValue = struct_NetS_NationRankMember_recv( buffer );
		table.insert( recvValue.m_cityrank, tmpValue );
	end
	recvValue.m_townrank = {};
	for tmpi=1,5,1 do
		local tmpValue={};
		tmpValue = struct_NetS_NationRankMember_recv( buffer );
		table.insert( recvValue.m_townrank, tmpValue );
	end
	recvValue.m_vote = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_NationOfficial_recv( buffer )
	local recvValue = {};
	recvValue.m_official = buffer:ReadSByte();
	recvValue.m_namelen = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_namelen );
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_battlepower = buffer:ReadInt();
	recvValue.m_zone = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_NationOfficialList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_NationOfficial_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_endtime = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_NationCandidate_recv( buffer )
	local recvValue = {};
	recvValue.m_namelen = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_namelen );
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_battlepower = buffer:ReadInt();
	recvValue.m_ballot = buffer:ReadInt();
	recvValue.m_actorid = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_NationCandidateList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_NationCandidate_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_endtime = buffer:ReadInt();
	recvValue.m_myvote = buffer:ReadShort();
	recvValue.m_isballot = buffer:ReadSByte();
	recvValue.m_tokenballot = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_WorldDataOpen_recv( buffer )
	local recvValue = {};
	recvValue.m_open_town3 = buffer:ReadSByte();
	recvValue.m_open_town6 = buffer:ReadSByte();
	recvValue.m_open_townking = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_NationReplace_recv( buffer )
	local recvValue = {};
	recvValue.m_namelen = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_namelen );
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_battlepower = buffer:ReadInt();
	recvValue.m_place = buffer:ReadSByte();
	recvValue.m_official = buffer:ReadSByte();
	recvValue.m_actorid = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_NationReplaceList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_NationReplace_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_op = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_NationLog_recv( buffer )
	local recvValue = {};
	recvValue.m_type = buffer:ReadSByte();
	recvValue.m_townid = buffer:ReadShort();
	recvValue.m_namelen = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_namelen );
	recvValue.m_target_nation = buffer:ReadSByte();
	recvValue.m_optime = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_NationLogList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_NationLog_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_NationHero_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadShort();
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_namelen = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_namelen );
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_posx = buffer:ReadShort();
	recvValue.m_posy = buffer:ReadShort();
	recvValue.m_buypos = buffer:ReadSByte();
	recvValue.m_state = buffer:ReadSByte();
	recvValue.m_forever = buffer:ReadSByte();
	recvValue.m_nation = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_NationHeroList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_NationHero_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_nationlevel={};
	for tmpi=1,3,1 do
		recvValue.m_nationlevel[tmpi] = buffer:ReadShort();
	end
	return recvValue;
end

function struct_NetS_CityNationHero_recv( buffer )
	local recvValue = {};
	recvValue.m_state = buffer:ReadSByte();
	recvValue.m_kind = buffer:ReadShort();
	recvValue.m_forever = buffer:ReadSByte();
	recvValue.m_loyal = buffer:ReadSByte();
	recvValue.m_runstamp = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_CityNationHeroList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_CityNationHero_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_NationHeroAttr_recv( buffer )
	local recvValue = {};
	recvValue.m_attr = struct_NetS_Hero_recv( buffer );
	recvValue.m_namelen = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_namelen );
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_open = buffer:ReadSByte();
	recvValue.m_nation = buffer:ReadSByte();
	recvValue.m_loyal = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_WishingShopItem_recv( buffer )
	local recvValue = {};
	recvValue.m_id = buffer:ReadShort();
	recvValue.m_color = buffer:ReadSByte();
	recvValue.m_awardkind = buffer:ReadInt();
	recvValue.m_awardnum = buffer:ReadInt();
	recvValue.m_costkind = buffer:ReadInt();
	recvValue.m_costnum = buffer:ReadInt();
	recvValue.m_open = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_WishingShop_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_WishingShopItem_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_openstamp = buffer:ReadInt();
	recvValue.m_todaybuy = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_WishingChange_recv( buffer )
	local recvValue = {};
	recvValue.m_silver = buffer:ReadInt();
	recvValue.m_wood = buffer:ReadInt();
	recvValue.m_silver_to_wood = buffer:ReadInt();
	recvValue.m_wood_to_silver = buffer:ReadInt();
	recvValue.m_silver_to_food = buffer:ReadInt();
	recvValue.m_wood_to_food = buffer:ReadInt();
	recvValue.m_food = buffer:ReadInt();
	recvValue.m_food_to_silver = buffer:ReadInt();
	recvValue.m_food_to_wood = buffer:ReadInt();
	recvValue.m_cd = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_WishingPackItem_recv( buffer )
	local recvValue = {};
	recvValue.m_awardkind = buffer:ReadInt();
	recvValue.m_awardnum = buffer:ReadInt();
	recvValue.m_costkind = buffer:ReadInt();
	recvValue.m_costnum = buffer:ReadInt();
	recvValue.m_id = buffer:ReadShort();
	recvValue.m_token = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_WishingPack_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_WishingPackItem_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_TianceQuest_recv( buffer )
	local recvValue = {};
	recvValue.m_tc_state = buffer:ReadShort();
	recvValue.m_tc_kind = buffer:ReadSByte();
	recvValue.m_tc_num = buffer:ReadSByte();
	recvValue.m_tc_tech = buffer:ReadShort();
	recvValue.m_nation_tiance_level = buffer:ReadSByte();
	recvValue.m_nation_tiance_point = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_Student_recv( buffer )
	local recvValue = {};
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_city_index = buffer:ReadInt();
	recvValue.m_shape = buffer:ReadSByte();
	recvValue.m_namelen = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_namelen );
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_place = buffer:ReadSByte();
	recvValue.m_battlepower = buffer:ReadInt();
	recvValue.m_isteacher = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_StudentList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_Student_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_te_award={};
	for tmpi=1,10,1 do
		recvValue.m_te_award[tmpi] = buffer:ReadShort();
	end
	recvValue.m_te_awarded={};
	for tmpi=1,10,1 do
		recvValue.m_te_awarded[tmpi] = buffer:ReadShort();
	end
	recvValue.m_teacheraward_count = buffer:ReadSByte();
	recvValue.m_teacheraward = {};
	for tmpi=1,recvValue.m_teacheraward_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_AwardInfo_recv( buffer );
		table.insert( recvValue.m_teacheraward, tmpValue );
	end
	return recvValue;
end

function struct_NetS_TeacherShopItem_recv( buffer )
	local recvValue = {};
	recvValue.m_awardkind = buffer:ReadInt();
	recvValue.m_buylevel = buffer:ReadShort();
	recvValue.m_point = buffer:ReadInt();
	recvValue.m_id = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_TeacherShopList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_TeacherShopItem_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_mypoint = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_Activity03Info_recv( buffer )
	local recvValue = {};
	recvValue.m_value = buffer:ReadInt();
	recvValue.m_state = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_Activity03List_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_Activity03Info_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_Activity05Item_recv( buffer )
	local recvValue = {};
	recvValue.m_awardkind = buffer:ReadInt();
	recvValue.m_awardnum = buffer:ReadInt();
	recvValue.m_token = buffer:ReadInt();
	recvValue.m_isbuy = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_Activity05List_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_Activity05Item_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_buynum = buffer:ReadSByte();
	recvValue.m_updatenum = buffer:ReadSByte();
	recvValue.m_updatestamp = buffer:ReadInt();
	recvValue.m_myxw = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_Activity01List_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_AwardInfo_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_fristpay = buffer:ReadSByte();
	recvValue.m_fristpay_award = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_Activity04Info_recv( buffer )
	local recvValue = {};
	recvValue.m_value = buffer:ReadInt();
	recvValue.m_state = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_Activity04List_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_Activity04Info_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	recvValue.m_myday = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_Girl_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadSByte();
	recvValue.m_color = buffer:ReadSByte();
	recvValue.m_soul = buffer:ReadShort();
	recvValue.m_love_level = buffer:ReadSByte();
	recvValue.m_love_exp = buffer:ReadInt();
	recvValue.m_love_today = buffer:ReadInt();
	recvValue.m_sflag = buffer:ReadSByte();
	recvValue.m_herokind = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_GirlList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadSByte();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_Girl_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_GirlGet_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadSByte();
	recvValue.m_path = buffer:ReadShort();
	recvValue.m_soulnum = buffer:ReadShort();
	recvValue.m_girl = struct_NetS_Girl_recv( buffer );
	return recvValue;
end

function struct_NetS_GirlLove_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadSByte();
	recvValue.m_love_level = buffer:ReadSByte();
	recvValue.m_love_exp = buffer:ReadShort();
	recvValue.m_love_today = buffer:ReadShort();
	recvValue.m_add = buffer:ReadShort();
	recvValue.m_path = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_GirlLoveResult_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadSByte();
	recvValue.m_sflag = buffer:ReadSByte();
	recvValue.m_born = buffer:ReadSByte();
	recvValue.m_makelove_exp = buffer:ReadShort();
	return recvValue;
end

