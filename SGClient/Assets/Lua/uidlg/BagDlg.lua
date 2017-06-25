-- 界面
local m_Dlg = nil;

-- 打开界面
function BagDlgOpen()
	m_Dlg = eye.uiManager:Open( "BagDlg" );
end

-- 隐藏界面
function BagDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "BagDlg" );
end

-- 删除界面
function BagDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BagDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            BagDlgClose();
        end
	end
end

-- 载入时调用
function BagDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function BagDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function BagDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BagDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function BagDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function BagDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
