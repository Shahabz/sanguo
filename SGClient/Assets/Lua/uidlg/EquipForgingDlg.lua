-- 界面
local m_Dlg = nil;

-- 打开界面
function EquipForgingDlgOpen()
	m_Dlg = eye.uiManager:Open( "EquipForgingDlg" );
end

-- 隐藏界面
function EquipForgingDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "EquipForgingDlg" );
end

-- 删除界面
function EquipForgingDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function EquipForgingDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            EquipForgingDlgClose();
        end
	end
end

-- 载入时调用
function EquipForgingDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function EquipForgingDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function EquipForgingDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function EquipForgingDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function EquipForgingDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function EquipForgingDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
