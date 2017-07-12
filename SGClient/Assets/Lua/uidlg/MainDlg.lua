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
local m_bMorePanel = false;

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
            MainDlgClose();

		elseif nControlID == 1 then
			HeroDlgShow();
			
		elseif nControlID == 2 then
			local kind = 21;
			local offsetlist ={ 49, 50, 51, 52 }
			for i=1, 2, 1 do
				local offset = math.random( 0, 63 )
				--local offset = offsetlist[i]
				BuildingGetDlgShow( {m_kind=kind,m_offset=offset,m_level=1} );
			end
		elseif nControlID == 3 then
			QuestAwardDlgShow();
		
		elseif nControlID == 5 then
			NpcTalk( "主公，大事不妙" )
			NpcTalkWait( BuildingGetDlgShow, {m_kind=21,m_offset=0,m_level=1} )
		
		elseif nControlID == 6 then
			--{m_kind=0,m_color=0,m_level=0,m_corps=0,m_exp=0,m_exp_max=0,m_soldiers=0,m_state=0,m_attack_base=0,m_attack_wash=0,m_defense_base=0,m_defense_wash=0,m_troops_base=0,m_troops_wash=0,m_attack=0,m_defense=0,m_troops=0,m_offset=0,},
			HeroGetDlgShow( { m_kind = 1, m_color=0, m_corps=0,m_soldiers=0,m_state=0,m_attack_base=0,m_attack_wash=0,m_defense_base=0,m_defense_wash=0,m_troops_base=0,m_troops_wash=0,m_attack=0,m_defense=0,m_troops=0,m_offset=0 } )
	
		elseif nControlID == 7 then
			NpcTalkOne( T(10001), T(10002), nil )
		
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
	m_uiHead:GetComponent( "Image" ).sprite = PlayerHeadSprite( GetPlayer().m_shape );
end

-- 等级
function MainDlgSetLevel()
	m_uiLevel:GetComponent( "UIText" ).text = "Lv."..GetPlayer().m_level;
end

-- 银币
function MainDlgSetSilver()
	m_uiSilverNum:GetComponent( "UIText" ).text = GetPlayer().m_silver;
end

-- 木材
function MainDlgSetWood()
	m_uiWoodNum:GetComponent( "UIText" ).text = GetPlayer().m_wood;
end

-- 粮草
function MainDlgSetFood()
	m_uiFoodNum:GetComponent( "UIText" ).text = GetPlayer().m_food;
end

-- 镔铁
function MainDlgSetIron()
	m_uiIronNum:GetComponent( "UIText" ).text = GetPlayer().m_iron;
end

-- 体力
function MainDlgSetBody()
	m_uiBodyNum:GetComponent( "UIText" ).text = GetPlayer().m_body;
end

-- 步兵
function MainDlgSetInfantry()
	m_uiInfantryNum:GetComponent( "UIText" ).text = GetPlayer().m_infantry_num;
end

-- 骑兵
function MainDlgSetCavalry()
	m_uiCavalryNum:GetComponent( "UIText" ).text = GetPlayer().m_cavalry_num;
end

-- 弓兵
function MainDlgSetArcher()
	m_uiArcherNum:GetComponent( "UIText" ).text = GetPlayer().m_archer_num;
end

-- 钻石
function MainDlgSetToken()
	m_uiTokenNum:GetComponent( "UIText" ).text = GetPlayer().m_token;
end

-- VIP
function MainDlgSetVipLevel()
	m_uiVipLevel:GetComponent( "UIText" ).text = "VIP "..GetPlayer().m_viplevel;
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
	
	-- 弹出改名写死
	if CacheQuest.m_list[1].m_questid == 4 then
		if NpcTalkIsShow() == true then
			NpcTalkWait( ChangeNameDlgShow, nil )
		else
			ChangeNameDlgShow();
		end
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
			NormalText:GetComponent( "UIText" ).text = "未解锁";
		else
			NormalText:GetComponent( "UIText" ).text = "空闲";
		end
		
		-- 缺省锤子形象
		local NormalShape = uiWorker.transform:Find("NormalShape");
		NormalShape.gameObject:SetActive(true);
		
		uiWorker.transform:Find("BuildingShape").gameObject:SetActive(false);
		uiWorker.transform:Find("BuildingTimer").gameObject:SetActive(false);
		uiWorker.transform:Find("BuildingName").gameObject:SetActive(false);
		
		-- 停止动画
		MainDlgWorkerStop( type+1 )
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
	
	-- 缺省锤子形象
	local NormalShape = uiWorker.transform:Find("NormalShape");
	NormalShape.gameObject:SetActive(false);
	NormalShape:GetComponent("UITweenScale"):Play(true);
	
	-- 缺省文字
	uiWorker.transform:Find("NormalText").gameObject:SetActive(false);
	
end

-- 停止动画
function MainDlgWorkerStop( type )
	m_uiBuildingShapeUITweenScale[type]:Kill(true);
	m_uiBuildingShapeUITweenScale[type]:Kill(true);
	m_uiBuildingTimerUITweenScale[type]:Kill(true);
	m_uiBuildingNameUITweenScale[type]:Kill(true);
	m_uiBuildingShapeUITweenScale1[type]:Kill(true);
	m_uiNormalShapeUITweenScale1[type]:Kill(true);
	m_uiBuildingTimerUITweenScale1[type]:Kill(true);
	m_uiBuildingNameUITweenScale1[type]:Kill(true);
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

