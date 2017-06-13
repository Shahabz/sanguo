-- 界面
local m_Dlg = nil;

-- 打开界面
function #NAME#Open()
	m_Dlg = eye.uiManager:Open( "#NAME#" );
end

-- 隐藏界面
function #NAME#Close()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "#NAME#" );
end

-- 删除界面
function #NAME#Destroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function #NAME#OnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            #NAME#Close();
        end
	end
end

-- 载入时调用
function #NAME#OnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function #NAME#OnStart( gameObject )
	
end

-- 界面显示时调用
function #NAME#OnEnable( gameObject )
	
end

-- 界面隐藏时调用
function #NAME#OnDisable( gameObject )
	
end

-- 界面删除时调用
function #NAME#OnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function #NAME#OnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
