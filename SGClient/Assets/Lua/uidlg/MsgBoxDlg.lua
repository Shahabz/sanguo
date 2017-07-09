-- 界面
local m_Dlg = nil;
local m_uiText = nil; --UnityEngine.GameObject
local m_uiCancelText = nil; --UnityEngine.GameObject
local m_uiOkText = nil; --UnityEngine.GameObject
local m_callback = nil;
-- 打开界面
function MsgBoxDlgOpen()
	m_Dlg = eye.uiManager:Open( "MsgBoxDlg" );
end

-- 隐藏界面
function MsgBoxDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MsgBoxDlg" );
end

-- 删除界面
function MsgBoxDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MsgBoxDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MsgBoxDlgClose();
		elseif nControlID == 1 then
			if m_callback then
				m_callback();
			end
			 MsgBoxDlgClose();
        end
	end
end

-- 载入时调用
function MsgBoxDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiText = objs[0];
	m_uiCancelText = objs[1];
	m_uiOkText = objs[2];	
end

-- 界面初始化时调用
function MsgBoxDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MsgBoxDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MsgBoxDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MsgBoxDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MsgBoxDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MsgBox( text, callback )
	MsgBoxDlgOpen()
	m_callback = callback
	SetText( m_uiText, text );
end
