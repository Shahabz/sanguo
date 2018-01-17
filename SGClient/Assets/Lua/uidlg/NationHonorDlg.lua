-- 界面
local m_Dlg = nil;

-- 打开界面
function NationHonorDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationHonorDlg" );
end

-- 隐藏界面
function NationHonorDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "NationHonorDlg" );
end

-- 删除界面
function NationHonorDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationHonorDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationHonorDlgClose();
        end
	end
end

-- 载入时调用
function NationHonorDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function NationHonorDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NationHonorDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationHonorDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationHonorDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationHonorDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationHonorDlgShow()
	NationHonorDlgOpen()
end
