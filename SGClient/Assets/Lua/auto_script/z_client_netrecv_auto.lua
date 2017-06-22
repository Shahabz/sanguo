--client_netrecv_auto.lua

function netrecv_login_C( buffer )
	local recvValue = struct_NetS_Login_recv( buffer );
	proc_login_C( recvValue );
end

function netrecv_list_C( buffer )
	local recvValue = struct_NetS_List_recv( buffer );
	proc_list_C( recvValue );
end

function netrecv_create_C( buffer )
	local recvValue = struct_NetS_Create_recv( buffer );
	proc_create_C( recvValue );
end

function netrecv_enterinfo_C( buffer )
	local recvValue = struct_NetS_EnterInfo_recv( buffer );
	proc_enterinfo_C( recvValue );
end

function netrecv_delete_C( buffer )
	local recvValue = struct_NetS_Delete_recv( buffer );
	proc_delete_C( recvValue );
end

function netrecv_heart_C( buffer )
	local recvValue = struct_NetS_Heart_recv( buffer );
	proc_heart_C( recvValue );
end

function netrecv_notify_C( buffer )
	local recvValue = struct_NetS_Notify_recv( buffer );
	proc_notify_C( recvValue );
end

function netrecv_actorinfo_C( buffer )
	local recvValue = struct_NetS_ActorInfo_recv( buffer );
	proc_actorinfo_C( recvValue );
end

function netrecv_itemuse_C( buffer )
	local recvValue = struct_NetS_ItemUse_recv( buffer );
	proc_itemuse_C( recvValue );
end

function netrecv_itemput_C( buffer )
	local recvValue = struct_NetS_ItemPut_recv( buffer );
	proc_itemput_C( recvValue );
end

function netrecv_itemsettle_C( buffer )
	local recvValue = struct_NetS_ItemSettle_recv( buffer );
	proc_itemsettle_C( recvValue );
end

function netrecv_lostitem_C( buffer )
	local recvValue = struct_NetS_LostItem_recv( buffer );
	proc_lostitem_C( recvValue );
end

function netrecv_getitem_C( buffer )
	local recvValue = struct_NetS_GetItem_recv( buffer );
	proc_getitem_C( recvValue );
end

function netrecv_itemlist_C( buffer )
	local recvValue = struct_NetS_ItemList_recv( buffer );
	proc_itemlist_C( recvValue );
end

function netrecv_iteminfo_C( buffer )
	local recvValue = struct_NetS_ItemInfo_recv( buffer );
	proc_iteminfo_C( recvValue );
end

function netrecv_addmapunit_C( buffer )
	local recvValue = struct_NetS_AddMapUnit_recv( buffer );
	proc_addmapunit_C( recvValue );
end

function netrecv_delmapunit_C( buffer )
	local recvValue = struct_NetS_DelMapUnit_recv( buffer );
	proc_delmapunit_C( recvValue );
end

function netrecv_updatemapunit_C( buffer )
	local recvValue = struct_NetS_UpdateMapUnit_recv( buffer );
	proc_updatemapunit_C( recvValue );
end

function netrecv_mapunitsingle_C( buffer )
	local recvValue = struct_NetS_AddMapUnit_recv( buffer );
	proc_mapunitsingle_C( recvValue );
end

function netrecv_worldmapinfo_C( buffer )
	local recvValue = struct_NetS_WorldMapInfo_recv( buffer );
	proc_worldmapinfo_C( recvValue );
end

function netrecv_mapunitcorrdinate_C( buffer )
	local recvValue = struct_NetS_MapUnitCorrdinate_recv( buffer );
	proc_mapunitcorrdinate_C( recvValue );
end

function netrecv_addmarchroute_C( buffer )
	local recvValue = struct_NetS_AddMarchRoute_recv( buffer );
	proc_addmarchroute_C( recvValue );
end

function netrecv_delmarchroute_C( buffer )
	local recvValue = struct_NetS_DelMarchRoute_recv( buffer );
	proc_delmarchroute_C( recvValue );
end

function netrecv_awardinfolist_C( buffer )
	local recvValue = struct_NetS_AwardInfoList_recv( buffer );
	proc_awardinfolist_C( recvValue );
end

function netrecv_experience_C( buffer )
	local recvValue = struct_NetS_Experience_recv( buffer );
	proc_experience_C( recvValue );
end

function netrecv_body_C( buffer )
	local recvValue = struct_NetS_Body_recv( buffer );
	proc_body_C( recvValue );
end

function netrecv_buildinglist_C( buffer )
	local recvValue = struct_NetS_BuildingList_recv( buffer );
	proc_buildinglist_C( recvValue );
end

function netrecv_changelevy_C( buffer )
	local recvValue = struct_NetS_Levy_recv( buffer );
	proc_changelevy_C( recvValue );
end

function netrecv_changesilver_C( buffer )
	local recvValue = struct_NetS_Silver_recv( buffer );
	proc_changesilver_C( recvValue );
end

function netrecv_changewood_C( buffer )
	local recvValue = struct_NetS_Wood_recv( buffer );
	proc_changewood_C( recvValue );
end

function netrecv_changefood_C( buffer )
	local recvValue = struct_NetS_Food_recv( buffer );
	proc_changefood_C( recvValue );
end

function netrecv_changeiron_C( buffer )
	local recvValue = struct_NetS_Iron_recv( buffer );
	proc_changeiron_C( recvValue );
end

function netrecv_changepeople_C( buffer )
	local recvValue = struct_NetS_People_recv( buffer );
	proc_changepeople_C( recvValue );
end

function netrecv_changeprestige_C( buffer )
	local recvValue = struct_NetS_Prestige_recv( buffer );
	proc_changeprestige_C( recvValue );
end

function netrecv_changefriendship_C( buffer )
	local recvValue = struct_NetS_Friendship_recv( buffer );
	proc_changefriendship_C( recvValue );
end

function netrecv_changevip_C( buffer )
	local recvValue = struct_NetS_Vip_recv( buffer );
	proc_changevip_C( recvValue );
end

function netrecv_changtoken_C( buffer )
	local recvValue = struct_NetS_Token_recv( buffer );
	proc_changtoken_C( recvValue );
end

Proc_Command = {
	[CMDS_LOGIN]=netrecv_login_C;
	[CMDS_LIST]=netrecv_list_C;
	[CMDS_CREATE]=netrecv_create_C;
	[CMDS_ENTERINFO]=netrecv_enterinfo_C;
	[CMDS_DELETE]=netrecv_delete_C;
	[CMDS_HEART]=netrecv_heart_C;
	[CMDS_NOTIFY]=netrecv_notify_C;
	[CMDS_ACTORINFO]=netrecv_actorinfo_C;
	[CMDS_ITEMUSE]=netrecv_itemuse_C;
	[CMDS_ITEMPUT]=netrecv_itemput_C;
	[CMDS_ITEMSETTLE]=netrecv_itemsettle_C;
	[CMDS_LOSTITEM]=netrecv_lostitem_C;
	[CMDS_GETITEM]=netrecv_getitem_C;
	[CMDS_ITEMLIST]=netrecv_itemlist_C;
	[CMDS_ITEMINFO]=netrecv_iteminfo_C;
	[CMDS_ADDMAPUNIT]=netrecv_addmapunit_C;
	[CMDS_DELMAPUNIT]=netrecv_delmapunit_C;
	[CMDS_UPDATEMAPUNIT]=netrecv_updatemapunit_C;
	[CMDS_MAPUNITSINGLE]=netrecv_mapunitsingle_C;
	[CMDS_WORLDMAPINFO]=netrecv_worldmapinfo_C;
	[CMDS_MAPUNITCORRDINATE]=netrecv_mapunitcorrdinate_C;
	[CMDS_ADDMARCHROUTE]=netrecv_addmarchroute_C;
	[CMDS_DELMARCHROUTE]=netrecv_delmarchroute_C;
	[CMDS_AWARDINFOLIST]=netrecv_awardinfolist_C;
	[CMDS_EXPERIENCE]=netrecv_experience_C;
	[CMDS_BODY]=netrecv_body_C;
	[CMDS_BUILDINGLIST]=netrecv_buildinglist_C;
	[CMDS_CHANGELEVY]=netrecv_changelevy_C;
	[CMDS_CHANGESILVER]=netrecv_changesilver_C;
	[CMDS_CHANGEWOOD]=netrecv_changewood_C;
	[CMDS_CHANGEFOOD]=netrecv_changefood_C;
	[CMDS_CHANGEIRON]=netrecv_changeiron_C;
	[CMDS_CHANGEPEOPLE]=netrecv_changepeople_C;
	[CMDS_CHANGEPRESTIGE]=netrecv_changeprestige_C;
	[CMDS_CHANGEFRIENDSHIP]=netrecv_changefriendship_C;
	[CMDS_CHANGEVIP]=netrecv_changevip_C;
	[CMDS_CHANGTOKEN]=netrecv_changtoken_C;
}

function in_proc_command_C( cmd, buffer )
	if Proc_Command[cmd] ~= nil then
		Proc_Command[cmd]( buffer );
		return 1;
	end
	return 0;
end
