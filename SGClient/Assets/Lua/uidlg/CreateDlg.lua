-- 界面
local m_Dlg = nil;

-- 打开界面
function CreateDlgOpen()
	m_Dlg = eye.uiManager:Open( "CreateDlg" );
end

-- 隐藏界面
function CreateDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "CreateDlg" );
end

-- 删除界面
function CreateDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function CreateDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            CreateDlgClose();
		elseif nControlID == 1 then
			print(os.time())
        end
	end
end

-- 载入时调用
function CreateDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function CreateDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function CreateDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function CreateDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function CreateDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function CreateDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
