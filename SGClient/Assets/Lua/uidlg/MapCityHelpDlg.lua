-- 界面
local m_Dlg = nil;

-- 打开界面
function MapCityHelpDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapCityHelpDlg" );
end

-- 隐藏界面
function MapCityHelpDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MapCityHelpDlg" );
end

-- 删除界面
function MapCityHelpDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapCityHelpDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapCityHelpDlgClose();
        end
	end
end

-- 载入时调用
function MapCityHelpDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function MapCityHelpDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapCityHelpDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapCityHelpDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapCityHelpDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapCityHelpDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapCityHelpDlgShow()
	MapCityHelpDlgOpen()
end
