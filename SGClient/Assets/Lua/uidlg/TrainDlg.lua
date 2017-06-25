-- 界面
local m_Dlg = nil;

-- 打开界面
function TrainDlgOpen()
	m_Dlg = eye.uiManager:Open( "TrainDlg" );
end

-- 隐藏界面
function TrainDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "TrainDlg" );
end

-- 删除界面
function TrainDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function TrainDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            TrainDlgClose();
        end
	end
end

-- 载入时调用
function TrainDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function TrainDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function TrainDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function TrainDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function TrainDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function TrainDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
