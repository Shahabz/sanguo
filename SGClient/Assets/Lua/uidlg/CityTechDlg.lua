-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiUIP_Tech = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiTeching = nil; --UnityEngine.GameObject

-- 打开界面
function CityTechDlgOpen()
	m_Dlg = eye.uiManager:Open( "CityTechDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, "科技", 5, CityTechDlgClose );
end

-- 隐藏界面
function CityTechDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	eye.uiManager:Close( "CityTechDlg" );
end

-- 删除界面
function CityTechDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function CityTechDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            CityTechDlgClose();
        end
	end
end

-- 载入时调用
function CityTechDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiUIP_Tech = objs[0];
	m_uiContent = objs[1];
	m_uiTeching = objs[2];
end

-- 界面初始化时调用
function CityTechDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function CityTechDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function CityTechDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function CityTechDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function CityTechDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function CityTechDlgOnShow()
	CityTechDlgOpen()
end
