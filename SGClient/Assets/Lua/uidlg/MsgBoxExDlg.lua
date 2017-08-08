-- 界面
local m_Dlg = nil;
local m_uiText = nil; --UnityEngine.GameObject
local m_uiCancelText = nil; --UnityEngine.GameObject
local m_uiOkText = nil; --UnityEngine.GameObject
local m_uiToggle = nil; --UnityEngine.GameObject
local m_callback = nil;
local m_toggle = false;

-- 打开界面
function MsgBoxExDlgOpen()
	m_Dlg = eye.uiManager:Open( "MsgBoxExDlg" );
end

-- 隐藏界面
function MsgBoxExDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MsgBoxExDlg" );
end

-- 删除界面
function MsgBoxExDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MsgBoxExDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MsgBoxExDlgClose();
		elseif nControlID == 1 then
			if m_callback then
				m_callback( m_toggle );
			end
			 MsgBoxExDlgClose();
        end
	elseif nType == UI_EVENT_TOGGLEVALUECHANGE then
		if nControlID == 1 then
			m_toggle = value;
		end
	end
end

-- 载入时调用
function MsgBoxExDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiText = objs[0];
	m_uiCancelText = objs[1];
	m_uiOkText = objs[2];	
	m_uiToggle = objs[3];
end

-- 界面初始化时调用
function MsgBoxExDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MsgBoxExDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MsgBoxExDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MsgBoxExDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MsgBoxExDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MsgBoxExDlgShow()
	MsgBoxExDlgOpen()
end
function MsgBoxEx( text, callback, toggle )
	if toggle == true then
		if m_callback then
			m_callback( toggle );
		end
		return
	end
	MsgBoxExDlgOpen()
	m_callback = callback
	SetText( m_uiText, text );
end