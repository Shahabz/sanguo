-- 界面
local m_Dlg = nil;
--local m_DialogFrameMod = nil;

-- 打开界面
function PolicyDlgOpen()
	m_Dlg = eye.uiManager:Open( "PolicyDlg" );
	--m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(0), nil, PolicyDlgClose );
end

-- 隐藏界面
function PolicyDlgClose()
	if m_Dlg == nil then
		return;
	end
	--DialogFrameModClose( m_DialogFrameMod );
	--m_DialogFrameMod = nil;
	eye.uiManager:Close( "PolicyDlg" );
end

-- 删除界面
function PolicyDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function PolicyDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            PolicyDlgClose();
        end
	end
end

-- 载入时调用
function PolicyDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function PolicyDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function PolicyDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function PolicyDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function PolicyDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function PolicyDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function PolicyDlgShow()
	PolicyDlgOpen()
end
