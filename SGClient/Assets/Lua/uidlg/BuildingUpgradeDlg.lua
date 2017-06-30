-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiLevel = nil; --UnityEngine.GameObject
local m_uiDesc1 = nil; --UnityEngine.GameObject
local m_uiDesc2 = nil; --UnityEngine.GameObject
local m_uiDesc3 = nil; --UnityEngine.GameObject
local m_uiSec = nil; --UnityEngine.GameObject
local m_uiCondition = nil; --UnityEngine.GameObject
local m_uiUIP_UpgradeCond = nil; --UnityEngine.GameObject

-- 打开界面
function BuildingUpgradeDlgOpen()
	m_Dlg = eye.uiManager:Open( "BuildingUpgradeDlg" );
end

-- 隐藏界面
function BuildingUpgradeDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "BuildingUpgradeDlg" );
end

-- 删除界面
function BuildingUpgradeDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BuildingUpgradeDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            BuildingUpgradeDlgClose();
        end
	end
end

-- 载入时调用
function BuildingUpgradeDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiShape = objs[0];
	m_uiName = objs[1];
	m_uiLevel = objs[2];
	m_uiDesc1 = objs[3];
	m_uiDesc2 = objs[4];
	m_uiDesc3 = objs[5];
	m_uiSec = objs[6];
	m_uiCondition = objs[7];
	m_uiUIP_UpgradeCond = objs[8];
end

-- 界面初始化时调用
function BuildingUpgradeDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function BuildingUpgradeDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BuildingUpgradeDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function BuildingUpgradeDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function BuildingUpgradeDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function BuildingUpgradeDlgShow( kind, offset )
	
end

