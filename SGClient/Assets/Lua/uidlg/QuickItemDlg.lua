-- 界面
local m_Dlg = nil;

local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiTargetName = nil; --UnityEngine.GameObject
local m_uiTimer = nil; --UnityEngine.GameObject
local m_uiUIP_QuickItem1 = nil; --UnityEngine.GameObject
local m_uiUIP_QuickItem2 = nil; --UnityEngine.GameObject
local m_uiUIP_QuickItem3 = nil; --UnityEngine.GameObject
local m_uiUIP_QuickItem4 = nil; --UnityEngine.GameObject

-- 打开界面
function QuickItemDlgOpen()
	m_Dlg = eye.uiManager:Open( "QuickItemDlg" );
end

-- 隐藏界面
function QuickItemDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "QuickItemDlg" );
end

-- 删除界面
function QuickItemDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function QuickItemDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            QuickItemDlgClose();
        end
	end
end

-- 载入时调用
function QuickItemDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiShape = objs[0];
	m_uiName = objs[1];
	m_uiTargetName = objs[2];
	m_uiTimer = objs[3];
	m_uiUIP_QuickItem1 = objs[4];
	m_uiUIP_QuickItem2 = objs[5];
	m_uiUIP_QuickItem3 = objs[6];
	m_uiUIP_QuickItem4 = objs[7];
end

-- 界面初始化时调用
function QuickItemDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function QuickItemDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function QuickItemDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function QuickItemDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function QuickItemDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function QuickItemDlgShow()
	QuickItemDlgOpen()
	GetItem():GetCount( nItemKind )
end

