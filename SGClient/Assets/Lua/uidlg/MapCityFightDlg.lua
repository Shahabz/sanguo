-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiMarchTime1 = nil; --UnityEngine.GameObject
local m_uiMarchTime2 = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Type1 = nil; --UnityEngine.GameObject
local m_uiUIP_Type2 = nil; --UnityEngine.GameObject
local m_uiUIP_Type3 = nil; --UnityEngine.GameObject
local m_uiWarn = nil; --UnityEngine.GameObject

local m_recvValue = nil
local m_canSelect = { 0,0,0 }

-- 打开界面
function MapCityFightDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_mapbattledlg" );
	m_Dlg = eye.uiManager:Open( "MapCityFightDlg" );
end

-- 隐藏界面
function MapCityFightDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MapCityFightDlg" );
end

-- 删除界面
function MapCityFightDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	ResourceManager.UnloadAssetBundle( "_ab_ui_mapbattledlg" );
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapCityFightDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapCityFightDlgClose();
		elseif nControlID >= 1 and nControlID <= 3 then
			MapCityFightDlgSelect( nControlID );
        end
	end
end

-- 载入时调用
function MapCityFightDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiName = objs[1];
	m_uiMarchTime1 = objs[2];
	m_uiMarchTime2 = objs[3];
	m_uiContent = objs[4];
	m_uiUIP_Type1 = objs[5];
	m_uiUIP_Type2 = objs[6];
	m_uiUIP_Type3 = objs[7];
	m_uiWarn = objs[8];
end

-- 界面初始化时调用
function MapCityFightDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapCityFightDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapCityFightDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapCityFightDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapCityFightDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapCityFightDlgShow( recvValue )
	MapCityFightDlgOpen()
	m_canSelect = { 0,0,0 }
	
	m_recvValue 	= recvValue;
	local state 	= recvValue.m_state;
	local name 		= recvValue.m_name;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local level 	= recvValue.m_char_value[1];
	local nation	= recvValue.m_char_value[2];
	local ptbuff	= recvValue.m_char_value[3];
	local unit_index = recvValue.m_unit_index;
	
	-- 形象
    m_uiShape:GetComponent("SpriteRenderer").sprite = LoadSprite( MapUnitCityShapeList[level].."_"..nation );
			
	-- 名字
	SetText( m_uiName, F(1228, level, name, posx, posy) )
	
	-- 行军时间
	m_marchtime = WorldMap.MarchTime( WorldMap.m_nMyCityPosx, WorldMap.m_nMyCityPosy, posx, posy )
	SetText( m_uiMarchTime1, F(953, secnum(m_marchtime) ) )
	SetText( m_uiMarchTime2, secnum(m_marchtime) )
	
	-- 官员减体力
	local subbody = 0;
	if GetPlayer().m_official > 0 and GetPlayer().m_official <= 6 then
		subbody = g_nation_official[GetPlayer().m_official].cityfight_body;
	end
				
	-- 城战令
	local itemkind = 486
	local itemnum = GetItem():GetCount( itemkind )
				
	-- 行军时间≤5分钟可使用闪电战
	if m_marchtime <= global.cityfight_sec_limit1 then
		SetTrue( m_uiUIP_Type1.transform:Find("MarchTime") )
		SetFalse( m_uiUIP_Type1.transform:Find("MarchWarn") )
		SetTrue( m_uiUIP_Type1.transform:Find("BodyIcon") )
		SetTrue( m_uiUIP_Type1.transform:Find("BodyText") )
		
		local costbody = global.cityfight_body_cost1 - subbody
		if global.cityfight_body_cost1 - subbody > 0 then
			-- 有城战令
			if itemnum >= global.cityfight_item_cost1 then
				SetText( m_uiUIP_Type1.transform:Find("BodyText"), item_getname(itemkind).."x"..global.cityfight_item_cost1 )
			else
				SetText( m_uiUIP_Type1.transform:Find("BodyText"), T(126).."x"..global.cityfight_body_cost1 )
			end
		else
			if costbody < 0 then
				costbody = 0
			end
			SetText( m_uiUIP_Type1.transform:Find("BodyText"), T(126).."x"..costbody )
		end
		SetFalse( m_uiUIP_Type1.transform:Find("BodyWarn") )
		SetText( m_uiUIP_Type1.transform:Find("MarchTime"), T(1251).." "..secnum_min( m_marchtime ) )
		SetButtonTrue( m_uiUIP_Type1.transform:Find("SelectButton") )
		m_canSelect[1] = 1;
	else
		SetFalse( m_uiUIP_Type1.transform:Find("MarchTime") )
		SetTrue( m_uiUIP_Type1.transform:Find("MarchWarn") )
		SetFalse( m_uiUIP_Type1.transform:Find("BodyIcon") )
		SetFalse( m_uiUIP_Type1.transform:Find("BodyText") )
		SetTrue( m_uiUIP_Type1.transform:Find("BodyWarn") )
		SetButtonFalse( m_uiUIP_Type1.transform:Find("SelectButton") )
	end
	
	-- 行军时间≤10分钟可使用奔袭战
	if m_marchtime <= global.cityfight_sec_limit2 then
		SetTrue( m_uiUIP_Type2.transform:Find("MarchTime") )
		SetFalse( m_uiUIP_Type2.transform:Find("MarchWarn") )
		SetTrue( m_uiUIP_Type2.transform:Find("BodyIcon") )
		SetTrue( m_uiUIP_Type2.transform:Find("BodyText") )
		
		local costbody = global.cityfight_body_cost2 - subbody
		if costbody > 0 then
			-- 有城战令
			if itemnum >= global.cityfight_item_cost2 then
				SetText( m_uiUIP_Type2.transform:Find("BodyText"), item_getname(itemkind).."x"..global.cityfight_item_cost2 )
			else
				SetText( m_uiUIP_Type2.transform:Find("BodyText"), T(126).."x"..global.cityfight_body_cost2 )
			end
		else
			if costbody < 0 then
				costbody = 0
			end
			SetText( m_uiUIP_Type2.transform:Find("BodyText"), T(126).."x"..costbody )
		end
		SetFalse( m_uiUIP_Type2.transform:Find("BodyWarn") )
		if m_marchtime < global.cityfight_sec_group2 then
			SetText( m_uiUIP_Type2.transform:Find("MarchTime"), T(1251).." "..secnum_min( m_marchtime + global.cityfight_sec_group2 ) )
		else
			SetText( m_uiUIP_Type2.transform:Find("MarchTime"), T(1251).." "..secnum_min( m_marchtime ) )
		end
		SetButtonTrue( m_uiUIP_Type2.transform:Find("SelectButton") )
		m_canSelect[2] = 1;
	else
		SetFalse( m_uiUIP_Type2.transform:Find("MarchTime") )
		SetTrue( m_uiUIP_Type2.transform:Find("MarchWarn") )
		SetFalse( m_uiUIP_Type2.transform:Find("BodyIcon") )
		SetFalse( m_uiUIP_Type2.transform:Find("BodyText") )
		SetTrue( m_uiUIP_Type2.transform:Find("BodyWarn") )
		SetButtonFalse( m_uiUIP_Type2.transform:Find("SelectButton") )
	end
	
	-- 远征军没有时间限制
	if m_marchtime <= global.cityfight_sec_limit3 then
		SetTrue( m_uiUIP_Type3.transform:Find("MarchTime") )
		SetFalse( m_uiUIP_Type3.transform:Find("MarchWarn") )
		SetTrue( m_uiUIP_Type3.transform:Find("BodyIcon") )
		SetTrue( m_uiUIP_Type3.transform:Find("BodyText") )
		
		local costbody = global.cityfight_body_cost3 - subbody
		if costbody > 0 then
			-- 有城战令
			if itemnum >= global.cityfight_item_cost3 then
				SetText( m_uiUIP_Type3.transform:Find("BodyText"), item_getname(itemkind).."x"..global.cityfight_item_cost3 )
			else
				SetText( m_uiUIP_Type3.transform:Find("BodyText"), T(126).."x"..global.cityfight_body_cost3 )
			end
		else
			if costbody < 0 then
				costbody = 0
			end
			SetText( m_uiUIP_Type3.transform:Find("BodyText"), T(126).."x"..costbody )
		end
		
		SetFalse( m_uiUIP_Type3.transform:Find("BodyWarn") )
		if m_marchtime < global.cityfight_sec_group3 then
			SetText( m_uiUIP_Type3.transform:Find("MarchTime"), T(1251).." "..secnum_min( m_marchtime + global.cityfight_sec_group3 ) )
		else
			SetText( m_uiUIP_Type3.transform:Find("MarchTime"), T(1251).." "..secnum_min( m_marchtime ) )
		end
		SetButtonTrue( m_uiUIP_Type3.transform:Find("SelectButton") )
		m_canSelect[3] = 1;
	else
		SetFalse( m_uiUIP_Type3.transform:Find("MarchTime") )
		SetTrue( m_uiUIP_Type3.transform:Find("MarchWarn") )
		SetFalse( m_uiUIP_Type3.transform:Find("BodyIcon") )
		SetFalse( m_uiUIP_Type3.transform:Find("BodyText") )
		SetTrue( m_uiUIP_Type3.transform:Find("BodyWarn") )
		SetButtonFalse( m_uiUIP_Type3.transform:Find("SelectButton") )
	end
	
end

-- 选择模式
function MapCityFightDlgSelect( type )
	if m_canSelect[type] == 0 then
		return
	end
	
	-- 城战令
	local itemkind = 486
	local itemnum = GetItem():GetCount( itemkind )
	if type == 1 then
		if itemnum < global.cityfight_item_cost1 then
			if GetPlayer().m_body < global.cityfight_body_cost1 then
				JumpBody()
				return
			end
		end
		
	elseif type == 2 then
		if itemnum < global.cityfight_item_cost2 then
			if GetPlayer().m_body < global.cityfight_body_cost2 then
				JumpBody()
				return
			end
		end
		
	elseif type == 3 then
		if itemnum < global.cityfight_item_cost3 then
			if GetPlayer().m_body < global.cityfight_body_cost3 then
				JumpBody()
				return
			end
		end
	end
	
	MapCityFightDlgClose()
	MapBattleDlgShow( m_recvValue, ARMY_ACTION_GROUP_CREATE, -1 )
	MapBattleDlgAppData( type )
end

