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
local m_uiQuestText = nil; --UnityEngine.GameObject
local m_uiButtonHero = nil; --UnityEngine.GameObject
local m_uiButtonQuest = nil; --UnityEngine.GameObject
local m_uiButtonNation = nil; --UnityEngine.GameObject
local m_uiButtonWorld = nil; --UnityEngine.GameObject
local m_uiButtonCity = nil; --UnityEngine.GameObject
local m_uiButtonStory = nil; --UnityEngine.GameObject
local m_uiButtonMail = nil; --UnityEngine.GameObject
local m_uiButtonMore = nil; --UnityEngine.GameObject
local m_uiButtonBag = nil; --UnityEngine.GameObject
local m_uiButtonFriend = nil; --UnityEngine.GameObject
local m_uiButtonNationEquip = nil; --UnityEngine.GameObject
local m_uiButtonRank = nil; --UnityEngine.GameObject
local m_uiButtonNotice = nil; --UnityEngine.GameObject
local m_uiButtonSetting = nil; --UnityEngine.GameObject
local m_uiButtonGM = nil; --UnityEngine.GameObject
local m_uiButtonRelogin = nil; --UnityEngine.GameObject
local m_uiButtonRestart = nil; --UnityEngine.GameObject
local m_uiFunctionPanel = nil; --UnityEngine.GameObject
local m_uiMorePanel = nil; --UnityEngine.GameObject
local m_bMorePanel = false;
local m_uiButtonBack = {}; --UnityEngine.GameObject
local m_hasButton = {};
local m_uiChat = nil; --UnityEngine.GameObject
local m_uiChatText = nil; --UnityEngine.GameObject
local m_uiQuest = nil; --UnityEngine.GameObject
local m_uiNationName = nil; --UnityEngine.GameObject
local m_uiZoneName = nil; --UnityEngine.GameObject
local m_uiWeatherDay = nil; --UnityEngine.GameObject
local m_uiWeatherIcon = nil; --UnityEngine.GameObject
local m_uiWeatherAbility = nil; --UnityEngine.GameObject

local m_uiMiniMap ={}
m_uiMiniMap.m_uiMoveLayer = nil; --UnityEngine.GameObject
m_uiMiniMap.m_uiUIP_MiniTmx = nil; --UnityEngine.GameObject
m_uiMiniMap.m_uiUIP_ZoneUnit = nil; --UnityEngine.GameObject

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

-- 打开界面
function MainDlgOpen()
	m_Dlg = eye.uiManager:Open( "MainDlg" );
end

-- 隐藏界面
function MainDlgClose()
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
        if nControlID == -1 then
            --MainDlgClose();
		
		-- 英雄
		elseif nControlID == 1 then
			HeroDlgShow();
			
		elseif nControlID == 2 then
			--local kind = 21;
			--local offsetlist ={ 49, 50, 51, 52 }
			--for i=1, 2, 1 do
				--local offset = math.random( 0, 63 )
				--BuildingGetDlgShow( {m_kind=kind,m_offset=offset,m_level=1} );
			--end
		elseif nControlID == 3 then
			--QuestAwardDlgShow();
		
		elseif nControlID == 4 or nControlID == 5 then
			if GameManager.currentScence == "city" then
				WorldMap.GotoWorldMap(-1, -1)
				MainDlgShowMap()
			else
				WorldMap.ReturnCity()
				MainDlgShowCity()
			end
		
		elseif nControlID == 6 then
			StoryDlgShow()
	
		elseif nControlID == 7 then
			MailDlgShow()
		
		-- 更多	
		elseif nControlID == 8 then
			if m_bMorePanel == true then
				SetFalse(m_uiMorePanel)
				m_bMorePanel = false;
			else
				SetTrue(m_uiMorePanel)
				m_bMorePanel = true;
			end
		
		-- 背包
		elseif nControlID == 11 then
			BagDlgShow();
		
		elseif nControlID == 12 then
			ChangeNameDlgShow();
			
		-- 返回登录
		elseif nControlID == 18 then
			MsgBox( T( 518 ),function()
				GameManager.Restart();
				GameManager.Logout( 1 );
			end )
		
		elseif nControlID == 19 then
			MsgBox( "重新创建角色吗？",function()
				system_askinfo( ASKINFO_ACTOR, "", 1 );
				GameManager.Restart();
				GameManager.Logout( 1 );
			end )
			
		-- 聊天
		elseif nControlID == 30 then
			ChatDlgShow();
		
		-- 任务导航
		elseif nControlID == 31 then
			QuestGoto( 1 );
			
		-- 点击建造队列
		elseif nControlID == 50 then
			City.GoToWorker()
		
		-- 点击建造队列商用
		elseif nControlID == 51 then
			City.GoToWorkerEx()
			
		elseif nControlID == 100 then
			PlayerDlgShow();
		elseif nControlID == 101 then
		
	
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
	m_uiButtonHero = objs[32];
	m_uiButtonQuest = objs[33];
	m_uiButtonNation = objs[34];
	m_uiButtonWorld = objs[35];
	m_uiButtonCity = objs[36];
	m_uiButtonStory = objs[37];
	m_uiButtonMail = objs[38];
	m_uiButtonMore = objs[39];
	m_uiButtonBag = objs[40];
	m_uiButtonFriend = objs[41];
	m_uiButtonNationEquip = objs[42];
	m_uiButtonRank = objs[43];
	m_uiButtonNotice = objs[44];
	m_uiButtonSetting = objs[45];
	m_uiButtonGM = objs[46];
	m_uiButtonRelogin = objs[47];
	m_uiFunctionPanel = objs[48];
	m_uiMorePanel = objs[49];
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
	m_uiButtonRestart = objs[74];
	m_uiNationName = objs[75];
	m_uiZoneName = objs[76];
	m_uiWeatherDay = objs[77];
	m_uiWeatherIcon = objs[78];
	m_uiWeatherAbility = objs[79];
	m_uiMiniMap.m_uiMoveLayer = objs[80];
	m_uiMiniMap.m_uiUIP_MiniTmx = objs[81];
	m_uiMiniMap.m_uiUIP_ZoneUnit = objs[82];
	MainDlgMiniMapInit();
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
	SetImage( m_uiButtonNation.transform:Find("Back"), NationSprite( GetPlayer().m_nation ) );
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
	SetText( m_uiTokenNum, GetPlayer().m_token );
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
	if recvValue.m_actorid < 0 then
		-- 系统
		local name = "<color=FF0000FF>["..recvValue.m_name.."]：</color>"
		local msg = recvValue.m_msg
		SetRichText( m_uiChatText, name..msg )
	else
		if recvValue.m_channel == 2 then
		else
			local nation = "<color=4F57FFFF>【"..Nation( recvValue.m_nation ).."】</color>"
			local name = "<color=FFB900FF>["..recvValue.m_name.."]：</color>"
			local msg = recvValue.m_msg
			SetRichText( m_uiChatText, nation..name..msg )
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
	
	-- 引导任务部分
	local questid = CacheQuest.m_list[1].m_questid;
	-- 弹出改名
	if questid == 4 then
		if NpcTalkIsShow() == true then
			NpcTalkWait( ChangeNameDlgShow, nil )
		else
			ChangeNameDlgShow();
		end
	
	-- 查看停留在木场上的马岱
	elseif questid == 5 then
		City.BuildingQuestMod( questid )
	
	-- 自动建造
	elseif questid == 6 then
		if HeroGetDlgIsShow() == false then
			HeroTalkKind( 1, T(10003) )
		end
	end
end

-- 邮件数量
function MainDlgSetMailNum( num )
	if m_Dlg == nil then
		return;
	end
	if num <= 0 then
		SetFalse( m_uiButtonMail.transform:Find("Back/NumBack") )
		SetFalse( m_uiButtonMail.transform:Find("Back/Num") )
	else
		SetTrue( m_uiButtonMail.transform:Find("Back/NumBack") )
		SetTrue( m_uiButtonMail.transform:Find("Back/Num") )
		SetText( m_uiButtonMail.transform:Find("Back/Num"), num );
	end
end

-- 建造队列
function MainDlgSetWorker()
	MainDlgWorkerObjectInit();
		
	MainDlgSetWorkerObject( 0,
	m_uiWorker, 
	GetPlayer().m_worker_kind, 
	GetPlayer().m_worker_offset, 
	GetPlayer().m_worker_needsec, 
	GetPlayer().m_worker_sec, 
	0 );
	
	MainDlgSetWorkerObject( 1,
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
		if type == 1 and expire <= 0 then
			NormalText:GetComponent( "UIText" ).text = T(601);
		else
			NormalText:GetComponent( "UIText" ).text = T(600);
		end
		
		-- 停止动画
		MainDlgWorkerStop( type+1 )
		
		uiWorker.transform:Find("NormalShape").gameObject:SetActive(true);
		uiWorker.transform:Find("NormalShape").transform.localScale = Vector3.one;
		uiWorker.transform:Find("WorkProgress").gameObject:SetActive(false);
		uiWorker.transform:Find("BuildingShape").gameObject:SetActive(false);
		uiWorker.transform:Find("BuildingTimer").gameObject:SetActive(false);
		uiWorker.transform:Find("BuildingName").gameObject:SetActive(false);
		
		return;
	end
	
	-- 建筑名
	local name = T(kind);
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		name = (offset%16+1).."号"..T(kind);		
	end
		
	-- 建筑形象
	local BuildingShape = uiWorker.transform:Find("BuildingShape");
	BuildingShape:GetComponent( "Image" ).sprite = BuildingSprite( kind );
	BuildingShape.gameObject:SetActive(true);
	
	-- 建筑名称
	local BuildingName = uiWorker.transform:Find("BuildingName");
	BuildingName:GetComponent( "UIText" ).text = name;
	BuildingName.gameObject:SetActive(false);
	BuildingName:GetComponent("UITweenScale"):Play(true);
	
	-- 建筑计时器
	local BuildingTimer = uiWorker.transform:Find("BuildingTimer");
	BuildingTimer.gameObject:SetActive(true);
	
	-- 计时器
	local timer = BuildingTimer:GetComponent( "UITextTimeCountdown" );
	timer:SetTime( needsec, needsec-sec );
	
	-- 进度条
	uiWorker.transform:Find("WorkProgress").gameObject:SetActive(true);
	
	-- 缺省锤子形象
	local NormalShape = uiWorker.transform:Find("NormalShape");
	NormalShape.gameObject:SetActive(false);
	NormalShape:GetComponent("UITweenScale"):Play(true);
	
	-- 缺省文字
	uiWorker.transform:Find("NormalText").gameObject:SetActive(false);
	
end

-- 停止动画
function MainDlgWorkerStop( type )
	m_uiBuildingShapeUITweenScale[type]:Kill(false);
	m_uiNormalShapeUITweenScale[type]:Kill(false);
	m_uiBuildingTimerUITweenScale[type]:Kill(false);
	m_uiBuildingNameUITweenScale[type]:Kill(false);
	
	m_uiBuildingShapeUITweenScale1[type]:Kill(false);
	m_uiNormalShapeUITweenScale1[type]:Kill(false);
	m_uiBuildingTimerUITweenScale1[type]:Kill(false);
	m_uiBuildingNameUITweenScale1[type]:Kill(false);
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

ButtonMap = {
[CITY_FUNCTION_NATION] = { offset = 3 },
[CITY_FUNCTION_STORY] = { offset = 5 },
[CITY_FUNCTION_MAIL] = { offset = 6 },
[CITY_FUNCTION_FRIEND] = { offset = 12 },
[CITY_FUNCTION_NATIONEQUIP] = { offset = 13 },
[CITY_FUNCTION_RANK] = { offset = 14 },
}

function MainDlgGetEmptyButton()
	for i=1, 24, 1 do
		if m_hasButton[i] == false and m_uiButtonBack[i] ~= nil then
			return i, m_uiButtonBack[i];
		end
	end
	return -1, nil;
end

-- 设置获得的功能按钮
function MainDlgSetButtons()
	for i=1, 24, 1 do
		m_hasButton[i] = false;
	end
--[[	SetParent( m_uiButtonHero = objs[32];
	SetParent( m_uiButtonQuest = objs[33];
	SetParent( m_uiButtonNation = objs[34];
	SetParent( m_uiButtonWorld = objs[35];
	SetParent( m_uiButtonCity = objs[36];
	SetParent( m_uiButtonStory = objs[37];
	SetParent( m_uiButtonMail = objs[38];
	SetParent( m_uiButtonMore = objs[39];
	SetParent( m_uiButtonBag = objs[40];
	SetParent( m_uiButtonFriend = objs[41];
	SetParent( m_uiButtonNationEquip = objs[42];
	SetParent( m_uiButtonRank = objs[43];
	SetParent( m_uiButtonNotice = objs[44];
	SetParent( m_uiButtonSetting = objs[45];
	SetParent( m_uiButtonGM = objs[46];
	SetParent( m_uiButtonRelogin = objs[47];--]]--]]
	
	
	-- 英雄
	SetParent( m_uiButtonHero, m_uiButtonBack[1] );
	m_hasButton[1] = true;
	
	-- 任务
	SetParent( m_uiButtonQuest, m_uiButtonBack[2] );
	m_hasButton[2] = true;
	
	-- 世界
	SetParent( m_uiButtonWorld, m_uiButtonBack[4] );
	SetParent( m_uiButtonCity, m_uiButtonBack[4] );
	SetFalse( m_uiButtonWorld );
	SetFalse( m_uiButtonCity );
	m_hasButton[4] = true;
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_WORLD ) == 1 then
		SetTrue( m_uiButtonWorld );
	end
	
	-- 更多
	SetParent( m_uiButtonMore, m_uiButtonBack[7] );
	m_hasButton[7] = true;
	
	-- 国家
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_NATION ) == 1 then
		local offset, root = MainDlgGetEmptyButton();
		if root ~= nil then
			SetParent( m_uiButtonNation, m_uiButtonBack[offset] );
			m_hasButton[offset] = true;
		end
	end
	
	-- 副本
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_STORY ) == 1 then
		local offset, root = MainDlgGetEmptyButton();
		if root ~= nil then
			SetParent( m_uiButtonStory, m_uiButtonBack[offset] );
			m_hasButton[offset] = true;
		end
	end
		
	-- 邮件
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_MAIL ) == 1 then
		local offset, root = MainDlgGetEmptyButton();
		if root ~= nil then
			SetParent( m_uiButtonMail, m_uiButtonBack[offset] );
			m_hasButton[offset] = true;
		end
	end	

	-- 背包
	local offset, root = MainDlgGetEmptyButton();
	if root ~= nil then
		SetParent( m_uiButtonBag, m_uiButtonBack[offset] );
		m_hasButton[offset] = true;
	end
	
	-- 好友
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_FRIEND ) == 1 then
		local offset, root = MainDlgGetEmptyButton();
		if root ~= nil then
			SetParent( m_uiButtonFriend, m_uiButtonBack[offset] );
			m_hasButton[offset] = true;
		end
	end	
	
	-- 国器
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_NATIONEQUIP ) == 1 then
		local offset, root = MainDlgGetEmptyButton();
		if root ~= nil then
			SetParent( m_uiButtonNationEquip, m_uiButtonBack[offset] );
			m_hasButton[offset] = true;
		end
	end	
	
	-- 排行榜
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_RANK ) == 1 then
		local offset, root = MainDlgGetEmptyButton();
		if root ~= nil then
			SetParent( m_uiButtonRank, m_uiButtonBack[offset] );
			m_hasButton[offset] = true;
		end
	end	
	
	-- 系统公告
	local offset, root = MainDlgGetEmptyButton();
	if root ~= nil then
		SetParent( m_uiButtonNotice, m_uiButtonBack[offset] );
		m_hasButton[offset] = true;
	end
	
	-- 设置
	local offset, root = MainDlgGetEmptyButton();
	if root ~= nil then
		SetParent( m_uiButtonSetting, m_uiButtonBack[offset] );
		m_hasButton[offset] = true;
	end
	
	-- 联系客服
	local offset, root = MainDlgGetEmptyButton();
	if root ~= nil then
		SetParent( m_uiButtonGM, m_uiButtonBack[offset] );
		m_hasButton[offset] = true;
	end
	
	-- 返回登陆
	local offset, root = MainDlgGetEmptyButton();
	if root ~= nil then
		SetParent( m_uiButtonRelogin, m_uiButtonBack[offset] );
		m_hasButton[offset] = true;
	end
	
	-- 重新创角
	local offset, root = MainDlgGetEmptyButton();
	if root ~= nil then
		SetParent( m_uiButtonRestart, m_uiButtonBack[offset] );
		m_hasButton[offset] = true;
	end
end

-- 获得功能按钮特效
function MainDlgButtonGetEffect( offset )
	
end

-- 显示内城的控件
function MainDlgShowCity()
	SetTrue( m_uiTop_City )
	SetFalse( m_uiTop_Map )
	SetTrue(m_uiButtonWorld )
	SetFalse( m_uiButtonCity )
	MapMainDlgClose()
end

-- 显示外城的空间
function MainDlgShowMap()
	SetTrue( m_uiTop_Map )
	SetFalse( m_uiTop_City )
	SetFalse(m_uiButtonWorld )
	SetTrue( m_uiButtonCity )
	MapMainDlgShow()
	SetText( m_uiNationName, NationEx( GetPlayer().m_nation ) );
end

-- 天气
function MainDlgSetWeather( game_day, game_weather )
	g_game_day = game_day;
	g_game_weather = game_weather;
	if g_game_day <= 0 then
		SetText( m_uiWeatherDay, "" )
		SetFalse( m_uiWeatherIcon )
		SetText( m_uiWeatherAbility, "" )
	else
		SetText( m_uiWeatherDay, T(g_weather[g_game_day][g_game_weather].nameid) )
		SetTrue( m_uiWeatherIcon )
		SetImage( m_uiWeatherIcon, LoadSprite( "ui_mapmain_weather_"..(game_weather+1) ) )
		SetText( m_uiWeatherAbility, T(g_weather[g_game_day][g_game_weather].descid) )
	end
end

-- 设置区域名称
function MainDlgSetZoneName( name )
	SetText( m_uiZoneName, name )
end

-- 小地图
function MainDlgMiniMapInit()
	for j=1, 5, 1 do
		for i=1, 5, 1 do
			local tmx = GameObject.Instantiate( m_uiMiniMap.m_uiUIP_MiniTmx );
			tmx.transform:SetParent( m_uiMiniMap.m_uiMoveLayer.transform );
			tmx.transform.localScale = Vector3.one;
			tmx.transform.localPosition = Vector3.New( (WorldMap.m_nMaxWidth*1024)/2/100 + (i-1)*TMX_WIDTH*1024/2/100 - (j-1)*TMX_WIDTH*1024/2/100,
												   -(i-1)*TMX_HEIGHT*512/2/100 - (j-1)*TMX_HEIGHT*512/2/100, 0 );
			SetTrue( tmx )
		end
	end
	-- 对象池
	m_uiMiniMap.m_ObjectPool = m_uiMiniMap.m_uiMoveLayer.transform.parent:GetComponent( typeof(ObjectPoolManager) );
	m_uiMiniMap.m_ObjectPool:CreatePool("m_uiUIP_ZoneUnit", 100, 100, m_uiMiniMap.m_uiUIP_ZoneUnit);
end
function MainDlgMiniMapChangeZone()
	MainDlgMiniMapClearUnit()
	--m_uiMoveLayer
	--m_uiMiniMap.m_uiMoveLayer.transform:Find( "MapBorder" ):GetComponent("MapBorder"):SetSize( 110 );
end
function MainDlgMiniMapMove()

end
function MainDlgMiniMapClearUnit()
	local objs = {};
	for i = 0 ,m_uiMiniMap.m_uiMoveLayer.transform.childCount - 1 do
		table.insert( objs, m_uiMiniMap.m_uiMoveLayer.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "m_uiUIP_ZoneUnit(Clone)" then
			m_ObjectPool:Release( "m_uiUIP_ZoneUnit", obj );
		end
    end
end
-- {m_posx=0,m_posy=0,m_nation=0,m_level=0,[m_count]}
function MainDlgMiniMapAddUnit( recvValue )
	--print( recvValue.m_posx..","..recvValue.m_posy )
	local uiObj = m_uiMiniMap.m_ObjectPool:Get( "m_uiUIP_ZoneUnit" );
	uiObj.transform:SetParent( m_uiMiniMap.m_uiMoveLayer.transform );
	uiObj.transform.localScale = Vector3.one;
	uiObj.transform.localPosition = Vector3.New( recvValue.m_posx, recvValue.m_posy )
end
