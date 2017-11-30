-- 界面
local m_Dlg = nil;
local m_uiItemObj = nil; --UnityEngine.GameObject
local m_uiBuildingObj = nil; --UnityEngine.GameObject
local m_uiGetBtn = nil; --UnityEngine.GameObject
local m_uiRebuildBtn = nil; --UnityEngine.GameObject
local m_kind = 0;
local m_offset = -1;
-- 打开界面
function ResDrawingDlgOpen()
	m_Dlg = eye.uiManager:Open( "ResDrawingDlg" );
end

-- 隐藏界面
function ResDrawingDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "ResDrawingDlg" );
end

-- 删除界面
function ResDrawingDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ResDrawingDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ResDrawingDlgClose();
			
		-- 获取
		elseif nControlID == 1 then
			ResDrawingDlgGet();
			
		-- 重新规划
		elseif nControlID == 2 then
			ResDrawingDlgBuild();
			
        end
	end
end

-- 载入时调用
function ResDrawingDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiItemObj = objs[0];
	m_uiBuildingObj = objs[1];
	m_uiGetBtn = objs[2];
	m_uiRebuildBtn = objs[3];

end

-- 界面初始化时调用
function ResDrawingDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function ResDrawingDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ResDrawingDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function ResDrawingDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function ResDrawingDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function ResDrawingDlgShow( kind, offset )
	if kind < 21 or kind > 24 then
		return
	end
	if offset < 0 or offset >= 64 then
		return
	end
	m_kind = kind;
	m_offset = offset;
	ResDrawingDlgOpen()
	local config = nil
	if kind == 21 then
		config = g_building_res_unlock[kind][offset];
	elseif kind == 22 then
		config = g_building_res_unlock[kind][offset-16];
	elseif kind == 23 then
		config = g_building_res_unlock[kind][offset-32];
	elseif kind == 24 then
		config = g_building_res_unlock[kind][offset-48];
	end
	if config == nil then
		return
	end

	local itemcolor = item_getcolor( config.itemkind );
	local itemcount = GetItem():GetCount( config.itemkind )
	SetImage( m_uiItemObj.transform:Find("Shape"), ItemSprite( config.itemkind ) )
	SetImage( m_uiItemObj.transform:Find("Color"), ItemColorSprite( itemcolor ) )
	SetText( m_uiItemObj.transform:Find("Name"), item_getname( config.itemkind ) )
	SetText( m_uiItemObj.transform:Find("Num"), itemcount.."/"..config.itemnum )
	
	if itemcount < config.itemnum then
		SetTrue( m_uiGetBtn )
		SetFalse( m_uiRebuildBtn )
	else
		SetFalse( m_uiGetBtn )
		SetTrue( m_uiRebuildBtn )
	end
	
	SetImage( m_uiBuildingObj.transform:Find("Shape"), BuildingSprite( kind ) )
	SetText( m_uiBuildingObj.transform:Find("Name"), BuildingName( kind ) )
end

-- 获取
function ResDrawingDlgGet()
	StoryDlgShow()
end

-- 重建
function ResDrawingDlgBuild()
	ResDrawingDlgClose()
	system_askinfo( ASKINFO_BUILDING, "", 6, m_kind, m_offset );
end
