-- 界面
local m_Dlg = nil;
local m_uiAlertText = nil; --UnityEngine.GameObject
local m_uiLevyButton = nil; --UnityEngine.GameObject
local m_uiBuyButton = nil; --UnityEngine.GameObject
local m_uiPayButton = nil; --UnityEngine.GameObject

-- 打开界面
function JumpDlgOpen()
	m_Dlg = eye.uiManager:Open( "JumpDlg" );
end

-- 隐藏界面
function JumpDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "JumpDlg" );
end

-- 删除界面
function JumpDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function JumpDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            JumpDlgClose();
		elseif nControlID == 1 then
			JumpDlgClose();
			LevyDlgShow();
		elseif nControlID == 2 then
			JumpDlgClose();
		elseif nControlID == 3 then
			JumpDlgClose();
        end
	end
end

-- 载入时调用
function JumpDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiAlertText = objs[0];
	m_uiLevyButton = objs[1];
	m_uiBuyButton = objs[2];
	m_uiPayButton = objs[3];
end

-- 界面初始化时调用
function JumpDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function JumpDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function JumpDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function JumpDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function JumpDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function JumpDlgShow()
	JumpDlgOpen()
end

function JumpToken()
	JumpDlgShow()
	SetFalse( m_uiLevyButton );
	SetFalse( m_uiBuyButton );
	SetTrue( m_uiPayButton );
	SetText( m_uiAlertText, F(764, T(125)) )
end

function JumpRes( res )
	JumpDlgShow()
	SetTrue( m_uiBuyButton );
	SetFalse( m_uiPayButton );
	if GetPlayer().m_levynum <= 0 then
		SetFalse( m_uiLevyButton );
	else
		SetTrue( m_uiLevyButton );
	end
	SetText( m_uiAlertText, F(764, T(120+res)) )
end
