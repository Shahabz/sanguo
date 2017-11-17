-- 界面
local m_Dlg = nil;

-- 打开界面
function CabinetDlgOpen()
	ResourceManager.LoadAssetBundle( "ui_static_cabinet1" );
	ResourceManager.LoadAssetBundle( "ui_static_cabinet2" );
	ResourceManager.LoadAssetBundle( "ui_static_cabinet3" );
	ResourceManager.LoadAssetBundle( "ui_static_cabinetback" );
	m_Dlg = eye.uiManager:Open( "CabinetDlg" );
end

-- 隐藏界面
function CabinetDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "CabinetDlg" );
end

-- 删除界面
function CabinetDlgDestroy()
	GameObject.Destroy( m_Dlg );
	ResourceManager.UnloadAssetBundle( "ui_static_cabinet1" )
	ResourceManager.UnloadAssetBundle( "ui_static_cabinet2" )
	ResourceManager.UnloadAssetBundle( "ui_static_cabinet3" )
	ResourceManager.UnloadAssetBundle( "ui_static_cabinetback" )
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
	CabinetDlgOpen()
end
