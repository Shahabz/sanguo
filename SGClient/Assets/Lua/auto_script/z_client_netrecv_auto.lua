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

function netrecv_townarmygroup_C( buffer )
	local recvValue = struct_NetS_TownArmyGroup_recv( buffer );
	proc_townarmygroup_C( recvValue );
end

function netrecv_townarmygrouplist_C( buffer )
	local recvValue = struct_NetS_TownArmyGroupList_recv( buffer );
	proc_townarmygrouplist_C( recvValue );
end

function netrecv_systalkjson_C( buffer )
	local recvValue = struct_NetS_SystalkJson_recv( buffer );
	proc_systalkjson_C( recvValue );
end

function netrecv_rollmsgjson_C( buffer )
	local recvValue = struct_NetS_RollMsgJson_recv( buffer );
	proc_rollmsgjson_C( recvValue );
end

function netrecv_rollmsg_C( buffer )
	local recvValue = struct_NetS_RollMsg_recv( buffer );
	proc_rollmsg_C( recvValue );
end

function netrecv_townowneraskList_C( buffer )
	local recvValue = struct_NetS_TownOwnerAskList_recv( buffer );
	proc_townowneraskList_C( recvValue );
end

function netrecv_townfightlist_C( buffer )
	local recvValue = struct_NetS_TownFightList_recv( buffer );
	proc_townfightlist_C( recvValue );
end

function netrecv_dialogupdate_C( buffer )
	local recvValue = struct_NetS_DialogUpdate_recv( buffer );
	proc_dialogupdate_C( recvValue );
end

function netrecv_maptownexinfo_C( buffer )
	local recvValue = struct_NetS_MapTownExInfo_recv( buffer );
	proc_maptownexinfo_C( recvValue );
end

function netrecv_mapzonetown_C( buffer )
	local recvValue = struct_NetS_MapZoneTown_recv( buffer );
	proc_mapzonetown_C( recvValue );
end

function netrecv_mapzonetownlist_C( buffer )
	local recvValue = struct_NetS_MapZoneTownList_recv( buffer );
	proc_mapzonetownlist_C( recvValue );
end

function netrecv_mapcentertown_C( buffer )
	local recvValue = struct_NetS_MapCenterTown_recv( buffer );
	proc_mapcentertown_C( recvValue );
end

function netrecv_mapcentertownlist_C( buffer )
	local recvValue = struct_NetS_MapCenterTownList_recv( buffer );
	proc_mapcentertownlist_C( recvValue );
end

function netrecv_citywarinfo_C( buffer )
	local recvValue = struct_NetS_CityWarInfo_recv( buffer );
	proc_citywarinfo_C( recvValue );
end

function netrecv_citywarlist_C( buffer )
	local recvValue = struct_NetS_CityWarList_recv( buffer );
	proc_citywarlist_C( recvValue );
end

function netrecv_citywardel_C( buffer )
	local recvValue = struct_NetS_CityWarDel_recv( buffer );
	proc_citywardel_C( recvValue );
end

function netrecv_worldquest_C( buffer )
	local recvValue = struct_NetS_WorldQuest_recv( buffer );
	proc_worldquest_C( recvValue );
end

function netrecv_worldboss_C( buffer )
	local recvValue = struct_NetS_WorldBoss_recv( buffer );
	proc_worldboss_C( recvValue );
end

function netrecv_lostrebuild_C( buffer )
	local recvValue = struct_NetS_LostRebuild_recv( buffer );
	proc_lostrebuild_C( recvValue );
end

function netrecv_kingwartown_C( buffer )
	local recvValue = struct_NetS_KingwarTown_recv( buffer );
	proc_kingwartown_C( recvValue );
end

function netrecv_kingwartownlist_C( buffer )
	local recvValue = struct_NetS_KingwarTownList_recv( buffer );
	proc_kingwartownlist_C( recvValue );
end

function netrecv_kingwaractivity_C( buffer )
	local recvValue = struct_NetS_KingWarActivity_recv( buffer );
	proc_kingwaractivity_C( recvValue );
end

function netrecv_kingwarnotify_C( buffer )
	local recvValue = struct_NetS_KingWarNotify_recv( buffer );
	proc_kingwarnotify_C( recvValue );
end

function netrecv_kingwarnotifylist_C( buffer )
	local recvValue = struct_NetS_KingWarNotifyList_recv( buffer );
	proc_kingwarnotifylist_C( recvValue );
end

function netrecv_kingwarranklist_C( buffer )
	local recvValue = struct_NetS_KingWarRankList_recv( buffer );
	proc_kingwarranklist_C( recvValue );
end

function netrecv_kingwarpk_C( buffer )
	local recvValue = struct_NetS_KingWarPK_recv( buffer );
	proc_kingwarpk_C( recvValue );
end

function netrecv_kingwarpoint_C( buffer )
	local recvValue = struct_NetS_KingWarPoint_recv( buffer );
	proc_kingwarpoint_C( recvValue );
end

function netrecv_treasureactivity_C( buffer )
	local recvValue = struct_NetS_TreasureActivity_recv( buffer );
	proc_treasureactivity_C( recvValue );
end

function netrecv_treasurehas_C( buffer )
	local recvValue = struct_NetS_TreasureHas_recv( buffer );
	proc_treasurehas_C( recvValue );
end

function netrecv_gotoasyn_C( buffer )
	local recvValue = struct_NetS_GotoAsyn_recv( buffer );
	proc_gotoasyn_C( recvValue );
end

function netrecv_treasureactorlist_C( buffer )
	local recvValue = struct_NetS_TreasureActorList_recv( buffer );
	proc_treasureactorlist_C( recvValue );
end

function netrecv_changeautobuild_C( buffer )
	local recvValue = struct_NetS_ChangeAutoBuild_recv( buffer );
	proc_changeautobuild_C( recvValue );
end

function netrecv_herovisitinfo_C( buffer )
	local recvValue = struct_NetS_HeroVisitInfo_recv( buffer );
	proc_herovisitinfo_C( recvValue );
end

function netrecv_nationbase_C( buffer )
	local recvValue = struct_NetS_NationBase_recv( buffer );
	proc_nationbase_C( recvValue );
end

function netrecv_herovisitaward_C( buffer )
	local recvValue = struct_NetS_HeroVisitAward_recv( buffer );
	proc_herovisitaward_C( recvValue );
end

function netrecv_fightplay_C( buffer )
	local recvValue = struct_NetS_FightPlay_recv( buffer );
	proc_fightplay_C( recvValue );
end

function netrecv_storysweepresult_C( buffer )
	local recvValue = struct_NetS_StorySweepResult_recv( buffer );
	proc_storysweepresult_C( recvValue );
end

function netrecv_questtalk_C( buffer )
	local recvValue = struct_NetS_QuestTalk_recv( buffer );
	proc_questtalk_C( recvValue );
end

function netrecv_ranklist_C( buffer )
	local recvValue = struct_NetS_RankList_recv( buffer );
	proc_ranklist_C( recvValue );
end

function netrecv_friendlist_C( buffer )
	local recvValue = struct_NetS_FriendList_recv( buffer );
	proc_friendlist_C( recvValue );
end

function netrecv_actorsearch_C( buffer )
	local recvValue = struct_NetS_ActorSearch_recv( buffer );
	proc_actorsearch_C( recvValue );
end

function netrecv_blackinfo_C( buffer )
	local recvValue = struct_NetS_BlackInfo_recv( buffer );
	proc_blackinfo_C( recvValue );
end

function netrecv_blacklist_C( buffer )
	local recvValue = struct_NetS_BlackList_recv( buffer );
	proc_blacklist_C( recvValue );
end

function netrecv_blacklistid_C( buffer )
	local recvValue = struct_NetS_BlackListID_recv( buffer );
	proc_blacklistid_C( recvValue );
end

function netrecv_nationequip_C( buffer )
	local recvValue = struct_NetS_NationEquip_recv( buffer );
	proc_nationequip_C( recvValue );
end

function netrecv_shoplist_C( buffer )
	local recvValue = struct_NetS_ShopList_recv( buffer );
	proc_shoplist_C( recvValue );
end

function netrecv_paystore_C( buffer )
	local recvValue = struct_NetS_PayStore_recv( buffer );
	proc_paystore_C( recvValue );
end

function netrecv_goodsinfo_C( buffer )
	local recvValue = struct_NetS_GoodsInfo_recv( buffer );
	proc_goodsinfo_C( recvValue );
end

function netrecv_paystoreactivity_C( buffer )
	local recvValue = struct_NetS_PayStoreActivity_recv( buffer );
	proc_paystoreactivity_C( recvValue );
end

function netrecv_payorder_C( buffer )
	local recvValue = struct_NetS_PayOrder_recv( buffer );
	proc_payorder_C( recvValue );
end

function netrecv_vipshop_C( buffer )
	local recvValue = struct_NetS_VipShop_recv( buffer );
	proc_vipshop_C( recvValue );
end

function netrecv_vipbag_C( buffer )
	local recvValue = struct_NetS_VipBag_recv( buffer );
	proc_vipbag_C( recvValue );
end

function netrecv_buffchange_C( buffer )
	local recvValue = struct_NetS_BuffChange_recv( buffer );
	proc_buffchange_C( recvValue );
end

function netrecv_changeautoguard_C( buffer )
	local recvValue = struct_NetS_ChangeAutoGuard_recv( buffer );
	proc_changeautoguard_C( recvValue );
end

function netrecv_activitylist_C( buffer )
	local recvValue = struct_NetS_ActivityList_recv( buffer );
	proc_activitylist_C( recvValue );
end

function netrecv_nationequiplist_C( buffer )
	local recvValue = struct_NetS_NationEquipList_recv( buffer );
	proc_nationequiplist_C( recvValue );
end

function netrecv_nationinfo_C( buffer )
	local recvValue = struct_NetS_NationInfo_recv( buffer );
	proc_nationinfo_C( recvValue );
end

function netrecv_nationtown_C( buffer )
	local recvValue = struct_NetS_NationTown_recv( buffer );
	proc_nationtown_C( recvValue );
end

function netrecv_nationtownlist_C( buffer )
	local recvValue = struct_NetS_NationTownList_recv( buffer );
	proc_nationtownlist_C( recvValue );
end

function netrecv_nationwarlist_C( buffer )
	local recvValue = struct_NetS_NationWarList_recv( buffer );
	proc_nationwarlist_C( recvValue );
end

function netrecv_nationcitywarlist_C( buffer )
	local recvValue = struct_NetS_NationCityWarList_recv( buffer );
	proc_nationcitywarlist_C( recvValue );
end

function netrecv_nationquestlist_C( buffer )
	local recvValue = struct_NetS_NationQuestList_recv( buffer );
	proc_nationquestlist_C( recvValue );
end

function netrecv_nationmissionlist_C( buffer )
	local recvValue = struct_NetS_NationMissionList_recv( buffer );
	proc_nationmissionlist_C( recvValue );
end

function netrecv_nationranklist_C( buffer )
	local recvValue = struct_NetS_NationRankList_recv( buffer );
	proc_nationranklist_C( recvValue );
end

function netrecv_nationofficiallist_C( buffer )
	local recvValue = struct_NetS_NationOfficialList_recv( buffer );
	proc_nationofficiallist_C( recvValue );
end

function netrecv_nationcandidatelist_C( buffer )
	local recvValue = struct_NetS_NationCandidateList_recv( buffer );
	proc_nationcandidatelist_C( recvValue );
end

function netrecv_worlddataopen_C( buffer )
	local recvValue = struct_NetS_WorldDataOpen_recv( buffer );
	proc_worlddataopen_C( recvValue );
end

function netrecv_nationreplacelist_C( buffer )
	local recvValue = struct_NetS_NationReplaceList_recv( buffer );
	proc_nationreplacelist_C( recvValue );
end

function netrecv_nationloglist_C( buffer )
	local recvValue = struct_NetS_NationLogList_recv( buffer );
	proc_nationloglist_C( recvValue );
end

function netrecv_nationherolist_C( buffer )
	local recvValue = struct_NetS_NationHeroList_recv( buffer );
	proc_nationherolist_C( recvValue );
end

function netrecv_citynationhero_C( buffer )
	local recvValue = struct_NetS_CityNationHero_recv( buffer );
	proc_citynationhero_C( recvValue );
end

function netrecv_citynationherolist_C( buffer )
	local recvValue = struct_NetS_CityNationHeroList_recv( buffer );
	proc_citynationherolist_C( recvValue );
end

function netrecv_nationheroattr_C( buffer )
	local recvValue = struct_NetS_NationHeroAttr_recv( buffer );
	proc_nationheroattr_C( recvValue );
end

function netrecv_wishingshop_C( buffer )
	local recvValue = struct_NetS_WishingShop_recv( buffer );
	proc_wishingshop_C( recvValue );
end

function netrecv_wishingchange_C( buffer )
	local recvValue = struct_NetS_WishingChange_recv( buffer );
	proc_wishingchange_C( recvValue );
end

function netrecv_wishingpack_C( buffer )
	local recvValue = struct_NetS_WishingPack_recv( buffer );
	proc_wishingpack_C( recvValue );
end

function netrecv_tiancequest_C( buffer )
	local recvValue = struct_NetS_TianceQuest_recv( buffer );
	proc_tiancequest_C( recvValue );
end

function netrecv_studentlist_C( buffer )
	local recvValue = struct_NetS_StudentList_recv( buffer );
	proc_studentlist_C( recvValue );
end

function netrecv_teachershoplist_C( buffer )
	local recvValue = struct_NetS_TeacherShopList_recv( buffer );
	proc_teachershoplist_C( recvValue );
end

function netrecv_activity03list_C( buffer )
	local recvValue = struct_NetS_Activity03List_recv( buffer );
	proc_activity03list_C( recvValue );
end

function netrecv_activity05list_C( buffer )
	local recvValue = struct_NetS_Activity05List_recv( buffer );
	proc_activity05list_C( recvValue );
end

function netrecv_activity01list_C( buffer )
	local recvValue = struct_NetS_Activity01List_recv( buffer );
	proc_activity01list_C( recvValue );
end

function netrecv_activity04list_C( buffer )
	local recvValue = struct_NetS_Activity04List_recv( buffer );
	proc_activity04list_C( recvValue );
end

function netrecv_girl_C( buffer )
	local recvValue = struct_NetS_Girl_recv( buffer );
	proc_girl_C( recvValue );
end

function netrecv_girllist_C( buffer )
	local recvValue = struct_NetS_GirlList_recv( buffer );
	proc_girllist_C( recvValue );
end

function netrecv_girlget_C( buffer )
	local recvValue = struct_NetS_GirlGet_recv( buffer );
	proc_girlget_C( recvValue );
end

function netrecv_girllove_C( buffer )
	local recvValue = struct_NetS_GirlLove_recv( buffer );
	proc_girllove_C( recvValue );
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
	[CMDS_TOWNARMYGROUP]=netrecv_townarmygroup_C;
	[CMDS_TOWNARMYGROUPLIST]=netrecv_townarmygrouplist_C;
	[CMDS_SYSTALKJSON]=netrecv_systalkjson_C;
	[CMDS_ROLLMSGJSON]=netrecv_rollmsgjson_C;
	[CMDS_ROLLMSG]=netrecv_rollmsg_C;
	[CMDS_TOWNOWNERASKLIST]=netrecv_townowneraskList_C;
	[CMDS_TOWNFIGHTLIST]=netrecv_townfightlist_C;
	[CMDS_DIALOGUPDATE]=netrecv_dialogupdate_C;
	[CMDS_MAPTOWNEXINFO]=netrecv_maptownexinfo_C;
	[CMDS_MAPZONETOWN]=netrecv_mapzonetown_C;
	[CMDS_MAPZONETOWNLIST]=netrecv_mapzonetownlist_C;
	[CMDS_MAPCENTERTOWN]=netrecv_mapcentertown_C;
	[CMDS_MAPCENTERTOWNLIST]=netrecv_mapcentertownlist_C;
	[CMDS_CITYWARINFO]=netrecv_citywarinfo_C;
	[CMDS_CITYWARLIST]=netrecv_citywarlist_C;
	[CMDS_CITYWARDEL]=netrecv_citywardel_C;
	[CMDS_WORLDQUEST]=netrecv_worldquest_C;
	[CMDS_WORLDBOSS]=netrecv_worldboss_C;
	[CMDS_LOSTREBUILD]=netrecv_lostrebuild_C;
	[CMDS_KINGWARTOWN]=netrecv_kingwartown_C;
	[CMDS_KINGWARTOWNLIST]=netrecv_kingwartownlist_C;
	[CMDS_KINGWARACTIVITY]=netrecv_kingwaractivity_C;
	[CMDS_KINGWARNOTIFY]=netrecv_kingwarnotify_C;
	[CMDS_KINGWARNOTIFYLIST]=netrecv_kingwarnotifylist_C;
	[CMDS_KINGWARRANKLIST]=netrecv_kingwarranklist_C;
	[CMDS_KINGWARPK]=netrecv_kingwarpk_C;
	[CMDS_KINGWARPOINT]=netrecv_kingwarpoint_C;
	[CMDS_TREASUREACTIVITY]=netrecv_treasureactivity_C;
	[CMDS_TREASUREHAS]=netrecv_treasurehas_C;
	[CMDS_GOTOASYN]=netrecv_gotoasyn_C;
	[CMDS_TREASUREACTORLIST]=netrecv_treasureactorlist_C;
	[CMDS_CHANGEAUTOBUILD]=netrecv_changeautobuild_C;
	[CMDS_HEROVISITINFO]=netrecv_herovisitinfo_C;
	[CMDS_NATIONBASE]=netrecv_nationbase_C;
	[CMDS_HEROVISITAWARD]=netrecv_herovisitaward_C;
	[CMDS_FIGHTPLAY]=netrecv_fightplay_C;
	[CMDS_STORYSWEEPRESULT]=netrecv_storysweepresult_C;
	[CMDS_QUESTTALK]=netrecv_questtalk_C;
	[CMDS_RANKLIST]=netrecv_ranklist_C;
	[CMDS_FRIENDLIST]=netrecv_friendlist_C;
	[CMDS_ACTORSEARCH]=netrecv_actorsearch_C;
	[CMDS_BLACKINFO]=netrecv_blackinfo_C;
	[CMDS_BLACKLIST]=netrecv_blacklist_C;
	[CMDS_BLACKLISTID]=netrecv_blacklistid_C;
	[CMDS_NATIONEQUIP]=netrecv_nationequip_C;
	[CMDS_SHOPLIST]=netrecv_shoplist_C;
	[CMDS_PAYSTORE]=netrecv_paystore_C;
	[CMDS_GOODSINFO]=netrecv_goodsinfo_C;
	[CMDS_PAYSTOREACTIVITY]=netrecv_paystoreactivity_C;
	[CMDS_PAYORDER]=netrecv_payorder_C;
	[CMDS_VIPSHOP]=netrecv_vipshop_C;
	[CMDS_VIPBAG]=netrecv_vipbag_C;
	[CMDS_BUFFCHANGE]=netrecv_buffchange_C;
	[CMDS_CHANGEAUTOGUARD]=netrecv_changeautoguard_C;
	[CMDS_ACTIVITYLIST]=netrecv_activitylist_C;
	[CMDS_NATIONEQUIPLIST]=netrecv_nationequiplist_C;
	[CMDS_NATIONINFO]=netrecv_nationinfo_C;
	[CMDS_NATIONTOWN]=netrecv_nationtown_C;
	[CMDS_NATIONTOWNLIST]=netrecv_nationtownlist_C;
	[CMDS_NATIONWARLIST]=netrecv_nationwarlist_C;
	[CMDS_NATIONCITYWARLIST]=netrecv_nationcitywarlist_C;
	[CMDS_NATIONQUESTLIST]=netrecv_nationquestlist_C;
	[CMDS_NATIONMISSIONLIST]=netrecv_nationmissionlist_C;
	[CMDS_NATIONRANKLIST]=netrecv_nationranklist_C;
	[CMDS_NATIONOFFICIALLIST]=netrecv_nationofficiallist_C;
	[CMDS_NATIONCANDIDATELIST]=netrecv_nationcandidatelist_C;
	[CMDS_WORLDDATAOPEN]=netrecv_worlddataopen_C;
	[CMDS_NATIONREPLACELIST]=netrecv_nationreplacelist_C;
	[CMDS_NATIONLOGLIST]=netrecv_nationloglist_C;
	[CMDS_NATIONHEROLIST]=netrecv_nationherolist_C;
	[CMDS_CITYNATIONHERO]=netrecv_citynationhero_C;
	[CMDS_CITYNATIONHEROLIST]=netrecv_citynationherolist_C;
	[CMDS_NATIONHEROATTR]=netrecv_nationheroattr_C;
	[CMDS_WISHINGSHOP]=netrecv_wishingshop_C;
	[CMDS_WISHINGCHANGE]=netrecv_wishingchange_C;
	[CMDS_WISHINGPACK]=netrecv_wishingpack_C;
	[CMDS_TIANCEQUEST]=netrecv_tiancequest_C;
	[CMDS_STUDENTLIST]=netrecv_studentlist_C;
	[CMDS_TEACHERSHOPLIST]=netrecv_teachershoplist_C;
	[CMDS_ACTIVITY03LIST]=netrecv_activity03list_C;
	[CMDS_ACTIVITY05LIST]=netrecv_activity05list_C;
	[CMDS_ACTIVITY01LIST]=netrecv_activity01list_C;
	[CMDS_ACTIVITY04LIST]=netrecv_activity04list_C;
	[CMDS_GIRL]=netrecv_girl_C;
	[CMDS_GIRLLIST]=netrecv_girllist_C;
	[CMDS_GIRLGET]=netrecv_girlget_C;
	[CMDS_GIRLLOVE]=netrecv_girllove_C;
}

function in_proc_command_C( cmd, buffer )
	if Proc_Command[cmd] ~= nil then
		Proc_Command[cmd]( buffer );
		return 1;
	end
	return 0;
end
