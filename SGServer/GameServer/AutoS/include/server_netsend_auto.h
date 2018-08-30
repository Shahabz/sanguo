#ifndef __NETSEND_AUTO_H0
#define __NETSEND_AUTO_H0

#include "server_structsend_auto.h"
int netsend_login_S( int actor_index, char send_type, SLK_NetS_Login *pValue );
int netsend_list_S( int actor_index, char send_type, SLK_NetS_List *pValue );
int netsend_create_S( int actor_index, char send_type, SLK_NetS_Create *pValue );
int netsend_enterinfo_S( int actor_index, char send_type, SLK_NetS_EnterInfo *pValue );
int netsend_delete_S( int actor_index, char send_type, SLK_NetS_Delete *pValue );
int netsend_heart_S( int actor_index, char send_type, SLK_NetS_Heart *pValue );
int netsend_notify_S( int actor_index, char send_type, SLK_NetS_Notify *pValue );
int netsend_actorinfo_S( int actor_index, char send_type, SLK_NetS_ActorInfo *pValue );
int netsend_itemuse_S( int actor_index, char send_type, SLK_NetS_ItemUse *pValue );
int netsend_itemput_S( int actor_index, char send_type, SLK_NetS_ItemPut *pValue );
int netsend_itemsettle_S( int actor_index, char send_type, SLK_NetS_ItemSettle *pValue );
int netsend_lostitem_S( int actor_index, char send_type, SLK_NetS_LostItem *pValue );
int netsend_getitem_S( int actor_index, char send_type, SLK_NetS_GetItem *pValue );
int netsend_itemlist_S( int actor_index, char send_type, SLK_NetS_ItemList *pValue );
int netsend_iteminfo_S( int actor_index, char send_type, SLK_NetS_ItemInfo *pValue );
int netsend_addmapunit_S( char *databuf, int size, SLK_NetS_AddMapUnit *pValue );
int netsend_delmapunit_S( char *databuf, int size, SLK_NetS_DelMapUnit *pValue );
int netsend_updatemapunit_S( char *databuf, int size, SLK_NetS_UpdateMapUnit *pValue );
int netsend_mapunitsingle_S( int actor_index, char send_type, SLK_NetS_AddMapUnit *pValue );
int netsend_worldmapinfo_S( int actor_index, char send_type, SLK_NetS_WorldMapInfo *pValue );
int netsend_mapunitcorrdinate_S( char *databuf, int size, SLK_NetS_MapUnitCorrdinate *pValue );
int netsend_addmarchroute_S( char *databuf, int size, SLK_NetS_AddMarchRoute *pValue );
int netsend_delmarchroute_S( char *databuf, int size, SLK_NetS_DelMarchRoute *pValue );
int netsend_awardinfolist_S( int actor_index, char send_type, SLK_NetS_AwardInfoList *pValue );
int netsend_experience_S( int actor_index, char send_type, SLK_NetS_Experience *pValue );
int netsend_body_S( int actor_index, char send_type, SLK_NetS_Body *pValue );
int netsend_buildinglist_S( int actor_index, char send_type, SLK_NetS_BuildingList *pValue );
int netsend_changelevy_S( int actor_index, char send_type, SLK_NetS_Levy *pValue );
int netsend_changesilver_S( int actor_index, char send_type, SLK_NetS_Silver *pValue );
int netsend_changewood_S( int actor_index, char send_type, SLK_NetS_Wood *pValue );
int netsend_changefood_S( int actor_index, char send_type, SLK_NetS_Food *pValue );
int netsend_changeiron_S( int actor_index, char send_type, SLK_NetS_Iron *pValue );
int netsend_changepeople_S( int actor_index, char send_type, SLK_NetS_People *pValue );
int netsend_changeprestige_S( int actor_index, char send_type, SLK_NetS_Prestige *pValue );
int netsend_changefriendship_S( int actor_index, char send_type, SLK_NetS_Friendship *pValue );
int netsend_changevip_S( int actor_index, char send_type, SLK_NetS_Vip *pValue );
int netsend_changtoken_S( int actor_index, char send_type, SLK_NetS_Token *pValue );
int netsend_equip_S( int actor_index, char send_type, SLK_NetS_Equip *pValue );
int netsend_equiplist_S( int actor_index, char send_type, SLK_NetS_EquipList *pValue );
int netsend_equipget_S( int actor_index, char send_type, SLK_NetS_EquipGet *pValue );
int netsend_equiplost_S( int actor_index, char send_type, SLK_NetS_EquipLost *pValue );
int netsend_hero_S( int actor_index, char send_type, SLK_NetS_Hero *pValue );
int netsend_herolist_S( int actor_index, char send_type, SLK_NetS_HeroList *pValue );
int netsend_heroexp_S( int actor_index, char send_type, SLK_NetS_HeroExp *pValue );
int netsend_herosoldiers_S( int actor_index, char send_type, SLK_NetS_HeroSoldiers *pValue );
int netsend_herostate_S( int actor_index, char send_type, SLK_NetS_HeroState *pValue );
int netsend_heroreplace_S( int actor_index, char send_type, SLK_NetS_HeroReplace *pValue );
int netsend_buildingupgradeinfo_S( int actor_index, char send_type, SLK_NetS_BuildingUpgradeInfo *pValue );
int netsend_building_S( int actor_index, char send_type, SLK_NetS_Building *pValue );
int netsend_buildingbarracks_S( int actor_index, char send_type, SLK_NetS_BuildingBarracks *pValue );
int netsend_buildingres_S( int actor_index, char send_type, SLK_NetS_BuildingRes *pValue );
int netsend_worker_S( int actor_index, char send_type, SLK_NetS_Worker *pValue );
int netsend_heroget_S( int actor_index, char send_type, SLK_NetS_HeroGet *pValue );
int netsend_buildingget_S( int actor_index, char send_type, SLK_NetS_BuildingGet *pValue );
int netsend_buildingbarracksget_S( int actor_index, char send_type, SLK_NetS_BuildingBarracksGet *pValue );
int netsend_buildingresget_S( int actor_index, char send_type, SLK_NetS_BuildingResGet *pValue );
int netsend_soldiers_S( int actor_index, char send_type, SLK_NetS_Soldiers *pValue );
int netsend_traininfo_S( int actor_index, char send_type, SLK_NetS_TrainInfo *pValue );
int netsend_quest_S( int actor_index, char send_type, SLK_NetS_Quest *pValue );
int netsend_questlist_S( int actor_index, char send_type, SLK_NetS_QuestList *pValue );
int netsend_questaward_S( int actor_index, char send_type, SLK_NetS_QuestAward *pValue );
int netsend_function_S( int actor_index, char send_type, SLK_NetS_Function *pValue );
int netsend_cityguard_S( int actor_index, char send_type, SLK_NetS_CityGuard *pValue );
int netsend_cityguardlist_S( int actor_index, char send_type, SLK_NetS_CityGuardList *pValue );
int netsend_cityguardsec_S( int actor_index, char send_type, SLK_NetS_CityGuardSec *pValue );
int netsend_buildingsmithy_S( int actor_index, char send_type, SLK_NetS_BuildingSmithy *pValue );
int netsend_changename_S( int actor_index, char send_type, SLK_NetS_ChangeName *pValue );
int netsend_buildingaction_S( int actor_index, char send_type, SLK_NetS_BuildingAction *pValue );
int netsend_levyinfo_S( int actor_index, char send_type, SLK_NetS_LevyInfo *pValue );
int netsend_chat_S( int actor_index, char send_type, SLK_NetS_Chat *pValue );
int netsend_chatlist_S( int actor_index, char send_type, SLK_NetS_ChatList *pValue );
int netsend_systalkid_S( int actor_index, char send_type, SLK_NetS_Systalkid *pValue );
int netsend_systalk_S( int actor_index, char send_type, SLK_NetS_Systalk *pValue );
int netsend_battlepower_S( int actor_index, char send_type, SLK_NetS_Battlepower *pValue );
int netsend_techchange_S( int actor_index, char send_type, SLK_NetS_TechChange *pValue );
int netsend_cityeventlist_S( int actor_index, char send_type, SLK_NetS_EventList *pValue );
int netsend_officialhirechange_S( int actor_index, char send_type, SLK_NetS_OfficialHireChange *pValue );
int netsend_cityprotect_S( int actor_index, char send_type, SLK_NetS_CityProtect *pValue );
int netsend_heroequip_S( int actor_index, char send_type, SLK_NetS_HeroEquip *pValue );
int netsend_herowash_S( int actor_index, char send_type, SLK_NetS_HeroWash *pValue );
int netsend_herocolorup_S( int actor_index, char send_type, SLK_NetS_HeroColorup *pValue );
int netsend_cityattr_S( int actor_index, char send_type, SLK_NetS_CityAttr *pValue );
int netsend_equipwash_S( int actor_index, char send_type, SLK_NetS_EquipWash *pValue );
int netsend_materiallist_S( int actor_index, char send_type, SLK_NetS_MaterialList *pValue );
int netsend_materialwilllist_S( int actor_index, char send_type, SLK_NetS_MaterialWillList *pValue );
int netsend_storylist_S( int actor_index, char send_type, SLK_NetS_StoryList *pValue );
int netsend_storyrank_S( int actor_index, char send_type, SLK_NetS_StoryRank *pValue );
int netsend_storystate_S( int actor_index, char send_type, SLK_NetS_StoryState *pValue );
int netsend_storyranknum_S( int actor_index, char send_type, SLK_NetS_StoryRanknum *pValue );
int netsend_storyranktime_S( int actor_index, char send_type, SLK_NetS_StoryRanktime *pValue );
int netsend_mapzonechange_S( int actor_index, char send_type, SLK_NetS_MapZoneChange *pValue );
int netsend_mapzoneunitlist_S( int actor_index, char send_type, SLK_NetS_MapZoneUnitList *pValue );
int netsend_mapzoneunit_S( int actor_index, char send_type, SLK_NetS_ZoneUnit *pValue );
int netsend_armyspeedupdate_S( int actor_index, char send_type, SLK_NetS_ArmySpeedUpdate *pValue );
int netsend_battlelist_S( int actor_index, char send_type, SLK_NetS_BattleList *pValue );
int netsend_battleinfo_S( int actor_index, char send_type, SLK_NetS_BattleInfo *pValue );
int netsend_mapresinfo_S( int actor_index, char send_type, SLK_NetS_MapResInfo *pValue );
int netsend_weatherchange_S( int actor_index, char send_type, SLK_NetS_WeatherChange *pValue );
int netsend_mail_S( int actor_index, char send_type, SLK_NetS_Mail *pValue );
int netsend_mailopresult_S( int actor_index, char send_type, SLK_NetS_MailOpResult *pValue );
int netsend_mailfight_S( int actor_index, char send_type, SLK_NetS_MailFight *pValue );
int netsend_mailview_S( int actor_index, char send_type, SLK_NetS_MailView *pValue );
int netsend_cityhelp_S( int actor_index, char send_type, SLK_NetS_CItyHelp *pValue );
int netsend_cityhelplist_S( int actor_index, char send_type, SLK_NetS_CItyHelpList *pValue );
int netsend_mapcityhelplist_S( int actor_index, char send_type, SLK_NetS_MapCItyHelpList *pValue );
int netsend_citystate_S( int actor_index, char send_type, SLK_NetS_CityState *pValue );
int netsend_cityarmygroup_S( int actor_index, char send_type, SLK_NetS_CityArmyGroup *pValue );
int netsend_cityarmygrouplist_S( int actor_index, char send_type, SLK_NetS_CityArmyGroupList *pValue );
int netsend_maptowninfo_S( int actor_index, char send_type, SLK_NetS_MapTownInfo *pValue );
int netsend_townarmygroup_S( int actor_index, char send_type, SLK_NetS_TownArmyGroup *pValue );
int netsend_townarmygrouplist_S( int actor_index, char send_type, SLK_NetS_TownArmyGroupList *pValue );
int netsend_systalkjson_S( int actor_index, char send_type, SLK_NetS_SystalkJson *pValue );
int netsend_rollmsgjson_S( int actor_index, char send_type, SLK_NetS_RollMsgJson *pValue );
int netsend_rollmsg_S( int actor_index, char send_type, SLK_NetS_RollMsg *pValue );
int netsend_townowneraskList_S( int actor_index, char send_type, SLK_NetS_TownOwnerAskList *pValue );
int netsend_townfightlist_S( int actor_index, char send_type, SLK_NetS_TownFightList *pValue );
int netsend_dialogupdate_S( int actor_index, char send_type, SLK_NetS_DialogUpdate *pValue );
int netsend_maptownexinfo_S( int actor_index, char send_type, SLK_NetS_MapTownExInfo *pValue );
int netsend_mapzonetown_S( int actor_index, char send_type, SLK_NetS_MapZoneTown *pValue );
int netsend_mapzonetownlist_S( int actor_index, char send_type, SLK_NetS_MapZoneTownList *pValue );
int netsend_mapcentertown_S( int actor_index, char send_type, SLK_NetS_MapCenterTown *pValue );
int netsend_mapcentertownlist_S( int actor_index, char send_type, SLK_NetS_MapCenterTownList *pValue );
int netsend_citywarinfo_S( int actor_index, char send_type, SLK_NetS_CityWarInfo *pValue );
int netsend_citywarlist_S( int actor_index, char send_type, SLK_NetS_CityWarList *pValue );
int netsend_citywardel_S( int actor_index, char send_type, SLK_NetS_CityWarDel *pValue );
int netsend_worldquest_S( int actor_index, char send_type, SLK_NetS_WorldQuest *pValue );
int netsend_worldboss_S( int actor_index, char send_type, SLK_NetS_WorldBoss *pValue );
int netsend_lostrebuild_S( int actor_index, char send_type, SLK_NetS_LostRebuild *pValue );
int netsend_kingwartown_S( int actor_index, char send_type, SLK_NetS_KingwarTown *pValue );
int netsend_kingwartownlist_S( int actor_index, char send_type, SLK_NetS_KingwarTownList *pValue );
int netsend_kingwaractivity_S( int actor_index, char send_type, SLK_NetS_KingWarActivity *pValue );
int netsend_kingwarnotify_S( int actor_index, char send_type, SLK_NetS_KingWarNotify *pValue );
int netsend_kingwarnotifylist_S( int actor_index, char send_type, SLK_NetS_KingWarNotifyList *pValue );
int netsend_kingwarranklist_S( int actor_index, char send_type, SLK_NetS_KingWarRankList *pValue );
int netsend_addzoneunit_S( int actor_index, char send_type, SLK_NetS_ZoneUnit *pValue );
int netsend_mapunitaction_S( int actor_index, char send_type, SLK_NetS_MapUnitAction *pValue );
int netsend_mapposaction_S( int actor_index, char send_type, SLK_NetS_MapPosAction *pValue );
int netsend_kingwarpk_S( int actor_index, char send_type, SLK_NetS_KingWarPK *pValue );
int netsend_kingwarpoint_S( int actor_index, char send_type, SLK_NetS_KingWarPoint *pValue );
int netsend_treasureactivity_S( int actor_index, char send_type, SLK_NetS_TreasureActivity *pValue );
int netsend_treasurehas_S( int actor_index, char send_type, SLK_NetS_TreasureHas *pValue );
int netsend_gotoasyn_S( int actor_index, char send_type, SLK_NetS_GotoAsyn *pValue );
int netsend_treasureactorlist_S( int actor_index, char send_type, SLK_NetS_TreasureActorList *pValue );
int netsend_changeautobuild_S( int actor_index, char send_type, SLK_NetS_ChangeAutoBuild *pValue );
int netsend_herovisitinfo_S( int actor_index, char send_type, SLK_NetS_HeroVisitInfo *pValue );
int netsend_nationbase_S( int actor_index, char send_type, SLK_NetS_NationBase *pValue );
int netsend_herovisitaward_S( int actor_index, char send_type, SLK_NetS_HeroVisitAward *pValue );
int netsend_fightplay_S( int actor_index, char send_type, SLK_NetS_FightPlay *pValue );
int netsend_storysweepresult_S( int actor_index, char send_type, SLK_NetS_StorySweepResult *pValue );
int netsend_questtalk_S( int actor_index, char send_type, SLK_NetS_QuestTalk *pValue );
int netsend_ranklist_S( int actor_index, char send_type, SLK_NetS_RankList *pValue );
int netsend_friendlist_S( int actor_index, char send_type, SLK_NetS_FriendList *pValue );
int netsend_actorsearch_S( int actor_index, char send_type, SLK_NetS_ActorSearch *pValue );
int netsend_blackinfo_S( int actor_index, char send_type, SLK_NetS_BlackInfo *pValue );
int netsend_blacklist_S( int actor_index, char send_type, SLK_NetS_BlackList *pValue );
int netsend_blacklistid_S( int actor_index, char send_type, SLK_NetS_BlackListID *pValue );
int netsend_nationequip_S( int actor_index, char send_type, SLK_NetS_NationEquip *pValue );
int netsend_shoplist_S( int actor_index, char send_type, SLK_NetS_ShopList *pValue );
int netsend_paystore_S( int actor_index, char send_type, SLK_NetS_PayStore *pValue );
int netsend_goodsinfo_S( int actor_index, char send_type, SLK_NetS_GoodsInfo *pValue );
int netsend_paystoreactivity_S( int actor_index, char send_type, SLK_NetS_PayStoreActivity *pValue );
int netsend_payorder_S( int actor_index, char send_type, SLK_NetS_PayOrder *pValue );
int netsend_vipshop_S( int actor_index, char send_type, SLK_NetS_VipShop *pValue );
int netsend_vipbag_S( int actor_index, char send_type, SLK_NetS_VipBag *pValue );
int netsend_buffchange_S( int actor_index, char send_type, SLK_NetS_BuffChange *pValue );
int netsend_changeautoguard_S( int actor_index, char send_type, SLK_NetS_ChangeAutoGuard *pValue );
int netsend_activitylist_S( int actor_index, char send_type, SLK_NetS_ActivityList *pValue );
int netsend_nationequiplist_S( int actor_index, char send_type, SLK_NetS_NationEquipList *pValue );
int netsend_nationinfo_S( int actor_index, char send_type, SLK_NetS_NationInfo *pValue );
int netsend_nationtown_S( int actor_index, char send_type, SLK_NetS_NationTown *pValue );
int netsend_nationtownlist_S( int actor_index, char send_type, SLK_NetS_NationTownList *pValue );
int netsend_nationwarlist_S( int actor_index, char send_type, SLK_NetS_NationWarList *pValue );
int netsend_nationcitywarlist_S( int actor_index, char send_type, SLK_NetS_NationCityWarList *pValue );
int netsend_nationquestlist_S( int actor_index, char send_type, SLK_NetS_NationQuestList *pValue );
int netsend_nationmissionlist_S( int actor_index, char send_type, SLK_NetS_NationMissionList *pValue );
int netsend_nationranklist_S( int actor_index, char send_type, SLK_NetS_NationRankList *pValue );
int netsend_nationofficiallist_S( int actor_index, char send_type, SLK_NetS_NationOfficialList *pValue );
int netsend_nationcandidatelist_S( int actor_index, char send_type, SLK_NetS_NationCandidateList *pValue );
int netsend_worlddataopen_S( int actor_index, char send_type, SLK_NetS_WorldDataOpen *pValue );
int netsend_nationreplacelist_S( int actor_index, char send_type, SLK_NetS_NationReplaceList *pValue );
int netsend_nationloglist_S( int actor_index, char send_type, SLK_NetS_NationLogList *pValue );
int netsend_nationherolist_S( int actor_index, char send_type, SLK_NetS_NationHeroList *pValue );
int netsend_citynationhero_S( int actor_index, char send_type, SLK_NetS_CityNationHero *pValue );
int netsend_citynationherolist_S( int actor_index, char send_type, SLK_NetS_CityNationHeroList *pValue );
int netsend_nationheroattr_S( int actor_index, char send_type, SLK_NetS_NationHeroAttr *pValue );
int netsend_wishingshop_S( int actor_index, char send_type, SLK_NetS_WishingShop *pValue );
int netsend_wishingchange_S( int actor_index, char send_type, SLK_NetS_WishingChange *pValue );
int netsend_wishingpack_S( int actor_index, char send_type, SLK_NetS_WishingPack *pValue );
int netsend_tiancequest_S( int actor_index, char send_type, SLK_NetS_TianceQuest *pValue );
int netsend_studentlist_S( int actor_index, char send_type, SLK_NetS_StudentList *pValue );
int netsend_teachershoplist_S( int actor_index, char send_type, SLK_NetS_TeacherShopList *pValue );
int netsend_activity03list_S( int actor_index, char send_type, SLK_NetS_Activity03List *pValue );
int netsend_activity05list_S( int actor_index, char send_type, SLK_NetS_Activity05List *pValue );
int netsend_activity01list_S( int actor_index, char send_type, SLK_NetS_Activity01List *pValue );
int netsend_activity04list_S( int actor_index, char send_type, SLK_NetS_Activity04List *pValue );
int netsend_girl_S( int actor_index, char send_type, SLK_NetS_Girl *pValue );
int netsend_girllist_S( int actor_index, char send_type, SLK_NetS_GirlList *pValue );
int netsend_girlget_S( int actor_index, char send_type, SLK_NetS_GirlGet *pValue );
int netsend_girllove_S( int actor_index, char send_type, SLK_NetS_GirlLove *pValue );
int netsend_girlloveresult_S( int actor_index, char send_type, SLK_NetS_GirlLoveResult *pValue );
int netsend_fsinfo_S( int actor_index, char send_type, SLK_NetS_FsInfo *pValue );
int netsend_fsvisitresult_S( int actor_index, char send_type, SLK_NetS_FsVisitResult *pValue );
int netsend_fspalace_S( int actor_index, char send_type, SLK_NetS_FsPalace *pValue );
int netsend_fspalaceresult_S( int actor_index, char send_type, SLK_NetS_FsPalaceResult *pValue );
int netsend_girlshop_S( int actor_index, char send_type, SLK_NetS_GirlShop *pValue );
int netsend_zonemaster_S( int actor_index, char send_type, SLK_NetS_ZoneMaster *pValue );
int netsend_zonemasterlist_S( int actor_index, char send_type, SLK_NetS_ZoneMasterList *pValue );
int netsend_delzoneunit_S( int actor_index, char send_type, SLK_NetS_DelZoneUnit *pValue );
int netsend_fighthelper_S( int actor_index, char send_type, SLK_NetS_FightHelper *pValue );
int netsend_fighthelperlist_S( int actor_index, char send_type, SLK_NetS_FightHelperList *pValue );
int netsend_redinfo_S( int actor_index, char send_type, SLK_NetS_RedInfo *pValue );
int netsend_everydayevent_S( int actor_index, char send_type, SLK_NetS_EverydayEvent *pValue );
int netsend_tokensale_S( int actor_index, char send_type, SLK_NetS_TokenSale *pValue );
int netsend_tokenret_S( int actor_index, char send_type, SLK_NetS_TokenRet *pValue );
int netsend_activity33_S( int actor_index, char send_type, SLK_NetS_Activity33 *pValue );
int netsend_NetS_EDayQuestList_S( int actor_index, char send_type, SLK_NetS_EDayQuestList *pValue );
int netsend_NetS_EDayShopList_S( int actor_index, char send_type, SLK_NetS_EDayShopList *pValue );
int netsend_NetS_EDayPoint_S( int actor_index, char send_type, SLK_NetS_EDayPoint *pValue );

int netsend_invitecodeed_S( int actor_index, char send_type, SLK_NetU_InviteCodeed *pValue );
int netsend_userawarded_S( int actor_index, char send_type, SLK_NetU_UserAwarded *pValue );
int netsend_gmlocalcmd_S( int actor_index, char send_type, SLK_NetU_Gmlocalcmd *pValue );
int netsend_logined_S( int actor_index, char send_type, SLK_NetU_Logined *pValue );
int netsend_timer_S( int actor_index, char send_type, char *pValue );

#endif
