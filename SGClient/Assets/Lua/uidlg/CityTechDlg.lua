-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiUIP_Tech = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiTeching = nil; --UnityEngine.GameObject
local m_uiTreeBtn = nil; --UnityEngine.GameObject
local m_uiTechInfo = nil; --UnityEngine.GameObject
local m_uiScroll = nil; --UnityEngine.GameObject

local m_uiOfficialShape = nil; --UnityEngine.GameObject
local m_uiOfficialTimeBack = nil; --UnityEngine.GameObject
local m_uiOfficialTime = nil; --UnityEngine.GameObject
local m_uiOfficialName = nil; --UnityEngine.GameObject
local m_uiOfficialDesc = nil; --UnityEngine.GameObject
local m_uiOfficialEffect = nil; --UnityEngine.GameObject
local m_uiOfficialBtn = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_kind = 0;
local m_canUpgrade = 0;
local m_firstKind = 0;

-- 打开界面
function CityTechDlgOpen()
	m_Dlg = eye.uiManager:Open( "CityTechDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, BuildingName(4), HELP_CityTechDlg, CityTechDlgClose );
end

-- 隐藏界面
function CityTechDlgClose()
	if m_Dlg == nil then
		return;
	end
	CityTechDlgClear()
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	if IsGuiding() then HideGuideFinger() end;
	eye.uiManager:Close( "CityTechDlg" );
end

-- 删除界面
function CityTechDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function CityTechDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            CityTechDlgClose();
		elseif nControlID == -2 then
			CityTechDlgSelect( -1 )
			
		-- 科技树
		elseif nControlID == 1 then
			CityTechTreeDlgShow()
			CityTechDlgClose();
			
		-- 加速
		elseif nControlID == 2 then
			CityTechDlgQuick();
			
		-- 研究
		elseif nControlID == 3 then
			CityTechDlgUpgrade();
			
		-- 免费加速
		elseif nControlID == 4 then
			CityTechDlgFreeQuick();
		
		-- 雇佣
		elseif nControlID == 5 then
			OfficialHireDlgShow( 2 );
			
		-- 点击科技
		elseif nControlID > 1000 and nControlID < 2000 then
			CityTechDlgSelect( nControlID - 1000 )
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 1 then
			CityTechDlgClose();
			CityTechTreeDlgClose()
		elseif nControlID == 2 then
			CityTechDlgSetOfficial()
		end
	end
end

-- 载入时调用
function CityTechDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiUIP_Tech = objs[0];
	m_uiContent = objs[1];
	m_uiTeching = objs[2];
	m_uiTreeBtn = objs[3];
	m_uiTechInfo = objs[4];
	m_uiScroll = objs[5];
	m_uiOfficialShape = objs[6];
	m_uiOfficialTimeBack = objs[7];
	m_uiOfficialTime = objs[8];
	m_uiOfficialName = objs[9];
	m_uiOfficialDesc = objs[10];
	m_uiOfficialEffect = objs[11];
	m_uiOfficialBtn = objs[12];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Tech", 5, 5, m_uiUIP_Tech);
end

-- 界面初始化时调用
function CityTechDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function CityTechDlgOnEnable( gameObject )

end

-- 界面隐藏时调用
function CityTechDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function CityTechDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function CityTechDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function CityTechDlgOnShow()
	CityTechDlgOpen()
	m_uiTreeBtn.transform:SetSiblingIndex(1000);	
	m_uiTechInfo.transform:SetSiblingIndex(1000);
	CityTechDlgOnSet();
end

function CityTechDlgOnSet()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	CityTechDlgClear()
	
	-- 太学院
	local pBuilding = GetPlayer():GetBuilding( BUILDING_Tech, -1 );
	if pBuilding == nil then
		return;
	end
	
	-- 科技树按钮
	if pBuilding.m_level >= global.techtree_level then
		SetTrue( m_uiTreeBtn )
	else
		SetFalse( m_uiTreeBtn )
	end

	-- 正在研究
	if pBuilding.m_value > 0 then
		SetTrue( m_uiTeching )
		local kind = pBuilding.m_value;
		local level = GetPlayer().m_techlevel[kind];
		local maxlevel = #g_techinfo[kind];
		local value = 0;
		if level > 0 then
			value = g_techinfo[kind][level].value;
		end
		local upvalue = g_techinfo[kind][level+1].value;
		
		local objs = m_uiTeching.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiName = objs[1];
		local uiDesc = objs[2];
		local uiTimer = objs[3];
		local uiQuickBtn = objs[4];
		local FreeQuickBtn = objs[5];
		
		SetImage( uiShape, TechSprite( kind ) );
		SetText( uiName, TechName( kind ).." Lv."..level.."->Lv."..(level+1) );
		SetText( uiDesc, Utils.StringFormat( TechDescUp( kind ), value, upvalue ) );
		SetTimer( uiTimer, pBuilding.m_sec, pBuilding.m_needsec, 1, T(702) )
		
		SetFalse( FreeQuickBtn )
		SetFalse( uiQuickBtn )
	else
		SetFalse( m_uiTeching )
	end
	
	-- 首选科技
	local fristTechObj = nil;
	
	-- 可研究的科技列表
	for kind=1, #g_techinfo, 1 do
		local level = GetPlayer().m_techlevel[kind];
		local uplevel = GetPlayer().m_techlevel[kind] + 1;
		local progress = GetPlayer().m_techprogress[kind];
		local maxlevel = #g_techinfo[kind];	
		-- 科技满足出现等级，并且未满级，并且前置任务满足
		if pBuilding.m_value ~= kind and level < maxlevel and pBuilding.m_level >= g_techinfo[kind][uplevel].buildinglevel then
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
				local uiSelect = objs[7];				
				local uiEffect = objs[8];

				
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
				
				-- 首选科技
				if m_firstKind > 0 and m_firstKind == kind then
					SetTrue( uiSelect )
					fristTechObj = uiObj;
				else
					SetFalse( uiSelect )
				end
			end
		end
	end
	
	-- 将首选科技放在前面
	if fristTechObj ~= nil then
		fristTechObj.transform:SetSiblingIndex(0);
	end
	m_uiScroll:GetComponent("UIScrollRect"):ResetScroll();
	m_uiContent.transform.localPosition = Vector2.zero
	CityTechDlgSetOfficial();
	
	if IsGuiding() then
		if GetCurrentGuideType() == GUIDE_TECH then 
			CityTechDlgGuideOpenEffect(true)
		end
	end
end

-- 设置雇佣官
function CityTechDlgSetOfficial()
	if m_Dlg == nil then
		return;
	end
	-- 免费按钮
	local objs = m_uiTeching.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiQuickBtn = objs[4];
	local FreeQuickBtn = objs[5];
	
	local info = GetPlayer().m_officialhire[2];
	-- 已雇佣
	if info.m_ofkind > 0 then
		SetTrue( m_uiOfficialShape )
		SetFalse( m_uiOfficialEffect )
		SetTrue( m_uiOfficialTimeBack )
		SetTrue( m_uiOfficialTime )
		SetTimer( m_uiOfficialTime, info.m_ofsec, info.m_ofsec, 2 )
		if g_official_tech[info.m_ofkind].token > 0 then
			SetText( m_uiOfficialName, T(719).."Lv."..g_official_tech[info.m_ofkind].level..T(723), Hex2Color(0xD95DF4FF) )
		else
			SetText( m_uiOfficialName, T(719).."Lv."..g_official_tech[info.m_ofkind].level..T(723), Hex2Color(0xF7F3BBFF) )
		end
		SetText( m_uiOfficialDesc, F(727, zhtime(g_official_tech[info.m_ofkind].quick) ), Hex2Color(0x25C9FFFF) )
		SetImage( m_uiOfficialShape, OfSprite( g_official_tech[info.m_ofkind].shape ) )
		
		if info.m_ofquick >= 0 then
			SetTrue( FreeQuickBtn )
			SetFalse( uiQuickBtn )
		else
			SetFalse( FreeQuickBtn )
			SetTrue( uiQuickBtn )
		end
	else
		-- 未雇佣
		SetFalse( FreeQuickBtn )
		SetTrue( uiQuickBtn )	
		SetFalse( m_uiOfficialShape )
		SetFalse( m_uiOfficialTimeBack )
		SetFalse( m_uiOfficialTime )
		local pBuilding = GetPlayer():GetBuilding( BUILDING_Tech, -1 );
		if pBuilding == nil then
			return;
		end
		if pBuilding.m_level < g_official_tech[1].buildinglevel then
			-- 太学院7级解锁Lv.1研究员
			SetText( m_uiOfficialName, F(726, BuildingName(BUILDING_Tech), g_official_tech[1].buildinglevel, g_official_tech[1].level, T(723) ), Hex2Color(0x8A8A8AFF) )
			SetText( m_uiOfficialDesc, F(727, zhtime(g_official_tech[1].quick) ), Hex2Color(0x8A8A8AFF) )
			SetFalse( m_uiOfficialEffect )
			SetImage( m_uiOfficialBtn.transform:Find("Back"), LoadSprite("ui_icon_back_2") )
		else
			SetTrue( m_uiOfficialEffect )
			SetImage( m_uiOfficialBtn.transform:Find("Back"), LoadSprite("ui_icon_back_4") )
			SetText( m_uiOfficialName, T(723).."("..T(2338)..")", Hex2Color(0x8A8A8AFF) )
			SetText( m_uiOfficialDesc, T(2340), Hex2Color(0x8A8A8AFF) )
			-- 可雇佣
		--[[	for kind=#g_official_tech, 1, -1 do
				if pBuilding.m_level >= g_official_tech[kind].buildinglevel then
					SetText( m_uiOfficialName, T(718).."Lv."..g_official_tech[kind].level..T(723) )
					SetText( m_uiOfficialDesc, F(727, zhtime(g_official_tech[kind].quick) ) )
					break;
				end
			end--]]
		end
	end

end

-- 清空
function CityTechDlgClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Tech(Clone)" then
			m_ObjectPool:Release( "UIP_Tech", obj );
		end
    end
	SetFalse( m_uiTeching )
	m_uiScroll:GetComponent("UIScrollRect"):ResetScroll();
	m_uiContent.transform.localPosition = Vector2.zero
	m_kind = 0;
end

-- 选择科技
function CityTechDlgSelect( kind )
	m_kind = kind;
	if m_kind <= 0 then
		SetFalse( m_uiTechInfo );
		return
	end
	SetTrue( m_uiTechInfo );
	m_canUpgrade = 0;
	
	local objs = m_uiTechInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiName = objs[1];
	local uiDesc = objs[2];
	local uiProgress = objs[3];
	local uiProgressImage = objs[4];
	local uiSilver = objs[5];
	local uiWood = objs[6];
	local uiFood = objs[7];
	local uiIron = objs[8];
	local uiUpgradeSec = objs[9];
	local uiCloseBtn = objs[10];
	local uiUpgradeBtn = objs[11];
	local uplevel = GetPlayer().m_techlevel[kind] + 1;
	local progress = GetPlayer().m_techprogress[kind];
	
	-- 形象
	SetImage( uiShape, TechSprite( kind ) );
	-- 名称
	SetText( uiName, TechName( kind ).." Lv."..uplevel );
	-- 描述
	SetText( uiDesc, Utils.StringFormat( TechDesc( kind ), g_techinfo[kind][uplevel].value ) );
	-- 进度
	SetText( uiProgress, F(701, progress, g_techinfo[kind][uplevel].progress ) );
	-- 进度显示
	for i = 0 ,uiProgressImage.transform.childCount - 1 do
		local obj = uiProgressImage.transform:GetChild(i).gameObject;
		if i < progress then
			SetImage( obj.transform, LoadSprite("ui_progress_image_4") )
			obj.transform:GetComponent( typeof(Image) ).enabled = true
		elseif i < g_techinfo[kind][uplevel].progress then
			obj.transform:GetComponent( typeof(Image) ).enabled = false
		else
			SetImage( obj.transform, LoadSprite("ui_icon_back_2") )
			obj.transform:GetComponent( typeof(Image) ).enabled = true
		end
    end
	
	-- 资源花费
	local value = 0;
	value = g_techinfo[kind][uplevel].silver
	if value > 0 then
		SetTrue( uiSilver );
		if GetPlayer().m_silver > value then
			SetText( uiSilver.transform:Find("Text"), "<color=#03DE27FF>"..knum(value).."/"..knum(GetPlayer().m_silver).."</color>" )
		else
			SetText( uiSilver.transform:Find("Text"), "<color=#03DE27FF>"..knum(value).."/</color><color=#A01515FF>"..knum(GetPlayer().m_silver).."</color>" )
			m_canUpgrade = 1;
		end
	else
		SetFalse( uiSilver );
	end
	
	value = g_techinfo[kind][uplevel].wood
	if value > 0 then
		SetTrue( uiWood );
		if GetPlayer().m_wood > value then
			SetText( uiWood.transform:Find("Text"), "<color=#03DE27FF>"..knum(value).."/"..knum(GetPlayer().m_wood).."</color>" )
		else
			SetText( uiWood.transform:Find("Text"), "<color=#03DE27FF>"..knum(value).."/</color><color=#A01515FF>"..knum(GetPlayer().m_wood).."</color>" )
			m_canUpgrade = 2;
		end
	else
		SetFalse( uiWood );
	end
	
	value = g_techinfo[kind][uplevel].food
	if g_techinfo[kind][uplevel].food > 0 then
		SetTrue( uiFood );
		if GetPlayer().m_food > value then
			SetText( uiFood.transform:Find("Text"), "<color=#03DE27FF>"..knum(value).."/"..knum(GetPlayer().m_food).."</color>" )
		else
			SetText( uiFood.transform:Find("Text"), "<color=#03DE27FF>"..knum(value).."/</color><color=#A01515FF>"..knum(GetPlayer().m_food).."</color>" )
			m_canUpgrade = 3;
		end
	else
		SetFalse( uiFood );
	end
	
	value = g_techinfo[kind][uplevel].iron;
	if value > 0 then
		SetTrue( uiIron );
		if GetPlayer().m_iron > value then
			SetText( uiIron.transform:Find("Text"), "<color=#03DE27FF>"..value.."/"..GetPlayer().m_iron.."</color>" )
		else
			SetText( uiIron.transform:Find("Text"), "<color=#03DE27FF>"..value.."/</color><color=#A01515FF>"..GetPlayer().m_iron.."</color>" )
			m_canUpgrade = 4;
		end
	else
		SetFalse( uiIron );
	end
	
	-- 所需时间
	SetText( uiUpgradeSec, secnum( g_techinfo[kind][uplevel].sec ) )
	
	-- 太学院
	local pBuilding = GetPlayer():GetBuilding( BUILDING_Tech, -1 );
	if pBuilding == nil then
		return;
	end
	
	-- 有正在升级的
	if pBuilding.m_value > 0 then
		SetTrue( uiCloseBtn );
		SetFalse( uiUpgradeBtn );	
	else
		SetTrue( uiCloseBtn );
		SetTrue( uiUpgradeBtn );
	end
	
	if IsGuiding() then
		if GetCurrentGuideType() == GUIDE_TECH then 
			GuideNext();
			CityTechDlgGuideOpenEffect(false)
			FindCmdTpye(uiUpgradeBtn.transform);
		end
	end
end

-- 升级
function CityTechDlgUpgrade()
	if m_kind <= 0 then
		return;
	end
	if m_canUpgrade > 0 then
		CityTechDlgSelect( -1 )
		JumpRes( m_canUpgrade )
		return
	end
	system_askinfo( ASKINFO_TECH, "", 1, m_kind );
	if IsGuiding() and GetCurrentGuideType() == GUIDE_TECH_SURE then 
		GuideNext();
	end
	CityTechDlgSelect( -1 )
	m_firstKind = 0;
end

-- 加速
function CityTechDlgQuick()
	-- 太学院
	local pBuilding = GetPlayer():GetBuilding( BUILDING_Tech, -1 );
	if pBuilding == nil then
		return;
	end
	local kind = pBuilding.m_value;
	if kind <= 0 then
		return
	end
	-- 加速消耗钻石
	local min = math.floor(pBuilding.m_sec/60) + 1
	local token = math.ceil( min*global.techquick_token)
	MsgBox( F(709, token ), function() 
		if GetPlayer().m_token < token then
			JumpToken();
		else
			system_askinfo( ASKINFO_TECH, "", 2 );
			CityTechDlgClose()
		end
	end )
end

-- 免费加速
function CityTechDlgFreeQuick()
	local pBuilding = GetPlayer():GetBuilding( BUILDING_Tech, -1 );
	if pBuilding == nil then
		return;
	end
	local kind = pBuilding.m_value;
	if kind <= 0 then
		return
	end
	system_askinfo( ASKINFO_TECH, "", 3 );
end

-- 设置优先选择的科技
function CityTechDlgSetFirst( firstkind )
	m_firstKind = firstkind;
end

--指引升级科技特效开启
function CityTechDlgGuideOpenEffect(bShow)
	if m_Dlg == nil then return; end
	local uiObj = m_uiContent.transform:GetChild(1);
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;	
	local uiEffect = objs[8];
	if bShow == true then 
		SetTrue(uiEffect);
	else
		SetFalse(uiEffect);
	end
end

