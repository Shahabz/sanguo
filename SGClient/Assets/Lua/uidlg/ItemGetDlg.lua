-- 界面
local m_Dlg = nil;
local m_uiNormalPanel = nil; --UnityEngine.GameObject
local m_uiItem = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiColor = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject

-- 打开界面
function ItemGetDlgOpen()
	m_Dlg = eye.uiManager:Open( "ItemGetDlg" );
end

-- 隐藏界面
function ItemGetDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "ItemGetDlg" );
end

-- 删除界面
function ItemGetDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ItemGetDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ItemGetDlgClose();
        end
	end
end

-- 载入时调用
function ItemGetDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiNormalPanel = objs[0];
	m_uiItem = objs[1];
	m_uiShape = objs[2];
	m_uiColor = objs[3];
	m_uiName = objs[4];
end

-- 界面初始化时调用
function ItemGetDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function ItemGetDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ItemGetDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function ItemGetDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function ItemGetDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function ItemGetDlgShow( kind, num )
	ItemGetDlgOpen()
	m_uiItem:GetComponent( "UITweenRectPosition" ):ToInit()
	SetTrue( m_uiNormalPanel );
	SetImage( m_uiShape, ItemSprite(kind) )
	SetImage( m_uiColor, ItemColorSprite( item_getcolor(kind) ) )
	SetText( m_uiName, item_getname(kind).."x"..num );
	Invoke(function() 
		SetFalse( m_uiNormalPanel );
	end, 2)
	
	Invoke(function() 
		m_uiItem:GetComponent( "UITweenRectPosition" ):Play( true );
	end, 2.5)
	
	Invoke(function() 
		ItemGetDlgClose()	
	end, 3)
end
