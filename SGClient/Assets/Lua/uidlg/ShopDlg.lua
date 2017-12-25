-- 界面
local m_Dlg = nil;

-- 打开界面
function ShopDlgOpen()
	m_Dlg = eye.uiManager:Open( "ShopDlg" );
end

-- 隐藏界面
function ShopDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "ShopDlg" );
end

-- 删除界面
function ShopDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ShopDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ShopDlgClose();
        end
	end
end

-- 载入时调用
function ShopDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function ShopDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function ShopDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ShopDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function ShopDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function ShopDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function ShopDlgShow()
	ShopDlgOpen()
end
