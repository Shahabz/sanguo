-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

-- 打开界面
function EquipWashDlgOpen()
	m_Dlg = eye.uiManager:Open( "EquipWashDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, "洗炼铺", 7, EquipWashDlgClose );
end

-- 隐藏界面
function EquipWashDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "EquipWashDlg" );
end

-- 删除界面
function EquipWashDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function EquipWashDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            EquipWashDlgClose();
        end
	end
end

-- 载入时调用
function EquipWashDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function EquipWashDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function EquipWashDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function EquipWashDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function EquipWashDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function EquipWashDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function EquipWashDlgShow()
	EquipWashDlgOpen();
end

