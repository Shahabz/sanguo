-- 界面
local m_Dlg = nil;

-- 打开界面
function HeroDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroDlg" );
end

-- 隐藏界面
function HeroDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "HeroDlg" );
end

-- 删除界面
function HeroDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroDlgClose();
        end
	end
end

-- 载入时调用
function HeroDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function HeroDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HeroDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
