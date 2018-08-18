-- 界面
local m_Dlg = nil;
local m_uiTop_City = nil; --UnityEngine.GameObject
local m_uiTop_Map = nil; --UnityEngine.GameObject
local m_uiFunctionPanel = nil; --UnityEngine.GameObject
local m_uiQuestList = nil; --UnityEngine.GameObject
local m_uiMorePanel = nil; --UnityEngine.GameObject
local m_uiHead = nil; --UnityEngine.GameObject
local m_uiLevel = nil; --UnityEngine.GameObject
local m_uiExpProgress = nil; --UnityEngine.GameObject
local m_uiBodyProgress = nil; --UnityEngine.GameObject
local m_uiNation = nil; --UnityEngine.GameObject
local m_uiSilver = nil; --UnityEngine.GameObject
local m_uiSilverNum = nil; --UnityEngine.GameObject
local m_uiWood = nil; --UnityEngine.GameObject
local m_uiWoodNum = nil; --UnityEngine.GameObject
local m_uiIron = nil; --UnityEngine.GameObject
local m_uiIronNum = nil; --UnityEngine.GameObject
local m_uiBody = nil; --UnityEngine.GameObject
local m_uiBodyNum = nil; --UnityEngine.GameObject
local m_uiInfantry = nil; --UnityEngine.GameObject
local m_uiInfantryNum = nil; --UnityEngine.GameObject
local m_uiCavalry = nil; --UnityEngine.GameObject
local m_uiCavalryNum = nil; --UnityEngine.GameObject
local m_uiArcher = nil; --UnityEngine.GameObject
local m_uiArcherNum = nil; --UnityEngine.GameObject
local m_uiTokenNum = nil; --UnityEngine.GameObject
local m_uiVipLevel = nil; --UnityEngine.GameObject
local m_uiWorker = nil; --UnityEngine.GameObject
local m_uiWorkerEx = nil; --UnityEngine.GameObject
local m_uiFood = nil; --UnityEngine.GameObject
local m_uiFoodNum = nil; --UnityEngine.GameObject
local m_uiAutoBuild = nil; --UnityEngine.GameObject
local m_uiAutoGuard = nil; --UnityEngine.GameObject
local m_uiQuestText = nil; --UnityEngine.GameObject
local ButtonTable = {}
ButtonTable.m_uiButtonHero = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonQuest = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonNation = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonWorld = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonCity = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonStory = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonMail = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonMore = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonBag = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonFriend = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonNationEquip = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonRank = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonNotice = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonSetting = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonGM = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonRelogin = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonRestart = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonGirl = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonEquip = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonUserCenter = nil; --UnityEngine.GameObject
ButtonTable.m_uiButtonInviteCode = nil; --UnityEngine.GameObject

local m_uiFunctionPanel = nil; --UnityEngine.GameObject
local m_uiMorePanel = nil; --UnityEngine.GameObject
local m_bMorePanel = false;
local m_uiButtonBack = {}; --UnityEngine.GameObject
local m_hasButton = {};
local m_uiChat = nil; --UnityEngine.GameObject
local m_uiChatText = nil; --UnityEngine.GameObject
local m_uiQuest = nil; --UnityEngine.GameObject
local m_uiTopLeft = nil; --UnityEngine.GameObject
local m_uiZoneName = nil; --UnityEngine.GameObject
local m_uiWeatherDay = nil; --UnityEngine.GameObject
local m_uiWeatherIcon = nil; --UnityEngine.GameObject
local m_uiWeatherAbility = nil; --UnityEngine.GameObject

local m_uiWarTable ={}
m_uiWarTable.m_uiWar = nil; --UnityEngine.GameObject
m_uiWarTable.m_uiWarScroll = nil;
m_uiWarTable.m_uiWarContent = nil
m_uiWarTable.m_uiUIP_WarText = nil; --UnityEngine.GameObject
m_uiWarTable.m_uiWarWarning = nil; --UnityEngine.GameObject
local m_uiCutScenes = nil; --UnityEngine.GameObject
local m_uiQuestTitleText = nil; --UnityEngine.GameObject
local m_uiQuestEffect = nil; --UnityEngine.GameObject

m_uiWarTable.m_cache = {}
m_uiWarTable.m_cacheObj = {}

local m_uiBuildingShape = {nil,nil}
local m_uiNormalShape = {nil,nil}
local m_uiBuildingTimer = {nil,nil}
local m_uiBuildingName = {nil,nil}

local m_uiBuildingShapeUITweenScale = {nil,nil}
local m_uiNormalShapeUITweenScale = {nil,nil}
local m_uiBuildingTimerUITweenScale = {nil,nil}
local m_uiBuildingNameUITweenScale = {nil,nil}

local m_uiBuildingShapeUITweenScale1 = {nil,nil}
local m_uiNormalShapeUITweenScale1 = {nil,nil}
local m_uiBuildingTimerUITweenScale1 = {nil,nil}
local m_uiBuildingNameUITweenScale1 = {nil,nil}

local m_ObjectPool = nil;
local m_finishTask = 0;

-- 打开界面
function MainDlgOpen()
	m_Dlg = eye.uiManager:Open( "MainDlg" );
	m_Dlg.transform:SetSiblingIndex(0);
end

-- 隐藏界面
function MainDlgClose()
	if IsGuiding() then
		HideGuideFinger();
	end
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "MainDlg" );
end

-- 删除界面
function MainDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MainDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		if nControlID ~= 8 and m_bMorePanel == true then
			SetFalse( ButtonTable.m_uiButtonMore.transform:Find("Select") )
			SetFalse(m_uiMorePanel)
			m_bMorePanel = false;
		end
        if nControlID == -1 then
            --MainDlgClose();
		
		-- 英雄
		elseif nControlID == 1 then
			HeroDlgShow();
		
		-- 任务	
		elseif nControlID == 2 then
			QuestDlgShow()
			
		-- 国家
		elseif nControlID == 3 then
			NationDlgShow()
			
		-- 城池，世界
		elseif nControlID == 4 or nControlID == 5 then
			if GameManager.currentScence == "city" then
				WorldMap.GotoWorldMap(-1, -1)
			else
				WorldMap.ReturnCity()
			end
		
		-- 副本
		elseif nControlID == 6 then
			StoryDlgShow()
		
		-- 邮件
		elseif nControlID == 7 then
			MailDlgShow()
		
		-- 更多	
		elseif nControlID == 8 then
			if m_bMorePanel == true then
				SetFalse( ButtonTable.m_uiButtonMore.transform:Find("Select") )
				SetFalse(m_uiMorePanel)
				m_bMorePanel = false;
			else
				SetTrue( ButtonTable.m_uiButtonMore.transform:Find("Select") )
				SetTrue(m_uiMorePanel)
				m_bMorePanel = true;
			end
		
		-- 背包
		elseif nControlID == 11 then
			BagDlgShow();

		-- 好友
		elseif nControlID == 12 then
			FriendDlgShow()
		-- 国器
		elseif nControlID == 13 then
			NationEquipDlgShow()
		-- 排行榜
		elseif nControlID == 14 then
			RankListDlgShow()
		-- 系统公告
		elseif nControlID == 15 then
			NoticeDlgShow()
		
		-- 设置
		elseif nControlID == 16 then
			m_Setting_state = 0
			SettingDlgOptionShow()
			
		-- 联系客服
		elseif nControlID == 17 then
			SDK.gmbug()
			
		-- 返回登录
		elseif nControlID == 18 then
			MsgBox( T( 518 ),function()
				GameManager.Restart();
				GameManager.Logout( 1 );
			end )
		
		elseif nControlID == 19 then
			MsgBox( "重新创建角色吗？",function()
				system_askinfo( ASKINFO_ACTOR, "", 1 );
				Invoke( function() 
					GameManager.Restart();
					GameManager.Logout( 1 );
				end, 0.3 );
			end )
		
		-- 女将	
		elseif nControlID == 20 then
			GirlDlgShow()
		
		-- 装备
		elseif nControlID == 21 then
			EquipDlgShow()
		
		-- 用户中心
		elseif nControlID == 22 then
			SDK.userCenter()
		
		-- 送福利邀请码
		elseif nControlID == 23 then
			InviteCodeDlgShow()
					
		-- 聊天
		elseif nControlID == 30 then
			ChatDlgShow();
		
		-- 主线任务导航
		elseif nControlID == 31 then
			if m_finishTask == 0 then
				QuestGoto( 1 );
			else
				system_askinfo( ASKINFO_QUEST, "", 3, CacheQuest.m_list[m_finishTask].m_questid );
				m_finishTask = 0;
			end
			
		-- 打开支线任务
		elseif nControlID == 32 then
			SetTrue( m_uiQuestList )
			SetFalse( m_uiQuest.transform:Find("Button") )
		
		-- 收起支线任务
		elseif nControlID == 33 then
			SetFalse( m_uiQuestList )
			if CacheQuest.m_count > 1 then
				SetTrue( m_uiQuest.transform:Find("Button") )
			end
		
		-- 战争警告
		elseif nControlID == 34 then
			if GameManager.currentScence == "city" then
				WorldMap.GotoWorldMap(-1, -1)
			end
			
		-- 点击支线任务导航
		elseif nControlID >= 40 and nControlID < 50 then
			QuestGoto( nControlID-40+2 );
					
		-- 点击建造队列
		elseif nControlID == 50 then
			City.GoToWorker()
		
		-- 点击建造队列商用
		elseif nControlID == 51 then
			City.GoToWorkerEx()
		
		-- 自动建造
		elseif nControlID == 52 then
			if GetPlayer().m_autobuildopen == 1 then
				GetPlayer().m_autobuildopen = 0
			else
				GetPlayer().m_autobuildopen = 1
			end
			MainDlgSetAutoBuild( 0 );
			system_askinfo( ASKINFO_AUTOBUILD, "", 0 );
		
		-- 城防补充
		elseif nControlID == 53 then
			if GetPlayer().m_autoguardopen == 1 then
				GetPlayer().m_autoguardopen = 0
			else
				GetPlayer().m_autoguardopen = 1
			end
			MainDlgSetAutoGuard( 0 );
			system_askinfo( ASKINFO_AUTOBUILD, "", 2 );
				
		-- VIP充值充值	
		elseif nControlID == 60 then
			VipDlgShow()
		
		-- 活动
		elseif nControlID == 61 then
			ActivityDlgShow()
			
		-- 特价礼包
		elseif nControlID == 62 then
			ShopDlgShowByType(1)
		
		-- 首充福利
		elseif nControlID == 63 then
			ActivityDlgShowByID( ACTIVITY_1 )
			
		-- 角色信息	
		elseif nControlID == 100 then
			PlayerDlgShow();
		
		-- 充值	
		elseif nControlID == 101 then
			PayDlgShow()
		
		-- 
		elseif nControlID == 102 then
		
		elseif nControlID == 104 then
		
			
		-- 点击资源
		elseif nControlID == 201 then
			MaterialGetDlgShow( 120 )
		elseif nControlID == 202 then
			MaterialGetDlgShow( 121 )
		elseif nControlID == 203 then
			MaterialGetDlgShow( 122 )
		elseif nControlID == 204 then
			MaterialGetDlgShow( 123 )
		-- 点击体力
		elseif nControlID == 205 then
			JumpBody()
		
		-- 区域地图
		elseif nControlID == 301 then
			MapZoneDlgShow();
			
		-- 天气
		elseif nControlID == 302 then
			if g_game_day <= 0 then
				pop(T(3016))
			end
        end
	elseif nType == UI_EVENT_TWEENFINISH then
		if nControlID == 1 then
			m_uiBuildingShape[1]:SetActive(false);
			m_uiNormalShape[1]:SetActive(true);
			m_uiNormalShapeUITweenScale[1]:Play(true);
			m_uiNormalShapeUITweenScale1[1]:Play(true);
		elseif nControlID == 2 then
			m_uiBuildingShape[1]:SetActive(true);
			m_uiNormalShape[1]:SetActive(false);
			m_uiBuildingShapeUITweenScale[1]:Play(true);
			m_uiBuildingShapeUITweenScale1[1]:Play(true);
		elseif nControlID == 3 then
			m_uiBuildingTimer[1]:SetActive(false);
			m_uiBuildingName[1]:SetActive(true);
			m_uiBuildingNameUITweenScale[1]:Play(true);
			m_uiBuildingNameUITweenScale1[1]:Play(true);
		elseif nControlID == 4 then
			m_uiBuildingTimer[1]:SetActive(true);
			m_uiBuildingName[1]:SetActive(false);
			m_uiBuildingTimerUITweenScale[1]:Play(true);
			m_uiBuildingTimerUITweenScale1[1]:Play(true);
		elseif nControlID == 11 then
			m_uiBuildingShape[2]:SetActive(false);
			m_uiNormalShape[2]:SetActive(true);
			m_uiNormalShapeUITweenScale[2]:Play(true);
			m_uiNormalShapeUITweenScale1[2]:Play(true);
		elseif nControlID == 12 then
			m_uiBuildingShape[2]:SetActive(true);
			m_uiNormalShape[2]:SetActive(false);
			m_uiBuildingShapeUITweenScale[2]:Play(true);
			m_uiBuildingShapeUITweenScale1[2]:Play(true);
		elseif nControlID == 13 then
			m_uiBuildingTimer[2]:SetActive(false);
			m_uiBuildingName[2]:SetActive(true);
			m_uiBuildingNameUITweenScale[2]:Play(true);
			m_uiBuildingNameUITweenScale1[2]:Play(true);
		elseif nControlID == 14 then
			m_uiBuildingTimer[2]:SetActive(true);
			m_uiBuildingName[2]:SetActive(false);
			m_uiBuildingTimerUITweenScale[2]:Play(true);
			m_uiBuildingTimerUITweenScale1[2]:Play(true);
		
		elseif nControlID == 20 then
			MainDlgStopCutScenes()
		end
		
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 101 then
			GetPlayer().m_worker_kind = 0
			MainDlgWorkerStop( 1 );
			MainDlgSetWorker()
		elseif nControlID == 102 then
			MainDlgWorkerStop( 2 )
			GetPlayer().m_worker_kind_ex = 0
			MainDlgSetWorker()
		end
	elseif nType == UI_EVENT_TIMECOUNTCHANGED then
		if nControlID == 101 then
			GetPlayer().m_worker_sec = value;
		elseif nControlID == 102 then
			GetPlayer().m_worker_sec_ex = value;
		end
	end
end

-- 载入时调用
function MainDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTop_City = objs[0];
	m_uiTop_Map = objs[1];
	m_uiFunctionPanel = objs[2];
	m_uiQuestList = objs[3];
	m_uiMorePanel = objs[4];
	m_uiHead = objs[5];
	m_uiLevel = objs[6];
	m_uiExpProgress = objs[7];
	m_uiBodyProgress = objs[8];
	m_uiNation = objs[9];
	m_uiSilver = objs[10];
	m_uiSilverNum = objs[11];
	m_uiWood = objs[12];
	m_uiWoodNum = objs[13];
	m_uiIron = objs[14];
	m_uiIronNum = objs[15];
	m_uiBody = objs[16];
	m_uiBodyNum = objs[17];
	m_uiInfantry = objs[18];
	m_uiInfantryNum = objs[19];
	m_uiCavalry = objs[20];
	m_uiCavalryNum = objs[21];
	m_uiArcher = objs[22];
	m_uiArcherNum = objs[23];
	m_uiTokenNum = objs[24];
	m_uiVipLevel = objs[25];
	m_uiWorker = objs[26];
	m_uiWorkerEx = objs[27];
	m_uiFood = objs[28];
	m_uiFoodNum = objs[29];
	m_uiAutoBuild = objs[30];
	m_uiQuestText = objs[31];
	ButtonTable.m_uiButtonHero = objs[32];
	ButtonTable.m_uiButtonQuest = objs[33];
	ButtonTable.m_uiButtonNation = objs[34];
	ButtonTable.m_uiButtonWorld = objs[35];
	ButtonTable.m_uiButtonCity = objs[36];
	ButtonTable.m_uiButtonStory = objs[37];
	ButtonTable.m_uiButtonMail = objs[38];
	ButtonTable.m_uiButtonMore = objs[39];
	ButtonTable.m_uiButtonBag = objs[40];
	ButtonTable.m_uiButtonFriend = objs[41];
	ButtonTable.m_uiButtonNationEquip = objs[42];
	ButtonTable.m_uiButtonRank = objs[43];
	ButtonTable.m_uiButtonNotice = objs[44];
	ButtonTable.m_uiButtonSetting = objs[45];
	ButtonTable.m_uiButtonGM = objs[46];
	ButtonTable.m_uiButtonRelogin = objs[47];
	ButtonTable.m_uiFunctionPanel = objs[48];
	ButtonTable.m_uiMorePanel = objs[49];
	m_uiButtonBack[1] = objs[50];
	m_uiButtonBack[2] = objs[51];
	m_uiButtonBack[3] = objs[52];
	m_uiButtonBack[4] = objs[53];
	m_uiButtonBack[5] = objs[54];
	m_uiButtonBack[6] = objs[55];
	m_uiButtonBack[7] = objs[56];
	for i=1, 14, 1 do
		m_uiButtonBack[10+i] = objs[56+i];
	end
	m_uiChat = objs[71];
	m_uiChatText = objs[72];
	m_uiQuest = objs[73];
	ButtonTable.m_uiButtonRestart = objs[74];
	m_uiTopLeft = objs[75];
	m_uiZoneName = objs[76];
	m_uiWeatherDay = objs[77];
	m_uiWeatherIcon = objs[78];
	m_uiWeatherAbility = objs[79];
	m_uiWarTable.m_uiWar = objs[80];
	m_uiWarTable.m_uiWarScroll = m_uiWarTable.m_uiWar.transform:Find("Scroll");
	m_uiWarTable.m_uiWarContent =  m_uiWarTable.m_uiWar.transform:Find("Scroll/Viewport/Content");
	m_uiWarTable.m_uiUIP_WarText = objs[81];
	m_uiWarTable.m_uiWarWarning = objs[82];
	ButtonTable.m_uiButtonGirl = objs[83];
	m_uiAutoGuard = objs[84];
	m_uiCutScenes = objs[85];
	m_uiQuestTitleText = objs[86];
	m_uiQuestEffect = objs[87];
	ButtonTable.m_uiButtonEquip = objs[88];
	ButtonTable.m_uiButtonUserCenter = objs[89];
	ButtonTable.m_uiButtonInviteCode = objs[90];
	--m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	--m_ObjectPool:CreatePool("UIP_WarText", 2, 2, m_uiWarTable.m_uiUIP_WarText);

	MainDlgWorkerObjectInit();
end 

-- 界面初始化时调用
function MainDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MainDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MainDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MainDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MainDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
-- 头像
function MainDlgSetHead()
	if m_Dlg == nil then
		return;
	end
	SetImage( m_uiHead, PlayerHeadSprite( GetPlayer().m_shape ) );
end

-- 等级
function MainDlgSetLevel()
	if m_Dlg == nil then
		return;
	end
	SetLevel( m_uiLevel, GetPlayer().m_level );
end

-- 国家
function MainDlgSetNation()
	if m_Dlg == nil then
		return;
	end
	SetImage( m_uiNation, NationSprite( GetPlayer().m_nation ) );
	SetImage( ButtonTable.m_uiButtonNation.transform:Find("Back"), NationSprite( GetPlayer().m_nation ) );
end

-- 银币
function MainDlgSetSilver()
	if m_Dlg == nil then
		return;
	end
	SetText( m_uiSilverNum, knum(GetPlayer().m_silver) );
end

-- 木材
function MainDlgSetWood()
	if m_Dlg == nil then
		return;
	end
	SetText( m_uiWoodNum, knum(GetPlayer().m_wood) );
end

-- 粮草
function MainDlgSetFood()
	if m_Dlg == nil then
		return;
	end
	SetText( m_uiFoodNum, knum(GetPlayer().m_food) );
end

-- 镔铁
function MainDlgSetIron()
	if m_Dlg == nil then
		return;
	end
	SetText( m_uiIronNum, knum(GetPlayer().m_iron) )
end

-- 体力
function MainDlgSetBody()
	if m_Dlg == nil then
		return;
	end
	if GetPlayer().m_body >= 100 then
		SetText( m_uiBodyNum, GetPlayer().m_body.."/100", Color.green );
	else
		SetText( m_uiBodyNum, GetPlayer().m_body.."/100", Color.white );
	end
	SetSlider( m_uiBodyProgress, GetPlayer().m_body/100 )
end

-- 步兵
function MainDlgSetInfantry()
	if m_Dlg == nil then
		return;
	end
	SetText( m_uiInfantryNum, GetPlayer().m_infantry_num );
end

-- 骑兵
function MainDlgSetCavalry()
	if m_Dlg == nil then
		return;
	end
	SetText( m_uiCavalryNum, GetPlayer().m_cavalry_num );
end

-- 弓兵
function MainDlgSetArcher()
	if m_Dlg == nil then
		return;
	end
	SetText( m_uiArcherNum, GetPlayer().m_archer_num );
end

-- 钻石
function MainDlgSetToken()
	if m_Dlg == nil then
		return;
	end
	SetText( m_uiTokenNum, knum(GetPlayer().m_token) );
end

-- VIP
function MainDlgSetVipLevel()
	if m_Dlg == nil then
		return;
	end
	SetText( m_uiVipLevel, "v"..GetPlayer().m_viplevel );
end

-- exp
function MainDlgSetExp()
	if m_Dlg == nil then
		return;
	end
	SetSlider( m_uiExpProgress, GetPlayer().m_exp/GetPlayer().m_exp_max )
end

-- 聊天
function MainDlgSetChat( recvValue )
	if m_Dlg == nil then
		return;
	end
	if recvValue.m_actorid < 0 then
		-- 系统
		local name = "<color=FF0000FF>["..recvValue.m_name.."]：</color>"
		local msg = recvValue.m_msg
		SetRichText( m_uiChatText, name..msg )
	else
		if recvValue.m_channel == 2 then
		else
			local nation = "<color="..NationColorStr(recvValue.m_nation)..">【"..Nation( recvValue.m_nation ).."】</color>"
			local name = "<color=FFB900FF>["..recvValue.m_name.."]：</color>"
			local msg = ChatDlgMakeMsg( recvValue )
			if recvValue.m_msgtype == CHAT_MSGTYPE_VS or recvValue.m_msgtype == CHAT_MSGTYPE_SPY then
				SetRichText( m_uiChatText, nation..name..msg, ChatDlgOnLinkClickMail )
			else
				SetRichText( m_uiChatText, nation..name..msg, ChatDlgOnLinkClickPos )
			end
		end
	end
end

-- 任务
function MainDlgSetQuest()
	if CacheQuest == nil then
		return;
	end
	if CacheQuest.m_list[1] == nil then
		return;
	end
	SetText( m_uiQuestText, QuestName( -1, CacheQuest.m_list[1] ) )
	SetText( m_uiQuestTitleText, QuestTypeName(1));
	SetFalse( m_uiQuestEffect.transform);
	-- 引导任务部分
	local questid = CacheQuest.m_list[1].m_questid;
	
	-- 弹出受伤少年
	if questid == 20 then
		City.BuildingQuestMod( questid )
		
	-- 清剿山贼
	elseif questid == 55 then
		City.BuildingQuestMod( questid )
	else
		BuildingQuestModHide()
	end
	
	-- 设置支线任务
	local questnum = 0;
	for i=0, 4, 1 do
		local questInfo = CacheQuest.m_list[i+2]
		local uiObj = m_uiQuestList.transform:GetChild( i )
		if questInfo == nil then
			SetFalse( uiObj );
		else
			SetTrue( uiObj );
			SetControlID( uiObj, 40+i )
			SetText( uiObj.transform:Find("Text"), QuestType( questInfo ) )
			SetText( uiObj.transform:Find("QuestText"), QuestName( -1, questInfo ) )
			questnum = questnum + 1;
		end
		
		CheckQuestFinish(questInfo,i+2);
	end
	
	if questnum > 0 then
		SetTrue( m_uiQuest.transform:Find("Button") )
		SetText( m_uiQuest.transform:Find("Button/Num"), questnum )
	else
		SetFalse( m_uiQuest.transform:Find("Button") )
	end
end

function CheckQuestFinish(questInfo,i)
	if questInfo ~= nil then
		-- 副本
		if questInfo.m_datatype == QUEST_DATATYPE_STORY then
			if questInfo.m_value > questInfo.m_datakind then
				SetText( m_uiQuestText, QuestName( -1, questInfo ));
				SetText( m_uiQuestTitleText, QuestTypeName(2));
				SetTrue( m_uiQuestEffect.transform);
				m_finishTask = i;
			end
		else	
			if questInfo.m_value >= questInfo.m_needvalue then
				SetText( m_uiQuestText, QuestName( -1, questInfo ));
				SetText( m_uiQuestTitleText, QuestTypeName(2));
				SetTrue( m_uiQuestEffect.transform);
				m_finishTask = i;
			end
		end
	end
end


-- 邮件数量
function MainDlgSetMailNum( num )
	GetMail().m_nNoReadCount = num;
	if m_Dlg == nil then
		return;
	end
	if num <= 0 then
		SetFalse( ButtonTable.m_uiButtonMail.transform:Find("Back/NumBack") )
		SetFalse( ButtonTable.m_uiButtonMail.transform:Find("Back/Num") )
	else
		SetTrue( ButtonTable.m_uiButtonMail.transform:Find("Back/NumBack") )
		SetTrue( ButtonTable.m_uiButtonMail.transform:Find("Back/Num") )
		SetText( ButtonTable.m_uiButtonMail.transform:Find("Back/Num"), num );
	end
end

-- 建造队列
function MainDlgSetWorker()
	--MainDlgWorkerObjectInit();
	MainDlgSetWorkerObject( 1,
	m_uiWorker, 
	GetPlayer().m_worker_kind, 
	GetPlayer().m_worker_offset, 
	GetPlayer().m_worker_needsec, 
	GetPlayer().m_worker_sec, 
	0 );
	
	MainDlgSetWorkerObject( 2,
	m_uiWorkerEx, 
	GetPlayer().m_worker_kind_ex, 
	GetPlayer().m_worker_offset_ex, 
	GetPlayer().m_worker_needsec_ex, 
	GetPlayer().m_worker_sec_ex,
	GetPlayer().m_worker_expire_ex );

end

function MainDlgSetWorkerObject( type, uiWorker, kind, offset, needsec, sec, expire )
	if uiWorker == nil then
		return;
	end
	if kind <= 0 then
		-- 缺省文字
		local NormalText = uiWorker.transform:Find("NormalText");
		NormalText.gameObject:SetActive(true);
		if type == 2 and expire <= 0 then
			SetText(NormalText, T(601))
		else
			SetText(NormalText, T(600))
		end
		
		-- 停止动画
		MainDlgWorkerStop( type )
		local NormalShape = m_uiNormalShape[type]
		if type == 2 then
			SetImage( NormalShape, LoadSprite( "ui_main_worker_3" ) )
		else
			SetImage( NormalShape, LoadSprite( "ui_main_worker_4" ) )
		end
		SetTrue(NormalShape)
		NormalShape.transform.localScale = Vector3.one;
		SetFalse(uiWorker.transform:Find("WorkProgress"))
		SetFalse(uiWorker.transform:Find("BuildingShape"))
		SetFalse(uiWorker.transform:Find("BuildingTimer"))
		SetFalse(uiWorker.transform:Find("BuildingName"))
		return;
	end
	
	-- 建筑名
	local name = T(kind);
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		--name = (offset%16+1)..T(597)..T(kind);		
		name = T(kind);
	end
		
	-- 建筑形象
	SetImage( m_uiBuildingShape[type], BuildingIconSprite( kind ) );
	SetFalse( m_uiBuildingShape[type] )
	
	-- 建筑名称
	SetText( m_uiBuildingName[type], name )
	SetTrue( m_uiBuildingName[type] )
	if m_uiBuildingNameUITweenScale[type].isPlaying == false then
		m_uiBuildingNameUITweenScale[type]:Play(true);
	end
	
	-- 建筑计时器
	SetTrue( m_uiBuildingTimer[type] )
	
	-- 计时器
	local timer = m_uiBuildingTimer[type]:GetComponent( "UITextTimeCountdown" );
	timer:SetTime( needsec, needsec-sec );
	SetFalse( m_uiBuildingTimer[type] )
	
	-- 进度条
	SetTrue( uiWorker.transform:Find("WorkProgress") )
	
	-- 缺省锤子形象
	local NormalShape = m_uiNormalShape[type];
	SetTrue( NormalShape )
	if NormalShape:GetComponent("UITweenScale").isPlaying == false then
		NormalShape:GetComponent("UITweenScale"):Play(true);
	end
	
	if type == 1 then
		if GetPlayer().m_worker_free > 0 then
			SetImage( NormalShape, LoadSprite( "ui_main_worker_free" ) )
		else
			SetImage( NormalShape, LoadSprite( "ui_main_worker_4" ) )
		end
	elseif type == 2 then 
		if GetPlayer().m_worker_free_ex > 0 then
			SetImage( NormalShape, LoadSprite( "ui_main_worker_free" ) )
		else
			SetImage( NormalShape, LoadSprite( "ui_main_worker_3" ) )
		end
	end
	-- 缺省文字
	SetFalse( uiWorker.transform:Find("NormalText") )
end

-- 停止动画
function MainDlgWorkerStop( type )
	m_uiBuildingShapeUITweenScale[type]:Kill(false);
	m_uiBuildingShapeUITweenScale[type]:ToInit()
	m_uiNormalShapeUITweenScale[type]:Kill(false);
	m_uiNormalShapeUITweenScale[type]:ToInit()
	m_uiBuildingTimerUITweenScale[type]:Kill(false);
	m_uiBuildingTimerUITweenScale[type]:ToInit()
	m_uiBuildingNameUITweenScale[type]:Kill(false);
	m_uiBuildingNameUITweenScale[type]:ToInit()
	
	m_uiBuildingShapeUITweenScale1[type]:Kill(false);
	m_uiBuildingShapeUITweenScale1[type]:ToInit()
	m_uiNormalShapeUITweenScale1[type]:Kill(false);
	m_uiNormalShapeUITweenScale1[type]:ToInit()
	m_uiBuildingTimerUITweenScale1[type]:Kill(false);
	m_uiBuildingTimerUITweenScale1[type]:ToInit()
	m_uiBuildingNameUITweenScale1[type]:Kill(false);
	m_uiBuildingNameUITweenScale1[type]:ToInit()
end

-- 建造队列动画所需缓存
function MainDlgWorkerObjectInit()
	m_uiBuildingShape[1] = m_uiWorker.transform:Find("BuildingShape").gameObject
	m_uiNormalShape[1] = m_uiWorker.transform:Find("NormalShape").gameObject
	m_uiBuildingTimer[1] = m_uiWorker.transform:Find("BuildingTimer").gameObject
	m_uiBuildingName[1] = m_uiWorker.transform:Find("BuildingName").gameObject
	
	m_uiBuildingShapeUITweenScale[1] = m_uiWorker.transform:Find("BuildingShape"):GetComponent("UITweenScale")
	m_uiNormalShapeUITweenScale[1] = m_uiWorker.transform:Find("NormalShape"):GetComponent("UITweenScale")
	m_uiBuildingTimerUITweenScale[1] = m_uiWorker.transform:Find("BuildingTimer"):GetComponent("UITweenScale")
	m_uiBuildingNameUITweenScale[1] = m_uiWorker.transform:Find("BuildingName"):GetComponent("UITweenScale")

	m_uiBuildingShapeUITweenScale1[1] = m_uiWorker.transform:Find("BuildingShape"):GetComponent("UITweenScale1")
	m_uiNormalShapeUITweenScale1[1] = m_uiWorker.transform:Find("NormalShape"):GetComponent("UITweenScale1")
	m_uiBuildingTimerUITweenScale1[1] = m_uiWorker.transform:Find("BuildingTimer"):GetComponent("UITweenScale1")
	m_uiBuildingNameUITweenScale1[1] = m_uiWorker.transform:Find("BuildingName"):GetComponent("UITweenScale1")
	
	m_uiBuildingShape[2] = m_uiWorkerEx.transform:Find("BuildingShape").gameObject
	m_uiNormalShape[2] = m_uiWorkerEx.transform:Find("NormalShape").gameObject
	m_uiBuildingTimer[2] = m_uiWorkerEx.transform:Find("BuildingTimer").gameObject
	m_uiBuildingName[2] = m_uiWorkerEx.transform:Find("BuildingName").gameObject
	
	m_uiBuildingShapeUITweenScale[2] = m_uiWorkerEx.transform:Find("BuildingShape"):GetComponent("UITweenScale")
	m_uiNormalShapeUITweenScale[2] = m_uiWorkerEx.transform:Find("NormalShape"):GetComponent("UITweenScale")
	m_uiBuildingTimerUITweenScale[2] = m_uiWorkerEx.transform:Find("BuildingTimer"):GetComponent("UITweenScale")
	m_uiBuildingNameUITweenScale[2] = m_uiWorkerEx.transform:Find("BuildingName"):GetComponent("UITweenScale")

	m_uiBuildingShapeUITweenScale1[2] = m_uiWorkerEx.transform:Find("BuildingShape"):GetComponent("UITweenScale1")
	m_uiNormalShapeUITweenScale1[2] = m_uiWorkerEx.transform:Find("NormalShape"):GetComponent("UITweenScale1")
	m_uiBuildingTimerUITweenScale1[2] = m_uiWorkerEx.transform:Find("BuildingTimer"):GetComponent("UITweenScale1")
	m_uiBuildingNameUITweenScale1[2] = m_uiWorkerEx.transform:Find("BuildingName"):GetComponent("UITweenScale1")
end

-- 自动建造
function MainDlgSetAutoBuild( path )
	if GetPlayer().m_autobuild > 0 then
		SetTrue( m_uiAutoBuild );
	else
		SetFalse( m_uiAutoBuild );
		return
	end
	if GetPlayer().m_autobuildopen == 1 then
		SetTrue( m_uiAutoBuild.transform:Find("Back/Effect") )
	else
		SetFalse( m_uiAutoBuild.transform:Find("Back/Effect") )
	end
	SetText( m_uiAutoBuild.transform:Find("Back/Num"), GetPlayer().m_autobuild )
	
	-- 播放特效
	if path == PATH_QUEST or path == PATH_GM then
		local tween = m_uiAutoBuild.transform:GetComponent( "UITweenRectPosition" );
		tween.from = Vector2.New( 315, -432 );
		tween:Play(true);
	end
	
	if IsGuiding() then
		if GetCurrentGuideType() == GUIDE_AUTOBUILDING then
			GuideNext();
		end
	end
end

-- 城防补充
function MainDlgSetAutoGuard( path )
	if GetPlayer().m_autoguard > 0 then
		SetTrue( m_uiAutoGuard );
	else
		SetFalse( m_uiAutoGuard );
		return
	end
	if GetPlayer().m_autoguardopen == 1 then
		SetTrue( m_uiAutoGuard.transform:Find("Back/Effect") )
	else
		SetFalse( m_uiAutoGuard.transform:Find("Back/Effect") )
	end
	SetText( m_uiAutoGuard.transform:Find("Back/Num"), GetPlayer().m_autoguard )
	
	-- 播放特效
	if path == PATH_QUEST or path == PATH_GM then
		local tween = m_uiAutoGuard.transform:GetComponent( "UITweenRectPosition" );
		tween.from = Vector2.New( 315, -432 );
		tween:Play(true);
	end
end

function MainDlgGetEmptyButton()
	for i=1, 24, 1 do
		if m_hasButton[i] == false and m_uiButtonBack[i] ~= nil then
			return i, m_uiButtonBack[i];
		end
	end
	return -1, nil;
end

-- 设置获得的功能按钮
function MainDlgSetButtons( openoffset )
	for i=1, 24, 1 do
		m_hasButton[i] = false;
	end	
	-- 英雄
	SetParent( ButtonTable.m_uiButtonHero, m_uiButtonBack[1] );
	m_hasButton[1] = true;
	
	-- 任务
	SetParent( ButtonTable.m_uiButtonQuest, m_uiButtonBack[2] );
	m_hasButton[2] = true;
	
	-- 世界
	SetParent( ButtonTable.m_uiButtonWorld, m_uiButtonBack[4] );
	SetParent( ButtonTable.m_uiButtonCity, m_uiButtonBack[4] );
	SetFalse( ButtonTable.m_uiButtonWorld );
	SetFalse( ButtonTable.m_uiButtonCity );
	m_hasButton[4] = true;
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_WORLD ) == 1 then
		SetTrue( ButtonTable.m_uiButtonWorld );
		if openoffset == CITY_FUNCTION_WORLD then
			MainDlgButtonPlayEffect( ButtonTable.m_uiButtonWorld )
		end
	end
	
	-- 更多
	SetParent( ButtonTable.m_uiButtonMore, m_uiButtonBack[7] );
	m_hasButton[7] = true;
	
	-- 国家
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_NATION ) == 1 then
		local offset, root = MainDlgGetEmptyButton();
		if root ~= nil then
			SetParent( ButtonTable.m_uiButtonNation, m_uiButtonBack[offset] );
			m_hasButton[offset] = true;
		end	
		if openoffset == CITY_FUNCTION_NATION then
			MainDlgButtonPlayEffect( ButtonTable.m_uiButtonNation )
		end
	end
	
	-- 副本
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_STORY ) == 1 then
		local offset, root = MainDlgGetEmptyButton();
		if root ~= nil then
			SetParent( ButtonTable.m_uiButtonStory, m_uiButtonBack[offset] );
			m_hasButton[offset] = true;
		end	
		if openoffset == CITY_FUNCTION_STORY then
			MainDlgButtonPlayEffect( ButtonTable.m_uiButtonStory )
		end
	end
		
	-- 邮件
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_MAIL ) == 1 then
		local offset, root = MainDlgGetEmptyButton();
		if root ~= nil then
			SetParent( ButtonTable.m_uiButtonMail, m_uiButtonBack[offset] );
			m_hasButton[offset] = true;
		end
		if openoffset == CITY_FUNCTION_MAIL then
			MainDlgButtonPlayEffect( ButtonTable.m_uiButtonMail )
		end
	end	

	-- 背包
	local offset, root = MainDlgGetEmptyButton();
	if root ~= nil then
		SetParent( ButtonTable.m_uiButtonBag, m_uiButtonBack[offset] );
		m_hasButton[offset] = true;
	end
	
	-- 装备
	local offset, root = MainDlgGetEmptyButton();
	if root ~= nil then
		SetParent( ButtonTable.m_uiButtonEquip, m_uiButtonBack[offset] );
		m_hasButton[offset] = true;
	end
	
	-- 好友
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_FRIEND ) == 1 then
		local offset, root = MainDlgGetEmptyButton();
		if root ~= nil then
			SetParent( ButtonTable.m_uiButtonFriend, m_uiButtonBack[offset] );
			m_hasButton[offset] = true;
		end
		if openoffset == CITY_FUNCTION_FRIEND then
			MainDlgButtonPlayEffect( ButtonTable.m_uiButtonFriend )
		end
	end	
	
	-- 女将
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_FANGSHI ) == 1 then
		local offset, root = MainDlgGetEmptyButton();
		if root ~= nil then
			SetParent( ButtonTable.m_uiButtonGirl, m_uiButtonBack[offset] );
			m_hasButton[offset] = true;
		end
		if openoffset == CITY_FUNCTION_FANGSHI then
			MainDlgButtonPlayEffect( ButtonTable.m_uiButtonGirl )
		end
	end	
	
	-- 国器
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_NATIONEQUIP ) == 1 then
		local offset, root = MainDlgGetEmptyButton();
		if root ~= nil then
			SetParent( ButtonTable.m_uiButtonNationEquip, m_uiButtonBack[offset] );
			m_hasButton[offset] = true;
		end
		if openoffset == CITY_FUNCTION_NATIONEQUIP then
			MainDlgButtonPlayEffect( ButtonTable.m_uiButtonNationEquip )
		end
	end	
	
	-- 排行榜
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_RANK ) == 1 then
		local offset, root = MainDlgGetEmptyButton();
		if root ~= nil then
			SetParent( ButtonTable.m_uiButtonRank, m_uiButtonBack[offset] );
			m_hasButton[offset] = true;
		end
		if openoffset == CITY_FUNCTION_RANK then
			MainDlgButtonPlayEffect( ButtonTable.m_uiButtonRank )
		end
	end	
	
	-- 系统公告
	local offset, root = MainDlgGetEmptyButton();
	if root ~= nil then
		SetParent( ButtonTable.m_uiButtonNotice, m_uiButtonBack[offset] );
		m_hasButton[offset] = true;
	end
	
	-- 设置
	local offset, root = MainDlgGetEmptyButton();
	if root ~= nil then
		SetParent( ButtonTable.m_uiButtonSetting, m_uiButtonBack[offset] );
		m_hasButton[offset] = true;
	end
	
	-- 联系客服
	local offset, root = MainDlgGetEmptyButton();
	if root ~= nil then
		SetParent( ButtonTable.m_uiButtonGM, m_uiButtonBack[offset] );
		m_hasButton[offset] = true;
	end
	
	-- 返回登陆
	local offset, root = MainDlgGetEmptyButton();
	if root ~= nil then
		SetParent( ButtonTable.m_uiButtonRelogin, m_uiButtonBack[offset] );
		m_hasButton[offset] = true;
	end
	
	-- 重新创角
	if Const.platid <= 11 then
		local offset, root = MainDlgGetEmptyButton();
		if root ~= nil then
			SetParent( ButtonTable.m_uiButtonRestart, m_uiButtonBack[offset] );
			m_hasButton[offset] = true;
		end
	end
	
	-- 聊天
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_CHAT ) == 1 then
		SetTrue( m_uiChat )
		if openoffset == CITY_FUNCTION_CHAT then
		end
	end	
	
	-- 用户中心
	if Const.platid == 1 or Const.platid >= 13 and Const.platid <= 17 then
		local offset, root = MainDlgGetEmptyButton();
		if root ~= nil then
			SetParent( ButtonTable.m_uiButtonUserCenter, m_uiButtonBack[offset] );
			m_hasButton[offset] = true;
		end
	end
	
	-- 送福利
	if Const.platid == 1 or Const.platid == 18 or Const.platid == 19 then
		local offset, root = MainDlgGetEmptyButton();
		if root ~= nil then
			SetParent( ButtonTable.m_uiButtonInviteCode, m_uiButtonBack[offset] );
			m_hasButton[offset] = true;
		end
	end
end

-- 播放获得功能按钮特效
function MainDlgButtonPlayEffect( uiObj )
	local effect = GameObject.Instantiate( LoadPrefab( "Tbcx" ) )
	effect.transform:SetParent( uiObj.transform );
	effect.transform.localPosition = Vector3( 0, 0, 0 );
	GameObject.Destroy( effect, 5 );
end

-- 显示内城的控件
function MainDlgShowCity()
	SetTrue( m_uiTop_City )
	SetFalse( m_uiTop_Map )
	SetTrue( ButtonTable.m_uiButtonWorld )
	SetFalse( ButtonTable.m_uiButtonCity )
	MapMainDlgClose()
	PatrolModPlay();
	MainDlgSetWorker()
end

-- 显示外城的空间
function MainDlgShowMap()
	SetTrue( m_uiTop_Map )
	SetFalse( m_uiTop_City )
	SetFalse( ButtonTable.m_uiButtonWorld )
	SetTrue( ButtonTable.m_uiButtonCity )
	MapMainDlgShow()
	SetText( m_uiTopLeft.transform:Find( "Name" ), NationEx( GetPlayer().m_nation ) );
	SetImage( m_uiTopLeft.transform:Find( "Icon" ), LoadSprite( "ui_worldmap_nation_"..GetPlayer().m_nation ) )
	PatrolModStop();
end

-- 天气
function MainDlgSetWeather( game_day, game_weather, game_day_loop )
	g_game_day = game_day;
	g_game_weather = game_weather;
	g_game_day_loop = game_day_loop;
	if g_game_day <= 0 then
		SetText( m_uiWeatherDay, "" )
		SetTrue( m_uiWeatherIcon )
		SetImage( m_uiWeatherIcon, LoadSprite( "ui_mapmain_weather_lock" ) )
		SetText( m_uiWeatherAbility, "" )
	else
		local day = g_game_day_loop;
		if day == 0 then
			day = 1;
		end
		SetText( m_uiWeatherDay, T(g_weather[day][g_game_weather].nameid) )
		SetTrue( m_uiWeatherIcon )
		SetImage( m_uiWeatherIcon, LoadSprite( "ui_mapmain_weather_"..(game_weather) ) )
		SetText( m_uiWeatherAbility, T(g_weather[g_game_day][g_game_weather].descid) )
	end
end

-- 设置区域名称
function MainDlgSetZoneName( name )
	SetText( m_uiZoneName, name )
end

-- 军情列表
function MainDlgSetWarCache( recvValue )
	MainDlgClearWarObj();
	for i=1, recvValue.m_count, 1 do
		table.insert( m_uiWarTable.m_cache, recvValue.m_list[i] );
		MainDlgCreateWarObj( recvValue.m_list[i] )
		--table.insert( m_uiWarTable.m_cacheObj, MainDlgCreateWarObj( recvValue.m_list[i] ) );
	end
	
	if #m_uiWarTable.m_cache == 0 then
		SetFalse( m_uiWarTable.m_uiWar )
		SetFalse( m_uiWarTable.m_uiWarWarning )
		SetFalse( m_uiWarTable.m_uiUIP_WarText )
	else
		SetTrue( m_uiWarTable.m_uiWar )
		SetTrue( m_uiWarTable.m_uiWarWarning )
		SetTrue( m_uiWarTable.m_uiUIP_WarText )
	end
end

-- 添加军情
-- m_group_index=0,m_group_id=0,m_from_nation=0,m_from_posx=0,m_from_posy=0,m_namelen=0,m_name="[m_namelen]",m_statetime=0,m_stateduration=0,
function MainDlgAddWar( recvValue )
	local update = 0;
	for i=1, #m_uiWarTable.m_cache, 1 do
		local info = m_uiWarTable.m_cache[i];
		if info ~= nil and info.m_group_index == recvValue.m_group_index and info.m_group_id == recvValue.m_group_id then
			m_uiWarTable.m_cache[i] = recvValue;
			MainDlgUpdateWarObj( m_uiWarTable.m_cacheObj[i], recvValue )
			update = 1;
		end
	end
	if update == 0 then
		table.insert( m_uiWarTable.m_cache, recvValue );
		MainDlgCreateWarObj( recvValue )
	end
	SetTrue( m_uiWarTable.m_uiWar )
	SetTrue( m_uiWarTable.m_uiWarWarning )
	SetTrue( m_uiWarTable.m_uiUIP_WarText )
end

-- 删除军情
-- m_group_index=0,
function MainDlgAddDel( recvValue )
	for i=1, #m_uiWarTable.m_cache, 1 do
		local info = m_uiWarTable.m_cache[i];
		if info ~= nil and info.m_group_index == recvValue.m_group_index then
			table.remove(  m_uiWarTable.m_cache, i );
			--m_ObjectPool:Release( "UIP_WarText", m_uiWarTable.m_cacheObj[i] );
			--table.remove(  m_uiWarTable.m_cacheObj, i );
			break
		end
	end
	if #m_uiWarTable.m_cache == 0 then
		SetFalse( m_uiWarTable.m_uiWar )
		SetFalse( m_uiWarTable.m_uiWarWarning )
		SetFalse( m_uiWarTable.m_uiUIP_WarText )
	else
		SetTrue( m_uiWarTable.m_uiWar )
		SetTrue( m_uiWarTable.m_uiWarWarning )
		SetTrue( m_uiWarTable.m_uiUIP_WarText )
	end
end

-- 设置军情
function MainDlgCreateWarObj( info )
	local uiObj = m_uiWarTable.m_uiUIP_WarText
	--local uiObj = m_ObjectPool:Get( "UIP_WarText" );
	uiObj.transform:SetParent( m_uiWarTable.m_uiWarContent.transform );
	local warn = F( 1345, Nation( info.m_from_nation ), info.m_name, info.m_from_posx, info.m_from_posy ).." {0}";
	SetTimer( uiObj.transform:Find( "Text" ), info.m_stateduration-info.m_statetime, info.m_stateduration, 0, warn )		
	return uiObj
end

-- 更新军情
function MainDlgUpdateWarObj( uiObj, info )	
	if uiObj == nil then
		return
	end
	uiObj.transform:SetParent( m_uiWarTable.m_uiWarContent.transform );
	local warn = F( 1345, Nation( info.m_from_nation ), info.m_name, info.m_from_posx, info.m_from_posy ).." {0}";
	SetTimer( uiObj.transform:Find( "Text" ), info.m_stateduration-info.m_statetime, info.m_stateduration, 0, warn )
end
	
function MainDlgClearWarObj()
	SetFalse(m_uiWarTable.m_uiUIP_WarText)
	--[[local objs = {};
	for i = 0 ,m_uiWarTable.m_uiWarContent.transform.childCount - 1 do
		table.insert( objs, m_uiWarTable.m_uiWarContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_WarText(Clone)" then
			m_ObjectPool:Release( "UIP_WarText", obj );
		end
    end--]]
end

-- 初始化切换场景
function MainDlgCutScenesInit()
	if m_Dlg == nil then
		return;
	end
	SetFalse( m_uiCutScenes )
	m_uiCutScenes.transform:SetParent( eye.uiManager:GetLayer( 3 ).transform )
end

-- 播放城内城外切换动画
function MainDlgPlayCutScenes()
	SetTrue( m_uiCutScenes )
end

function MainDlgStopCutScenes()
	SetFalse( m_uiCutScenes )
end

function MainDlgFristPayShow( show )
	if m_Dlg == nil then
		return;
	end
	SetShow( m_uiTop_City.transform:Find("Right/FristPay"), show )
	SetShow( m_uiTop_City.transform:Find("Right/PayBag"), not show )
end

-- 国家按钮红点
function MainDlgButtonNationShow( show )
	if m_Dlg == nil then
		return;
	end
	SetShow( ButtonTable.m_uiButtonNation.transform:Find("Back/Red"), show  )
end

-- 活动按钮红点
function MainDlgButtonActivityShow( show )
	if m_Dlg == nil then
		return;
	end
	SetShow( m_uiTop_City.transform:Find("Right/Activity/Back/Red"), show )
end

-- 得到点击区域位置
function GetQuestDlgPos()
	return m_uiQuest.transform.position
end

function GetCopyPos()
	return m_uiButtonBack[3].transform.position;
end

function GetWorldPos()
	SetTrue( ButtonTable.m_uiButtonWorld );
	return m_uiButtonBack[4].transform.position
end

function GetBackPos()
	return m_uiButtonBack[4].transform.position
end

function GetAutoBuildPos()
	return m_uiAutoBuild.transform.position;
end

function GetHeroDlgPos()
	return m_uiButtonBack[1].transform.position;
end

function GetWorkPos()
	return m_uiButtonBack[2].transform.position;
end