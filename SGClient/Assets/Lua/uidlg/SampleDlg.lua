-- 界面
local m_Dlg = nil;

-- 打开界面
function SampleDlgOpen()
	m_Dlg = eye.uiManager:Open( "SampleDlg" );
end

-- 隐藏界面
function SampleDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "SampleDlg" );
end

-- 删除界面
function SampleDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function SampleDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            SampleDlgClose();
        end
	end
end

-- 载入时调用
function SampleDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function SampleDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function SampleDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function SampleDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function SampleDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function SampleDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
