-- 界面
local m_Dlg = nil;

-- 打开界面
function ItemCityMoveDlgOpen()
	m_Dlg = eye.uiManager:Open( "ItemCityMoveDlg" );
end

-- 隐藏界面
function ItemCityMoveDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "ItemCityMoveDlg" );
end

-- 删除界面
function ItemCityMoveDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ItemCityMoveDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ItemCityMoveDlgClose();
        end
	end
end

-- 载入时调用
function ItemCityMoveDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function ItemCityMoveDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function ItemCityMoveDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ItemCityMoveDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function ItemCityMoveDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function ItemCityMoveDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function ItemCityMoveDlgShow()
	ItemCityMoveDlgOpen()
end
