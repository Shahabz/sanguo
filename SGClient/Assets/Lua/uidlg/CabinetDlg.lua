-- 界面
local m_Dlg = nil;
local m_uiGather = nil; --UnityEngine.GameObject
local m_uiGuard = nil; --UnityEngine.GameObject
local m_uiBooks = nil; --UnityEngine.GameObject
local m_DialogFrameMod = nil;
local m_pBuilding = nil;
-- 打开界面
function CabinetDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_cabinet1" );
	ResourceManager.LoadAssetBundle( "_ab_ui_static_cabinet2" );
	ResourceManager.LoadAssetBundle( "_ab_ui_static_cabinet3" );
	m_Dlg = eye.uiManager:Open( "CabinetDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, BuildingName(BUILDING_Cabinet).." Lv."..m_pBuilding.m_level, HELP_CabinetDlg, CabinetDlgClose );
end

-- 隐藏界面
function CabinetDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "CabinetDlg" );
end

-- 删除界面
function CabinetDlgDestroy()
	GameObject.Destroy( m_Dlg );
	Invoke( function() 
			ResourceManager.UnloadAssetBundleImmediately( "_ab_ui_static_cabinet1" )
			ResourceManager.UnloadAssetBundleImmediately( "_ab_ui_static_cabinet2" )
			ResourceManager.UnloadAssetBundleImmediately( "_ab_ui_static_cabinet3" )
		end, 0.3 );
		
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function CabinetDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            CabinetDlgClose();
		
		-- 财赋署
		elseif nControlID == 1 then
			HeroGatherDlgShow()
			
		-- 御林卫
		elseif nControlID == 2 then
			HeroGuardDlgShow()
			
		-- 天策府
		elseif nControlID == 3 then
			HeroLevyDlgShow()
        end
	end
end

-- 载入时调用
function CabinetDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiGather = objs[0];
	m_uiGuard = objs[1];
	m_uiBooks = objs[2];
end

-- 界面初始化时调用
function CabinetDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function CabinetDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function CabinetDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function CabinetDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function CabinetDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function CabinetDlgShow()
	m_pBuilding = GetPlayer():GetBuilding( BUILDING_Cabinet );
	CabinetDlgOpen()
	local buildingname = BuildingName(BUILDING_Cabinet)	
	if m_pBuilding.m_level == 1 then
		SetTrue( m_uiGather.transform:Find("EnterButton") )
		SetText( m_uiGather.transform:Find("Lock"), "" )
		
		SetFalse( m_uiGuard.transform:Find("EnterButton") )
		SetText( m_uiGuard.transform:Find("Lock"), F(725, buildingname, 2 ) )
		
		SetFalse( m_uiBooks.transform:Find("EnterButton") )
		SetText( m_uiBooks.transform:Find("Lock"), F(725, buildingname, 3 ) )
		
	elseif m_pBuilding.m_level == 2 then
		SetTrue( m_uiGather.transform:Find("EnterButton") )
		SetText( m_uiGather.transform:Find("Lock"), "" )
		
		SetTrue( m_uiGuard.transform:Find("EnterButton") )
		SetText( m_uiGuard.transform:Find("Lock"), "" )
		
		SetFalse( m_uiBooks.transform:Find("EnterButton") )
		SetText( m_uiBooks.transform:Find("Lock"), F(725, buildingname, 3 ) )
		
	elseif m_pBuilding.m_level == 3 then
		SetTrue( m_uiGather.transform:Find("EnterButton") )
		SetText( m_uiGather.transform:Find("Lock"), "" )
		
		SetTrue( m_uiGuard.transform:Find("EnterButton") )
		SetText( m_uiGuard.transform:Find("Lock"), "" )
		
		SetTrue( m_uiBooks.transform:Find("EnterButton") )
		SetText( m_uiBooks.transform:Find("Lock"), "" )
	end
end
