-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_TechRow = nil; --UnityEngine.GameObject
local m_uiTeching = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;

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
	SetFalse( m_uiTeching )
	
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
	m_uiTeching = objs[3];
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
	
	local uiTechObjs = {};
	for kind=1, #g_techinfo, 1 do
		local conf = g_citytech_tree[kind]
		local x = conf.x;
		local y = conf.y;
		local uiRow = uiTechRowContentList[x];
		local uiObj = uiRow.transform:GetChild(y-1).transform;
		uiTechObjs[kind] = uiObj;
		SetTrue( uiObj.transform:Find("Content") );
	end
	
	for kind=1, #g_techinfo, 1 do
		local level = GetPlayer().m_techlevel[kind];
		local uplevel = GetPlayer().m_techlevel[kind] + 1;
		local progress = GetPlayer().m_techprogress[kind];
		local maxlevel = #g_techinfo[kind];	
			
		local uiObj = uiTechObjs[kind];
		local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiName = objs[1];
		local uiLevel = objs[2];
		local uiIcon = objs[3];
		local uiLine = objs[4];
		local uiContent = objs[5];
		local uiLevelBack = objs[6];
		
		SetTrue( uiObj );
		SetImage( uiShape, TechSprite( kind ) );
		SetText( uiName, TechName( kind ) );
		
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
			if pBuilding.m_level >= g_techinfo[kind][uplevel].buildinglevel then
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
	-- 科技列表
	--[[for kind=1, #g_techinfo, 1 do
		
		local level = GetPlayer().m_techlevel[kind];
		local uplevel = GetPlayer().m_techlevel[kind] + 1;
		local progress = GetPlayer().m_techprogress[kind];
		local maxlevel = #g_techinfo[kind];	
		-- 科技满足出现等级，并且未满级，并且前置任务满足
		if level < maxlevel and pBuilding.m_level >= g_techinfo[kind][uplevel].buildinglevel then
			-- 前置科技满足
			local prekind = g_techinfo[kind][uplevel].prekind;
			local prelevel = g_techinfo[kind][uplevel].prelevel;
			if prekind <= 0 or GetPlayer().m_techlevel[prekind] >= prelevel then
				local uiObj = m_ObjectPool:Get( "UIP_Tech" );
				uiObj.transform:SetParent( m_uiContent.transform );
				local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
				local uiShape = objs[0];
				local uiName = objs[1];
				local uiDesc = objs[2];
				local uiProgress = objs[3];
				local uiViewBtn = objs[4];
				local uiUpgradeBtn = objs[5];
				local uiContinueBtn = objs[6];
				
				-- 形象
				SetImage( uiShape, TechSprite( kind ) );
				-- 名称
				SetText( uiName, TechName( kind ).." Lv."..uplevel );
				-- 描述
				SetText( uiDesc, Utils.StringFormat( TechDesc( kind ), g_techinfo[kind][uplevel].value ) );
				-- 进度
				SetText( uiProgress, F(701, progress, g_techinfo[kind][uplevel].progress ) );
				
				-- 有正在升级的，显示查看
				if pBuilding.m_value > 0 then
					SetControlID( uiViewBtn, 1000+kind )
					SetTrue( uiViewBtn );
					SetFalse( uiUpgradeBtn );
					SetFalse( uiContinueBtn );
				
				-- 显示继续研究	
				elseif progress >= 1 then
					SetControlID( uiContinueBtn, 1000+kind )
					SetTrue( uiContinueBtn );
					SetFalse( uiViewBtn );
					SetFalse( uiUpgradeBtn );
					
				-- 显示研究
				else
					SetControlID( uiUpgradeBtn, 1000+kind )
					SetTrue( uiUpgradeBtn );
					SetFalse( uiViewBtn );
					SetFalse( uiContinueBtn );
				end
				
			end
		end
	end--]]
	--m_uiScroll:GetComponent("UIScrollRect"):ResetScroll();
	m_uiContent.transform.localPosition = Vector2.zero
end

