-- 界面
local m_Dlg = nil;

-- 打开界面
function MapGlobalDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapGlobalDlg" );
end

-- 隐藏界面
function MapGlobalDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MapGlobalDlg" );
end

-- 删除界面
function MapGlobalDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapGlobalDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapGlobalDlgClose();
		elseif nControlID >= 1 and nControlID <= 30 then
			MapGlobalDlgSelect( nControlID )
        end
	end
end

-- 载入时调用
function MapGlobalDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function MapGlobalDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapGlobalDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapGlobalDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapGlobalDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapGlobalDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapGlobalDlgShow()
	MapGlobalDlgOpen()
end

-- 选择区域
function MapGlobalDlgSelect( zoneid )
	if g_zoneinfo[zoneid] == nil then
		return
	end
	 MapZoneDlgShow( zoneid )
	--WorldMap.GotoCoor( g_zoneinfo[zoneid].center_posx, g_zoneinfo[zoneid].center_posy )
	MapGlobalDlgClose();
end
