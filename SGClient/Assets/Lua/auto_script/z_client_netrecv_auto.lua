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

function netrecv_equip_C( buffer )
	local recvValue = struct_NetS_Equip_recv( buffer );
	proc_equip_C( recvValue );
end

function netrecv_equiplist_C( buffer )
	local recvValue = struct_NetS_EquipList_recv( buffer );
	proc_equiplist_C( recvValue );
end

function netrecv_equipget_C( buffer )
	local recvValue = struct_NetS_EquipGet_recv( buffer );
	proc_equipget_C( recvValue );
end

function netrecv_equiplost_C( buffer )
	local recvValue = struct_NetS_EquipLost_recv( buffer );
	proc_equiplost_C( recvValue );
end

function netrecv_hero_C( buffer )
	local recvValue = struct_NetS_Hero_recv( buffer );
	proc_hero_C( recvValue );
end

function netrecv_herolist_C( buffer )
	local recvValue = struct_NetS_HeroList_recv( buffer );
	proc_herolist_C( recvValue );
end

function netrecv_heroexp_C( buffer )
	local recvValue = struct_NetS_HeroExp_recv( buffer );
	proc_heroexp_C( recvValue );
end

function netrecv_herosoldiers_C( buffer )
	local recvValue = struct_NetS_HeroSoldiers_recv( buffer );
	proc_herosoldiers_C( recvValue );
end

function netrecv_herostate_C( buffer )
	local recvValue = struct_NetS_HeroState_recv( buffer );
	proc_herostate_C( recvValue );
end

function netrecv_heroreplace_C( buffer )
	local recvValue = struct_NetS_HeroReplace_recv( buffer );
	proc_heroreplace_C( recvValue );
end

function netrecv_buildingupgradeinfo_C( buffer )
	local recvValue = struct_NetS_BuildingUpgradeInfo_recv( buffer );
	proc_buildingupgradeinfo_C( recvValue );
end

function netrecv_building_C( buffer )
	local recvValue = struct_NetS_Building_recv( buffer );
	proc_building_C( recvValue );
end

function netrecv_buildingbarracks_C( buffer )
	local recvValue = struct_NetS_BuildingBarracks_recv( buffer );
	proc_buildingbarracks_C( recvValue );
end

function netrecv_buildingres_C( buffer )
	local recvValue = struct_NetS_BuildingRes_recv( buffer );
	proc_buildingres_C( recvValue );
end

function netrecv_worker_C( buffer )
	local recvValue = struct_NetS_Worker_recv( buffer );
	proc_worker_C( recvValue );
end

function netrecv_heroget_C( buffer )
	local recvValue = struct_NetS_HeroGet_recv( buffer );
	proc_heroget_C( recvValue );
end

function netrecv_buildingget_C( buffer )
	local recvValue = struct_NetS_BuildingGet_recv( buffer );
	proc_buildingget_C( recvValue );
end

function netrecv_buildingbarracksget_C( buffer )
	local recvValue = struct_NetS_BuildingBarracksGet_recv( buffer );
	proc_buildingbarracksget_C( recvValue );
end

function netrecv_buildingresget_C( buffer )
	local recvValue = struct_NetS_BuildingResGet_recv( buffer );
	proc_buildingresget_C( recvValue );
end

function netrecv_soldiers_C( buffer )
	local recvValue = struct_NetS_Soldiers_recv( buffer );
	proc_soldiers_C( recvValue );
end

function netrecv_traininfo_C( buffer )
	local recvValue = struct_NetS_TrainInfo_recv( buffer );
	proc_traininfo_C( recvValue );
end

function netrecv_quest_C( buffer )
	local recvValue = struct_NetS_Quest_recv( buffer );
	proc_quest_C( recvValue );
end

function netrecv_questlist_C( buffer )
	local recvValue = struct_NetS_QuestList_recv( buffer );
	proc_questlist_C( recvValue );
end

function netrecv_questaward_C( buffer )
	local recvValue = struct_NetS_QuestAward_recv( buffer );
	proc_questaward_C( recvValue );
end

function netrecv_function_C( buffer )
	local recvValue = struct_NetS_Function_recv( buffer );
	proc_function_C( recvValue );
end

function netrecv_cityguard_C( buffer )
	local recvValue = struct_NetS_CityGuard_recv( buffer );
	proc_cityguard_C( recvValue );
end

function netrecv_cityguardlist_C( buffer )
	local recvValue = struct_NetS_CityGuardList_recv( buffer );
	proc_cityguardlist_C( recvValue );
end

function netrecv_cityguardsec_C( buffer )
	local recvValue = struct_NetS_CityGuardSec_recv( buffer );
	proc_cityguardsec_C( recvValue );
end

function netrecv_buildingsmithy_C( buffer )
	local recvValue = struct_NetS_BuildingSmithy_recv( buffer );
	proc_buildingsmithy_C( recvValue );
end

function netrecv_changename_C( buffer )
	local recvValue = struct_NetS_ChangeName_recv( buffer );
	proc_changename_C( recvValue );
end

function netrecv_buildingaction_C( buffer )
	local recvValue = struct_NetS_BuildingAction_recv( buffer );
	proc_buildingaction_C( recvValue );
end

function netrecv_levyinfo_C( buffer )
	local recvValue = struct_NetS_LevyInfo_recv( buffer );
	proc_levyinfo_C( recvValue );
end

function netrecv_chat_C( buffer )
	local recvValue = struct_NetS_Chat_recv( buffer );
	proc_chat_C( recvValue );
end

function netrecv_chatlist_C( buffer )
	local recvValue = struct_NetS_ChatList_recv( buffer );
	proc_chatlist_C( recvValue );
end

function netrecv_systalkid_C( buffer )
	local recvValue = struct_NetS_Systalkid_recv( buffer );
	proc_systalkid_C( recvValue );
end

function netrecv_systalk_C( buffer )
	local recvValue = struct_NetS_Systalk_recv( buffer );
	proc_systalk_C( recvValue );
end

function netrecv_battlepower_C( buffer )
	local recvValue = struct_NetS_Battlepower_recv( buffer );
	proc_battlepower_C( recvValue );
end

function netrecv_techchange_C( buffer )
	local recvValue = struct_NetS_TechChange_recv( buffer );
	proc_techchange_C( recvValue );
end

function netrecv_cityeventlist_C( buffer )
	local recvValue = struct_NetS_EventList_recv( buffer );
	proc_cityeventlist_C( recvValue );
end

function netrecv_officialhirechange_C( buffer )
	local recvValue = struct_NetS_OfficialHireChange_recv( buffer );
	proc_officialhirechange_C( recvValue );
end

function netrecv_cityprotect_C( buffer )
	local recvValue = struct_NetS_CityProtect_recv( buffer );
	proc_cityprotect_C( recvValue );
end

function netrecv_heroequip_C( buffer )
	local recvValue = struct_NetS_HeroEquip_recv( buffer );
	proc_heroequip_C( recvValue );
end

function netrecv_herowash_C( buffer )
	local recvValue = struct_NetS_HeroWash_recv( buffer );
	proc_herowash_C( recvValue );
end

function netrecv_herocolorup_C( buffer )
	local recvValue = struct_NetS_HeroColorup_recv( buffer );
	proc_herocolorup_C( recvValue );
end

function netrecv_cityattr_C( buffer )
	local recvValue = struct_NetS_CityAttr_recv( buffer );
	proc_cityattr_C( recvValue );
end

function netrecv_equipwash_C( buffer )
	local recvValue = struct_NetS_EquipWash_recv( buffer );
	proc_equipwash_C( recvValue );
end

function netrecv_materiallist_C( buffer )
	local recvValue = struct_NetS_MaterialList_recv( buffer );
	proc_materiallist_C( recvValue );
end

function netrecv_materialwilllist_C( buffer )
	local recvValue = struct_NetS_MaterialWillList_recv( buffer );
	proc_materialwilllist_C( recvValue );
end

function netrecv_storylist_C( buffer )
	local recvValue = struct_NetS_StoryList_recv( buffer );
	proc_storylist_C( recvValue );
end

function netrecv_storyrank_C( buffer )
	local recvValue = struct_NetS_StoryRank_recv( buffer );
	proc_storyrank_C( recvValue );
end

function netrecv_storystate_C( buffer )
	local recvValue = struct_NetS_StoryState_recv( buffer );
	proc_storystate_C( recvValue );
end

function netrecv_storyranknum_C( buffer )
	local recvValue = struct_NetS_StoryRanknum_recv( buffer );
	proc_storyranknum_C( recvValue );
end

function netrecv_storyranktime_C( buffer )
	local recvValue = struct_NetS_StoryRanktime_recv( buffer );
	proc_storyranktime_C( recvValue );
end

function netrecv_mapzonechange_C( buffer )
	local recvValue = struct_NetS_MapZoneChange_recv( buffer );
	proc_mapzonechange_C( recvValue );
end

function netrecv_mapzoneunitlist_C( buffer )
	local recvValue = struct_NetS_MapZoneUnitList_recv( buffer );
	proc_mapzoneunitlist_C( recvValue );
end

function netrecv_mapzoneunit_C( buffer )
	local recvValue = struct_NetS_ZoneUnit_recv( buffer );
	proc_mapzoneunit_C( recvValue );
end

function netrecv_armyspeedupdate_C( buffer )
	local recvValue = struct_NetS_ArmySpeedUpdate_recv( buffer );
	proc_armyspeedupdate_C( recvValue );
end

function netrecv_battlelist_C( buffer )
	local recvValue = struct_NetS_BattleList_recv( buffer );
	proc_battlelist_C( recvValue );
end

function netrecv_battleinfo_C( buffer )
	local recvValue = struct_NetS_BattleInfo_recv( buffer );
	proc_battleinfo_C( recvValue );
end

function netrecv_mapresinfo_C( buffer )
	local recvValue = struct_NetS_MapResInfo_recv( buffer );
	proc_mapresinfo_C( recvValue );
end

function netrecv_weatherchange_C( buffer )
	local recvValue = struct_NetS_WeatherChange_recv( buffer );
	proc_weatherchange_C( recvValue );
end

function netrecv_mail_C( buffer )
	local recvValue = struct_NetS_Mail_recv( buffer );
	proc_mail_C( recvValue );
end

function netrecv_mailopresult_C( buffer )
	local recvValue = struct_NetS_MailOpResult_recv( buffer );
	proc_mailopresult_C( recvValue );
end

function netrecv_mailfight_C( buffer )
	local recvValue = struct_NetS_MailFight_recv( buffer );
	proc_mailfight_C( recvValue );
end

function netrecv_mailview_C( buffer )
	local recvValue = struct_NetS_MailView_recv( buffer );
	proc_mailview_C( recvValue );
end

function netrecv_cityhelp_C( buffer )
	local recvValue = struct_NetS_CItyHelp_recv( buffer );
	proc_cityhelp_C( recvValue );
end

function netrecv_cityhelplist_C( buffer )
	local recvValue = struct_NetS_CItyHelpList_recv( buffer );
	proc_cityhelplist_C( recvValue );
end

function netrecv_mapcityhelplist_C( buffer )
	local recvValue = struct_NetS_MapCItyHelpList_recv( buffer );
	proc_mapcityhelplist_C( recvValue );
end

function netrecv_citystate_C( buffer )
	local recvValue = struct_NetS_CityState_recv( buffer );
	proc_citystate_C( recvValue );
end

function netrecv_cityarmygroup_C( buffer )
	local recvValue = struct_NetS_CityArmyGroup_recv( buffer );
	proc_cityarmygroup_C( recvValue );
end

function netrecv_cityarmygrouplist_C( buffer )
	local recvValue = struct_NetS_CityArmyGroupList_recv( buffer );
	proc_cityarmygrouplist_C( recvValue );
end

function netrecv_maptowninfo_C( buffer )
	local recvValue = struct_NetS_MapTownInfo_recv( buffer );
	proc_maptowninfo_C( recvValue );
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
	[CMDS_EQUIP]=netrecv_equip_C;
	[CMDS_EQUIPLIST]=netrecv_equiplist_C;
	[CMDS_EQUIPGET]=netrecv_equipget_C;
	[CMDS_EQUIPLOST]=netrecv_equiplost_C;
	[CMDS_HERO]=netrecv_hero_C;
	[CMDS_HEROLIST]=netrecv_herolist_C;
	[CMDS_HEROEXP]=netrecv_heroexp_C;
	[CMDS_HEROSOLDIERS]=netrecv_herosoldiers_C;
	[CMDS_HEROSTATE]=netrecv_herostate_C;
	[CMDS_HEROREPLACE]=netrecv_heroreplace_C;
	[CMDS_BUILDINGUPGRADEINFO]=netrecv_buildingupgradeinfo_C;
	[CMDS_BUILDING]=netrecv_building_C;
	[CMDS_BUILDINGBARRACKS]=netrecv_buildingbarracks_C;
	[CMDS_BUILDINGRES]=netrecv_buildingres_C;
	[CMDS_WORKER]=netrecv_worker_C;
	[CMDS_HEROGET]=netrecv_heroget_C;
	[CMDS_BUILDINGGET]=netrecv_buildingget_C;
	[CMDS_BUILDINGBARRACKSGET]=netrecv_buildingbarracksget_C;
	[CMDS_BUILDINGRESGET]=netrecv_buildingresget_C;
	[CMDS_SOLDIERS]=netrecv_soldiers_C;
	[CMDS_TRAININFO]=netrecv_traininfo_C;
	[CMDS_QUEST]=netrecv_quest_C;
	[CMDS_QUESTLIST]=netrecv_questlist_C;
	[CMDS_QUESTAWARD]=netrecv_questaward_C;
	[CMDS_FUNCTION]=netrecv_function_C;
	[CMDS_CITYGUARD]=netrecv_cityguard_C;
	[CMDS_CITYGUARDLIST]=netrecv_cityguardlist_C;
	[CMDS_CITYGUARDSEC]=netrecv_cityguardsec_C;
	[CMDS_BUILDINGSMITHY]=netrecv_buildingsmithy_C;
	[CMDS_CHANGENAME]=netrecv_changename_C;
	[CMDS_BUILDINGACTION]=netrecv_buildingaction_C;
	[CMDS_LEVYINFO]=netrecv_levyinfo_C;
	[CMDS_CHAT]=netrecv_chat_C;
	[CMDS_CHATLIST]=netrecv_chatlist_C;
	[CMDS_SYSTALKID]=netrecv_systalkid_C;
	[CMDS_SYSTALK]=netrecv_systalk_C;
	[CMDS_BATTLEPOWER]=netrecv_battlepower_C;
	[CMDS_TECHCHANGE]=netrecv_techchange_C;
	[CMDS_CITYEVENTLIST]=netrecv_cityeventlist_C;
	[CMDS_OFFICIALHIRECHANGE]=netrecv_officialhirechange_C;
	[CMDS_CITYPROTECT]=netrecv_cityprotect_C;
	[CMDS_HEROEQUIP]=netrecv_heroequip_C;
	[CMDS_HEROWASH]=netrecv_herowash_C;
	[CMDS_HEROCOLORUP]=netrecv_herocolorup_C;
	[CMDS_CITYATTR]=netrecv_cityattr_C;
	[CMDS_EQUIPWASH]=netrecv_equipwash_C;
	[CMDS_MATERIALLIST]=netrecv_materiallist_C;
	[CMDS_MATERIALWILLLIST]=netrecv_materialwilllist_C;
	[CMDS_STORYLIST]=netrecv_storylist_C;
	[CMDS_STORYRANK]=netrecv_storyrank_C;
	[CMDS_STORYSTATE]=netrecv_storystate_C;
	[CMDS_STORYRANKNUM]=netrecv_storyranknum_C;
	[CMDS_STORYRANKTIME]=netrecv_storyranktime_C;
	[CMDS_MAPZONECHANGE]=netrecv_mapzonechange_C;
	[CMDS_MAPZONEUNITLIST]=netrecv_mapzoneunitlist_C;
	[CMDS_MAPZONEUNIT]=netrecv_mapzoneunit_C;
	[CMDS_ARMYSPEEDUPDATE]=netrecv_armyspeedupdate_C;
	[CMDS_BATTLELIST]=netrecv_battlelist_C;
	[CMDS_BATTLEINFO]=netrecv_battleinfo_C;
	[CMDS_MAPRESINFO]=netrecv_mapresinfo_C;
	[CMDS_WEATHERCHANGE]=netrecv_weatherchange_C;
	[CMDS_MAIL]=netrecv_mail_C;
	[CMDS_MAILOPRESULT]=netrecv_mailopresult_C;
	[CMDS_MAILFIGHT]=netrecv_mailfight_C;
	[CMDS_MAILVIEW]=netrecv_mailview_C;
	[CMDS_CITYHELP]=netrecv_cityhelp_C;
	[CMDS_CITYHELPLIST]=netrecv_cityhelplist_C;
	[CMDS_MAPCITYHELPLIST]=netrecv_mapcityhelplist_C;
	[CMDS_CITYSTATE]=netrecv_citystate_C;
	[CMDS_CITYARMYGROUP]=netrecv_cityarmygroup_C;
	[CMDS_CITYARMYGROUPLIST]=netrecv_cityarmygrouplist_C;
	[CMDS_MAPTOWNINFO]=netrecv_maptowninfo_C;
}

function in_proc_command_C( cmd, buffer )
	if Proc_Command[cmd] ~= nil then
		Proc_Command[cmd]( buffer );
		return 1;
	end
	return 0;
end
