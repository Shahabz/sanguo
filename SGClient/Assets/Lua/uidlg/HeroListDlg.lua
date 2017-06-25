-- 界面
local m_Dlg = nil;

-- 打开界面
function HeroListDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroListDlg" );
end

-- 隐藏界面
function HeroListDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "HeroListDlg" );
end

-- 删除界面
function HeroListDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroListDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroListDlgClose();
        end
	end
end

-- 载入时调用
function HeroListDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function HeroListDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroListDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HeroListDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroListDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroListDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
