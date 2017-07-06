-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiGuardNum = nil; --UnityEngine.GameObject
local m_uiUIP_Guard = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiTimerText = nil; --UnityEngine.GameObject
local m_uiDescText = nil; --UnityEngine.GameObject
local m_uiCDButton = nil; --UnityEngine.GameObject
local m_uiCallButton = nil; --UnityEngine.GameObject
local m_level = 1;

-- 打开界面
function CityGuardDlgOpen()
	m_Dlg = eye.uiManager:Open( "CityGuardDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, "城墙"..m_level.."级", 4, CityGuardDlgClose );
end

-- 隐藏界面
function CityGuardDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	eye.uiManager:Close( "CityGuardDlg" );
end

-- 删除界面
function CityGuardDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function CityGuardDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            CityGuardDlgClose();
        end
	end
end

-- 载入时调用
function CityGuardDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiGuardNum = objs[0];
	m_uiUIP_Guard = objs[1];
	m_uiContent = objs[2];
	m_uiTimerText = objs[3];
	m_uiDescText = objs[4];
	m_uiCDButton = objs[5];
	m_uiCallButton = objs[6];
end

-- 界面初始化时调用
function CityGuardDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function CityGuardDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function CityGuardDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function CityGuardDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function CityGuardDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function CityGuardDlgShow()
	m_level = 1;
	CityGuardDlgOpen();
	
end
