-- 界面
local m_Dlg = nil;
local m_uiTalk = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiDesc = nil; --UnityEngine.GameObject
local m_uiBtnText = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_callback = nil;
-- 打开界面
function NpcAlertDlgOpen()
	m_Dlg = eye.uiManager:Open( "NpcAlertDlg" );
end

-- 隐藏界面
function NpcAlertDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "NpcAlertDlg" );
end

-- 删除界面
function NpcAlertDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NpcAlertDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NpcAlertDlgClose();
		elseif nControlID == 1 then
			if m_callback ~= nil then
				m_callback()
			end
			NpcAlertDlgClose();
        end
	end
end

-- 载入时调用
function NpcAlertDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTalk = objs[0];
	m_uiName = objs[1];
	m_uiDesc = objs[2];
	m_uiBtnText = objs[3];
	m_uiShape = objs[4];
end

-- 界面初始化时调用
function NpcAlertDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NpcAlertDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NpcAlertDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NpcAlertDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NpcAlertDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NpcAlertDlgShow()
	NpcAlertDlgOpen()
end

function NpcAlert( talk, name, desc, btntext, callback )
	NpcAlertDlgShow()
	SetText( m_uiTalk, talk )
	SetText( m_uiName, name )
	SetText( m_uiDesc, desc )
	SetText( m_uiBtnText, btntext )
	m_callback = callback;
end
