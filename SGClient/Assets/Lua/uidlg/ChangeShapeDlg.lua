-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject

local m_SelectShape = -1;

-- 打开界面
function ChangeShapeDlgOpen()
	m_Dlg = eye.uiManager:Open( "ChangeShapeDlg" );
end

-- 隐藏界面
function ChangeShapeDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "ChangeShapeDlg" );
end

-- 删除界面
function ChangeShapeDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ChangeShapeDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ChangeShapeDlgClose();
        elseif nControlID >= 0 and nControlID <= 40 then
        		ChangeShapeDlgSelect( nControlID );
        end
	end
end

-- 载入时调用
function ChangeShapeDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiShape = objs[0];	
end

-- 界面初始化时调用
function ChangeShapeDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function ChangeShapeDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ChangeShapeDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function ChangeShapeDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function ChangeShapeDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function ChangeShapeDlgShow()
	ChangeShapeDlgOpen()
	ChangeShapeDlgSelect( GetPlayer().m_shape );
end

function ChangeShapeDlgSelect( shape )
	m_SelectShape = shape;
	m_uiShape:GetComponent( "Image" ).sprite = PlayerFaceSprite( shape );
end
