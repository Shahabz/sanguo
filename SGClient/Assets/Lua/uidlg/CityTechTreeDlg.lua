-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_TechRow = nil; --UnityEngine.GameObject
local m_uiTechInfo = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_uiTechObjs = {};
local m_LastSelectTech = nil;
local m_kind = 0;
-- 打开界面
function CityTechTreeDlgOpen()
	m_Dlg = eye.uiManager:Open( "CityTechTreeDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(708), 5, CityTechTreeDlgClose );
end

-- 隐藏界面
function CityTechTreeDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_TechRow(Clone)" then
			m_ObjectPool:Release( "UIP_TechRow", obj );
		end
    end
	CityTechTreeDlgClick( -1 );
	DialogFrameModClose( m_DialogFrameMod );
	eye.uiManager:Close( "CityTechTreeDlg" );
end

-- 删除界面
function CityTechTreeDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function CityTechTreeDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            CityTechTreeDlgClose();
		elseif nControlID == -2 then
			CityTechTreeDlgClick( -1 );
		elseif nControlID == 1 then
			pop( T(713) )
		elseif nControlID == 2 then	
			CityTechTreeDlgSelect()
		elseif nControlID >= 1000 and nControlID <= 2000 then
			CityTechTreeDlgClick( nControlID-1000 )
        end
	end
end

-- 载入时调用
function CityTechTreeDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiScroll = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_TechRow = objs[2];
	m_uiTechInfo = objs[3];
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_TechRow", 11, 11, m_uiUIP_TechRow);
end

-- 界面初始化时调用
function CityTechTreeDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function CityTechTreeDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function CityTechTreeDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function CityTechTreeDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function CityTechTreeDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function CityTechTreeDlgShow()
	CityTechTreeDlgOpen()
	
	-- 太学院
	local pBuilding = GetPlayer():GetBuilding( BUILDING_Tech, -1 );
	if pBuilding == nil then
		return;
	end
	
	local uiTechRowContentList = {}
	for row = 1, 11, 1 do
		local uiTechRow = m_ObjectPool:Get( "UIP_TechRow" );
		uiTechRow.transform:SetParent( m_uiContent.transform );
		local uiTechRowContent = uiTechRow.transform:Find("Content");
		for i = 0, uiTechRowContent.transform.childCount - 1 do
			SetFalse( uiTechRowContent.transform:GetChild(i).transform:Find("Content") );
		end
		uiTechRowContentList[row] = uiTechRowContent;
	end
	
	m_uiTechObjs = {};
	for kind=1, #g_techinfo, 1 do
		local conf = g_citytech_tree[kind]
		local x = conf.x;
		local y = conf.y;
		local uiRow = uiTechRowContentList[x];
		local uiObj = uiRow.transform:GetChild(y-1).transform;
		m_uiTechObjs[kind] = uiObj;
		SetTrue( uiObj.transform:Find("Content") );
	end
	
	for kind=1, #g_techinfo, 1 do
		local level = GetPlayer().m_techlevel[kind];
		local uplevel = GetPlayer().m_techlevel[kind] + 1;
		local progress = GetPlayer().m_techprogress[kind];
		local maxlevel = #g_techinfo[kind];	
			
		local uiObj = m_uiTechObjs[kind];
		local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiName = objs[1];
		local uiLevel = objs[2];
		local uiIcon = objs[3];
		local uiLine = objs[4];
		local uiContent = objs[5];
		local uiLevelBack = objs[6];
		local uiSelect = objs[7];
		
		SetControlID( uiObj.transform:Find("Content"), 1000+kind )
		SetImage( uiShape, TechSprite( kind ) );
		SetText( uiName, TechName( kind ) );
		SetFalse( uiSelect );
		
		-- 等级
		local prekind = 0
		local prelevel = 0
		if level >= maxlevel then
			SetText( uiLevel, level );
			SetFalse( uiLevel );
			SetFalse( uiLevelBack );
			prekind = g_techinfo[kind][level].prekind;
			prelevel = g_techinfo[kind][level].prelevel;
			SetFalse( uiIcon );
		else
			SetText( uiLevel, uplevel );
			SetTrue( uiLevel );
			SetTrue( uiLevelBack );
			prekind = g_techinfo[kind][uplevel].prekind;
			prelevel = g_techinfo[kind][uplevel].prelevel;
			
			-- 是否解锁
			if pBuilding.m_level >= g_techinfo[kind][uplevel].buildinglevel and GetPlayer().m_techlevel[prekind] >= prelevel then
				SetImage( uiIcon, LoadSprite("ui_icon_arrow_up") )
				SetTrue( uiIcon );
			else
				SetImage( uiIcon, LoadSprite("ui_icon_lock") )
				SetTrue( uiIcon );
			end
		end
		
		-- 是否有前置科技
		if prekind > 0 then
			SetTrue( uiLine );
			local offsetx = g_citytech_tree[kind].x - g_citytech_tree[prekind].x;
			print(offsetx)
		else
			SetFalse( uiLine );
		end
		
	end
	m_uiContent.transform.localPosition = Vector2.zero
end

local function TechSelectShow( uiObj, bShow )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiSelect = objs[7];
	SetShow( uiSelect, bShow )
end

-- 点击科技
function CityTechTreeDlgClick( kind )
	m_kind = kind
	if kind <= 0 then
		TechSelectShow( m_LastSelectTech, false )
		m_LastSelectTech = nil;
		m_uiTechInfo:SetActive(false);
		return
	end
	
	if m_LastSelectTech ~= nil then
		TechSelectShow( m_LastSelectTech, false )
	end

	m_LastSelectTech = m_uiTechObjs[kind];
	TechSelectShow( m_LastSelectTech, true )
	
	-- 设置位置
	local siblingIndex = m_LastSelectTech.parent.parent:GetSiblingIndex();
    if m_uiTechInfo.transform:GetSiblingIndex() < siblingIndex then
        m_uiTechInfo.transform:SetSiblingIndex(siblingIndex);
    else
        m_uiTechInfo.transform:SetSiblingIndex(siblingIndex + 1);
    end
    m_uiTechInfo:SetActive(true);
	
	 -- 判断弹出菜单是否显示在最下面
    local uiParent = m_LastSelectTech.parent.parent.transform;
    local posY = - uiParent.localPosition.y - m_uiContent.transform.offsetMax.y;
    if posY + 300 > m_uiScroll.transform.rect.height then
        local siblingIndex = m_uiTechInfo.transform:GetSiblingIndex();
        m_uiScroll.transform:GetComponent("UIScrollRect"):ScrollToBottom(siblingIndex);
    end
	
	-- 设置信息
	local level = GetPlayer().m_techlevel[kind];
	local uplevel = GetPlayer().m_techlevel[kind] + 1;
	local progress = GetPlayer().m_techprogress[kind];
	local maxlevel = #g_techinfo[kind];	
	
	local objs = m_uiTechInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiName = objs[0];
	local uiDesc = objs[1];
	local uiLockDesc = objs[2];
	local uiUnlockBtn = objs[3];
	local uiSelectBtn = objs[4];
	
	-- 名称
	SetText( uiName, TechName( kind ) );
	
	-- 描述
	if level >= maxlevel then
		-- 满级情况
		SetText( uiDesc, Utils.StringFormat( TechDesc( kind ), g_techinfo[kind][level].value ) );
		SetText( uiLockDesc, T(712), Hex2Color(0x8E8E8EFF) )
		SetFalse( uiUnlockBtn );
		SetFalse( uiSelectBtn );
	else
		SetText( uiDesc, Utils.StringFormat( TechDesc( kind ), g_techinfo[kind][uplevel].value ) );
		
		-- 如果台学院未达到要求
		local pBuilding = GetPlayer():GetBuilding( BUILDING_Tech, -1 );
		if pBuilding == nil then
			return;
		end
		if pBuilding.m_level < g_techinfo[kind][uplevel].buildinglevel then
			SetText( uiLockDesc, T(711)..": "..BuildingName( BUILDING_Tech ).."Lv."..g_techinfo[kind][uplevel].buildinglevel, Hex2Color(0xB99838FF) )
			SetTrue( uiUnlockBtn );
			SetFalse( uiSelectBtn );
		else
			-- 前置科技
			local prekind = g_techinfo[kind][uplevel].prekind;
			local prelevel = g_techinfo[kind][uplevel].prelevel;
			if prekind > 0 then
				-- 前置科技未达到要求
				if GetPlayer().m_techlevel[prekind] < prelevel then
					SetText( uiLockDesc, T(711)..":"..TechName( prekind ).."Lv."..prelevel, Hex2Color(0xB99838FF) )
					SetTrue( uiUnlockBtn );
					SetFalse( uiSelectBtn );
				else
					-- 达到要求，直接显示进度
					SetText( uiLockDesc, F(701, progress, g_techinfo[kind][uplevel].progress ), Hex2Color(0x1EFF00FF) );
					SetFalse( uiUnlockBtn );
					SetTrue( uiSelectBtn );
				end
			else
				-- 没前置，直接显示进度
				SetText( uiLockDesc, F(701, progress, g_techinfo[kind][uplevel].progress ), Hex2Color(0x1EFF00FF) );
				SetFalse( uiUnlockBtn );
				SetTrue( uiSelectBtn );
			end
		end
		
	end
	
end

-- 选择科技
function CityTechTreeDlgSelect()
	if m_kind <= 0 then
		return;
	end
	CityTechTreeDlgClose()
end
