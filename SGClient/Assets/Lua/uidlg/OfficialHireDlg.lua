-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiText = nil; --UnityEngine.GameObject
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiUIP_Info = {nil,nil,nil}; --UnityEngine.GameObject
local m_DlgName = "";
local m_type = 0;

-- 打开界面
function OfficialHireDlgOpen()
	m_Dlg = eye.uiManager:Open( "OfficialHireDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, m_DlgName, HELP_LevyDlg, OfficialHireDlgClose );
end

-- 隐藏界面
function OfficialHireDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "OfficialHireDlg" );
end

-- 删除界面
function OfficialHireDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function OfficialHireDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            OfficialHireDlgClose();
		
		-- 购买
		elseif nControlID > 0 and nControlID < 100 then
			OfficialHireDlgBuy( nControlID )
	
        end
	end
end

-- 载入时调用
function OfficialHireDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiText = objs[0];
	m_uiScroll = objs[1];
	m_uiUIP_Info[1] = objs[2];
	m_uiUIP_Info[2] = objs[3];
	m_uiUIP_Info[3] = objs[4];
end

-- 界面初始化时调用
function OfficialHireDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function OfficialHireDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function OfficialHireDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function OfficialHireDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function OfficialHireDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function OfficialHireDlgShow( type )
	m_type = type;
	if type == 0 then
		-- 官府
		local pBuilding = GetPlayer():GetBuilding( BUILDING_Main, -1 );
		if pBuilding == nil then
			return;
		end
		if pBuilding.m_level < g_official_forging[1].buildinglevel then
			AlertMsg( F(725, BuildingName(BUILDING_Main), g_official_forging[1].buildinglevel )..T(721) )
			return
		end
		m_DlgName = T(721)..T(720)
	elseif type == 1 then
		-- 官府
		local pBuilding = GetPlayer():GetBuilding( BUILDING_Main, -1 );
		if pBuilding == nil then
			return;
		end
		if pBuilding.m_level < g_official_gov[1].buildinglevel then
			AlertMsg( F(725, BuildingName(BUILDING_Main), g_official_gov[1].buildinglevel )..T(722) )
			return
		end
		m_DlgName = T(722)..T(720)
	elseif type == 2 then
		-- 太学院
		local pBuilding = GetPlayer():GetBuilding( BUILDING_Tech, -1 );
		if pBuilding == nil then
			return;
		end
		if pBuilding.m_level < g_official_tech[1].buildinglevel then
			AlertMsg( F(725, BuildingName(BUILDING_Tech), g_official_tech[1].buildinglevel )..T(723) )
			return
		end
		m_DlgName = T(723)..T(720)
	end
	OfficialHireDlgOpen()
	OfficialHireDlgSet( type )
end

function OfficialHireDlgSet( type )
	if m_Dlg == nil then
		return;
	end
	SetFalse( m_uiUIP_Info[1] );
	SetFalse( m_uiUIP_Info[2] );
	SetFalse( m_uiUIP_Info[3] );
	if type == 0 then
		OfficialHireDlgSetForging()
	elseif type == 1 then
		OfficialHireDlgSetGov()
	elseif type == 2 then
		OfficialHireDlgSetTech()
	end
end

local function UIP_InfoSet( uiObj, kind, shape, color, name, desc, otherDesc, timerDesc, sec, title, titlecolor , res, silver, token, free, getfree )
	SetTrue(uiObj)
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiName = objs[2];
	local uiDesc = objs[3];
	local uiOtherDesc = objs[4];
	local uiResGroup = objs[5];
	local uiTimerDesc = objs[6];
	local uiTimer = objs[7];
	local uiTitle = objs[8];
	local uiButton = objs[9];
	local uiFreeButton = objs[10];
	local uiButtonIcon = objs[11];
	local uiButtonNum = objs[12];
	
	if shape == nil then
		SetFalse( uiShape )
	else
		SetTrue( uiShape )
		SetImage( uiShape, shape )
	end
	
	if color == nil then
		SetFalse( uiColor )
	else
		SetTrue( uiColor )
		SetImage( uiColor, color )
	end
	
	if name == nil then
		SetFalse( uiName )
	else
		SetTrue( uiName )
		SetText( uiName, name );
	end
	
	if desc == nil then
		SetFalse( uiDesc )
	else
		SetTrue( uiDesc )
		SetText( uiDesc, desc );
	end
	
	if otherDesc == nil then
		SetFalse( uiOtherDesc )
	else
		SetTrue( uiOtherDesc )
		SetText( uiOtherDesc, otherDesc );
	end
	
	if timerDesc == nil then
		SetFalse( uiTimerDesc )
	else
		SetTrue( uiTimerDesc )
		SetText( uiTimerDesc, timerDesc );
	end
	
	if sec == nil then
		SetFalse( uiTimer )
	else
		SetTrue( uiTimer )
		SetTimer( uiTimer, sec, sec, 1, T(702) );
	end

	if title == nil then
		SetFalse( uiTitle )
	else
		SetTrue( uiTitle )
		SetText( uiTitle, title , titlecolor );
	end
	
	if res == nil then
		SetFalse( uiResGroup )
	else
		SetTrue( uiResGroup )
		for i=1, 4, 1 do
			local obj = uiResGroup.transform:GetChild(i-1).gameObject;
			if res[i] == 1 then
				SetImage( obj.transform:Find("Icon"), LoadSprite("ui_cond_yes") )
			else
				SetImage( obj.transform:Find("Icon"), LoadSprite("ui_cond_no") )
			end
		end
	end
	
	if free == 1 and Utils.get_int_sflag( getfree, kind ) == 0 then
		SetFalse( uiButton )
		SetTrue( uiFreeButton )
		SetControlID( uiFreeButton, kind )
	else
		SetFalse( uiFreeButton )
		if silver <= 0 and token <= 0 then
			SetFalse( uiButton )
		else
			SetTrue( uiButton )
			SetControlID( uiButton, kind )
			if silver > 0 then
				SetImage( uiButtonIcon, LoadSprite("ui_icon_res_silver") );
				SetText( uiButtonNum, knum(silver) ) 
			else
				SetImage( uiButtonIcon, LoadSprite("ui_icon_res_token") );
				SetText( uiButtonNum, token ) 
			end
		end
	end
end

-- 铁匠
function OfficialHireDlgSetForging()
	if m_Dlg == nil then
		return;
	end
	SetText( m_uiText, T(715) )
	local info = GetPlayer().m_officialhire[0];
	-- 已雇佣
	local haslevel = 0
	if info.m_ofkind > 0 then
		local kind = info.m_ofkind;
		local ptr = g_official_forging[kind];
		haslevel = ptr.level
		UIP_InfoSet( m_uiUIP_Info[1], kind,
						OfSprite(ptr.shape), -- shape
						ItemColorSprite(ptr.color), -- color
						"Lv."..g_official_forging[kind].level.."  "..T(721), -- name
						F(769, zhtime(g_official_forging[kind].quick) ), -- desc
						nil,  -- otherDesc
						"\n\n"..T(771),  -- timerDesc
						info.m_ofsec, -- sec
						T(719)..T(721), -- title
						NameColor(3),	-- titlecolor
						nil, -- res
						0, -- silver
						0, -- token
						0, -- free
						0 ) -- getfree
						
		
	else
		-- 未雇佣
		UIP_InfoSet( m_uiUIP_Info[1], 0, nil, nil, T(728)..T(721), nil, nil, nil, nil, T(719)..T(721), NameColor(0), nil, 0, 0, 0, 0 )
	end
	
	-- 可雇佣
	local index = 2;
	local pBuilding = GetPlayer():GetBuilding( BUILDING_Main, -1 );
	if pBuilding == nil then
		return;
	end
	
	-- 找一个可雇佣的
	local cankind = 1;
	for kind=#g_official_forging, 1, -1 do
		local ptr = g_official_forging[kind];
		if pBuilding.m_level >= ptr.buildinglevel and info.m_ofkind ~= kind and ptr.level > haslevel then
			UIP_InfoSet( m_uiUIP_Info[index], kind,
						OfSprite(ptr.shape), -- shape
						ItemColorSprite(ptr.color), -- color
						"Lv."..g_official_forging[kind].level.."  "..T(721), -- name
						F(769, zhtime(g_official_forging[kind].quick) ), -- desc
						nil,  -- otherDesc
						nil,  -- timerDesc
						nil, -- sec
						T(718)..T(721), -- title
						NameColor(1),	-- titlecolor
						nil, -- res
						g_official_forging[kind].silver, -- silver
						g_official_forging[kind].token, -- token
						g_official_forging[kind].free,
						info.m_offree ) -- token
			m_uiUIP_Info[index].transform:SetSiblingIndex(kind);
			if index == #m_uiUIP_Info then
				return
				else
				index = index + 1;			
			end
			cankind = kind;
			break;
		end
	end
	
	-- 找一个即将雇佣的
	for kind=cankind+1, #g_official_forging, 1 do
		local ptr = g_official_forging[kind];
		if info.m_ofkind ~= kind and ptr.level > haslevel then
			UIP_InfoSet( m_uiUIP_Info[index], kind,
						OfSprite(ptr.shape), -- shape
						ItemColorSprite(ptr.color), -- color
						"Lv."..g_official_forging[kind].level.."  "..T(721), -- name
						F(769, zhtime(g_official_forging[kind].quick) ), -- desc
						nil,  -- otherDesc
						BuildingNameLv(BUILDING_Main,nil,ptr.buildinglevel).." "..T(724),  -- timerDesc
						nil, -- sec
						T(718)..T(721), -- title
						NameColor(1),	-- titlecolor
						nil, -- res
						0, -- silver
						0, -- token
						0,
						info.m_offree ) -- token
			m_uiUIP_Info[index].transform:SetSiblingIndex(kind);
			
			if IsGuiding() then
				if GetCurrentGuideType() == GUIDE_MAKE_CHOOSE then
					FindCmdTpye(m_uiUIP_Info[index].transform);
				end
			end
			break;
		end
	end
end

-- 内政官
function OfficialHireDlgSetGov()
	if m_Dlg == nil then
		return;
	end
	SetText( m_uiText, T(716) )
	local info = GetPlayer().m_officialhire[1];
	-- 已雇佣
	if info.m_ofkind > 0 then
		local kind = info.m_ofkind;
		local ptr = g_official_gov[kind];
		UIP_InfoSet( m_uiUIP_Info[1], kind,
						OfSprite(ptr.shape), -- shape
						ItemColorSprite(ptr.color), -- color
						"Lv."..g_official_gov[kind].level.."  "..T(722), -- name
						F(732, g_official_gov[kind].produce), -- desc
						nil,  -- otherDesc
						nil,  -- timerDesc
						info.m_ofsec, -- sec
						T(719)..T(722), -- title
						NameColor(3),	-- titlecolor
						{ 1,1,1,g_official_gov[kind].haveiron }, -- res
						0, -- silver
						0, -- token
						0, -- free
						0 ) -- getfree
						
		-- 如果雇佣的是高级的就不用设置了
		if g_official_gov[kind].token > 0 then
			return
		end
		
	else
		-- 未雇佣
		UIP_InfoSet( m_uiUIP_Info[1], 0, nil, nil, T(728)..T(722), nil, nil, nil, nil, T(719)..T(722), NameColor(3), nil, 0, 0, 0, 0 )
	end
	
	-- 可雇佣
	local index = 3;
	if info.m_ofkind > 0 then
		index = 2
	end
	local pBuilding = GetPlayer():GetBuilding( BUILDING_Main, -1 );
	if pBuilding == nil then
		return;
	end
	for kind=#g_official_gov, 1, -1 do
		local ptr = g_official_gov[kind];
		if pBuilding.m_level >= ptr.buildinglevel and info.m_ofkind ~= kind then
			UIP_InfoSet( m_uiUIP_Info[index], kind,
						OfSprite(ptr.shape), -- shape
						ItemColorSprite(ptr.color), -- color
						"Lv."..g_official_gov[kind].level.."  "..T(722), -- name
						F(732, g_official_gov[kind].produce), -- desc
						nil,  -- otherDesc
						nil,  -- timerDesc
						nil, -- sec
						T(718)..T(722), -- title
						NameColor(1),	-- titlecolor
						{ 1,1,1,g_official_gov[kind].haveiron }, -- res
						g_official_gov[kind].silver, -- silver
						g_official_gov[kind].token, -- token
						g_official_gov[kind].free,
						info.m_offree ) -- token
			if index == 2 then
				return
			else
				index = index - 1;			
			end
		end
	end
end

local function TechVipDesc( vip )
	if vip <= 0 or Utils.get_int_sflag( GetPlayer().m_actor_sflag, ACTOR_SFLAG_OFFICIAL_TECH ) == 1 then
		return nil;
	end
	return T(2321)
end
-- 研究员
function OfficialHireDlgSetTech()
	if m_Dlg == nil then
		return;
	end
	SetText( m_uiText, T(717) )
	local info = GetPlayer().m_officialhire[2];
	-- 已雇佣
	if info.m_ofkind > 0 then
		local kind = info.m_ofkind;
		local ptr = g_official_tech[kind];
		UIP_InfoSet( m_uiUIP_Info[1], kind,
						OfSprite(ptr.shape), -- shape
						ItemColorSprite(ptr.color), -- color
						"Lv."..g_official_tech[kind].level.."  "..T(723), -- name
						F(727, zhtime(g_official_tech[kind].quick) ), -- desc
						nil,  -- otherDesc
						nil,  -- timerDesc
						info.m_ofsec, -- sec
						T(719)..T(723), -- title
						NameColor(3),	-- titlecolor
						nil, -- res
						0, -- silver
						0, -- token
						0, -- free
						0 ) -- getfree
						
		-- 如果雇佣的是高级的就不用设置了
		if g_official_tech[kind].token > 0 then
			return
		end
		
	else
		-- 未雇佣
		UIP_InfoSet( m_uiUIP_Info[1], 0, nil, nil, T(728)..T(723), nil, nil, nil, nil, T(719)..T(723),NameColor(0), nil, 0, 0, 0, 0 )
	end
	
	-- 可雇佣
	local index = 3;
	if info.m_ofkind > 0 then
		index = 2
	end
	local pBuilding = GetPlayer():GetBuilding( BUILDING_Tech, -1 );
	if pBuilding == nil then
		return;
	end
	for kind=#g_official_tech, 1, -1 do
		local ptr = g_official_tech[kind];
		if pBuilding.m_level >= ptr.buildinglevel and info.m_ofkind ~= kind then
			UIP_InfoSet( m_uiUIP_Info[index], kind,
						OfSprite(ptr.shape), -- shape
						ItemColorSprite(ptr.color), -- color
						"Lv."..g_official_tech[kind].level.."  "..T(723), -- name
						F(727, zhtime(g_official_tech[kind].quick) ), -- desc
						TechVipDesc(g_official_tech[kind].vip),  -- otherDesc
						nil,  -- timerDesc
						nil, -- sec
						T(718)..T(723), -- title
						NameColor(1),	-- titlecolor
						nil, -- res
						g_official_tech[kind].silver, -- silver
						g_official_tech[kind].token, -- token
						g_official_tech[kind].free,
						info.m_offree ) -- token
			if index == 2 then
				return
			else
				index = index - 1;			
			end
		end
	end
end

-- 购买雇佣
function OfficialHireDlgBuy( kind )
	local config = nil
	if m_type == 0 then
		config = g_official_forging[kind];
	elseif m_type == 1 then
		config = g_official_gov[kind];
	elseif m_type == 2 then
		config = g_official_tech[kind];
	else
		return
	end
	
	local info = GetPlayer().m_officialhire[m_type];
	if config.free == 1 and Utils.get_int_sflag( info.m_offree, kind ) == 0 then
		if IsGuiding() then
			if GetCurrentGuideType() == GUIDE_MAKE_CHOOSE then
				GuideNext();
			end
		end
		system_askinfo( ASKINFO_OFFICIALHIRE, "", 0, m_type, kind )
		
	elseif config.silver > 0 then
		if GetPlayer().m_silver < config.silver then
			JumpRes(1);
			return
		end
		system_askinfo( ASKINFO_OFFICIALHIRE, "", 0, m_type, kind )
		
	elseif config.token > 0 then
		MsgBox( F(755, config.token ), function()
			if GetPlayer().m_token < config.token then
				JumpToken();
				return
			end
			system_askinfo( ASKINFO_OFFICIALHIRE, "", 0, m_type, kind )
		end )
	end
end
