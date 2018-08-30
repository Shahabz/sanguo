-- 界面
local m_Dlg = nil;
--local m_DialogFrameMod = nil;
local m_uiQuestLayer = nil; --UnityEngine.GameObject
local m_uiQuestScroll = nil; --UnityEngine.GameObject
local m_uiQuestContent = nil; --UnityEngine.GameObject
local m_uiUIP_Quest = nil; --UnityEngine.GameObject
local m_uiShopLayer = nil; --UnityEngine.GameObject
local m_uiShopScroll = nil; --UnityEngine.GameObject
local m_uiShopContent = nil; --UnityEngine.GameObject
local m_uiUIP_Shop = nil; --UnityEngine.GameObject
local m_uiAwardDescLayer = nil; --UnityEngine.GameObject

local m_QuestRecvValue = nil
local m_ShopRecvValue = nil

-- 打开界面
function EveryDayQuestDlgOpen()
	m_Dlg = eye.uiManager:Open( "EveryDayQuestDlg" );
	--m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(0), nil, EveryDayQuestDlgClose );
end

-- 隐藏界面
function EveryDayQuestDlgClose()
	if m_Dlg == nil then
		return;
	end
	--DialogFrameModClose( m_DialogFrameMod );
	--m_DialogFrameMod = nil;
	eye.uiManager:Close( "EveryDayQuestDlg" );
	m_QuestRecvValue = nil
	m_ShopRecvValue = nil
end

-- 删除界面
function EveryDayQuestDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function EveryDayQuestDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            EveryDayQuestDlgClose();
		elseif nControlID == 1 then
			EveryDayQuestDlgQuestShow()
		elseif nControlID == 2 then
			EveryDayQuestDlgShopShow()
		
		-- 领取
		elseif nControlID >= 1000 and nControlID < 2000 then
			EveryDayQuestDlgQuestAward( nControlID-1000 )
		
		-- 前往
		elseif nControlID >= 2000 and nControlID < 3000 then
			EveryDayQuestDlgQuestGoto( nControlID-2000 )
        end
	end
end

-- 载入时调用
function EveryDayQuestDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiQuestLayer = objs[0];
	m_uiQuestScroll = objs[1];
	m_uiQuestContent = objs[2];
	m_uiUIP_Quest = objs[3];
	m_uiShopLayer = objs[4];
	m_uiShopScroll = objs[5];
	m_uiShopContent = objs[6];
	m_uiUIP_Shop = objs[7];
	m_uiAwardDescLayer = objs[8];
	
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("m_uiUIP_Quest", 16, 16, m_uiUIP_Quest);
	m_ObjectPool:CreatePool("m_uiUIP_Shop", 6, 6, m_uiUIP_Shop);
end

-- 界面初始化时调用
function EveryDayQuestDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function EveryDayQuestDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function EveryDayQuestDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function EveryDayQuestDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function EveryDayQuestDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function EveryDayQuestDlgShow()
	EveryDayQuestDlgOpen()
	system_askinfo( ASKINFO_EVERYDAY_QUEST, 0 )
end

-- 任务
function EveryDayQuestDlgQuestShow()
	SetTrue( m_uiQuestLayer )
	SetFalse( m_uiShopLayer )
end
-- m_count=0,m_list={m_id=0,m_textid=0,m_value=0,m_needvalue=0,m_sort=0,m_awardkind={[2]},m_awardnum={[2]},[m_count]},m_mypoint=0,
function EveryDayQuestDlgQuestRecv( recvValue )
	m_QuestRecvValue = recvValue	
	EveryDayQuestDlgQuestSet()
	EveryDayQuestDlgQuestShow()
end
function EveryDayQuestDlgQuestSet()
	if m_QuestRecvValue == nil then
		return
	end
	EveryDayQuestDlgQuestClear()
	-- 先排序
	local tmptable = {}
	for i=1, m_QuestRecvValue.m_count, 1 do
		local isget = m_QuestRecvValue.m_list[i].m_isget
		local value = m_QuestRecvValue.m_list[i].m_value
		local needvalue = m_QuestRecvValue.m_list[i].m_needvalue
		local rank = 0;
		if value >= needvalue then
			if isget == 1 then-- 完成已经领取
				rank = -1000 - m_QuestRecvValue.m_list[i].m_id
			else -- 完成未领取
				rank = 0
			end
		else
			rank = -m_QuestRecvValue.m_list[i].m_id
		end
		table.insert( tmptable, {rank=rank, info=m_QuestRecvValue.m_list[i]} )
	end
	table.sort( tmptable, function(a,b) 
		if a.rank > b.rank then
			return true
		else
			return false
		end
	end )
	-- 创建
	for i=1, #tmptable, 1 do
		local info = tmptable[i].info
			
		local uiObj = m_ObjectPool:Get( "m_uiUIP_Quest" );
		uiObj.transform:SetParent( m_uiQuestContent.transform );
		uiObj.transform.localScale = Vector3.one;
		uiObj.gameObject:SetActive( true );
		
		local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiName = objs[0]
		local uiAwardList = objs[1]
		local uiStateBtn = objs[2]
		local uiState = objs[3]
		local uiGotoBtn = objs[4]
		
		SetControlID( uiStateBtn, 1000+info.m_id )
		SetControlID( uiGotoBtn, 2000+info.m_id )
		
		if info.m_value > info.m_needvalue then
			SetText( uiName, FQUEST(info.m_textid, info.m_needvalue, info.m_needvalue) )
		else
			SetText( uiName, FQUEST(info.m_textid, info.m_value, info.m_needvalue) )
		end
		
		if info.m_value >= info.m_needvalue then
			if info.m_isget == 0 then
				SetTrue( uiStateBtn )
				SetFalse( uiGotoBtn )
				SetFalse( uiState )
			else
				SetTrue( uiState )
				SetFalse( uiGotoBtn )
				SetFalse( uiStateBtn )
			end 
		else
			SetTrue( uiGotoBtn )
			SetFalse( uiStateBtn )
			SetFalse( uiState )
		end
		
		-- 奖励
		local awardkind = { info.awardkind0, info.awardkind1, info.awardkind2 }
		local awardnum = { info.awardnum0, info.awardnum1, info.awardnum2 }
		for i=1, 2, 1 do
			local awardObj = uiAwardList.transform:GetChild(i-1);
			local awardkind = info.m_awardkind[i]
			local awardnum = info.m_awardnum[i]
			if awardkind > 0 then
				local sprite, color, name, c = AwardInfo( awardkind )
				SetTrue( awardObj )
				SetControlID( awardObj, 1000000+awardkind )
				SetImage( awardObj.transform:Find("Shape"), sprite );
				SetText( awardObj.transform:Find("Name"), name.."x"..awardnum, NameColor(c) );
			else
				SetFalse( awardObj )
			end
		end
		
	end
end
function EveryDayQuestDlgQuestClear()
	local objs = {};
	for i=0,m_uiQuestContent.transform.childCount-1 do
		table.insert(objs,m_uiQuestContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "m_uiUIP_Quest(Clone)" then
			m_ObjectPool:Release( "m_uiUIP_Quest", obj );
		end
	end
end

-- 商店
function EveryDayQuestDlgShopShow()
	SetFalse( m_uiQuestLayer )
	SetTrue( m_uiShopLayer )
	EveryDayQuestDlgShopSet()
end
-- m_count=0,m_list={m_id=0,m_awardkind=0,m_awardnum=0,m_point=0,[m_count]},
function EveryDayQuestDlgShopRecv()
	m_QuestRecvValue = recvValue
end
function EveryDayQuestDlgShopSet()
	if m_ShopRecvValue == nil then
		return
	end
	EveryDayQuestDlgShopClear()
	
end
function EveryDayQuestDlgShopClear()
	local objs = {};
	for i=0,m_uiShopContent.transform.childCount-1 do
		table.insert(objs,m_uiShopContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "m_uiUIP_Shop(Clone)" then
			m_ObjectPool:Release( "m_uiUIP_Shop", obj );
		end
	end
end
function EveryDayQuestDlgUpdatePoint()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
end

-- 领取
function EveryDayQuestDlgQuestAward( id )
	system_askinfo( ASKINFO_EVERYDAY_QUEST, 1, id )
end

-- 前往
function EveryDayQuestDlgQuestGoto( id )
	if id == 1 then
		g_QuestTargetBuildingOprator = 3
		if GetPlayer():BuildingLevel( BUILDING_Infantry, -1 ) > 0 then
			EveryDayQuestDlgClose()
			City.Move( BUILDING_Infantry, -1, true );
		else
			pop(T(579))
		end
		
	elseif id == 2 then
		g_QuestTargetBuildingOprator = 3
		if GetPlayer():BuildingLevel( BUILDING_Cavalry, -1 ) > 0 then
			EveryDayQuestDlgClose()
			City.Move( BUILDING_Cavalry, -1, true );
		else
			pop(T(579))
		end
		
	elseif id == 3 then	
		g_QuestTargetBuildingOprator = 3
		if GetPlayer():BuildingLevel( BUILDING_Archer, -1 ) > 0 then
			EveryDayQuestDlgClose()
			City.Move( BUILDING_Archer, -1, true );
		else
			pop(T(579))
		end
	elseif id >= 4 and id <= 11 or id == 19 or id == 20 or id == 21 then
		if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_WORLD ) == 1 then
			if GameManager.currentScence == "city" then
				EveryDayQuestDlgClose()
				WorldMap.GotoWorldMap(-1, -1)
			end
		else
			pop(T(579))
		end
		
	elseif id == 16 and id <= 17 then
		EveryDayQuestDlgClose()
		City.Move( BUILDING_Main, 0, true )
		
	elseif id == 18 then
		local unitObj = City.GetBuilding( BUILDING_Wishing, -1 )
		if unitObj then
			EveryDayQuestDlgClose()
			City.Move( BUILDING_Wishing, -1, true );
		else
			pop(T(579))
		end
		
	elseif id == 22 then
		local unitObj = City.GetBuilding( BUILDING_Wash, -1 )
		if unitObj then
			EveryDayQuestDlgClose()
			City.Move( BUILDING_Wash, -1, true );
		else
			pop(T(579))
		end
		
	elseif id == 23 then
		local unitObj = City.GetBuilding( BUILDING_Hero, -1 )
		if unitObj then
			EveryDayQuestDlgClose()	
			City.Move( BUILDING_Hero, -1, true );
		else
			pop(T(579))
		end
		
	elseif id == 24 then
		if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_NATION ) == 1 then
			EveryDayQuestDlgClose()	
			NationDlgShow()
		else
			pop(T(579))
		end
		
	elseif id == 25 then	
			EveryDayQuestDlgClose()	
	end
	
end