local m_Mod;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ResDrawingModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
		ResDrawingModOpenDlg( gameObject )
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	end
end

-- 载入时调用
function ResDrawingModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
end

-- 界面初始化时调用
function ResDrawingModOnStart( gameObject )
	
end

-- 界面显示时调用
function ResDrawingModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ResDrawingModOnDisable( gameObject )
	
end

-- 界面删除时调用
function ResDrawingModOnDestroy( gameObject )
	
end

-- 每帧调用
function ResDrawingModOnLogic( gameObject )
	
end

function ResDrawingModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
function ResDrawingModOpenDlg( gameObject )
	local shareData = gameObject.transform.parent.transform:GetComponent("CityBuilding");
	ResDrawingDlgShow( shareData.kind, shareData.offset );
end