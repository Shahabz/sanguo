-- 界面
local m_Dlg = nil;

-- 打开界面
function ChatDlgOpen()
	m_Dlg = eye.uiManager:Open( "ChatDlg" );
end

-- 隐藏界面
function ChatDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "ChatDlg" );
end

-- 删除界面
function ChatDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ChatDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ChatDlgClose();
        end
	end
end

-- 载入时调用
function ChatDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function ChatDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function ChatDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ChatDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function ChatDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function ChatDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
