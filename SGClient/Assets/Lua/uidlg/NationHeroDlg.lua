-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiNowText = nil; --UnityEngine.GameObject
local m_uiScrollView = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject

-- 打开界面
function NationHeroDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationHeroDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(1539), HELP_NationDlg, NationHeroDlgClose );
end

-- 隐藏界面
function NationHeroDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "NationHeroDlg" );
end

-- 删除界面
function NationHeroDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationHeroDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationHeroDlgClose();
        end
	end
end

-- 载入时调用
function NationHeroDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiNowText = objs[0];
	m_uiScrollView = objs[1];
	m_uiContent = objs[2];
end

-- 界面初始化时调用
function NationHeroDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NationHeroDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationHeroDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationHeroDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationHeroDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationHeroDlgShow()
	NationHeroDlgOpen()
	
end
