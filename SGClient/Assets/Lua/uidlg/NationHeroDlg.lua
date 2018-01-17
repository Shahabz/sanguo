-- 界面
local m_Dlg = nil;

-- 打开界面
function NationHeroDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationHeroDlg" );
end

-- 隐藏界面
function NationHeroDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "NationHeroDlg" );
end

-- 删除界面
function NationHeroDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationHeroDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationHeroDlgClose();
        end
	end
end

-- 载入时调用
function NationHeroDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function NationHeroDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NationHeroDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationHeroDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationHeroDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationHeroDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationHeroDlgShow()
	NationHeroDlgOpen()
end
