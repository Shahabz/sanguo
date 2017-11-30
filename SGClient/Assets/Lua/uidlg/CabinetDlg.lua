-- 界面
local m_Dlg = nil;
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
        end
	end
end

-- 载入时调用
function CabinetDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
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
end
