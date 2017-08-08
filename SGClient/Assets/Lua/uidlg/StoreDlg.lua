-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
--------------------------------------------------------------
local m_uiRebuildNum = nil; --UnityEngine.GameObject
local m_uiGrid = nil; --UnityEngine.GameObject

local m_pBuilding = nil;

-- 打开界面
function StoreDlgOpen()
	m_Dlg = eye.uiManager:Open( "StoreDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(BUILDING_StoreHouse).." Lv."..m_pBuilding.m_level, 8, StoreDlgClose );
end

-- 隐藏界面
function StoreDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "StoreDlg" );
end

-- 删除界面
function StoreDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function StoreDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            StoreDlgClose();
        end
	end
end

-- 载入时调用
function StoreDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiRebuildNum = objs[0];
	m_uiGrid = objs[1];
end

-- 界面初始化时调用
function StoreDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function StoreDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function StoreDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function StoreDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function StoreDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function StoreDlgShow()
	-- 仓库
	m_pBuilding = GetPlayer():GetBuilding( BUILDING_StoreHouse, -1 );
	if m_pBuilding == nil then
		return;
	end 
	StoreDlgOpen()
	-- 高级重建次数
	SetText( m_uiRebuildNum, T(792).."x" )
	-- 资源道具列表
	local ResKindList = { 120, 121, 122 }
	-- 当前资源量
	local ResNumList = { GetPlayer().m_silver, GetPlayer().m_wood, GetPlayer().m_food }
	-- 保护资源量
	local ResProjectList = { 
	g_building_upgrade[BUILDING_StoreHouse][m_pBuilding.m_level].value0, 
	g_building_upgrade[BUILDING_StoreHouse][m_pBuilding.m_level].value1, 
	g_building_upgrade[BUILDING_StoreHouse][m_pBuilding.m_level].value2 }
	
	for i=1, #ResKindList do
		local uiRes = m_uiGrid.transform:GetChild(i-1).gameObject;
		local objs = uiRes.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiName = objs[1];
		local uiProgress = objs[2];
		local uiImage = objs[3];
		local uiText = objs[4];
		SetImage( uiShape, ItemSprite( ResKindList[i] ) )
		SetText( uiName, item_getname( ResKindList[i] ) )
		SetProgress( uiProgress, ResNumList[i]/ResProjectList[i] )
		SetText( uiText, knum(ResNumList[i]).."/"..knum(ResProjectList[i]) )
		if ResNumList[i] > ResProjectList[i] then
			SetImage( uiImage, LoadSprite("ui_progress_image_3") )
		else
			SetImage( uiImage, LoadSprite("ui_progress_image_4") )
		end
	end
end
