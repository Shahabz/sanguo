-- 界面
local m_Dlg = nil;

-- 打开界面
function BuyWorkerDlgOpen()
	m_Dlg = eye.uiManager:Open( "BuyWorkerDlg" );
end

-- 隐藏界面
function BuyWorkerDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "BuyWorkerDlg" );
end

-- 删除界面
function BuyWorkerDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BuyWorkerDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            BuyWorkerDlgClose();
		elseif nControlID == 1 then
			MsgBox( T(617), function()
			end )
			BuyWorkerDlgClose();
		elseif nControlID == 2 then
			MsgBox( T(618), function()
			end )
			BuyWorkerDlgClose();
        end
	end
end

-- 载入时调用
function BuyWorkerDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function BuyWorkerDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function BuyWorkerDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BuyWorkerDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function BuyWorkerDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function BuyWorkerDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function BuyWorkerDlgShow()
	BuyWorkerDlgOpen()
end

