-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

-- 打开界面
function MapZoneDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapZoneDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(1003), 5, MapZoneDlgClose );
end

-- 隐藏界面
function MapZoneDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "MapZoneDlg" );
end

-- 删除界面
function MapZoneDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapZoneDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapZoneDlgClose();
			
		-- 世界地图
		elseif nControlID == 1 then
			 MapGlobalDlgShow()
        end
	end
end

-- 载入时调用
function MapZoneDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function MapZoneDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapZoneDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapZoneDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapZoneDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapZoneDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapZoneDlgShow()
	MapZoneDlgOpen()
end
